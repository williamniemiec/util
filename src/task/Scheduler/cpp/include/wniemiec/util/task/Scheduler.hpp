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
        static std::map<time_t, bool> timeoutRoutine;
        static time_t currentRoutineId;
        static void (*currentRoutine)();
        static pthread_t controlThread;


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
        ///
        /// <returns>
        ///     True if the routine has not finished executing within the time
        ///     limit; false otherwise
        /// </returns>
        static bool set_timeout_to_routine(void (*routine)(), int timeout);
    private:
        static void run_routine(void (*routine)(), time_t id);
        static void initialize_routine_id();
        static time_t get_current_time();
        static void* control_routine(void* args);
        static void wait_routine_for(int time);
        static double time_elapsed_in_milliseconds(time_t start);
        static bool has_routine_finished();
        static void finish_routine(pthread_t routineThread);
    };
}
