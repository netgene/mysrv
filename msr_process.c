#include "msr_process.h"

int msr_processes_num;
msr_process_t    msr_processes[MSR_MAX_PROCESSES];
int g_quit = 0;

void msr_master_process_cycle()
{
	//for ( ;; ) {
		msr_start_worker_processes();

		msr_do_master_process();
	//}
}

void kill_work_processes()
{
	int i;
	for (i = 0; i < msr_processes_num; i++) {
		kill(msr_processes[i].pid, SIGTERM);
		printf("kill aaa pid:%d\n", msr_processes[i].pid);
	}
}

void msr_start_worker_processes()
{
	int i;
	msr_processes_num = 2;
	for (i = 0; i < msr_processes_num; i++) {
		msr_spawn_process(i);
	}

	for (i = 0; i < msr_processes_num; i++) {
		printf("bbbbb pid:%d %d %d\n", msr_processes[i].pid, msr_processes[i].channel[0],
			msr_processes[i].channel[1]);
	}

	//waitpid(-1, NULL, 0);
	//write(msr_processes[0].channel[0], "p", 1);
	//write(msr_processes[1].channel[0], "k", 1);
}

void msr_spawn_process(int i)
{

	socketpair(AF_UNIX, SOCK_STREAM, 0, msr_processes[i].channel);

	int pid = fork();
	if (pid < 0) {
		printf("error in fork!\n");
	} else if (pid == 0) {
		printf("i am the child process, my process id is %d\n", getpid());
		msr_worker_process_cycle(i);
		exit(0);
	} else {
		msr_processes[i].pid = pid;
		//printf("i am the parent process, my process id is %d\n", getpid());
	}
}

void msr_do_master_process()
{
	while(1)
	{
		sleep(5);
		if (g_quit == 1) {
			printf("get g_quit = 1\n");
			kill_work_processes();
			printf("i am quit.pid:%d\n", getpid());
			break;
		}
		else
			printf("get g_quit = 0\n");
	}
}

void msr_worker_process_cycle(int i)
{
	while(1)
	{
		sleep(5);
		if (g_quit == 1) {
			printf("i am chilid quit.pid:%d\n", getpid());
			break;
		}
		printf("i am child pid:%d\n", getpid());
	}
	//char c;

	//read(msr_processes[i].channel[1], &c, 1);
	//printf("read from parent:%c\n", c);
}
