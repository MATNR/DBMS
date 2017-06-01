//-----------------------------------------------------------------------------
// File: dbtable.h
// Auth: SnipGhost
//                                                     Структура класса DBTable
//-----------------------------------------------------------------------------
#pragma once
#ifndef DBTABLE_H 
#define DBTABLE_H
//-----------------------------------------------------------------------------
#include "kernel.h"
//-----------------------------------------------------------------------------
class Kernel::DBTable
{
 private:
	 string tableName;    // Имя таблицы
	 Header colHeaders;   // Заголовок таблицы (имена столбцов + тип значений)
	 vector<Row> records; // Вектор записей
	 friend DBSet;
	 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 void sort(string colName, RowCmp cmp, bool isReverse); // Сортировка
 public:
	 DBTable();  // Конструктор по-умолчанию
	 ~DBTable(); // Деструктор по-умолчанию
	 // Конструктор с заполнением из файла (с выбранными разделителями)
	 DBTable(string path, char *delims = STD_DELIMS);
	 DBTable(Header head);
	 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 Header getColHeaders();            // Возвращает копию заголовка таблицы
	 bool isColExist(string colName);   // Проверяет на существование столбец
	 size_t getSize();                  // Получить количество записей-строк
	 Row& operator[](size_t index);     // Обращение к строке с номером index
	 string operator[](string colName); // Обращение к заголовку colName
	 string getColType(string colName); // Получить имя типа столбца colName
	 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 // Прочитать таблицу из файла (с выбранными разделителями)
	 bool readFromFile(string path, char *delims = STD_DELIMS);
	 // Печать таблицы (печатать заголовок да/нет, поток вывода)
	 void printTable(bool withHeader = 1, ostream &out = cout, 
		             string cols = "*", bool save = 0);
	 // Сохранение таблицы в поток out
	 bool DBTable::writeTable(ostream &out = cout);
	 // Печать указанного значения таблицы (с выбранным потоком вывода)
	 bool printValue(size_t rowNum, string colName, ostream &out = cout);
	 // Удаление записи-строки из таблицы с индексом rowNum
	 bool removeRow(size_t rowNum);
	 // Добавить в конец таблицы новую запись из строки
	 bool insertRow(string line, char *delims = STD_DELIMS);
	 // Поиск указанного значения столбца, вернет первое вхождение или -1
	 int findRow(string colName, const char *val, int start = 0);
	 // Поиск максимального/минимального значения по столбцу (ТОЛЬКО ДЛЯ INT)
	 int findM(string colName, bool isMin = false); // Для авто-инкремента ID
	 // Интерфейс сортировки по столбцу colName (+ обратная сортировка да/нет)
	 bool sortRecords(string colName, bool isReverse = false);
	 // Сравнивает значения данных в столбце colName двух строк таблицы 
	 bool ValueIsEqual(string colName, void* objA, void* objB);
	 // Возвращае вектор из строк где значение столбца равно value
	 vector<Row> getSelfRows(string colName, const char* value);
};
//-----------------------------------------------------------------------------
#endif /* DBTABLE_H */
//-----------------------------------------------------------------------------