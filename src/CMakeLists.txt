list(APPEND PoliedriMesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp)
list(APPEND PoliedriMesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/PoliedriMesh.hpp)
list(APPEND PoliedriMesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/Traingolazione.hpp)

list(APPEND PoliedriMesh_sources ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp)
list(APPEND PoliedriMesh_sources ${CMAKE_CURRENT_SOURCE_DIR}/Traingolazione.cpp)

list(APPEND PoliedriMesh_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(PoliedriMesh_sources ${PoliedriMesh_sources} PARENT_SCOPE)
set(PoliedriMesh_headers ${PoliedriMesh_headers} PARENT_SCOPE)
set(PoliedriMesh_includes ${PoliedriMesh_includes} PARENT_SCOPE)