// Create a C++11 thread to monitor cluster file changes 
// 2018.06.11

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <unistd.h>
#include <errno.h>
#include <sys/inotify.h>

#define EVENT_SIZE  sizeof(struct inotify_event)
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

using namespace std;

void monitor_file(const char* path, const char* filename) {
    cout << "[INFO] monitor thread created." << endl;
    int i = 0;
    int fd, wd;

    // Initialize Inotify
    fd = inotify_init();
    if (fd < 0) cout << "Couldn't initialize inotify" << endl;

    // add watch to starting directory
    wd = inotify_add_watch(fd, path, IN_MODIFY);

    if (wd == -1) 
        cout << "Couldn't add watch to " << path << endl;
    else
        cout << "Watching: " << path << endl;

    // start watching
    // while(true) {
    while(1) {
        int i = 0;
        char buffer[BUF_LEN];
        int length = read(fd, buffer, BUF_LEN);
        if (length < 0) cout << "read.." << endl;

        while (i < length) {
            cout << "[INFO] Waiting event.. " << endl;

            struct inotify_event *event = (struct inotify_event *) &buffer[i];

            if (event->len) {
                if (event->mask & IN_MODIFY) {
                    cout << "[INFO] The file " << event->name << " was modified with watch descriptor " << event->wd << endl;

                    // if event is triggered by target file
                    if (strcmp(event->name, filename) == 0)
                        cout << "[ACTION] Do cluster update stuff.. " << endl;
                }
            }
            // update index to start of next event
            i += EVENT_SIZE + event->len;
        }
    } 

    // Clean up
    // Supposed to be unreached.
    inotify_rm_watch(fd, wd);
    close(fd);
}

int main() {
    
    const char* filename = "cluster.cfg";
    const char* dir = "/home/hylee/elastic-dev/cpp_ver/";

    // create a monitored thread
    std::thread t1(monitor_file, std::ref(dir), std::ref(filename));

    // cout << "main thread" << endl;

    // join the thread with the main thread
    t1.join();
    return 0;
}
