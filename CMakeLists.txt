cmake_minimum_required(VERSION 3.16)

# Define the preferred order of compilers
set(PREFERRED_CXX_COMPILERS 
    icpx      # Intel oneAPI compiler
    cl        # MSVC compiler
    g++       # MinGW or Cygwin g++
    clang++   # Clang 
)

# Function to find the first available compiler from the list
function(find_preferred_cxx_compiler)
    foreach(compiler ${PREFERRED_CXX_COMPILERS})
        # On Windows, use full paths and additional executable names
        if(WIN32)
            # Check full paths in typical installation locations
            set(COMPILER_PATHS
                "C:/Program Files/Intel/oneAPI/compiler/latest/windows/bin/icpx.exe"
                "C:/Program Files (x86)/Intel/oneAPI/compiler/latest/windows/bin/icpx.exe"
                "C:/Program Files/MSVC/bin/Hostx64/x64/cl.exe"
                "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/*/bin/Hostx64/x64/cl.exe"
                "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/*/bin/Hostx64/x64/cl.exe"
                "C:/MinGW/bin/g++.exe"
                "C:/Program Files/LLVM/bin/clang++.exe"
            )

            foreach(compiler_path ${COMPILER_PATHS})
                file(GLOB MATCHED_PATHS "${compiler_path}")
                foreach(matched_path ${MATCHED_PATHS})
                    if(EXISTS "${matched_path}")
                        set(CMAKE_CXX_COMPILER "${matched_path}" CACHE FILEPATH "C++ compiler" FORCE)
                        message(STATUS "Using preferred C++ compiler (path): ${matched_path}")
                        return()
                    endif()
                endforeach()
            endforeach()
        else()
            # Existing Linux/Unix logic
            find_program(CXX_COMPILER_PATH ${compiler})
            if(CXX_COMPILER_PATH)
                set(CMAKE_CXX_COMPILER "${CXX_COMPILER_PATH}" CACHE FILEPATH "C++ compiler" FORCE)
                message(STATUS "Using preferred C++ compiler (path): ${CXX_COMPILER_PATH}")
                return()
            endif()
        endif()
    endforeach()
    
    message(WARNING "None of the preferred C++ compilers were found. Using CMake's default.")
endfunction()

# Call the function to find the preferred compiler
find_preferred_cxx_compiler()

# Set the project name (same name used for the exe)
set(PROJNAME "memory-hogger")

project(${PROJNAME} VERSION 1.0.0 LANGUAGES CXX)
set(MODULE_NAME ${PROJECT_NAME})
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Get the source code files
file(GLOB SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Create the executable
add_executable(${PROJNAME} "${SOURCE_FILES}")

# Link the standard libraries in a platform-independent way
target_link_libraries(${PROJNAME} PRIVATE ${CMAKE_CXX_STANDARD_LIBRARIES})

# Build options
option(BUILD_DEBUG "Build debug version" OFF)

# Set compiler flags based on the compiler and build type
if(MSVC)
    # MSVC compiler flags
    set(COMMON_FLAGS "/W4")
    set(DEBUG_FLAGS "/Od /Zi")
    set(RELEASE_FLAGS "/O2")
    target_link_libraries(${PROJNAME} PRIVATE 
        kernel32 user32 gdi32 winspool shell32 
        ole32 oleaut32 uuid comdlg32 advapi32
    )
    message("Using MSVC")
else()
    # GCC, Clang, and other compiler flags
    set(COMMON_FLAGS "-Wall -Wextra")
    set(DEBUG_FLAGS "-O0 -g")
    set(RELEASE_FLAGS "-O3")
endif()

if(BUILD_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} ${DEBUG_FLAGS}")
    target_compile_definitions(${PROJNAME} PRIVATE "_DEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} ${RELEASE_FLAGS}")
endif()

# Check the compiler and set compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    # Settings for Intel C++ compilers
    # add_compile_options(-Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # Settings for GNU compilers (e.g., GCC, G++)
    # add_compile_options(-Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Settings for Microsoft Visual C++ compiler
    # add_compile_options(/W4)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Settings for Clang and AppleClang compilers
    # add_compile_options(-Wall -Wextra -Wpedantic)
else()
    # Settings for other compilers
    message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()