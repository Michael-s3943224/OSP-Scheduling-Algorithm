#include <iostream>
#include <vector>

#include "loader.h"
#include "pcb.h"
#include "types.h"
#include "simulator.h"

#define QUANTUM_INDEX 1
#define FILE_NAME_INDEX 2
#define NUM_CMD_ARGS 3

int main(int argc, char** argv)
{
    // check if the number of cmd arguments is correct
    // if it doesn't exit the program
    if (argc != NUM_CMD_ARGS) {
        std::string error = "ERROR : incorrect numnber of args : should be ./rr <quantum> <datafile>";
        std::cout << error << std::endl;
        return EXIT_FAILURE;
    }

    // get the quantum string from cmd args
    std::string quantumStr = argv[QUANTUM_INDEX];

    // try to parse the quantum string
    osp2023::time_type quantum = osp2023::time_not_set;
    try {
        quantum = std::stoll(quantumStr);
    } catch (const std::exception& e) {
        std::string error = "ERROR : unable to parse quantum cmd arg";
        std::cout << error << std::endl;
        return EXIT_FAILURE;
    }

    // check if quantum is not negative
    if (quantum < pcb::MIN_DURATION || quantum > pcb::MAX_DURATION) {
        std::string error 
            = "ERROR : quantum must be between " + 
            std::to_string(pcb::MIN_DURATION) + " and " + std::to_string(pcb::MAX_DURATION);
        std::cout << error << std::endl;
        return EXIT_FAILURE;
    }

    // get the file name from cmd args
    std::string fileName = argv[FILE_NAME_INDEX];

    //try to load the processes CSV file
    std::vector<pcb> processes;
    try{
        processes = loader::load_csv(fileName);
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << " ----==== FIFO ====----" << std::endl;
    simulator::run_fifo(processes);
    std::cout << std::endl;

    std::cout << "----==== FIFO QUEUE ====----" << std::endl;
    simulator::run_fifo_queue(processes);
    std::cout << std::endl;

    std::cout << "----==== SJF ====----" << std::endl;
    simulator::run_sjf(processes);
    std::cout << std::endl;

    std::cout << "----==== SJF QUEUE ====----" << std::endl;
    simulator::run_sjf_queue(processes);
    std::cout << std::endl;

    std::cout << "----==== RR ====----" << std::endl;
    simulator::run_rr(quantum, processes);
    std::cout << std::endl;

    std::cout << "----==== RR QUEUE ====----" << std::endl;
    simulator::run_rr_queue(quantum, processes);
    std::cout << std::endl;

    //exit main
    return EXIT_SUCCESS;
}