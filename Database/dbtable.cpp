//-----------------------------------------------------------------------------
// File: dbtable.cpp
// Auth: SnipGhost
//                                            Реализация методов класса DBTable
//-----------------------------------------------------------------------------
#include "dbtable.h"
//-----------------------------------------------------------------------------
void* DBTable::dataToVoid(string &type, char* value) // Вспомогательная функция
{                                              // Переводит строку в соотв. тип
	void *vp = NULL;
	if (type == "Integer") 
	{
		int *buffer = new int(atoi(value));
		vp = buffer;
	}
	else if (type == "Float" || type == "Double")
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
void DBTable::printValue(string colName, void *val)
{
	if (colHeaders[colName] == "Integer") cout << (*((int*)val));
	else if (colHeaders[colName] == "Float") cout << (*((double*)val));
	else cout << (char*)(val);
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
	colHeaders.clear();
	records.clear();
}
//-----------------------------------------------------------------------------
bool DBTable::readFromFile(string path)
{
	ifstream fin(path);                // Входной типа-CSV файл с таблицей
	vector<pair<string, string>> head; // Вспомогательный буфер под заголовок
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	getline(fin, tableName);           // Получили имя таблицы
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	char line[MAX_LINE], *first_token, *second_token, *delims = "|";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	fin.getline(line, MAX_LINE);       // Получили заголовок таблицы, режем его
	first_token = strtok(line, delims);
	while (first_token != NULL) 
	{
		second_token = strtok(NULL, delims);
		colHeaders[first_token] = second_token;
		head.push_back(pair<string, string>(first_token, second_token));
		first_token = strtok(NULL, delims);
	}
	if (colHeaders.size() != head.size()) return 0;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Получаем очередную запись в цикле
	while (fin.getline(line, 255) && strlen(line) > 1)
	{ 
		size_t col = 0;  // Параллельно отслеживаем номер столбца
		Row rec;         // Начинаем формировать текущую запись
		first_token = strtok(line, delims);
		while (first_token != NULL)
		{
			// dataToVoid требует очистки памяти каждого val в деструкторе
			void *val = dataToVoid(head[col].second, first_token);
			rec[head[col].first] = val;
			col++;
			first_token = strtok(NULL, delims);
		}
		records.push_back(rec); // Добавляем запись в конец таблицы
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	fin.close(); // Закрывам файл
	return 1;    // Возвращаемые значения: 1 - успех, 0 - произошла ошибка
}
//-----------------------------------------------------------------------------
void DBTable::printTable()
{
	if (tableName.size() == 0) {
		cout << "Таблица не создана\n";
		return;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	cout << tableName << endl; // Вывод названия таблицы
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Вывод заголовка
	for (It_head i = colHeaders.begin(); i != colHeaders.end(); ++i)
		cout << setw(7) << i->first << ": " << setw(7) << i->second << " | ";
	cout << endl;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (this->getSize() == 0) {
		cout << "Таблица пуста\n";
		return;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Вывод записей
	for (size_t i = 0; i < records.size(); ++i)
	{
		for (It_body c = records[i].begin(); c != records[i].end(); ++c)
		{
			cout << setw(16);
			printValue(c->first, c->second);
			cout << " | ";
		}
		cout << endl;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	cout << endl;
}
//-----------------------------------------------------------------------------
size_t DBTable::getSize()
{
	return (int)records.size();
}
//-----------------------------------------------------------------------------
Row& DBTable::operator[](int index)
{
	return records[index];
}
//-----------------------------------------------------------------------------