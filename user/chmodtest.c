#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  int fd;
  char *filename = "testfile";
  char buf[50];

  // Creación del archivo
  fd = open(filename, O_CREATE | 2);
  if(fd < 0){
    printf("Error al crear el archivo\n");
    exit(1);
  }

  // Escritura inicial en el archivo
  if(write(fd, "Hola mundo!\n", 12) != 12){
    printf("Error al escribir en el archivo\n");
    exit(1);
  }
  close(fd);

  // Cambio de permisos del archivo a solo lectura
  if(chmod(filename, 1) < 0){
    printf("Error al cambiar los permisos del archivo\n");
    exit(1);
  }

  // Intento de escritura en el archivo con permisos de solo lectura
  fd = open(filename, 1);
  if(fd >= 0){
    printf("Error: se abrió el archivo con permisos de solo lectura en modo escritura\n");
    exit(1);
  } else {
    printf("No se puede abrir el archivo con permisos de solo lectura en modo escritura\n");
  }

  // Cambio de permisos del archivo de vuelta a lectura y escritura
  if(chmod(filename, 3) < 0){
    printf("Error al cambiar los permisos del archivo\n");
    exit(1);
  }

  // Abrir el archivo para nueva escritura
  fd = open(filename, 2);
  if(fd < 0){
    printf("Error al abrir el archivo\n");
    exit(1);
  }

  // Mover el offset al final del archivo
  char temp[1];
  while(read(fd, temp, 1) > 0);

  // Nueva escritura en el archivo
  if(write(fd, "Adios mundo!", 12) != 12){
    printf("Error al escribir en el archivo después de restaurar los permisos\n");
    exit(1);
  }
  close(fd);

  // Cambiar los permisos del archivo a inmutable y legible
  if(chmod(filename, 5) < 0){
    printf("Error al cambiar los permisos del archivo a inmutable\n");
    exit(1);
  } else {
    printf("Permisos cambiados a inmutable y solo lectura\n");
  }

  // Intentar abrir el archivo en modo escritura (debe fallar)
  fd = open(filename, 1);
  if(fd >= 0){
    printf("Error: se abrió un archivo inmutable en modo escritura\n");
    exit(1);
  } else {
    printf("No se puede abrir un archivo inmutable en modo escritura\n");
  }

  // Intentar cambiar los permisos de vuelta a lectura y escritura (debe fallar)
  if(chmod(filename, 3) < 0){
    printf("No se pueden cambiar los permisos de un archivo inmutable\n");
  } else {
    printf("Error: se cambiaron los permisos de un archivo inmutable\n");
    exit(1);
  }

  // Lectura del archivo para verificar contenido
  fd = open(filename, 0);
  if(fd < 0){
    printf("Error al abrir el archivo en modo lectura\n");
    exit(1);
  }
  int n = read(fd, buf, sizeof(buf) - 1);
  if(n >= 0){
    buf[n] = '\0';
    printf("Contenido del archivo:\n%s\n", buf);
  } else {
    printf("Error al leer el archivo\n");
  }
  close(fd);

  exit(0);
}