#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cerrno>
#include <fstream>

using namespace std;

string get_ppid(string pid) {
	ifstream status;
	string ppid, result, current_input;
	int start, end;
	
	//get ppid of current process
	status.open("/proc/" + pid + "/status");
	if(status.is_open()) {
		while(!status.eof()) {
			status >> current_input;
			result += current_input;
		}
	}
	else
		cout << "Error opening /proc/" + pid + "/status\n";
	
	//parse /proc/pid/status for ppid
	start = result.find("PPid:") + 5;
	end = result.find("Tracer");
	ppid = result.substr(start, end - start);		
	status.close();

	return ppid;
}

string get_target(string pid) {
	char buf[512];
	int count;
	string path, result;
	const char *c_path;
    
	path = "/proc/" + pid + "/exe";
	c_path = path.c_str();

	//read /proc/pid/exe symlink 
	count = readlink(c_path, buf, sizeof(buf) - 1);

	if(count >= 0) { 
		buf[count] = '\0';
		result = buf;	
	}
	else if(count != -1)
		result = "Error calling readlink()\n";

	return result;
}

int main(int argc, char *argv[]) {
	string pid_str, ppid_str, pid_target, ppid_target;
	int pid;
	static const size_t npos = -1;
	struct dirent *current_dir;
	DIR *pdir;

	//open /proc/ directory
	pdir = opendir("/proc");
	if(!pdir) {
		cout << "Error calling opendir()" << endl;
	}

	//iterate through all /proc/ directories
	while((current_dir = readdir(pdir))) {
		//get pid, ppid, and ppid target
		pid = atoi(current_dir->d_name);
		if(pid > 0) { 
			pid_str = current_dir->d_name;
			pid_target = get_target(pid_str);
			
			//only print info for pid targets containing "sudo"
			if(pid_target.find("sudo") != npos) {
				ppid_str = get_ppid(pid_str);
				ppid_target = get_target(ppid_str);

				cout << "pid: " << pid_str << "    ppid: " << ppid_str << "    " << ppid_target << endl;
			}
		}
	}
	closedir(pdir);

	return 0;
}
