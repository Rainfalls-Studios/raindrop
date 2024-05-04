# Ensure C++11 standard is used
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all source files in the current directory
file(GLOB_RECURSE SOURCES *.cpp)

# Add the -fPIC flag to the compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
add_library(Prograd SHARED ${SOURCES})

# Determine which Raindrop library to link based on RAINDROP_BUILD_SHARED flag
if (RAINDROP_BUILD_SHARED)
    set(RAINDROP_LIB Raindrop-SHARED)
else()
    set(RAINDROP_LIB Raindrop-STATIC)
endif()

# Link libraries to the barebone target
target_link_libraries(
    Prograd
    PRIVATE
    ${RAINDROP_LIB}
    spdlog::spdlog_header_only
)

target_include_directories(
	Prograd
	PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
)

# Exclude system zlib library from linking
set_target_properties(
    Prograd
    PROPERTIES
    LINK_FLAGS "-Wl,--exclude-libs,ALL"
)

# Set output directory and other properties for the barebone target
set_target_properties(
    Prograd
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
    DEBUG_POSTFIX ""
    PREFIX ""
)

# Include the current binary directory for header files
target_include_directories(
    Prograd
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)
