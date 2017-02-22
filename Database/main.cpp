//-----------------------------------------------------------------------------
// File: main.cpp
// Auth: SnipGhost
//                                                              Главная функция
//-----------------------------------------------------------------------------
#include "config.h"
#include "dbtable.h"
//-----------------------------------------------------------------------------
class DataBase
{
 private:
	 string dbName;              // Имя базы данных
	 map<string, DBTable> dbTab; // Набор таблиц
 public:

};
//
int main()
{
	system("chcp 1251 > nul");
	readConfig("config.txt");
	DBTable *t = new DBTable("table.txt");
	t->printTable();
	delete t;
	system("pause");
	return 0;
}
//-----------------------------------------------------------------------------