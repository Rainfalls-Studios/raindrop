#ifndef __RAINDROP_COMMON_HPP__
#define __RAINDROP_COMMON_HPP__

#include <glm/glm.hpp>
#include <easylogging++.h>
#include <tinyxml2.h>
#include <imgui/imgui.h>

#include <memory>
#include <functional>
#include <algorithm>
#include <utility>
#include <filesystem>
#include <fstream>
#include <regex>
#include <typeinfo>

#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <array>
#include <queue>
#include <deque>
#include <bitset>
#include <any>

#include <thread>
#include <mutex>
#include <barrier>
#include <shared_mutex>
#include <semaphore>

#ifdef RAINDROP_EDITOR
	#define RAINDROP_EDITOR_DIR std::filesystem::path(RAINDROP_EDITOR_PATH)
#endif

#endif