cmake_minimum_required(VERSION 3.8)
project(service_testing)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(THIS_PACKAGE_INCLUDE_DEPENDS
  rclcpp
  example_interfaces
)

find_package(ament_cmake REQUIRED)

foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

add_executable(client src/client.cpp)
ament_target_dependencies(client ${THIS_PACKAGE_INCLUDE_DEPENDS})

add_executable(server src/server.cpp)
ament_target_dependencies(server ${THIS_PACKAGE_INCLUDE_DEPENDS})

install(TARGETS
  client
  server
  DESTINATION lib/${PROJECT_NAME})

ament_package()
