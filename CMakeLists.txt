
cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{PSN00BSDK_LIBS})
	set(CMAKE_TOOLCHAIN_FILE $ENV{PSN00BSDK_LIBS}/cmake/sdk.cmake)
endif()

project(
	PSn00bSDK-template
	LANGUAGES    C CXX ASM
	VERSION      1.0.0
	DESCRIPTION  "PSn00bSDK template"
	HOMEPAGE_URL "http://lameguy64.net/?page=psn00bsdk"
)




psn00bsdk_add_executable(
	template STATIC
	main.c
	texture.c
	data.s
	
)

psn00bsdk_add_cd_image(
	iso      # Target name
	template # Output file name (= template.bin + template.cue)
	iso.xml  # Path to config file
	DEPENDS template
)
