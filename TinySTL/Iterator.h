#ifndef _ITERATOR_H_
#define _ITERATOR_H_

namespace TinySTL{

	struct input_iterator_tag{};	//只读且读后往后移动
	struct output_iterator_tag{}; 	//只写且写后往后移动
	struct forward_iterator_tag :public input_iterator_tag {};//可以从前往后遍历的指针
	struct bidirectional_iterator_tag :public forward_iterator_tag {};//从两个方向都可以遍历的指针
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};//提供和指针相同的功能,换而言之它可以跳跃而不是逐个遍历

	template <class T, class Distance> struct input_iterator
	{
		typedef input_iterator_tag	iterator_category;//迭代器本身
		typedef T					value_type;//定义类型
		typedef Distance			difference_type;//大小
		typedef T*					pointer;//定义指针
		typedef T&					reference;//定义引用
	};
	struct output_iterator
	{
		typedef output_iterator_tag iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;
	};
	template <class T, class Distance> struct forward_iterator
	{
		typedef forward_iterator_tag	iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};
	template <class T, class Distance> struct bidirectional_iterator
	{
		typedef bidirectional_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};
	template <class T, class Distance> struct random_access_iterator
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template<class Category, class T, class Distance = ptrdiff_t,
	class Pointer = T*, class Reference = T&>		//为什么模板参数列表要加上指针和引用的类型，而不是根据T生成
	struct iterator
	{ 
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;			//？？？？？？？？
		typedef Reference	reference;
	};
	/* 
	【Primer Page 628】 类模板部分特例化
	*/
	template<class Iterator>
	struct iterator_traits		// 最通用的模板
	{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference 			reference;
	};
	template<class T>
	struct iterator_traits<T*>		// 部分特例化版本
	{
		typedef random_access_iterator_tag 	iterator_category;
		typedef T 							value_type;
		typedef ptrdiff_t 					difference_type;
		typedef T*							pointer;
		typedef T& 							reference;
	};
	template<class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag 	iterator_category;
		typedef T 							value_type;
		typedef ptrdiff_t 					difference_type;
		typedef const T*					pointer;
		typedef const T& 					reference;
	};

	//以下三条函数均使用强制转型提取iterator中的元素
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category	// 返回值 需要用typename声明
		iterator_category(const Iterator& It){
			typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator& It){
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		difference_type(const Iterator& It){
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}
											// 为什么需要这三个呢？？？？？？？？
											// 而且It参数也没用？？？
}

#endif