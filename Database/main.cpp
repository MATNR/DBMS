//-----------------------------------------------------------------------------
// File: main.cpp
// Auth: SnipGhost
//                                                          Примеры работы с БД
//-----------------------------------------------------------------------------
#include "dbset.h"
using namespace Kernel;
//-----------------------------------------------------------------------------
void testDBDate()
{
	DBDate dobP("06.06.1799");
	DBDate dobM("16.12.1998");
	showMsg(0, to_string(dobM - dobP) + " дней между рождением Пушкина и МК");
}
//-----------------------------------------------------------------------------
int main()
{
	system("chcp 1251 > nul");
	if (!readConfig("config.txt")) return 1;

	testDBDate(); // TODO: Все работает, удалить после ознакомления

	DBSet db("tables.txt"); // Задаем базу данных по файлу списка таблиц

	// EXAMPLE #0 - произвольный SELECT по двум таблицам
	db["students"].sortRecords("AId"); // Отсортируем для корректной работы
	string cols = "list.AId, list.BookId, list.StuId, students.Name";
	string exp = "list.StuId=students.AId";
	DBTable *t = db.selectData(cols, exp);
	t->printTable();
	// t->printTable(true, *logs); // Аналог, но записать в лог
	delete t;                      // Проверка глубокого копирования

	for (size_t i = 0; i < db.getSize(); ++i)
	{		
		DBTable *t = &db[i]; // Назначили синоним
		t->printTable();
		// EXAMPLE #1 - сортировка //
		if (t->sortRecords("Name"))
			t->printTable();
		if (t->sortRecords("Group", 1))
			t->printTable();
		if (t->sortRecords("Mark"))
			t->printTable();
		// EXAMPLE #2 - печать значения //
		if (t->printValue(2, "Name")) {
			showMsg(0, "Успешно распечатал значение [2][Name]");
		} else {
			showMsg(0, "Значение [2][Name] не найдено");
		}
		// EXAMPLE #3 - удаление записи //
		if (t->removeRow(2))
			t->printTable();
		// EXAMPLE #4 - поиск и удаление //
		int index = t->findRow("Type", "3");
		if (index != -1) 
			index = t->findRow("Mark", "4.6");
		if (index >= 0) {
			t->removeRow(index);
			t->printTable();
		}
		// EXAMPLE #5 - добавление строки //
		string s = "Name=Алёна Баранова|Mark=5.0|Group=524|AId=";
		s += to_string(t->findM("AId") + 1); // Авто-инкремент
		if (t->insertRow(s, "|="))
			t->printTable();
		showMsg(2, "Работа с таблицей #" + to_string(i) + " завершена");
	}

	// EXAMPLE #6 - прямая работа с данными //
	if (db["students"].isColExist("Name")) {
		string val = extValue(db["students"]["Name"], db["students"][3]["Name"]);
		showMsg(2, "[students : 3 : Name]: " + val);
	}

	// EXAMPLE #7 - получение всех совпадений в векторе
	vector<Row> v = db["students"].getSelfRows("Name", "Антон Андреев");
	for (auto i = v.begin(); i != v.end(); ++i)
	{
		for (auto j = i->begin(); j != i->end(); ++j)
		{
			string v = extValue(db["students"][j->first], j->second);
			showMsg(0, j->first + " : " + v, cout);
		}
		showMsg(3, "End of Row", cout); // Вывод в консоль
	}

	// EXAMPLE #8 - другой вариант обращения к таблицам в БД
	db["students"].printTable(true, cout, "AId, Group"); // Тагиру лень
	db.dropTable("students");                            // А мне норм
	db["students"].printTable();

	showMsg(0, "Конец показательной программы", cout);

	system("pause");
	return 0;
}
//-----------------------------------------------------------------------------