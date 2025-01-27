#include "../Alloc.h"

namespace TinySTL{

	//做初始化
	char *alloc::start_free = 0;
	char *alloc::end_free = 0;
	size_t alloc::heap_size = 0;

	alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] = {
		0, 0, 0, 0, 	0, 0, 0, 0, 	0, 0, 0, 0, 	0, 0, 0, 0
	};

	void *alloc::allocate(size_t bytes){
		if (bytes > EMaxBytes::MAXBYTES){
			return malloc(bytes);	//比最大块要大直接调用malloc
		}
		size_t index = FREELIST_INDEX(bytes);	//选择区块
		obj *list = free_list[index];	//选定区块
		if (list){//此list还有空间给我们
			free_list[index] = list->next;	//把后一块内存的地址存入，便于下次取用
			return list;	//如果这块内存恰好空着，把这块内存送出
		}
		else{//此list没有足够的空间，需要从内存池里面取空间
			return refill(ROUND_UP(bytes));
		}
	}
	void alloc::deallocate(void *ptr, size_t bytes){
		if (bytes > EMaxBytes::MAXBYTES){
			free(ptr);	//用malloc取来的内存用free还
		}
		else{
			size_t index = FREELIST_INDEX(bytes);	//该还去哪块
			obj *node = static_cast<obj *>(ptr);	//强制转型
			node->next = free_list[index];
			free_list[index] = node;	//将这块内存放在最前方，之前的内存挂在其后
		}
	}
	void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz){
		deallocate(ptr, old_sz);	//销毁
		ptr = allocate(new_sz);		//重分配

		return ptr;
	}
	//返回一个大小为n的对象，并且有时候会为适当的free list增加节点
	//假设bytes已经上调为8的倍数
	void *alloc::refill(size_t bytes){
		size_t nobjs = ENObjs::NOBJS;
		//从内存池里取
		char *chunk = chunk_alloc(bytes, nobjs);	//直接从内存池取
		obj **my_free_list = 0;
		obj *result = 0;
		obj *current_obj = 0, *next_obj = 0;

		if (nobjs == 1){//取出的空间只够一个对象使用，无需分割直接返回
			return chunk;
		}
		else{
			my_free_list = free_list + FREELIST_INDEX(bytes);
			result = (obj *)(chunk);
			*my_free_list = next_obj = (obj *)(chunk + bytes);
			//将取出的多余的空间加入到相应的free list里面去
			for (int i = 1;; ++i){
				current_obj = next_obj;
				next_obj = (obj *)((char *)next_obj + bytes);
				if (nobjs - 1 == i){
					current_obj->next = 0;	//分得块数足够，内存已经切完，最后一块指向0，退出循环
					break;
				}
				else{
					current_obj->next = next_obj;
				}
			}
			return result;
		}
	}
	//假设bytes已经上调为8的倍数
	char *alloc::chunk_alloc(size_t bytes, size_t& nobjs){
		char *result = 0;
		size_t total_bytes = bytes * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes){//内存池剩余空间完全满足需要
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else if (bytes_left >= bytes){//内存池剩余空间不能完全满足需要，但足够供应一个或以上的区块
			nobjs = bytes_left / bytes;
			total_bytes = nobjs * bytes;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else{//内存池剩余空间连一个区块的大小都无法提供
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);	//向系统所要的内存为两倍的此次需求加上16倍堆数
			if (bytes_left > 0){
				obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj *)start_free)->next = *my_free_list;
				*my_free_list = (obj *)start_free;
			}
			start_free = (char *)malloc(bytes_to_get);	//要一块新的内存，并把内存起点重新定义
			if (!start_free){	
				//malloc没有得到内存
				obj **my_free_list = 0, *p = 0;
				for (int i = 0; i <= EMaxBytes::MAXBYTES; i += EAlign::ALIGN){
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0){
						*my_free_list = p->next;
						start_free = (char *)p;
						end_free = start_free + i;
						return chunk_alloc(bytes, nobjs);
					}
				}
				end_free = 0;
			}
			heap_size += bytes_to_get;	//扩充内存
			end_free = start_free + bytes_to_get;	//重定义末尾内存
			return chunk_alloc(bytes, nobjs);	
		}
	}
}