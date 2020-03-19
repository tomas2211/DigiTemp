cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME eink_weather_disp)

set(${PROJECT_NAME}_BOARD pro)
set(ARDUINO_CPU 8MHzatmega328)
project(${PROJECT_NAME})


### Additional settings to add non-standard or your own Arduino libraries.
# For this example (libs will contain additional arduino libraries)
# An Arduino library my_lib will contain files in libs/my_lib/: my_lib.h, my_lib.cpp + any other cpp files
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/epd2in9)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../arduino_libs)

### Additional static libraries to include in the target.
#set(${PROJECT_NAME}_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs/epd2in9/epd2in9.cpp ${CMAKE_CURRENT_SOURCE_DIR}/libs/epd2in9/epd2in9.h)


### Main sketch file
set(${PROJECT_NAME}_SKETCH eink_weather_disp.ino)

### Add project directories into the build
# add_subdirectory()

#set(${PROJECT_NAME}_LIBS epd2in9)
#set(epd2in9_RECURSE True)


#### Additional settings for programmer. From programmers.txt
set(${PROJECT_NAME}_PROGRAMMER arduinoasisp)
set(${PROJECT_NAME}_PORT /dev/ttyUSB0)
# set(pro.upload.speed 9600)

## Verbose build process
set(${PROJECT_NAME}_AFLAGS -v)

generate_arduino_firmware(${PROJECT_NAME})

#target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/epd2in9)
#target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})
#
#target_include_directories(${PROJECT_NAME} PUBLIC
#        "${PROJECT_BINARY_DIR}"
#        ${EXTRA_INCLUDES}
#        )

print_board_list()
print_programmer_list()
