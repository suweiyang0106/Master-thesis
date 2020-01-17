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
	string path = "D:/��s��/�Ѻ�_���]/Output/2019_08_28/";//���G�ɮ׻P��k�ͦ������|
	void	our_policy(int start_or_end, vector<Task_Set *>model, RAM *ram);//�ڭ̪���k(�ϥΤU����function)
	void	compute_models_new_contention_degree(vector<Task_Set*>model);	//�Hlatency�Msub task�ƶq�p��contention���{��
	void	compute_models_priority(vector<Task_Set*>model);				//�ھ�contention���{�׳]�m�U��model��prioirty
	void	compute_models_alpha(vector<Task_Set*>model, RAM *ram);			//�ھ�swap�����Υh�]�m���t�����
	void	file_output(vector<Task_Set*>model,RAM *ram);					//�N���G��X��csv�ɮ�
	};

