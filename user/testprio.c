#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_PROCESOS 20 // definimos número de procesos que se crearán

// función para crear un proceso hijo
void proceso_hijo(int num) {
  printf("Ejecutando proceso: %d PID: %d\n", num, getpid()); // imprimimos en pantalla el número del proceso y su pid
  sleep(10); // detenemos el proceso por unos segundos
  exit(0); // terminamos el proceso
}

int
main(void)
{
  int i;
  // creamos los procesos
  for(i = 0; i < NUM_PROCESOS; i++) {
    int pid = fork(); // creamos nuevo proceso hijo
    if(pid < 0) {
      printf("Error en fork\n"); // imprimimos en pantalla el error si falla el fork
      exit(1); // terminamos el programa
    } else if(pid == 0) {
      proceso_hijo(i); // ejecutamos el proceso hijo
    }
    wait(0); // esperamos a que el proceso termine antes de crear otro
  }
  exit(0); // terminamos el programa
}