
add_executable(echo_crc64_table echo_crc64_table.cpp)
add_executable(vnxdump vnxdump.cpp)
add_executable(vnxplay vnxplay.cpp)
add_executable(vnxread vnxread.cpp)
add_executable(vnxreadcsv vnxreadcsv.cpp)
add_executable(vnxrecord vnxrecord.cpp)
add_executable(vnxrouter vnxrouter.cpp)
add_executable(vnxtimeserver vnxtimeserver.cpp)
add_executable(vnxclose vnxclose.cpp)
add_executable(vnxtopic vnxtopic.cpp)
add_executable(vnxgraph vnxgraph.cpp)
add_executable(vnxservice vnxservice.cpp)
add_executable(vnxpasswd vnxpasswd.cpp)

target_link_libraries(echo_crc64_table vnx_base)
target_link_libraries(vnxdump vnx_base)
target_link_libraries(vnxplay vnx_base)
target_link_libraries(vnxread vnx_base)
target_link_libraries(vnxreadcsv vnx_base)
target_link_libraries(vnxrecord vnx_base)
target_link_libraries(vnxrouter vnx_base)
target_link_libraries(vnxtimeserver vnx_base)
target_link_libraries(vnxclose vnx_base)
target_link_libraries(vnxtopic vnx_base)
target_link_libraries(vnxgraph vnx_base)
target_link_libraries(vnxservice vnx_base)
target_link_libraries(vnxpasswd vnx_base)

install(TARGETS
	vnxrouter
	vnxclose
	vnxdump
	vnxread
	vnxreadcsv
	vnxtimeserver
	vnxtopic
	vnxrecord
	vnxplay
	vnxgraph
	vnxservice
	vnxpasswd
	DESTINATION bin
)
