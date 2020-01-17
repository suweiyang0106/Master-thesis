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
#include "RAM.h"
#include "GPU.h"
using namespace std;
class Our_policy
{	
public:
	Our_policy();
	~Our_policy();
	string path = "D:/研究所/書維_打包/Output/2019_08_28/";//結果檔案與方法生成的路徑
	void	our_policy(int start_or_end, vector<Task_Set *>model, RAM *ram);//我們的方法(使用下面的function)
	void	compute_models_new_contention_degree(vector<Task_Set*>model);	//以latency和sub task數量計算contention的程度
	void	compute_models_priority(vector<Task_Set*>model);				//根據contention的程度設置各項model的prioirty
	void	compute_models_alpha(vector<Task_Set*>model, RAM *ram);			//根據swap的情形去設置分配的比例
	void	file_output(vector<Task_Set*>model,RAM *ram);					//將結果輸出成csv檔案
	};

