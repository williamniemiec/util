package wniemiec.util.task;

import java.util.*;
import java.util.concurrent.*;

/**
 * Responsible for calling routines after a certain time or whenever the the 
 * timer expires.
 * 
 * @author		William Niemiec &lt; williamniemiec@hotmail.com &gt;
 */
public class Scheduler {
	
	//-------------------------------------------------------------------------
	//		Attributes
	//-------------------------------------------------------------------------
	/**
	 * Stores routines sent to setInterval method along with its ids.
	 */
	private static Map<Integer, Timer> intervalRoutines = new HashMap<>();
	
	/**
	 * Stores routines sent to setTimeout method along with its ids.
	 */
	private static Map<Integer, Timer> delayRoutines = new HashMap<>();

	private static Map<Long, Boolean> timeoutRoutines = new HashMap<>();

	
	//-------------------------------------------------------------------------
	//		Constructor
	//-------------------------------------------------------------------------
	private Scheduler() {
	}
	
	
	//-------------------------------------------------------------------------
	//		Methods
	//-------------------------------------------------------------------------
	/**
	 * Sets a timer which executes a routine once the timer expires.
	 * 
	 * @param		routine Routine to be performed
	 * @param		id Routine identifier (necessary to be able to stop it)
	 * @param		delay Waiting time before the routine is executed (in 
	 * milliseconds)
	 * 
	 * @return		False if an timeout has has already been set with the 
	 * given id or true otherwise
	 * 
	 * @throws		IllegalArgumentException If routine is null or if id or 
	 * delay is negative
	 */
	public static boolean setTimeout(Runnable routine, int id, int delay) {
		if (routine == null)
			throw new IllegalArgumentException("Routine cannot be null");
		
		if (id < 0)
			throw new IllegalArgumentException("Id cannot be negative");
		
		if (delay < 0)
			throw new IllegalArgumentException("Delay cannot be negative");
		
		if (delayRoutines.containsKey(id))
			return false;
		
		delayRoutines.put(id, scheduleTimeout(routine, delay));
		
		return true;
	}
	
	private static Timer scheduleTimeout(Runnable routine, int delay) {
		Timer timer = new Timer();
		timer.schedule(createTaskFromRoutine(routine), delay);
		
		return timer; 
	}

	private static TimerTask createTaskFromRoutine(Runnable routine) {
		return new TimerTask() {
		    @Override
		    public void run() {
		       routine.run();
		    }
		};
	}
	
	/**
	 * Repeatedly calls a routine with a fixed time delay between each call.
	 * 
	 * @param		routine Routine to be performed
	 * @param		id Routine identifier (necessary to be able to stop it)
	 * @param		interval Interval that the routine will be invoked (in 
	 * milliseconds)
	 * 
	 * @return		False if an interval has has already been set with the 
	 * given id or true otherwise
	 * 
	 * @throws		IllegalArgumentException If routine is null or if id or 
	 * interval is negative
	 */
	public static boolean setInterval(Runnable routine, int id, int interval) {
		if (routine == null)
			throw new IllegalArgumentException("Routine cannot be null");
		
		if (id < 0)
			throw new IllegalArgumentException("Id cannot be negative");
		
		if (interval < 0)
			throw new IllegalArgumentException("Interval cannot be negative");
		
		if (intervalRoutines.containsKey(id))
			return false;
		
		intervalRoutines.put(id, scheduleInterval(routine, interval));
		
		return true;
	}
	
	private static Timer scheduleInterval(Runnable routine, int interval) {
		Timer timer = new Timer();
		timer.scheduleAtFixedRate(createTaskFromRoutine(routine), 0, interval);
		
		return timer; 
	}
	
	/**
	 * Cancels a timed, repeating action, which was previously established by a
	 * call to {@link #setInterval(Runnable, int, int)}.
	 * 
	 * @param		id Routine id
	 */
	public static void clearInterval(int id) {
		if (!intervalRoutines.containsKey(id))
			return;
		
		intervalRoutines.get(id).cancel();
		intervalRoutines.remove(id);
	}
	
	/**
	 * Cancels a timed action which was previously established by a
	 * call to {@link #setTimeout(Runnable, int, int)}.
	 * 
	 * @param		id Routine id
	 */
	public static void clearTimeout(int id) {
		if (!delayRoutines.containsKey(id))
			return;
		
		delayRoutines.get(id).cancel();
		delayRoutines.remove(id);
	}
	
	public static void clearAllTimeout() {
		for (int timeoutId : delayRoutines.keySet()) {
			clearTimeout(timeoutId);
		}
	}
	
	public static void clearAllInterval() {
		for (int intervalId : intervalRoutines.keySet()) {
			clearInterval(intervalId);
		}
	}

	/**
	 * Runs a routine within a timeout.
	 *
	 * @param		routine Routine
	 * @param		timeout Maximum execution time (in milliseconds)
	 *
	 * @return		True if the routine has not finished executing within the
	 * time limit; false otherwise
	 *
	 * @throws IllegalArgumentException If routine is null or if timeout is
	 * is negative
	 */
	public static boolean setTimeoutToRoutine(Runnable routine, int timeout) {
		if (routine == null)
			throw new IllegalArgumentException("Routine cannot be null");

		if (timeout < 0)
			throw new IllegalArgumentException("Timeout cannot be negative");

		long id = getCurrentTime();

		Future<?> controlThread = runRoutine(routine, id);
		waitFor(timeout);
		finishRoutine(controlThread);

		return !timeoutRoutines.get(id);
	}

	private static long getCurrentTime() {
		return new Date().getTime();
	}

	private static Future<?> runRoutine(Runnable routine, long id) {
		ExecutorService controlService = Executors.newCachedThreadPool();

		return controlService.submit(() -> {
			timeoutRoutines.put(id, false);
			routine.run();
			timeoutRoutines.put(id, true);
		});
	}

	private static void waitFor(int time) {
		try {
			Thread.sleep(time);
		}
		catch (InterruptedException e) {
			// Just stop waiting
		}
	}

	private static void finishRoutine(Future<?> controlThread) {
		controlThread.cancel(true);
	}
}
