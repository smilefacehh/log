cmake_minimum_required(VERSION 3.0.0)
project(Nlog)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wall -Werror -Wno-error=unused-variable -O3 -fPIC)

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src "*.cpp")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
#add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

install(DIRECTORY ./src/
  DESTINATION ./include/Nlog
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include)

install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Config.cmake
  DESTINATION cmake)