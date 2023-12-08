#include "pcb.h"

// constructor
pcb::pcb(osp2023::id_type id, osp2023::time_type burst_time):
    id(id), burst_time(burst_time), 
    time_used(osp2023::time_zero),
    wait_time(osp2023::time_not_set), 
    turnaround_time(osp2023::time_not_set),
    response_time(osp2023::time_not_set) {};

// copy constructor for std::sort
pcb::pcb(const pcb& p) {
    *this = p;
};

// printing for debugging
std::string pcb::to_string() const {
    return "(id: " + std::to_string(id)  + ", total_time: " + std::to_string(burst_time) + ")";
}

std::ostream& operator<<(std::ostream& os, const pcb& p) {
    os << p.to_string();
    return os;
}

// assignment operator for std::sort
pcb& pcb::operator=(const pcb& other) {

    if (this == &other) {
        return *this;
    }

    id = other.id;
    burst_time = other.burst_time;
    time_used = other.time_used;
    wait_time = other.wait_time;
    response_time = other.response_time;
    turnaround_time = other.turnaround_time;

    return *this;
}

// getters
osp2023::id_type pcb::getId() const {
    return id;
}

osp2023::time_type pcb::getBurstTime() const {
    return burst_time;
}

osp2023::time_type pcb::getWaitTime() const {
    return wait_time;
}

osp2023::time_type pcb::getTimeUsed() const {
    return time_used;
}

osp2023::time_type pcb::getTurnaroundTime() const {
    return turnaround_time;
}

osp2023::time_type pcb::getResponseTime() const {
    return response_time;
}

// setters 
void pcb::setWaitTime(osp2023::time_type newWaitTime) {
    wait_time = newWaitTime;
}

void pcb::setTimeUsed(osp2023::time_type newTimeUsed) {
    time_used = newTimeUsed;
}

void pcb::setResponseTime(osp2023::time_type newResponseTime) {
    response_time = newResponseTime;
}

void pcb::setTurnaroundTime(osp2023::time_type newTurnaroundTime) {
    turnaround_time = newTurnaroundTime;
}