cmake_minimum_required(VERSION 3.10)
project(vnx)

if(NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT WIN32)
	add_compile_options(-Wall)
endif()

find_package(Threads REQUIRED)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	add_compile_options(-fdiagnostics-color=auto)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_FOUND}")
endif()

file(GLOB GENERATED_SOURCE_FILES generated/src/*.cpp)

if(WIN32)
	list(APPEND PLATFORM_SOURCE_FILES
		src/win32/Directory.cpp
		src/win32/Endpoint.cpp
		src/win32/Process.cpp
		src/win32/Terminal.cpp
		src/win32/File.cpp
		src/win32/Util.cpp
	)
else()
	list(APPEND PLATFORM_SOURCE_FILES
		src/linux/Directory.cpp
		src/linux/Endpoint.cpp
		src/linux/Process.cpp
		src/linux/Terminal.cpp
		src/linux/File.cpp
		src/linux/Util.cpp
	)
endif()

list(APPEND SRC_FILES
	${GENERATED_SOURCE_FILES}
	${PLATFORM_SOURCE_FILES}
	src/test/Test.cpp
	src/Authentication.cpp
	src/Binary.cpp
	src/Client.cpp
	src/JRPC_Client.cpp
	src/Config.cpp
	src/CRC64.cpp
	src/SHA256.cpp
	src/Input.cpp
	src/Module.cpp
	src/ModuleBase.cpp
	src/Node.cpp
	src/Output.cpp
	src/Pipe.cpp
	src/Process.cpp
	src/BaseProxy.cpp
	src/Proxy.cpp
	src/JRPC_Proxy.cpp
	src/Publisher.cpp
	src/Router.cpp
	src/BaseServer.cpp
	src/Server.cpp
	src/JRPC_Server.cpp
	src/SpyTool.cpp
	src/Subscriber.cpp
	src/Terminal.cpp
	src/TerminalInput.cpp
	src/Time.cpp
	src/TimeServer.cpp
	src/Topic.cpp
	src/Type.cpp
	src/Util.cpp
	src/Value.cpp
	src/Visitor.cpp
	src/Variant.cpp
	src/Memory.cpp
	src/Object.cpp
	src/Endpoint.cpp
	src/Buffer.cpp
	src/Thread.cpp
	src/File.cpp
	src/Recorder.cpp
	src/RecordWriter.cpp
	src/Player.cpp
	src/RecordReader.cpp
	src/MultiRecordReader.cpp
	src/AsyncClient.cpp
	src/ThreadPool.cpp
	src/Return.cpp
	src/LogMsg.cpp
	src/Generic.cpp
	src/GenericClient.cpp
	src/GenericAsyncClient.cpp
	src/InternalError.cpp
	src/Exception.cpp
	src/Session.cpp
	src/JSON.cpp
	src/Tuple.cpp
	src/ToObject.cpp
	src/PrettyPrinter.cpp
	src/ModuleInfo.cpp
	src/JRPC_Request.cpp
	src/JRPC_Response.cpp
	src/Stream.cpp
	src/Gateway.cpp
	src/RequestQueue.cpp
)

set(VNX_LIBRARY_TYPE "SHARED" CACHE STRING "VNX library build type (SHARED;STATIC)")
set_property(CACHE VNX_LIBRARY_TYPE PROPERTY STRINGS "SHARED;STATIC")

if(${VNX_LIBRARY_TYPE} STREQUAL "SHARED") 
	set(VNX_LIBRARY_NAME vnx_base)
else()
	set(VNX_LIBRARY_NAME vnx_base_static)
endif()

add_library(${VNX_LIBRARY_NAME} ${VNX_LIBRARY_TYPE} ${SRC_FILES} )

include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(${VNX_LIBRARY_NAME} BASE_NAME vnx_base)

target_include_directories(${VNX_LIBRARY_NAME}
	PUBLIC include generated/include ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${VNX_LIBRARY_NAME} Threads::Threads)

target_compile_definitions(${VNX_LIBRARY_NAME} PUBLIC NOGDI NOMINMAX)

if(${VNX_LIBRARY_TYPE} STREQUAL "STATIC") 
	target_compile_definitions(${VNX_LIBRARY_NAME} PUBLIC VNX_BASE_STATIC_DEFINE)
endif()


if(MSVC)
	include(FetchContent)
	FetchContent_Declare(dirent
		GIT_REPOSITORY https://github.com/tronkko/dirent.git
		GIT_TAG "a318cee5adaed3dec654773fd0a02723d844461c"
	)
	FetchContent_MakeAvailable(dirent)
	set(dirent_INCLUDE_DIR ${dirent_SOURCE_DIR}/include)
	target_include_directories(${VNX_LIBRARY_NAME} PUBLIC ${dirent_INCLUDE_DIR})
	
	if(${VNX_LIBRARY_TYPE} STREQUAL "SHARED") 
		set_target_properties(${VNX_LIBRARY_NAME} PROPERTIES ENABLE_EXPORTS ON)
		set_target_properties(${VNX_LIBRARY_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
	endif()

	target_link_libraries(${VNX_LIBRARY_NAME} ws2_32)
endif()

if(VNX_BUILD_TESTS)
	add_subdirectory(test)
endif()

if(VNX_BUILD_TOOLS)
	add_subdirectory(tools)
endif()

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)

if("${LIB64}" STREQUAL "TRUE")
    set(LIBSUFFIX 64)
else()
    set(LIBSUFFIX "")
endif()

install(DIRECTORY include/vnx generated/include/vnx DESTINATION include)
install(DIRECTORY interface/ modules/ DESTINATION interface/vnx)

install(TARGETS ${VNX_LIBRARY_NAME} DESTINATION lib${LIBSUFFIX})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vnx_base_export.h DESTINATION include)

