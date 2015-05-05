#include <Windows.h>
#include <iostream>
#include <string>
#include "argstream.h"

using namespace std;

inline std::ostream& blue(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
              |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& red(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
                FOREGROUND_RED|FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& green(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
              FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& yellow(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
         FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& white(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
       FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
    return s;
}

struct color {
    color(WORD attribute):m_color(attribute){};
    WORD m_color;
};

template <class _Elem, class _Traits>
std::basic_ostream<_Elem,_Traits>& 
      operator<<(std::basic_ostream<_Elem,_Traits>& i, color& c)
{
    HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout,c.m_color);
    return i;
}

template<typename T1, typename T2>
bool TestEqual(T1& v1, T2& v2, wchar_t* desc)
{
	bool result = (v1 == v2);
	if (result)
	{
		wcout <<green << L"[Passed] " << white << desc << endl;
	}
	else
	{
		wcout << red << L"[Failed] " << white << desc << endl;
	}

	return result;
}

int wmain()
{
	{ //Test string parameter
		wchar_t* TEST_STR1 = L"StringWithoutSpaces";
		wchar_t* argv[] = {
			L"test.exe",
			L"-s",
			TEST_STR1
		};
		int argc = sizeof(argv);
		wstring testStr;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::parameter(L's', testStr, L"desc", false);
		as.defaultErrorHandling();

		TestEqual(TEST_STR1, testStr, L"Test: StringWithoutSpaces");
		
	}

	{ //Test int parameter

	}

	{ //Test float parameter

	}

	{ //Test bool parameter

	}

	cin.get();
	return 0;
}
