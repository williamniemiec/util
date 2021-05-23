#include "../../../../include/wniemiec/util/task/Scheduler.hpp"

#include <ctime>
#include <chrono>
#include <thread>

using namespace wniemiec::util::task;

std::function<void(void)> null_routine = [](){};

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
std::map<unsigned long, bool> Scheduler::delayRoutines = std::map<unsigned long, bool>();
std::map<unsigned long, bool> Scheduler::intervalRoutines = std::map<unsigned long, bool>();
std::map<unsigned long, bool> Scheduler::timeoutRoutine = std::map<unsigned lonf, bool>();
std::function<void(void)>& Scheduler::currentRoutine = null_routine;
unsigned long Scheduler::currentRoutineId;
pthread_t Scheduler::controlThread;


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
unsigned long Scheduler::set_timeout(const std::function<void(void)>& routine, long delay)
{
    initialize_routine_id();
    currentRoutine = routine;

    pthread_t thread;
    pthread_create(&thread, nullptr, delay_control_routine, (void*) delay);

    return currentRoutineId;
}

void Scheduler::initialize_routine_id()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    currentRoutineId = get_current_time();
}

void* Scheduler::delay_control_routine(void* arg)
{
    long delay = (long) arg;
    unsigned long id = currentRoutineId;
    const std::function<void(void)> routine = currentRoutine;
    
    if (delay < 0)
        throw std::invalid_argument("Delay cannot be negative");

    delayRoutines[id] = true;

    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    if (delayRoutines[id])
        routine();
    
    return nullptr;
}

void Scheduler::clear_timeout(unsignedlong id)
{
    delayRoutines[id] = false;
}

unsigned long Scheduler::set_interval(const std::function<void(void)>& routine, long interval)
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
    unsigned long id = currentRoutineId;
    const std::function<void(void)> routine = currentRoutine;

    if (interval < 0)
        throw std::invalid_argument("Interval cannot be negative");
    
    intervalRoutines[id] = true;

    while (intervalRoutines[id])
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        routine();
    }
    
    return nullptr;
}

void Scheduler::clear_interval(unsigned long id)
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
    
    return nullptr;
}

void Scheduler::wait_routine_for(long time)
{
    time_t start = get_current_time();

    while ((time_elapsed_in_milliseconds(start) < (double) time) && !has_routine_finished())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
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
