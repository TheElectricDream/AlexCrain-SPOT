"""Shared bits for the mock themes."""
import urllib.parse


def face(family, file, weight="400", style="normal", extra=""):
    return ("@font-face { font-family: '%s'; src: url('/mockfonts/%s'); font-weight: %s; "
            "font-style: %s; font-display: block; %s }\n" % (family, file, weight, style, extra))


def svg(body, viewbox="0 0 24 24"):
    s = "<svg xmlns='http://www.w3.org/2000/svg' viewBox='%s'>%s</svg>" % (viewbox, body)
    return "url(\"data:image/svg+xml," + urllib.parse.quote(s, safe="/:=' ") + "\")"


# The SRL emblem's four-pointed star (concave sides), in a 24 x 24 box.
STAR = ("M12 0.6 C12.9 7.6 16.4 11.1 23.4 12 C16.4 12.9 12.9 16.4 12 23.4 "
        "C11.1 16.4 7.6 12.9 0.6 12 C7.6 11.1 11.1 7.6 12 0.6Z")


def star(fill="#e91c24", stroke="#ffffff", sw=1.4):
    return svg("<path d='%s' fill='%s' stroke='%s' stroke-width='%s' stroke-linejoin='round'/>"
               % (STAR, fill, stroke, sw), "-1 -1 26 26")


def viewer_js(bg, floor, grid1, grid2):
    return [("this.scene.background = new THREE.Color(0x0e1014);",
             "this.scene.background = new THREE.Color(0x%s);" % bg),
            ("this.scene.fog = new THREE.Fog(0x0e1014, 9, 24);",
             "this.scene.fog = new THREE.Fog(0x%s, 9, 24);" % bg),
            ("new THREE.MeshStandardMaterial({ color: 0x111317, roughness: 0.95 })",
             "new THREE.MeshStandardMaterial({ color: 0x%s, roughness: 0.95 })" % floor),
            ("new THREE.GridHelper(40, 80, 0x20242c, 0x1a1d23)",
             "new THREE.GridHelper(40, 80, 0x%s, 0x%s)" % (grid1, grid2))]


def ink_js(text, muted, grid, axis, surface):
    return [("text: '#e6e9ef', muted: '#8b93a3', grid: '#232833', axis: '#3a4150', surface: '#0f1217',",
             "text: '%s', muted: '%s', grid: '%s', axis: '%s', surface: '%s'," % (text, muted, grid, axis, surface))]
