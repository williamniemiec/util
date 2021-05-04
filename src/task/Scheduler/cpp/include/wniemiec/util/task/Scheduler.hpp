#pragma once
#include <pthread.h>
#include <map>

namespace wniemiec::util::task
{
    /// <summary>
    ///     Responsible for executing routines within time intervals.
    /// </summary
    class Scheduler
    {
    //-------------------------------------------------------------------------
    //		Attributes
    //-------------------------------------------------------------------------
    private:
        static std::map<int, time_t> timeoutRoutine;
        static time_t currentRoutineId;
        static void (*currentRoutine)();


    //-------------------------------------------------------------------------
    //		Constructor
    //-------------------------------------------------------------------------
    private:
        Scheduler();


    //-------------------------------------------------------------------------
    //		Methods
    //-------------------------------------------------------------------------
    public:
        /// <summary>
        ///     Runs a routine within a timeout.
        /// </summary>
        ///
        /// <param name="function">Routine</param>
        /// <param name="timeout">Maximum execution time (in milliseconds)</param>
        static bool set_timeout_to_routine(void (*routine)(), double timeout);
    private:
        static void* control_routine(void* args);
        static void wait_for(double time);
        static time_t get_current_time();
        static pthread_t run_routine(void (*routine)(), time_t id);
        static void finish_routine(pthread_t routineThread);
    };
}
