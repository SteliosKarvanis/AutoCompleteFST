project(autocomplete)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
# Move bin to bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Include qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)



# Create executables
add_executable(autocomplete main.cpp gui.cpp gui.h fst.h fst.cpp data_types.h sort.cpp)
target_link_libraries(autocomplete Qt5::Widgets)

add_executable(test_autocomplete test.cpp fst.h fst.cpp data_types.h sort.cpp)
add_executable(test_levestein main_levestein.cpp fst.h fst.cpp data_types.h sort.cpp)
