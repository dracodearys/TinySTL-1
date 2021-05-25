#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "Alloc.h"
#include "Construct.h"

#include <cassert>
#include <new>

namespace TinySTL{
	//�ռ����������Ա�����ĿΪ��λ����
	template<class T>
	class allocator{
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;//ptrdiff_t���ͱ���ͨ��������������ָ����������Ľ���������ɸ�����һ�֡����롱���
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
		return static_cast<T *>(alloc::allocate(sizeof(T)));//alloc���allocate�����allocate��һ����
		/* 	static_cast��һ��c++������������ǰ�һ�����ʽת��Ϊĳ�����ͣ���û������ʱ���ͼ������֤ת���İ�ȫ�ԡ�
			static_cast<new_type> (expression) 
			����Ҫ�����¼����÷���      									(���磬�˴���ʹ�����÷�3)
			���������νṹ�л��ࣨ���ࣩ�������ࣨ���ࣩ֮��ָ������õ�ת����
				��������ת�������������ָ�������ת���ɻ����ʾ���ǰ�ȫ�ģ�
				��������ת�����ѻ���ָ�������ת�����������ʾ��ʱ������û�ж�̬���ͼ�飬�����ǲ���ȫ�ġ�
			�����ڻ�����������֮���ת�������intת����char����intת����enum������ת���İ�ȫ��ҲҪ������Ա����֤��
			�۰ѿ�ָ��ת����Ŀ�����͵�ָ�롣
			�ܰ��κ����͵ı��ʽת����void���͡�
			ע�⣺static_cast����ת����expression��const��volatile������__unaligned���ԡ�
		*/
	}
	template<class T>
	T *allocator<T>::allocate(size_t n){
		if (n == 0) return 0;
		return static_cast<T *>(alloc::allocate(sizeof(T) * n));//һ����������T��С�Ŀռ�
	}
	template<class T>
	void allocator<T>::deallocate(T *ptr){
		alloc::deallocate(static_cast<void *>(ptr), sizeof(T));//ע��ǿ��ת�ͣ�����alloc�е�deallocate
	}
	template<class T>
	void allocator<T>::deallocate(T *ptr, size_t n){
		if (n == 0) return;
		alloc::deallocate(static_cast<void *>(ptr), sizeof(T)* n);//��������deallocate
	}

	template<class T>
	void allocator<T>::construct(T *ptr){
		new(ptr)T();//��newΪ��ַptr���ù��캯���������T()������ֵ��ʼ��
		/* 	��Primer Page 408�������ڶ������Լ��Ĺ��캯���������ͣ�Ҫ��ֵ��ʼ����û������ģ�
			���Ƕ����������ͣ�ֵ��ʼ�������ö����ֵ����Ĭ�ϳ�ʼ��������T������ţ������ֵ��
			Ϊ����ģ������ñ�����ʼ����ͬ���뷨���Զ�̬����Ķ�����г�ʼ���Ǹ������⡣
		*/
		/*
			���� new operator; operator new; placement new:
			1.	new operator����ƽʱʹ�õ�new�����磺ʹ�ùؼ���new�ڶ��϶�̬����һ�������
				A *ptr = new A;
				��ʵ�������������£���ȡһ���ڴ�ռ�[operator newʵ��]�����ù��캯��[placement newʵ��]��������ȷ��ָ��
				(ֻ������һ��ָ�������ת����ʵ���ϱ�����Ĵ����в�����Ҫ����ת��)��
				�������������ǲ��ܸ��ģ����Ǿ��嵽ĳһ�����е���Ϊ����������������ǵľ���Ҫ��ʱ�����Ը������� 
				��Primer Page 728��
			2.	operator new Ĭ����������ȵ��÷����ڴ�Ĵ��룬���Եõ�һ�ζ��ϵĿռ䡣
				���ɹ��ͷ��أ���ʧ�������new_hander��Ȼ������ظ�ǰ����̡������������̲����⣬
				��������operator new������������ϣ������Ϊ��
			3. 	placement new ����ʵ�ֶ�λ���졣��ȡ����һ���������ָ�����Ͷ�����ڴ��������ڴ��Ϲ���һ������
				��������ͷ�ļ�<new>��<new.h>����ʹ��placement new�� 
				new(ptrA) A(4); 
				ʵ������ָ���ڴ��ַ��(ָ��ptrA��ָ����ڴ��ַ)��ָ�����͵Ĺ��캯��(���ô������Ĺ��캯��A(4))
				������һ������Ĺ��ܡ�������ʾ�ص����������� ptrA->~A();
				��Primer Page 729��
		*/
	}
	template<class T>
	void allocator<T>::construct(T *ptr, const T& value){
		new(ptr)T(value);//��newΪ��ַptr���ù��캯��
	}
	template<class T>
	void allocator<T>::destroy(T *ptr){
		ptr->~T();//������������
	}
	template<class T>
	void allocator<T>::destroy(T *first, T *last){
		for (; first != last; ++first){
			first->~T();//��һ������������
		}
	}
}

#endif