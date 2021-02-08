package wniemiec.util.task.process;

import java.io.IOException;

class UnixProcessManager extends ProcessManager {
	
	//-------------------------------------------------------------------------
	//		Methods
	//-------------------------------------------------------------------------
	public void forceKillProcessWithPid(long pid) throws IOException {
		runtime.exec("kill -9 " +pid);
	}
}
