//-----------------------------------------------------------------------------
// File: main.cpp
// Auth: SnipGhost
//                                               �������������� ������� �� ����
//-----------------------------------------------------------------------------
#include "../../Database/dbset.h"
//-----------------------------------------------------------------------------
using namespace Kernel;
//-----------------------------------------------------------------------------
int main()
{
	system("chcp 1251 > nul");
	if (!readConfig("config.txt")) return 1;

	DBSet db("tables.txt"); // ������ ���� ������ �� ����� ������ ������

	// EXAMPLE #0 - ������������ SELECT �� ���� ��������
	db["students"].sortRecords("AId"); // ����������� ��� ���������� ������
	string cols = "list.AId, list.BookId, list.StuId, students.Name";
	string exp = "list.StuId=students.AId";
	DBTable *t = db.selectData(cols, exp);
	t->printTable();
	// t->printTable(true, *logs); // ������, �� �������� � ���
	delete t;                      // �������� ��������� �����������

	for (size_t i = 0; i < db.getSize(); ++i)
	{		
		DBTable *t = &db[i]; // ��������� �������
		t->printTable();
		// EXAMPLE #1 - ���������� //
		if (t->sortRecords("Name"))
			t->printTable();
		if (t->sortRecords("Group", 1))
			t->printTable();
		if (t->sortRecords("Mark"))
			t->printTable();
		// EXAMPLE #2 - ������ �������� //
		if (t->printValue(2, "Name")) {
			showMsg(0, "������� ���������� �������� [2][Name]");
		} else {
			showMsg(0, "�������� [2][Name] �� �������");
		}
		// EXAMPLE #3 - �������� ������ //
		if (t->removeRow(2))
			t->printTable();
		// EXAMPLE #4 - ����� � �������� //
		int index = t->findRow("Type", "3");
		if (index != -1) 
			index = t->findRow("Mark", "4.6");
		if (index >= 0) {
			t->removeRow(index);
			t->printTable();
		}
		// EXAMPLE #5 - ���������� ������ //
		string s = "Name=���� ��������|Mark=5.0|Group=524|AId=";
		s += to_string(t->findM("AId") + 1); // ����-���������
		if (t->insertRow(s, "|="))
			t->printTable();
		showMsg(2, "������ � �������� #" + to_string(i) + " ���������");
	}

	// EXAMPLE #6 - ������ ������ � ������� //
	if (db["students"].isColExist("Name")) {
		string val = extValue(db["students"]["Name"], db["students"][3]["Name"]);
		showMsg(2, "[students : 3 : Name]: " + val);
	}

	// EXAMPLE #7 - ��������� ���� ���������� � �������
	vector<Row> v = db["students"].getSelfRows("Name", "����� �������");
	for (auto i = v.begin(); i != v.end(); ++i)
	{
		for (auto j = i->begin(); j != i->end(); ++j)
		{
			string v = extValue(db["students"][j->first], j->second);
			showMsg(0, j->first + " : " + v, cout);
		}
		showMsg(3, "End of Row", cout); // ����� � �������
	}

	// EXAMPLE #8 - ������ ������� ��������� � �������� � ��
	db["students"].printTable(true, cout, "AId, Group"); // ������ ����
	db.dropTable("students");                            // � ��� ����
	db["students"].printTable();

	showMsg(0, "����� ������������� ���������", cout);

	system("pause");
	return 0;
}
//--------------------------------------------------------------