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

file(GLOB_RECURSE SRCS src/*.cpp src/*.h)
set (SRCS_NO_GUI src/fst.h src/fst.cpp src/data_types.h src/utils.cpp src/utils.h)

# Create executables
add_executable(autocomplete main.cpp ${SRCS})
target_link_libraries(autocomplete Qt5::Widgets)

add_executable(test_autocomplete test.cpp ${SRCS_NO_GUI})
add_executable(test_levestein main_levestein.cpp ${SRCS_NO_GUI})
add_executable(test_binary_search binary_search.cpp ${SRCS_NO_GUI})

# Package generation
set(CPACK_PACKAGE_NAME "autocomplete")
set(CPACK_PACKAGE_CONTACT "Samcbk")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5, libqt5core5a, libqt5gui5")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/../packages")
install(TARGETS autocomplete DESTINATION bin)
install(DIRECTORY data/ DESTINATION data)
include(CPack)
