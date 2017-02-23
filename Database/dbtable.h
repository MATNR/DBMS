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
class DBTable
{
 private:
	 string tableName;    // Имя таблицы
	 Header colHeaders;   // Заголовок таблицы (имена столбцов + тип значений)
	 vector<Row> records; // Вектор записей
	 bool isColExist(string colName);
	 void sort(string colName, RowCmp cmp, bool isReverse);
 public:
	 DBTable();
	 ~DBTable();
	 DBTable(string path, char *delims = STD_DELIMS);
	 size_t getSize();
	 Row& operator[](size_t index);
	 string getColType(string colName);
	 bool readFromFile(string path, char *delims = STD_DELIMS);
	 void printTable(bool withHeader = true);
	 bool printValue(size_t rowNum, string colName);
	 bool removeRow(size_t rowNum);
	 int findRow(string colName, char *val);
	 bool sortRecords(string colName, bool isReverse = false);
};
//-----------------------------------------------------------------------------
#endif /* DBTABLE_H */
//-----------------------------------------------------------------------------