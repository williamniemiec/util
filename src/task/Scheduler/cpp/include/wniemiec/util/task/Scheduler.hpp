#pragma once
#include <pthread.h>
#include <map>

namespace wniemiec::util::task
{
    /**
     * Responsible for executing routines within time intervals.
     */
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
        /**
         * Runs a routine within a timeout.
         *
         * @param       routine Routine
         * @param       timeout Maximum execution time (in milliseconds)
         *
         * @return True if the routine has not finished executing within the time
         * limit; false otherwise
         */
        static bool set_timeout_to_routine(void (*routine)(), long timeout);
    private:
        static void run_routine(void (*routine)());
        static void initialize_routine_id();
        static time_t get_current_time();
        static void* control_routine(void* args);
        static void wait_routine_for(long time);
        static double time_elapsed_in_milliseconds(time_t start);
        static bool has_routine_finished();
        static void finish_routine();
    };
}
