package wniemiec.util.task.process;

import java.io.IOException;

class WindowsProcessManager extends ProcessManager {
	
	//-------------------------------------------------------------------------
	//		Methods
	//-------------------------------------------------------------------------
	public void forceKillProcessWithPid(long pid) throws IOException {
		runtime.exec("taskkill /F /PID" + pid);
	}
}
