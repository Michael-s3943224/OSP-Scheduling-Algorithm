#include "loader.h"

std::vector<std::string> loader::split_string(const std::string& line, char delimiter)
{
    // store the split strings by delimiter
    std::vector<std::string> result;

    // put the line into a stream
    std::stringstream stream(line);

    // read each cell and add it to to the result list
    std::string cell;
    while (std::getline(stream, cell, delimiter)) {
        result.push_back(cell);
    }

    // return the result
    return result;
}

pcb loader::try_parse_line(const std::string& line, unsigned int lineNumber)
{
    // split the line into cells
    std::vector<std::string> cells = split_string(line, DELIMITER);

    // check if we have the correct number of cells
    if (cells.size() != NUM_PBT_FIELDS) {
        std::string error = 
            "ERROR : loader : incorrect number of values : at line " + std::to_string(lineNumber);
        throw std::runtime_error(error);
    }

    // grab the individual cells
    std::string processIDStr = cells.at(PROCESS_ID_INDEX);
    std::string burstTimeStr = cells.at(BURST_T_INDEX);

    // store the values that have been parsed
    osp2023::id_type processId = 0;
    osp2023::time_type burstTime = osp2023::time_not_set;
    
    // try parse the processId
    try {
        processId = std::stoi(processIDStr);
    } catch (const std::exception& e) {
        std::string error = 
            "ERROR : loader : could not parse processId : at line " + std::to_string(lineNumber);
        throw std::runtime_error(error);
    }

    // check if process id is not less than 1
    if (processId < MIN_PROCESS_ID) {
        std::string error = 
            "ERROR : loader : processId cannot be less than " + 
            std::to_string(MIN_PROCESS_ID) + " : at line " + std::to_string(lineNumber);

        throw std::runtime_error(error);
    }

    // try parse the burstTime
    try {
        burstTime = std::stoul(burstTimeStr);
    } catch (const std::exception& e) {
        std::string error 
            = "ERROR : loader : could not parse burstTime : at line " + std::to_string(lineNumber);
        throw std::runtime_error(error);
    }

    // check if burstTime is not less than 0
    if (burstTime < MIN_BURST_T) {
        std::string error = 
            "ERROR: loader : burstTime cannot be less than " +
            std::to_string(MIN_BURST_T) + " : at line " + std::to_string(lineNumber);
        throw std::runtime_error(error);
    }
    
    // instantiate the pair object
    pcb result(processId, burstTime);

    // return the pair object
    return result;
}

std::vector<pcb> loader::load_csv(const std::string& fileName)
{
    // store the parsed CSV results
    std::vector<pcb> result;

    // open the file
    std::ifstream file(fileName, std::ifstream::in);

    // check if the file does not exist
    // if it doesn't, throw an error
    if (!file) {
        std::string error = "ERROR : loader : file \"" + fileName + "\" does not exist";
        throw std::runtime_error(error);
    }

    // read each line and try to parse it
    // then if successful, add it to the result list
    std::string line;
    unsigned int lineNumber = 1;
    while (std::getline(file, line))
    {
        pcb row = try_parse_line(line, lineNumber);
        result.push_back(row);
        ++lineNumber;
    }

    // don't forget to close the file
    file.close();

    // return the result
    return result;
}   

