
file(GLOB GENERATED_SOURCE_FILES generated/src/*.cpp)

add_library(vnx_test SHARED
	${GENERATED_SOURCE_FILES}
)

include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(vnx_test)

target_include_directories(vnx_test
	PUBLIC generated/include ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(vnx_test PUBLIC VNX_TEST_EXPORT_ENABLE)

if(MSVC)
	set_target_properties(vnx_test PROPERTIES ENABLE_EXPORTS ON)
	set_target_properties(vnx_test PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

target_link_libraries(vnx_test vnx_base)

add_executable(test_input_output test_input_output.cpp)
target_link_libraries(test_input_output vnx_test)

add_executable(test_variant test_variant.cpp)
target_link_libraries(test_variant vnx_test)

add_executable(test_config test_config.cpp)
target_link_libraries(test_config vnx_test)

add_executable(test_json_parse test_json_parse.cpp)
target_link_libraries(test_json_parse vnx_base)
