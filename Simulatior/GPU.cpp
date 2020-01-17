#include "GPU.h"
#include "DMA.h"
#include <iostream> 
using namespace std;
void GPU::add_thread_to_sm_queue(Task *thread)
{
	task_number_in_queue = task_number_in_queue + 1;
	if (head == 0)									// 若list沒有node, 令newNode為first
	{
		head = thread;
		return;
	}
	Task *gpu_current = head;
	while (gpu_current->gpu_next != 0)
	{
		gpu_current = gpu_current->gpu_next;
	}
	gpu_current->gpu_next = thread;					// 將newNode接在list的尾巴	
}
void GPU::execute_operation(DMA *dma)
{
	if (head != 0) 
	{
		Task* current = head;
		loading = 0;
		while (current != 0)
		{
			if (current->gpu[1] != 0 && loading + (current->thread_per_block * current->blocks) <= limitation_sm)
			{
				cout << "gpu  excuted time: " << current->gpu[0] << " remain time: " << current->gpu[1] << ", which model id: " << current->model_id << ", task id: " << current->task_id << endl;
				current->gpu[1] = current->gpu[1] - 1;
				loading = loading + (current->thread_per_block * current->blocks);					//update gpu's loading stat
			}
			current = current->gpu_next;
		}
		current = head;
		while (current != 0) 
		{
			if (current->gpu[1] == 0) 
			{
				dma->add_task_to_DMA_queue(current);
				if (current->memory_page > 0)
				{
					dma->add_task_to_victim_queue(current);
				}
				remove_thread_from_sm_queue(current);
			}
			current = current->gpu_next;
		}
		loading = 0;
	}
}
void GPU::remove_thread_from_sm_queue(Task *thread)
{	
	cout << "Remove thread from SM queue, which model id: " << thread->model_id<<", task id: "<<thread->task_id << endl;
	task_number_in_queue = task_number_in_queue - 1;
	if (thread == head&&thread->gpu_next ==0)
	{
		head = 0;
		return;
	}
	else if (thread==head&&thread->gpu_next!=0) 
	{
		head = thread->gpu_next;
		thread->gpu_next = 0;
		return;
	}
	Task *current = head;
	while (current->gpu_next != thread) 
	{
		current = current->gpu_next;
	}
	if (current->gpu_next->gpu_next != 0)	
		current->gpu_next = current->gpu_next->gpu_next;
	
	else
		current->gpu_next = 0;

}
void GPU::travel_gpu_queue() 
{
	if (head == 0) {                      // 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head;             // 用pointer *current在list中移動
	while (current != 0) {                 // Traversal
		cout << "gpu execution time: " << current->gpu[0] << ", remained time: " << current->gpu[1] << endl;
		current = current->gpu_next;
	}
	cout << endl;
}
/*************************************************Queue reset***********************************************************/
void GPU::clear() 
{
	head = 0;
}
/*************************************************Queue sorting*********************************************************/
void GPU::sort_sm_queue(Task **headRef)
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->gpu_next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_sm_queue(head, &a, &b);

	/* Recursively sort the sublists */
	sort_sm_queue(&a);
	sort_sm_queue(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_sm_queue(a, b);
}
void GPU::splitList_sm_queue(Task *source, Task **frontRef, Task **backRef)
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->gpu_next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->gpu_next;
		if (fast != NULL) {
			slow = slow->gpu_next;
			fast = fast->gpu_next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->gpu_next;
	slow->gpu_next = NULL;
}
Task *GPU::mergelist_sm_queue(Task *a, Task *b)
{
	Task *result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->priority <= b->priority) {
		result = a;
		result->gpu_next = mergelist_sm_queue(a->gpu_next, b);
	}
	else {
		result = b;
		result->gpu_next = mergelist_sm_queue(a, b->gpu_next);
	}
	return (result);
}

/***********************************************************************************************************************/