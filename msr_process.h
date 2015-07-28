#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

extern int g_quit;

typedef struct {
	int channel[2];
	pid_t pid;
}msr_process_t;

#define MSR_MAX_PROCESSES         512

void msr_master_process_cycle();
void msr_start_worker_processes();
void msr_spawn_process(int i);
void msr_worker_process_cycle(int i);
void msr_do_master_process();