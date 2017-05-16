//-----------------------------------------------------------------------------
// File: dbdate.h
// Auth: SnipGhost
//                                                      Структура класса DBDate
//-----------------------------------------------------------------------------
#pragma once
#ifndef DBDATE_H
#define DBDATE_H
//-----------------------------------------------------------------------------
#include <string>
using namespace std;
//-----------------------------------------------------------------------------
#define MAX_LEN 128
//-----------------------------------------------------------------------------
class DBDate
{
 private:
	 int d;  // Day
	 int m;  // Month
	 int y;  // Year
	 bool f; // Fail-flag
	 bool check()
	 {
		if (m < 1 || m > 12) return 0;
		if (d < 1 || d > getDaysInMounth(m, y)) return 0;
		return 1;
	 }
 public:
	 static int getDaysInMounth(const int x, const int y)
	 {
		if ((!(y % 4) && (y % 100) || !(y % 400)) && (x == 2)) return 29;
		return (28 + (x + (x/8)) % 2 + 2 % x + 2 * (1/x));
	 }
	 DBDate(): d(1), m(1), y(0) {}
	 ~DBDate() {}
	 const bool fail() { return f; }
	 DBDate(const char *str, const char *delims = ".")
	 {
		char line[MAX_LEN];
		strcpy(line, str);
		char *token = strtok(line, delims);
		d = atoi(token);
		token = strtok(NULL, delims);
		m = atoi(token);
		token = strtok(NULL, delims);
		y = atoi(token);
		f = check();
	 }
	 friend const bool DBDate::operator>(const DBDate &a, const DBDate &b)
	 {
		 if (a.y > b.y) return 1;
		 if (a.y < b.y) return 0;
		 if (a.m > b.m) return 1;
		 if (a.m < b.m) return 0;
		 if (a.d > b.d) return 1;
		 return 0;
	 }
	 friend const bool DBDate::operator<(const DBDate &b, const DBDate &a)
	 {
		 if (a.y > b.y) return 1;
		 if (a.y < b.y) return 0;
		 if (a.m > b.m) return 1;
		 if (a.m < b.m) return 0;
		 if (a.d > b.d) return 1;
		 return 0;
	 }
	 friend const bool DBDate::operator==(const DBDate &a, const DBDate &b)
	 {
		 return ((a.y == b.y)&&(a.m == b.m)&&(a.d == b.d));
	 }
	 friend const bool DBDate::operator>=(const DBDate &a, const DBDate &b)
	 {
		 return ((a > b) || (a == b));
	 }
	 friend const bool DBDate::operator<=(const DBDate &a, const DBDate &b)
	 {
		 return ((a < b) || (a == b));
	 }
	 friend const DBDate DBDate::operator+(const DBDate &a, const int days)
	 {
		DBDate tmp = a;
		tmp.d += days;
		while (tmp.d > getDaysInMounth(tmp.m, tmp.y))
		{
			tmp.d -= getDaysInMounth(tmp.m, tmp.y);
			tmp.m++;
			while (tmp.m > 12)
			{
				tmp.m -= 12;
				tmp.y++;
			}
		}
		return tmp;
	 }
	 friend const DBDate DBDate::operator+(const DBDate &a, const DBDate &b)
	 {	
		DBDate tmp = a;
		tmp.d += b.d;
		tmp.m += b.m;
		tmp.y += b.y;
		while (tmp.d > getDaysInMounth(tmp.m, tmp.y))
		{
			tmp.d -= getDaysInMounth(tmp.m, tmp.y);
			tmp.m++;
			while (tmp.m > 12)
			{
				tmp.m -= 12;
				tmp.y++;
			}
		}		
		return tmp;
	 }
	 friend const DBDate DBDate::operator-(const DBDate &a, const int days)
	 {
		DBDate tmp = a;
		tmp.d -= days;
		while (tmp.d < 1)
		{
			tmp.m--;		
			while (tmp.m < 1)
			{
				tmp.y--;
				tmp.m -= 12;
			}
			tmp.d += getDaysInMounth(tmp.m, tmp.y);
		}
		return tmp;
	 }
	 friend const int DBDate::operator-(const DBDate &a, const DBDate &b)
	 {	
		if (a == b) return 0;
		DBDate t1 = a, t2 = b;
		int i1 = t1.d, i2 = t2.d;
		t1.d = 0;
		t2.d = 0;
		while (t1.y > 0)
		{	
			while (t1.m > 0)
			{
				i1 += getDaysInMounth(t1.m, t1.y);
				t1.m--;
			}
			t1.y--;
			t1.m = 12;
		}
		while (t2.y > 0)
		{	
			while (t2.m > 0)
			{
				i2 += getDaysInMounth(t2.m, t2.y);
				t2.m--;
			}
			t2.y--;
			t2.m = 12;
		}
		return (i1-i2);
	 }
	 friend ostream &operator<<(ostream &os, const DBDate &x)
	 { 
		 return os << x.d << '.' << x.m << '.' << x.y;
	 }
	 string toStr()
	 {
		 return (to_string(d) + '.' + to_string(m) + '.' + to_string(y));
	 }
};
//-----------------------------------------------------------------------------
#endif /* DBDATE_H */
//-----------------------------------------------------------------------------