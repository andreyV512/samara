#pragma once

struct Frame
{	
	struct Data
	{
		char buf[24000];
	} data;
	Frame *next;
	static void Drop(void *);	
	template<class T>static Frame *Get()
	{
		sizeof(size_of_type_Additional_must_be_greater_than_or_equal_to_the_type_of_T<sizeof(Data) >= sizeof(T), sizeof(T)>);
		return _Get();
	}
private:
	static Frame *_Get();
	template<bool, int>struct size_of_type_Additional_must_be_greater_than_or_equal_to_the_type_of_T;
	template<int N>struct size_of_type_Additional_must_be_greater_than_or_equal_to_the_type_of_T<true, N>{};
	Frame();
	Frame(const Frame &);
	const Frame &operator=(const Frame &);
};