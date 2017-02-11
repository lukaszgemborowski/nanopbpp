project(nanopbpp)

cmake_minimum_required(VERSION 3.2)

set(NANOPB_HEADERS
	nanopb/pb.h
	nanopb/pb_decode.h
	nanopb/pb_encode.h)

set(NANOPBPP_HEADERS
	include/nanopbpp/basic_decoder.h
	include/nanopbpp/basic_encoder.h
	include/nanopbpp/extension.h
	include/nanopbpp/meta_decoder.h
	include/nanopbpp/meta_encoder.h
	include/nanopbpp/metadata.h)

add_library(nanopbpp
	nanopb/pb_common.c
	nanopb/pb_decode.c
	nanopb/pb_encode.c)

install(TARGETS nanopbpp DESTINATION lib)
install(FILES ${NANOPB_HEADERS} DESTINATION include)
install(FILES ${NANOPBPP_HEADERS} DESTINATION include/nanopbpp)
install(FILES include/nanopbpp/cpp/get_by_type.h DESTINATION include/nanopbpp/cpp)
