#pragma once
#include "Task.h"
#include "Task_Set.h"
#include "GPU.h"

#define ACT_STBY	48.6
#define PRE_STBY	8.9
#define ACT_PDN		4.7
#define PRE_PDN		0.8
#define REF			22.5
#define RD			64.6
#define WR			35.3
#define ACT			153.9
#define DQ			117.3
#define termW		45.6
#define termRoth	40
#define termWoth	70.8
class DMA;
class RAM
{
public:
	/********************************Bandwdith, loading and memory total availabele pages***************/
	int bandwidth=20000;			//bandwidth (MBs) of RAM
	int ssd_bandwidth_read = 492;	//bandwidth (MBs) of reading of SSD
	int ssd_bandwidth_write = 445;	//bandwidth (MBs) of writing of SSD
									/*Samsung 860 EVO SATA 2 : R:268 MBs W:254 MBs idle: 35mW Active: 219mW*/
									/*Samsung 860 EVO SATA 3 : R:492 MBs W:445 MBs idle: 35mW Active: 219mW*/
									/*Samsung 970 EVO PCIE 3.0 : R:2338 MBs W:2128 MBs idle: 72mW Active: 1261mW*/
									/*source: https://www.anandtech.com/show/13761/the-samsung-970-evo-plus-ssd-review/7*/
	int ram_loading=0;				//number of ram pages
	int swap_loading=0;				//number of swap pages
	double utilization=0;			//utilization
			
	int system_memory_total_page = 4718592;	//number of total memory pages
									/*8G  : 2097152 pages*/
									/*10G :	2621440 pages*/
									/*12G :	3145728 pages*/
									/*14G :	3670016 pages*/
									/*16G : 4194304 pages*/
									/*18G : 4718592 pages*/
									/*benchmark 50% : 40988*/
									/*benchmark 75% : 61482*/
									/*benchmark 100% : 81976*/
	double swap_condition = 1.0;	/*memory 滿到多少比例開始用到swap機制*/
	/**********************************************Task head*********************************************/
	Task *head_of_allocate = 0;		//head of allocating queue
	Task *head_of_read = 0;			//head of reading queue
	Task *head_of_write = 0;		//head of writing queue	
	/*********************************************Power related  parameter*********************************************/
	double frequency_ratio = 1.0;	//frequency from 0 to 1
	double ram_power = frequency_ratio*(PRE_PDN+PRE_STBY+ACT_PDN+ACT_STBY+WR+RD)+REF+DQ+termW+termRoth+termWoth;	//power of memory
	double ram_background_power = 85.5;
	double ssd_power_idle = 72;		//idle power of ssd
	double ssd_power_work = 1261;	//working power of ssd
	int ram_energy_consumption = 0;	//total energy consumption of memory
	int ssd_energy_consumption = 0;	//total energy consumption of ssd
	RAM() {};
	~RAM() {};
	/****************************************New function***************************************/	
	void add_task_to_allocate_queue(Task *request);						//將sub task加入allocation queue
	void add_task_to_read_queue(Task* request);							//將sub task加入reading queue
	void add_task_to_write_queue(Task* request);						//將sub task加入writing queue
	void remove_task_from_allocate_queue(Task *request , DMA *dma);		//將sub task從allocation queue 移除並重新加入DMA queue
	void remove_task_from_read_queue(Task* request,GPU *gpu_sm);		//將sub task從reading queue 移除並加入gpu queue
	void remove_task_from_write_queue(Task *request , DMA *dma,GPU *gpu_sm);		//將sub task從writing queue 移除
	void execute_allocating_operation(DMA* dma);						//消耗allocation所要消耗的時間
	void execute_reading_operation(GPU* gpu_sm);						//消耗reading所要消耗的時間
	void execute_writing_operation(DMA* dma,GPU *gpu_sm,int time_tick);				//消耗writing所要消耗的時間
	/****************************************Release pages to Memory*********************************************/
	bool check_the_same_task_in_alloc_write_read_and_gpu(DMA *dma, GPU* gpu_sm, Task* sub_task);
	void release_model_memory(Task *thread, DMA * dma);							//透過這個trhead找到頭，把整個Task Set的記憶體清除。
	/*************************************************Traveling queue********************************************************/
	void travel_allocate_queue();										//travel整個allocation queue
	void travel_read_queue();											//travel整個reading queue
	void travel_write_queue();											//travel整個writing queue
	/*************************************************Queue reset***********************************************************/
	void clear();
	/***********************************************************************************************************************/
};