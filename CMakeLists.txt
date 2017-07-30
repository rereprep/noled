cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_C_COMPILER "arm-vita-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-vita-eabi-g++")

project(HENkaku)

set(CMAKE_C_FLAGS "-Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

if (NOT ${RELEASE})
	add_definitions(-DENABLE_LOGGING)
endif()

add_executable(noled.elf
	main.c
)

target_link_libraries(noled.elf
	taihenForKernel_stub
	SceLedForDriver_stub
	ScePowerForDriver_stub
	SceGpioForDriver_stub
	)

set_target_properties(noled.elf
	PROPERTIES LINK_FLAGS "-nostdlib"
	COMPILE_FLAGS "-D__VITA_KERNEL__"
)

add_custom_target(noled.skprx ALL
	COMMAND vita-elf-create -e ${CMAKE_SOURCE_DIR}/noled.yml noled.elf noled.velf ${CMAKE_SOURCE_DIR}/taihen.json
	COMMAND vita-make-fself -c noled.velf noled.skprx
)
add_dependencies(noled.skprx noled.elf)

add_custom_target(send
	COMMAND curl -T noled.skprx ftp://$(PSVITAIP):1337/ux0:/data/tai/
	DEPENDS noled.skprx
)
