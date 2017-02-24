//-----------------------------------------------------------------------------
// File: config.cpp
// Auth: SnipGhost
//                                                 Вспомогательные функции ядра
//-----------------------------------------------------------------------------
#include "config.h"
//-----------------------------------------------------------------------------
ostream *logs;              // Поток вывода логов СУБД
//-----------------------------------------------------------------------------
map<string, int> typeCodes; // Коды зарегестрированных типов
bool showDateTime;   // Показывать дату и время в сообщениях
char TIME_FORMAT[80];   // Формат вывода даты и времени
char STD_DELIMS[20];    // Разделители по умолчанию
char SIG_CRIT[10];      // Пометка о критическом сбое
char SIG_WARN[10];      // Пометка о предупреждении
char SIG_NORM[10];      // Пометка об успешном заверешении операции
//-----------------------------------------------------------------------------
string getLocTime(const char *format) // Жуткая Сишная функция
{
	char buffer[80];
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	strftime(buffer, 80, format, timeinfo);
	return string(buffer);
}
//-----------------------------------------------------------------------------
void showMsg(int type, string msg, ostream &out)
{
	string t = "";
	if (showDateTime) t = "(" + getLocTime(TIME_FORMAT) + ") ";
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
}
//-----------------------------------------------------------------------------
size_t getTypeSize(string type, void *val)
{
	switch (typeCodes[type])
	{
		case 1: return sizeof(int);
		case 2: return sizeof(double);
		case 3: return sizeof(char)*strlen((char*)val);
		default: return 1;
	}
}
//-----------------------------------------------------------------------------
bool readConfig(string path)
{
	// Выставление значений по-умолчанию
	logs = &cout;
	strcpy(TIME_FORMAT, "%d.%m.%Y %I:%M:%S");
	showDateTime = true;
	strcpy(SIG_CRIT, "[!] ");
	strcpy(SIG_WARN, "[-] ");
	strcpy(SIG_NORM, "[+] ");
	typeCodes["Integer"] = 1;
	typeCodes["Real"] = 2;
	typeCodes["String"] = 3;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ifstream fin(path);
	if (fin.fail()) {
		showMsg(0, "Не удалось прочитать файл конфигурации");
		return 0;
	}
	char line[MAX_LINE], *conf_delims = "=";
	while (fin.getline(line, MAX_LINE))
	{
		if (strlen(line) <= 1) continue; // пропускаем пустые строки
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		char *key = strtok(line, conf_delims);
		char *val = strtok(NULL, conf_delims);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (key[0] == '#') continue;    // пропускаем все комментарии
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (key[0] == '@') {
			string s = key+1;
			typeCodes[s] = atoi(val);
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (strcmp(key, "LOGS") == 0) {
			if (strcmp(val, "cout") == 0) logs = &cout;
			else logs = new ofstream(val);
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
		if (strcmp(key, "SIG_CRIT") == 0) {
			strcpy(SIG_NORM, val);
			continue;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}
	showMsg(2, "Успешно загружена конфигурация из файла " + path);
	return 1;
}
//-----------------------------------------------------------------------------
void* getValue(string type, char* value) // Переводит строку в соотв. тип
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
string extValue(string type, void *val)
{
	switch (typeCodes[type]) 
	{
		case 1: return to_string(*((int*)val));
		case 2: return to_string(*((double*)val));
		case 3: return string((char*)(val));
		default: showMsg(0, "Несуществующий тип");
	}
	return string("NULL");
}
//-----------------------------------------------------------------------------
int rowIntCmp(Row &a, Row &b, string s)
{
	return (*(int*)a[s] - *(int*)b[s]);
}
//-----------------------------------------------------------------------------
int rowDouCmp(Row &a, Row &b, string s)
{
	return ((*(double*)a[s] - *(double*)b[s]) > 0) ? 1 : -1;
}
//-----------------------------------------------------------------------------
int rowStrCmp(Row &a, Row &b, string s)
{
	return strcmp((char*)a[s], (char*)b[s]);
}
//-----------------------------------------------------------------------------