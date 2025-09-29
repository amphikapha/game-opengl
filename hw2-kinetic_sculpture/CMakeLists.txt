cmake_minimum_required(VERSION 3.10)
project(hw2_kinetic_sculpture VERSION 1.0 LANGUAGES C CXX)

# Inform user about vcpkg toolchain if provided
if(DEFINED CMAKE_TOOLCHAIN_FILE)
  message(STATUS "CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
else()
  message(WARNING "CMAKE_TOOLCHAIN_FILE not defined. If you use vcpkg, pass -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find packages (vcpkg will provide these if toolchain used)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glad CONFIG)
if(NOT glad_FOUND)
  find_package(glad REQUIRED)
endif()

# Configure root_directory.h with the correct project path
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/root_directory.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/root_directory.h @ONLY)

# Define sources
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.c src/*.cc)
file(GLOB_RECURSE HDR_FILES src/*.h src/*.hpp)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

# Link libraries
if (TARGET glfw::glfw)
  target_link_libraries(${PROJECT_NAME} PRIVATE glfw::glfw)
elseif (TARGET glfw)
  target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif()

if (TARGET glad::glad)
  target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
elseif (TARGET glad)
  target_link_libraries(${PROJECT_NAME} PRIVATE glad)
endif()

if (TARGET glm::glm)
  target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

# Copy files to build root directory (not Debug/Release subdirs)
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)

if(EXISTS ${SHADERS_DIR})
  file(GLOB SHADER_FILES ${SHADERS_DIR}/*)
  foreach(sfile IN LISTS SHADER_FILES)
    get_filename_component(fname ${sfile} NAME)
    configure_file(${sfile} ${CMAKE_BINARY_DIR}/shaders/${fname} COPYONLY)
  endforeach()
endif()

if(EXISTS ${RES_DIR})
  file(GLOB_RECURSE RES_FILES ${RES_DIR}/*)
  foreach(rfile IN LISTS RES_FILES)
    file(RELATIVE_PATH rel ${CMAKE_CURRENT_SOURCE_DIR} ${rfile})
    get_filename_component(outdir ${CMAKE_BINARY_DIR}/${rel} DIRECTORY)
    file(MAKE_DIRECTORY ${outdir})
    configure_file(${rfile} ${CMAKE_BINARY_DIR}/${rel} COPYONLY)
  endforeach()
endif()

# Also copy to Debug and Release directories for Visual Studio
foreach(config Debug Release RelWithDebInfo MinSizeRel)
    if(EXISTS ${SHADERS_DIR})
        file(GLOB SHADER_FILES ${SHADERS_DIR}/*)
        foreach(sfile IN LISTS SHADER_FILES)
            get_filename_component(fname ${sfile} NAME)
            configure_file(${sfile} ${CMAKE_BINARY_DIR}/${config}/shaders/${fname} COPYONLY)
        endforeach()
    endif()
    
    if(EXISTS ${RES_DIR})
        file(GLOB_RECURSE RES_FILES ${RES_DIR}/*)
        foreach(rfile IN LISTS RES_FILES)
            file(RELATIVE_PATH rel ${CMAKE_CURRENT_SOURCE_DIR} ${rfile})
            get_filename_component(outdir ${CMAKE_BINARY_DIR}/${config}/${rel} DIRECTORY)
            file(MAKE_DIRECTORY ${outdir})
            configure_file(${rfile} ${CMAKE_BINARY_DIR}/${config}/${rel} COPYONLY)
        endforeach()
    endif()
endforeach()

# Set working directory for Visual Studio debugger
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "Project configured. Sources: ${SRC_FILES}")