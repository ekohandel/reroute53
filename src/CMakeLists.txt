cmake_minimum_required(VERSION 3.18)

add_subdirectory(interfaces)
add_subdirectory(external_ip)
add_subdirectory(aws_route53_dns_updater)

set(
	TARGET_NAME
		reroute53
)

add_executable(
	${TARGET_NAME}
		${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_link_libraries(
	${TARGET_NAME}
	PRIVATE
		external_ip
		aws_route53_dns_updater
)

target_include_directories(
	${TARGET_NAME}
	PRIVATE
		external_ip
		aws_route53_dns_updater
)
