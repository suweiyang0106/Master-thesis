#pragma once
#include "Task_Set.h"
#include "Task.h"
#include "RAM.h"
#include "GPU.h"
#include "Our_policy.h"
#include <vector>
class DMA
{
	string path = "H:/M10607416/實驗室lab/RTLAB/simulator_input_output/2019_08_05/";
public:
	DMA() {};
	~DMA() {};
	/**********************************************Task head*********************************************/
	Task* head_of_DMA = 0;									//head of DMA queue
	Task* head_of_victim_list = 0;							//head of victim queue
	/**********************************************Add thread to queue***********************************/
	void add_task_to_DMA_queue(Task* sub_task);				//將sub task 加入 DMA queue
	//void add_model_to_DMA_queue(vector<Task_Set*>& model);	//分配各個sub task一開始如何放進DMA (目前以fifo)
	void add_model_to_DMA_queue_by_disorder(vector<Task_Set*>& model);
	void add_task_to_victim_queue(Task *sub_task);			//將sub task 加入victim queue
	void add_lru_counter(Task *sub_task);					//將LRU的counter做加一
	/**********************************************Remove thread from queue******************************/
	void remove_task_from_DMA_queue(Task* sub_task);		//remove the thread from the DMA queue
	void remove_task_from_victim_queue(Task* sub_task);		//將sub task從 victim queue裡移除
	/**********************************************Prepare memory pages**********************************/
	void prepare_memory_pages(RAM *ram);					//在memory上(RAM)整理出一段空間給head_of_DMA，而swap的動作也在裡面(原先作業系統的做法)
	bool check_victim_can_satisfy_dma(RAM *ram);
	void execution_swap_time(RAM *ram);						//扣除swap要花費的時間
	void organize_dma_and_victim_queue();					//整理dma，內容包含按照priority排程dma queue，以及重新更新victim queue裡的成員
	void organize_victim_queue();							//整理victim queue，將memory page為零的sub task從victim queue拿掉
	/**********************************************Calculate operation time******************************/
	void calculate_swap_time(RAM* ram);						//計算swap需要多少時間
	void calculate_swap_time_without_victim(RAM *ram);		//計算swap需要多少時間(不同於calculate_swap_time(RAM *ram)，這個是記憶體足夠  但資料要般移回來)
	void calculate_allocate_time(RAM *ram);					//計算allocation需要多少時間
	void calculate_read_time(RAM *ram);						//計算reading需要多少時間
	void calculate_write_time(RAM *ram);					//計算writing需要多少時間
	/**********************************************Travel the queue**************************************/
	void travel_DMA_queue();								//travel整個DMA queue，看裡面有誰
	void travel_victim_queue();								//travel整個victim queue
	void travel_DMA_queue_SM();
	void profile_swap_in_and_out(vector<Task_Set*> &model);	//將swap in / out的量印出來
	void profile_response_time(vector<Task_Set*>& model);	//將application 的 response time印出來(包含response time with/without interference和 latency)
	void check_each_task_fit_memory_loading(RAM* ram,vector<Task_Set*>& model);//check所有sub task的memory page加起來會等於當前的memory loading
	/**********************************************Assign thread to which memory operation***************/
	void assign_task_to_memory_component(RAM *ram);			//將head_of_DMA分配到各個component中
	/**********************************************Check if there is any thread in alloc, read or write component****/
	bool check_task_in_alloc_write_read_queue(RAM *ram);	//確認allocation queue, reading queue 和 writing queue有無sub task，有則回傳true，無則回傳false
	bool check_task_in_dma_queue();							//確認dma queue有無sub task，有則回傳true，無則回傳false
	bool check_task_in_gpu_queue(GPU *gpu_sm);				//確認gpu queue有無sub task，有則回傳true，無則回傳false
	/**********************************************Clear the parameters in the class****/
	void clear();											//清除dma queue裡頭的參數(目前僅有將head of DMA歸零)
	/**********************************************Our policy*********************************************/
	void our_policy_prepare_memory_pages(RAM *ram);			//在memory上(RAM)整理出一段空間給head_of_DMA，並按照我們給予的priority做排程(我們的架構以及做法)
	void our_policy_assign_task_to_memory_component(RAM *ram);//允許各個memory component能夠接收來自swap space的執行
	/**********************************************Related work*******************************************/
	void related_work_prepare_memory_pages(RAM *ram);
	void related_work_assign_task_to_memory_component(RAM *ram);
	void related_work_close_or_open_application(Task *sub_task, bool turn_on_or_off);		//將同屬一個application的所有sub task的SM_close調為true或是false
	void related_work_organize_dma_and_victim_queue();
	bool related_check_task_SM_close_in_dma_and_victim_queue();//check每個sub task在dma 和victim queue中都是false才回傳false。(用來確保我們只會關掉其中一個application)
	void related_work_check_each_task_memory_equal_total(vector<Task_Set *>&model, RAM* ram);
	/**********************************************Sort queue*********************************************/
	void sort_dma_queue(Task** headRef);
	void splitList_dma_queue(Task* source, Task** frontRef, Task** backRef);
	Task* mergelist_dma_queue(Task* a, Task* b);

	void sort_victim_queue(Task** headRef);
	void splitList_victim_queue(Task* source, Task** frontRef, Task** backRef);
	Task* mergelist_victim_queue(Task* a, Task* b);

	void sort_dma_queue_by_SM_close(Task** headRef);
	void splitList_dma_queue_by_SM_close(Task* source, Task** frontRef, Task** backRef);
	Task* mergelist_dma_queue_by_SM_close(Task* a, Task* b);
};

