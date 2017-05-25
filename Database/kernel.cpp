//-----------------------------------------------------------------------------
// File: kernel.cpp
// Auth: SnipGhost, HeliSRaiN
//                                                 Вспомогательные функции ядра
//-----------------------------------------------------------------------------
#include "kernel.h"
//-----------------------------------------------------------------------------
using namespace Kernel;             // Пространство имен ядра
//-----------------------------------------------------------------------------
ostream *Kernel::logs;              // Поток вывода логов СУБД
char Kernel::STD_DELIMS[20];        // Разделители по умолчанию
map<string, int> Kernel::typeCodes; // Коды зарегестрированных типов
//-----------------------------------------------------------------------------
bool DEBUG_CRIT;                    // Вывод критических сообщений
bool DEBUG_WARN;                    // Вывод предупреждений
bool DEBUG_NORM;                    // Вывод сообщений об успехе
//-----------------------------------------------------------------------------
bool Kernel::INTERFACE_THEME;		// Стиль вывода таблицы
//-----------------------------------------------------------------------------
size_t PRINT_PREC;          // Точность вывода нецелых значений
bool showDateTime;          // Показывать дату и время в сообщениях
char TIME_FORMAT[80];       // Формат вывода даты и времени
//-----------------------------------------------------------------------------
char SIG_CRIT[10];          // Пометка о критическом сбое
char SIG_WARN[10];          // Пометка о предупреждении
char SIG_NORM[10];          // Пометка об успешном заверешении операции
//-----------------------------------------------------------------------------
Row::iterator Row::begin()
{ return data.begin(); }
Row::iterator Row::end()
{ return data.end(); }
Row::const_iterator Row::begin() const
{ return data.begin(); }
Row::const_iterator Row::end() const
{ return data.end(); }
size_t Row::size()
{ return data.size(); }
void Row::clear()
{ data.clear(); }
void*& Row::operator[](string key)
{ return data[key]; }
string Row::extValue(string name, string type)
{ return Kernel::extValue(type, data[name]); }
//-----------------------------------------------------------------------------
string Kernel::getLocalTime(const char *format) // Жуткая Сишная функция
{
	char buffer[80];
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	strftime(buffer, 80, format, timeinfo);
	return string(buffer);
}
//-----------------------------------------------------------------------------
void Kernel::showMsg(int type, string msg, ostream &out)
{
	string t = "";
	if (showDateTime) t = "(" + getLocalTime(TIME_FORMAT) + ") ";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (type == 0 && DEBUG_CRIT) 
	{
		out << SIG_CRIT << t << msg << endl;
		return;
	}
	if (type == 1 && DEBUG_WARN) 
	{
		out << SIG_WARN << t << msg << endl;
		return;
	}
	if (type == 2 && DEBUG_NORM) 
	{
		out << SIG_NORM << t << msg << endl;
		return;
	}
	#ifdef DEBUG_AMSG
		out << "[ ] " << t << msg << endl;
	#endif
}
//-----------------------------------------------------------------------------
size_t Kernel::getTypeSize(string type, void *val)
{
	switch (typeCodes[type])
	{
		case 1: return sizeof(int);
		case 2: return sizeof(double);
		case 3: return sizeof(char)*(strlen((char*)val)+1);
		case 4: return sizeof(DBDate);
		default: return 1;
	}
}
//-----------------------------------------------------------------------------
bool Kernel::readConfig(string path)
{
	// Выставление значений по-умолчанию
	logs = &cout;
	INTERFACE_THEME = 0;
	DEBUG_CRIT = 1;
	DEBUG_WARN = 1;
	DEBUG_NORM = 1; 
	PRINT_PREC = 0;
	strcpy(TIME_FORMAT, "%d.%m.%Y %I:%M:%S");
	showDateTime = 1;
	strcpy(SIG_CRIT, "[!] ");
	strcpy(SIG_WARN, "[-] ");
	strcpy(SIG_NORM, "[+] ");
	typeCodes["Integer"] = 1;
	typeCodes["Real"] = 2;
	typeCodes["String"] = 3;
	typeCodes["Date"] = 4;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ifstream fin(path);
	if (fin.fail()) {
		showMsg(0, "Не удалось прочитать файл конфигурации");
		return 0;
	}
	string erl = "";  // Ошибочные строки
	size_t count = 0;      // Количество строк
	char line[MAX_LINE], *conf_delims = "=";
	while (fin.getline(line, MAX_LINE))
	{
		count++;
		if (strlen(line) <= 1) continue; // пропускаем пустые строки
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		char *key = strtok(line, conf_delims);
		char *val = strtok(NULL, conf_delims);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// пропускаем все комментарии
		if (key[0] == '#' || key[1] == '#') continue; 
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (key[0] == '@') {
			string s = key+1;
			typeCodes[s] = atoi(val);
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (strcmp(key, "DEBUG") == 0) {
			if (val[0] == '0') DEBUG_CRIT = 0;
			if (val[1] == '0') DEBUG_WARN = 0;
			if (val[2] == '0') DEBUG_NORM = 0;
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (strcmp(key, "LOGS") == 0) {
			if (strcmp(val, "cout") == 0) logs = &cout;
			else logs = new ofstream(val);
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (strcmp(key, "INTERFACE_THEME") == 0) {
			if (strcmp(val, "standart") == 0) INTERFACE_THEME = 0;
			else if (strcmp(val, "new") == 0) INTERFACE_THEME = 1;
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (strcmp(key, "SHOW_TIME") == 0) {
			if (strcmp(val, "true") == 0 || strcmp(val, "True") == 0)
				showDateTime = true;
			else
				showDateTime = false;
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (strcmp(key, "TIME_FORMAT") == 0) {
			strcpy(TIME_FORMAT, val);
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (strcmp(key, "STD_DELIMS") == 0) {
			strcpy(STD_DELIMS, val);
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (strcmp(key, "SIG_CRIT") == 0) {
			strcpy(SIG_CRIT, val);
			continue;
		}
		if (strcmp(key, "SIG_WARN") == 0) {
			strcpy(SIG_WARN, val);
			continue;
		}
		if (strcmp(key, "SIG_NORM") == 0) {
			strcpy(SIG_NORM, val);
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (strcmp(key, "PRINT_PRECISION") == 0) {
			PRINT_PREC = atoi(val);
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (count != 1) erl += to_string(count) + ", ";
	}
	showMsg(2, "Успешно загружена конфигурация из файла " + path);
	if (erl.size() > 0) { 
		erl = "Некорректные строки: " + erl.substr(0, erl.size()-2);
		showMsg(1, erl);
	}
	fin.close();
	return 1;
}
//-----------------------------------------------------------------------------
void* Kernel::getValue(string type, const char* value)
{
	void *vp = NULL;
	switch(typeCodes[type])
	{
		case 1:
		{
			int *buffer = new int(atoi(value));
			vp = buffer;
			break;
		}
		case 2:
		{
			double *buffer = new double(atof(value));
			vp = buffer;
			break;
		}
		case 4:
		{
			DBDate *buffer = new DBDate(value);
			vp = buffer;
			break;
		}
		case 3:
		default:
		{
			char *buffer = new char[strlen(value)+1];
			memcpy(buffer, value, strlen(value)+1);
			vp = buffer;
		}
	}
	return vp;
}
//-----------------------------------------------------------------------------
string Kernel::extValue(string type, void *val)
{
	if (!val) return "NULL";
	switch (typeCodes[type]) 
	{
		case 1: 
			return to_string(*((int*)val));
		case 2: 
		{
			stringstream strstr;
			if (PRINT_PREC != 0) 
				strstr << fixed << setprecision(PRINT_PREC);
			strstr << *((double*)val);
			return strstr.str();
		}
		case 3: 
			return string((char*)(val));
		case 4:
			return ((DBDate*)val)->toStr();
		default: showMsg(0, "Несуществующий тип");
	}
	return string("NULL");
}
//-----------------------------------------------------------------------------
int Kernel::rowIntCmp(Row &a, Row &b, string s)
{
	return (*(int*)a[s] - *(int*)b[s]);
}
//-----------------------------------------------------------------------------
int Kernel::rowDouCmp(Row &a, Row &b, string s)
{
	return ((*(double*)a[s] - *(double*)b[s]) > 0) ? 1 : -1;
}
//-----------------------------------------------------------------------------
int Kernel::rowStrCmp(Row &a, Row &b, string s)
{
	return strcmp((char*)a[s], (char*)b[s]);
}
//-----------------------------------------------------------------------------