# Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.5)

project(cyclonedds_cmake_module)

find_package(ament_cmake REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package(
  CONFIG_EXTRAS "cyclonedds_cmake_module-extras.cmake"
)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
find_package(CycloneDDS)

if (NOT CycloneDDS_FOUND)
  include(ExternalProject)
  ExternalProject_Add(cyclonedds
    PREFIX cyclonedds
    URL https://github.com/eclipse-cyclonedds/cyclonedds/archive/master.zip
    CMAKE_ARGS
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/cyclonedds_install
      -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/cyclonedds_install"
    CONFIGURE_COMMAND cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} ../cyclonedds/src
    BUILD_COMMAND cmake --build .
    INSTALL_COMMAND cmake --build . --target install
    )
  # The external project will install to the build folder, but we'll install that on make install.
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cyclonedds_install/ DESTINATION ${CMAKE_INSTALL_PREFIX})
else()
  message(STATUS "Found CycloneDDS.")
endif()

install(DIRECTORY cmake
  DESTINATION share/${PROJECT_NAME})
