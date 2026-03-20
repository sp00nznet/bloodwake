
# Blood Wake - Recompiled Game Executable

# Collect generated recompiled code
file(GLOB RECOMP_GEN_SOURCES "recomp/gen/recomp_*.c")

add_executable(bloodwake WIN32
    main.c
    recomp/recomp_stubs.c
    recomp/recomp_manual.c
    ${RECOMP_GEN_SOURCES}
)

# Generated code uses register name macros
target_compile_definitions(bloodwake PRIVATE RECOMP_GENERATED_CODE)

# Suppress warnings in mechanically translated code
target_compile_options(bloodwake PRIVATE /w)

# Large stack for deep call chains in recompiled code
target_link_options(bloodwake PRIVATE
    /STACK:16777216
    /DYNAMICBASE:NO
)

target_include_directories(bloodwake PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/recomp
)

# Link xboxrecomp runtime libraries
target_link_libraries(bloodwake PRIVATE
    xbox_kernel
    xbox_d3d8
    xbox_dsound
    xbox_input
    # Uncomment as needed:
    # xbox_apu
    # xbox_nv2a
)

# System libraries
target_link_libraries(bloodwake PRIVATE
    d3d11
    dxgi
    d3dcompiler
    xinput
    winmm
)

# Output to bin/
set_target_properties(bloodwake PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin/RelWithDebInfo"
)
