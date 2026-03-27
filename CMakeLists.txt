cmake_minimum_required(VERSION 3.20)

project(WinKoGNN
    VERSION 1.0.0
    DESCRIPTION "OpenCog Workbench for Windows NT4 Cognitive Kernel with Graph Neural Networks"
    LANGUAGES C CXX
)

# -----------------------------------------------------------------------
# Build options
# -----------------------------------------------------------------------
option(BUILD_ATENSPACE "Build ATenSpace atomspace targets (requires LibTorch)" OFF)
option(BUILD_COG0 "Build cog0 targets (requires cog0 source tree)" OFF)

# -----------------------------------------------------------------------
# Global settings
# -----------------------------------------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands so tools (e.g. MSVC code-analysis) can consume them.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------
# NT4 kernel source tree
# -----------------------------------------------------------------------
set(NT4_ROOT "${CMAKE_SOURCE_DIR}/private/ntos")

# NT compatibility shims — cross-platform stubs for missing DDK/SDK headers.
# MUST appear first so they are found before any system headers.
set(NT_COMPAT_DIR "${CMAKE_SOURCE_DIR}/include/nt_compat")

# Primary include directories taken from the NT4 source tree.
set(NT4_INCLUDE_DIRS
    "${NT_COMPAT_DIR}"
    "${NT4_ROOT}/inc"
    "${NT4_ROOT}/dbgk"
    "${NT4_ROOT}/ke"
    "${NT4_ROOT}/mm"
    "${NT4_ROOT}/ps"
    "${NT4_ROOT}/io"
    "${NT4_ROOT}/ob"
    "${NT4_ROOT}/se"
    "${NT4_ROOT}/rtl"
    "${NT4_ROOT}/ex"
    "${NT4_ROOT}/lpc"
    "${NT4_ROOT}/nls"
    "${NT4_ROOT}/config"
)

# Preprocessor definitions required when analysing NT kernel-mode sources.
set(NT4_COMPILE_DEFINITIONS
    _X86_
    i386
    STD_CALL
    CONDITION_HANDLING=1
    NT_UP=1
    NT_INST=0
    WIN32=100
    _NT1X_=100
    WINNT=1
    _WIN32_WINNT=0x0400
    WIN32_LEAN_AND_MEAN=1
    DBG=0
    DEVL=1
    FPO=0
    NDEBUG
)

# -----------------------------------------------------------------------
# Collect NT4 C sources (recursively, excluding architecture-specific
# sub-trees that require a targeted cross-compiler to analyse).
# -----------------------------------------------------------------------
file(GLOB_RECURSE NT4_SOURCES
    "${NT4_ROOT}/afd/*.c"
    "${NT4_ROOT}/bowser/*.c"
    "${NT4_ROOT}/cache/*.c"
    "${NT4_ROOT}/cdfs/*.c"
    "${NT4_ROOT}/config/*.c"
    "${NT4_ROOT}/dbgk/*.c"
    "${NT4_ROOT}/diskreg/*.c"
    "${NT4_ROOT}/dlc/*.c"
    "${NT4_ROOT}/ex/*.c"
    "${NT4_ROOT}/fastfat/*.c"
    "${NT4_ROOT}/fsrec/*.c"
    "${NT4_ROOT}/fsrtl/*.c"
    "${NT4_ROOT}/init/*.c"
    "${NT4_ROOT}/io/*.c"
    "${NT4_ROOT}/kd/*.c"
    "${NT4_ROOT}/ke/*.c"
    "${NT4_ROOT}/lfs/*.c"
    "${NT4_ROOT}/lpc/*.c"
    "${NT4_ROOT}/mailslot/*.c"
    "${NT4_ROOT}/mm/*.c"
    "${NT4_ROOT}/mup/*.c"
    "${NT4_ROOT}/nbt/*.c"
    "${NT4_ROOT}/ndis/*.c"
    "${NT4_ROOT}/netbios/*.c"
    "${NT4_ROOT}/nls/*.c"
    "${NT4_ROOT}/npfs/*.c"
    "${NT4_ROOT}/ob/*.c"
    "${NT4_ROOT}/po/*.c"
    "${NT4_ROOT}/ps/*.c"
    "${NT4_ROOT}/raw/*.c"
    "${NT4_ROOT}/rdr/*.c"
    "${NT4_ROOT}/rtl/*.c"
    "${NT4_ROOT}/se/*.c"
    "${NT4_ROOT}/seaudit/*.c"
    "${NT4_ROOT}/smbtrsup/*.c"
    "${NT4_ROOT}/srv/*.c"
    "${NT4_ROOT}/tdi/*.c"
    "${NT4_ROOT}/udfs/*.c"
    "${NT4_ROOT}/vdm/*.c"
    "${NT4_ROOT}/video/*.c"
)

# -----------------------------------------------------------------------
# NT4 static analysis target
# Using OBJECT library so sources appear in compile_commands.json without
# a link step (kernel code cannot be linked into a user-mode binary).
# -----------------------------------------------------------------------
if(NT4_SOURCES)
    add_library(nt4_kernel OBJECT ${NT4_SOURCES})

    target_include_directories(nt4_kernel
        PRIVATE
            ${NT4_INCLUDE_DIRS}
    )

    target_compile_definitions(nt4_kernel
        PRIVATE
            ${NT4_COMPILE_DEFINITIONS}
    )

    if(MSVC)
        # Suppress SAL annotations that conflict with NT4 IN/OUT/OPTIONAL usage.
        # Also suppress warnings about type redefinitions between our shims and
        # the Windows SDK (winnt.h) — these are intentional compatibility overlaps.
        target_compile_definitions(nt4_kernel
            PRIVATE
                _NO_SAL_2          # Disable SAL2 annotations
                _SAL_NO_ATTR_1     # Disable SAL1 attributes
                _CRT_SECURE_NO_WARNINGS
        )
        # Suppress C2011 (type redefinition) and C2371 (redefinition; different basic types)
        # These occur when our ntdef.h types overlap with winnt.h types.
        # Our ntdef.h guards most types with #ifndef on _WIN32, but some
        # edge cases remain with the NT4 source tree's own internal headers.
        target_compile_options(nt4_kernel
            PRIVATE
                /W3          # Warning level 3
                /WX-         # Warnings are not errors (legacy code)
                /wd4013      # undefined; assuming extern returning int
                /wd4033      # function must return a value
                /wd4047      # differs in levels of indirection
                /wd4048      # declared array bounds differ
                /wd4055      # conversion from data ptr to function ptr
                /wd4113      # function parameter lists differ
                /wd4131      # old-style declarator
                /wd4142      # benign redefinition of type
                /wd4200      # zero-sized array in struct/union
                /wd4201      # nameless struct/union
                /wd4214      # bit-field type other than int
                /wd4244      # conversion, possible loss of data
                /wd4245      # signed/unsigned mismatch
                /wd4267      # size_t to int conversion
                /wd4273      # inconsistent DLL linkage
                /wd4295      # array too small
                /wd4310      # cast truncates constant value
                /wd4312      # conversion to greater-size type
                /wd4700      # uninitialized local variable used
                /wd4706      # assignment within conditional
                /wd4996      # deprecated functions
                /wd4005      # macro redefinition
                /wd4081      # expected pragma token
                /wd4668      # not defined as preprocessor macro
        )
    endif()
endif()

# -----------------------------------------------------------------------
# ATenSpace integration (o9nn/ATenSpace, vendored at aten/)
#
# The ATenSpace library provides a C++17 header-only AtomSpace backed by
# ATen tensor operations.  Sources live under:
#   aten/aten/src/ATen/atomspace/  -- AtomSpace C++ headers (*.h)
#   aten/aten/src/TH/              -- Low-level Torch-CPU C sources (*.c / *.cpp)
#   aten/aten/src/ATen/            -- ATen tensor layer (*.h / *.cpp)
#
# When LibTorch is present (BUILD_ATENSPACE=ON), we expose the atomspace
# headers as an INTERFACE library so every downstream target can do:
#   #include <ATen/atomspace/ATenSpace.h>
# -----------------------------------------------------------------------
set(ATENSPACE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/aten/aten/src")

add_library(atenspace_headers INTERFACE)
target_include_directories(atenspace_headers INTERFACE "${ATENSPACE_INCLUDE_DIR}")
target_compile_features(atenspace_headers INTERFACE cxx_std_17)

if(BUILD_ATENSPACE)
    find_package(Torch QUIET)
    if(Torch_FOUND)
        message(STATUS "LibTorch found -- building ATenSpace atomspace targets")
        add_subdirectory(aten/aten/src/ATen/atomspace EXCLUDE_FROM_ALL)
    else()
        message(WARNING
            "BUILD_ATENSPACE=ON but LibTorch was not found. "
            "Set Torch_DIR to your LibTorch installation and re-run cmake.")
    endif()
endif()

# -----------------------------------------------------------------------
# cog0 integration stub (ReZorg/cog0, vendored at cog0/)
#
# NOTE: ReZorg/cog0 is a private repository and could not be cloned
# automatically.  Once the source tree is available, vendor it at cog0/
# and set BUILD_COG0=ON.  The expected layout mirrors cog0's own layout:
#   cog0/include/   -- public C/C++ headers (*.h / *.hxx)
#   cog0/src/       -- implementation sources (*.c / *.cpp / *.cxx)
# -----------------------------------------------------------------------
if(BUILD_COG0)
    if(EXISTS "${CMAKE_SOURCE_DIR}/cog0/CMakeLists.txt")
        add_subdirectory(cog0 EXCLUDE_FROM_ALL)
    else()
        message(WARNING
            "BUILD_COG0=ON but cog0/CMakeLists.txt not found. "
            "Vendor the cog0 source tree at ${CMAKE_SOURCE_DIR}/cog0/ first.")
    endif()
endif()

# -----------------------------------------------------------------------
# Enable CTest
# -----------------------------------------------------------------------
enable_testing()

# -----------------------------------------------------------------------
# Optional: Python e2e tests via CTest (only when Python is available)
# -----------------------------------------------------------------------
find_package(Python3 COMPONENTS Interpreter QUIET)
if(Python3_FOUND)
    add_test(
        NAME winkognn_e2e_tests
        COMMAND ${Python3_EXECUTABLE} -m pytest
                "${CMAKE_SOURCE_DIR}/tests"
                --tb=short -q
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
    set_tests_properties(winkognn_e2e_tests PROPERTIES
        TIMEOUT 120
    )
endif()
