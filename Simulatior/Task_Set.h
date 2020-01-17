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
	string path = "D:/研究所/書維_打包/Input/";//資料產生與讀入的路徑
public:
	int total_memory_size=0;					//這個sub_task要使用的memory量
	int block = 0;								//gpu 所用到的block
	int thread_per_block = 0;					//gpu 每個block所用到的thread
	int sub_task_number = 0;						//這個application有幾個sub task
	Task *first=0;								//head of thread for a process
	Task_Set *next=0;							//next process of this process
	/******************************************Elpased Time, Contention Degree and Alpha******************************/
	int actual_latency = 0;						//model的latency，有多個Task Set干擾
	int elapsed_time = 0;						//model的執行時間，有多個Task Set干擾
	int single_elpased_time = 0;				//單一個model執行時間，無其他Task Set干擾
	double alpha = 1.0;							//政策決定的Alpha
	double contention_degree = 0;				//影響所導致的contention degree
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
						int single_elpased_time);			//創立我們的input變成檔案型式
	void create_model(vector<Task_Set *> &models);			//匯入model
	void import_model(string model_name, int model_id);		//匯入model
	void import_model_by_irregular(string model_name, int model_id);
	void assign_priority(int priority);						//分配priority
	void assign_alpha(double alpha);						//分配alpha
};

