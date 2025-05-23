cmake_minimum_required(VERSION 3.0.0)

project(ProgettoPCS2025 LANGUAGES C CXX VERSION 1.0.0)

# Set C++ Standard
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPOSE WARNINGS ON DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")

# IMPOSE CXX FLAGS FOR WINDOWS
if (WIN32)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif (WIN32)

# Add dependencies
################################################################################

## Eigen3
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND ProgettoPCS2025_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND ProgettoPCS2025_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)
list(APPEND ProgettoPCS2025_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND ProgettoPCS2025_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND ProgettoPCS2025_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND ProgettoPCS2025_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

# Add subdirectories (they must contain their own CMakeLists.txt)
add_subdirectory(src)
add_subdirectory(ExportParaview)
add_subdirectory(src_test)

# Collect sources, headers, includes
list(APPEND ProgettoPCS2025_sources ${src_sources} ${src_paraview_sources})  
list(APPEND ProgettoPCS2025_includes ${src_includes} ${src_paraview_includes})
list(APPEND ProgettoPCS2025_headers ${src_headers} ${src_paraview_headers})

# Collect sources, headers, includes exported from src
list(APPEND ProgettoPCS2025_sources ${PoliedriMesh_sources})
list(APPEND ProgettoPCS2025_headers ${PoliedriMesh_headers})
list(APPEND ProgettoPCS2025_includes ${PoliedriMesh_includes})

#Append test sources/includes
list(APPEND ProgettoPCS2025_TEST_sources ${${PROJECT_NAME}_sources} ${src_test_sources})
list(APPEND ProgettoPCS2025_TEST_headers ${${PROJECT_NAME}_headers} ${src_test_headers})
list(APPEND ProgettoPCS2025_TEST_includes ${${PROJECT_NAME}_includes} ${src_test_includes})


########################### Add Executable: ProgettoPCS2025 ##############################

add_executable(${PROJECT_NAME} 
    main.cpp 
    ${ProgettoPCS2025_sources}
    ${ProgettoPCS2025_headers}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${ProgettoPCS2025_includes}
    ${CMAKE_CURRENT_SOURCE_DIR}/PoliedriMesh
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${ProgettoPCS2025_LINKED_LIBRARIES})
target_compile_options(${PROJECT_NAME} PUBLIC -fPIC)

########################### Add Executable: ProgettoPCS2025_TEST ##############################

add_executable(${PROJECT_NAME}_TEST 
    main_test.cpp 
    ${ProgettoPCS2025_TEST_sources}
    ${ProgettoPCS2025_TEST_headers}
)

target_include_directories(${PROJECT_NAME}_TEST PUBLIC
    ${ProgettoPCS2025_TEST_includes}
    ${CMAKE_CURRENT_SOURCE_DIR}/PoliedriMesh
)

target_link_libraries(${PROJECT_NAME}_TEST PUBLIC ${ProgettoPCS2025_LINKED_LIBRARIES})
target_compile_options(${PROJECT_NAME}_TEST PUBLIC -fPIC)



