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

std::string ConfigINI::getOptionToString(std::string section, std::string key)
{
	fin.open(fname);
	if (fin.is_open())
	{
		std::string res,sect;
		int p,com;
		while (!fin.eof())
		{
			std::getline(fin,res);
			com = res.find_first_of(";#");
			if (com > 0)
				res = res.substr(0, com);
			
			if (res.find_first_of("[") == 0)
			{	
				sect = res;
				continue;
			}
			p = res.find(key);
			if (p == 0 && sect == "[" + section + "]")
				break;
		}
		fin.close();
		if (p == 0)
		{
			return res.substr(key.size() + 1);
		}
		else return "NAN";
	}
	this->clear();	
	return "NAN";
}

int ConfigINI::getOptionToInt(std::string section, std::string key)
{
	std::string res = getOptionToString(section,key);
	return std::atoi(res.c_str());
}

bool ConfigINI::addNewOption(std::string section, std::string key, std::string value)
{
	bool b = false;
	std::string res = getOptionToString(section,key);
	if (res != "NAN")
		return b;

	fin.open(fname);
	if (fin.is_open()) 
	{
		std::string res, sect,output;
		int p;
		while (!fin.eof())
		{
			std::getline(fin, res);
			if (res == "")continue;
			if (res.find_first_of('[') == 0)
			{
				output += res + "\n";
				sect = res.substr(0, res.find_first_of(']') + 1);
				if (sect == "[" + section + "]")
				{
					b = true;
					output += key + " " + value + "\n";
				}
				continue;
			}
			output += res + "\n";
		}
		if (!b)
		{
			output += "[" + section + "]\n" + key + " " + value + "\n";
			b = true;
		}
		fin.close();
		fout.open(fname);
		if (fout.is_open())
		{
			fout << output;
			fout.close();
		}else b = false;
	}
	return b;
}

bool ConfigINI::addNewOption(std::string section, std::string key, int value)
{
	return this->addNewOption(section,key, std::to_string(value));
}

bool ConfigINI::updateOption(std::string section, std::string key, std::string value)
{
	std::string output;
	fin.open(fname);
	bool b = false;
	if (fin.is_open())
	{
		std::string res, sect;
		int p;
		while (!fin.eof())
		{
			std::getline(fin, res);
			if (res == "")continue;
			if (res.find_first_of('[') == 0)
			{
				output += res + "\n";
				sect = res.substr(0, res.find_first_of(']') + 1);
				continue;
			}
			if (sect != "[" + section + "]")
			{
				output += res + "\n";
				continue;
			}
			p = res.find(key);
			if (p == 0)
			{
				b = true;
				int com = res.find_first_of(";#");
				if (com > 0)
					res = key + " " + value + res.substr(com);
				else res = key + " " + value;
			}
			output += res + "\n";
		}
		fin.close();
		fout.open(fname);
		if (fout.is_open())
		{
			fout << output;
			fout.close();
		}
	}
	return b;
}

bool ConfigINI::updateOption(std::string section, std::string key, int value)
{
	return this->updateOption(section,key, std::to_string(value));
}