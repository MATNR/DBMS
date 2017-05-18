//-----------------------------------------------------------------------------
// File: kernel.h
// Auth: SnipGhost
//                                                 Вспомогательные функции ядра
//-----------------------------------------------------------------------------
#pragma once
#ifndef KERNEL_H 
#define KERNEL_H
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS // За..колебали уже ворнинги тупых функций MS
//#define DEBUG_AMSG            // Вывод любых отладочных сообщений
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <map>
#include "dbdate.h"
//-----------------------------------------------------------------------------
namespace Kernel                                       // Пространсво имен ядра
{
	using namespace std;        // Вкладываем STD
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class DBTable;              // Класс таблиц      (реализация: dbtable.cpp)
	class DBSet;                // Класс базы данных (реализация: dbset.cpp)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	const int MAX_LINE = 1023;  // Максимальная длина строки считываемого файла
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern ostream *logs;              // Поток вывода логов СУБД
	extern char STD_DELIMS[20];        // Разделители по умолчанию
	extern map<string, int> typeCodes; // Коды зарегестрированных типов
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Получить текущее время в виде format (например: "%d.%m.%Y %I:%M:%S")
	string getLocalTime(const char *format);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 // Вывести сообщение (msg) указанного типа (type) в указанный поток (out)
	void showMsg(int type, string msg, ostream &out = *logs);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Получить размер типа (байт) в конкретной ячейке
	size_t getTypeSize(string type, void *val);
	// Считать конфигурацию из указанного файла path
	bool readConfig(string path);
	// Переводит строку value в void* (выделяет память)
	void* getValue(string type, const char* value);
	// Извлечь значение из val с типом type в строку
	string extValue(string type, void *val);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	typedef map<string, string> Header;              // Тип заголовка таблицы
	typedef map<string, string>::iterator It_head;   // Тип итератора заголовка
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Row
	{
	public:
		typedef map<string, void*>::iterator iterator;
		typedef map<string, void*>::const_iterator const_iterator;
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		size_t size();
		void clear();
		void*& operator[](string key);
		string extValue(string name, string type);
	private:
		map<string, void*> data;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	typedef Row::iterator It_body;                   // Тип итератора по данным
	typedef map<string, DBTable*> Tab;               // Формат хранения таблиц
	typedef int (*RowCmp)(Row &a, Row &b, string s); // Указатель на компаратор
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int rowIntCmp(Row &a, Row &b, string s); // Компаратор для целых ячеек
	int rowDouCmp(Row &a, Row &b, string s); // Компаратор для нецелых ячеек
	int rowStrCmp(Row &a, Row &b, string s); // Компаратор для строковых ячеек
};
//-----------------------------------------------------------------------------
#endif /* KERNEL_H */
//-----------------------------------------------------------------------------