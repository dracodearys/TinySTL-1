#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <cstdlib>

/* 	1. Primer 19.6节：union
	2. 枚举类型
*/

namespace TinySTL{

	/*
	**空间配置器，以字节数为单位分配
	**内部使用
	*/
	class alloc{
	private:
		// 枚举类型 enum
		// ps类似限定作用域的宏定义
		enum EAlign{ ALIGN = 8};//小型区块的上调边界
		enum EMaxBytes{ MAXBYTES = 128};//小型区块的上限，超过的区块由malloc分配
		enum ENFreeLists{ NFREELISTS = (EMaxBytes::MAXBYTES / EAlign::ALIGN)};//free-lists的个数，有多少个区块就有多少个freelist保证其回收
		enum ENObjs{ NOBJS = 20};//每次增加的节点数
	private:
		//free-lists的节点构造
		union obj{
			//利用union节省了空间，这个区块既可以放下一块的指针，又可以存放数据
			union obj *next;
			char client[1];
			// ps扩展：含有类类型成员的union需要封装到类中并定义拷贝成员控制函数，见Primer
		};
		static obj *free_list[ENFreeLists::NFREELISTS];//这里调用了上方的宏，之所以使用静态是为了让这部分唯一（不唯一则不安全）
	private:
		static char *start_free;//内存池起始位置
		static char *end_free;//内存池结束位置
		static size_t heap_size;//堆的大小
		//之所以在这里使用静态内存的原因也很清楚，这些数据都是唯一的，不可能同时存在两个，一旦改变（一般也不改变）就是全局一起改变
	private:
		//将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes){
			return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));  
		}
		//根据区块大小，决定使用第n号free-list，n从0开始计算
		static size_t FREELIST_INDEX(size_t bytes){
			return (((bytes)+EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
		static void *refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便（其实就是没有这么多连续内存拿得出来），nobjs可能会降低
		static char *chunk_alloc(size_t size, size_t& nobjs);

	public:
		//外部可以调用的部分
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	};//静态方法效率上比实例化要高，这种经常要用的函数最好设置静态，因为每次实例化都要成本
}

#endif