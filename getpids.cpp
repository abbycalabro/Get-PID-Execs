#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
	int pid, count, start_pos, target;
	string pid_string, path;
	const char *c_path;
	char buf[512];
	static const size_t npos = -1;

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
			
			//read exe symlink 
			count = readlink(c_path, buf, sizeof(buf) - 1);

			//print pid with corresponding symlink target for paths containing "sudo"
			if(count >= 0) { 
				buf[count] = '\0';
				string target = buf;	

				if(target.find("sudo") != npos)
					cout << "pid: " << pid << " " << target << endl;
			}
			else if(count != -1)
				cout << "Error calling readlink()\n";
		}
	}

	closedir(pdir);

	return 0;
}
