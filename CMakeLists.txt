if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(
    CMAKE_TOOLCHAIN_FILE
    "C:\\Users\\kront\\tools\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"
    CACHE STRING
    "Vcpkg toolchain file"
  )
endif()


cmake_minimum_required(VERSION 3.10)
project(lab5_multithreading)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

add_executable(blur_app src/main.cpp src/blur.cpp)
target_include_directories(blur_app PRIVATE src)
target_link_libraries(blur_app PRIVATE ${OpenCV_LIBS})

add_executable(atomic_example src/atomic_example.cpp)
