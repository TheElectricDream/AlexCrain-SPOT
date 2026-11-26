"""Renders the real SPOT Console with a theme applied on the fly (nothing in the repo changes).

The page's own app.css/docs.css are fetched, their colours re-mapped through the theme's
neutral ramp and accent, and the theme's extra CSS appended; a few colours that live in the
scripts (3D scene, plots) are swapped the same way. Usage: shoot.py <theme> <page,page,...>
"""
import colorsys
import importlib.util
import json
import pathlib
import re
import sys
import urllib.request

from playwright.sync_api import sync_playwright

S = pathlib.Path(__file__).parent
EXE = str(pathlib.Path.home() / ".cache/ms-playwright/chromium-1228/chrome-linux64/chrome")
TOKEN = json.load(open(S / "proj/.spot_cache/console.json"))["token"]
BASE = "http://127.0.0.1:8791"
URL = BASE + "/?capture&token=" + TOKEN

HEX = re.compile(r"#([0-9a-fA-F]{8}|[0-9a-fA-F]{6}|[0-9a-fA-F]{3})(?![0-9a-zA-Z_-])")
RGB = re.compile(r"rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*(?:,\s*([\d.]+)\s*)?\)")
DECL = re.compile(r"([a-zA-Z-]+\s*:\s*)([^;{}]+)")


def hex2rgb(h):
    h = h.lstrip("#")
    if len(h) == 3:
        h = "".join(c * 2 for c in h)
    return tuple(int(h[i:i + 2], 16) for i in (0, 2, 4))


def out(rgb, a):
    r, g, b = (max(0, min(255, round(v))) for v in rgb)
    if a is None or a >= 0.999:
        return "#%02x%02x%02x" % (r, g, b)
    return "rgba(%d,%d,%d,%s)" % (r, g, b, ("%.3f" % a).rstrip("0").rstrip("."))


def ramp(stops, x):
    """Piecewise-linear colour ramp: stops = [(lightness_in, '#hex'), ...]."""
    stops = sorted(stops)
    if x <= stops[0][0]:
        return hex2rgb(stops[0][1])
    for (x0, c0), (x1, c1) in zip(stops, stops[1:]):
        if x <= x1:
            t = (x - x0) / (x1 - x0) if x1 > x0 else 0
            a, b = hex2rgb(c0), hex2rgb(c1)
            return tuple(a[i] + (b[i] - a[i]) * t for i in range(3))
    return hex2rgb(stops[-1][1])


def hls(rgb):
    return colorsys.rgb_to_hls(*(v / 255 for v in rgb))


def from_hls(h, l, s):
    return tuple(v * 255 for v in colorsys.hls_to_rgb(h, max(0, min(1, l)), max(0, min(1, s))))


def map_color(rgb, a, T):
    h, l, s = hls(rgb)
    light = T.get("light", False)
    if s < 0.3:                                         # the blue-grey neutrals
        if rgb == (0, 0, 0) and a is not None and a < 1:    # shadows stay shadows
            return out(hex2rgb(T.get("shadow", "#000000")), a * T.get("shadow_k", 1.0))
        return out(ramp(T["ramp"], l), a)
    hue = h * 360
    if 200 <= hue <= 236:                               # the accent blue family
        ah, al, as_ = hls(hex2rgb(T["accent"]))
        if light:
            if l < 0.32:
                return out(from_hls(ah, 0.93 - l * 0.3, as_ * 0.7), a)
            if l > 0.72:
                return out(from_hls(ah, al * 0.8, as_), a)
            return out(from_hls(ah, al, as_), a)
        if l < 0.32:
            return out(from_hls(ah, l * T.get("accent_dark_k", 1.0), as_ * 0.7), a)
        return out(from_hls(ah, al + (l - 0.65) * 0.8, as_ * min(1, s / 0.8 + 0.2)), a)
    if light:                                           # other hues keep their meaning
        if a is not None and a < 1:
            return out(from_hls(h, min(l, 0.45), s), a)
        if l < 0.2:
            return out(from_hls(h, 0.94 - l * 0.4, s * 0.6), a)
        if l > 0.6:
            return out(from_hls(h, 0.36, min(s, 0.85)), a)
    return out(rgb, a)


def remap(css, T):
    def colors(text):
        text = HEX.sub(lambda m: (lambda v: map_color(
            hex2rgb(v[:6] if len(v) == 8 else v), int(v[6:], 16) / 255 if len(v) == 8 else None,
            T))(m.group(1)), text)
        return RGB.sub(lambda m: map_color(tuple(int(m.group(i)) for i in (1, 2, 3)),
                                           float(m.group(4)) if m.group(4) else None, T), text)
    return DECL.sub(lambda m: m.group(1) + colors(m.group(2)), css)


def load_theme(name):
    sys.path.insert(0, str(S / "themes"))
    spec = importlib.util.spec_from_file_location("theme_" + name, S / "themes" / (name + ".py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod.THEME


def fetch(path):
    with urllib.request.urlopen(BASE + path) as r:
        return r.read().decode("utf-8")


def run(name, pages, size=(1600, 960), suffix=""):
    T = load_theme(name) if name != "current" else None
    with sync_playwright() as p:
        b = p.chromium.launch(executable_path=EXE, args=[
            "--use-gl=angle", "--use-angle=swiftshader", "--enable-unsafe-swiftshader",
            "--ignore-gpu-blocklist"])
        pg = b.new_page(viewport={"width": size[0], "height": size[1]})
        errs = []
        pg.on("console", lambda m: errs.append(m.text) if m.type == "error" else None)
        pg.on("pageerror", lambda e: errs.append(str(e)))
        if T:
            def css_route(route):
                path = "/" + route.request.url.split("/", 3)[3].split("?")[0]
                body = remap(fetch(path), T)
                if path.endswith("app.css"):
                    body += "\n" + T.get("fontface", "")
                if path.endswith("docs.css"):       # last sheet: the theme's own rules win
                    body += "\n" + T["css"]
                route.fulfill(status=200, content_type="text/css", body=body)

            def js_route(route):
                path = "/" + route.request.url.split("/", 3)[3].split("?")[0]
                body = fetch(path)
                for a, c in T.get("js", {}).get(path.rsplit("/", 1)[1], []):
                    if a not in body:
                        print("  (js swap not found in %s: %s)" % (path, a[:40]))
                    body = body.replace(a, c)
                route.fulfill(status=200, content_type="text/javascript", body=body)

            def font_route(route):
                f = S / "fonts" / route.request.url.rsplit("/", 1)[1]
                route.fulfill(status=200, content_type="font/ttf", body=f.read_bytes())

            pg.route("**/static/app.css", css_route)
            pg.route("**/static/docs.css", css_route)
            for script in T.get("js", {}):
                pg.route("**/static/" + script, js_route)
            pg.route("**/mockfonts/*", font_route)
        pg.goto(URL)
        pg.wait_for_timeout(3000)
        for spec in pages:
            page, _, action = spec.partition(":")
            btn = pg.locator('.page-tabs button[data-page="%s"]' % page)
            btn.click()
            pg.wait_for_timeout(1500)
            if T and T.get("dom"):
                pg.evaluate(T["dom"])
            if page == "simulation":            # hold the replay at a fixed moment
                pg.evaluate("""() => { const r = document.querySelector('.pb-track input');
                    if (!r) return; const b = document.querySelector('.pb-play');
                    if (b && /pause|❚|⏸/i.test(b.title + b.textContent + b.innerHTML)) b.click(); }""")
                pg.wait_for_timeout(200)
                pg.evaluate("""() => { const r = document.querySelector('.pb-track input');
                    if (!r) return; r.value = String(Number(r.max) * 0.335);
                    r.dispatchEvent(new Event('input', { bubbles: true })); }""")
            for step in filter(None, action.split("+")):
                ACTIONS[step](pg)
            pg.wait_for_timeout(900)
            shot = S / "shots" / ("%s-%s%s.png" % (name, spec.replace(":", "_").replace("+", "_"), suffix))
            pg.screenshot(path=str(shot))
            print("wrote", shot.name)
        if errs:
            print("page errors:", errs[:4])
        b.close()


def _sim_tab(label):
    return lambda pg: pg.locator(".sim-tabs button", has_text=label).first.click()


def _results_plot(pg):
    for _ in range(12):                     # close what earlier runs left loaded
        old = pg.locator(".res-ds [data-unload]")
        if not old.count():
            break
        old.first.click()
        pg.wait_for_timeout(700)
    pg.locator("#resLoadSim").click()
    pg.wait_for_timeout(3000)
    for d in pg.locator(".res-signals details").all()[:3]:
        if d.get_attribute("open") is None:
            d.locator("summary").first.click()
    pg.wait_for_timeout(300)
    names = pg.locator(".res-sig-name")
    for i in range(names.count()):
        if names.nth(i).inner_text() in ("Px_m", "Py_m"):
            names.nth(i).click()
            pg.wait_for_timeout(250)


ACTIONS = {
    "platforms": _sim_tab("Platforms"), "control": _sim_tab("Control"),
    "calibration": _sim_tab("Calibration"), "workspace": _sim_tab("Workspace"),
    "plot": _results_plot,
    "top": lambda pg: pg.locator(".viewer-toolbar button", has_text="Top").first.click(),
}

if __name__ == "__main__":
    run(sys.argv[1], sys.argv[2].split(","), suffix=sys.argv[3] if len(sys.argv) > 3 else "")
