# argstream
A mod of original argstream to support both char and wchar_t string

## Usage

	#include "argstream.h"
	int _tmain(int argc, _TCHAR* argv[])
	{
		argstream::argstream<_TCHAR> as(argc, argv);
		PARAM p = GetGlobalParam();
		typename argstream::TSTR<_TCHAR>::type filePath;
		as >> argstream::parameter(
			_T('f'), //short parameter name
			_T("file"), //long parameter name
			filePath, //variable stores the value
			_T("Full path of file "), //description in argstream::usage()
			false); //the parameter is mandatory or not
		as >> argstream::help<_TCHAR>();
	#ifdef UNICODE
		std::wcout << as.usage();
	#else
		std::cout << as.usage();
	#endif
		as.defaultErrorHandling();
		return 0;
	}

Hope this mod is useful :-)
