#pragma once
#include <string>
#include <fstream>

//Класс конфигурации, который сохраняет данные настроек.
class ConfigINI
{
public:
	ConfigINI(std::string filename);
	~ConfigINI();
	//Очистить файл.
	void clear();
	//Получить строку по сектору и ключу настройки.
	std::string getOptionToString(std::string section,std::string key);
	//Получить число по сектору и ключу настройки.
	int getOptionToInt(std::string section, std::string key);
	//Добавить новую запись. Возвращает истину при удачной записи.
	bool addNewOption(std::string section, std::string key, std::string value);
	//Добавить новую запись. Возвращает истину при удачной записи.
	bool addNewOption(std::string section, std::string key, int value);
	//Изменить значение. Возвращает истину при удачной записи.
	bool updateOption(std::string section, std::string key, std::string value);
	//Изменить значение. Возвращает истину при удачной записи.
	bool updateOption(std::string section, std::string key, int value);
private:
	std::string fname;
	std::ofstream fout;
	std::ifstream fin;
};