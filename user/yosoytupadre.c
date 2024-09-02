#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  printf("PID: %d, PPID: %d\n", getpid(), getppid());
  printf("\n");
  for(int i = 0; i < 4; i++){
    printf("Ancestor %d PID: %d\n", i, getancestor(i));
  }
  exit(0);
}