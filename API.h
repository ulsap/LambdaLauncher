#pragma once
#include <string>
#include <map>
class API {


public: class API_FUNCTIONS {
private:
	static inline std::map<std::string, bool> Addons;
public:
	static void Set_Addons(std::string Addon, bool Status);
	static std::map<std::string, bool> Return_Addons();
	static std::map<std::string, bool> Get_Addons();
	static  void Save_Addon_Change();
};
};