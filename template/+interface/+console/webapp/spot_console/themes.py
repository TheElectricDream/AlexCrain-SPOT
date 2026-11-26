"""The page's colour themes: another look, without a second stylesheet.

A theme is an entry in static/themes/themes.json (its name, a ramp for
the greys and an accent colour) and a stylesheet of its own rules,
static/themes/<theme>.css. The page asks for /theme/<theme>.css and
gets its usual stylesheets (app.css and docs.css) with every colour
re-mapped for the theme, followed by the theme's rules. The page's own
look is not touched: without a theme the page never asks.

Because the theme is made from the stylesheets as they are now, a rule
added to app.css is themed without anyone copying it across.

Part of the SPOT Console (webapp/app.py starts it).
"""

import colorsys
import json
import re
import threading

from .common import STATIC_DIR

THEMES_DIR = STATIC_DIR / "themes"

# The stylesheets a theme re-colours, in the order the page loads them.
BASE_SHEETS = ("app.css", "docs.css")

NAME_RE = re.compile(r"^[a-z0-9-]{1,40}$")

# A colour written as #rgb, #rrggbb, #rrggbbaa, rgb() or rgba().
HEX_RE = re.compile(r"#([0-9a-fA-F]{8}|[0-9a-fA-F]{6}|[0-9a-fA-F]{3})(?![0-9a-zA-Z_-])")
RGB_RE = re.compile(r"rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*(?:,\s*([\d.]+)\s*)?\)")

# A declaration (property: value), so that selectors are left alone.
DECLARATION_RE = re.compile(r"([a-zA-Z-]+\s*:\s*)([^;{}]+)")

NEUTRAL_SATURATION = 0.3    # below this a colour is one of the greys
ACCENT_HUES = (200, 236)    # [deg] the page's accent blues

_cache = {}
_lock = threading.Lock()


def hex_to_rgb(text):
    """Return the (r, g, b) of a colour written #rgb or #rrggbb."""
    digits = text.lstrip("#")
    if len(digits) == 3:
        digits = "".join(c * 2 for c in digits)
    return tuple(int(digits[i:i + 2], 16) for i in (0, 2, 4))


def to_css(rgb, alpha):
    """Return a colour as CSS: #rrggbb, or rgba() when see-through.

    Args:
        rgb: Red, green and blue, 0 to 255 (rounded and clamped here).
        alpha: The opacity, 0 to 1; None for an opaque colour.
    """
    r, g, b = (max(0, min(255, int(round(v)))) for v in rgb)
    if alpha is None or alpha >= 0.999:
        return "#%02x%02x%02x" % (r, g, b)
    return "rgba(%d,%d,%d,%s)" % (r, g, b, ("%.3f" % alpha).rstrip("0").rstrip("."))


def ramp_color(stops, lightness):
    """Return the ramp's colour at a lightness, between its stops.

    Args:
        stops: The ramp, a list of [lightness, "#rrggbb"] pairs.
        lightness: The lightness to look up, 0 to 1.

    Returns:
        The (r, g, b), 0 to 255, not rounded.
    """
    stops = sorted((float(x), hex_to_rgb(c)) for x, c in stops)
    if lightness <= stops[0][0]:
        return stops[0][1]
    for (x0, c0), (x1, c1) in zip(stops, stops[1:]):
        if lightness <= x1:
            part = (lightness - x0) / (x1 - x0) if x1 > x0 else 0.0
            return (c0[0] + (c1[0] - c0[0]) * part, c0[1] + (c1[1] - c0[1]) * part,
                    c0[2] + (c1[2] - c0[2]) * part)
    return stops[-1][1]


def map_color(rgb, alpha, theme):
    """Return one of the page's colours as the theme shows it.

    The greys (and the blue-greys) go onto the theme's ramp by their
    lightness, the accent blues onto the theme's accent; every other
    hue (the greens, ambers and reds that mean something, the
    platforms' colours) and black shadows stay as they are.

    Args:
        rgb: The colour, 0 to 255.
        alpha: Its opacity, 0 to 1; None when opaque.
        theme: The theme's entry in themes.json.

    Returns:
        The colour as CSS text.
    """
    hue, lightness, saturation = colorsys.rgb_to_hls(*(v / 255.0 for v in rgb))

    # The greys; a see-through black is a shadow, and stays one.
    if saturation < NEUTRAL_SATURATION:
        if rgb == (0, 0, 0) and alpha is not None and alpha < 1:
            return to_css(rgb, alpha)
        return to_css(ramp_color(theme["ramp"], lightness), alpha)

    # The accent blues: the theme's accent, darker and lighter as they
    # are.
    if ACCENT_HUES[0] <= hue * 360 <= ACCENT_HUES[1]:
        a_hue, a_light, a_sat = colorsys.rgb_to_hls(
            *(v / 255.0 for v in hex_to_rgb(theme["accent"])))
        if lightness < 0.32:
            a_sat *= 0.7
        else:
            lightness = a_light + (lightness - 0.65) * 0.8
            a_sat *= min(1.0, saturation / 0.8 + 0.2)
        red, green, blue = colorsys.hls_to_rgb(
            a_hue, max(0.0, min(1.0, lightness)), max(0.0, min(1.0, a_sat)))
        return to_css((red * 255, green * 255, blue * 255), alpha)

    return to_css(rgb, alpha)


def recolor(css, theme):
    """Return a stylesheet with every colour in it mapped for a theme.

    Only the values of declarations are changed, so a selector such as
    #add is not mistaken for a colour.

    Args:
        css: The stylesheet's text.
        theme: The theme's entry in themes.json.
    """
    def from_hex(match):
        """Return a #rgb, #rrggbb or #rrggbbaa colour, mapped."""
        digits = match.group(1)
        alpha = int(digits[6:], 16) / 255.0 if len(digits) == 8 else None
        return map_color(hex_to_rgb(digits[:6] if len(digits) == 8 else digits), alpha, theme)

    def from_rgb(match):
        """Return an rgb() or rgba() colour, mapped."""
        alpha = float(match.group(4)) if match.group(4) else None
        return map_color(tuple(int(match.group(i)) for i in (1, 2, 3)), alpha, theme)

    def value(match):
        """Return a declaration with the colours of its value mapped."""
        return match.group(1) + RGB_RE.sub(from_rgb, HEX_RE.sub(from_hex, match.group(2)))

    return DECLARATION_RE.sub(value, css)


def catalog():
    """Return the themes: {name: {"name": title, "ramp", "accent"}}.

    An unreadable themes.json gives none, so the page keeps its own
    look.
    """
    try:
        found = json.loads((THEMES_DIR / "themes.json").read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}
    return found if isinstance(found, dict) else {}


def stylesheet(name):
    """Return a theme's whole stylesheet, or None if it doesn't exist.

    It is built again only when one of its files has changed.

    Args:
        name: The theme's name, as in themes.json.

    Returns:
        The stylesheet (UTF-8 bytes), or None.
    """
    if not NAME_RE.match(name or ""):
        return None
    sources = ([STATIC_DIR / sheet for sheet in BASE_SHEETS]
               + [THEMES_DIR / "themes.json", THEMES_DIR / (name + ".css")])
    try:
        stamp = tuple(p.stat().st_mtime_ns for p in sources)
    except OSError:
        return None

    with _lock:
        if _cache.get(name, (None, None))[0] == stamp:
            return _cache[name][1]
        theme = catalog().get(name)
        if not isinstance(theme, dict):
            return None
        try:
            parts = [recolor(p.read_text(encoding="utf-8"), theme) for p in sources[:2]]
            parts.append(sources[3].read_text(encoding="utf-8"))
        except (OSError, KeyError, TypeError, ValueError):
            return None
        body = "\n".join(parts).encode("utf-8")
        _cache[name] = (stamp, body)
        return body
