//-----------------------------------------------------------------------------
// File: dbtable.h
// Auth: SnipGhost
//                                            ��������� ������ DBTable � ������
//-----------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
typedef map<string, string> Header;            // ��� ��������� �������
typedef map<string, string>::iterator It_head; // ��� ��������� �� ���������
typedef map<string, void*> Row;                // ��� ����� ������ � �������
typedef map<string, void*>::iterator It_body;  // ��� ��������� �� ������
//-----------------------------------------------------------------------------
const int MAX_LINE = 255; // ������������ ����� ������ �����
//-----------------------------------------------------------------------------
class DBTable
{
 private:
	 string tableName;    // ��� �������
	 Header colHeaders;   // ��������� ������� (����� �������� + ��� ��������)
	 vector<Row> records; // ������ �������
 public:
	 DBTable();
	 DBTable(string path);
	 ~DBTable();
	 bool readFromFile(string path);
	 void printTable();
	 //Row getRecord(string value, string colName);
};
//-----------------------------------------------------------------------------