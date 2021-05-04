#include <map>
#include <ctime>
#include "../../../../include/wniemiec/util/task/Scheduler.hpp"

using namespace wniemiec::util::task;

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
std::map<int, time_t> Scheduler::timeoutRoutine = std::map<int, time_t>();
void (*Scheduler::currentRoutine)();
time_t Scheduler::currentRoutineId;


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
bool Scheduler::set_timeout_to_routine(void (*routine)(), double timeout)
{
    time_t id = get_current_time();
    
    pthread_t controlThread = run_routine(routine, id);
    wait_for(timeout/1000);
    finish_routine(controlThread);

    return !timeoutRoutine[id];
}

pthread_t Scheduler::run_routine(void (*function)(), time_t id)
{
    pthread_t controlThread;

    currentRoutine = function;
    currentRoutineId = id;
    pthread_create(&controlThread, nullptr, control_routine, nullptr);

    return controlThread;
}

void Scheduler::finish_routine(pthread_t routineThread)
{
    pthread_cancel(routineThread);
}

void* Scheduler::control_routine(void* args)
{
    time_t id = currentRoutineId;
    void (*routine)() = currentRoutine;
    timeoutRoutine[id] = false;
    routine();
    timeoutRoutine[id] = true;
}

void Scheduler::wait_for(double time)
{
    time_t start = get_current_time();

    while (get_current_time() - start < time)
        ;
}

time_t Scheduler::get_current_time()
{
    return std::time(nullptr);
}
