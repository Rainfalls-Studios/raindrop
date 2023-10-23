file(GLOB SOURCES *.cpp)
add_library(Raindrop STATIC ${SOURCES})

find_package(Boost REQUIRED COMPONENTS Filesystem)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)

include_directories(
	"${CMAKE_SOURCE_DIR}/include"
	BOOST_INCLUDE_DIRS
	GLM_INCLUDE_DIRS
	SPDLOG_INCLUDE_DIRS
)

add_subdirectory(Core)

target_link_libraries(Raindrop PRIVATE
	Raindrop::Core
	Boost::filesystem
	spdlog::spdlog
	glm
)