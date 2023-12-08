#include <iostream>
#include <vector>

#include "loader.h"
#include "pcb.h"
#include "types.h"
#include "simulator.h"

#define NUM_CMD_ARGS 2
#define FILE_NAME_INDEX 1

int main(int argc, char** argv)
{
    // check if the number of cmd arguments is correct
    // if it doesn't exit the program
    if (argc != NUM_CMD_ARGS) {
        std::string error = "ERROR : incorrect numnber of args : should be ./fifo <datafile>";
        std::cout << error << std::endl;
        return EXIT_FAILURE;
    }

    // get the file name from cmd args
    std::string fileName = argv[FILE_NAME_INDEX];
    
    std::vector<pcb> processes;
    try{
        // try to load the processes CSV file
        processes = loader::load_csv(fileName);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // start the simulation
    simulator::run_fifo(processes);
    
    //exit main
    return EXIT_SUCCESS;
}