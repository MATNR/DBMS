//-----------------------------------------------------------------------------
// File: dbtable.cpp
// Auth: SnipGhost
//                                            Реализация методов класса DBTable
//-----------------------------------------------------------------------------
#include "dbtable.h"
//-----------------------------------------------------------------------------
bool DBTable::isColExist(string colName)
{
	if (colHeaders.count(colName) == 0) {
		showMsg(1, "Такого столбца не существует");
		return 0;
	}
	return 1;
}
//-----------------------------------------------------------------------------
DBTable::DBTable()
{
	tableName = "";
	showMsg(2, "Успешно создана пустая таблица");
}
//-----------------------------------------------------------------------------
DBTable::~DBTable()
{
	for (size_t i = 0; i < records.size(); ++i)
		for (It_body j = records[i].begin(); j != records[i].end(); ++j)
			delete [] j->second;
	colHeaders.clear();
	records.clear();
	showMsg(2, "Таблица успешно удалена, память очищена");
}
//-----------------------------------------------------------------------------
DBTable::DBTable(string path, char *delims)
{
	if (!readFromFile(path, delims)) {
		tableName = "";
		colHeaders.clear();
		records.clear();
		showMsg(1, "Таблицу загрузить не удалось, создана пустая");
	} else {
		showMsg(2, "Успешно создана загруженная таблица");
	}
}
//-----------------------------------------------------------------------------
size_t DBTable::getSize()
{
	return (int)records.size();
}
//-----------------------------------------------------------------------------
Row& DBTable::operator[](size_t index)
{
	return records[index % getSize()];
}
//-----------------------------------------------------------------------------
string DBTable::getColType(string colName)
{
	return colHeaders[colName];
}
//-----------------------------------------------------------------------------
bool DBTable::readFromFile(string path, char *delims)
{
	ifstream fin(path);                // Входной типа-CSV файл с таблицей
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (fin.fail()) {
		showMsg(0, "Не удалось открыть файл " + path);
		return 0;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	char line[MAX_LINE], *first_token, *second_token;
	vector<pair<string, string>> head; // Вспомогательный буфер под заголовок
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	getline(fin, tableName);           // Получили название таблицы
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
	if (colHeaders.size() != head.size()) {
		showMsg(1, "Некорректный заголовок таблицы в файле " + path);
		return 0;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Получаем очередную запись в цикле
	while (fin.getline(line, 255) && strlen(line) > 1)
	{ 
		size_t col = 0;  // Параллельно отслеживаем номер столбца
		Row rec;         // Начинаем формировать текущую запись
		first_token = strtok(line, delims);
		while (first_token != NULL)
		{
			if (col > head.size()-1) {
				showMsg(1, "Слишком много данных в текущей строке");
				return 0;
			}
			// dataToVoid требует очистки памяти каждого val в деструкторе
			void *val = getValue(head[col].second, first_token);
			rec[head[col].first] = val;
			col++;
			first_token = strtok(NULL, delims);
		}
		records.push_back(rec); // Добавляем запись в конец таблицы
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	showMsg(2, "Файл с таблицей успешно прочитан");
	fin.close(); // Закрывам файл
	return 1;    // Возвращаемые значения: 1 - успех, 0 - произошла ошибка
}
//-----------------------------------------------------------------------------
void DBTable::printTable(bool withHeader)
{                             // TODO: сделать размеры полей вывода изменяемыми
	if (colHeaders.size() == 0) {
		showMsg(0, "Таблица не создана");
		return;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (withHeader)
	{
		cout << tableName << endl; // Вывод названия таблицы
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Вывод заголовка
		for (It_head i = colHeaders.begin(); i != colHeaders.end(); ++i)
		{
			cout << setw(7) << i->first << ": ";
			cout << setw(7) << i->second << " | ";
		}
		cout << endl;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (getSize() == 0) {
		showMsg(1, "Таблица пуста");
		return;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Вывод записей
	for (size_t i = 0; i < records.size(); ++i)
	{
		for (It_body c = records[i].begin(); c != records[i].end(); ++c)
		{
			cout << setw(16);
			extValue(colHeaders[c->first], c->second);
			cout << " | ";
		}
		cout << endl;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	cout << endl;
	showMsg(2, "Печать таблицы завершена");
}
//-----------------------------------------------------------------------------
bool DBTable::printValue(size_t rowNum, string colName)
{
	if (!isColExist(colName)) return 0;
	extValue(colHeaders[colName], records[rowNum][colName]);
	return 1;
}
//-----------------------------------------------------------------------------
bool DBTable::removeRow(size_t rowNum)
{
	if (rowNum >= getSize()) {
		showMsg(1, "Нет строки с таким номером");
		return 0;
	}
	records.erase(records.begin() += rowNum);
	return 1; // 1 - удаление прошло успешно, 0 - записи не найдено
}
//-----------------------------------------------------------------------------
int DBTable::findRow(string colName, char *value)
{
	if (!isColExist(colName)) return -1;
	void *val = getValue(getColType(colName), value);
	for (auto it = records.begin(); it != records.end(); ++it)
	{
		int size = getTypeSize(colHeaders[colName], val);
		int i = memcmp(val, (*it)[colName], size);
		if (i == 0) return (it-records.begin());
	}
	showMsg(1, "Запись со значением " + colName + "=" + value + " не найдена");
	return -1; // -1 - запись в таблице не найдена
}
//-----------------------------------------------------------------------------
void DBTable::sort(string colName, RowCmp cmp, bool isReverse)
{
	for (size_t i = 0; i < records.size(); ++i)
	{
		size_t mi = i;
		for (size_t j = i+1; j < records.size(); ++j)
		{
			int r = cmp(records[j], records[mi], colName);
			if (isReverse ? r > 0 : r < 0) 
				mi = j;
		}
		if (mi != i) swap(records[mi], records[i]);
	}
}
//-----------------------------------------------------------------------------
bool DBTable::sortRecords(string colName, bool isReverse)
{
	if (!isColExist(colName)) return 0;
	switch (typeCodes[colHeaders[colName]])
	{
		case 1:
			sort(colName, rowIntCmp, isReverse);
			break;
		case 2:
			sort(colName, rowDouCmp, isReverse);
			break;
		case 3:
			sort(colName, rowStrCmp, isReverse);
			break;
		default:
			showMsg(0, "По столбцу '" + colName + "' нельзя отсортировать");
			return 0;
	}
	showMsg(2, "Таблица успешно отсортированна по столбцу '" + colName + "'");
	return 1;
}
//-----------------------------------------------------------------------------