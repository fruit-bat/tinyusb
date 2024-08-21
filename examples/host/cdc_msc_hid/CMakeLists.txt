cmake_minimum_required(VERSION 3.17)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../hw/bsp/family_support.cmake)

# gets PROJECT name for the example (e.g. <BOARD>-<DIR_NAME>)
family_get_project_name(PROJECT ${CMAKE_CURRENT_LIST_DIR})

project(${PROJECT} C CXX ASM)

# Checks this example is valid for the family and initializes the project
family_initialize_project(${PROJECT} ${CMAKE_CURRENT_LIST_DIR})

# Espressif has its own cmake build system
if(FAMILY STREQUAL "espressif")
  return()
endif()

add_executable(${PROJECT})

target_compile_definitions(${PROJECT} PRIVATE
	HID_PROTOCOL_DEFAULT=HID_PROTOCOL_REPORT
)

# Example source
target_sources(${PROJECT} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cdc_app.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hid_app.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hid_host_utils.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hid_host_info.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hid_host_common.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hid_host_mouse.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hid_host_joy.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/msc_app.c
  )

# Example include
target_include_directories(${PROJECT} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

# Configure compilation flags and libraries for the example without RTOS.
# See the corresponding function in hw/bsp/FAMILY/family.cmake for details.
family_configure_host_example(${PROJECT} noos)
