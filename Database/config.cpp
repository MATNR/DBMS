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
bool showDateTime = true;   // Показывать дату и время в сообщениях
char *STD_DELIMS = NULL;    // Разделители по умолчанию
char *SIG_CRIT = NULL;      // Пометка о критическом сбое
char *SIG_WARN = NULL;      // Пометка о предупреждении
char *SIG_NORM = NULL;      // Пометка об успешном заверешении операции
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
	if (showDateTime) t = "(" + getLocTime("%d.%m.%Y %I:%M:%S") + ") ";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (!SIG_CRIT) SIG_CRIT = "[!] ";
	if (!SIG_WARN) SIG_WARN = "[-] ";
	if (!SIG_NORM) SIG_NORM = "[+] ";
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
void readConfig(string path)
{
	// TODO: Сделать загрузку из файла конфигурации
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//logs = &cout;
	logs = new ofstream("database.log");
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	showDateTime = true;
	STD_DELIMS = "|";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	SIG_CRIT = "[!] ";
	SIG_WARN = "[-] ";
	SIG_NORM = "[+] ";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	typeCodes["Integer"] = 1;
	typeCodes["Float"] = 2;
	typeCodes["Double"] = 2;
	typeCodes["String"] = 3;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	showMsg(2, "Успешно загружена конфигурация из файла " + path);
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