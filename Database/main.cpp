//-----------------------------------------------------------------------------
// File: main.cpp
// Auth: SnipGhost
//                                                              Главная функция
//-----------------------------------------------------------------------------
#include "config.h"
#include "dbtable.h"
//-----------------------------------------------------------------------------
int main()
{
	system("chcp 1251 > nul");
	if (!readConfig("config.txt")) return 1;
	// Создание и загрузка таблицы //
	DBTable *t = new DBTable("table.txt");
	t->printTable();
	// EXAMPLE #0 - сортировка //
	if (t->sortRecords("Name"))
		t->printTable();
	if (t->sortRecords("Group", 1))
		t->printTable();
	if (t->sortRecords("Mark"))
		t->printTable();
	// EXAMPLE #1 - печать значения //
	if (t->printValue(2, "Name"))
		cout << endl;
	// EXAMPLE #2 - удаление записи //
	if (t->removeRow(2))
		t->printTable();
	// EXAMPLE #3 - поиск и удаление //
	int i;
	i = t->findRow("Mark", "4.5");
	i = t->findRow("Hupp", "4.4");
	i = t->findRow("Mark", "4.6");
	if (i >= 0) {
		t->removeRow(i);
		t->printTable();
	}
	// EXAMPLE #4 - добавление строки //
	string s = "Name=Алёна Баранова|Mark=5.0|Group=524|AId=";
	s += to_string(t->findM("AId") + 1); // Авто-инкремент
	if (t->insertRow(s, "|="))
		t->printTable(1, *logs);
	// Удаление всей таблицы //
	delete t;
	system("pause");
	return 0;
}
//-----------------------------------------------------------------------------