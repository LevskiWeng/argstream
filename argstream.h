/* Copyright (C) 2004 Xavier Décoret <Xavier.Decoret@imag.fr>
*  Portions Copyright (C) 2015 Levski Weng <levskiweng@gmail.com>
*
* argsteam is a free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* 
* argstream is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with argstream; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#ifndef ARGSTREAM_H
#define ARGSTREAM_H

#include <Windows.h>

#include <string>
#include <list>
#include <deque>
#include <map>
#include <stdexcept>
#include <sstream>
#include <iostream> 

namespace argstream
{
	template<typename CHARTYPE>
	struct TSTR
	{
		typedef std::basic_string<CHARTYPE, std::char_traits<CHARTYPE>, std::allocator<CHARTYPE>> type;

		static std::basic_string<CHARTYPE, std::char_traits<CHARTYPE>, std::allocator<CHARTYPE>> ToString(char* s);
		static std::basic_string<CHARTYPE, std::char_traits<CHARTYPE>, std::allocator<CHARTYPE>> ToString(wchar_t* s);
		
	};

	template<>
	std::wstring TSTR<wchar_t>::ToString(char* s)
	{
		wchar_t buf[MAX_PATH+1] = {0};

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s, strlen(s), buf, sizeof(buf)/sizeof(buf[0]));

		return std::wstring(buf);
	}

	template<>
	std::wstring TSTR<wchar_t>::ToString(wchar_t* s)
	{
		return std::wstring(s);
	}

	template<>
	std::string TSTR<char>::ToString(char* s)
	{
		return std::string(s);
	}

	template<>
	std::string TSTR<char>::ToString(wchar_t* s)
	{
		char buf[MAX_PATH+1] = {0};

		WideCharToMultiByte(CP_ACP, MB_PRECOMPOSED, s, wcslen(s), buf, sizeof(buf)/sizeof(buf[0]), NULL, NULL);

		return std::string(buf);
	}

	template<typename CHARTYPE>
	struct TSTRSTREAM
	{
		typedef std::basic_istringstream<CHARTYPE, std::char_traits<CHARTYPE>,std::allocator<CHARTYPE>> I;
		typedef std::basic_ostringstream<CHARTYPE, std::char_traits<CHARTYPE>, std::allocator<CHARTYPE>> O;
	};

	template<typename CHARTYPE> class argstream;

	template<typename CHARTYPE, typename T>	class ValueHolder;
	template<typename CHARTYPE, typename T, typename O> class ValuesHolder;
	template<typename CHARTYPE, typename T> argstream<CHARTYPE>& operator>> (argstream<CHARTYPE>&, const ValueHolder<CHARTYPE, T>&);
	template<typename CHARTYPE, typename T, typename O> argstream<CHARTYPE>& operator>> (argstream<CHARTYPE>&, const ValuesHolder<CHARTYPE, T, O>&);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Interface of ValueHolder<CHARTYPE, T>
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	template<typename CHARTYPE, typename T>
	class ValueHolder
	{
	public:
		ValueHolder(CHARTYPE s,
			const CHARTYPE* l,
			T& b,
			const CHARTYPE* desc,
			bool mandatory);
		ValueHolder(const CHARTYPE* l,
			T& b,
			const CHARTYPE* desc,
			bool mandatory);
		ValueHolder(CHARTYPE s,
			T& b,
			const CHARTYPE* desc,
			bool mandatory);
		friend argstream<CHARTYPE>& operator>><CHARTYPE, T> (argstream<CHARTYPE>& s,const ValueHolder<CHARTYPE, T>& v);
		typename TSTR<CHARTYPE>::type name() const;
		typename TSTR<CHARTYPE>::type description() const;
	private:
		typename TSTR<CHARTYPE>::type shortName_;
		typename TSTR<CHARTYPE>::type longName_;
		T* value_;
		T initialValue_;
		typename TSTR<CHARTYPE>::type description_;  
		bool mandatory_;
	};
	template <typename CHARTYPE, typename T>
	inline ValueHolder<CHARTYPE, T>
		parameter(CHARTYPE s,
		const CHARTYPE* l,
		T& b,
		const CHARTYPE* desc,
		bool mandatory = true)
	{
		return ValueHolder<CHARTYPE, T>(s,l,b,desc,mandatory);
	}
	template <typename CHARTYPE, typename T>
	inline ValueHolder<CHARTYPE, T>
		parameter(CHARTYPE s,
		T& b,
		const CHARTYPE* desc,
		bool mandatory = true)
	{
		return ValueHolder<CHARTYPE, T>(s, b, desc, mandatory);
	}
	template <typename CHARTYPE, typename T>
	inline ValueHolder<CHARTYPE, T>
		parameter(const CHARTYPE* l,
		T& b,
		const CHARTYPE* desc,
		bool mandatory = true)
	{
		return ValueHolder<CHARTYPE, T>(l, b, desc, mandatory);
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Interface of OptionHolder
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	template <typename CHARTYPE>
	class OptionHolder
	{
	public:
		inline OptionHolder(CHARTYPE s,
			const CHARTYPE* l,
			bool& b,
			const CHARTYPE* desc);
		inline OptionHolder(const CHARTYPE* l,
			bool& b,
			const CHARTYPE* desc);
		inline OptionHolder(CHARTYPE s,
			bool& b,
			const CHARTYPE* desc);
		template<typename CHARTYPE>
		friend argstream<CHARTYPE>& operator>>(argstream<CHARTYPE>& s,const OptionHolder<CHARTYPE>& v);
		inline typename TSTR<CHARTYPE>::type name() const;
		inline typename TSTR<CHARTYPE>::type description() const;
	protected:
		inline OptionHolder(CHARTYPE s,
			const CHARTYPE* l,
			const CHARTYPE* desc);

		template<typename CHARTYPE>
		friend OptionHolder<CHARTYPE> help(CHARTYPE s, //default value: 0
			const CHARTYPE* l, //default value: NULL
			const CHARTYPE* desc); //default value: NULL
	private:
		typename TSTR<CHARTYPE>::type shortName_;
		typename TSTR<CHARTYPE>::type longName_;
		bool* value_;
		typename TSTR<CHARTYPE>::type description_;  
	};
	template<typename CHARTYPE>
	inline OptionHolder<CHARTYPE> option(
		CHARTYPE s,
		const CHARTYPE* l,
		bool& b,
		const CHARTYPE* desc)
	{
		return OptionHolder<CHARTYPE>(s, l, b, desc);
	}
	template<typename CHARTYPE>
	inline OptionHolder<CHARTYPE> option(
		CHARTYPE s,
		bool& b,
		const CHARTYPE* desc)
	{
		return OptionHolder<CHARTYPE>(s, b, desc);
	}
	template<typename CHARTYPE>
	inline OptionHolder<CHARTYPE> option(
		const CHARTYPE* l,
		bool& b,
		const CHARTYPE* desc)
	{
		return OptionHolder<CHARTYPE>(l, b, desc);
	}
	template<typename CHARTYPE>
	inline OptionHolder<CHARTYPE> help(
		CHARTYPE s = 0,
		const CHARTYPE* l = NULL,
		const CHARTYPE* desc = NULL)
	{
		CHARTYPE sDefault = 'h';
		typename TSTR<CHARTYPE>::type lDefault = TSTR<CHARTYPE>::ToString("help");
		typename TSTR<CHARTYPE>::type descDefault = TSTR<CHARTYPE>::ToString("Display this help");
		return OptionHolder<CHARTYPE>(s ? s : sDefault, l ? l : lDefault.c_str(), desc ? desc : descDefault.c_str());
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Interface of ValuesHolder
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	template<typename CHARTYPE, typename T, typename O>
	class ValuesHolder
	{
	public:
		ValuesHolder(const O& o,
			const CHARTYPE* desc,
			int len);
		template<typename CHARTYPE, typename T, typename O>
		friend argstream& operator>><CHARTYPE, T, O>(argstream& s, const ValuesHolder<CHARTYPE, T, O>& v);
		typename TSTR<CHARTYPE>::type name() const;
		typename TSTR<CHARTYPE>::type description() const;
		typedef T value_type;
	private:
		mutable O   value_;
		typename TSTR<CHARTYPE>::type description_;
		int         len_;
		_TCHAR        letter_;
	};
	template<typename CHARTYPE, typename T, typename O>
	inline ValuesHolder<CHARTYPE, T, O>
		values(const O& o,
		const CHARTYPE* desc,
		int len=-1)
	{
		return ValuesHolder<CHARTYPE, T, O>(o, desc, len);
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Interface of ValueParser
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	template <typename CHARTYPE, class T>
	class ValueParser
	{
	public:
		inline T operator()(const typename TSTR<CHARTYPE>::type& s) const
		{
			typename TSTRSTREAM<CHARTYPE>::I is(s);
			T t;
			is>>t;
			return t;
		}
	};
	// We need to specialize for string otherwise parsing of a value that
	// contains space (for example a string with space passed in quotes on the
	// command line) would parse only the first element of the value!!!
	template <>
	class ValueParser<char, std::string>
	{
	public:
		inline std::string operator()(std::string& s) const
		{
			return s;
		}
	};  
	template <>
	class ValueParser<wchar_t, std::wstring>
	{
	public:
		inline std::wstring operator()(std::wstring& s) const
		{
			return s;
		}
	};  
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Interface of argstream
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	template<typename CHARTYPE>
	class argstream
	{
	public:
		inline argstream<CHARTYPE>(int argc,CHARTYPE** argv);
		inline argstream<CHARTYPE>(const CHARTYPE* c);
		template<typename CHARTYPE, typename T> friend argstream<CHARTYPE>& operator>>(argstream<CHARTYPE>& s,const ValueHolder<CHARTYPE, T>& v);
		template<typename CHARTYPE> friend inline argstream<CHARTYPE>& operator>>(argstream<CHARTYPE>& s,const OptionHolder<CHARTYPE>& v);
		template<typename CHARTYPE, typename T, typename O> friend argstream<CHARTYPE>& operator>>(argstream<CHARTYPE>& s,const ValuesHolder<CHARTYPE, T,O>& v);

		inline bool helpRequested() const;
		inline bool isOk() const;
		inline typename TSTR<CHARTYPE>::type errorLog() const;
		inline typename TSTR<CHARTYPE>::type usage() const;
		inline void defaultErrorHandling(bool ignoreUnused=false) const;
		static inline CHARTYPE uniqueLetter();
	protected:
		void parse(int argc, CHARTYPE** argv);
	private:
		typedef typename std::list<typename TSTR<CHARTYPE>::type>::iterator value_iterator;
		typedef typename std::pair<typename TSTR<CHARTYPE>::type, typename TSTR<CHARTYPE>::type> help_entry;
		typename TSTR<CHARTYPE>::type progName_;
		std::map<typename TSTR<CHARTYPE>::type, value_iterator> options_;
		std::list<typename TSTR<CHARTYPE>::type> values_;
		bool minusActive_;
		bool isOk_;
		std::deque<std::pair<typename TSTR<CHARTYPE>::type, typename TSTR<CHARTYPE>::type>> argHelps_;
		typename TSTR<CHARTYPE>::type cmdLine_;
		std::deque<typename TSTR<CHARTYPE>::type> errors_; 
		bool helpRequested_;
	};
	//************************************************************
	// Implementation of ValueHolder<CHARTYPE, T>
	//************************************************************
	template<typename CHARTYPE, typename T> 
	ValueHolder<CHARTYPE, T>::ValueHolder(CHARTYPE s,
		const CHARTYPE* l,
		T& v,
		const CHARTYPE* desc,
		bool mandatory)
		:  shortName_(1,s),
		longName_(l),
		value_(&v),
		initialValue_(v),
		description_(desc),
		mandatory_(mandatory)
	{
	}
	template<typename CHARTYPE, typename T>
	ValueHolder<CHARTYPE, T>::ValueHolder(const CHARTYPE* l,
		T& v,
		const CHARTYPE* desc,
		bool mandatory)
		:  longName_(l),
		value_(&v),
		initialValue_(v),
		description_(desc),
		mandatory_(mandatory)
	{
	}
	template<typename CHARTYPE, typename T>
	ValueHolder<CHARTYPE, T>::ValueHolder(CHARTYPE s,
		T& v,
		const CHARTYPE* desc,
		bool mandatory)
		:  shortName_(1,s),
		value_(&v),
		initialValue_(v),
		description_(desc),
		mandatory_(mandatory)
	{
	}
	template<typename CHARTYPE, typename T>
	typename TSTR<CHARTYPE>::type ValueHolder<CHARTYPE, T>::name() const
	{
		typename TSTRSTREAM<CHARTYPE>::O os;
		if (!shortName_.empty()) os<<'-'<<shortName_;
		if (!longName_.empty()) {      
			if (!shortName_.empty()) os<<'/';
			os<<TSTR<CHARTYPE>::ToString("--")<<longName_;
		}
		return os.str();
	}
	template<typename CHARTYPE, typename T>
	typename TSTR<CHARTYPE>::type ValueHolder<CHARTYPE, T>::description() const
	{
		typename TSTRSTREAM<CHARTYPE>::O os;
		os<<description_;
		if (mandatory_)
		{
			os<<TSTR<CHARTYPE>::ToString("(mandatory)");
		}
		else
		{
			os<<_T("(default=")<<initialValue_<<_T(")");
		}
		return os.str();
	}
	//************************************************************
	// Implementation of OptionHolder
	//************************************************************
	template<typename CHARTYPE>
	inline OptionHolder<CHARTYPE>::OptionHolder(CHARTYPE s,
		const CHARTYPE* l,
		bool& b,
		const CHARTYPE* desc)
		: shortName_(1,s),
		longName_(l),
		value_(&b),
		description_(desc)
	{
	}

	template<typename CHARTYPE>
	inline OptionHolder<CHARTYPE>::OptionHolder(const CHARTYPE* l,
		bool& b,
		const CHARTYPE* desc)
		: longName_(l),
		value_(&b),
		description_(desc)
	{
	}

	template<typename CHARTYPE>
	inline OptionHolder<CHARTYPE>::OptionHolder(CHARTYPE s,
		bool& b,
		const CHARTYPE* desc)
		: shortName_(1,s),
		value_(&b),
		description_(desc)
	{
	}

	template<typename CHARTYPE>
	inline OptionHolder<CHARTYPE>::OptionHolder(CHARTYPE s,
		const CHARTYPE* l,
		const CHARTYPE* desc)
		: shortName_(1,s),
		longName_(l),
		value_(NULL),
		description_(desc)
	{
	}

	template<typename CHARTYPE>
	inline typename TSTR<CHARTYPE>::type OptionHolder<CHARTYPE>::name() const
	{
		typename TSTRSTREAM<CHARTYPE>::O os;
		if (!shortName_.empty()) os<<'-'<<shortName_;
		if (!longName_.empty())
		{
			if (!shortName_.empty()) os<<'/';
			os<<TSTR<CHARTYPE>::ToString("--")<<longName_;
		}
		return os.str();
	}

	template<typename CHARTYPE>
	inline typename TSTR<CHARTYPE>::type OptionHolder<CHARTYPE>::description() const
	{
		return description_;
	}
	//************************************************************
	// Implementation of ValuesHolder<T,O>
	//************************************************************
	template<typename CHARTYPE, typename T, typename O>
	ValuesHolder<CHARTYPE, T, O>::ValuesHolder(const O& o,
		const CHARTYPE* desc,
		int len)
		: value_(o),
		description_(desc),
		len_(len)
	{
		letter_ = argstream::uniqueLetter();
	}
	template <typename CHARTYPE, typename T, typename O>
	typename TSTR<CHARTYPE>::type ValuesHolder<CHARTYPE, T, O>::name() const
	{
		TSTRSTREAM<CHARTYPE>::O os;
		os<< letter_ <<TSTR<CHARTYPE>::ToString("i");
		return os.str();
	}
	template <typename CHARTYPE, typename T, typename O>
	typename TSTR<CHARTYPE>::type ValuesHolder<CHARTYPE, T, O>::description() const
	{
		return description_;
	}
	//************************************************************
	// Implementation of argstream
	//************************************************************
	template<typename CHARTYPE>
	inline argstream<CHARTYPE>::argstream(int argc,CHARTYPE** argv)
		: progName_(argv[0]),
		minusActive_(true),
		isOk_(true)
	{
		parse(argc,argv);
	}

	template<typename CHARTYPE>
	inline argstream<CHARTYPE>::argstream(const CHARTYPE* c)
		: progName_(TSTR<CHARTYPE>::ToString("")),
		minusActive_(true),
		isOk_(true)
	{
		typename TSTR<CHARTYPE>::type s(c);
		// Build argc, argv from s. We must add a dummy first element for
		// progName because parse() expects it!!
		std::deque<typename TSTR<CHARTYPE>::type> args;
		args.push_back(TSTR<CHARTYPE>::ToString(""));
		typename TSTRSTREAM<CHARTYPE>::I is(s);
		while (is.good())
		{
			typename TSTR<CHARTYPE>::type t;
			is>>t;
			args.push_back(t);
		}
		CHARTYPE* pargs = new CHARTYPE[args.size()];
		CHARTYPE** p = &pargs;
		for (std::deque<typename TSTR<CHARTYPE>::type>::const_iterator
			iter = args.begin();
			iter != args.end();++iter)
		{
			*p++ = const_cast<_TCHAR*>(iter->c_str());
		}
		parse(args.size(), &pargs);
		delete[] pargs;
	}

	template<typename CHARTYPE>
	inline void	argstream<CHARTYPE>::parse(int argc,CHARTYPE** argv)
	{
		// Run thru all arguments.
		// * it has -- in front : it is a long name option, if remainder is empty,
		//                        it is an error
		// * it has - in front  : it is a sequence of short name options, if
		//                        remainder is empty, deactivates option (- will
		//                        now be considered a _TCHAR).
		// * if any other _TCHAR, or if option was deactivated
		//                      : it is a value. Values are split in parameters
		//                      (immediately follow an option) and pure values.
		// Each time a value is parsed, if the previously parsed argument was an
		// option, then the option is linked to the value in case of it is a
		// option with parameter.  The subtle point is that when several options
		// are given with short names (ex: -abc equivalent to -a -b -c), the last
		// parsed option is -c).
		// Since we use map for option, any successive call overides the previous
		// one: foo -a -b -a hello is equivalent to foo -b -a hello
		// For values it is not true since we might have several times the same
		// value. 
		value_iterator* lastOption = NULL;
		for (CHARTYPE** a = argv,**astop=a+argc;++a!=astop;)
		{
			typename TSTR<CHARTYPE>::type s(*a);
			if (minusActive_ && s[0] == '-')
			{
				if (s.size() > 1 && s[1] == '-')
				{
					if (s.size() == 2)
					{
						minusActive_ = false;
						continue;
					}
					lastOption = &(options_[s.substr(2)] = values_.end());
				}
				else 
				{
					if (s.size() > 1)
					{
						// Parse all _TCHARs, if it is a minus we have an error
						for (typename TSTR<CHARTYPE>::type::const_iterator cter = s.begin();
							++cter != s.end();)
						{
							if (*cter == '-')
							{
								isOk_ = false;
								typename TSTRSTREAM<CHARTYPE>::O os;
								os<<TSTR<CHARTYPE>::ToString("- in the middle of a switch ")<<a;
								errors_.push_back(os.str());
								break;
							}
							lastOption = &(options_[TSTR<CHARTYPE>::type(1,*cter)] = values_.end());
						}
					}
					else
					{
						isOk_ = false;
						errors_.push_back(TSTR<CHARTYPE>::ToString("Invalid argument -"));
						break;
					}
				}
			}
			else
			{
				values_.push_back(s);
				if (lastOption != NULL)
				{
					*lastOption = --values_.end();
				}
				lastOption = NULL;
			}
		}
#ifdef ARGSTREAM_DEBUG
		for (std::map<_TSTRING,value_iterator>::const_iterator
			iter = options_.begin();iter != options_.end();++iter)
		{
#ifdef _UNICODE
			std::wcout<<L"DEBUG: option "<<iter->first;
#else
			std::cout<<"DEBUG: option "<<iter->first;
#endif
			if (iter->second != values_.end())
			{
#ifdef _UNICODE
				std::wcout<<L" -> "<<*(iter->second);
#else
				std::cout<<" -> "<<*(iter->second);
#endif
			}
			std::cout<<std::endl;
		}
		for (std::list<_TSTRING>::const_iterator
			iter = values_.begin();iter != values_.end();++iter)
		{
#ifdef _UNICODE
			std::wcout<<L"DEBUG: value  "<<*iter<<std::endl;
#else
			std::cout<<"DEBUG: value  "<<*iter<<std::endl;
#endif
		}
#endif // ARGSTREAM_DEBUG
	}

	template<typename CHARTYPE>
	inline bool	argstream<CHARTYPE>::isOk() const
	{
		return isOk_;
	}

	template<typename CHARTYPE>
	inline bool	argstream<CHARTYPE>::helpRequested() const
	{
		return helpRequested_;
	}

	template<typename CHARTYPE>
	inline typename TSTR<CHARTYPE>::type argstream<CHARTYPE>::usage() const
	{
		typename TSTRSTREAM<CHARTYPE>::O os;
		os<<TSTR<CHARTYPE>::ToString("usage: ")<<progName_<<cmdLine_<<'\n';
		unsigned int lmax = 0;
		for (std::deque<help_entry>::const_iterator
			iter = argHelps_.begin();iter != argHelps_.end();++iter)
		{
			if (lmax<iter->first.size()) lmax = iter->first.size();
		}  
		for (std::deque<help_entry>::const_iterator
			iter = argHelps_.begin();iter != argHelps_.end();++iter)
		{
			os << '\t' << iter->first << TSTR<CHARTYPE>::type(lmax-iter->first.size(),' ')
				<<" : "<<iter->second<<'\n';
		}
		return os.str();
	}

	template<typename CHARTYPE>
	inline typename TSTR<CHARTYPE>::type argstream<CHARTYPE>::errorLog() const
	{
		typename TSTR<CHARTYPE>::type s;
		for(std::deque<typename TSTR<CHARTYPE>::type>::const_iterator iter = errors_.begin();
			iter != errors_.end();++iter)
		{
			s += *iter;
			s += '\n';
		}
		return s;
	}

	template<typename CHARTYPE>
	inline CHARTYPE
		argstream<CHARTYPE>::uniqueLetter()
	{
		static unsigned int c = 'a';
		return c++;
	}
	template<typename CHARTYPE, typename T>
	argstream<CHARTYPE>& operator>>(argstream<CHARTYPE>& s,const ValueHolder<CHARTYPE, T>& v)
	{
		// Search in the options if there is any such option defined either with a
		// short name or a long name. If both are found, only the last one is
		// used.
#ifdef ARGSTREAM_DEBUG    
#ifdef _UNICODE
		std::wcout<<L"DEBUG: searching "<<v.shortName_<<L" "<<v.longName_<<std::endl;
#else
		std::cout<<"DEBUG: searching "<<v.shortName_<<" "<<v.longName_<<std::endl;
#endif
#endif    
		s.argHelps_.push_back(argstream<CHARTYPE>::help_entry(v.name(),v.description()));
		if (v.mandatory_)
		{
			if (!v.shortName_.empty())
			{
				s.cmdLine_ += TSTR<CHARTYPE>::ToString(" -");
				s.cmdLine_ += v.shortName_;
			}
			else
			{
				s.cmdLine_ += TSTR<CHARTYPE>::ToString(" --");
				s.cmdLine_ += v.longName_;
			}
			s.cmdLine_ += TSTR<CHARTYPE>::ToString(" value");
		}
		else
		{
			if (!v.shortName_.empty())
			{
				s.cmdLine_ += TSTR<CHARTYPE>::ToString(" [-");
				s.cmdLine_ += v.shortName_;
			}
			else
			{
				s.cmdLine_ += TSTR<CHARTYPE>::ToString(" [--");
				s.cmdLine_ += v.longName_;
			}  
			s.cmdLine_ += TSTR<CHARTYPE>::ToString(" value]");

		}
		std::map<typename TSTR<CHARTYPE>::type, typename argstream<CHARTYPE>::value_iterator>::iterator iter =
			s.options_.find(v.shortName_);
		if (iter == s.options_.end())
		{
			iter = s.options_.find(v.longName_);
		}
		if (iter != s.options_.end())
		{
			// If we find counterpart for value holder on command line, either it
			// has an associated value in which case we assign it, or it has not, in
			// which case we have an error.
			if (iter->second != s.values_.end())
			{
#ifdef ARGSTREAM_DEBUG
#ifdef _UNICODE
				std::wcout<<L"DEBUG: found value "<<*(iter->second)<<std::endl;
#else
				std::cout<<"DEBUG: found value "<<*(iter->second)<<std::endl;
#endif
#endif	
				ValueParser<CHARTYPE, T> p;
				*(v.value_) = p(*(iter->second));
				// The option and its associated value are removed, the subtle thing
				// is that someother options might have this associated value too,
				// which we must invalidate.
				// Modified by Levski Weng
				//s.values_.erase(iter->second);      
				for (std::map<typename TSTR<CHARTYPE>::type, typename argstream<CHARTYPE>::value_iterator>::iterator
					jter = s.options_.begin();jter != s.options_.end();++jter)
				{
					if (jter->second == iter->second)
					{
						jter->second = s.values_.end();
					}
				}
				s.options_.erase(iter);
			}
			else
			{
				s.isOk_ = false;
				typename TSTRSTREAM<CHARTYPE>::O os;
				os<<"No value following switch "<<iter->first //TODO
					<<" on command line"; //TODO
				s.errors_.push_back(os.str());
			}
		}
		else
		{
			if (v.mandatory_)
			{
				s.isOk_ = false;
				TSTRSTREAM<CHARTYPE>::O os;
				os<<"Mandatory parameter "; //TODO
				if (!v.shortName_.empty()) os<<'-'<<v.shortName_;
				if (!v.longName_.empty())
				{
					if (!v.shortName_.empty()) os<<'/';
					os<<"--"<<v.longName_;
				}
				os<<" missing";
				s.errors_.push_back(os.str());
			}
		}
		return s;
	}

	template<typename CHARTYPE>
	inline argstream<CHARTYPE>&	operator>>(argstream<CHARTYPE>& s,const OptionHolder<CHARTYPE>& v)
	{
		// Search in the options if there is any such option defined either with a
		// short name or a long name. If both are found, only the last one is
		// used.
#ifdef ARGSTREAM_DEBUG    
#ifdef _UNICODE
		std::wcout<<L"DEBUG: searching "<<v.shortName_<<L" "<<v.longName_<<std::endl;
#else
		std::cout<<"DEBUG: searching "<<v.shortName_<<" "<<v.longName_<<std::endl;
#endif
#endif
		s.argHelps_.push_back(argstream<CHARTYPE>::help_entry(v.name(),v.description()));
		{
			TSTR<CHARTYPE>::type c;
			if (!v.shortName_.empty())
			{
				c += TSTR<CHARTYPE>::ToString(" [-");
				c += v.shortName_;
			}
			else
			{
				c += TSTR<CHARTYPE>::ToString(" [--");
				c += v.longName_;
			}      
			c += TSTR<CHARTYPE>::ToString("]");
			s.cmdLine_ = c+s.cmdLine_;
		}
		std::map<TSTR<CHARTYPE>::type, argstream<CHARTYPE>::value_iterator>::iterator iter =
			s.options_.find(v.shortName_);
		if (iter == s.options_.end())
		{
			iter = s.options_.find(v.longName_);
		}
		if (iter != s.options_.end())
		{
			// If we find counterpart for value holder on command line then the
			// option is true and if an associated value was found, it is ignored
			if (v.value_ != NULL)
			{
				*(v.value_) = true;
			}
			else
			{
				s.helpRequested_ = true;
			}
			// The option only is removed
			s.options_.erase(iter);
		}
		else
		{
			if (v.value_ != NULL)
			{
				*(v.value_) = false;
			}
			else
			{
				s.helpRequested_ = false;
			}
		}
		return s;
	}

	template<typename CHARTYPE, typename T, typename O>
	argstream<CHARTYPE>& operator>>(argstream<CHARTYPE>& s,const ValuesHolder<CHARTYPE,T,O>& v)
	{
		s.argHelps_.push_back(argstream::help_entry(v.name(),v.description()));
		{
			STRSTREAM<CHARTYPE>::O os;
			os<<' '<<v.letter_<<'1';
			switch (v.len_)
			{
			case -1:
				os<< TSTR<CHARTYPE>::ToString("...");
				break;
			case 1:
				break;
			default:
				os<< TSTR<CHARTYPE>::ToString("...") << v.letter_ << v.len_;
				break;
			}
			s.cmdLine_ += os.str();
		}
		std::list<TSTR<CHARTYPE>::type>::iterator first = s.values_.begin();
		// We add to the iterator as much values as we can, limited to the length
		// specified (if different of -1)
		int n = v.len_ != -1?v.len_:s.values_.size();
		while (first != s.values_.end() && n-->0)
		{
			// Read the value from the string *first
			ValueParser<CHARTYPE, T> p;
			*(v.value_++) = p(*first );
			s.argHelps_.push_back(argstream::help_entry(v.name(),v.description()));
			// The value we just removed was maybe "remembered" by an option so we
			// remove it now.
			for (std::map<typename TSTR<CHARTYPE>::type, typename argstream<CHARTYPE>::value_iterator>::iterator
				jter = s.options_.begin();jter != s.options_.end();++jter)
			{
				if (jter->second == first)
				{
					jter->second = s.values_.end();
				}
			}
			++first;
		}
		// Check if we have enough values
		if (n != 0)
		{
			s.isOk_ = false;
			typename TSTRSTREAM<CHARTYPE>::O os;
			os<<TSTR<CHARTYPE>::ToString("Expecting ")<<v.len_<<TSTR<CHARTYPE>::ToString(" values");
			s.errors_.push_back(os.str());
		}
		// Erase the values parsed
		s.values_.erase(s.values_.begin(),first);
		return s;
	}

	template<typename CHARTYPE>
	inline void
		argstream<CHARTYPE>::defaultErrorHandling(bool ignoreUnused) const
	{
		if (helpRequested_)
		{
#ifdef _UNICODE
			std::wcout<<usage();
#else
			std::cout<<usage();
#endif
			exit(1);
		}
		if (!isOk_)
		{
#ifdef _UNICODE
			std::wcerr<<errorLog();
#else
			std::cerr<<errorLog();
#endif
			exit(1);
		}
		if (!ignoreUnused &&
			(!values_.empty() || !options_.empty()))
		{
#ifdef _UNICODE
			std::wcerr<<L"Unused arguments"<<std::endl;
#else
			std::cerr<<"Unused arguments"<<std::endl;
#endif
			exit(1);
		}
	}
};
#endif // ARGSTREAM_H

