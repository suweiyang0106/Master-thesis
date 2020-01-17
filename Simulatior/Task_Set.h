#pragma once
#include <cstdlib> 
#include <ctime>  
#include <iostream> 
#include <iomanip>
#include<vector>
#include <fstream>
#include <string>
#include "Task.h"
using namespace std;
/*A Task_Set is a model*/
class Task_Set
{
	string path = "D:/��s��/�Ѻ�_���]/Input/";//��Ʋ��ͻPŪ�J�����|
public:
	int total_memory_size=0;					//�o��sub_task�n�ϥΪ�memory�q
	int block = 0;								//gpu �ҥΨ쪺block
	int thread_per_block = 0;					//gpu �C��block�ҥΨ쪺thread
	int sub_task_number = 0;						//�o��application���X��sub task
	Task *first=0;								//head of thread for a process
	Task_Set *next=0;							//next process of this process
	/******************************************Elpased Time, Contention Degree and Alpha******************************/
	int actual_latency = 0;						//model��latency�A���h��Task Set�z�Z
	int elapsed_time = 0;						//model������ɶ��A���h��Task Set�z�Z
	int single_elpased_time = 0;				//��@��model����ɶ��A�L��LTask Set�z�Z
	double alpha = 1.0;							//�F���M�w��Alpha
	double contention_degree = 0;				//�v�T�ҾɭP��contention degree
	/*****************************************************************************************************************/
public:
	Task_Set() {};
	~Task_Set() {};
	void create_model(string model_name, 
						int task_number, 
						int memory_page, 
						int gpu_exe, 
						int block, 
						int thread_per_block, 
						int single_elpased_time);			//�Хߧڭ̪�input�ܦ��ɮ׫���
	void create_model(vector<Task_Set *> &models);			//�פJmodel
	void import_model(string model_name, int model_id);		//�פJmodel
	void import_model_by_irregular(string model_name, int model_id);
	void assign_priority(int priority);						//���tpriority
	void assign_alpha(double alpha);						//���talpha
};

