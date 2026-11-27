#!/usr/bin/env python3
"""Check (or snapshot) the fixed mechanical placement of TeensyRPN42.

Fixed items: OLED (J3), Kailh Choc V1 key switches, mounting holes H1-H4, board outline (Edge.Cuts).
They must match fixed_placement.json; change them only after explicit OK.

  python3 check_fixed.py            # compare TeensyRPN42.kicad_pcb to fixed_placement.json
  python3 check_fixed.py other.kicad_pcb # check another board file
  python3 check_fixed.py --snapshot # (re)write fixed_placement.json from the board
"""
import json, re, sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
PCB = Path(next((a for a in sys.argv[1:] if a.endswith(".kicad_pcb")), HERE / "TeensyRPN42.kicad_pcb"))
REF = HERE / "fixed_placement.json"


def blocks(s, head):
    for m in re.finditer(r"\n\t\(" + head, s):
        start = j = m.start() + 2  # skip "\n\t"
        depth = 0
        while True:
            c = s[j]
            if c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        yield s[start:j + 1]


def is_fixed(lib):
    return lib.startswith("PCM_Switch_Keyboard_Kailh:SW_Kailh_Choc") or lib == "RPN42Library:OLED-2.8inch16pin" or lib.startswith("MountingHole:")


def read_board():
    s = PCB.read_text()
    fps = {}
    for b in blocks(s, "footprint "):
        lib = re.match(r'\(footprint "([^"]+)"', b).group(1)
        if not is_fixed(lib):
            continue
        ref = re.search(r'\(property "Reference" "([^"]+)"', b).group(1)
        at = [float(v) for v in re.search(r"^\t\t\(at ([^)]+)\)", b, re.M).group(1).split()]
        fps[ref] = {
            "footprint": lib,
            "x": at[0], "y": at[1], "rot": at[2] if len(at) > 2 else 0.0,
            "layer": re.search(r'^\t\t\(layer "([^"]+)"\)', b, re.M).group(1),
        }
    outline = []
    for b in blocks(s, "gr_"):
        if '(layer "Edge.Cuts")' not in b:
            continue
        kind = re.match(r"\(gr_(\w+)", b).group(1)
        pts = [[float(x), float(y)] for x, y in re.findall(r"\((?:start|end|mid|center|xy) ([-\d.]+) ([-\d.]+)\)", b)]
        outline.append({"shape": kind, "points": pts})
    outline.sort(key=lambda o: (o["shape"], o["points"]))
    return {"footprints": dict(sorted(fps.items())), "outline": outline}


def main():
    cur = read_board()
    if "--snapshot" in sys.argv:
        REF.write_text(json.dumps(cur, indent=1) + "\n")
        print(f"wrote {REF.name}: {len(cur['footprints'])} footprints, {len(cur['outline'])} outline shape(s)")
        return 0
    ref = json.loads(REF.read_text())
    errs = []
    for r, want in ref["footprints"].items():
        got = cur["footprints"].get(r)
        if got is None:
            errs.append(f"{r}: missing on board")
            continue
        for k in ("footprint", "layer", "rot"):
            if got[k] != want[k]:
                errs.append(f"{r}: {k} {got[k]} != {want[k]}")
        if abs(got["x"] - want["x"]) > 1e-3 or abs(got["y"] - want["y"]) > 1e-3:
            errs.append(f"{r}: at ({got['x']}, {got['y']}) != ({want['x']}, {want['y']})")
    for r in cur["footprints"].keys() - ref["footprints"].keys():
        errs.append(f"{r}: new fixed-type footprint not in {REF.name}")
    if cur["outline"] != ref["outline"]:
        errs.append(f"outline changed: {cur['outline']} != {ref['outline']}")
    for e in errs:
        print("FAIL", e)
    print("OK: fixed placement unchanged" if not errs else f"{len(errs)} deviation(s)")
    return 1 if errs else 0


if __name__ == "__main__":
    sys.exit(main())
