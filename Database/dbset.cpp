//-----------------------------------------------------------------------------
// File: dbset.cpp
// Auth: SnipGhost
//                                              Реализация методов класса DBSet
//-----------------------------------------------------------------------------
#include "dbset.h"
//-----------------------------------------------------------------------------
using namespace Kernel;
//-----------------------------------------------------------------------------
DBSet::DBSet()
{
	dbName = "";
	showMsg(2, "Пустая база данных " + dbName + " успешно создана");
}
//-----------------------------------------------------------------------------
DBSet::~DBSet()
{
	for (auto it = tables.begin(); it != tables.end(); ++it)
		delete it->second;
	showMsg(2, "База данных успешно очищена из памяти");
}
//-----------------------------------------------------------------------------
DBSet::DBSet(string path)
{
	if (!readTableSet(path)) {
		dbName = "";
		tables.clear();
	}
	showMsg(2, "База данных " + dbName + " успешно создана");
}
//-----------------------------------------------------------------------------
bool DBSet::isTableExist(string name)
{
	if (tables.count(name) < 1) {
		showMsg(0, "Обращение к несуществующей таблице: " + name);
		return 0;
	}
	return 1;
}
//-----------------------------------------------------------------------------
size_t DBSet::getSize()
{
	return tables.size();
}
//-----------------------------------------------------------------------------
DBTable& DBSet::operator[](string name)
{
	if (!isTableExist(name)) exit(0); // Критический сбой
	return *tables[name];
}
//-----------------------------------------------------------------------------
DBTable& DBSet::operator[](size_t index)
{
	auto it = tables.begin(); // TODO: Переделать эти ужасные костыли
	index %= tables.size();
	while (index) {
		--index;
		++it;
	}
	return *it->second;
}
//-----------------------------------------------------------------------------
bool DBSet::readTableSet(string path)
{
	ifstream fin(path);
	if (fin.fail()) {
		showMsg(0, "Ошибка открытия файла " + path);
		return 0;
	}
	getline(fin, dbName);
	string line;
	showMsg(0, "Начал загрузку таблиц в БД " + dbName);
	while (getline(fin, line)) 
		createTable(line);
	fin.close();
	showMsg(2, "Загрузка таблиц в БД " + dbName + " успешно завершена");
	return 1;
}
//-----------------------------------------------------------------------------
bool DBSet::createTable(string path)
{
	char line[MAX_LINE], *token_name;
	strcpy(line, path.c_str());
	token_name = strtok(line, "."); // TODO: Переделать для поддержи точек в пути
	if (strlen(token_name) < 1) {
		showMsg(2, "Какой-то кривой путь: " + path);
		return 0;
	}
	DBTable *tab = new DBTable(path);
	tables.insert(make_pair(string(token_name), tab));
	showMsg(2, "Таблица успешно добавлена");
	return 1;
}
//-----------------------------------------------------------------------------