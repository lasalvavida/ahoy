cmake_minimum_required (VERSION 3.1.0)

set(PROJECT_NAME ahoy)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS_OFF)

project(${PROJECT_NAME})

# cmake -DTEST_AHOY=ON to build with tests
option(TEST_AHOY "Build tests for Ahoy" OFF)

# ahoy
set(AHOY_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${AHOY_INCLUDE_DIRS})
file(GLOB LIB_HEADERS "include/ahoy/*.h")
file(GLOB LIB_SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} ${LIB_HEADERS} ${LIB_SOURCES})

if(TEST_AHOY)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  enable_testing()

  # gtest
  include_directories(dependencies/googletest/googletest/include)
  add_subdirectory(dependencies/googletest/googletest)

  # Unit Tests
  include_directories(test/include)
  file(GLOB TEST_HEADERS "test/include/*.h")
  file(GLOB TEST_SOURCES "test/src/*.cpp")

  add_executable(${PROJECT_NAME}-test ${TEST_HEADERS} ${TEST_SOURCES})
  target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME} gtest)

  add_test(ParserTest ${PROJECT_NAME}-test)
endif()
