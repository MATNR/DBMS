//-----------------------------------------------------------------------------
// File: config.cpp
// Auth: SnipGhost
//                                                 Вспомогательные функции ядра
//-----------------------------------------------------------------------------
#include "config.h"
//-----------------------------------------------------------------------------
bool DebugMsg = 1;
//-----------------------------------------------------------------------------
map<string, int> typeCodes; // Коды зарегестрированных типов
char *STD_DELIMS = NULL;    // Разделители по умолчанию
char *SIG_CRIT = NULL;      // Пометка о критическом сбое
char *SIG_WARN = NULL;      // Пометка о предупреждении
char *SIG_NORM = NULL;      // Пометка об успешном заверешении операции
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
void showMsg(int type, string msg)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (!SIG_CRIT) SIG_CRIT = "[!] ";
	if (!SIG_WARN) SIG_WARN = "[-] ";
	if (!SIG_NORM) SIG_NORM = "[+] ";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (type == 0 && DEBUG_CRIT) 
	{
		cout << SIG_CRIT << msg << endl;
		return;
	}
	if (type == 1 && DEBUG_WARN) 
	{
		cout << SIG_WARN << msg << endl;
		return;
	}
	if (type == 2 && DEBUG_NORM) 
	{
		cout << SIG_NORM << msg << endl;
		return;
	}
}
//-----------------------------------------------------------------------------
void readConfig(string path)
{
	// TODO: Сделать загрузку из файла конфигурации
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
void extValue(string type, void *val, ostream &out)
{
	switch (typeCodes[type]) 
	{
		case 1:
			out << *((int*)val);
			break;
		case 2:
			out << *((double*)val);
			break;
		case 3:
		default:
			out << (char*)(val);
	}
}
//-----------------------------------------------------------------------------