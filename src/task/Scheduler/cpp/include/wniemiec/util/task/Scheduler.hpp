#pragma once

#include <functional>
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
        static std::map<long, bool> intervalRoutines;
        static std::map<long, bool> timeoutRoutine;
        static long currentRoutineId;
        static std::function<void(void)>& currentRoutine;
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
        static long set_

        /**
         * Repeatedly calls a routine with a fixed time delay between each
         * call.
         *
         * @param       routine Routine to be performed
         * @param       interval Interval that the routine will be invoked (in milliseconds)
         *
         * @return      Routine id, being useful for future cancellation
         *
         * @see         Scheduler::clear_interval
         */
        static long set_interval(const std::function<void(void)>& routine, long interval);

        /**
         * Cancels a timed, repeating action, which was previously established by a
         * call to {@link Scheduler::set_interval}.
         *
         * @param       id Routine identifier
         */
        static void clear_interval(long id);

        /**
         * Runs a routine within a timeout.
         *
         * @param       routine Routine
         * @param       timeout Maximum execution time (in milliseconds)
         *
         * @return      True if the routine has not finished executing within the time
         * limit; false otherwise
         */
        static bool set_timeout_to_routine(const std::function<void(void)>& routine, long timeout);
    private:
        static void run_routine(const std::function<void(void)>& routine);
        static void initialize_routine_id();
        static time_t get_current_time();
        static void* interval_control_routine(void* args);
        static void* control_routine(void* args);
        static void wait_routine_for(long time);
        static double time_elapsed_in_milliseconds(time_t start);
        static bool has_routine_finished();
        static void finish_routine();
    };
}
