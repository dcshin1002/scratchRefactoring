cmake_minimum_required(VERSION 3.0.0)

#
# project settings
#
project(myRefactoring VERSION 0.1.0)

#
# find pthread for googletest
#
# find_package(Threads REQUIRED)

#
# import googletest as an external project
#
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(
  COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(
  COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines the gtest and gtest_main
# targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we have to add them
# here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

#
# create common variable
#
add_definitions("-DPATH_TO_JSON=\"${PROJECT_SOURCE_DIR}/json\"")
file(GLOB MY_SRCS 
    ${PROJECT_SOURCE_DIR}/src/Main.cpp
    ${PROJECT_SOURCE_DIR}/src/Statement.cpp
    ${PROJECT_SOURCE_DIR}/json/jsoncpp/jsoncpp.cpp)
set(MY_INCLUDES ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/json/jsoncpp)


#
# Enable testing for the project
#
enable_testing()

#
# sub directories
#
add_subdirectory(src)
add_subdirectory(test)
