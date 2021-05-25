#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new>

#include "TypeTraits.h"

	/*
	参考：
	《STL源码剖析》 P51、P70. 
	*/

namespace TinySTL{

	template<class T1, class T2>
	inline void construct(T1 *ptr1, const T2& value){ 
		new(ptr1) T1(value);//用new来给对应地址分配空间
	}

	template<class T>
	inline void destroy(T *ptr){
		ptr->~T();//调用对应的析构函数进行析构
	}
	// ？？？？
	//本函数对连续的pod型数据结构进行处理，由于这类函数的情况特殊，无法定义一个统一的函数析构，所以这里只有一个空壳
	template<class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type){
	}

	//本函数对非pod型进行处理，这一类型可以直接destroy
	template<class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type){
		for (; first != last; ++first){
			destroy(&*first);//沿着内存一次往后销毁（连续空间的好处）
			// &*first和first是一样的，但是由于destroy()只接受T* 指针，函数体内的->运算符可能缺少重载。
			// &和*是有重载的，可以生成指向ForwardIterator类型的引用，相当于一定会生成指针。
			// 参考 https://segmentfault.com/q/1010000004284760
		}
	}

	//本函数其实起到了鉴别是否为pod的作用，根据数据结构的类型选择了上方的两种函数
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last){
		typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
		_destroy(first, last, is_POD_type());
	}
}
//之所以使用inline是因为这里的函数代码量都非常短，逻辑简单没有递归，嵌入成本很低，而作为独立函数成本很高

#endif