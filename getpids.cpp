#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <map>

using namespace std;

int main(int argc, char *argv[]) {
	int pid;
	string pid_string, path;
	const char *c_path;
	char buf[512];
	int count; 
	//map<int, string> pid_execs;

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
			const char *c_path = path.c_str();
			
			count = readlink(c_path, buf, sizeof(buf) - 1);
			
			//print pid with corresponding exe symlink location
			if(count >= 0) { 
				buf[count] = '\0';		
				cout << pid << ": " << buf << endl;
			}
			else 
				cout << pid << ": no symlink found\n";
		
		}
	}

	closedir(pdir);

	return 0;
}
