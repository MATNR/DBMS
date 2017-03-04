//-----------------------------------------------------------------------------
// File: dbset.cpp
// Auth: SnipGhost
//                                              ���������� ������� ������ DBSet
//-----------------------------------------------------------------------------
#include "dbset.h"
//-----------------------------------------------------------------------------
DBSet::DBSet()
{
	dbName = "";
	showMsg(2, "������ ���� ������ " + dbName + " ������� �������");
}
//-----------------------------------------------------------------------------
DBSet::~DBSet()
{
	for (auto it = tables.begin(); it != tables.end(); ++it)
		delete it->second;
	showMsg(2, "���� ������ ������� ������� �� ������");
}
//-----------------------------------------------------------------------------
DBSet::DBSet(string path)
{
	if (!readTableSet(path)) {
		dbName = "";
		tables.clear();
	}
	showMsg(2, "���� ������ " + dbName + " ������� �������");
}
//-----------------------------------------------------------------------------
bool DBSet::isTableExist(string name)
{
	if (tables.count(name) < 1) {
		showMsg(0, "��������� � �������������� �������: " + name);
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
	if (!isTableExist(name)) exit(0);
	return *tables[name];
}
//-----------------------------------------------------------------------------
DBTable& DBSet::operator[](size_t index)
{
	auto it = tables.begin(); // TODO: ���������� ��� ������� �������
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
		showMsg(0, "������ �������� ����� " + path);
		return 0;
	}
	getline(fin, dbName);
	string line;
	showMsg(0, "����� �������� ������ � �� " + dbName);
	while (getline(fin, line)) 
		createTable(line);
	fin.close();
	showMsg(2, "�������� ������ � �� " + dbName + " ������� ���������");
	return 1;
}
//-----------------------------------------------------------------------------
bool DBSet::createTable(string path)
{
	char line[MAX_LINE], *token_name;
	strcpy(line, path.c_str());
	token_name = strtok(line, "."); // TODO: ���������� ��� �������� ����� � ����
	if (strlen(token_name) < 1) {
		showMsg(2, "�����-�� ������ ����: " + path);
		return 0;
	}
	DBTable *tab = new DBTable(path);
	tables.insert(make_pair(string(token_name), tab));
	showMsg(2, "������� ������� ���������");
	return 1;
}
//-----------------------------------------------------------------------------