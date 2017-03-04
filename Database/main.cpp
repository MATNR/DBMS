//-----------------------------------------------------------------------------
// File: main.cpp
// Auth: SnipGhost
//                                                              Главная функция
//-----------------------------------------------------------------------------
#include "dbset.h"
//-----------------------------------------------------------------------------
int main()
{
	system("chcp 1251 > nul");
	if (!readConfig("config.txt")) return 1;
	DBSet db("tables.txt");
	for (size_t i = 0; i < db.getSize(); ++i)
	{		
		DBTable *t = &db[i];
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
		int index = t->findRow("Type", "3");
		if (index != -1) 
			index = t->findRow("Mark", "4.6");
		if (index >= 0) {
			t->removeRow(index);
			t->printTable();
		}
		// EXAMPLE #4 - добавление строки //
		string s = "Name=Алёна Баранова|Mark=5.0|Group=524|AId=";
		s += to_string(t->findM("AId") + 1); // Авто-инкремент
		if (t->insertRow(s, "|="))
			t->printTable(1, *logs);
		showMsg(2, "Работа с таблицей #" + to_string(i) + " завершена");
	}
	// EXAMPLE #5 - прямая работа с данными //
	if (db["students"].isColExist("Name"))
		showMsg(0, extValue(db["students"]["Name"], db["students"][3]["Name"]));
	db["students"].printTable();
	system("pause");
	return 0;
}
//-----------------------------------------------------------------------------