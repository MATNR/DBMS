//-----------------------------------------------------------------------------
// File: dbtable.h
// Auth: SnipGhost
//                                            Структура класса DBTable и замены
//-----------------------------------------------------------------------------
#ifndef DBTABLE_H 
#define DBTABLE_H
//-----------------------------------------------------------------------------
#include "config.h"
//-----------------------------------------------------------------------------
typedef map<string, string> Header;            // Тип заголовка таблицы
typedef map<string, string>::iterator It_head; // Тип итератора по заголовку
typedef map<string, void*> Row;                // Тип одной записи в таблице
typedef map<string, void*>::iterator It_body;  // Тип итератора по данным
//-----------------------------------------------------------------------------
class DBTable
{
 private:
	 string tableName;    // Имя таблицы
	 Header colHeaders;   // Заголовок таблицы (имена столбцов + тип значений)
	 vector<Row> records; // Вектор записей
	 void* getValue(string &type, char* value);
	 void extractValue(string colName, void *val);
 public:
	 DBTable();
	 ~DBTable();
	 DBTable(string path, char *delims = STD_DELIMS);
	 size_t getSize();
	 Row& operator[](size_t index);
	 bool readFromFile(string path, char *delims = STD_DELIMS);
	 void printTable();
};
//-----------------------------------------------------------------------------
#endif /* DBTABLE_H */
//-----------------------------------------------------------------------------