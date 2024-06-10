#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include "system_info.h"

// 시스템 정보를 수집하여 버퍼에 저장하는 함수
void get_system_info(char *buffer, size_t size) {
    struct utsname sysinfo;
    if (uname(&sysinfo) == -1) {
        perror("uname");
        exit(EXIT_FAILURE);
    }

    snprintf(buffer, size, "System Name: %s\nNode Name: %s\nRelease: %s\nVersion: %s\nMachine: %s\n",
             sysinfo.sysname, sysinfo.nodename, sysinfo.release, sysinfo.version, sysinfo.machine);
}

