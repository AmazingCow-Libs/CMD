cmake_minimum_required(VERSION 3.5)

##----------------------------------------------------------------------------##
## Project                                                                    ##
##----------------------------------------------------------------------------##
set(PROJECT_NAME CMD)
project(${PROJECT_NAME})


##----------------------------------------------------------------------------##
## Sources                                                                    ##
##----------------------------------------------------------------------------##
include_directories(CMD)

set(SOURCES
    CMD/src/Parser.cpp
    CMD/src/Flag.cpp
    CMD/src/FlagOptions.cpp
    CMD/src/private/Logger.cpp
)


##----------------------------------------------------------------------------##
## Library                                                                    ##
##----------------------------------------------------------------------------##
add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


##----------------------------------------------------------------------------##
## Dependecies                                                                ##
##----------------------------------------------------------------------------##
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC acow_cpp_goodies)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC CoreAssert      )
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC CoreString      )
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC CoreLog         )
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC libtermcolor    )
