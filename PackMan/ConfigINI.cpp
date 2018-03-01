#include "ConfigINI.h"

ConfigINI::ConfigINI(std::string filename)
{
	fname = filename;
}

ConfigINI::~ConfigINI()
{
}

void ConfigINI::clear()
{
	fout.open(fname);
	fout << "[INI]";
	fout.close();
}

std::string ConfigINI::getOptionToString(std::string key)
{
	fin.open(fname);
	if (fin.is_open())
	{
		std::string res;
		int p;
		while (!fin.eof())
		{
			std::getline(fin,res);
			p = res.find(key);
			if (p == 0)
				break;
		}
		fin.close();
		if (p == 0)
			return res.substr(key.size()+1);
		else return "NAN";
	}
	this->clear();	
	return "NAN";
}

int ConfigINI::getOptionToInt(std::string key)
{
	std::string res = getOptionToString(key);
	return std::atoi(res.c_str());
}

void ConfigINI::addNewOption(std::string key, std::string value)
{
	std::string res = getOptionToString(key);
	if (res != "NAN")
		return;
	fout.open(fname,fout.app);
	if (fout.is_open())
		fout << std::endl << key << " " << value;
	fout.close();
}

void ConfigINI::addNewOption(std::string key, int value)
{
	this->addNewOption(key, std::to_string(value));
}

void ConfigINI::updateOption(std::string key, std::string value)
{
	std::string output;
	fin.open(fname);
	if (fin.is_open())
	{
		std::string res;
		int p;
		while (!fin.eof())
		{
			std::getline(fin, res);
			if (res == "")continue;
			p = res.find(key);
			if (p == 0)
			{
				res = key + " " + value;
			}
			output += res+"\n";
		}
		fin.close();
		fout.open(fname);
		if (fout.is_open())
		{
			fout << output;
			fout.close();
		}
	}
}

void ConfigINI::updateOption(std::string key, int value)
{
	this->updateOption(key, std::to_string(value));
}