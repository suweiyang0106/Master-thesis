#include "CPU.h"
#include <iostream> 
using namespace std;


void CPU::add_thread_to_cortex_waiting_queue(Task *thread)
{
	if (head_of_waiting_queue == 0)
	{
		head_of_waiting_queue = thread;
		return;
	}
	/*if waiting
 queue is not empty.*/
	Task *waiting_current = head_of_waiting_queue;
	while (waiting_current->cortex_waiting_queue_next != 0)
	{
		waiting_current = waiting_current->cortex_waiting_queue_next;
	}
	waiting_current->cortex_waiting_queue_next = thread;
}
void CPU::add_thread_to_cortex_waiting_queue_2(Task *thread) 
{
	if (head_of_waiting_queue == 0)
	{
		head_of_waiting_queue = thread;
		return;
	}
	/*if waiting queue is not empty.*/
	Task *waiting_current = head_of_waiting_queue;
	while (waiting_current->cortex_waiting_queue_next2 != 0)
	{
		waiting_current = waiting_current->cortex_waiting_queue_next2;
	}
	waiting_current->cortex_waiting_queue_next2 = thread;
}
void CPU::add_thread_to_cortex_waiting_queue_3(Task *thread) 
{
	if (head_of_waiting_queue == 0)
	{
		head_of_waiting_queue = thread;
		return;
	}
	/*if waiting queue is not empty.*/
	Task *waiting_current = head_of_waiting_queue;
	while (waiting_current->cortex_waiting_queue_next3 != 0)
	{
		waiting_current = waiting_current->cortex_waiting_queue_next3;
	}
	waiting_current->cortex_waiting_queue_next3 = thread;
}
void CPU::add_thread_to_cortex_waiting_queue_4(Task *thread) 
{
	if (head_of_waiting_queue == 0)
	{
		head_of_waiting_queue = thread;
		return;
	}
	/*if waiting queue is not empty.*/
	Task *waiting_current = head_of_waiting_queue;
	while (waiting_current->cortex_waiting_queue_next4 != 0)
	{
		waiting_current = waiting_current->cortex_waiting_queue_next4;
	}
	waiting_current->cortex_waiting_queue_next4 = thread;
}
void CPU::add_thread_to_denver_waiting_queue(Task *thread) 
{
	if (head_of_waiting_queue == 0)
	{
		head_of_waiting_queue = thread;
		return;
	}
	/*if waiting queue is not empty.*/
	Task *waiting_current = head_of_waiting_queue;
	while (waiting_current->denver_waiting_queue_next != 0)
	{
		waiting_current = waiting_current->denver_waiting_queue_next;
	}
	waiting_current->denver_waiting_queue_next = thread;
}
void CPU::add_thread_to_denver_waiting_queue_2(Task *thread) 
{
	if (head_of_waiting_queue == 0)
	{
		head_of_waiting_queue = thread;
		return;
	}
	/*if waiting queue is not empty.*/
	Task *waiting_current = head_of_waiting_queue;
	while (waiting_current->denver_waiting_queue_next2 != 0)
	{
		waiting_current = waiting_current->denver_waiting_queue_next2;
	}
	waiting_current->denver_waiting_queue_next2 = thread;
}
void CPU::add_thread_to_ready_queue(Task *thread) 
{
	cout << "Add thread to ready queue, which model is: " << thread->model_id << ", task id is: " << thread->task_id << endl;
	if (head_of_ready_queue == 0)
	{
		head_of_ready_queue = thread;
		return;
	}
	/*if waiting queue is not empty.*/
	Task *ready_current = head_of_ready_queue;
	while (ready_current->cortext_ready_queue_next != 0)
	{
		ready_current = ready_current->cortext_ready_queue_next;
	}
	Task *current = thread;
	ready_current->cortext_ready_queue_next = current;
	ready_current = ready_current->cortext_ready_queue_next;
}
void CPU::execute_thread_in_waiting_queue(Task *thread,int time)
{
	cout << "cpu waiting excuted time: "<< thread->cpu_waiting[0]<<" remain time: "<< thread->cpu_waiting[1] << ", which model is: " << thread->model_id << ", task id: " << thread->task_id << endl;
	thread->cpu_waiting[1] = thread->cpu_waiting[1] - time;
}
void CPU::execute_thread_in_ready_queue(Task *thread,int time) 
{
	cout << "cpu ready excuted time: " << thread->cpu_ready[0] << " remain time: " << thread->cpu_ready[1] << ", which model is: " << thread->model_id << ", task id: " << thread->task_id << endl;
	thread->cpu_ready[1] = thread->cpu_ready[1] - time;
}
void CPU::remove_thread_from_waiting_queue(Task *thread) 
{
	cout << "Remove thread from waiting queue, which model is: " << thread->model_id << ", task id: " << thread->task_id << endl;
	/*if (thread->cortex_waiting_queue_next != 0)
	{
		head_of_waiting_queue= thread->cortex_waiting_queue_next;
		thread->cortex_waiting_queue_next=0;
	}
	else
	{
		head_of_waiting_queue = 0;
		thread->cortex_waiting_queue_next = 0;
	}*/
	if (head_of_waiting_queue == thread)
	{
		if (thread->cortex_waiting_queue_next != 0)
			head_of_waiting_queue = thread->cortex_waiting_queue_next;
		else
			head_of_waiting_queue = 0;
		thread->cortex_waiting_queue_next = 0;

	}
	else
	{
		Task *current = head_of_waiting_queue;
		while (current->cortex_waiting_queue_next != thread)
		{
			current = current->cortex_waiting_queue_next;
		}
		current->cortex_waiting_queue_next = thread->cortex_waiting_queue_next;
		thread->cortex_waiting_queue_next = 0;
	}
}
void CPU::remove_thread_from_waiting_queue_2(Task *thread) 
{
	cout << "Remove thread from waiting queue2, which model is: " << thread->model_id << ", task id: " << thread->task_id << endl;	
	if (head_of_waiting_queue == thread)
	{
		if (thread->cortex_waiting_queue_next2 != 0)
			head_of_waiting_queue = thread->cortex_waiting_queue_next2;
		else
			head_of_waiting_queue = 0;
		thread->cortex_waiting_queue_next2 = 0;

	}
	else
	{
		Task *current = head_of_waiting_queue;
		while (current->cortex_waiting_queue_next2 != thread)
		{
			current = current->cortex_waiting_queue_next2;
		}
		current->cortex_waiting_queue_next2 = thread->cortex_waiting_queue_next2;
		thread->cortex_waiting_queue_next2 = 0;
	}
}
void CPU::remove_thread_from_waiting_queue_3(Task *thread) 
{
	cout << "Remove thread from waiting queue3, which model is: " << thread->model_id << ", task id: " << thread->task_id << endl;
	if (head_of_waiting_queue == thread)
	{
		if (thread->cortex_waiting_queue_next3 != 0)
			head_of_waiting_queue = thread->cortex_waiting_queue_next3;
		else
			head_of_waiting_queue = 0;
		thread->cortex_waiting_queue_next3 = 0;

	}
	else
	{
		Task *current = head_of_waiting_queue;
		while (current->cortex_waiting_queue_next3 != thread)
		{
			current = current->cortex_waiting_queue_next3;
		}
		current->cortex_waiting_queue_next3 = thread->cortex_waiting_queue_next3;
		thread->cortex_waiting_queue_next3 = 0;
	}
}
void CPU::remove_thread_from_waiting_queue_4(Task *thread) 
{
	cout << "Remove thread from waiting queue4, which model is: " << thread->model_id << ", task id: " << thread->task_id << endl;
	if (head_of_waiting_queue == thread)
	{
		if (thread->cortex_waiting_queue_next4 != 0)
			head_of_waiting_queue = thread->cortex_waiting_queue_next4;
		else
			head_of_waiting_queue = 0;
		thread->cortex_waiting_queue_next4 = 0;

	}
	else
	{
		Task *current = head_of_waiting_queue;
		while (current->cortex_waiting_queue_next4 != thread)
		{
			current = current->cortex_waiting_queue_next4;
		}
		current->cortex_waiting_queue_next4 = thread->cortex_waiting_queue_next4;
		thread->cortex_waiting_queue_next4 = 0;
	}
}
void CPU::remove_thread_from_denver_waiting_queue(Task *thread) 
{
	cout << "Remove thread from denver waiting queue 5, which model is: " << thread->model_id << ", task id: " << thread->task_id << endl;
	if (head_of_waiting_queue == thread)
	{
		if (thread->denver_waiting_queue_next != 0)
			head_of_waiting_queue = thread->denver_waiting_queue_next;
		else
			head_of_waiting_queue = 0;
		thread->denver_waiting_queue_next = 0;

	}
	else
	{
		Task *current = head_of_waiting_queue;
		while (current->denver_waiting_queue_next != thread)
		{
			current = current->denver_waiting_queue_next;
		}
		current->denver_waiting_queue_next = thread->denver_waiting_queue_next;
		thread->denver_waiting_queue_next = 0;
	}
}
void CPU::remove_thread_from_denver_waiting_queue_2(Task *thread) 
{
	cout << "Remove thread from denver waiting queue 5, which model is: " << thread->model_id << ", task id: " << thread->task_id << endl;
	if (head_of_waiting_queue == thread)
	{
		if (thread->denver_waiting_queue_next2 != 0)
			head_of_waiting_queue = thread->denver_waiting_queue_next2;
		else
			head_of_waiting_queue = 0;
		thread->denver_waiting_queue_next2 = 0;

	}
	else
	{
		Task *current = head_of_waiting_queue;
		while (current->denver_waiting_queue_next2 != thread)
		{
			current = current->denver_waiting_queue_next2;
		}
		current->denver_waiting_queue_next2 = thread->denver_waiting_queue_next2;
		thread->denver_waiting_queue_next2 = 0;
	}
}
void CPU::remove_thread_from_ready_queue(Task *thread) 
{
	if (head_of_ready_queue == thread)
	{
		if (thread->cortext_ready_queue_next != 0)
			head_of_ready_queue = thread->cortext_ready_queue_next;
		else
			head_of_ready_queue = 0;
	}
	else
	{
		Task *current = head_of_ready_queue;
		while (current!=0 && current->cortext_ready_queue_next != thread)
		{
			current = current->cortext_ready_queue_next;
		}
		if(current!=0)
			current->cortext_ready_queue_next = thread->cortext_ready_queue_next;		
	}
	thread->cortext_ready_queue_next = 0;
}
void CPU::travel_waiting_queue()
{
	if (head_of_waiting_queue == 0) {                      // 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_waiting_queue;             // 用pointer *current在list中移動
	while (current != 0) {                 // Traversal
		cout << "cpu waiting execution time " << current->cpu_waiting[0] << ", remained time: " << current->cpu_waiting[1] << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		current = current->cortex_waiting_queue_next;
	}
	cout << endl;
}
void CPU::travel_waiting_queue_2()
{
	if (head_of_waiting_queue == 0) {                      // 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_waiting_queue;             // 用pointer *current在list中移動
	while (current != 0) {                 // Traversal
		cout << "cpu waiting execution time " << current->cpu_waiting[0] << ", remained time: " << current->cpu_waiting[1] << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		current = current->cortex_waiting_queue_next2;
	}
	cout << endl;
}
void CPU::travel_waiting_queue_3()
{
	if (head_of_waiting_queue == 0) {                      // 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_waiting_queue;             // 用pointer *current在list中移動
	while (current != 0) {                 // Traversal
		cout << "cpu waiting execution time " << current->cpu_waiting[0] << ", remained time: " << current->cpu_waiting[1] << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		current = current->cortex_waiting_queue_next3;
	}
	cout << endl;
}
void CPU::travel_waiting_queue_4()
{
	if (head_of_waiting_queue == 0) {                      // 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_waiting_queue;             // 用pointer *current在list中移動
	while (current != 0) {                 // Traversal
		cout << "cpu waiting execution time " << current->cpu_waiting[0] << ", remained time: " << current->cpu_waiting[1] << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		current = current->cortex_waiting_queue_next4;
	}
	cout << endl;
}
void CPU::travel_waiting_queue_5()
{
	if (head_of_waiting_queue == 0) {                      // 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_waiting_queue;             // 用pointer *current在list中移動
	while (current != 0) {                 // Traversal
		cout << "cpu waiting execution time " << current->cpu_waiting[0] << ", remained time: " << current->cpu_waiting[1] << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		current = current->denver_waiting_queue_next;
	}
	cout << endl;
}
void CPU::travel_waiting_queue_6()
{
	if (head_of_waiting_queue == 0) {                      // 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_waiting_queue;             // 用pointer *current在list中移動
	while (current != 0) {                 // Traversal
		cout << "cpu waiting execution time " << current->cpu_waiting[0] << ", remained time: " << current->cpu_waiting[1] << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		current = current->denver_waiting_queue_next2;
	}
	cout << endl;
}
void CPU::travel_ready_queue() 
{
	if (head_of_ready_queue == 0) {                      // 如果first node指向NULL, 表示list沒有資料
		cout << "List is empty.\n";
		return;
	}
	Task *current = head_of_ready_queue;             // 用pointer *current在list中移動
	while (current != 0) {                 // Traversal
		cout << "cpu ready execution time " << current->cpu_ready[0] << ", remained time: " << current->cpu_ready[1] << ", model id: " << current->model_id << ", task id: " << current->task_id << endl;
		current = current->cortext_ready_queue_next;
	}
	cout << endl;
}
/*************************************************Accumulate power***********************************************************/
void CPU::accucmulate_cpu_active_power() 
{
	double activate_power = 1000.00*(4.99*frequency*frequency*frequency - 5.65*frequency*frequency + 3.41*frequency - 0.35);
	energy_consumption = energy_consumption + activate_power;
}
void CPU::accucmulate_cpu_idle_power()
{
	energy_consumption = energy_consumption + power_idle;
}
void CPU::travel_power_consumption() 
{
	cout << "CPU power: "<<energy_consumption<<" mW" << endl;
}
/*************************************************Queue reset***********************************************************/
void CPU::clear() 
{
	head_of_ready_queue = 0;
	head_of_waiting_queue = 0;
	energy_consumption = 0;
}
/*************************************************Queue sorting*********************************************************/

void CPU::sort_ready_queue(Task **headRef)
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->cortext_ready_queue_next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_ready_queue(head, &a, &b);

	/* Recursively sort the sublists */
	sort_ready_queue(&a);
	sort_ready_queue(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_ready_queue(a, b);
}
Task *CPU::mergelist_ready_queue(Task *a, Task *b)
{
	Task *result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->priority <= b->priority) {
		result = a;
		result->cortext_ready_queue_next = mergelist_ready_queue(a->cortext_ready_queue_next, b);
	}
	else {
		result = b;
		result->cortext_ready_queue_next = mergelist_ready_queue(a, b->cortext_ready_queue_next);
	}
	return (result);
}
void CPU::splitList_ready_queue(Task *source, Task **frontRef, Task **backRef)
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->cortext_ready_queue_next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->cortext_ready_queue_next;
		if (fast != NULL) {
			slow = slow->cortext_ready_queue_next;
			fast = fast->cortext_ready_queue_next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->cortext_ready_queue_next;
	slow->cortext_ready_queue_next = NULL;
}
/*確認是否排程正確*/
void CPU::sort_waiting_queue_1(Task **headRef)
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->cortex_waiting_queue_next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_waiting_queue_1(head, &a, &b);

	/* Recursively sort the sublists */
	sort_waiting_queue_1(&a);
	sort_waiting_queue_1(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_waiting_queue_1(a, b);
}
Task *CPU::mergelist_waiting_queue_1(Task *a, Task *b) 
{
	Task *result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->priority <= b->priority) {
		result = a;
		result->cortex_waiting_queue_next = mergelist_waiting_queue_1(a->cortex_waiting_queue_next, b);
	}
	else {
		result = b;
		result->cortex_waiting_queue_next = mergelist_waiting_queue_1(a, b->cortex_waiting_queue_next);
	}
	return (result);
}
void CPU::splitList_waiting_queue_1(Task *source, Task **frontRef, Task **backRef) 
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->cortex_waiting_queue_next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->cortex_waiting_queue_next;
		if (fast != NULL) {
			slow = slow->cortex_waiting_queue_next;
			fast = fast->cortex_waiting_queue_next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->cortex_waiting_queue_next;
	slow->cortex_waiting_queue_next = NULL;
}

void CPU::sort_waiting_queue_2(Task **headRef)
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 2 */
	if ((head == NULL) || (head->cortex_waiting_queue_next2 == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_waiting_queue_2(head, &a, &b);

	/* Recursively sort the sublists */
	sort_waiting_queue_2(&a);
	sort_waiting_queue_2(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_waiting_queue_2(a, b);
}
Task *CPU::mergelist_waiting_queue_2(Task *a, Task *b)
{
	Task *result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->priority <= b->priority) {
		result = a;
		result->cortex_waiting_queue_next2 = mergelist_waiting_queue_2(a->cortex_waiting_queue_next2, b);
	}
	else {
		result = b;
		result->cortex_waiting_queue_next2 = mergelist_waiting_queue_2(a, b->cortex_waiting_queue_next2);
	}
	return (result);
}
void CPU::splitList_waiting_queue_2(Task *source, Task **frontRef, Task **backRef)
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->cortex_waiting_queue_next2;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->cortex_waiting_queue_next2;
		if (fast != NULL) {
			slow = slow->cortex_waiting_queue_next2;
			fast = fast->cortex_waiting_queue_next2;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->cortex_waiting_queue_next2;
	slow->cortex_waiting_queue_next2 = NULL;
}

void CPU::sort_waiting_queue_3(Task **headRef)
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 3 */
	if ((head == NULL) || (head->cortex_waiting_queue_next3 == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_waiting_queue_3(head, &a, &b);

	/* Recursively sort the sublists */
	sort_waiting_queue_3(&a);
	sort_waiting_queue_3(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_waiting_queue_3(a, b);
}
Task *CPU::mergelist_waiting_queue_3(Task *a, Task *b)
{
	Task *result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->priority <= b->priority) {
		result = a;
		result->cortex_waiting_queue_next3 = mergelist_waiting_queue_3(a->cortex_waiting_queue_next3, b);
	}
	else {
		result = b;
		result->cortex_waiting_queue_next3 = mergelist_waiting_queue_3(a, b->cortex_waiting_queue_next3);
	}
	return (result);
}
void CPU::splitList_waiting_queue_3(Task *source, Task **frontRef, Task **backRef)
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->cortex_waiting_queue_next3;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->cortex_waiting_queue_next3;
		if (fast != NULL) {
			slow = slow->cortex_waiting_queue_next3;
			fast = fast->cortex_waiting_queue_next3;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->cortex_waiting_queue_next3;
	slow->cortex_waiting_queue_next3 = NULL;
}

void CPU::sort_waiting_queue_4(Task **headRef)
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 4 */
	if ((head == NULL) || (head->cortex_waiting_queue_next4 == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_waiting_queue_4(head, &a, &b);

	/* Recursively sort the sublists */
	sort_waiting_queue_4(&a);
	sort_waiting_queue_4(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_waiting_queue_4(a, b);
}
Task *CPU::mergelist_waiting_queue_4(Task *a, Task *b)
{
	Task *result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->priority <= b->priority) {
		result = a;
		result->cortex_waiting_queue_next4 = mergelist_waiting_queue_4(a->cortex_waiting_queue_next4, b);
	}
	else {
		result = b;
		result->cortex_waiting_queue_next4 = mergelist_waiting_queue_4(a, b->cortex_waiting_queue_next4);
	}
	return (result);
}
void CPU::splitList_waiting_queue_4(Task *source, Task **frontRef, Task **backRef)
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->cortex_waiting_queue_next4;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->cortex_waiting_queue_next4;
		if (fast != NULL) {
			slow = slow->cortex_waiting_queue_next4;
			fast = fast->cortex_waiting_queue_next4;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->cortex_waiting_queue_next4;
	slow->cortex_waiting_queue_next4 = NULL;
}

void  CPU::sort_waiting_queue_5(Task **headRef) 
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 4 */
	if ((head == NULL) || (head->denver_waiting_queue_next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_waiting_queue_5(head, &a, &b);

	/* Recursively sort the sublists */
	sort_waiting_queue_5(&a);
	sort_waiting_queue_5(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_waiting_queue_5(a, b);
}
Task * CPU::mergelist_waiting_queue_5(Task *a, Task *b) 
{
	Task *result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->priority <= b->priority) {
		result = a;
		result->denver_waiting_queue_next = mergelist_waiting_queue_5(a->denver_waiting_queue_next, b);
	}
	else {
		result = b;
		result->denver_waiting_queue_next = mergelist_waiting_queue_5(a, b->denver_waiting_queue_next);
	}
	return (result);
}
void  CPU::splitList_waiting_queue_5(Task *source, Task **frontRef, Task **backRef) 
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->denver_waiting_queue_next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->denver_waiting_queue_next;
		if (fast != NULL) {
			slow = slow->denver_waiting_queue_next;
			fast = fast->denver_waiting_queue_next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->denver_waiting_queue_next;
	slow->denver_waiting_queue_next = NULL;
}

void  CPU::sort_waiting_queue_6(Task **headRef) 
{
	Task* head = *headRef;
	Task* a;
	Task* b;

	/* Base case -- length 0 or 4 */
	if ((head == NULL) || (head->denver_waiting_queue_next2 == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	splitList_waiting_queue_6(head, &a, &b);

	/* Recursively sort the sublists */
	sort_waiting_queue_6(&a);
	sort_waiting_queue_6(&b);

	/* answer = merge the two sorted lists together */
	*headRef = mergelist_waiting_queue_6(a, b);
}
Task * CPU::mergelist_waiting_queue_6(Task *a, Task *b) 
{
	Task *result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->priority <= b->priority) {
		result = a;
		result->denver_waiting_queue_next2 = mergelist_waiting_queue_6(a->denver_waiting_queue_next2, b);
	}
	else {
		result = b;
		result->denver_waiting_queue_next2 = mergelist_waiting_queue_6(a, b->denver_waiting_queue_next2);
	}
	return (result);
}
void  CPU::splitList_waiting_queue_6(Task *source, Task **frontRef, Task **backRef) 
{
	Task* fast;
	Task* slow;
	slow = source;
	fast = source->denver_waiting_queue_next2;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->denver_waiting_queue_next2;
		if (fast != NULL) {
			slow = slow->denver_waiting_queue_next2;
			fast = fast->denver_waiting_queue_next2;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->denver_waiting_queue_next2;
	slow->denver_waiting_queue_next2 = NULL;
}
/***********************************************************************************************************************/