package wniemiec.util.task.process;

import java.io.IOException;

/**
 * Responsible for managing processes.
 * 
 * @author		William Niemiec &lt; williamniemiec@hotmail.com &gt;
 */
public abstract class ProcessManager {
	
	//-------------------------------------------------------------------------
	//		Attributes
	//-------------------------------------------------------------------------
	protected Runtime runtime = Runtime.getRuntime();
	
	
	//-------------------------------------------------------------------------
	//		Methods
	//-------------------------------------------------------------------------
	public static ProcessManager getInstance() {
		if (isWindowsOS())
			return new WindowsProcessManager();
		else
			return new UnixProcessManager();
	}
		
	public abstract void forceKillProcessWithPid(long pid) throws IOException;
	
	private static boolean isWindowsOS() {
		return System.getProperty("os.name").toLowerCase().contains("windows");
	}
}
