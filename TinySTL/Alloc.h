#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <cstdlib>

/* 	1. Primer 19.6�ڣ�union
	2. ö������
*/

namespace TinySTL{

	/*
	**�ռ������������ֽ���Ϊ��λ����
	**�ڲ�ʹ��
	*/
	class alloc{
	private:
		// ö������ enum
		// ps�����޶�������ĺ궨��
		enum EAlign{ ALIGN = 8};//С��������ϵ��߽�
		enum EMaxBytes{ MAXBYTES = 128};//С����������ޣ�������������malloc����
		enum ENFreeLists{ NFREELISTS = (EMaxBytes::MAXBYTES / EAlign::ALIGN)};//free-lists�ĸ������ж��ٸ�������ж��ٸ�freelist��֤�����
		enum ENObjs{ NOBJS = 20};//ÿ�����ӵĽڵ���
	private:
		//free-lists�Ľڵ㹹��
		union obj{
			//����union��ʡ�˿ռ䣬�������ȿ��Է���һ���ָ�룬�ֿ��Դ������
			union obj *next;
			char client[1];
			// ps��չ�����������ͳ�Ա��union��Ҫ��װ�����в����忽����Ա���ƺ�������Primer
		};
		static obj *free_list[ENFreeLists::NFREELISTS];//����������Ϸ��ĺ֮꣬����ʹ�þ�̬��Ϊ�����ⲿ��Ψһ����Ψһ�򲻰�ȫ��
	private:
		static char *start_free;//�ڴ����ʼλ��
		static char *end_free;//�ڴ�ؽ���λ��
		static size_t heap_size;//�ѵĴ�С
		//֮����������ʹ�þ�̬�ڴ��ԭ��Ҳ���������Щ���ݶ���Ψһ�ģ�������ͬʱ����������һ���ı䣨һ��Ҳ���ı䣩����ȫ��һ��ı�
	private:
		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes){
			return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));  
		}
		//���������С������ʹ�õ�n��free-list��n��0��ʼ����
		static size_t FREELIST_INDEX(size_t bytes){
			return (((bytes)+EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}
		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void *refill(size_t n);
		//����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�������������㣨��ʵ����û����ô�������ڴ��õó�������nobjs���ܻή��
		static char *chunk_alloc(size_t size, size_t& nobjs);

	public:
		//�ⲿ���Ե��õĲ���
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	};//��̬����Ч���ϱ�ʵ����Ҫ�ߣ����־���Ҫ�õĺ���������þ�̬����Ϊÿ��ʵ������Ҫ�ɱ�
}

#endif