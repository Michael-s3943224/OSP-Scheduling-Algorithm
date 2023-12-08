#include "simulator.h"

void simulator::print_results(const std::vector<pcb>& processes)
{
    // calculate the total wait time of all processes
    osp2023::time_type totalWait = std::accumulate(
        processes.begin(), processes.end(), osp2023::time_zero,
        [] (osp2023::time_type result, const pcb& process) { 
            return result + process.getWaitTime(); 
        }
    );

    // calculate average wait time of all processes
    double avgWait = (processes.empty()) ? 0.0 : totalWait / (double)processes.size();

    // calculate the total turnaround time of all processes
    osp2023::time_type totalTurnaround = std::accumulate(
        processes.begin(), processes.end(), osp2023::time_zero,
        [] (osp2023::time_type result, const pcb& process) { 
            return result + process.getTurnaroundTime(); 
        } 
    );
    // calculate average turnaround time of all processes
    double avgTurnaround = (processes.empty()) ? 0.0 : totalTurnaround / (double)processes.size();

    // calculate the total response time of all processes
    osp2023::time_type totalResponse = std::accumulate(
        processes.begin(), processes.end(), osp2023::time_zero,
        [] (osp2023::time_type result, const pcb& process) { 
            return result + process.getResponseTime(); 
        } 
    );
    // calculate average response time of all processes
    double avgResponse = (processes.empty()) ? 0.0 : totalResponse / (double)processes.size();

    // print out the header of the tables
    std::cout << 
        PROCESS_ID_HEADER << TABLE_VERT_SEP <<
        BURST_TIME_HEADER << TABLE_VERT_SEP <<
        WAIT_TIME_HEADER << TABLE_VERT_SEP <<
        TURNAROUND_TIME_HEADER << TABLE_VERT_SEP <<
        RESPONSE_TIME_HEADER <<
        std::endl;
    
    // get the length of table header
    int divisions = NUM_TABLE_COL - 1;
    std::size_t processIdColLen = strlen(PROCESS_ID_HEADER);
    std::size_t burstTimeColLen = strlen(BURST_TIME_HEADER);
    std::size_t waitTimeColLen = strlen(WAIT_TIME_HEADER);
    std::size_t turnAroundTimeColLen = strlen(TURNAROUND_TIME_HEADER);
    std::size_t responseTimeColLen = strlen(RESPONSE_TIME_HEADER);

    // calculate the length of a row
    std::size_t rowLen = 
        processIdColLen + burstTimeColLen + waitTimeColLen + 
        turnAroundTimeColLen + responseTimeColLen + divisions;

    // print out the horizontal line below the header
    std::cout << std::string(rowLen, TABLE_HORZ_SEP) << std::endl;

    // print out each table row
    for (const pcb& pcb : processes) {
        std::cout << 
            std::setw(processIdColLen) << std::left << pcb.getId() << TABLE_VERT_SEP <<
            std::setw(burstTimeColLen) << std::left << pcb.getBurstTime() << TABLE_VERT_SEP <<
            std::setw(waitTimeColLen) << std::left << pcb.getWaitTime() << TABLE_VERT_SEP <<
            std::setw(turnAroundTimeColLen) << std::left << pcb.getTurnaroundTime() << TABLE_VERT_SEP <<
            std::setw(responseTimeColLen) << std::left << pcb.getResponseTime() <<
        std::endl;
    }

    // print out message for no processes
    if (processes.empty()) {
        char padChar = ' ';
        std::string noProcessesMsg = "NO PROCESSES ADDED :C";
        std::size_t noProcessLeftPad = (rowLen - noProcessesMsg.length()) / 2;
        std::cout << std::string(noProcessLeftPad, padChar) << noProcessesMsg << std::endl; 
    }
    
    // print out the average times
    std::cout << std::endl;
    std::cout << AVG_WAIT_TIME_TITLE << avgWait << std::endl;
    std::cout << AVG_TURNAROUND_TIME_TITLE << avgTurnaround << std::endl;
    std::cout << AVG_RESPONSE_TIME_TITLE << avgResponse << std::endl;
}

void simulator::run_fifo(std::vector<pcb> processes)
{
    // --- calculate times --- 
    if (!processes.empty()) {
        // set the first process's burst, turnaround and response times
        pcb& firstProcess = processes.front();
        firstProcess.setWaitTime(osp2023::time_zero);
        firstProcess.setResponseTime(osp2023::time_zero);
        osp2023::time_type firstTurnaround = firstProcess.getBurstTime();
        firstProcess.setTurnaroundTime(firstTurnaround);
    }
    
    // calculate the times for the rest of the processes
    std::size_t firstIndexWithWait = 1;
    for (std::size_t i = firstIndexWithWait; i < processes.size(); ++i) {
        // get current process
        pcb& currentProcess = processes.at(i);
        // get prev process
        std::size_t prevIndex = i - 1;
        pcb& prevProcess = processes.at(prevIndex);

        // waitTime = turnaroundTime - burstTime
        // turnaroundTime = completionTime - arrivalTime
        // responseTime = firstTime - arrivalTime

        // completionTime = waitTime + burstTime 
        // because arrival time is all 0 and therefore the CPU is not idling

        // because the CPU does not idle and this is fifo
        // the completion time for a previous process is the waiting for the current process
        // therefore waitTimeCurrent = waitTimePrevious + burstTimePrevious
        currentProcess.setWaitTime(
            prevProcess.getBurstTime() + prevProcess.getWaitTime()
        );

        // completionTime = waitTime + burstTime
        // since turnaroundTime = completionTime because arrivalTime = 0
        // turnaroundTimeCurrent = waitTimeCurrent + burstTimeCurrent
        currentProcess.setTurnaroundTime(
            currentProcess.getBurstTime() + currentProcess.getWaitTime()
        );

        // responseTime = firstTime - arrivalTime
        // because the CPU does not idle and this is fifo
        // firstTime = waitTime
        // therefore responseTimeCurrent = waitTimeCurrent
        // because arrivalTime = 0
        currentProcess.setResponseTime(
            currentProcess.getWaitTime()
        );
    }

    // --- print the results ---
    // response time is the same as turn around time
    print_results(processes);
}

void simulator::run_sjf(std::vector<pcb> processes)
{
    // --- sort the processes by burst times (and then the order they appear in) ---
    // keep in mind we are sorting a copy of the pcb vector
    std::stable_sort(processes.begin(), processes.end(), 
        [](const pcb& left, const pcb& right) {
            return left.getBurstTime() < right.getBurstTime();
        }
    );

    // the rest is the the same as fifo
    // so I'm just running fifo for the sorted processes
    run_fifo(processes);
}

void simulator::run_rr(osp2023::time_type quantum, std::vector<pcb> processes)
{
    // check if quantum is not negative
    if (quantum < pcb::MIN_DURATION || quantum > pcb::MAX_DURATION) {
        std::string error 
            = "ERROR : simulator : run rr quantum must be between " + 
            std::to_string(pcb::MIN_DURATION) + " and " + std::to_string(pcb::MAX_DURATION);
            throw std::runtime_error(error);
    }

    bool finished = false;
    osp2023::time_type curr_completion = osp2023::time_zero;

    while (!finished)
    {
        bool allExecuted = true;

        for (pcb& process: processes) {
            
            // check if the process still needs to be executed
            if (process.getTimeUsed() < process.getBurstTime()) {
                
                // if we reached here then there are processes that are not finished
                allExecuted = false;

                // set the time elapsed
                osp2023::time_type timeElapsed = osp2023::time_zero;
                if (process.getTimeUsed() + quantum <= process.getBurstTime()) {
                    // if adding the quantum doesn't end the process or just barely
                    // then the elapsed time will be the quantum time
                    timeElapsed = quantum;
                } else {
                    // if adding the quantum ends the process (and not barely)
                    // then the elapsed time will the difference between burst time and used time
                    timeElapsed = process.getBurstTime() - process.getTimeUsed();
                }

                // check if this the first time the process is being executed
                if (process.getTimeUsed() == osp2023::time_zero) {
                    // responseTime = firstTime - arrivalTime
                    // since arrivalTime = 0
                    // then responseTime = firstTime
                    process.setResponseTime(
                        curr_completion
                    );
                }

                // elapse the time
                curr_completion += timeElapsed;
                // update the time used for the process
                process.setTimeUsed(process.getTimeUsed() + timeElapsed);

                // check if the process is finished
                if (process.getTimeUsed() == process.getBurstTime()) {

                    // turnaroundTime = completionTime - arrivalTime
                    // since arrivalTime = 0
                    // then turnaroundTime = completionTime 
                    process.setTurnaroundTime(
                        curr_completion
                    );

                    // waitTime = turnaroundTime - burstTime
                    process.setWaitTime(
                        process.getTurnaroundTime() - process.getBurstTime()
                    );
                }
            }
        }

        // if all proccesses are finished executing, then we are finished
        if (allExecuted) {
            finished = true;
        }
    }

    // print the results
    print_results(processes);
}

void simulator::run_fifo_queue(std::vector<pcb> processes) {
    // insert vector of processes into queue
    // since we want to modify it, its references of pcb
    std::deque<pcb*> readyQueue;
    for (pcb& process: processes) {
        readyQueue.push_back(&process);
    }

    osp2023::time_type firstTurnaround;
    if (!readyQueue.empty()) {
        // set the first process's burst, turnaround and response times
        pcb* firstProccess = readyQueue.front();
        firstProccess->setWaitTime(osp2023::time_zero);
        firstProccess->setResponseTime(osp2023::time_zero);
        firstTurnaround = firstProccess->getBurstTime();
        firstProccess->setTurnaroundTime(firstTurnaround);
        readyQueue.pop_front();
    }

    // set the rest of the processes' burst, turnaround and response times
    osp2023::time_type curr_completion = firstTurnaround;
    while (!readyQueue.empty()) {
        // get the current process
        pcb* currentProcess = readyQueue.front();
        // calculate the times (and increment the current completion time)
        osp2023::time_type wait_time = curr_completion;
        osp2023::time_type response_time = curr_completion;
        curr_completion += currentProcess->getBurstTime();
        osp2023::time_type turnaround_time = curr_completion;
        // set the times
        currentProcess->setWaitTime(wait_time);
        currentProcess->setResponseTime(response_time);
        currentProcess->setTurnaroundTime(turnaround_time);
        // pop the current element
        readyQueue.pop_front();
    }

    // print the results
    print_results(processes);
}

void simulator::run_sjf_queue(std::vector<pcb> processes)
{
    // pretty much the same as run_sjf
    // but we are calling run_fifo_queue instead of run_fifo at the end

    // --- sort the processes by burst times (and then the order they appear in) ---
    // keep in mind we are sorting a copy of the pcb vector
    std::stable_sort(processes.begin(), processes.end(), 
        [](const pcb& left, const pcb& right) {
            return left.getBurstTime() < right.getBurstTime();
        }
    );

    // the rest is the the same as fifo
    // so I'm just running fifo for the sorted processes
    run_fifo_queue(processes);
}

void simulator::run_rr_queue(osp2023::time_type quantum, std::vector<pcb> processes)
{
    // check if quantum is not negative
    if (quantum < pcb::MIN_DURATION || quantum > pcb::MAX_DURATION) {
        std::string error 
            = "ERROR : simulator : run rr queue quantum must be between " + 
            std::to_string(pcb::MIN_DURATION) + " and " + std::to_string(pcb::MAX_DURATION);
            throw std::runtime_error(error);
    }

    // insert vector of processes into queue
    // since we want to modify it, its references of pcb
    std::deque<pcb*> readyQueue;
    for (pcb& process: processes) {
        readyQueue.push_back(&process);
    }

    osp2023::time_type curr_completion = osp2023::time_zero;
    while (!readyQueue.empty()) {
        // get the current process in the queue
        pcb* currentProcess = readyQueue.front();
        // pop the process off the queue
        readyQueue.pop_front();

        // set the time elapsed
        osp2023::time_type timeElapsed = osp2023::time_zero;
        if (currentProcess->getTimeUsed() + quantum <= currentProcess->getBurstTime()) {
            // if adding the quantum doesn't end the process or just barely
            // then the elapsed time will be the quantum time
            timeElapsed = quantum;
        } else {
            // if adding the quantum ends the process (and not barely)
            // then the elapsed time will the difference between burst time and used time
            timeElapsed = currentProcess->getBurstTime() - currentProcess->getTimeUsed();
        }

        // check if this the first time the process is being executed
        if (currentProcess->getTimeUsed() == osp2023::time_zero) {
            // responseTime = firstTime - arrivalTime
            // since arrivalTime = 0
            // then responseTime = firstTime
            currentProcess->setResponseTime(
                curr_completion
            );
        }

        // elapse the time
        curr_completion += timeElapsed;
        // update the time used for the process
        currentProcess->setTimeUsed(currentProcess->getTimeUsed() + timeElapsed);

        // check if the process is finished
        if (currentProcess->getTimeUsed() == currentProcess->getBurstTime()) {

            // turnaroundTime = completionTime - arrivalTime
            // since arrivalTime = 0
            // then turnaroundTime = completionTime 
            currentProcess->setTurnaroundTime(
                curr_completion
            );

            // waitTime = turnaroundTime - burstTime
            currentProcess->setWaitTime(
                currentProcess->getTurnaroundTime() - currentProcess->getBurstTime()
            );
        } 
        else
        {
            // if the process is not finished yet
            // then we want to add it back to the end of the queue
            readyQueue.push_back(currentProcess);
        }
    }

    // print results
    print_results(processes);
}