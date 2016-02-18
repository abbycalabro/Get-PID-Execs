#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
	int pid, count, start, end;
	string pid_string, path, ppid, target, result, new_input;
	const char *c_path;
	char buf[512];
	static const size_t npos = -1;
	ifstream status;

	//open /proc/ directory
	struct dirent *current;
	DIR *pdir = opendir("/proc");
	if(!pdir) {
		cout << "Error calling opendir()" << endl;
	}

	while((current = readdir(pdir))) {
		//print numerical directory contents
		pid = atoi(current->d_name);
		if(pid > 0) { 
			pid_string = current->d_name;
			path = "/proc/" + pid_string + "/exe";
			c_path = path.c_str();
			
			//read /proc/pid/exe symlink 
			count = readlink(c_path, buf, sizeof(buf) - 1);

			//print pid and parent pid for paths containing "sudo"
			if(count >= 0) { 
				buf[count] = '\0';
				target = buf;	

				if(target.find("sudo") != npos) {
					//get ppid of current process
					status.open("/proc/" + pid_string + "/status");
					if(status.is_open()) {
						while(!status.eof()) {
							status >> new_input;
							result += new_input;
						}
					}
					else
						cout << "Error opening /proc/" + pid_string + "/status\n";
					//parse for ppid
					start = result.find("PPid:") + 5;
					end = result.find("Tracer");
					ppid = result.substr(start, end - start);		
				
					//get target for ppid symlink
					path = "/proc/" + ppid + "/exe";
					c_path = path.c_str();
					count = readlink(c_path, buf, sizeof(buf) - 1);

					if(count >= 0)
						buf[count] = '\0';
					else
						cout << "Error getting ppid target\n";

					status.close();	

					cout << "pid: " << pid << "    ppid: " << ppid << "    " << buf << endl;
				}
			}
			else if(count != -1)
				cout << "Error calling readlink()\n";
		}
	}
	closedir(pdir);

	return 0;
}
