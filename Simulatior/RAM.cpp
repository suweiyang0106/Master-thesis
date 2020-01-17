#include "RAM.h"
#include "DMA.h"

using namespace std;
/****************************************Release pages to Memory*********************************************/
bool RAM::check_the_same_task_in_alloc_write_read_and_gpu(DMA* dma, GPU* gpu_sm, Task* sub_task)
{
	Task* current = head_of_allocate;
	while (current != 0)
	{
		if (current->model_id == sub_task->model_id)
		{
			return true;
		}
		current = current->ram_allocate_next;
	}
	current = head_of_read;
	while (current != 0)
	{
		if (current->model_id == sub_task->model_id)
		{
			return true;
		}
		current = current->ram_read_next;
	}
	current = head_of_write;
	while (current != 0)
	{
		if (current->model_id == sub_task->model_id)
		{
			return true;
		}
		current = current->ram_write_next;
	}
	current = gpu_sm->head;
	while (current != 0)
	{
		if (current->model_id == sub_task->model_id)
		{
			return true;
		}
		current = current->gpu_next;
	}
	current = dma->head_of_DMA;
	while (current != 0)
	{
		if (current->model_id == sub_task->model_id)
		{
			return true;
		}
		current = current->DMA_queue_next;
	}

	return false;
}
void RAM::release_model_memory(Task *thread,DMA *dma) 
{
	Task *current = thread->head_thread_of_process;
	while (current != 0)
	{
		ram_loading = ram_loading-current->memory_page;
		utilization = ram_loading / system_memory_total_page;
		dma->remove_task_from_victim_queue(current);
		current->memory_page = 0;
		current = current->next;
	}
}
/*************************************************Traveling queue********************************************************/
void RAM::travel_allocate_queue()
{
	if (head_of_allocate == 0) {                      // 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_allocate;						// 用pointer *current在list中移動
	while (current != 0) {									// Traversal
		cout << "memory assign: Total(page):" << current->total_memory_page << ", RAM(page): " << current->memory_page << ", SSD(page): " << current->ssd_page << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		//cout << "ram execution time: " << current->mem_allocate[0] << ", remained time: " << current->mem_allocate[1] << endl;
		current = current->ram_allocate_next;
	}
	cout << endl;
}
void RAM::travel_read_queue()
{
	if (head_of_read == 0) {								// 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_read;							// 用pointer *current在list中移動
	while (current != 0) {									// Traversal
		cout << "execution time: " << current->mem_read[0] << ", remained time: " << current->mem_read[1] << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		current = current->ram_read_next;
	}
	cout << endl;
}
void RAM::travel_write_queue()
{
	if (head_of_write == 0)
	{														// 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_write;							// 用pointer *current在list中移動
	while (current != 0)
	{                 // Traversal
		cout << "execution time: " << current->mem_write[0] << ", remained time: " << current->mem_write[1] << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		current = current->ram_write_next;
	}
	cout << endl;
}
/*************************************************Queue reset***********************************************************/
void RAM::clear() 
{
	ram_loading = 0;
	swap_loading = 0;
	utilization = 0;
	head_of_allocate = 0;
	head_of_write = 0;
	ram_energy_consumption = 0;
	ssd_energy_consumption = 0;
}
/****************************************New function***************************************/
void RAM::add_task_to_allocate_queue(Task* request)
{
	cout << "Add thread to allocate queue, wich model is: " << request->model_id << ", task id is: " << request->task_id << endl;
	if (head_of_allocate == 0)									
	{
		head_of_allocate = request;
		return;
	}
	Task* ram_current = head_of_allocate;
	while (ram_current->ram_allocate_next != 0)
	{
		ram_current = ram_current->ram_allocate_next;
	}
	ram_current->ram_allocate_next = request;
}

void RAM::add_task_to_read_queue(Task* request)
{
	cout << "Add thread to read queue, wich model is: " << request->model_id << ", task id is: " << request->task_id << endl;	
	if (head_of_read == 0)			
	{
		head_of_read = request;
		return;
	}
	Task* ram_current = head_of_read;
	while (ram_current->ram_read_next != 0)
	{
		ram_current = ram_current->ram_read_next;
	}
	ram_current->ram_read_next = request;					
}

void RAM::add_task_to_write_queue(Task* request)
{
	cout << "Add thread to write queue, wich model is: " << request->model_id << ", task id is: " << request->task_id << endl;
	if (head_of_write == 0)									
	{
		head_of_write = request;
		return;
	}
	Task* ram_current = head_of_write;
	while (ram_current->ram_write_next != 0)
	{
		ram_current = ram_current->ram_write_next;
	}
	ram_current->ram_write_next = request;					
}

void RAM::remove_task_from_allocate_queue(Task *request, DMA *dma)
{
	cout << "Remove thread from allocating queue, which model is: " << request->model_id << ", task id: " << request->task_id << endl;

	if (head_of_allocate == request)
	{
		if (request->ram_allocate_next != 0)
			head_of_allocate = request->ram_allocate_next;
		else
			head_of_allocate = 0;
	}
	else
	{
		Task* current = head_of_allocate;
		while (current->ram_allocate_next != request)
		{
			current = current->ram_allocate_next;
		}
		current->ram_allocate_next = request->ram_allocate_next;		
	}
	if (request->memory_page > 0)
	{
		dma->add_task_to_victim_queue(request);
	}
	request->ram_allocate_next = 0;
	dma->add_task_to_DMA_queue(request);
}

void RAM::remove_task_from_read_queue(Task* request, GPU *gpu_sm)
{
	cout << "Remove thread from read queue, which model is: " << request->model_id << ", task id: " << request->task_id << endl;
	if (head_of_read == request)
	{
		if (request->ram_read_next != 0)
			head_of_read = request->ram_read_next;
		else
			head_of_read = 0;
	}
	else
	{
		Task* current = head_of_read;
		while (current->ram_read_next != request)
		{
			current = current->ram_read_next;
		}
		current->ram_read_next = request->ram_read_next;
	}
	request->ram_read_next = 0;
	gpu_sm->add_thread_to_sm_queue(request);
}

void RAM::remove_task_from_write_queue(Task* request, DMA *dma,GPU *gpu_sm)
{
	cout << "Remove thread from writing queue " << " which model is: " << request->model_id << ", task id: " << request->task_id << endl;

	if (head_of_write == request)
	{
		if (request->ram_write_next != 0)
			head_of_write = request->ram_write_next;
		else
			head_of_write = 0;
	}
	else
	{
		Task* current = head_of_write;
		while (current->ram_write_next != request)
		{
			current = current->ram_write_next;
		}
		current->ram_write_next = request->ram_write_next;
	}
	if (check_the_same_task_in_alloc_write_read_and_gpu(dma,gpu_sm,request)==false)
	{
		release_model_memory(request,dma);
		if (ram_loading < 0) 
		{
			cout << "bug here" << endl;
		}		
	}
	else if(request->memory_page>0)
	{
		dma->add_task_to_victim_queue(request);
	}
	request->ram_write_next = 0;
}

void RAM::execute_allocating_operation(DMA* dma)
{
	if (head_of_allocate != 0) 
	{
		cout << "ram allocated excuted time: " << head_of_allocate->mem_allocate[0] << " remain time: " << head_of_allocate->mem_allocate[1] << ", which model is: " << head_of_allocate->model_id << ", task id: " << head_of_allocate->task_id << endl;
		head_of_allocate->mem_allocate[1] = head_of_allocate->mem_allocate[1] - 1;
		if (head_of_allocate->mem_allocate[1] == 0) 
		{
			remove_task_from_allocate_queue(head_of_allocate,dma);
		}
	}
}

void RAM::execute_reading_operation(GPU *gpu_sm)
{
	if (head_of_read != 0)
	{
		cout << "ram reading excuted time: " << head_of_read->mem_read[0] << " remain time: " << head_of_read->mem_read[1] << ", which model is: " << head_of_read->model_id << ", task id: " << head_of_read->task_id << endl;
		head_of_read->mem_read[1] = head_of_read->mem_read[1] - 1;
		if (head_of_read->mem_read[1] == 0)
		{
			remove_task_from_read_queue(head_of_read, gpu_sm);
		}
	}
	
}

void RAM::execute_writing_operation(DMA* dma , GPU* gpu_sm, int time_tick)
{
	if (head_of_write != 0)
	{
		cout << "ram writing excuted time: " << head_of_write->mem_write[0] << " remain time: " << head_of_write->mem_write[1] << ", which model is: " << head_of_write->model_id << ", task id: " << head_of_write->task_id << endl;
		head_of_write->mem_write[1] = head_of_write->mem_write[1] - 1;
		if (head_of_write->mem_write[1] == 0)
		{
			head_of_write->end_time = time_tick;
			remove_task_from_write_queue(head_of_write,dma,gpu_sm);
		}
	}
	
}
