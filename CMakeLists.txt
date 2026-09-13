cmake_minimum_required(VERSION 3.16)
project(morphological_dilation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(dilation_core
    src/binary_image.cpp
    src/structuring_element.cpp
    src/dilation.cpp
    src/erosion.cpp
    src/pipeline.cpp
)

target_include_directories(dilation_core PUBLIC include)

if(MSVC)
    target_compile_options(dilation_core PRIVATE /W4)
else()
    target_compile_options(
        dilation_core
        PRIVATE -Wall -Wextra -Wpedantic
    )
endif()

add_executable(dilation_cli src/main.cpp)
target_link_libraries(dilation_cli PRIVATE dilation_core)

if(MSVC)
    target_compile_options(dilation_cli PRIVATE /W4)
else()
    target_compile_options(
        dilation_cli
        PRIVATE -Wall -Wextra -Wpedantic
    )
endif()
