cmake_minimum_required(VERSION 3.30)
project(Infi_Advent_of_Code)

set(CMAKE_CXX_STANDARD 23)

add_executable(dag01
        dag01/dag01.cpp
        dag01/dag01.h)

# Function to detect the compiler and apply warning flags
function(add_compile_warnings target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target} PRIVATE
            -Wall                        # Enable most warnings
            -Wextra                      # Enable extra warnings
            -Wpedantic                   # Enforce strict ISO C++ compliance
            -Wshadow                     # Warn if a variable shadows another
            -Wnon-virtual-dtor            # Warn if a class with virtual functions has a non-virtual destructor
            -Wold-style-cast              # Warn if using C-style casts
            -Wcast-align                  # Warn about potential performance issues from casting pointers
            -Woverloaded-virtual          # Warn if a base class function is hidden
            -Wconversion                  # Warn on implicit conversions
            -Wsign-conversion             # Warn on implicit sign conversions
            -Wmisleading-indentation      # Warn if indentation does not match the code flow
            -Wduplicated-cond             # Warn if there are duplicated conditions
            -Wduplicated-branches         # Warn if there are duplicated branches
            -Wlogical-op                  # Warn on suspicious logical operations
            -Wnull-dereference            # Warn if a null pointer is dereferenced
            -Wuseless-cast                # Warn on casts that do nothing
            -Wdouble-promotion            # Warn if a `float` is implicitly promoted to a `double`
            -Wformat=2                    # Warn on format strings
            -Wimplicit-fallthrough        # Warn on missing `break` statements in switch cases
            -Wunreachable-code            # Warn on unreachable code
            -Wunsafe-loop-optimizations   # Warn if a loop might have unsafe optimizations
            -Wextra-semi                  # Warn on unnecessary semicolons
            -Wstrict-aliasing=2           # Warn on potential strict aliasing violations
            -fstack-protector-strong      # Enable stack protection
            -fno-omit-frame-pointer       # Don't omit frame pointers for better debugging
            -g                            # Enable debug information
        )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE
            /W4                         # Set warning level to 4 (all recommended warnings)
            /permissive-                # Enforce strict C++ compliance
            /analyze                    # Enable static code analysis
            /sdl                        # Enable additional security checks
            /Zi                         # Enable debug information
            /WX                         # Treat warnings as errors (optional)
            /EHsc                       # Enable standard C++ exception handling
            /MP                         # Enable multi-processor compilation
        )
    endif()
endfunction()

# Add the warning flags to the `dag01` target
add_compile_warnings(dag01)