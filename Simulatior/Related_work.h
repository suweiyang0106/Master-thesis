#pragma once
#include <iostream> 
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
#include "Our_policy.h"
#include "RAM.h"
#include "CPU.h"
using namespace std;
class Related_work
{	
	int T_MIL = 10;			//memory interference latency
	int O_thr =0;			//memory requests threshold
	CPU *normal_group;
	string path = "D:/2019_07_26_progress/Experiment/2019_07_26/";
public:
	Related_work();
	~Related_work();
	void excessive_memory_contention_prediction( CPU* cortex, CPU* cortex2, CPU* cortex3, CPU* cortex4,CPU *denver, RAM* ram, int time_tick);
	bool critical_task_chain_manager(CPU *normal_group, CPU *cortex, CPU *cortex2, CPU *cortex3, CPU *cortex4);	//critical task chain manager. The funcion is responsible for put task into the critical cgroup.	
	CPU* Related_work::differentiate_normal_group_and_critical_group(CPU* cortex, CPU* cortex2, CPU* cortex3, CPU* cortex4, CPU* denver);
	void find_task_priority(int & cortex_priority,int& cortex2_priority,int& cortex3_priority,int &cortex4_priority,int &denver_priority, CPU* cortex, CPU* cortex2, CPU* cortex3, CPU* cortex4, CPU* denver);//р–queue┮局Τ程priorityㄓゑт程cpu
	void find_task_priority(Task *current,int &priority);//рwaiting queue柑sub task程priority计тㄓ
	void decrease_memory_request_rate(CPU *normal_group,double decreasing_frequency,double increasing_frequency);
	void increase_memory_request_rate(CPU *normal_group,double decreasing_frequency,double increasing_frequency);
};

