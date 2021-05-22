#include "../../../../include/wniemiec/util/task/Scheduler.hpp"

#include <ctime>
#ifdef LINUX
#   include <unistd.h>
#endif
#ifdef WINDOWS
#   include <windows.h>
#endif

using namespace wniemiec::util::task;

std::function<void(void)> null_routine = [](){};

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
std::map<long, bool> Scheduler::delayRoutines = std::map<long, bool>();
std::map<long, bool> Scheduler::intervalRoutines = std::map<long, bool>();
std::map<long, bool> Scheduler::timeoutRoutine = std::map<time_t, bool>();
std::function<void(void)>& Scheduler::currentRoutine = null_routine;
long Scheduler::currentRoutineId;
pthread_t Scheduler::controlThread;


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
long Scheduler::set_timeout(const std::function<void(void)>& routine, long delay)
{
    initialize_routine_id();
    currentRoutine = routine;

    pthread_t thread;
    pthread_create(&thread, nullptr, delay_control_routine, (void*) delay);

    return currentRoutineId;
}

void Scheduler::initialize_routine_id()
{
    currentRoutineId = get_current_time();
}

void* Scheduler::delay_control_routine(void* arg)
{
    long delay = (long) arg;
    long id = currentRoutineId;
    const std::function<void(void)>& routine = currentRoutine;

    delayRoutines[id] = true;

    #ifdef LINUX
        usleep(delay * 1000);
    #endif
    #ifdef WINDOWS
        Sleep(delay);
    #endif

    if (delayRoutines[id])
        routine();
}

void Scheduler::clear_timeout(long id)
{
    delayRoutines[id] = false;
}

long Scheduler::set_interval(const std::function<void(void)>& routine, long interval)
{
    initialize_routine_id();
    currentRoutine = routine;

    pthread_t thread;
    pthread_create(&thread, nullptr, interval_control_routine, (void*) interval);

    return currentRoutineId;
}

void* Scheduler::interval_control_routine(void* arg)
{
    long interval = (long) arg;
    long id = currentRoutineId;
    const std::function<void(void)>& routine = currentRoutine;

    intervalRoutines[id] = true;

    while (intervalRoutines[id])
    {
        routine();

        #ifdef LINUX
            usleep(interval * 1000);
        #endif
        #ifdef WINDOWS
            Sleep(interval);
        #endif
    }
}

void Scheduler::clear_interval(long id)
{
    intervalRoutines[id] = false;
}

bool Scheduler::set_timeout_to_routine(const std::function<void(void)>& routine, long timeout)
{
    run_routine(routine);
    wait_routine_for(timeout);
    finish_routine();

    return !has_routine_finished();
}

void Scheduler::run_routine(const std::function<void(void)>& routine)
{
    initialize_routine_id();

    timeoutRoutine[currentRoutineId] = false;
    currentRoutine = routine;

    pthread_create(&controlThread, nullptr, control_routine, nullptr);
}

time_t Scheduler::get_current_time()
{
    return std::time(nullptr);
}

void* Scheduler::control_routine(void* args)
{
    time_t id = currentRoutineId;
    std::function<void(void)>& routine = currentRoutine;

    routine();
    timeoutRoutine[id] = true;
}

void Scheduler::wait_routine_for(long time)
{
    time_t start = get_current_time();

    while ((time_elapsed_in_milliseconds(start) < (double) time) && !has_routine_finished())
    {
        #ifdef LINUX
            usleep(200 * 1000);
        #endif
        #ifdef WINDOWS
            Sleep(200);
        #endif
    }
}

double Scheduler::time_elapsed_in_milliseconds(time_t start)
{
    return (difftime(get_current_time(), start) * 1000);
}

bool Scheduler::has_routine_finished()
{
    return timeoutRoutine[currentRoutineId];
}

void Scheduler::finish_routine()
{
    pthread_cancel(controlThread);
}
