# argstream
The purpose of this mod of [argstream](http://maverick.inria.fr/Membres/Xavier.Decoret/resources/argstream/index.html) is to extend argstream and support both char and wchar_t string.

## Usage

``` c++
#include "argstream.h"
int _tmain(int argc, _TCHAR* argv[])
{
	argstream::argstream<_TCHAR> as(argc, argv);
	argstream::TSTR<_TCHAR>::type filePath(_T("C:\\default_file.name"));
	int size;
	as >> argstream::parameter(
		_T('f'), //short parameter name
		_T("file"), //long parameter name
		filePath, //variable stores the value
		_T("Full path of file "), //description in argstream::usage()
		false) //the parameter is not mandatory
	   >> argstream::paramater(_T('s'), // another parameter
		_T("size"),
		size,
		_T("File size "),
		true); //the parameter is mandatory
	as >> argstream::help<_TCHAR>(); //Add help parameter
#ifdef UNICODE
	std::wcout << as.usage();
#else
	std::cout << as.usage();
#endif
	as.defaultErrorHandling();

	//TODO: Write your code here

	return 0;
}
```
Hope this mod is useful for you :-)
