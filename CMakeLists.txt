cmake_minimum_required(VERSION 3.28)
project(VelvetEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Enforces the standard we set instead of silently using an older standard if not available
set(CMAKE_CXX_EXTENSIONS ON)            # Enables the use of compiler-specific extensions

set(WARNING_FLAGS
        #-Werror    # Turns all warnings into errors # Would not recommend enabling unless you're planning to fix *every* warning
        -Wall   # Enable most warning messages.
        -Wextra # Print extra (possibly unwanted) warnings.
        #-Wpedantic # Issue warnings needed for strict compliance to the standard. # We'll probably never need this
        -Wcast-align    # Warn about pointer casts which increase alignment. For example, warn if a char * is cast to an int * on machines where integers can only be accessed at two- or four-byte boundaries.
        -Wcast-qual # Warn about casts which discard qualifiers. For example, warn if a const char * is cast to an ordinary char *
        -Wctor-dtor-privacy # Warn when all constructors and destructors are private.
        -Wdisabled-optimization # Warn when an optimization pass is disabled.
        -Wformat=2  # Warn about printf/scanf/strftime/strfmon format string anomalies.
        -Wformat-truncation
        -Winit-self # Warn about variables which are initialized to themselves.
        -Wlogical-op    # Warn when a logical operator is suspiciously always evaluating to true or false.
        #-Wmissing-declarations # Warn about global functions without previous declarations.
        -Wmissing-include-dirs # Warn about user-specified include directories that do not exist.
        -Wnoexcept  # Warn when a noexcept expression evaluates to false even though the expression can't actually throw.
        -Wold-style-cast    # Warn if a C-style cast is used in a program.
        -Woverloaded-virtual    # Warn about overloaded virtual function names.
        -Wredundant-decls   # Warn about multiple declarations of the same object.
        -Wshadow    # Warn when one variable shadows another # Might disable this one
        -Wsign-conversion   # Warn for implicit size conversions between signed and unsigned integers.
        -Wconversion
        -Wsign-promo    # Warn when overload promotes from unsigned to signed.
        -Wstrict-null-sentinel  # Warn about uncasted NULL used as sentinel.
        -Wstrict-overflow=5 # Warn about optimizations that assume that signed overflow is undefined.
        -Wswitch-default    # Warn about enumerated switches missing a "default:" statement.
        -Wundef # Warn if an undefined macro is used in an #if directive.
        -Wzero-as-null-pointer-constant # Warn when a literal '0' is used as null pointer.
        -Wuseless-cast  # Warn about useless casts.
        -Wpadded # Warn when padding is required to align structure members.
        -Wdouble-promotion

        -Wno-unknown-pragmas # Disables warning about unkown pragmas since both clion and clang-tidy use their own
) # May need to add or remove some warnings from this list depending on if they actually apply to our system or not
#set(WARNING_FLAGS -w) # Disable all warnings

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DEVKITPRO $ENV{DEVKITPRO} CACHE PATH "Path to the main DevKitPro tools directory")
set(LIBOGC_PATH $ENV{LIBOGC_PATH} CACHE PATH "Path to the custom libogc source code")

set(CMAKE_SYSTEM_NAME NintendoGameCube)
include(${DEVKITPRO}/cmake/GameCube.cmake)
include(${DEVKITPRO}/cmake/Platform/NintendoGameCube.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT}")

set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Og -DDEBUG -save-temps -fverbose-asm")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -Ofast -DNDEBUG")

get_filename_component(STATICLCF static.lcf ABSOLUTE)

# map file parsing for Dolphin
get_filename_component(PARSEMAPPY parse_map.py ABSOLUTE)
find_package(Python REQUIRED)

# System includes
unset(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES CACHE) # get rid of default libogc system includes from the toolchain file
include_directories(SYSTEM ${LIBOGC_PATH}/gc)
include_directories(SYSTEM ${DEVKITPRO}/portlibs/ppc/include)

# libogc link paths
link_directories(${LIBOGC_PATH}/lib/cuberiders)
link_directories(${DEVKITPRO}/portlibs/ppc/lib)

add_link_options(-T ${STATICLCF}#[[/devkitPPC/powerpc-eabi/lib/ogc.ld]] -Wl,-Map=main.map,--gc-sections,-d)
# libogc and libm get linked by toolchain file
# link_libraries(ogc m)
include_directories(src include)
add_compile_options(${WARNING_FLAGS}
        -memb
        -fno-threadsafe-statics -fno-rtti -fno-asynchronous-unwind-tables -fno-common
        -mmultiple -mregnames -mno-longcall -msdata=eabi
)
add_executable(main
        main.cpp
        src/velvet/core/Engine.cpp
        src/velvet/core/Engine.hpp
        src/velvet/core/vtx/VtxDescription.hpp
        src/velvet/Renderer.cpp
        src/velvet/Renderer.hpp
        src/velvet/Camera.cpp
        src/velvet/Camera.hpp
        include/defines.hpp
        src/velvet/dvd/DVD.cpp
        src/velvet/dvd/DVD.hpp
        src/velvet/input/Controller.cpp
        src/velvet/input/Controller.hpp
        src/velvet/input/Input.cpp
        src/velvet/input/Input.hpp
)

function(create_dol target)
    ogc_create_dol(${target})
    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${Python_EXECUTABLE} ${PARSEMAPPY} ${PROJECT_BINARY_DIR})
endfunction()

create_dol(main)