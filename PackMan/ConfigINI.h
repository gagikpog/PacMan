#pragma once
#include <string>
#include <fstream>

class ConfigINI
{
public:
	ConfigINI(std::string filename);
	~ConfigINI();
	void clear();
	std::string getOptionToString(std::string key);
	int getOptionToInt(std::string key);
	void addNewOption(std::string key, std::string value);
	void addNewOption(std::string key, int value);
	void updateOption(std::string key, std::string value);
	void updateOption(std::string key, int value);
private:
	std::string fname;
	std::ofstream fout;
	std::ifstream fin;
};