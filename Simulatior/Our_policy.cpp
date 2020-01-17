#include "Our_policy.h"
#define final 1
#define start 0
using namespace std;
void Our_policy::our_policy(int start_or_end, vector<Task_Set *>model, RAM *ram)
{
	if (start_or_end == start)
	{
		//At the beginning
		string line;
		string str;
		string str2;
		int str_number = 0;
		bool keep_policy = false;
		ifstream policy;
		policy.open(this->path + "policy.txt");
		while (getline(policy, line))
		{
			str = "Model" + to_string(model[model.size() - 1]->first->model_id) + "_priority ";
			str2 = "Model" + to_string((model[model.size() - 1]->first->model_id) + 1) + "_priority ";
			if (line.find(str) <= line.length())
			{
				keep_policy = true;
			}
			else if (line.find(str2) <= line.length())
			{
				keep_policy = false;
			}
		}
		policy.close();
		if (!keep_policy)
		{
			char *path_c = new char[(this->path + "policy.txt").length() + 1];
			strcpy_s(path_c, (this->path + "policy.txt").length() + 1, (this->path + "policy.txt").c_str());
			if (remove(path_c) != 0)
				perror("Error deleting file.");
			else
				puts("File successfully deleted.");
		}		
		/***************************************Setting prioity and alpha****************************************/
		policy.open(this->path + "policy.txt");
		while (keep_policy && getline(policy, line))
		{
			for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
			{
				str = "Model" + to_string((*it)->first->model_id) + "_priority ";
				for (int i = 0; str[i]; i++)
					str_number++;
				if (line.find(str) <= line.length())
				{
					(*it)->assign_priority(atof(line.substr(str_number).c_str()));
				}
				str_number = 0;
				str = "Model" + to_string((*it)->first->model_id) + "_alpha ";
				for (int i = 0; str[i]; i++)
					str_number++;
				if (line.find(str) <= line.length())
				{
					(*it)->alpha = 1.0 - atof(line.substr(str_number).c_str());
					(*it)->assign_alpha((*it)->alpha);//為了滿足sudio code所以減一
				}
				str_number = 0;
			}

		}
		policy.close();
		return;
	}
	else if (start_or_end == final)
	{
		bool execute_schdule_policy = true;
		bool execute_thrashing_policy = true;
		compute_models_new_contention_degree(model);
		/*再讀一次檔案並恢復一開始的priority*/
		int priority = 0;
		string line;
		string str;
		int str_number = 0;
		ifstream policy_input;
		policy_input.open(this->path + "policy.txt");
		while (getline(policy_input, line))
		{
			for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
			{
				str = "Model" + to_string((*it)->first->model_id) + "_priority ";
				for (int i = 0; str[i]; i++)
					str_number++;
				if (line.find(str) <= line.length())
				{
					(*it)->assign_priority(atof(line.substr(str_number).c_str()));
				}
				str_number = 0;
			}

		}
		policy_input.close();
		priority = model[0]->first->priority;
		for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
		{
			if (priority != (*it)->first->priority)
			{
				execute_schdule_policy = false;
				break;
			}
			priority = (*it)->first->priority;
		}
		for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
		{
			if (execute_schdule_policy || (*it)->alpha != 1.0)//為了滿足sudio code所以在此減一
			{
				execute_thrashing_policy = false;
				break;
			}
		}
		if (execute_thrashing_policy)
			compute_models_alpha(model, ram);
		if (execute_schdule_policy)
			compute_models_priority(model);
		ofstream policy;
		policy.open(this->path + "policy.txt");
		policy << fixed << setprecision(2);
		policy << "Contention Degree" << endl;
		for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
		{
			policy << "Model" << (*it)->first->model_id << "_contention " << (*it)->contention_degree << endl;
		}
		policy << "Priority" << endl;
		for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
		{
			policy << "Model" << (*it)->first->model_id << "_priority " << (*it)->first->priority << endl;
		}
		policy << "Alpha" << endl;
		for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
		{
			policy << "Model" << (*it)->first->model_id << "_alpha " << 1.00-(*it)->alpha << endl;//為了滿足sudio code所以在此減一
		}
		return;
	}
	cout << "Error state." << endl;
}
void	Our_policy::compute_models_new_contention_degree(vector<Task_Set*>model) 
{
	for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
	{
		(*it)->contention_degree = (*it)->actual_latency / (1.0*(*it)->single_elpased_time) / (1.0 * (*it)->sub_task_number);
	}
}
void	Our_policy::compute_models_priority(vector<Task_Set*>model) //根據contention的程度設置各項model的prioirty
{
	vector<Task_Set*>models_priority;
	for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
	{
		models_priority.push_back((*it));
	}
	sort(models_priority.begin(), models_priority.end(),
		[](const Task_Set  *a, const Task_Set  *b) -> bool {	return (a->contention_degree) > (b->contention_degree); }
	);
	int i = 1;
	for (vector<Task_Set *>::iterator it = models_priority.begin(); it != models_priority.end(); it++)
	{
		(*it)->assign_priority(i);
		i++;
	}
}				
void	Our_policy::compute_models_alpha(vector<Task_Set*>model, RAM *ram) //根據swap的情形去設置分配的比例
{
	vector<Task_Set*> alpha_setting;
	Task *current = 0;
	int total_task_pages = 0;
	int current_block = 0;
	int swap_request = 0;
	for (vector<Task_Set *>::iterator it = model.begin(); it != model.end(); it++)
		alpha_setting.push_back((*it));
	sort(alpha_setting.begin(), alpha_setting.end(),
		[](const Task_Set  *a, const Task_Set  *b) -> bool {	return a->first->priority > b->first->priority; }
	);
	for (vector<Task_Set *>::iterator it = alpha_setting.begin(); it != alpha_setting.end(); it++) 
	{
		Task *current = (*it)->first;
		while (current != 0) 
		{
			total_task_pages = total_task_pages + current->total_memory_page;
			current = current->next;
		}		
	}
	swap_request = total_task_pages - floor(ram->system_memory_total_page*ram->swap_condition);
	/*從最低priorty的task開始分*/
	for (vector<Task_Set *>::iterator it = alpha_setting.begin(); it != alpha_setting.end(); it++) 
	{		
		int task_total_pages=0;
		Task *current = (*it)->first;
		while (current != 0) 
		{
			task_total_pages = task_total_pages + current->total_memory_page;
			current = current->next;
		}
		if (swap_request < task_total_pages && 0 < swap_request) 
		{
			//(*it)->alpha = 1.0*floor(100.0-((100.0* swap_request) / task_total_pages))/100;
			(*it)->alpha = 1.0-1.0 * ceil((100.0 * swap_request) / task_total_pages) / 100; //留在memory的比例
			break;
		}
		else if(0 < swap_request)
		{
			swap_request = swap_request - task_total_pages;
			(*it)->alpha = 0.00;
		}
	}	
}
void Our_policy::file_output(vector<Task_Set*> model, RAM* ram)
{
	ofstream outData;
	outData.open(path + "our_policy_SATA_2.csv", ios::app);
	outData << "Single elapsed time" << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << "model" << (*it)->first->model_id << ",";
	outData << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << (*it)->single_elpased_time << ",";
	outData << endl;
	outData << "Elapsed time" << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << "model" << (*it)->first->model_id << ",";
	outData << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << (*it)->elapsed_time << ",";
	outData << endl;
	outData << "Latency" << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << "model" << (*it)->first->model_id << ",";
	outData << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << (*it)->actual_latency << ",";
	outData << endl;
	outData << "Contention degree" << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << "model" << (*it)->first->model_id << ",";
	outData << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << (*it)->contention_degree << ",";
	outData << endl;
	outData << "Priority setting" << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << "model" << (*it)->first->model_id << ",";
	outData << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << (*it)->first->priority << ",";
	outData << endl;
	outData << "Swap ratio" << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << "model" << (*it)->first->model_id << ",";
	outData << endl;
	for (vector<Task_Set*>::iterator it = model.begin(); it != model.end(); it++)
		outData << (*it)->alpha << ",";
	outData << endl;
	outData << "Power sum" << endl;
	outData << "RAM(mW),SSD(mW)" << endl;
	outData << ram->ram_energy_consumption << "," << ram->ssd_energy_consumption << endl;
	outData.close();
}
Our_policy::Our_policy()
{
}
Our_policy::~Our_policy()
{
}
