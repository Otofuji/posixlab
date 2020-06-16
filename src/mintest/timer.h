/*

IMPORTANT

NOTICE
SUPPORT FILE SOURCED FROM INTERNET

REFERRED FROM https://lowlevelbits.org/handling-timeouts-in-child-processes/
AUTHOR: AlexDenisov
ORIGIN: https://github.com/AlexDenisov/articles

With improvements to fit this posixlab.

*/


#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

typedef int (*work_t)(int);

pid_t waitpid_eintr(int *status);

int watchdog_worker_timer(work_t work, long long timeout, int i);

void handle_alarm_signal(int signal, siginfo_t *info, void *context);
void setup_timer(long long timeout);
void system_timers(work_t work, long long timeout, int i);

void endless_loop(void);
void short_task(void);

/* int main() {
  system_timers(endless_loop, 1000);
  system_timers(short_task, 1000);

  watchdog_worker_timer(endless_loop, 1000);
  watchdog_worker_timer(short_task, 1000);

  return 0;
} */

void flame() {
    printf("\033[0;31m");
}

void extinguisher() {
  printf("\033[0m");
}

pid_t waitpid_eintr(int *status) {
  pid_t pid = 0;
  while ( (pid = waitpid(WAIT_ANY, status, 0)) == -1 ) {
    if (errno == EINTR) {
      continue;
    } else {
      perror("waitpid");
      abort();
    }
  }
  return pid;
}

int watchdog_worker_timer(work_t work, long long timeout, int i) {
  const pid_t timer_pid = fork();
  if (timer_pid == -1) {
    perror("fork timer");
    abort();
  }

  if (timer_pid == 0) {
    /// Timer process
    usleep(timeout * 1000);
    exit(0);
  }

  const pid_t worker_pid = fork();
  if (worker_pid == -1) {
    perror("fork worker");
    abort();
  }
  if (worker_pid == 0) {
    /// Worker process
    int child_child_pass_count = work(i);
    if (child_child_pass_count == 1) {
      exit(EXIT_SUCCESS);
    }
    else {exit(EXIT_FAILURE);}
  }

  int status = 0;
  const pid_t finished_first = waitpid_eintr(&status);
  if (finished_first == timer_pid) {
    flame();
    printf("%s: [TIME] FAULT CAUSED BY RUNTIME TIMEOUT \n", all_tests[i].name);
    extinguisher();
    kill(worker_pid, SIGKILL);
    return EXIT_FAILURE;
  } else if (finished_first == worker_pid) {
    return !WEXITSTATUS(status);
    kill(timer_pid, SIGKILL);

  } 
//   else {
//     assert(0 && "Something went wrong");
//   }

  waitpid_eintr(&status);
}

void handle_alarm_signal(int signal, siginfo_t *info, void *context) {
  _exit(112);
}

void setup_timer(long long timeout) {
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_sigaction = &handle_alarm_signal;
  if (sigaction(SIGALRM, &action, NULL) != 0) {
    perror("sigaction");
    abort();
  }

  struct itimerval timer;
  timer.it_value.tv_sec = timeout / 1000;
  /// Cut off seconds, and convert what's left into microseconds
  timer.it_value.tv_usec = (timeout % 1000) * 1000;

  /// Do not repeat
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 0;

  if (setitimer(ITIMER_REAL, &timer, NULL) != 0) {
    perror("setitimer");
    abort();
  }
}

void system_timers(work_t work, long long timeout, int i) {
  const pid_t worker_pid = fork();
  if (worker_pid == -1) {
    perror("fork worker");
    abort();
  }
  if (worker_pid == 0) {
    setup_timer(timeout);
    work(i);
    exit(0);
  }

  int status = 0;
  waitpid_eintr(&status);

  if (WIFEXITED(status) && WEXITSTATUS(status) == 112) {
    printf("timed out\n");
  }

  else if (WIFEXITED(status) && WEXITSTATUS(status) != 144) {
   
  }
}

void endless_loop(void) {
  printf("starting endless loop\n");

  /// Avoiding UB
  volatile int x = 0;
  while (1) {
    if (x != 0) {
      break;
    }
  }
}

void short_task(void) {
  printf("finished short task\n");
}

