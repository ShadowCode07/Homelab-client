cmake_minimum_required (VERSION 3.21)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" 
	CACHE STRING "Vcpkg toolchain file")


project (HomelabClient VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

find_package(PahoMqttCpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Sources are listed explicitly: add new files here.
set(HomelabClient_SOURCES
		src/main.cpp
		src/mqttConnection.cpp
		src/mqttConnection.h
		src/helpers.h
		src/agentConfig.cpp
		src/agentConfig.h
)

add_executable(HomelabClient ${HomelabClient_SOURCES})

target_include_directories(HomelabClient PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(HomelabClient PRIVATE
		PahoMqttCpp::paho-mqttpp3
		nlohmann_json::nlohmann_json
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
