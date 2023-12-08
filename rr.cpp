#include <iostream>
#include <vector>
#include <queue>

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
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    //start the simulation
    simulator::run_rr(quantum, processes);

    //exit main
    return EXIT_SUCCESS;
}