# Blood Wake - Static Recompilation

A static recompilation of the original Xbox game **Blood Wake** (2001, Stormfront Studios)
into a native Windows PC executable.

This project translates the game's original x86 machine code into portable C source
code, which is then compiled to a native x86-64 Windows binary. No emulation is
involved at runtime.

## Status

| Phase | Status | Details |
|-------|--------|---------|
| XBE Parsing | Done | 24 sections, 118 kernel imports identified |
| Disassembly | Done | All sections, 4,608 functions |
| Function ID | Done | 9 CRT identified, custom Stormfront engine |
| Recompilation | Done | 367,758 lines of C across 5 source files |
| Kernel Layer | Not Started | 118 imports to map |
| First Build | Not Started | Pending kernel layer |
| First Boot | Not Started | |
| Graphics (D3D8->D3D11) | Not Started | |
| Audio (DS->XAudio2) | Not Started | |
| Input (XPP->XInput) | Not Started | |
| Gameplay | Not Started | |

## XBE Analysis

| Property | Value |
|----------|-------|
| Title | Blood Wake |
| Developer | Stormfront Studios |
| Publisher | Microsoft Game Studios |
| Release | November 15, 2001 (launch title) |
| Base Address | 0x00010000 |
| Entry Point | 0x000DE15F |
| Code Size | ~931 KB (.text) |
| Total Sections | 24 (incl. 9 Bink video codec) |
| Kernel Imports | 118 |
| Functions | 4,608 (4,565 translated, 43 failed) |

### Memory Map

```
0x00011000 - 0x000F9C10  .text     (931 KB)   Game code
0x000F9C20 - 0x00116E38  DSOUND    (116 KB)   DirectSound library
0x00116E40 - 0x00124F48  WMADEC    (56 KB)    WMA audio decoder
0x00124F60 - 0x00136B1C  D3D       (71 KB)    Direct3D library
0x00136B20 - 0x00152FC0  D3DX      (113 KB)   D3DX extensions
0x00152FC0 - 0x00154561  XGRPH     (5 KB)     Xbox graphics
0x00154580 - 0x0016CCC0  BINK*     (96 KB)    Bink video (9 sections)
0x0016CE20 - 0x001745BC  XPP       (30 KB)    Xbox Platform Plugin
0x001745C0 - 0x00184320  .rdata    (63 KB)    Constants
0x00184320 - 0x0028799C  .data     (1037 KB)  Data + BSS
0x002879A0 - 0x0028E738  DOLBY     (27 KB)    Dolby audio
0x0028E740 - 0x00292990  BINKDATA  (17 KB)    Bink data
0x002929A0 - 0x002951A0  $$XTIMAGE (10 KB)    Title image
```

## Building

### Prerequisites

- Windows 11
- Visual Studio 2022 (MSVC compiler)
- CMake 3.20+
- Python 3.10+ with `capstone` (`pip install capstone`)
- [xboxrecomp](https://github.com/sp00nznet/xboxrecomp) toolkit (cloned alongside this repo)

### Pipeline (generate recompiled code)

```bash
# 1. Disassemble
py -3 -m tools.disasm "Blood Wake (USA)/Default.xbe" \
    --analysis-json "Blood Wake (USA)/default_analysis.json" -v

# 2. Identify functions
py -3 -m tools.func_id "Blood Wake (USA)/Default.xbe" \
    --functions tools/disasm/output/functions.json \
    --strings tools/disasm/output/strings.json \
    --xrefs tools/disasm/output/xrefs.json -v

# 3. Recompile to C
py -3 -m tools.recomp "Blood Wake (USA)/Default.xbe" \
    --all --split 1000 --gen-dir src/game/recomp/gen -v
```

### Build (compile native executable)

```bash
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

## Architecture

Built on the [xboxrecomp](https://github.com/sp00nznet/xboxrecomp) framework,
the second game targeted after
[Burnout 3](https://github.com/sp00nznet/burnout3) and alongside
[Wreckless](https://github.com/sp00nznet/wreckless).

```
bloodwake/
  src/
    game/       Recompiled game code + entry point
      recomp/
        gen/    Auto-generated C source (367K lines, git-ignored)
  tools/        xboxrecomp pipeline (disasm, func_id, recomp)
```

## Technical Notes

- Blood Wake uses a **custom Stormfront Studios engine** (not RenderWare)
- First-party Microsoft launch title — likely exemplary XDK usage
- Engine reportedly built by a single programmer during a holiday break
- Uses **Bink video** (.bik) instead of XMV for FMV sequences (9 dedicated codec sections)
- Uses **WMA audio decoder** (WMADEC section) rather than standard DirectSound streaming
- 28 discrete missions with environmental and voice data per mission
- All game assets stored in custom .pak format
- Includes Fuzion Frenzy demo and other Xbox launch demos on disc

## License

This project contains no copyrighted game assets. You must provide your own copy
of the original game. The recompilation tools and compatibility layers are provided
for educational and preservation purposes.
