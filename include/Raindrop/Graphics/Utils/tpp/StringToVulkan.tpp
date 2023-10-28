template<typename T>
T StringToVulkan::toEnum(const std::string& str, T d = {}){
	return static_cast<T>(toEnum(str, static_cast<uint32_t>(d)));
}