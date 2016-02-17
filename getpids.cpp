#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include <string>
#include <cstring>
#include <cstdio>

using namespace std;

int main(int argc, char *argv[]) {
	int pid;
	int count = 0;
	
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
			cout << pid << endl;
			count++;
		}
	}
	cout << "Total pids: " << count << endl;

	closedir(pdir);

	return 0;
}
