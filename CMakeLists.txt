cmake_minimum_required(VERSION 3.0.0)
project(gzipper VERSION 0.1.0)

# flags
set(COMPILE_TESTS ON)

include(CTest)
enable_testing()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(SOURCES ${SRC_DIR}/linux/HTTPSocket.cpp
            ${SRC_DIR}/URLParser.cpp)



if(${COMPILE_TESTS})
  # from gtest docs

  configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif()

  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                  ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                  EXCLUDE_FROM_ALL)

  
  ## INDIVIDUAL TEST ##
  # test files

  ## TEST LISTENER ##
  add_library(TestListener ${TEST_DIR}/linux/TestListener.cpp)
  target_include_directories(TestListener PRIVATE ${INC_DIR})
                  
  add_executable(URLParser_test ${SRC_DIR}/URLParser.cpp ${TEST_DIR}/URLParser_test.cpp)
  target_include_directories(URLParser_test PRIVATE ${INC_DIR})
  target_link_libraries(URLParser_test gtest_main)
  add_test(NAME URLParserTest COMMAND URLParser_test)

  add_executable(HTTPSocket_test ${SOURCES} ${TEST_DIR}/HTTPSocket_test.cpp)
  target_include_directories(HTTPSocket_test PRIVATE ${INC_DIR})
  target_link_libraries(HTTPSocket_test TestListener gtest_main)
  add_test(NAME HTTPSocketTest COMMAND HTTPSocket_test)

  # test socket

endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
