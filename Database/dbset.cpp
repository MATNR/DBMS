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
bool DBSet::dropTable(string name)
{
	if (isTableExist(name)) {
		tables.erase(name);
		return 0;
	} else {
		showMsg(1, "Таблица " + name + " не найдена");
		return 1;
	}
}
//-----------------------------------------------------------------------------
DBTable* DBSet::selectData(string line, string exp)
{
	DBTable *result = NULL;
	char buf[MAX_LINE], *token, *last_token;
	char *delims1 = ", |", *delims2 = ".:=";
	strcpy(buf, line.c_str());
	token = strtok_s(buf, delims1, &last_token);
	Header head;
	vector<pair<string, string>> tabCols;
	vector<string> tabNames;
	while (token != NULL)
	{
		string column = string(token);
		string tabName = string(strtok(token, delims2));
		string tabCol = string(strtok(NULL, delims2));
		tabCols.push_back(make_pair(tabName, tabCol));
		size_t i;
		for (i = 0; i < tabNames.size(); ++i)
			if (tabNames[i] == tabName) break;
		if (i == tabNames.size()) tabNames.push_back(tabName);
		string type = tables[tabName]->colHeaders[tabCol];
		token = strtok_s(NULL, delims1, &last_token);
		head[column] = type;
	}
	vector<pair<pair<string, string>, pair<string, string>>> expr;
	strcpy(buf, exp.c_str());
	token = strtok_s(buf, delims1, &last_token);
	while (token != NULL)
	{
		string tn1 = string(strtok(token, delims2));
		string cn1 = string(strtok(NULL, delims2));
		string tn2 = string(strtok(NULL, delims2));
		string cn2 = string(strtok(NULL, delims2));
		expr.push_back(make_pair(make_pair(tn1, cn1), make_pair(tn2, cn2)));
		token = strtok_s(NULL, delims1, &last_token);
	}
	result = new DBTable(head);
	DBTable *t1 = tables[expr[0].first.first];
	string r1 = expr[0].first.second;
	DBTable *t2 = tables[expr[0].second.first];
	string r2 = expr[0].second.second;
	for (size_t i = 0; i < tables[tabNames[0]]->getSize(); ++i)
	{
		for (size_t j = 0; j < tables[tabNames[1]]->getSize(); ++j)
		{
			string s1 = extValue(t1->colHeaders[r1], t1->records[i][r1]);
			string s2 = extValue(t2->colHeaders[r2], t2->records[j][r2]);
			showMsg(4, s1 + " // " + s2);
			showMsg(4, t1->colHeaders[r1]);
			showMsg(4, t2->colHeaders[r2]);
			if (s1 == s2)
			{
				Row rec;
				showMsg(4, "i=" + to_string(i) + " j=" + to_string(j));
				for (size_t k = 0; k < tabCols.size(); ++k)
				{
					string c = tabCols[k].first + "." + tabCols[k].second;
					DBTable *t = tables[tabCols[k].first];
					string &type = t->colHeaders[tabCols[k].second];
					if (tabNames[0] == tabCols[k].first)
					{
						size_t size = getTypeSize(type, t->records[i][tabCols[k].second]);
						rec[c] = (void*)malloc(size);
						memcpy(rec[c], t->records[i][tabCols[k].second], size);
					}
					else if (tabNames[1] == tabCols[k].first)
					{
						size_t size = getTypeSize(type, t->records[j][tabCols[k].second]);
						rec[c] = (void*)malloc(size);
						memcpy(rec[c], t->records[j][tabCols[k].second], size);
					}
					else
					{
						rec[c] = NULL;
						showMsg(1, "Can't resolve column: " + tabCols[k].first);
					}
				}
				result->records.push_back(rec);
			}
		}
	}
	showMsg(2, "Выборка по таблицам готова");
	return result;
}
//-----------------------------------------------------------------------------