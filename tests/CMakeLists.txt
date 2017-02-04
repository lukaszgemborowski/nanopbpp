project(tests)
cmake_minimum_required(VERSION 3.2)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(NANOPB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../nanopb)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(test-pb ${BUILD_DIR}/test.pb)
set(test-pb-c ${TEST_DIR}/test.pb.c)

add_custom_command(
	OUTPUT ${test-pb-c}
	COMMAND python ${NANOPB_DIR}/generator/nanopb_generator.py ${test-pb}
	COMMAND cp ${BUILD_DIR}/test.pb.? ${TEST_DIR}/
	MAIN_DEPENDENCY ${test-pb})

add_custom_command(
	OUTPUT ${test-pb}
	COMMAND protoc -o ${test-pb} ${TEST_DIR}/test.proto -I ${TEST_DIR} -I ${NANOPB_DIR}/generator/proto
	MAIN_DEPENDENCY ${TEST_DIR}/test.proto)

include_directories(
	${ROOT_DIR}/include
	${NANOPB_DIR}
	${GTEST_INCLUDE_DIRS})

set(NANOPB_SRC
	${NANOPB_DIR}/pb_common.c
	${NANOPB_DIR}/pb_decode.c
	${NANOPB_DIR}/pb_encode.c)

add_executable(tests
	test.pb.c
	nanopb_basic_tests.cc
	basic_encoder_tests.cc
	basic_decoder_tests.cc
	${test-pb-c}
	${NANOPB_SRC})

target_compile_features(tests PRIVATE cxx_range_for)
target_link_libraries(tests ${GTEST_MAIN_LIBRARIES} ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
