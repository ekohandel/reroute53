cmake_minimum_required(VERSION 3.18)

include(FindPkgConfig)
pkg_check_modules(
	CURLPP
	REQUIRED
	curlpp
)

set(
	TARGET_NAME
		external_ip
)
add_library(
	${TARGET_NAME}
	STATIC
		${CMAKE_CURRENT_LIST_DIR}/my_external_ip.cpp
)
target_include_directories(
	${TARGET_NAME}
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}
		${CURLPP_INCLUDE_DIRS}
	PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(
	${TARGET_NAME}
	PRIVATE
		${CURLPP_LDFLAGS}
	PUBLIC
		external_ip_interface
)