#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t handled = 0;
volatile sig_atomic_t sent = 0;

void handler(int sig, siginfo_t *info, void *context) {
    handled++;
}

int main() {
    struct sigaction sa;
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;

    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        return 1;
    }

    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 1000000;

    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 1000000;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        return 1;
    }

    for (int i = 0; i < 10000; i++) {
        sent++;
        usleep(100);
    }

    sleep(2);

    timer_delete(timerid);

    printf("Signals sent: %d\n", sent);
    printf("Signals handled: %d\n", handled);
    printf("Signals lost: %d\n", sent - handled);

    return 0;
}
