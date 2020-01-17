#include <iostream> 
#include <ctime>   /* 時間相關函數 */
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
#include <cstdio>
#include "Task_Set.h"
#include "Task.h"
#include "RAM.h"
#include "GPU.h"
#include "Our_policy.h"
#include "DMA.h"
#define start 0
#define final 1
#define baseline 1
#define related_work 1
#define ourpolicy 1

using namespace std;

int main() 
{
	/***********************************Creat a process with multiple threads**********************/	
	
	if (baseline) 
	{
		vector<Task_Set*> model;
		Task_Set* create_models = new Task_Set();
		int time_tick = 0;
		string path;
		RAM* ram = new RAM();
		GPU* gpu_sm = new GPU();
		GPU* gpu_sm2 = new GPU();
		DMA* dma = new DMA();
		Our_policy* policy = new Our_policy();
		cout << "==============simulation begin==============" << endl;
		create_models->create_model(model);
		//dma->add_model_to_DMA_queue(model);
		dma->add_model_to_DMA_queue_by_disorder(model);
		cout << "dma queue" << endl;
		dma->travel_DMA_queue();
		while (true) 
		{		
			if (ram->ram_loading > ram->system_memory_total_page)
			{
				cout << "bug here" << endl;
			}
				dma->related_work_check_each_task_memory_equal_total(model,ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_writing_operation(dma,gpu_sm,time_tick);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			gpu_sm->execute_operation(dma);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_reading_operation(gpu_sm);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_allocating_operation(dma);	
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			dma->assign_task_to_memory_component(ram);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			dma->prepare_memory_pages(ram);
			if (dma->check_task_in_alloc_write_read_queue(ram)==false&&
				dma->check_task_in_dma_queue()==false &&
				dma->check_task_in_gpu_queue(gpu_sm)==false) 
			{				
				break;
			}
			cout << "Time tick= " << time_tick << endl;
			time_tick = time_tick + 1;
		}
		dma->profile_swap_in_and_out(model);
		dma->profile_response_time(model);
		policy->our_policy(final, model, ram);
		dma->travel_DMA_queue();
		policy->file_output(model,ram);
	}	
	if (related_work) 
	{
		vector<Task_Set*> model;
		Task_Set* create_models = new Task_Set();
		int time_tick = 0;
		string path;
		RAM* ram = new RAM();
		GPU* gpu_sm = new GPU();
		GPU* gpu_sm2 = new GPU();
		DMA* dma = new DMA();
		Our_policy* policy = new Our_policy();
		cout << "==============simulation begin==============" << endl;
		create_models->create_model(model);
		//dma->add_model_to_DMA_queue(model);
		dma->add_model_to_DMA_queue_by_disorder(model);
		while (true)
		{
			if (time_tick == -1)
			{
				cout << "dma queue" << endl;
				dma->travel_DMA_queue();
				cout << "dma queue" << endl;
				dma->travel_DMA_queue_SM();
				cout << "victim queue" << endl;
				dma->travel_victim_queue();
				cout << "gpu queue" << endl;
				gpu_sm->travel_gpu_queue();
				cout << "allocate queue" << endl;
				ram->travel_allocate_queue();
				cout << "read queue" << endl;
				ram->travel_read_queue();
				cout << "write queue" << endl;
				ram->travel_write_queue();
			}
			if (ram->ram_loading > ram->system_memory_total_page)
			{
				cout << "bug here" << endl;
			}
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_writing_operation(dma, gpu_sm, time_tick);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			gpu_sm->execute_operation(dma);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_reading_operation(gpu_sm);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_allocating_operation(dma);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			dma->related_work_assign_task_to_memory_component(ram);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			dma->related_work_prepare_memory_pages(ram);
			if (dma->check_task_in_alloc_write_read_queue(ram) == false &&
				dma->check_task_in_dma_queue() == false &&
				dma->check_task_in_gpu_queue(gpu_sm) == false)
			{
				break;
			}
			cout << "Time tick= " << time_tick << endl;
			time_tick = time_tick + 1;
		}
		dma->profile_swap_in_and_out(model);
		dma->profile_response_time(model);
		dma->travel_DMA_queue();
		policy->file_output(model, ram);
	}
	if (ourpolicy)
	{
		vector<Task_Set*> model;
		Task_Set* create_models = new Task_Set();
		int time_tick = 0;
		string path;
		RAM* ram = new RAM();
		GPU* gpu_sm = new GPU();
		GPU* gpu_sm2 = new GPU();
		DMA* dma = new DMA();
		Our_policy* policy = new Our_policy();
		cout << "==============simulation begin==============" << endl;
		create_models->create_model(model);
		//dma->add_model_to_DMA_queue(model);
		dma->add_model_to_DMA_queue_by_disorder(model);
		policy->our_policy(start, model, ram);
		while (true)
		{
			if (time_tick == 2231 || time_tick == 2232 || time_tick == 2233)
			{
				cout << "dma queue" << endl;
				dma->travel_DMA_queue();
				cout << "victim queue" << endl;
				dma->travel_victim_queue();
				cout << "gpu queue" << endl;
				gpu_sm->travel_gpu_queue();
				cout << "allocate queue" << endl;
				ram->travel_allocate_queue();
				cout << "read queue" << endl;
				ram->travel_read_queue();
				cout << "write queue" << endl;
				ram->travel_write_queue();
			}
			if (ram->ram_loading > ram->system_memory_total_page)
			{
				cout << "bug here" << endl;
			}
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_writing_operation(dma, gpu_sm, time_tick);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			gpu_sm->execute_operation(dma);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_reading_operation(gpu_sm);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_allocating_operation(dma);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			dma->our_policy_assign_task_to_memory_component(ram);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			dma->our_policy_prepare_memory_pages(ram);
			if (dma->check_task_in_alloc_write_read_queue(ram) == false &&
				dma->check_task_in_dma_queue() == false &&
				dma->check_task_in_gpu_queue(gpu_sm) == false)
			{
				break;
			}
			cout << "Time tick= " << time_tick << endl;
			time_tick = time_tick + 1;
		}
		dma->profile_swap_in_and_out(model);
		dma->profile_response_time(model);
		policy->our_policy(final, model, ram);
		dma->travel_DMA_queue();
		policy->file_output(model, ram);
	}
	if (ourpolicy)
	{
		vector<Task_Set*> model;
		Task_Set* create_models = new Task_Set();
		int time_tick = 0;
		string path;
		RAM* ram = new RAM();
		GPU* gpu_sm = new GPU();
		GPU* gpu_sm2 = new GPU();
		DMA* dma = new DMA();
		Our_policy* policy = new Our_policy();
		cout << "==============simulation begin==============" << endl;
		create_models->create_model(model);
		//dma->add_model_to_DMA_queue(model);
		dma->add_model_to_DMA_queue_by_disorder(model);
		policy->our_policy(start, model, ram);
		while (true)
		{
			if (time_tick == 2231 || time_tick == 2232 || time_tick == 2233)
			{
				cout << "dma queue" << endl;
				dma->travel_DMA_queue();
				cout << "victim queue" << endl;
				dma->travel_victim_queue();
				cout << "gpu queue" << endl;
				gpu_sm->travel_gpu_queue();
				cout << "allocate queue" << endl;
				ram->travel_allocate_queue();
				cout << "read queue" << endl;
				ram->travel_read_queue();
				cout << "write queue" << endl;
				ram->travel_write_queue();
			}
			if (ram->ram_loading > ram->system_memory_total_page)
			{
				cout << "bug here" << endl;
			}
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_writing_operation(dma, gpu_sm, time_tick);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			gpu_sm->execute_operation(dma);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_reading_operation(gpu_sm);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			ram->execute_allocating_operation(dma);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			dma->our_policy_assign_task_to_memory_component(ram);
				dma->related_work_check_each_task_memory_equal_total(model, ram);
				dma->check_each_task_fit_memory_loading(ram, model);
			dma->our_policy_prepare_memory_pages(ram);
			if (dma->check_task_in_alloc_write_read_queue(ram) == false &&
				dma->check_task_in_dma_queue() == false &&
				dma->check_task_in_gpu_queue(gpu_sm) == false)
			{
				break;
			}
			cout << "Time tick= " << time_tick << endl;
			time_tick = time_tick + 1;
		}
		dma->profile_swap_in_and_out(model);
		dma->profile_response_time(model);
		policy->our_policy(final, model, ram);
		dma->travel_DMA_queue();
		policy->file_output(model, ram);
		/*****************************************Remove policy*****************************************/
		char* path_c = new char[(policy->path + "policy.txt").length() + 1];
		strcpy_s(path_c, (policy->path + "policy.txt").length() + 1, (policy->path + "policy.txt").c_str());
		if (remove(path_c) != 0)
			perror("Error deleting file.");
		else
			puts("File successfully deleted.");
	}
	
	cout << "==============simulation end==============" << endl;
	cout << "Hello world!" << endl;
	system("pause");
	return 0;
}





