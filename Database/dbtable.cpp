//-----------------------------------------------------------------------------
// File: dbtable.cpp
// Auth: SnipGhost, HeliSRaiN
//                                            Реализация методов класса DBTable
//-----------------------------------------------------------------------------
#include "dbtable.h"
//-----------------------------------------------------------------------------
using namespace Kernel;
//-----------------------------------------------------------------------------
enum color // Консольные цвета
{ 
	BLACK = 0,
	BLUE_B,
	GREEN_B,
	Cyan,
	Red_B,
	Magenta,
	Brown,
	GRAY_A,
	GRAY_B,
	BLUE_A,
	GREEN_A,
	LightCyan,
	RED_A,
	LightMagenta,
	YELLOW,
	WHITE 
};
//-----------------------------------------------------------------------------
void setColor(color text, color background)  
{
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
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
DBTable::DBTable(Header head)
{
	if (head.size() < 1) {
		tableName = "";
		colHeaders.clear();
		records.clear();
		showMsg(1, "Таблицу заселектить не удалось, создана пустая");
	} else {
		tableName = ".selected";
		colHeaders = head;
		records.clear();
		showMsg(2, "Создана незаполненная selected-таблица");
	}
}
//-----------------------------------------------------------------------------
Header DBTable::getColHeaders()
{
	return colHeaders;
}
//-----------------------------------------------------------------------------
bool DBTable::isColExist(string colName)
{
	if (colHeaders.count(colName) == 0) {
		showMsg(1, "Столбца " + colName + " не существует");
		return 0;
	}
	return 1;
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
string DBTable::operator[](string colName)
{
	if (!isColExist(colName)) return "NULL";
	return colHeaders[colName];
}
//-----------------------------------------------------------------------------
string DBTable::getColType(string colName)
{
	if (!isColExist(colName)) return "NULL";
	return colHeaders[colName];
}
//-----------------------------------------------------------------------------
bool DBTable::readFromFile(string path, char *delims)
{
	tableName = "";
	colHeaders.clear();                // Предварительная очистка таблицы
	records.clear();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
	// TODO: исправить чтение имени таблицы
	tableName = path.substr(0, path.size()-4);     // Получили название таблицы
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	fin.getline(line, MAX_LINE);       // Получили заголовок таблицы, режем его
	first_token = strtok(line, delims);
	while (first_token != NULL) 
	{
		second_token = strtok(NULL, delims);
		colHeaders[first_token] = second_token;
		head.push_back(make_pair(first_token, second_token));
		first_token = strtok(NULL, delims);
	}
	if (colHeaders.size() != head.size()) {
		showMsg(1, "Некорректный заголовок таблицы в файле " + path);
		return 0;
	}
	string msg = "Неверные данные, файл " + path + ", строка #";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Получаем очередную запись в цикле
	size_t count = 0;
	while (fin.getline(line, MAX_LINE))
	{ 
		count++;
		Row rec;         // Начинаем формировать текущую запись
		first_token = strtok(line, delims);
		while (first_token != NULL)
		{	
			//showMsg(0, first_token);
			if (rec.size() > head.size()-1) { break; }
			// getValue требует очистки памяти каждого val в деструкторе
			void *val = getValue(head[rec.size()].second, first_token);
			rec[head[rec.size()].first] = val;
			first_token = strtok(NULL, delims);
		}
		if (rec.size() == head.size())
			records.push_back(rec); // Добавляем запись в конец таблицы
		else
			showMsg(1, msg + to_string(count + 1));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	showMsg(2, "Файл " + path + " успешно прочитан");
	fin.close(); // Закрывам файл
	return 1;    // Возвращаемые значения: 1 - успех, 0 - произошла ошибка
}
//-----------------------------------------------------------------------------
void DBTable::printTable(bool withHeader, ostream &out, string cols, bool save)
{
	if (!save) out << endl; //Отступ от верхушки таблиц
	if (colHeaders.size() == 0) {
		showMsg(0, "Таблица " + tableName + "совершенно пуста");
		return;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	map<string, bool> colums;
	int colN = 0;
	if (cols != "*") {
		char *token, line[MAX_LINE];
		strcpy(line, cols.c_str());
		token = strtok(line, ", ");
		while (token != NULL) 
		{
			if (isColExist(token)) colums[string(token)] = true;
			else showMsg(1, "Столбец " + string(token) + " не найден");
			token = strtok(NULL, ", ");
			++colN;
		}
	}
	else colN = -1;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int WIDTH = 26;
	int WIDTH1 = WIDTH/2;
	int WIDTH2 = WIDTH1 - 5;
	int WIDTH3 = WIDTH - 3;
	string DL1 = ": ";
	string DL2 = "  ";
	if (save) 
	{
		WIDTH = 0;
		WIDTH1 = 0;
		WIDTH2 = 0;
		WIDTH3 = 0;
		DL1 = ":";
		DL2 = "|";
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (withHeader)
	{   
		if(INTERFACE_THEME == 1) 
		{
			setColor(WHITE, GRAY_B); //Белый текст на тёмно-сером фоне
			bool nameShowsFirst = true; //Название показывается впервые
			if (colN == -1)  //Если выводим все колоночки
			{
				for (It_head i = colHeaders.begin(); i != colHeaders.end(); ++i) 
				{
					if (nameShowsFirst == true) 
					{ 
						if (!save) out << left << setw(WIDTH) << tableName; 
						nameShowsFirst = false;
					} else { 
						out << setw(WIDTH) << "";
					}
				}
			} else { //Если выводим определённое количество колоночек
				for (int i = 0; i < colN; i++) 
				{
					if (nameShowsFirst == true) 
					{ 
						if (!save) out << left << setw(WIDTH) << tableName; 
						nameShowsFirst = false;
					} else { 
						out << setw(WIDTH) << "";
					}
				}
			}
			setColor(WHITE, BLACK); //Белый текст на чёрном фоне
			if (!save) out << right << endl;
	    } 
		else if(INTERFACE_THEME == 0) {
				out << tableName << endl;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Вывод заголовка
		for (It_head i = colHeaders.begin(); i != colHeaders.end(); ++i) 
		{
			if (colums[i->first] || cols == "*") 
			{
				if(INTERFACE_THEME == 1) {
					setColor(BLACK, GRAY_A); //Чёрный текст на сером фоне
					out << setw(WIDTH1) << i->first << DL1;
					out << setw(WIDTH2) << i->second << DL2;
					setColor(BLACK, GRAY_B); //Чёрный текст на тёмно-сером фоне
					if (!save) out << " ";
					setColor(WHITE, BLACK); //Белый текст на чёрном фоне
				} else if(INTERFACE_THEME == 0) {
					out << setw(WIDTH1) << i->first << DL1;
					out << setw(WIDTH2) << i->second << DL2;
				}
			}
		}
		out << endl;
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
			if (colums[c->first] || cols == "*") {
				if(INTERFACE_THEME == 1) {
					setColor(BLACK, WHITE); //Чёрный текст на белом фоне
					out << setw(WIDTH3) << extValue(colHeaders[c->first], c->second) << DL2;
					setColor(BLACK, GRAY_B); //Чёрный текст на тёмно-сером фоне
					if (!save) out << " ";
					setColor(WHITE, BLACK); //Белый текст на чёрном фоне
				} else if(INTERFACE_THEME == 0) {
					out << setw(WIDTH3) << extValue(colHeaders[c->first], c->second);
					out << DL1;
				}
			}
		}
		out << endl;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	out << endl; //Отступ от таблиц
	showMsg(2, "Печать таблицы " + tableName + " завершена");
}
//-----------------------------------------------------------------------------
bool DBTable::printValue(size_t rowNum, string colName, ostream &out)
{
	if (!isColExist(colName)) return 0;
	out << extValue(colHeaders[colName], records[rowNum][colName]) << endl;
	return 1;
}
//-----------------------------------------------------------------------------
bool DBTable::removeRow(size_t rowNum)
{
	if (rowNum >= getSize()) {
		showMsg(1, "Нет строки с номером " + rowNum);
		return 0;
	}
	records.erase(records.begin() += rowNum);
	showMsg(2, "Запись с номером " + to_string(rowNum) + " успешно удалена");
	return 1; // 1 - удаление прошло успешно, 0 - записи не найдено
}
//-----------------------------------------------------------------------------
bool DBTable::insertRow(string line, char *delims)
{
	Row rec; // Начинаем формировать текущую запись
	if (line.size() > MAX_LINE) {
		showMsg(0, "Слишком длинная строка");
		return 0;
	}
	char buff[MAX_LINE]; // Буфер под строку line
	strcpy(buff, line.c_str());
	char *last_token = strtok(buff, delims), *token = strtok(NULL, delims);
	while (token != NULL)
	{
		if (rec.size() > colHeaders.size()-1) { rec.clear(); break; }
		if (!isColExist(last_token)) return 0;
		// getValue требует очистки памяти каждого val в деструкторе
		void *val = getValue(colHeaders[last_token], token);
		rec[last_token] = val;
		last_token = strtok(NULL, delims);
		token = strtok(NULL, delims);
	}
	if (rec.size() != colHeaders.size())
	{
		showMsg(1, "Некорректные данные в строке " + line);
		return 0;
	}
	records.push_back(rec); // Добавляем запись в конец таблицы
	showMsg(2, "Запись успешно добавлена в " + tableName);
	return 1; // 1 - вставка прошла успешно, 0 - произошли сбои
}
//-----------------------------------------------------------------------------
int DBTable::findRow(string colName, const char *value, int start)
{
	if (!isColExist(colName)) return -1;
	void *val = getValue(getColType(colName), value);
	for (auto it = records.begin()+start; it != records.end(); ++it)
	{
		int size = getTypeSize(colHeaders[colName], val);
		int i = memcmp(val, (*it)[colName], size);
		if (i == 0) return (it-records.begin());
	}
	showMsg(1, "Запись со значением " + colName + "=" + string(value) + " не найдена");
	return -1; // -1 - запись в таблице не найдена
}
//-----------------------------------------------------------------------------
int DBTable::findM(string colName, bool isMin) // (ТОЛЬКО ДЛЯ INT)
{
	if (!isColExist(colName)) return -1;
	if (typeCodes[colHeaders[colName]] != 1) 
	{
		showMsg(0, "По столбцу '" + colName + "' нельзя произвести поиск");
		return -1;
	} 
	size_t mi = 0;
	for (size_t j = 1; j < records.size(); ++j)
	{
		int r = rowIntCmp(records[j], records[mi], colName);
		if (isMin ? r < 0 : r > 0) 
			mi = j;
	}
	return *(int*)records[mi][colName];
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
bool DBTable::ValueIsEqual(string colName, void* objA, void* objB)
{
	string type = colHeaders[colName];
	string s1 = extValue(type, objA);
	string s2 = extValue(type, objB);
	return (s1 == s2);
}
//-----------------------------------------------------------------------------
vector<Row> DBTable::getSelfRows(string colName, const char* value)
{
	vector<Row> res;
	int index = -1;
	do
	{
		showMsg(3, to_string(index));
		index = findRow(colName, value, index+1);
		showMsg(3, to_string(index));
		if (index != -1) res.push_back(records[index]);
	} while (index != -1);
	return res;
}
//-----------------------------------------------------------------------------