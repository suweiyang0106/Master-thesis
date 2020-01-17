#include "DMA.h"

void DMA::add_task_to_DMA_queue(Task* task)
{
	cout << "Add task to DMA queue, wich model is: " << task->model_id << ", task id is: " << task->task_id << endl;
	if (head_of_DMA == 0)									
	{
		head_of_DMA = task;
		return;
	}
	Task* ram_current = head_of_DMA;
	while (ram_current->DMA_queue_next != 0)
	{
		ram_current = ram_current->DMA_queue_next;
	}
	ram_current->DMA_queue_next = task;
	task->DMA_queue_next = 0;
}

/*void DMA::add_model_to_DMA_queue(vector<Task_Set*>& model)
{
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++) 
	{
		Task* current = (*it)->first;
		while (current != 0) 
		{
			add_task_to_DMA_queue(current);
			current = current->next;
		}
	}
}*/

void DMA::add_model_to_DMA_queue_by_disorder(vector<Task_Set*>& model)
{	
	int model_number = model.size();
	vector<Task*>current_model;

	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
	{
		current_model.push_back((*it)->first);
	}

	while (true) 
	{
		bool end = true;
		for (vector<Task*>::iterator it = current_model.begin(); it != current_model.end(); it++)
		{
			if ((*it) != 0) {
				end = false;
				add_task_to_DMA_queue((*it));
				(*it) = (*it)->next;
			}			
		}
		if (end == true) {
			break;
		}
	}
}

void DMA::add_task_to_victim_queue(Task* sub_task)
{
	cout << "Add task to victim  queue, wich model is: " << sub_task->model_id << ", task id is: " << sub_task->task_id << endl;
	if (head_of_victim_list == 0)									
	{
		head_of_victim_list = sub_task;
		return;
	}
	Task* ram_current = head_of_victim_list;
	while (ram_current->ram_victim_next != 0)
	{
		ram_current = ram_current->ram_victim_next;
	}
	ram_current->ram_victim_next = sub_task;
	sub_task->ram_victim_next = 0;
}

void DMA::add_lru_counter(Task* sub_task)
{
	sub_task->lru_counter = sub_task->lru_counter + 1;
}

void DMA::remove_task_from_DMA_queue(Task* sub_task)
{
	cout << "Remove task from DMA queue, which model is: " << sub_task->model_id << ", task id: " << sub_task->task_id << endl;

	if (head_of_DMA == sub_task)
	{
		if (sub_task->DMA_queue_next != 0)
			head_of_DMA = sub_task->DMA_queue_next;
		else
			head_of_DMA = 0;
	}
	else
	{
		Task* current = head_of_DMA;
		while (current->DMA_queue_next != sub_task)
		{
			current = current->DMA_queue_next;
		}
		current->DMA_queue_next = sub_task->DMA_queue_next;
	}
	sub_task->DMA_queue_next = 0;
}

void DMA::remove_task_from_victim_queue(Task* sub_task)
{
	if (head_of_victim_list == sub_task)
	{
		if (sub_task->ram_victim_next != 0)
			head_of_victim_list = sub_task->ram_victim_next;
		else
			head_of_victim_list = 0;
		cout << "Remove task from victim queue, which model is: " << sub_task->model_id << ", task id: " << sub_task->task_id << endl;
	}
	else if (head_of_victim_list != 0)
	{
		Task* current = head_of_victim_list;
		while (current->ram_victim_next != 0 && current->ram_victim_next != sub_task)
		{
			current = current->ram_victim_next;
		}
		if (current->ram_victim_next != 0) 
		{
			current->ram_victim_next = sub_task->ram_victim_next;
		}			
		cout << "Remove task from victim queue, which model is: " << sub_task->model_id << ", task id: " << sub_task->task_id << endl;
	}	
	sub_task->ram_victim_next = 0;
}

void DMA::prepare_memory_pages( RAM* ram)
{		
	if (head_of_DMA != 0) 
	{
		remove_task_from_victim_queue(head_of_DMA);
		if (head_of_DMA->mem_swap[0] == 0) 
		{
			organize_dma_and_victim_queue();
		}
		if (head_of_DMA->total_memory_page == head_of_DMA->memory_page&&
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0)
		{
			//do nothing			
		}
		else if (head_of_DMA->memory_page==0&&
			head_of_DMA->ssd_page==0&&
			(ram->ram_loading+head_of_DMA->total_memory_page)<=floor(ram->system_memory_total_page*ram->swap_condition)) 
		{
			//Not yest has allocated any memory pages at the sub task
			ram->ram_loading = ram->ram_loading + head_of_DMA->total_memory_page;
			head_of_DMA->memory_page = head_of_DMA->total_memory_page;
		}
		else if (head_of_DMA->total_memory_page != head_of_DMA->memory_page &&
			(ram->ram_loading + (head_of_DMA->total_memory_page - head_of_DMA->memory_page))>=floor(ram->system_memory_total_page*ram->swap_condition) &&
			head_of_DMA->mem_swap[0]==0 &&
			head_of_DMA->mem_swap[1] == 0 &&
			check_victim_can_satisfy_dma(ram)==true &&
			check_task_in_alloc_write_read_queue(ram) == false )
		{
			//the memory pages still not all on memory.
			//So we need to swap page from victim.
			/*//cout << "travel_allocate_queue" << endl;
			ram->travel_allocate_queue();	
			//cout << "travel_read_queue" << endl;
			ram->travel_read_queue();
			//cout << "travel_write_queue" << endl;
			ram->travel_write_queue();*/
			calculate_swap_time(ram);
		}
		else if (head_of_DMA->total_memory_page != head_of_DMA->memory_page &&
			(ram->ram_loading + (head_of_DMA->total_memory_page - head_of_DMA->memory_page)) <= floor(ram->system_memory_total_page * ram->swap_condition) &&
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0 &&
			check_task_in_alloc_write_read_queue(ram) == false)
		{
			calculate_swap_time_without_victim(ram);
		}
		else if (head_of_DMA->mem_swap[0] != 0 &&
			head_of_DMA->mem_swap[1]!=0)
		{
			//execute one tick
			execution_swap_time(ram);
		}
		else
		{
			cout << "bug is here" << endl;
		}
		
	}
	
}

bool DMA::check_victim_can_satisfy_dma(RAM* ram)
{
	int dma_need = head_of_DMA->total_memory_page - head_of_DMA->memory_page;
	int victim_can_give = 0;
	Task* victim_current = head_of_victim_list;
	while (victim_current != 0) 
	{
		if (dma_need - victim_current->memory_page > 0)
		{
			dma_need = dma_need - victim_current->memory_page;
		}
		else 
		{
			dma_need = 0;
			return true;
		}
		victim_current = victim_current->ram_victim_next;
	}
	return false;
}

void DMA::execution_swap_time(RAM* ram)
{
	cout << "ram swap excuted time: " << head_of_DMA->mem_swap[0] << " remain time: " << head_of_DMA->mem_swap[1] << ", which model is: " << head_of_DMA->model_id << ", task id: " << head_of_DMA->task_id << endl;
	head_of_DMA->mem_swap[1] = head_of_DMA->mem_swap[1] - 1;
	if (head_of_DMA->mem_swap[1] == 0)
	{
		//把自己的mem page加滿，並ssd page變0
		if(head_of_DMA->alpha==1.0)
		{
			head_of_DMA->mem_swap[0] = 0;
			head_of_DMA->mem_swap[1] = 0;
			head_of_DMA->memory_page = head_of_DMA->total_memory_page;
			head_of_DMA->ssd_page = 0;
		}
		else 
		{
			head_of_DMA->mem_swap[0] = 0;
			head_of_DMA->mem_swap[1] = 0;
			head_of_DMA->memory_page = floor(1.0*head_of_DMA->total_memory_page*head_of_DMA->alpha);
			head_of_DMA->ssd_page = head_of_DMA->total_memory_page-head_of_DMA->memory_page;
		}
	}
}

void DMA::organize_dma_and_victim_queue()
{
	Task* original_head_dma = head_of_DMA;
	sort_dma_queue(&head_of_DMA);
	Task* current_head_dma = head_of_DMA;
	if (original_head_dma != current_head_dma) 
	{
		remove_task_from_victim_queue(current_head_dma);
		if (original_head_dma->memory_page > 0)
			add_task_to_victim_queue(original_head_dma);
	}
	organize_victim_queue();
	sort_victim_queue(&head_of_victim_list);
}

void DMA::organize_victim_queue()
{
	Task* current = head_of_victim_list;
	while (current != 0) 
	{
		if (current->ram_victim_next!=0&&current->ram_victim_next->memory_page == 0) 
		{
			remove_task_from_victim_queue(current->ram_victim_next);
			current = current->ram_victim_next;
		}
		else 
		{
			current = current->ram_victim_next;
		}
		
	}
}

void DMA::calculate_swap_time(RAM* ram)
{
	Task* victim_current = head_of_victim_list;
	int pages_request_need = head_of_DMA->total_memory_page - head_of_DMA->memory_page;
	//int memory_remain = ram->system_memory_total_page - ram->ram_loading;	
	if (head_of_DMA->alpha == 1.0) 
	{
		//pages_request_need = pages_request_need - memory_remain;
		//ram->ram_loading = ram->ram_loading + memory_remain;
		//head_of_DMA->memory_page = head_of_DMA->memory_page + memory_remain;
		while (victim_current != 0)
		{
			if (pages_request_need == 0)
			{
				break;
			}
			if (pages_request_need > victim_current->memory_page &&
				head_of_DMA->priority <= victim_current->priority&&
				victim_current->memory_page>0)
			{
				victim_current->block_out = victim_current->block_out + victim_current->memory_page;
				head_of_DMA->block_in = head_of_DMA->block_in + victim_current->memory_page;				
				pages_request_need = pages_request_need - victim_current->memory_page;
				victim_current->ssd_page = victim_current->ssd_page + victim_current->memory_page;
				victim_current->memory_page = victim_current->memory_page- victim_current->memory_page;
			}
			else if (pages_request_need <= victim_current->memory_page&&
				head_of_DMA->priority <= victim_current->priority &&
				victim_current->memory_page > 0)
			{
				victim_current->block_out = victim_current->block_out + pages_request_need;
				head_of_DMA->block_in = head_of_DMA->block_in + pages_request_need;
				victim_current->memory_page = victim_current->memory_page - pages_request_need;
				victim_current->ssd_page = victim_current->ssd_page + pages_request_need;
				pages_request_need = 0;
			}
			victim_current = victim_current->ram_victim_next;
		}		
		
		if (pages_request_need == 0)
		{
			head_of_DMA->mem_swap[0] =
				head_of_DMA->mem_swap[1] = ceil(100.0 * (head_of_DMA->total_memory_page - head_of_DMA->memory_page) * 4096 / ram->ssd_bandwidth_write / 1024 / 1024)
				+ ceil(100.0 * (head_of_DMA->total_memory_page - head_of_DMA->memory_page) * 4096 / ram->ssd_bandwidth_read / 1024 / 1024);
			head_of_DMA->memory_page = head_of_DMA->total_memory_page;
			head_of_DMA->ssd_page = 0;
		}
		else
		{
			cout << "bug here!!" << endl;
		}
		/*************************************計算memory存取所耗的power********************************************/
		ram->ssd_energy_consumption = ram->ssd_energy_consumption + head_of_DMA->mem_swap[0] * ram->ssd_power_work;
	}
	else
	{
		head_of_DMA->mem_swap[0] =
			head_of_DMA->mem_swap[1] = ceil(100.0 * head_of_DMA->total_memory_page* (1-head_of_DMA->alpha) * 4096 / ram->ssd_bandwidth_write / 1024 / 1024);
		ram->ram_loading = ram->ram_loading + floor(1.0*head_of_DMA->total_memory_page * head_of_DMA->alpha);
		/*************************************計算memory存取所耗的power********************************************/
		int ssd_consumption_time = ceil(100.0 * head_of_DMA->total_memory_page * (1 - head_of_DMA->alpha) * 4096 / ram->ssd_bandwidth_write / 1024 / 1024);
		ram->ssd_energy_consumption = ram->ssd_energy_consumption + ssd_consumption_time * ram->ssd_power_work;
	}
}

void DMA::calculate_swap_time_without_victim(RAM* ram)
{	
	head_of_DMA->mem_swap[0] =
		head_of_DMA->mem_swap[1] = ceil(100.0 * (head_of_DMA->total_memory_page - head_of_DMA->memory_page) * 4096 / ram->ssd_bandwidth_read / 1024 / 1024);
	ram->ram_loading = ram->ram_loading + (head_of_DMA->total_memory_page - head_of_DMA->memory_page);
	head_of_DMA->memory_page = head_of_DMA->total_memory_page;
	head_of_DMA->ssd_page = 0;
	/*************************************計算memory存取所耗的power********************************************/
	ram->ssd_energy_consumption = ram->ssd_energy_consumption + head_of_DMA->mem_swap[0] * ram->ssd_power_work;
}

void DMA::calculate_allocate_time(RAM* ram)
{
	head_of_DMA->mem_allocate[0] =
		head_of_DMA->mem_allocate[1] = ceil(100.0*head_of_DMA->memory_page * 4096 / ram->bandwidth / 1024 / 1024)+ceil(100.0 * head_of_DMA->ssd_page * 4096 / ram->ssd_bandwidth_write / 1024 / 1024);
	/*************************************計算memory存取所耗的power********************************************/
	int memory_power = ceil(100.0 * head_of_DMA->memory_page * 4096 / ram->bandwidth / 1024 / 1024);
	int ssd_power = floor(100.0 * head_of_DMA->ssd_page * 4096 / ram->ssd_bandwidth_write / 1024 / 1024);
	ram->ram_energy_consumption = ram->ram_energy_consumption + memory_power * ram->ram_power;
	ram->ssd_energy_consumption = ram->ssd_energy_consumption + ssd_power * ram->ssd_power_work;
}

void DMA::calculate_read_time(RAM* ram)
{
	head_of_DMA->mem_read[0] =
		head_of_DMA->mem_read[1] = ceil(100.0 * head_of_DMA->memory_page * 4096 / ram->bandwidth / 1024 / 1024) + ceil(100.0 * head_of_DMA->ssd_page * 4096 / ram->ssd_bandwidth_read / 1024 / 1024);;
	/*************************************計算memory存取所耗的power********************************************/
	int memory_power = ceil(100.0 * head_of_DMA->memory_page * 4096 / ram->bandwidth / 1024 / 1024);
	int ssd_power = floor(100.0 * head_of_DMA->ssd_page * 4096 / ram->ssd_bandwidth_write / 1024 / 1024);
	ram->ram_energy_consumption = ram->ram_energy_consumption + memory_power * ram->ram_power;
	ram->ssd_energy_consumption = ram->ssd_energy_consumption + ssd_power * ram->ssd_power_work;
}

void DMA::calculate_write_time(RAM* ram)
{
	head_of_DMA->mem_write[0] =
		head_of_DMA->mem_write[1] = ceil(100.0 * head_of_DMA->memory_page * 4096 / ram->bandwidth / 1024 / 1024) + ceil(100.0 * head_of_DMA->ssd_page * 4096 / ram->ssd_bandwidth_write / 1024 / 1024);
	/*************************************計算memory存取所耗的power********************************************/
	int memory_power = ceil(100.0 * head_of_DMA->memory_page * 4096 / ram->bandwidth / 1024 / 1024);
	int ssd_power = floor(100.0 * head_of_DMA->ssd_page * 4096 / ram->ssd_bandwidth_write / 1024 / 1024);
	ram->ram_energy_consumption = ram->ram_energy_consumption + memory_power * ram->ram_power;
	ram->ssd_energy_consumption = ram->ssd_energy_consumption + ssd_power * ram->ssd_power_work;
}

void DMA::travel_DMA_queue()
{
	Task* current = head_of_DMA;
	if (head_of_DMA == 0)
		cout << "The list is empty." << endl;
	while (current != 0) 
	{
		cout << "Task Sset: " << current->model_id<<", task id: " << current->task_id << endl;;
		current = current->DMA_queue_next;
	}
}

void DMA::travel_victim_queue()
{
	Task* current = head_of_victim_list;
	if (current == 0) 
	{
		cout << "List is empty!" << endl;
		return;
	}
	while (current != 0) 
	{
		cout << "model id: " << current->model_id << ", task id: " << current->task_id <<", LRU counter: "<<current->lru_counter<< endl;
		current = current->ram_victim_next;
	}
}

void DMA::travel_DMA_queue_SM()
{
	Task* current = head_of_DMA;
	while (current != 0) 
	{
		cout << "Application id: " << current->model_id << " task id: " << current->task_id << " SM close: " << current->SM_close << endl;
		current = current->DMA_queue_next;
	}
}

void DMA::profile_swap_in_and_out(vector<Task_Set*>& model)
{
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
	{
		Task* current = (*it)->first;
		int blockout = 0;
		int blockin = 0;
		while (current != 0)
		{
			blockout = blockout + current->block_out;
			blockin = blockin + current->block_in;
			current = current->next;
		}
		cout << "Application id: " << (*it)->first->model_id<<" block in:"<<blockin<<"pages block out: "<<blockout << endl;
	}
}

void DMA::profile_response_time(vector<Task_Set*>& model)
{
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
	{
		Task* current = (*it)->first;
		int max_end_time = 0;
		int min_start_time = 0;
		int response_time = 0;
		while (current != 0)
		{
			if (max_end_time < current->end_time)
				max_end_time = current->end_time;
			if (min_start_time > current->start_time)
				min_start_time = current->start_time;
			current = current->next;
		}
		response_time = max_end_time - min_start_time;
		cout << "Application id: " << (*it)->first->model_id << " response time:" << response_time <<", lengthened latency: "<<response_time-(*it)->single_elpased_time<<", response time(without interference):"<<(*it)->single_elpased_time<< endl;
		(*it)->actual_latency = response_time - (*it)->single_elpased_time;
		(*it)->elapsed_time = response_time;
	}
}

void DMA::check_each_task_fit_memory_loading(RAM *ram,vector<Task_Set *>&model)
{
	int memory_page_of_tasks = 0;
	Task* current = 0;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++) 
	{
		current = (*it)->first;
		while (current != 0) 
		{
			memory_page_of_tasks = memory_page_of_tasks + current->memory_page;
			current = current->next;
		}
	}
	if (memory_page_of_tasks != ram->ram_loading) 
	{
		cout << "there is a bug" << endl;
	}
	else 
	{
		//cout << "It is fine!!" << endl;
	}
}

void DMA::assign_task_to_memory_component( RAM *ram)
{
	if(check_task_in_alloc_write_read_queue(ram)==false && 
		head_of_DMA!=0 &&
		head_of_DMA->total_memory_page==head_of_DMA->memory_page&&
		head_of_DMA->mem_swap[0]==0&&
		head_of_DMA->mem_swap[1]==0)
	{
		if (head_of_DMA->mem_allocate[0] == 0)
		{
			calculate_allocate_time(ram);
		}
		else if (head_of_DMA->mem_read[0] == 0)
		{
			calculate_read_time(ram);
		}
		else if ((head_of_DMA->mem_write[0] == 0))
		{
			calculate_write_time(ram);
		}
		if (head_of_DMA->mem_allocate[0] != 0 && head_of_DMA->mem_allocate[1] != 0)
		{
			ram->add_task_to_allocate_queue(head_of_DMA);
		}
		else if (head_of_DMA->mem_read[0] != 0 && head_of_DMA->mem_read[1] != 0)
		{
			ram->add_task_to_read_queue(head_of_DMA);
		}
		else if (head_of_DMA->mem_write[0] != 0 && head_of_DMA->mem_write[1] != 0)
		{
			ram->add_task_to_write_queue(head_of_DMA);
		}
		add_lru_counter(head_of_DMA);
		remove_task_from_DMA_queue(head_of_DMA);		
	}
}

bool DMA::check_task_in_alloc_write_read_queue( RAM* ram)
{
	Task* current = ram->head_of_allocate;
	if (current != 0) return true;
	current = ram->head_of_read;
	if (current != 0) return true;
	current = ram->head_of_write;
	if (current != 0) return true;
	return false;
}

bool DMA::check_task_in_dma_queue()
{
	Task* current = head_of_DMA;
	if (current != 0) return true;
	return false;
}

bool DMA::check_task_in_gpu_queue(GPU* gpu_sm)
{
	Task* current = gpu_sm->head;
	if (current != 0) return true;
	return false;
}

void DMA::clear()
{
	head_of_DMA = 0;
}

void DMA::our_policy_prepare_memory_pages(RAM *ram)
{
	if (head_of_DMA != 0)
	{		
		remove_task_from_victim_queue(head_of_DMA);
		if (head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0 )
		{
			organize_dma_and_victim_queue();
		}		
		if (head_of_DMA->total_memory_page == head_of_DMA->memory_page&& 
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0)
		{
			//do nothing
		}
		else if (head_of_DMA->memory_page == 0 &&
			head_of_DMA->ssd_page == 0 &&
			(ram->ram_loading + head_of_DMA->total_memory_page) <= floor(ram->system_memory_total_page * ram->swap_condition)&&
			head_of_DMA->alpha==1.0)
		{
			//Not yet  allocate any memory pages at the sub task
			ram->ram_loading = ram->ram_loading + head_of_DMA->total_memory_page;
			head_of_DMA->memory_page = head_of_DMA->total_memory_page;
		}
		else if (head_of_DMA->memory_page == 0 &&
			head_of_DMA->ssd_page == 0 &&
			head_of_DMA->alpha < 1.0&&
			floor(head_of_DMA->total_memory_page*head_of_DMA->alpha)+ram->ram_loading<= floor(ram->system_memory_total_page * ram->swap_condition))
		{
			//Not yet  allocate any memory pages at the sub task
			ram->ram_loading = ram->ram_loading + floor(head_of_DMA->total_memory_page * head_of_DMA->alpha);
			head_of_DMA->memory_page = floor(head_of_DMA->total_memory_page * head_of_DMA->alpha);
			head_of_DMA->ssd_page = head_of_DMA->total_memory_page-floor(head_of_DMA->total_memory_page * head_of_DMA->alpha);
		}
		else if (head_of_DMA->total_memory_page != head_of_DMA->memory_page &&
			(ram->ram_loading + (head_of_DMA->total_memory_page - head_of_DMA->memory_page)) > floor(ram->system_memory_total_page * ram->swap_condition) &&
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0 &&
			check_victim_can_satisfy_dma(ram) == true&&
			head_of_DMA->alpha==1.0 &&
			check_task_in_alloc_write_read_queue(ram) == false)
		{
			//the memory pages still not all on memory.
			//So we need to swap page from victim.
			calculate_swap_time(ram);
		}
		else if (head_of_DMA->total_memory_page != head_of_DMA->memory_page &&
			(ram->ram_loading + (head_of_DMA->total_memory_page - head_of_DMA->memory_page)) < floor(ram->system_memory_total_page * ram->swap_condition) &&
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0 &&
			head_of_DMA->alpha == 1.0 &&
			check_task_in_alloc_write_read_queue(ram) == false)
		{
			calculate_swap_time_without_victim(ram);
		}
		else if (head_of_DMA->mem_swap[0] != 0 &&
			head_of_DMA->mem_swap[1] != 0)
		{
			//execute one tick
			execution_swap_time(ram);
		}
		else if(head_of_DMA->total_memory_page == head_of_DMA->memory_page+head_of_DMA->ssd_page 
			&& head_of_DMA->memory_page == floor(head_of_DMA->total_memory_page * head_of_DMA->alpha)
			&& head_of_DMA->ssd_page == head_of_DMA->total_memory_page - floor(head_of_DMA->total_memory_page * head_of_DMA->alpha))
		{
			//do nothing
		}
		else
		{
			cout << "bug is here" << endl;
		}
	}
}

void DMA::our_policy_assign_task_to_memory_component(RAM* ram)
{
	if ((check_task_in_alloc_write_read_queue(ram) == false &&
		head_of_DMA != 0&&
		head_of_DMA->total_memory_page==head_of_DMA->memory_page&&
		head_of_DMA->mem_swap[0] == 0 &&
		head_of_DMA->mem_swap[1] == 0 &&
		head_of_DMA->alpha==1.0)	||
		(check_task_in_alloc_write_read_queue(ram) == false &&
			head_of_DMA != 0 &&
			head_of_DMA->total_memory_page == (head_of_DMA->memory_page + head_of_DMA->ssd_page) &&
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0&&head_of_DMA->alpha<1.0))
	{		
		if (head_of_DMA->mem_allocate[0] == 0)
		{
			calculate_allocate_time(ram);
		}
		else if (head_of_DMA->mem_read[0] == 0)
		{
			calculate_read_time(ram);
		}
		else if ((head_of_DMA->mem_write[0] == 0))
		{
			calculate_write_time(ram);
		}
		if (head_of_DMA->mem_allocate[0] != 0 && head_of_DMA->mem_allocate[1] != 0)
		{
			ram->add_task_to_allocate_queue(head_of_DMA);
		}
		else if (head_of_DMA->mem_read[0] != 0 && head_of_DMA->mem_read[1] != 0)
		{
			ram->add_task_to_read_queue(head_of_DMA);
		}
		else if (head_of_DMA->mem_write[0] != 0 && head_of_DMA->mem_write[1] != 0)
		{
			ram->add_task_to_write_queue(head_of_DMA);
		}
		remove_task_from_DMA_queue(head_of_DMA);
	}
}

void DMA::related_work_prepare_memory_pages(RAM* ram)
{
	if (head_of_DMA != 0)
	{	
		remove_task_from_victim_queue(head_of_DMA);
		if (head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0 &&
			head_of_DMA->SM_close == true)
		{			
			related_work_organize_dma_and_victim_queue();
		}
		if (head_of_DMA->total_memory_page == head_of_DMA->memory_page&& 
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0)
		{
			//do nothing
		}
		else if (head_of_DMA->memory_page == 0 &&
			head_of_DMA->ssd_page == 0 &&
			(ram->ram_loading + head_of_DMA->total_memory_page) <= floor(ram->system_memory_total_page * ram->swap_condition)&& 
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0)
		{
			//Not yest has allocated any memory pages at the sub task
			ram->ram_loading = ram->ram_loading + head_of_DMA->total_memory_page;
			head_of_DMA->memory_page = head_of_DMA->total_memory_page;
		}
		else if (head_of_DMA->memory_page==0 && 
			head_of_DMA->ssd_page==0 &&
			(ram->ram_loading + head_of_DMA->total_memory_page) >= floor(ram->system_memory_total_page * ram->swap_condition) &&
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0 &&
			head_of_DMA->SM_close==false&&
			related_check_task_SM_close_in_dma_and_victim_queue()==false&&
			check_victim_can_satisfy_dma(ram)==true &&
			check_task_in_alloc_write_read_queue(ram) == false)
		{
			calculate_swap_time(ram);
			related_work_close_or_open_application(head_of_DMA,true);
		}
		else if (head_of_DMA->total_memory_page!=head_of_DMA->memory_page &&
			(ram->ram_loading + (head_of_DMA->total_memory_page - head_of_DMA->memory_page)) >= floor(ram->system_memory_total_page * ram->swap_condition) &&
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0 &&
			head_of_DMA->SM_close == false&&
			related_check_task_SM_close_in_dma_and_victim_queue() == false &&
			check_victim_can_satisfy_dma(ram)==true &&
			check_task_in_alloc_write_read_queue(ram) == false)
		{
			calculate_swap_time(ram);
			related_work_close_or_open_application(head_of_DMA, true);
		}
		else if (head_of_DMA->total_memory_page != head_of_DMA->memory_page &&
			(ram->ram_loading + (head_of_DMA->total_memory_page - head_of_DMA->memory_page)) > floor(ram->system_memory_total_page * ram->swap_condition) &&
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0 &&
			head_of_DMA->SM_close == false &&
			check_victim_can_satisfy_dma(ram)==true &&
			check_task_in_alloc_write_read_queue(ram) == false)
		{
			calculate_swap_time(ram);
		}
		else if ((head_of_DMA->memory_page+head_of_DMA->ssd_page)==head_of_DMA->total_memory_page&&
			head_of_DMA->total_memory_page != head_of_DMA->memory_page &&
			(ram->ram_loading + (head_of_DMA->total_memory_page - head_of_DMA->memory_page)) < floor(ram->system_memory_total_page * ram->swap_condition) &&
			head_of_DMA->mem_swap[0] == 0 &&
			head_of_DMA->mem_swap[1] == 0 &&
			check_task_in_alloc_write_read_queue(ram) == false)
		{
			calculate_swap_time_without_victim(ram);
			if (head_of_DMA->SM_close == true)
			{
				related_work_close_or_open_application(head_of_DMA, false);
			}
		}
		else if (head_of_DMA->mem_swap[0] != 0 &&
			head_of_DMA->mem_swap[1] != 0)
		{
			//execute one tick
			execution_swap_time(ram);
		}
		else
		{
			cout << "bug is here" << endl;
		}
		

	}
}

void DMA::related_work_assign_task_to_memory_component(RAM* ram)
{
	if (check_task_in_alloc_write_read_queue(ram) == false &&
		head_of_DMA != 0 &&
		head_of_DMA->total_memory_page == head_of_DMA->memory_page &&
		head_of_DMA->mem_swap[0] == 0 &&
		head_of_DMA->mem_swap[1] == 0)
	{
		if (head_of_DMA->mem_allocate[0] == 0)
		{
			calculate_allocate_time(ram);
		}
		else if (head_of_DMA->mem_read[0] == 0)
		{
			calculate_read_time(ram);
		}
		else if ((head_of_DMA->mem_write[0] == 0))
		{
			calculate_write_time(ram);
		}
		if (head_of_DMA->mem_allocate[0] != 0 && head_of_DMA->mem_allocate[1] != 0)
		{
			ram->add_task_to_allocate_queue(head_of_DMA);
		}
		else if (head_of_DMA->mem_read[0] != 0 && head_of_DMA->mem_read[1] != 0)
		{
			ram->add_task_to_read_queue(head_of_DMA);
		}
		else if (head_of_DMA->mem_write[0] != 0 && head_of_DMA->mem_write[1] != 0)
		{
			ram->add_task_to_write_queue(head_of_DMA);
		}
		add_lru_counter(head_of_DMA);
		remove_task_from_DMA_queue(head_of_DMA);
	}
}

void DMA::related_work_close_or_open_application(Task* sub_task, bool turn_on_or_off)
{
	Task* current = sub_task->head_thread_of_process;
	while (current != 0) 
	{
		current->SM_close = turn_on_or_off;
		current = current->next;
	}
}

void DMA::related_work_organize_dma_and_victim_queue()
{
	Task* original_head_dma = head_of_DMA;
	sort_dma_queue_by_SM_close(&head_of_DMA);
	Task* current_head_dma = head_of_DMA;
	if (original_head_dma != current_head_dma)
	{
		remove_task_from_victim_queue(current_head_dma);
		if(original_head_dma->memory_page>0)
			add_task_to_victim_queue(original_head_dma);
	}
	sort_victim_queue(&head_of_victim_list);
}

bool DMA::related_check_task_SM_close_in_dma_and_victim_queue()
{
	Task* current = head_of_DMA;
	while (current != 0) 
	{
		if (current->SM_close == true) 
		{
			return true;
		}
		current = current->DMA_queue_next;
	}
	current = head_of_victim_list;
	while (current != 0) 
	{
		if (current->SM_close == true)
		{
			return true;
		}
		current = current->ram_victim_next;
	}
	return false;
}

void DMA::related_work_check_each_task_memory_equal_total(vector<Task_Set*>& model,RAM *ram)
{
	int memory_page_of_tasks = 0;
	Task* current = 0;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
	{
		current = (*it)->first;
		while (current != 0)
		{
			if (current->total_memory_page < (current->memory_page + current->ssd_page))
			{
				cout << "here is a bug" << endl;
				return;
			}
			current = current->next;
		}
	}
}

void DMA::sort_dma_queue(Task** headRef)
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->DMA_queue_next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_dma_queue(head, &a, &b);

	/* Recursively sort the sublists */
	sort_dma_queue(&a);
	sort_dma_queue(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_dma_queue(a, b);
}
void DMA::splitList_dma_queue(Task* source, Task** frontRef, Task** backRef)
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->DMA_queue_next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->DMA_queue_next;
		if (fast != NULL) {
			slow = slow->DMA_queue_next;
			fast = fast->DMA_queue_next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->DMA_queue_next;
	slow->DMA_queue_next = NULL;
}
Task* DMA::mergelist_dma_queue(Task* a, Task* b)
{
	Task* result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->priority <= b->priority) {
		result = a;
		result->DMA_queue_next = mergelist_dma_queue(a->DMA_queue_next, b);
	}
	else {
		result = b;
		result->DMA_queue_next = mergelist_dma_queue(a, b->DMA_queue_next);
	}
	return (result);
}

void DMA::sort_victim_queue(Task** headRef)
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->ram_victim_next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_victim_queue(head, &a, &b);

	/* Recursively sort the sublists */
	sort_victim_queue(&a);
	sort_victim_queue(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_victim_queue(a, b);
}
void DMA::splitList_victim_queue(Task* source, Task** frontRef, Task** backRef)
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->ram_victim_next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->ram_victim_next;
		if (fast != NULL) {
			slow = slow->ram_victim_next;
			fast = fast->ram_victim_next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->ram_victim_next;
	slow->ram_victim_next = NULL;
}
Task* DMA::mergelist_victim_queue(Task* a, Task* b)
{

	Task* result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->lru_counter <= b->lru_counter) {
		result = a;
		result->ram_victim_next = mergelist_victim_queue(a->ram_victim_next, b);
	}
	else {
		result = b;
		result->ram_victim_next = mergelist_victim_queue(a, b->ram_victim_next);
	}
	return (result);
}

void DMA::sort_dma_queue_by_SM_close(Task** headRef)
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->DMA_queue_next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_dma_queue_by_SM_close(head, &a, &b);

	/* Recursively sort the sublists */
	sort_dma_queue_by_SM_close(&a);
	sort_dma_queue_by_SM_close(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_dma_queue_by_SM_close(a, b);
}
void DMA::splitList_dma_queue_by_SM_close(Task* source, Task** frontRef, Task** backRef)
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->DMA_queue_next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->DMA_queue_next;
		if (fast != NULL) {
			slow = slow->DMA_queue_next;
			fast = fast->DMA_queue_next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->DMA_queue_next;
	slow->DMA_queue_next = NULL;
}
Task* DMA::mergelist_dma_queue_by_SM_close(Task* a, Task* b)
{

	Task* result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->SM_close<= b->SM_close  ) {
		result = a;
		result->DMA_queue_next = mergelist_dma_queue_by_SM_close(a->DMA_queue_next, b);
	}
	else {
		result = b;
		result->DMA_queue_next = mergelist_dma_queue_by_SM_close(a, b->DMA_queue_next);
	}
	return (result);
}


