file(GLOB_RECURSE SOURCES *.cpp)
add_executable(RaindropTestbed ${SOURCES})

target_include_directories(RaindropTestbed PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(RaindropTestbed Raindrop)

set_target_properties(RaindropTestbed PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_include_directories(RaindropTestbed PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
set(RESOURCE_DIR "\"${CMAKE_CURRENT_SOURCE_DIR}\"")

configure_file(config.hpp.in config.hpp)