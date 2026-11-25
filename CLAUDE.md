# Rules

Do not git anything yourself. only read is ok like checking diffs etc.
This includes the kicad MCP's `vcs_*` tools (`vcs_commit_checkpoint`, `vcs_restore_checkpoint`, `vcs_tag_release`, `vcs_init_git`). Don't call them.

# Fixed mechanics (TeensyRPN42) — do not change

These are final and must stay exactly where they are. Never move, rotate, flip or replace them, and
don't let auto-place/sync tools touch them. If a change seems necessary, **ask first**.

- **OLED** `J3` (`RPN42Library:OLED-2.8inch16pin`) at (103.6965, 92.202), F.Cu (moved to top 2026-11, approved).
- **37 Kailh Choc V1 key switches** (`SW38`–`SW78`; 1u/1.25u/2u), all at rot 180 on F.Cu.
- **Mounting holes** `H1`–`H4` (M3, `MountingHole_3.7mm` for OLED tolerance, changed 2026-11) at x 57.912 / 149.352, y 78.486 / 105.918.
- **Board outline:** Edge.Cuts rounded rect (47.068, 72.40032)–(160.068, 256.486) = 113.0 × 184.086 mm (top edge moved to J7 USB edge mark 2026-11), 3 mm corner radius (4 lines + 4 arcs).

All of these are also locked in KiCad (`(locked yes)`); never unlock them.
Reference: `PCB-KiCAD/TeensyRPN42/fixed_placement.json`. After any PCB edit run
`python3 check_fixed.py` there (exit 0 = unchanged). `--snapshot` rewrites the reference — only after the user OKs a change.

# Design preferences

- **SMD parts by default.** Resistors and capacitors in **0603** (`Resistor_SMD:R_0603_1608Metric`,
  `Capacitor_SMD:C_0603_1608Metric`); the `_HandSolder` pad variants are fine too.
  Connectors may stay THT (pin headers).
- **All parts on the top side** (single-sided assembly) — no components on B.Cu, including decoupling caps.
  Plan room for them during placement/fan-out instead of moving them to the bottom.
  TeensyRPN42 exception (intentional, leave as is): `J1` SWD and `J4` serial are on B.Cu.
  Everything else (incl. `J3` OLED and the side buttons `SW1`/`SW2`), and anything new, goes on top.
- **Ground fills on both sides:** GND zone on F.Cu and B.Cu
- **Heat:** under parts that get hot, add thermal vias (in/next to the thermal pad) and extra copper
  zones on both layers to spread heat. If the thermal pad is not GND (e.g. LM317 tab = OUT), the
  spreading zones are on that pad's net with higher priority than GND — never short it to GND.
- **Fab: AISLER 2-layer simple** rules from `/home/mario/DevelSelf/aisler-support/kicad/aisler-2-layer-simple-drc/`
  (min clearance 0.15, track 0.2, via 0.7/0.3 drill, copper-edge 0.3, text ≥0.8 mm).
  The project's `.kicad_dru` is an unmodified copy of the upstream file. Keep `aisler-support`
  pulled: versions before mid-2025 fail the solder-mask rule on every pad in KiCad 10 (unset is
  `null`), and had a 0.6 mm netclass via. Constraints/severities live in `.kicad_pro` → import via
  Board Setup → Import Settings from the AISLER `.kicad_pcb`.

# Editing existing designs

- Make **targeted edits** (e.g. footprint change → `sch_modify_property` / `sch_update_properties`).
  Do **not** rebuild the sheet with `sch_build_circuit` for small changes: it regenerates all symbol
  UUIDs (breaks schematic↔PCB links, so `pcb_sync_from_schematic` replaces every footprint),
  drops manual edits, and leaves `*.bak` files. `sch_build_circuit` is only for new/empty sheets.

# KiCad MCP setup (kicad-mcp-pro)

- Server `kicad` is configured in `.mcp.json`: profile `full`, mode `write`.
- KiCad 10.0.7 with the IPC API enabled (socket `/tmp/kicad/api.sock`).
- Start of session: call `kicad_set_project` if needed, then `kicad_get_version` to check IPC status.
- Reports from ERC/DRC go to `<project>/output/`.
- 3D models: `KICAD10_3DMODEL_DIR` = `/home/mario/DevelSelf/kicad-packages3D` (git clone, set in KiCad's
  Configure Paths → `~/.config/kicad/10.0/kicad_common.json`). `kicad-cli` and the MCP pick it up from there.
  3D check: `kicad-cli pcb render --perspective --rotate "-45,0,-30" -o out.png board.kicad_pcb`.

## Gotchas

- **Live-PCB tools only show up if a PCB is open in KiCad when the MCP connects.**
  The server hides IPC-dependent `pcb_*` tools (outline, tracks, save, place…)
  when no board is open, and Claude Code fetches the tool list only on connect.
  If they're missing: ask the user to open the PCB, then `/mcp` → `kicad` → Reconnect.
- **Schematic editing is file-based, not live.** KiCad 10 IPC only covers the PCB editor.
  The user should close the schematic editor while `sch_*` tools edit
  `.kicad_sch`, then reopen it (or File → Revert). Otherwise KiCad and the MCP
  overwrite each other's changes. `sch_reload` needs `experimental` mode, which isn't enabled.
- Many `sch_add_*` / `sch_build_circuit` / `sch_annotate` tools are marked
  experimental. Verify results with `sch_render_png` and `run_erc`.
- **KiCad 10 footprints with a `(point ...)` reference point break live IPC reads** (e.g. TO-220-3_Vertical).
  `kicad-python` 0.8.0 fails with "Missing type mapping for ...ReferencePoint". Workaround: delete the
  `(point ...)` block from that footprint in the `.kicad_pcb`, then `pcb_revert` to reload.
- **Live graphic shapes trigger a KiCad assert** (`FromProtoEnum<StrokeLineStyle>`): `pcb_set_board_outline`
  and other shape tools pop a dialog in KiCad that blocks IPC until the user dismisses it. Prefer writing
  outlines/zones into the file (validate on a scratch copy with `kicad-cli pcb drc --refill-zones`).
- **File-edit pattern while the PCB is open:** `pcb_save` → edit/copy file → `pcb_revert` (reloads from disk).
  `pcb_sync_from_schematic(allow_open_board=True)` works the same way.
- **Board Setup changes (e.g. rule import) live only in KiCad's memory until saved.** `kicad-cli`/MCP DRC
  read `.kicad_pro` from disk → `pcb_save` first. Never `pcb_revert` while unsaved setup changes exist.
- **Cosmetic angled header in 3D** (keep vertical footprint): model `PinHeader_1x02_P2.54mm_Horizontal.step`,
  rotate z 180, offset y −2.54 (footprint at −90°) → pins point out over the top edge.
- **Silkscreen branding:** knockout text = `(layer "B.SilkS" knockout)`; `(bold yes)`/`(italic yes)` in `font`;
  back-side text needs `(justify mirror)`. `kicad-cli pcb render` (3D) ignores italic — check the real
  artwork with `kicad-cli pcb export svg --layers B.SilkS,... --mirror`. Keep silk off via mask openings.
- Tool inventory/debugging: `kicad-mcp-pro tools list --json --profile full --mode write`
  (needs `KICAD_MCP_PROJECT_DIR` set).

- **Rerouting tip:** GND fan-out/stitching vias block the router. Before rerouting, move all GND vias out
  of the way (or delete them) and recreate them once routing is done.

## Workflow

Schematic (file edits) → `pcb_sync_from_schematic` → placement/routing on the live board → `run_drc` / `run_erc` → `pcb_save`.

## Finishing touches (once the board is functionally done)

0. **Generous GND stitching vias:** besides vias at SMD GND pads, spread stitching vias over the
  GND fills (edges, corners, around split areas) so top/bottom GND are well tied together.
1. **Silkscreen:** connector pin labels on top (e.g. `IN G G OUT`), part refs ≥0.8 mm placed clear of pads.
2. **Back-side branding:** knockout bold-italic `@mb` plate + spaced year (`2 0 2 6`) below it, plus a fine
   italic line with the key formula (e.g. `Vout=1.25V×(1+R2/R1)`). Mirrored, kept clear of via openings
   (thin out non-critical vias if needed). Verify via SVG export of `B.SilkS`.
3. **3D renders** into `<project>/renders/` (1920×1200, transparent PNG). Run one at a time; in zsh use a
   function, not a command stored in a variable:
   ```sh
   run(){ out=$1; shift; kicad-cli pcb render --quality high --floor --width 1920 --height 1200 "$@" -o renders/$out board.kicad_pcb; }
   run <name>_iso_front.png   --perspective --rotate "-50,0,-35"  --zoom 1.1
   run <name>_iso_headers.png --perspective --rotate "-55,0,-145" --zoom 1.1
   run <name>_iso_back.png    --perspective --rotate "130,0,-35"  --zoom 1.1
   run <name>_top.png    --side top    --zoom 0.9
   run <name>_bottom.png --side bottom --zoom 0.9
   ```
4. **`<project>/README.md`:** short description, image table (renders), specs, pinout, BOM,
   design notes.
5. **Interactive HTML BOM** into `<project>/bom/<name>_ibom.html` (plugin installed via PCM; runs headless
   with the system python that has `pcbnew`):
   ```sh
   INTERACTIVE_HTML_BOM_NO_DISPLAY=1 /usr/bin/python3 \
     ~/.local/share/kicad/10.0/3rdparty/plugins/org_openscopeproject_InteractiveHtmlBom/generate_interactive_bom.py \
     board.kicad_pcb --no-browser --dest-dir bom --name-format "%f_ibom" --include-tracks --include-nets --layer-view FB
   ```
   Data is LZString-compressed — can't grep parts; the user checks it in a browser. Link it from the README.
