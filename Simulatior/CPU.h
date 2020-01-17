#pragma once
#include "Task.h"
#include "Task_Set.h"
class CPU
{
public:
	int loading;
	double frequency=1;				//2000MHz
	int utilization;				//utilization
	int power;						//power
	Task *head_of_ready_queue=0;	//head of ready queue	
	Task *head_of_waiting_queue=0;	//head of waiting queue
	Task *current_ready_queue=0;	//current task of ready queue
	Task *current_waiting_queue=0;	//current task of waiting queue
	double power_idle = 27.72;		//idle power 27.72mW
	int energy_consumption = 0;

public:
	CPU() {};
	~CPU() {};
	void add_thread_to_cortex_waiting_queue(Task *thread);
	void add_thread_to_cortex_waiting_queue_2(Task *thread);
	void add_thread_to_cortex_waiting_queue_3(Task *thread);
	void add_thread_to_cortex_waiting_queue_4(Task *thread);
	void add_thread_to_denver_waiting_queue(Task *thread);
	void add_thread_to_denver_waiting_queue_2(Task *thread);
	void add_thread_to_ready_queue(Task *thread);
	void execute_thread_in_waiting_queue(Task *thread,int time);
	void execute_thread_in_ready_queue(Task *thread,int time);
	void remove_thread_from_waiting_queue(Task *thread);
	void remove_thread_from_waiting_queue_2(Task *thread);
	void remove_thread_from_waiting_queue_3(Task *thread);
	void remove_thread_from_waiting_queue_4(Task *thread);
	void remove_thread_from_denver_waiting_queue(Task *thread);
	void remove_thread_from_denver_waiting_queue_2(Task *thread);
	void remove_thread_from_ready_queue(Task *thread);
	void travel_waiting_queue();
	void travel_waiting_queue_2();
	void travel_waiting_queue_3();
	void travel_waiting_queue_4();
	void travel_waiting_queue_5();
	void travel_waiting_queue_6();
	void travel_ready_queue();
	/*************************************************Accumulate power***********************************************************/
	void accucmulate_cpu_active_power();
	void accucmulate_cpu_idle_power();
	void travel_power_consumption();
	/*************************************************Queue reset***********************************************************/
	void clear();
	/*************************************************Queue sorting*********************************************************/

	void sort_ready_queue(Task **headRef);
	Task *mergelist_ready_queue(Task *a,Task *b);
	void splitList_ready_queue(Task *source,Task **frontRef, Task **backRef);

	void sort_waiting_queue_1(Task **headRef);
	Task *mergelist_waiting_queue_1(Task *a, Task *b);
	void splitList_waiting_queue_1(Task *source, Task **frontRef, Task **backRef);
	
	void sort_waiting_queue_2(Task **headRef);
	Task *mergelist_waiting_queue_2(Task *a, Task *b);
	void splitList_waiting_queue_2(Task *source, Task **frontRef, Task **backRef);
	
	void sort_waiting_queue_3(Task **headRef);
	Task *mergelist_waiting_queue_3(Task *a, Task *b);
	void splitList_waiting_queue_3(Task *source, Task **frontRef, Task **backRef);
	
	void sort_waiting_queue_4(Task **headRef);
	Task *mergelist_waiting_queue_4(Task *a, Task *b);
	void splitList_waiting_queue_4(Task *source, Task **frontRef, Task **backRef);

	void sort_waiting_queue_5(Task **headRef);
	Task *mergelist_waiting_queue_5(Task *a, Task *b);
	void splitList_waiting_queue_5(Task *source, Task **frontRef, Task **backRef);

	void sort_waiting_queue_6(Task **headRef);
	Task *mergelist_waiting_queue_6(Task *a, Task *b);
	void splitList_waiting_queue_6(Task *source, Task **frontRef, Task **backRef);
	/***********************************************************************************************************************/
	friend class RAM;
};

