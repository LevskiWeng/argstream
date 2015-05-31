#ifdef WIN32
#include <Windows.h>
#endif
#include <iostream>
#include <string>
#include "argstream.h"

using namespace std;
enum color
{
#ifdef WIN32
	red		= FOREGROUND_RED | FOREGROUND_INTENSITY,
	green	= FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	blue	= FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	yellow	= FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
	white	= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#else
	red		= 31,
	green	= 32,
	blue	= 34,
	yellow	= 33,
	white	= 0
#endif
};

template <class _Elem, class _Traits>
std::basic_ostream<_Elem,_Traits>& 
operator<<(std::basic_ostream<_Elem,_Traits>& i, const color& c)
{
#ifdef WIN32
    HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout,c);
#else
	argstream::TSTR<_Elem> s = 
		argstream::TSTR<_Elem>::ToString("\033[0;") +
		argstream::TSTR<_Elem>::ToString(itoa(c)) +
		argstream::TSTR<_Elem>::ToString("m");
	i << s;
#endif
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
// Partial Specialization for float
template<>
bool TestEqual(float& v1, float& v2, wchar_t* desc)
{
	bool result = (v1 - v2) < 0.0001; // 0.1%
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

// Partial Specialization for double
template<>
bool TestEqual(double& v1, double& v2, wchar_t* desc)
{
	bool result = (v1 - v2) < 0.0001; //0.1%
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
	bool total_result = true;
	{ //Test string parameter 1
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
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_OK;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_STR1, testStr, L"Test: StringWithoutSpaces");
	}

	{ //Test string parameter 2
		wchar_t* TEST_STR1 = L"String with spaces";
		wchar_t* argv[] = {
			L"test.exe",
			L"-s",
			TEST_STR1
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		wstring testStr;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::parameter(L's', L"testString", testStr, L"desc", false);
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_OK;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_STR1, testStr, L"Test: Parse stringWithSpaces");
	}

	{ //Test string parameter 3
		wchar_t* TEST_STR1 = L"StringWithoutSpaces";
		wstring cmdline = wstring(L"test.exe -s ") + wstring(TEST_STR1);
		wstring testStr;
		argstream::argstream<wchar_t> as(cmdline.c_str());
		as >> argstream::parameter(L's', L"testString", testStr, L"desc", false);
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_OK;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_STR1, testStr, L"Test: Parse command line.");
	}

	{ //Test int parameter
		int TEST_INT = 1234;
		wchar_t* argv[] = {
			L"test.exe",
			L"-s",
			L"1234"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		int testInt = 0;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::parameter(L's', L"testInt", testInt, L"desc", false);
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_OK;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_INT, testInt, L"Test: Parse int.");
	}

	{ //Test float parameter
		float TEST_FLOAT = 1234.01f;
		wchar_t* argv[] = {
			L"test.exe",
			L"-s",
			L"1234.01"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		float testFloat = 0.0f;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::parameter(L's', L"testFloat", testFloat, L"desc", false);
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_OK;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_FLOAT, testFloat, L"Test: Parse float.");

	}

	{ //Test double parameter
		double TEST_FLOAT = 1234.01;
		wchar_t* argv[] = {
			L"test.exe",
			L"-s",
			L"1234.01"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		double testFloat = 0.0;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::parameter(L's', L"testFloat", testFloat, L"desc", false);
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_OK;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_FLOAT, testFloat, L"Test: Parse double.");

	}

	{ //Test options 1
		bool TEST_OPT = true;
		wchar_t* argv[] = {
			L"test.exe",
			L"-s"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		bool testOpt = false;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::option(L's', L"testString", testOpt, L"desc");
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_OK;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_OPT, testOpt, L"Test: Option 1");
	}

	{ //Test options 2
		bool TEST_OPT = false;
		wchar_t* argv[] = {
			L"test.exe"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		bool testOpt = true;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::option(L's', L"testString", testOpt, L"desc");
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_OK;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_OPT, testOpt, L"Test: Option 2");
	}

	{ //Test unused parameters 1
		bool TEST_OPT = false;
		wchar_t* argv[] = {
			L"test.exe",
			L"-s",
			L"-t"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		bool testOpt = true;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::option(L's', L"testString", testOpt, L"desc");
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_ERR_UNUSED_PARAMETER;

		total_result &= TestEqual(res, res2, L"Test: Unused option");
	}

	{ //Test unused parameters 2
		wchar_t* TEST_STR1 = L"StringWithoutSpaces";
		wchar_t* argv[] = {
			L"test.exe",
			L"-s",
			TEST_STR1,
			L"-t"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		wstring testStr;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::parameter(L's', L"testString", testStr, L"desc", false);
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_ERR_UNUSED_PARAMETER;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_STR1, testStr, L"Test: StringWithoutSpaces");
	}

	{ //Test long parameter name
		wchar_t* TEST_STR1 = L"StringWithoutSpaces";
		wchar_t* argv[] = {
			L"test.exe",
			L"--testString",
			TEST_STR1,
			L"--test"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		wstring testStr;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::parameter(L's', L"testString", testStr, L"desc", false);
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_ERR_UNUSED_PARAMETER;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_STR1, testStr, L"Test: Long parameter name");
	}

	{ //Test long option name
		bool TEST_OPT = true;
		wchar_t* argv[] = {
			L"test.exe",
			L"--testString",
			L"--test"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		bool testOpt = false;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::option(L's', L"test", testOpt, L"desc");
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_ERR_UNUSED_PARAMETER;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_OPT, testOpt, L"Test: Long parameter name");
	}
	/* It seems not good
	{ //Test values
		wchar_t* argv[] = {
			L"test.exe",
			L"--testString",
			L"Value1",
			L"Value2",
			L"Value3",
			L"--UnusedParameter",
			L"AnotherValue"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		bool testOpt = false;
		argstream::argstream<wchar_t> as(argc, argv);
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_ERR_UNUSED_PARAMETER;

		total_result &= TestEqual(res, res2, L"Test: Parse argument");
		total_result &= TestEqual(TEST_OPT, testOpt, L"Test: Long parameter name");
	}
	*/

	{ //Test help request 1
		bool TEST_OPT = true;
		wchar_t* argv[] = {
			L"test.exe",
			L"--testString",
			L"-h"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		bool testOpt = false;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::option(L's', L"test", testOpt, L"desc") >> argstream::help<wchar_t>();
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_ERR_HELP_REQUESTED;

		total_result &= TestEqual(res, res2, L"Test: Help requested 1");
	}

	{ //Test help request 2
		bool TEST_OPT = true;
		wchar_t* argv[] = {
			L"test.exe",
			L"-h"
		};
		int argc = sizeof(argv)/sizeof(wchar_t*);
		bool testOpt = false;
		argstream::argstream<wchar_t> as(argc, argv);
		as >> argstream::option(L's', L"test", testOpt, L"desc") >> argstream::help<wchar_t>();
		argstream::RESULT_OF_PARSE res = as.defaultErrorHandling(false);
		argstream::RESULT_OF_PARSE res2 = argstream::RESULT_OF_PARSE::PARSED_ERR_HELP_REQUESTED;

		total_result &= TestEqual(res, res2, L"Test: Help requested 2.");
	}
	cout << "---------------------" << endl;
	if (total_result)
	{
		cout << green << "All passed." << endl;
	}
	else
	{
		cout << red << "Some tests failed." << endl;
	}
	//cin.get();
	if (!total_result)
	{
		return 1; //error occurred
	}
	return 0; //no error
}
