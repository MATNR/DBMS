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
 public:
	 DBTable();
	 ~DBTable();
	 DBTable(string path, char *delims = STD_DELIMS);
	 size_t getSize();
	 Row& operator[](size_t index);
	 string getColType(string colName);
	 bool readFromFile(string path, char *delims = STD_DELIMS);
	 void printTable();
	 void printValue(size_t rowNum, string colName);
	 bool removeRow(size_t rowNum);
	 int findRow(string colName, char *val);
};
//-----------------------------------------------------------------------------
#endif /* DBTABLE_H */
//-----------------------------------------------------------------------------