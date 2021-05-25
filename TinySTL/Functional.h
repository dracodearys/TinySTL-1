#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_

namespace TinySTL{
	//********** [less] ****************
	//顾名思义是个比大小的类
	template<class T>
	struct less{
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;

		result_type operator()(const first_argument_type& x, const second_argument_type& y){
			return x < y;
		}
	};
	//********** [equal_to] ****************
	//顾名思义是个判断是否相等的类
	template<class T>
	struct equal_to{
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;

		result_type operator()(const first_argument_type& x, const second_argument_type& y){
			return x == y;
		}
	};
}
#endif