#include "Related_work.h"



Related_work::Related_work()
{
}
Related_work::~Related_work()
{
}
void Related_work::excessive_memory_contention_prediction( CPU* cortex, CPU* cortex2, CPU* cortex3, CPU* cortex4, CPU* denver, RAM* ram, int time_tick)
{
	if(time_tick==1)
	{
		normal_group = differentiate_normal_group_and_critical_group(cortex, cortex2, cortex3, cortex4, denver);
	}
	if (time_tick % T_MIL == 0)
	{
		//activate the predictor
		if (critical_task_chain_manager(denver, cortex, cortex2, cortex3, cortex4))
		{
			Task* cortex_waiting = cortex->head_of_waiting_queue;
			Task* cortex2_waiting = cortex2->head_of_waiting_queue;
			Task* cortex3_waiting = cortex3->head_of_waiting_queue;
			Task* cortex4_waiting = cortex4->head_of_waiting_queue;
			Task* denver_waiting = denver->head_of_waiting_queue;
			Task* alloc = ram->head_of_allocate;
			Task* read = ram->head_of_read;
			Task* write = ram->head_of_write;			
			int request_number = 0;
			while (cortex_waiting != 0)
			{
				cortex_waiting = cortex_waiting->cortex_waiting_queue_next;
				request_number++;
			}
			while (cortex2_waiting != 0)
			{
				cortex2_waiting = cortex2_waiting->cortex_waiting_queue_next2;
				request_number++;
			}
			while (cortex3_waiting != 0)
			{
				cortex3_waiting = cortex3_waiting->cortex_waiting_queue_next3;
				request_number++;
			}
			while (cortex4_waiting != 0)
			{
				cortex4_waiting = cortex4_waiting->cortex_waiting_queue_next4;
				request_number++;
			}
			while (denver_waiting != 0)
			{
				denver_waiting = denver_waiting->denver_waiting_queue_next;
				request_number++;
			}
			while (alloc != 0)
			{
				alloc = alloc->ram_allocate_next;
				request_number++;
			}
			while (read != 0)
			{
				read = read->ram_read_next;
				request_number++;
			}
			while (write != 0)
			{
				write = write->ram_write_next;
				request_number++;
			}

			if (request_number > O_thr)
			{
				decrease_memory_request_rate(cortex4, 0.35, 1.0);
			}
			else
			{
				increase_memory_request_rate(cortex4, 0.35, 1.0);
			}
		}
	}
}
bool Related_work::critical_task_chain_manager(CPU *normal_group, CPU *cortex,  CPU *cortex2, CPU *cortex3, CPU *cortex4)
{
	if (cortex->head_of_waiting_queue != 0|| cortex2->head_of_waiting_queue != 0|| cortex3->head_of_waiting_queue != 0|| cortex4->head_of_waiting_queue != 0)
		return true;
	else
		return false;
}
bool sortbysec2(const pair<CPU*, int>& a,
	const pair<CPU*, int>& b)
{
	return (a.second > b.second);
}
CPU *Related_work::differentiate_normal_group_and_critical_group(CPU* cortex, CPU* cortex2, CPU* cortex3, CPU* cortex4, CPU* denver)
{
	Task *cortex_current=cortex->head_of_waiting_queue;
	Task* cortex2_current = cortex2->head_of_waiting_queue;
	Task* cortex3_current = cortex3->head_of_waiting_queue;
	Task* cortex4_current = cortex4->head_of_waiting_queue;
	Task* denver_current = denver->head_of_waiting_queue;
	vector <pair<CPU*,int>> pick_normal_group;
	int cortex_priority		= 1;
	int cortex2_priority	= 1;
	int cortex3_priority	= 1;
	int cortex4_priority	= 1;
	int denver_priority		= 1;
	find_task_priority(cortex_priority, cortex2_priority, cortex3_priority, cortex3_priority,denver_priority,cortex,cortex2,cortex3,cortex4,denver);
	pick_normal_group.push_back(make_pair(cortex, cortex_priority));
	pick_normal_group.push_back(make_pair(cortex2, cortex2_priority));
	pick_normal_group.push_back(make_pair(cortex3, cortex3_priority));
	pick_normal_group.push_back(make_pair(cortex4, cortex4_priority));
	pick_normal_group.push_back(make_pair(denver, denver_priority));
		
	sort(pick_normal_group.begin(), pick_normal_group.end(), sortbysec2);
	return pick_normal_group[0].first;
}
void Related_work::find_task_priority(int& cortex_priority, int& cortex2_priority, int& cortex3_priority, int& cortex4_priority, int& denver_priority, CPU* cortex, CPU* cortex2, CPU* cortex3, CPU* cortex4, CPU* denver)
{
	Task *cortex_current=cortex->head_of_waiting_queue;
	Task* cortex2_current = cortex2->head_of_waiting_queue;
	Task* cortex3_current = cortex3->head_of_waiting_queue;
	Task* cortex4_current = cortex4->head_of_waiting_queue;
	Task* denver_current = denver->head_of_waiting_queue;
	find_task_priority(cortex_current, cortex_priority);
	find_task_priority(cortex2_current, cortex2_priority);
	find_task_priority(cortex3_current, cortex3_priority);
	find_task_priority(cortex4_current, cortex4_priority);
	find_task_priority(denver_current, denver_priority);	
}
void Related_work::find_task_priority(Task* current, int& priority)
{
	ifstream related_work;
	string line;
	string str;
	string str2;
	int str_number = 0;
	related_work.open(path + "policy.txt");
	while (current != 0)
	{
		while (getline(related_work, line))
		{
			str = "Model" + to_string(current->model_id) + "_priority ";
			for (int i = 0; str[i]; i++)
				str_number++;
			if (line.find(str) <= line.length())
			{
				if (atof(line.substr(str_number).c_str()) > priority)
					priority = atof(line.substr(str_number).c_str());
			}
			str_number = 0;
		}
		current = current->cortex_waiting_queue_next;
	}
	related_work.close();
}
void Related_work::increase_memory_request_rate(CPU *normal_group, double decreasing_frequency,double increasing_frequency)
{	
	Task *current = normal_group->head_of_waiting_queue;
	//while (current != 0 && normal_group->frequency == decreasing_frequency)
	while (current != 0 )
	{	
		if(current->cpu_waiting[0]!=100)
		{
			current->cpu_waiting[0] = int(1.0 * current->cpu_waiting[0] * normal_group->frequency);
			current->cpu_waiting[1] = int(1.0*current->cpu_waiting[1] * normal_group->frequency);
		}
		current = current->next;
	}
	normal_group->frequency = increasing_frequency;
}
void Related_work::decrease_memory_request_rate(CPU *normal_group, double decreasing_frequency, double increasing_frequency)
{
	Task *current = normal_group->head_of_waiting_queue;
	//while (current != 0 && normal_group->frequency == increasing_frequency)
	while (current != 0)
	{
		normal_group->frequency = decreasing_frequency;
		if(current->cpu_waiting[0]==100)
		{
			current->cpu_waiting[0] = int(1.0 * current->cpu_waiting[0] / normal_group->frequency);
			current->cpu_waiting[1] = int(1.0*current->cpu_waiting[1] / normal_group->frequency);
		}
		current = current->next;
	}
}
