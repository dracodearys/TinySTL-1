#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "Alloc.h"
#include "Construct.h"

#include <cassert>
#include <new>

namespace TinySTL{
	//空间配置器，以变量数目为单位分配
	template<class T>
	class allocator{
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;//ptrdiff_t类型变量通常用来保存两个指针减法操作的结果，可正可负，是一种“距离”概念。
	public:
		static T *allocate();
		static T *allocate(size_t n);
		static void deallocate(T *ptr);
		static void deallocate(T *ptr, size_t n);

		static void construct(T *ptr);
		static void construct(T *ptr, const T& value);
		static void destroy(T *ptr);
		static void destroy(T *first, T *last);
	};

	template<class T>
	T *allocator<T>::allocate(){
		return static_cast<T *>(alloc::allocate(sizeof(T)));//alloc里的allocate和这个allocate不一样！
		/* 	static_cast是一个c++运算符，功能是把一个表达式转换为某种类型，但没有运行时类型检查来保证转换的安全性。
			static_cast<new_type> (expression) 
			它主要有如下几种用法：      									(比如，此处就使用了用法3)
			①用于类层次结构中基类（父类）和派生类（子类）之间指针或引用的转换。
				进行上行转换（把派生类的指针或引用转换成基类表示）是安全的；
				进行下行转换（把基类指针或引用转换成派生类表示）时，由于没有动态类型检查，所以是不安全的。
			②用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
			③把空指针转换成目标类型的指针。
			④把任何类型的表达式转换成void类型。
			注意：static_cast不能转换掉expression的const、volatile、或者__unaligned属性。
		*/
	}
	template<class T>
	T *allocator<T>::allocate(size_t n){
		if (n == 0) return 0;
		return static_cast<T *>(alloc::allocate(sizeof(T) * n));//一次性申请多个T大小的空间
	}
	template<class T>
	void allocator<T>::deallocate(T *ptr){
		alloc::deallocate(static_cast<void *>(ptr), sizeof(T));//注意强制转型，调用alloc中的deallocate
	}
	template<class T>
	void allocator<T>::deallocate(T *ptr, size_t n){
		if (n == 0) return;
		alloc::deallocate(static_cast<void *>(ptr), sizeof(T)* n);//批量处理deallocate
	}

	template<class T>
	void allocator<T>::construct(T *ptr){
		new(ptr)T();//用new为地址ptr调用构造函数，这里的T()进行了值初始化
		/* 	【Primer Page 408】：对于定义了自己的构造函数的类类型，要求值初始化是没有意义的；
			但是对于内置类型，值初始化有良好定义的值，而默认初始化（不在T后加括号）对象的值是
			为定义的，出于让变量初始化相同的想法，对动态分配的对象进行初始化是个好主意。
		*/
		/*
			区分 new operator; operator new; placement new:
			1.	new operator就是平时使用的new，例如：使用关键字new在堆上动态创建一个类对象，
				A *ptr = new A;
				它实际上做了三件事：获取一块内存空间[operator new实现]、调用构造函数[placement new实现]、返回正确的指针
				(只是做了一个指针的类型转换，实际上编译出的代码中并不需要这种转换)。
				这三个步骤我们不能更改，但是具体到某一步骤中的行为，如果它不满足我们的具体要求时，可以更改它。 
				【Primer Page 728】
			2.	operator new 默认情况下首先调用分配内存的代码，尝试得到一段堆上的空间。
				若成功就返回，若失败则调用new_hander，然后继续重复前面过程。如果对这个过程不满意，
				可以重载operator new，来设置我们希望的行为。
			3. 	placement new 用来实现定位构造。在取得了一块可以容纳指定类型对象的内存后，在这块内存上构造一个对象。
				必须引用头文件<new>或<new.h>才能使用placement new。 
				new(ptrA) A(4); 
				实现了在指定内存地址上(指针ptrA所指向的内存地址)用指定类型的构造函数(调用带参数的构造函数A(4))
				来构造一个对象的功能。必须显示地调用析构函数 ptrA->~A();
				【Primer Page 729】
		*/
	}
	template<class T>
	void allocator<T>::construct(T *ptr, const T& value){
		new(ptr)T(value);//用new为地址ptr调用构造函数
	}
	template<class T>
	void allocator<T>::destroy(T *ptr){
		ptr->~T();//调用析构函数
	}
	template<class T>
	void allocator<T>::destroy(T *first, T *last){
		for (; first != last; ++first){
			first->~T();//逐一调用析构函数
		}
	}
}

#endif