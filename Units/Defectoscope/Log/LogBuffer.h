#pragma once
#include "LogMessages.h"

class Log
{	
	static void Insert(int id, double val, bool);
	template<class T>struct the_index_variable_is_out_of_range_look_at_N{typedef T Result;};
	template<>struct the_index_variable_is_out_of_range_look_at_N<void>{typedef int Result;};
	template<class T>static double Convert(T t){return (double)t;}
public:	
	struct TData
	{
		unsigned id;
		unsigned time;
		double value;
	};
#pragma warning(disable: 4101)
	static LONG lastMessage;
	template<LogMess::ID N>struct Filter{static const bool value = true;};
	template<LogMess::ID N>static void Mess(typename the_index_variable_is_out_of_range_look_at_N<typename LogMess::Row<N>::type>::Result value = 0)
	{	
		Insert(N, Convert(value), Filter<N>::value);
	}
#pragma warning(default: 4101)
	static bool IsRow(int , TData *&);
	static int LastMessageIndex();
	static bool LastMessage(TData *&d);
};

#define DROP_MESSAGE(N)template<>struct Log::Filter<N>{static const bool value = false;};
DROP_MESSAGE(LogMess::ErrStop)
DROP_MESSAGE(LogMess::ProgramOpen)
#undef DROP_MESSAGE
