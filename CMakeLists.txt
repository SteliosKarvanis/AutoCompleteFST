cmake_minimum_required(VERSION 3.15)
project(FST)

set(CMAKE_CXX_STANDARD 17)
# Move bin to bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Include qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE SRCS src/*.cpp src/*.h)
set (SRCS_NO_GUI src/fst.cpp src/fst.cpp src/data_types.h src/utils.cpp src/utils.cpp src/levesteinDFA.cpp)

# Create executables
add_executable(app applications/app.cpp ${SRCS})
target_link_libraries(app Qt5::Widgets)

add_executable(autocomplete applications/autocomplete.cpp ${SRCS_NO_GUI})
add_executable(levestein applications/levestein.cpp ${SRCS_NO_GUI})
add_executable(levesteinDFA applications/levesteinDFA.cpp ${SRCS_NO_GUI})
add_executable(binary_search applications/binary_search.cpp ${SRCS_NO_GUI})
add_executable(write_fst applications/write_fst.cpp ${SRCS_NO_GUI})
add_executable(timer applications/time_counter.cpp ${SRCS_NO_GUI})
add_executable(indexes_analiser applications/index_analysis.cpp ${SRCS_NO_GUI})

# Package generation
set(CPACK_PACKAGE_NAME "autocomplete")
set(CPACK_PACKAGE_CONTACT "Samcbk")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5, libqt5core5a, libqt5gui5")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/../packages")
install(TARGETS autocomplete DESTINATION bin)
install(DIRECTORY data/ DESTINATION data)
include(CPack)
