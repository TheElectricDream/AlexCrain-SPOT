"""Remap 'Line(s) a–b' references on a wiki page after its source changed: python3 remap_lines.py PAGE.md SOURCE [SOURCE2...]
The old text comes from the page's own listing; prints the references it changed."""
import difflib, re, sys
from pathlib import Path
page = Path(sys.argv[1]); root = Path(__file__).resolve().parent.parent
text = page.read_text()
head, sep, tail = text.partition('## The code')
for src in sys.argv[2:]:
    m = re.search(r'<summary><b>%s</b>.*?```\w*\n(.*?)\n```' % re.escape(src), tail, re.S)
    old = [re.sub(r'^ *\d+  ', '', l, count=1) for l in m.group(1).split('\n')]
    new = (root / src).read_text().rstrip('\n').split('\n')
    mp = {}
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, old, new, autojunk=False).get_opcodes():
        for k in range(i1, i2):
            mp[k + 1] = (j1 + (k - i1) + 1) if tag == 'equal' else None
    def near(n, end):
        if mp.get(n): return mp[n]
        ks = range(n, 0, -1) if end else range(n, len(old) + 1)
        for k in ks:
            if mp.get(k): return mp[k]
        return n
    # only remap references in this source's section: if one source, the whole head
    def rep(mt):
        a = int(mt.group(2)); b = mt.group(4)
        na = near(a, False)
        if b:
            nb = near(int(b), True)
            return '%s %d%s%d' % (mt.group(1), na, mt.group(3), nb)
        return '%s %d' % (mt.group(1), na)
    new_head = re.sub(r'(Lines?) (\d+)(?:(–|-)(\d+))?', rep, head)
    changed = [(a, b) for a, b in zip(re.findall(r'Lines? \d+(?:[–-]\d+)?', head), re.findall(r'Lines? \d+(?:[–-]\d+)?', new_head)) if a != b]
    print(page.name, src, len(changed), 'changed:', changed[:60])
    head = new_head
page.write_text(head + sep + tail)
