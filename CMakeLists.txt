cmake_minimum_required (VERSION 3.21)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" 
	CACHE STRING "Vcpkg toolchain file")


project (HomelabClient VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

find_package(PahoMqttCpp CONFIG REQUIRED)

add_executable(HomelabClient
		src/mqttConnection.cpp
		src/mqttConnection.h
		src/helpers.h)

file(GLOB_RECURSE HomelabClient_SOURCES	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)

target_sources(HomelabClient PRIVATE ${HomelabClient_SOURCES})

target_include_directories(HomelabClient PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(HomelabClient PRIVATE
	MY_TEST_DEFINE
)

target_link_libraries(HomelabClient PRIVATE
	PahoMqttCpp::paho-mqttpp3
)

target_link_directories(HomelabClient PRIVATE

)

if(MSVC)
	target_compile_options(HomelabClient PRIVATE
		/W3
		/MP
		/WX
		/ZI
	)
else()
	target_compile_options(HomelabClient PRIVATE
		-Wall
		-Wextra
		-Wpedantic
	)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT HomelabClient)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${HomelabClient_SOURCES})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
