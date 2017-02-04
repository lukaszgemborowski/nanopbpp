project(tests)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(NANOPB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../nanopb)

include_directories(
	${NANOPB_DIR}
	${GTEST_INCLUDE_DIRS})

set(NANOPB_SRC
	${NANOPB_DIR}/pb_common.c
	${NANOPB_DIR}/pb_decode.c
	${NANOPB_DIR}/pb_encode.c)

add_executable(tests
	nanopb_basic_tests.cc
	${NANOPB_SRC})

target_compile_features(tests PRIVATE cxx_range_for)
target_link_libraries(tests ${GTEST_MAIN_LIBRARIES} ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
