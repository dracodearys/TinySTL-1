#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

namespace TinySTL{

	namespace{
		// 对namespace的定义
		template<bool, class Ta, class Tb>
		struct IfThenElse;
		template<class Ta, class Tb>
		struct IfThenElse < true, Ta, Tb > {
			using result = Ta;
		};
		template<class Ta, class Tb>
		struct IfThenElse < false, Ta, Tb > {
			using result = Tb;
		};
	}

	struct _true_type { };
	struct _false_type { };

	/*	
		1.类模板不能被重载，可以全特化和偏特化
		2.函数模板可以重载和全特化，不能偏特化（重载就是偏特化了）
		3.模板类调用优先级：全特化类 > 偏特化类 > 主版本模板类
	*/

	/*
		萃取传入的T类型的类型特性
		区分是否是POD数据
		POD类型分为两种：1. trivial 2. standard layout
		::trivial::
		（1）拥有平凡的构造函数(trivial constructor)和析构函数(trivial destructor)。
			平凡的默认构造函数即构造函数“什么都不干”。通常情况下，不定义类的构造函数，编译器就会为我们生成一个平凡的默认构造函数。
		（2）拥有平凡的拷贝构造函数（trivial copy constructor）和移动构造函数（trivial copy constructor）。
			平凡的拷贝构造函数基本上等同于memcpy进行类型的构造。同平凡构造函数一样，编译器在用户不提供的情况下，通常会提供平凡的拷贝构造函数。
		（3）拥有平凡的拷贝赋值运算符（trivial assignment operator）和移动赋值运算符（move operator）。
		（4）不能包含虚函数以及虚基类。
			C++11提供了内置函数来支持Trivial类型的判断。
				template <typename T> struct std::is_trivial
		::standard layout::
				没看懂
	*/

	template<class T>
	struct _type_traits
	{
		typedef _false_type		has_trivial_default_constructor;
		typedef _false_type		has_trivial_copy_constructor;
		typedef _false_type		has_trivial_assignment_operator;
		typedef _false_type		has_trivial_destructor;
		typedef _false_type		is_POD_type;
	};
		// 为什么需要特化这么多次？？？？？
	template<>
	struct _type_traits<bool>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<char>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<unsigned char>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	//在32位系统中一个char类型一般为8个bit，所以能存储的数据范围为-128~127，而unsigned char则是0~255，存储范围相同
	struct _type_traits<signed char>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	//wchar_t的表示范围远远大于char
	struct _type_traits<wchar_t>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<short>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<unsigned short>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<int>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<unsigned int>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<unsigned long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<long long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<unsigned long long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<float>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<double>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<long double>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<class T>
	struct _type_traits<T*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<class T>
	struct _type_traits<const T*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<unsigned char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<signed char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<const char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<const unsigned char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
	template<>
	struct _type_traits<const signed char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

}
#endif