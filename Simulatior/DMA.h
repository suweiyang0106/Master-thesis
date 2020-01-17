#pragma once
#include "Task_Set.h"
#include "Task.h"
#include "RAM.h"
#include "GPU.h"
#include "Our_policy.h"
#include <vector>
class DMA
{
	string path = "H:/M10607416/�����lab/RTLAB/simulator_input_output/2019_08_05/";
public:
	DMA() {};
	~DMA() {};
	/**********************************************Task head*********************************************/
	Task* head_of_DMA = 0;									//head of DMA queue
	Task* head_of_victim_list = 0;							//head of victim queue
	/**********************************************Add thread to queue***********************************/
	void add_task_to_DMA_queue(Task* sub_task);				//�Nsub task �[�J DMA queue
	//void add_model_to_DMA_queue(vector<Task_Set*>& model);	//���t�U��sub task�@�}�l�p���iDMA (�ثe�Hfifo)
	void add_model_to_DMA_queue_by_disorder(vector<Task_Set*>& model);
	void add_task_to_victim_queue(Task *sub_task);			//�Nsub task �[�Jvictim queue
	void add_lru_counter(Task *sub_task);					//�NLRU��counter���[�@
	/**********************************************Remove thread from queue******************************/
	void remove_task_from_DMA_queue(Task* sub_task);		//remove the thread from the DMA queue
	void remove_task_from_victim_queue(Task* sub_task);		//�Nsub task�q victim queue�̲���
	/**********************************************Prepare memory pages**********************************/
	void prepare_memory_pages(RAM *ram);					//�bmemory�W(RAM)��z�X�@�q�Ŷ���head_of_DMA�A��swap���ʧ@�]�b�̭�(����@�~�t�Ϊ����k)
	bool check_victim_can_satisfy_dma(RAM *ram);
	void execution_swap_time(RAM *ram);						//����swap�n��O���ɶ�
	void organize_dma_and_victim_queue();					//��zdma�A���e�]�t����priority�Ƶ{dma queue�A�H�έ��s��svictim queue�̪�����
	void organize_victim_queue();							//��zvictim queue�A�Nmemory page���s��sub task�qvictim queue����
	/**********************************************Calculate operation time******************************/
	void calculate_swap_time(RAM* ram);						//�p��swap�ݭn�h�֮ɶ�
	void calculate_swap_time_without_victim(RAM *ram);		//�p��swap�ݭn�h�֮ɶ�(���P��calculate_swap_time(RAM *ram)�A�o�ӬO�O���騬��  ����ƭn�벾�^��)
	void calculate_allocate_time(RAM *ram);					//�p��allocation�ݭn�h�֮ɶ�
	void calculate_read_time(RAM *ram);						//�p��reading�ݭn�h�֮ɶ�
	void calculate_write_time(RAM *ram);					//�p��writing�ݭn�h�֮ɶ�
	/**********************************************Travel the queue**************************************/
	void travel_DMA_queue();								//travel���DMA queue�A�ݸ̭�����
	void travel_victim_queue();								//travel���victim queue
	void travel_DMA_queue_SM();
	void profile_swap_in_and_out(vector<Task_Set*> &model);	//�Nswap in / out���q�L�X��
	void profile_response_time(vector<Task_Set*>& model);	//�Napplication �� response time�L�X��(�]�tresponse time with/without interference�M latency)
	void check_each_task_fit_memory_loading(RAM* ram,vector<Task_Set*>& model);//check�Ҧ�sub task��memory page�[�_�ӷ|�����e��memory loading
	/**********************************************Assign thread to which memory operation***************/
	void assign_task_to_memory_component(RAM *ram);			//�Nhead_of_DMA���t��U��component��
	/**********************************************Check if there is any thread in alloc, read or write component****/
	bool check_task_in_alloc_write_read_queue(RAM *ram);	//�T�{allocation queue, reading queue �M writing queue���Lsub task�A���h�^��true�A�L�h�^��false
	bool check_task_in_dma_queue();							//�T�{dma queue���Lsub task�A���h�^��true�A�L�h�^��false
	bool check_task_in_gpu_queue(GPU *gpu_sm);				//�T�{gpu queue���Lsub task�A���h�^��true�A�L�h�^��false
	/**********************************************Clear the parameters in the class****/
	void clear();											//�M��dma queue���Y���Ѽ�(�ثe�Ȧ��Nhead of DMA�k�s)
	/**********************************************Our policy*********************************************/
	void our_policy_prepare_memory_pages(RAM *ram);			//�bmemory�W(RAM)��z�X�@�q�Ŷ���head_of_DMA�A�ë��ӧڭ̵�����priority���Ƶ{(�ڭ̪��[�c�H�ΰ��k)
	void our_policy_assign_task_to_memory_component(RAM *ram);//���\�U��memory component��������Ӧ�swap space������
	/**********************************************Related work*******************************************/
	void related_work_prepare_memory_pages(RAM *ram);
	void related_work_assign_task_to_memory_component(RAM *ram);
	void related_work_close_or_open_application(Task *sub_task, bool turn_on_or_off);		//�N�P�ݤ@��application���Ҧ�sub task��SM_close�լ�true�άOfalse
	void related_work_organize_dma_and_victim_queue();
	bool related_check_task_SM_close_in_dma_and_victim_queue();//check�C��sub task�bdma �Mvictim queue�����Ofalse�~�^��false�C(�ΨӽT�O�ڭ̥u�|�����䤤�@��application)
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

