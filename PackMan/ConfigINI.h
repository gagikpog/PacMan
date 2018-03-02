#pragma once
#include <string>
#include <fstream>

class ConfigINI
{
public:
	ConfigINI(std::string filename);
	~ConfigINI();
	void clear();
	std::string getOptionToString(std::string section,std::string key);
	int getOptionToInt(std::string section, std::string key);
	bool addNewOption(std::string section, std::string key, std::string value);
	bool addNewOption(std::string section, std::string key, int value);
	bool updateOption(std::string section, std::string key, std::string value);
	bool updateOption(std::string section, std::string key, int value);
private:
	std::string fname;
	std::ofstream fout;
	std::ifstream fin;
};