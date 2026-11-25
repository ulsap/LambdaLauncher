#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include "API.h"
#include <sstream>

		 std::map<std::string, bool> API::API_FUNCTIONS::Return_Addons(){
			 return Addons;
		 }
		 void API::API_FUNCTIONS::Set_Addons(std::string Addon, bool Status) {
			 Addons[Addon] = Status;
		};

		 std::map<std::string, bool> API::API_FUNCTIONS::Get_Addons(){
			int i = 0;
			char buffer[1000] = {};
			DWORD sz = sizeof(buffer);
		    RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Valve\\Steam", "InstallPath", RRF_RT_ANY, NULL, buffer, &sz);
			std::string path(buffer);
			std::string ParseTo;
			std::ifstream file(path +  "\\steamapps\\common\\Lambda Wars\\lambdawars\\addonlist.txt");
			
			while (std::getline(file, ParseTo)) {
				if (i++ < 2 ) {
					continue;
				}
				if (ParseTo == "}") {
					continue;
				}
				std::string Key, Value;
				std::istringstream parse(ParseTo);
				
				parse >> std::quoted(Key) >> std::quoted(Value);
				Addons[Key] = std::stoi(Value);
			};
			//ParseTo << file;
			//MessageBoxA(NULL, , "INFO", MB_ICONINFORMATION);
			return Addons;
		};

		 void API::API_FUNCTIONS::Save_Addon_Change() {
			 int i = 0;
			 char buffer[1000] = {};
			 DWORD sz = sizeof(buffer);
			 RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Valve\\Steam", "InstallPath", RRF_RT_ANY, NULL, buffer, &sz);
			 std::string path(buffer);
			 std::ofstream file(path + "\\steamapps\\common\\Lambda Wars\\lambdawars\\addonlist.txt");
			 std::string Addon = "\"Addonlist\"\n {\n ";
			 for (const auto& [key, value] : Addons) {
				 Addon.append("\"" + key + "\"");
				 Addon.append(" \"");
				 Addon.append(std::to_string(value) + "");
				 Addon.append("\"\n");
			 }
			 Addon.append("}");
			 file << Addon;

		 };