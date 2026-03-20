# Blood Wake - Static Recompilation

## Project Overview
Static recompilation of the original Xbox game "Blood Wake" (2001)
into a native Windows PC executable using the xboxrecomp toolkit.

## Game Info
- **Title:** Blood Wake
- **Developer:** Stormfront Studios
- **Publisher:** Microsoft Game Studios
- **Release:** November 15, 2001 (Xbox launch title)
- **Genre:** Naval combat (28 missions)
- **Platform:** Xbox exclusive (never ported)

## XBE Analysis
- **XBE Size:** 1,757,184 bytes (~1.7 MB)
- **Base Address:** 0x00010000
- **Entry Point:** 0x000DE15F
- **Kernel Thunk Table:** 0x001745C0
- **Sections:** 24 (including 9 BINK video codec sections)
- **Kernel Imports:** 118
- **Functions:** 4,608 (4,565 translated, 43 failed = 99.1% success)
- **Generated C:** 367,758 lines across 5 source files

### Memory Map
```
0x00011000 - 0x000F9C10  .text     (931 KB)   Game code (3,195 functions)
0x000F9C20 - 0x00116E38  DSOUND    (116 KB)   DirectSound library (422 functions)
0x00116E40 - 0x00124F48  WMADEC    (56 KB)    WMA audio decoder (2 functions)
0x00124F60 - 0x00136B1C  D3D       (71 KB)    Direct3D library (236 functions)
0x00136B20 - 0x00152FC0  D3DX      (113 KB)   D3DX extensions (391 functions)
0x00152FC0 - 0x00154561  XGRPH     (5 KB)     Xbox graphics (22 functions)
0x00154580 - 0x0016CCC0  BINK*     (96 KB)    Bink video codec (9 sections, 113 functions)
0x0016CE20 - 0x001745BC  XPP       (30 KB)    Xbox Platform Plugin (181 functions)
0x001745C0 - 0x00184320  .rdata    (63 KB)    Constants
0x00184320 - 0x0028799C  .data     (1037 KB)  Data + BSS
0x002879A0 - 0x0028E738  DOLBY     (27 KB)    Dolby audio
0x0028E740 - 0x00292990  BINKDATA  (17 KB)    Bink data
0x002929A0 - 0x002951A0  $$XTIMAGE (10 KB)    Title image
```

### Notable Characteristics
- **Bink Video:** Uses Bink (.bik) for FMV, not XMV like later Xbox games
- **9 Bink sections:** Separate code sections for each pixel format decoder
- **Custom engine:** Stormfront Studios proprietary engine (no RenderWare)
- **Large .data section:** 1 MB+ data, likely pre-allocated game state
- **118 kernel imports:** Slightly more than Wreckless (113), fewer than Burnout 3 (147)
- **WMA audio:** Uses WMA decoder, not the typical XDK DirectSound streaming

## Architecture
Based on the [xboxrecomp](https://github.com/sp00nznet/xboxrecomp) framework.

### Key Directories
- `src/game/` - Recompiled game code and entry point
- `src/game/recomp/gen/` - Auto-generated C source (git-ignored)
- `tools/` - xboxrecomp pipeline tools
- `Blood Wake (USA)/` - Extracted game files (git-ignored)

### Build System
- CMake 3.20+ with MSVC (Visual Studio 2022), x64
- C11 standard
- Links against xboxrecomp runtime libraries (../xboxrecomp/)

### Pipeline
1. `tools.disasm` - Disassemble all 24 sections (4,608 functions found)
2. `tools.func_id` - Identify/classify functions (9 CRT found)
3. `tools.recomp` - Lift x86 to C (367K lines, 4,565/4,608 translated)
4. CMake build - Compile native Windows executable

## Game Asset Structure
```
Blood Wake (USA)/
├── Default.xbe          # Game executable (1.7 MB)
├── Splash.toc           # Splash screen table of contents
├── audio/               # Sound effects (.pak)
├── boats/               # Boat models (.pak) - Basilisk, Catamaran, Destroyer, etc.
├── common/              # Shared assets
├── fonts/               # Font data
├── languages/           # Localization
├── misc/                # Miscellaneous data
├── missions/            # 28 missions (01.pak through 28.pak + env/voice)
├── music/               # Background music
├── objects/             # World objects
├── overlays/            # HUD/overlay graphics
├── screens/             # Menu screens, act intros, frontend (.pak)
├── video/               # Bink video (.bik) - MS logo, Stormfront logo
└── weapons/             # Weapon data
```

## Progress
- [x] ISO extracted (450 files)
- [x] Project repo initialized
- [x] XBE parsed and analyzed (24 sections, 118 kernel imports)
- [x] Disassembly complete (4,608 functions across all sections)
- [x] Function identification (9 CRT, 0 RenderWare)
- [x] x86-to-C recompilation (367K lines, 99.1% success rate)
- [x] Project scaffolded (CMakeLists.txt, main.c, recomp types/dispatch)
- [ ] Kernel layer adapted (118 imports to map)
- [ ] First build
- [ ] First boot
- [ ] Graphics scaffolded
- [ ] Audio/input integration
- [ ] Gameplay debugging

## Known Issues
- 43 functions failed translation (complex instruction patterns)
- Need to identify SEH prolog/epilog addresses for this game
- Bink video sections may need special handling (demand-loaded?)
- WMA decoder section (WMADEC) different from Wreckless's WMV decoder
