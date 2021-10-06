find_package(GTest REQUIRED)
include(GoogleTest)

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/tests/)

add_subdirectory(unit)
#add_subdirectory(integration)
#add_subdirectory(mocks)
