//-----------------------------------------------------------------------------
// File: dbtable.h
// Auth: SnipGhost
//                                            Структура класса DBTable и замены
//-----------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
typedef map<string, string> Header;            // Тип заголовка таблицы
typedef map<string, string>::iterator It_head; // Тип итератора по заголовку
typedef map<string, void*> Row;                // Тип одной записи в таблице
typedef map<string, void*>::iterator It_body;  // Тип итератора по данным
//-----------------------------------------------------------------------------
const int MAX_LINE = 255; // Максимальная длина строки файла
//-----------------------------------------------------------------------------
class DBTable
{
 private:
	 string tableName;    // Имя таблицы
	 Header colHeaders;   // Заголовок таблицы (имена столбцов + тип значений)
	 vector<Row> records; // Вектор записей
 public:
	 DBTable();
	 DBTable(string path);
	 ~DBTable();
	 bool readFromFile(string path);
	 void printTable();
	 //Row getRecord(string value, string colName);
};
//-----------------------------------------------------------------------------