find_package(GTest REQUIRED)
include(GoogleTest)

file(GLOB_RECURSE SOURCES *.cpp)
add_executable(RaindropUnitTests ${SOURCES})

target_include_directories(RaindropUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(RaindropUnitTests Raindrop GTest::GTest GTest::Main GTest::gmock_main GTest::gmock)


set_target_properties(RaindropUnitTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
