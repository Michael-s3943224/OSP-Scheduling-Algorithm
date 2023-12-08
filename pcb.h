#include <iostream>
#include "types.h"

#pragma once  // tells the compiler to only include this file once
              /**
 * the process control block - represents a process that needs to be executed in
 * our simulator. Please note the I used some custom types of osp2023::time_type
 * and osp2023::id_type. these are just aliases to long integers but they are
 * more meaningful type names, hinting at how they will be used. Please see
 * their definitions in types.h.
 **/
class pcb {

    private:
        // the unique process id
        osp2023::id_type id;
        // the total time that a process / job should run for
        // CHANGE: renamed from total_time to burst_time
        osp2023::time_type burst_time;
        // the time that has been used so far for this process
        osp2023::time_type time_used;
        // the priority level for this process
        // how much time has this process spent waiting for the cpu?
        // CHANGE: renamed from total_wait_time to wait_time
        osp2023::time_type wait_time;
        // ADDED: turnaround time
        osp2023::time_type turnaround_time;
        // ADDED: response time
        osp2023::time_type response_time;

        // what time was this process last on the cpu?

   public:
        // max and min duration for a process in our system.
        static constexpr osp2023::time_type MAX_DURATION = 1000;
        static constexpr osp2023::time_type MIN_DURATION = 10;

        // constructor
        pcb(osp2023::id_type id, osp2023::time_type burst_time);

        // copy constructor for std::sort to work
        pcb(const pcb& p);
        // assignment operator for std::sort to work
        pcb& operator=(const pcb& other);

        // convert to string
        std::string to_string() const;
        // print out pcb
        friend std::ostream& operator<<(std::ostream& os, const pcb& p);

        // getters
        osp2023::id_type getId() const;
        osp2023::time_type getBurstTime() const;
        osp2023::time_type getWaitTime() const;
        osp2023::time_type getTimeUsed() const;
        osp2023::time_type getResponseTime() const;
        osp2023::time_type getTurnaroundTime() const;

        // setters
        void setWaitTime(osp2023::time_type newWaitTime);
        void setTimeUsed(osp2023::time_type newTimeUsed);
        void setResponseTime(osp2023::time_type newResponseTime);
        void setTurnaroundTime(osp2023::time_type newTurnaroundTime);
};
