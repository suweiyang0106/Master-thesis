#pragma once
/*A Task is a thread*/
class Task
{
public:
	int model_id = 0;
	int task_id = 0;
	int start_time = 0;
	int end_time = 0;
	int total_memory_page = 0;					//total memory pages requiring.
	int memory_page=0;							//allocated number of pages
	int ssd_page=0;								//allocated number of pages	
	int threads = 0;							//thread per block(Means how much threads in gpu execute the task.)
	int blocks=0;								//how many block will use for the task in gpu.
	int thread_per_block = 0;					//how many thread in a block.
	
	
	Task *head_thread_of_process = 0;			//head task in Task_Set
	Task *next=0;								//next task in Task_Set
	Task *gpu_next=0;							//next task in the GPU sm
	Task *ram_read_next=0;						//next task in the RAM read
	Task *ram_write_next=0;						//next task in the RAM write
	Task *ram_allocate_next=0;					//next task in the RAM allocate
	Task *ram_victim_next = 0;					//next task in victim queue
	Task* DMA_queue_next = 0;

	Task *mix_next = 0;
	/******************************************I/O counter*******************************************************************/
	int block_in = 0;							//block in; pages from ssd to memory
	int block_out = 0;							//block out; pages from memory to ssd
	/******************************************LRU counter*******************************************************************/
	int lru_counter = 0;
	/******************************************Application in the SM close or not********************************************/
	bool SM_close = false;						//用各項sub task來確認這個application 能不能繼續執行還是只能被迫移出page來。(false繼續執行,true不行)
	/******************************************Execution & remain time for each component************************************/
	//*[0]代表執行時間  *[1]代表剩餘時間
	int mem_allocate[2]			={0};			//time of cpu allocating to memory
	int mem_read[2]				={0};			//time of gpu getting data from memory
	int mem_write[2]			={0};			//time of gpu writting data to memory
	int mem_swap[2]				={0};			//time of ram swapping data from ram to swap space	
	int gpu[2]					={0};			//time of gpu executing tasks	
	int priority = 1;
	double alpha = 1.0;
	/***********************************************************************************************************************/

public:
	Task() {};
	~Task() {};
};

