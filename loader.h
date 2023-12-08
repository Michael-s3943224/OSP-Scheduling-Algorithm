#ifndef LOADER_H
#define LOADER_H

#define DELIMITER ','

#define PROCESS_ID_INDEX 0
#define BURST_T_INDEX 1
#define NUM_PBT_FIELDS 2

#define MIN_PROCESS_ID 1
#define MIN_BURST_T 0

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include "pcb.h"

class loader
{
    private:
        static std::vector<std::string> split_string(const std::string& line, char delimiter);
        static pcb try_parse_line(const std::string& line, unsigned int lineNumber);
    public:
        static std::vector<pcb> load_csv(const std::string& fileName);
};
#endif