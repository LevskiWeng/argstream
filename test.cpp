#include <Windows.h>
#include <iostream>
#include <string>
#include "argstream.h"

using namespace std;
enum color
{
	red=FOREGROUND_RED|FOREGROUND_INTENSITY,
	green=FOREGROUND_GREEN|FOREGROUND_INTENSITY,
	blue=FOREGROUND_BLUE|FOREGROUND_INTENSITY,
	yellow=FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY,
	white=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE
};

template <class _Elem, class _Traits>
std::basic_ostream<_Elem,_Traits>& 
operator<<(std::basic_ostream<_Elem,_Traits>& i, const color& c)
{
    HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout,c);
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
		int argc = sizeof(argv)/sizeof(wchar_t*);
		wstring testStr;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::parameter(L's', L"testString", testStr, L"desc", false);
		as.defaultErrorHandling(true);

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
