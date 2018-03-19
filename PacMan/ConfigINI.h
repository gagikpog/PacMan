#ifndef CONFIGINI_H
#define CONFIGINI_H

#include <string>
#include <fstream>

//����� ������������, ������� ��������� ������ ��������.
class ConfigINI
{
public:
	ConfigINI(const std::string& filename);
	~ConfigINI();
	//�������� ����.
	void clear();
	//�������� ������ �� ������� � ����� ���������.
	std::string getOptionToString(const std::string& section,const std::string& key);
	//�������� ����� �� ������� � ����� ���������.
	int getOptionToInt(const std::string& section,const std::string& key);
	//�������� ����� ������. ���������� ������ ��� ������� ������.
	bool addNewOption(const std::string& section,const std::string& key,const std::string& value);
	//�������� ����� ������. ���������� ������ ��� ������� ������.
	bool addNewOption(const std::string& section,const std::string& key,const int& value);
	//�������� ��������. ���������� ������ ��� ������� ������.
	bool updateOption(const std::string& section,const std::string& key,const std::string& value);
	//�������� ��������. ���������� ������ ��� ������� ������.
	bool updateOption(const std::string& section,const std::string& key,const int& value);
private:
	std::string fname;
	std::ofstream fout;
	std::ifstream fin;
};

#endif // !CONFIGINI_H