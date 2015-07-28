#include "mysrv.h"

static int g_quit_cmd = 0;
extern int g_quit;
int parse_cmd_line(int argc, char *const argv[])
{
    int iOpt;
    const char *optString = "sv";
    char *configFile = NULL;

    while( (iOpt = getopt(argc, argv, optString)) != EOF ) {
        switch( iOpt ) {
            case 's':
                g_quit_cmd = 1;
                break;
            default:
                break;
        }
    }
}

int msr_get_pid(char *file, char *pid)
{
    if(access(file, F_OK) != 0) {
        printf("access file error\n");
            return -1;
    }

    int hd= open(file, O_RDONLY);
    read(hd, pid, 20);
}

int msr_set_pid(char *file, int pid)
{
    char str_pid[20];
    sprintf(str_pid, "%d", pid);
    //printf("dddd:%s\n", str_pid);
    int hd= open(file, O_RDWR|O_CREAT|O_TRUNC);
    int len = 0;
    if ((len = write(hd, str_pid, strlen(str_pid))) != strlen(str_pid)) 
        printf("write error:%d\n", len);
    close(hd);
}

static void sig_term(int signum)
{
    if(signum == SIGTERM)
    {
        printf("SIGTERM received pid:%d\n", getpid());
        g_quit = 1;
        printf("set g_quit = 1\n");
    }
    else
    {
        printf("signal %d received\n", signum);
    }
}

void start_daemon()
{
    int pid = 0;
    if( (pid = fork()) > 0 ) {
        exit(0);
    }
    setsid();
    if( (pid = fork()) > 0 ) {
        exit(0);
    }
}

int main(int argc, char **argv)
{
    parse_cmd_line(argc, argv);

    //start_daemon();

    struct sigaction sa_usr;
    sa_usr.sa_flags = 0;
    sa_usr.sa_handler = sig_term;
    sigaction(SIGTERM, &sa_usr, NULL);

	if (g_quit_cmd == 1) {
		char str_pid[20] = {0};
        if(msr_get_pid("./msr.pid", str_pid) < 0) return -1;
        printf("get pid:%s\n", str_pid);
        int pid = atoi(str_pid);
        kill(pid, SIGTERM);
        printf("kill pid:%d\n", pid);
        exit(0);
	}

    msr_set_pid("./msr.pid", getpid());

	msr_master_process_cycle();
}
