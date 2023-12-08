#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <iomanip>
#include <numeric>
#include <string>
#include <string.h>
#include <algorithm>
#include <deque>
#include "pcb.h"
#include "types.h"

#define TABLE_VERT_SEP '|'
#define TABLE_HORZ_SEP '-'

#define PROCESS_ID_HEADER " Process ID "
#define BURST_TIME_HEADER " Burst Time "
#define WAIT_TIME_HEADER " Wait Time "
#define TURNAROUND_TIME_HEADER " Turnaround Time "
#define RESPONSE_TIME_HEADER " Response Time "
#define NUM_TABLE_COL 5

#define AVG_WAIT_TIME_TITLE "Avg. waiting time = "
#define AVG_TURNAROUND_TIME_TITLE "Avg. turnaround time = "
#define AVG_RESPONSE_TIME_TITLE "Avg. response time = " 

class simulator
{
    private:
        static void print_results(const std::vector<pcb>& processes);
    public:
        // implemented both vector and queue cpu scheduling algorithms

        // make sure we're copying the original pcb vector so we don't modify the original
        static void run_fifo(std::vector<pcb> processes);
        static void run_sjf(std::vector<pcb> processes);
        static void run_rr(osp2023::time_type quantum, std::vector<pcb> processes);

        // now the same implementations but using a queue instead
        // still passing a vector, so i can still use the same print function
        // and can still sort
        static void run_fifo_queue(std::vector<pcb> processes);
        static void run_sjf_queue(std::vector<pcb> processes);
        static void run_rr_queue(osp2023::time_type quantum, std::vector<pcb> processes);
};

#endif