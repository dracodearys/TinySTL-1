#include "../Alloc.h"

namespace TinySTL{

	//����ʼ��
	char *alloc::start_free = 0;
	char *alloc::end_free = 0;
	size_t alloc::heap_size = 0;

	alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] = {
		0, 0, 0, 0, 	0, 0, 0, 0, 	0, 0, 0, 0, 	0, 0, 0, 0
	};

	void *alloc::allocate(size_t bytes){
		if (bytes > EMaxBytes::MAXBYTES){
			return malloc(bytes);	//������Ҫ��ֱ�ӵ���malloc
		}
		size_t index = FREELIST_INDEX(bytes);	//ѡ������
		obj *list = free_list[index];	//ѡ������
		if (list){//��list���пռ������
			free_list[index] = list->next;	//�Ѻ�һ���ڴ�ĵ�ַ���룬�����´�ȡ��
			return list;	//�������ڴ�ǡ�ÿ��ţ�������ڴ��ͳ�
		}
		else{//��listû���㹻�Ŀռ䣬��Ҫ���ڴ������ȡ�ռ�
			return refill(ROUND_UP(bytes));
		}
	}
	void alloc::deallocate(void *ptr, size_t bytes){
		if (bytes > EMaxBytes::MAXBYTES){
			free(ptr);	//��mallocȡ�����ڴ���free��
		}
		else{
			size_t index = FREELIST_INDEX(bytes);	//�û�ȥ�Ŀ�
			obj *node = static_cast<obj *>(ptr);	//ǿ��ת��
			node->next = free_list[index];
			free_list[index] = node;	//������ڴ������ǰ����֮ǰ���ڴ�������
		}
	}
	void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz){
		deallocate(ptr, old_sz);	//����
		ptr = allocate(new_sz);		//�ط���

		return ptr;
	}
	//����һ����СΪn�Ķ��󣬲�����ʱ���Ϊ�ʵ���free list���ӽڵ�
	//����bytes�Ѿ��ϵ�Ϊ8�ı���
	void *alloc::refill(size_t bytes){
		size_t nobjs = ENObjs::NOBJS;
		//���ڴ����ȡ
		char *chunk = chunk_alloc(bytes, nobjs);	//ֱ�Ӵ��ڴ��ȡ
		obj **my_free_list = 0;
		obj *result = 0;
		obj *current_obj = 0, *next_obj = 0;

		if (nobjs == 1){//ȡ���Ŀռ�ֻ��һ������ʹ�ã�����ָ�ֱ�ӷ���
			return chunk;
		}
		else{
			my_free_list = free_list + FREELIST_INDEX(bytes);
			result = (obj *)(chunk);
			*my_free_list = next_obj = (obj *)(chunk + bytes);
			//��ȡ���Ķ���Ŀռ���뵽��Ӧ��free list����ȥ
			for (int i = 1;; ++i){
				current_obj = next_obj;
				next_obj = (obj *)((char *)next_obj + bytes);
				if (nobjs - 1 == i){
					current_obj->next = 0;	//�ֵÿ����㹻���ڴ��Ѿ����꣬���һ��ָ��0���˳�ѭ��
					break;
				}
				else{
					current_obj->next = next_obj;
				}
			}
			return result;
		}
	}
	//����bytes�Ѿ��ϵ�Ϊ8�ı���
	char *alloc::chunk_alloc(size_t bytes, size_t& nobjs){
		char *result = 0;
		size_t total_bytes = bytes * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes){//�ڴ��ʣ��ռ���ȫ������Ҫ
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else if (bytes_left >= bytes){//�ڴ��ʣ��ռ䲻����ȫ������Ҫ�����㹻��Ӧһ�������ϵ�����
			nobjs = bytes_left / bytes;
			total_bytes = nobjs * bytes;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else{//�ڴ��ʣ��ռ���һ������Ĵ�С���޷��ṩ
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);	//��ϵͳ��Ҫ���ڴ�Ϊ�����Ĵ˴��������16������
			if (bytes_left > 0){
				obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj *)start_free)->next = *my_free_list;
				*my_free_list = (obj *)start_free;
			}
			start_free = (char *)malloc(bytes_to_get);	//Ҫһ���µ��ڴ棬�����ڴ�������¶���
			if (!start_free){	
				//mallocû�еõ��ڴ�
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
			heap_size += bytes_to_get;	//�����ڴ�
			end_free = start_free + bytes_to_get;	//�ض���ĩβ�ڴ�
			return chunk_alloc(bytes, nobjs);	
		}
	}
}