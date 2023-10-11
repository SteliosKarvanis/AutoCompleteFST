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

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

# Create executable
add_executable(autocomplete ${SOURCES} ${HEADERS})
target_link_libraries(autocomplete Qt5::Widgets)

