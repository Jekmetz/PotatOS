#include "pcb_queue.h"
#include "pcb_constants.h"

queue_t* ready_queue, blocked_queue, suspended_r_queue, suspended_b_queue;


void init_queue(){
	ready_queue = construct_queue();
	blocked_queue = construct_queue();
	suspended_r_queue = construct_queue();
	suspended_b_queue = construct_queue();
}