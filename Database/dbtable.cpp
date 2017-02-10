//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "dbtable.h"
//-----------------------------------------------------------------------------
void* dataToVoid(string &type, char* value) // Вспомогательная функция
{                                           // Переводит строку в соотв. тип
	void *vp;
	if (type == "Integer") 
	{
		int *buffer = new int(atoi(value));
		vp = buffer;
	} 
	else if (type == "Float") 
	{
		double *buffer = new double(atof(value));
		vp = buffer;
	} 
	else 
	{
		char *buffer = new char[strlen(value)+1];
		memcpy(buffer, value, strlen(value)+1);
		vp = buffer;
	}
	return vp;
}
//-----------------------------------------------------------------------------
DBTable::DBTable()
{
	tableName = "";
}
//-----------------------------------------------------------------------------
DBTable::DBTable(string path)
{
	if (!readFromFile(path)) {
		tableName = "";
		colHeaders.clear();
		records.clear();
	}
}
//-----------------------------------------------------------------------------
DBTable::~DBTable()
{
	for (size_t i = 0; i < records.size(); ++i)
		for (It_body j = records[i].begin(); j != records[i].end(); ++j)
			delete [] j->second;
}
//-----------------------------------------------------------------------------
bool DBTable::readFromFile(string path)
{
	ifstream fin(path);                // Входной типа-CSV файл с таблицей
	vector<pair<string, string>> head; // Вспомогательный буффер под заголовок
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	getline(fin, tableName); // Получили имя таблицы
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	char line[255], *first_token, *second_token, *delims = "|";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	fin.getline(line, 255);  // Получили заголовок таблицы, режем его
	first_token = strtok(line, delims);
	while (first_token != NULL) 
	{
		second_token = strtok(NULL, delims);
		colHeaders.insert(pair<string, string>(first_token, second_token));
		head.push_back(pair<string, string>(first_token, second_token));
		first_token = strtok(NULL, delims);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Получаем очередную запись в цикле
	while (fin.getline(line, 255) && strlen(line) > 1)
	{ 
		size_t col = 0;
		Row rec;
		first_token = strtok(line, delims);
		while (first_token != NULL)
		{
			// dataToVoid требует очистки памяти в деструкторе
			void *val = dataToVoid(head[col].second, first_token);
			rec.insert(pair<string, void*>(head[col].first, val));
			col++;
			first_token = strtok(NULL, delims);
		}
		records.push_back(rec);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	fin.close();
	return 1;
}
//-----------------------------------------------------------------------------
void DBTable::printTable()
{
	cout << tableName << endl;
	for (It_head i = colHeaders.begin(); i != colHeaders.end(); ++i)
		cout << setw(10) << i->first << " : " << setw(7) << i->second << " | ";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	cout << endl;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (size_t i = 0; i < records.size(); ++i)
	{
		It_head col = colHeaders.begin();
		for (It_body j = records[i].begin(); j != records[i].end(); ++j, ++col)
		{
			cout << setw(20);
			if (col->second == "Integer") cout << (*((int*)j->second));
			else if (col->second == "Float") cout << (*((double*)j->second));
			else cout << (char*)(j->second);
			cout << " | ";
		}
		cout << endl;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	cout << endl;
}
//-----------------------------------------------------------------------------