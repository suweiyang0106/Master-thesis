#pragma once
#include "Task.h"
class DMA;
class GPU
{
public:
	int loading=0;					//number of threads
	int task_number_in_queue = 0;	//number of task in queue
	int limitation_sm=2048;			//2048 threads per SM
	//int limitation_block = 1024;	//1024 threads per block
	//int frequency = 1300;			//frequency = 1.3G Hz
	Task *head;						//head of task queue
	Task *current;					//current executed task in gpu
public:
	GPU() {};
	~GPU() {};
	void add_thread_to_sm_queue(Task *thread);			//�Nsub task �[�Jgpu queue
	void execute_operation(DMA* dma);					//����gpu�ҭn���Ӫ��ɶ�
	void remove_thread_from_sm_queue(Task *thread);		//�Nsub task�qgpu queue�������í��s�[�JDMA queue
	void travel_gpu_queue();							//travel���gpu queue
	/*************************************************Queue reset***********************************************************/
	void clear();										//�Ngpu queue�̪��Ѽ��k�s
	/*************************************************Queue sorting*********************************************************/
	void sort_sm_queue(Task **headRef);
	void splitList_sm_queue(Task *source, Task **frontRef, Task **backRef);
	Task *mergelist_sm_queue(Task *a, Task *b);

};

