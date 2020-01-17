#include "Task_Set.h"


void Task_Set::create_model(string model_name, 
							int task_number, 
							int memory_page,  
							int gpu_exe,
							int block,
							int thread_per_block,
							int single_elpased_time)
{
	string path = this->path + model_name;
	ofstream model;
	model.open(path);
	model << fixed << setprecision(2);
	model << "task number " << task_number << endl;
	model << "total memory page " << memory_page << endl;
	model << "gpu execution time " << gpu_exe << endl;
	model << "block " << block << endl;
	model << "thread per block " << thread_per_block << endl;
	model << "single elapsed time " << single_elpased_time << endl;
	model.close();
}
void Task_Set::create_model(vector<Task_Set *> &model) 
{
	Task_Set *model1 = new Task_Set();
	//model1->create_model("benchmark_GESUMMV.txt",128,128*256 , 31, 1, 256, 634);
	model1->import_model("197000_5_low_computing.txt", 1);
	Task_Set *model2 = new Task_Set();
	model2->import_model("197000_7_low_computing.txt", 2);
	Task_Set *model3 = new Task_Set();
	model3->import_model("197000_9_low_computing.txt", 3);
	Task_Set *model4 = new Task_Set();
	model4->import_model("197000_11_low_computing.txt", 4);
	model.push_back(model1);
	model.push_back(model2);
	model.push_back(model3);
	model.push_back(model4);
}
void Task_Set::import_model(string model_name, int model_id) 
{
	string path = this->path + model_name;
	ifstream model;
	string line;
	model.open(path);
	int wait_exe = 0;
	int ready_exe = 0;
	int gpu_exe = 0;
	while (getline(model, line))
	{
		if (line.find("task number") <= line.length())
		{
			sub_task_number=atof(line.substr(12).c_str());
		}
		else if (line.find("total memory page") <= line.length()) 
		{
			total_memory_size = atof(line.substr(18).c_str());
		}
		else if (line.find("gpu execution time") <= line.length())
		{
			gpu_exe = atof(line.substr(19).c_str());
		}
		else if (line.find("thread per block") <= line.length())
		{
			thread_per_block = atof(line.substr(17).c_str());
		}
		else if (line.find("block") <= line.length())
		{
			block = atof(line.substr(6).c_str());
		}				
		else if (line.find("single elapsed time") <= line.length())
		{
			single_elpased_time = atof(line.substr(20).c_str());
		}
	}
	Task *new_thread;
	for (int i = 1; i <= sub_task_number; i++) 
	{
		new_thread = new Task();
		new_thread->model_id = model_id;
		new_thread->task_id = i;
		new_thread->total_memory_page = total_memory_size / sub_task_number;	
		new_thread->gpu[0] = new_thread->gpu[1] = gpu_exe;
		//new_thread->gpu[0] = new_thread->gpu[1] = 100;
		new_thread->thread_per_block = thread_per_block;
		new_thread->blocks = block;
		//new_thread->blocks = 8;
		new_thread->threads = new_thread->thread_per_block*new_thread->blocks;
		if (first == 0) {
			first = new_thread;
			first->head_thread_of_process = first;
		}
		else
		{	
			Task *current = first;
			while (current->next != 0) {
				current = current->next;
			}
			current->next = new_thread;
			new_thread->head_thread_of_process = first;
		}
	}
}
void Task_Set::import_model_by_irregular(string model_name, int model_id)
{
	srand(5);
	string path = this->path + model_name;
	ifstream model;
	string line;
	model.open(path);
	int wait_exe = 0;
	int ready_exe = 0;
	int gpu_exe = 0;
	while (getline(model, line))
	{
		if (line.find("task number") <= line.length())
		{
			sub_task_number = atof(line.substr(12).c_str());
		}
		else if (line.find("total memory page") <= line.length())
		{
			total_memory_size = atof(line.substr(18).c_str());
		}
		else if (line.find("gpu execution time") <= line.length())
		{
			gpu_exe = atof(line.substr(19).c_str());
		}
		else if (line.find("thread per block") <= line.length())
		{
			thread_per_block = atof(line.substr(17).c_str());
		}
		else if (line.find("block") <= line.length())
		{
			block = atof(line.substr(6).c_str());
		}
		else if (line.find("single elapsed time") <= line.length())
		{
			single_elpased_time = atof(line.substr(20).c_str());
		}
	}
	Task* new_thread;
	for (int i = 1; i <= sub_task_number; i++)
	{
		new_thread = new Task();
		new_thread->model_id = model_id;
		new_thread->task_id = i;
		new_thread->total_memory_page = (total_memory_size / sub_task_number) * (((rand() % 20) - 10) / 100.0 + 1);
		new_thread->gpu[0] = new_thread->gpu[1] = gpu_exe;
		//new_thread->gpu[0] = new_thread->gpu[1] = 100;
		new_thread->thread_per_block = thread_per_block;
		new_thread->blocks = block;
		new_thread->threads = new_thread->thread_per_block * new_thread->blocks;
		if (first == 0) {
			first = new_thread;
			first->head_thread_of_process = first;
		}
		else
		{
			Task* current = first;
			while (current->next != 0) {
				current = current->next;
			}
			current->next = new_thread;
			new_thread->head_thread_of_process = first;
		}
	}

}
void Task_Set::assign_priority(int priority) 
{
	Task *current = first;
	while (current != 0) 
	{
		current->priority = priority;
		current = current->next;
	}
}
void Task_Set::assign_alpha(double alpha)
{
	Task* current = first;
	while (current != 0)
	{
		current->alpha = alpha;
		current = current->next;
	}
}
