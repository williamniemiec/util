#include <map>
#include <ctime>
#ifdef LINUX
#   include <unistd.h>
#endif
#ifdef WINDOWS
#   include <windows.h>
#endif
#include "../../../../include/wniemiec/util/task/Scheduler.hpp"

using namespace wniemiec::util::task;

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
std::map<time_t, bool> Scheduler::timeoutRoutine = std::map<time_t, bool>();
void (*Scheduler::currentRoutine)();
time_t Scheduler::currentRoutineId;
pthread_t Scheduler::controlThread;


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
bool Scheduler::set_timeout_to_routine(void (*routine)(), int timeout)
{
    run_routine(routine);
    wait_for(timeout);
    finish_routine();

    return !has_routine_finished();
}

void Scheduler::run_routine(void (*function)())
{
    initialize_routine_id();
    
    currentRoutine = function;
    pthread_create(&controlThread, nullptr, control_routine, nullptr);
}

void Scheduler::initialize_routine_id()
{
    currentRoutineId = get_current_time();
}

time_t Scheduler::get_current_time()
{
    return std::time(nullptr);
}

void* Scheduler::control_routine(void* args)
{
    time_t id = currentRoutineId;
    void (*routine)() = currentRoutine;
    
    timeoutRoutine[id] = false;
    routine();
    timeoutRoutine[id] = true;
}

void Scheduler::wait_routine_for(int time)
{
    time_t id = currentRoutineId;
    time_t start = get_current_time();

    while ((time_elapsed_in_milliseconds(start) < (double) time) && !has_routine_finished())
    {
        #ifdef LINUX
            sleep(1);
        #endif
        #ifdef WINDOWS
            Sleep(1000);
        #endif
    }
}

double Scheduler::time_elapsed_in_milliseconds(time_t start)
{
    return ((difftime(get_current_time(), start) * 1000);
}

bool Scheduler::has_routine_finished()
{
    return timeoutRoutine[currentRoutineId];
}

void Scheduler::finish_routine()
{
    pthread_cancel(controlThread);
}
