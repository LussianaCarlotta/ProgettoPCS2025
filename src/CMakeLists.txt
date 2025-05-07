list(APPEND poliedrMesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp)
list(APPEND poliedriMesh_sources ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp)

list(APPEND poliedriMesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/PoliedriMesh.hpp)

list(APPEND poliedriMesh_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(poliedriMesh_sources ${polygonalMesh_sources} PARENT_SCOPE)
set(poliedriMesh_headers ${polygonalMesh_headers} PARENT_SCOPE)
set(poliedriMesh_includes ${polygonalMesh_includes} PARENT_SCOPE)