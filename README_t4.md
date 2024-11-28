# Tarea 4 Sistemas Operativos

## Introducción

En este informe se detalla el funcionamiento y lógica de los permisos básicos para modificar los permisos de los archivos y cambiar el acceso de lectura/escritura a sólo lectura y viceversa. Además se explican las modificaciones realizadas en el código fuente de xv6 para realizar esto y el programa que prueba su funcionamiento, junto con las dificultades encontradas en el proceso de esto y cómo se solucionaron.

## Funcionamiento y lógica

Los permisos básicos en un sistema de archivos determinan qué operaciones pueden realizar los usuarios sobre los archivos y directorios. Estos permisos incluyen sólo lectura (valor 1), sólo escritura (valor 2), lectura y escritura (valor 3), inmutabilidad y lectura (valor 5), entre otros.
Para modificar los permisos de un archivo, se utilizan comandos y/o llamadas a sistema que cambian los valores asociados a estos permisos. Es importante entender cómo cada permiso puede afectar el acceso y las operaciones permitidas tanto de los archivos como de los directorios para poder garantizar la seguridad y el comportamiento esperado del sistema.

## Modificaciones primera parte

### Modificación en la estructura del inode:

- Se modificó la estructura del inode en disco, la cual se encuentra en el archivo "fs.h". Se agregó el campo de permisos "int perm" a la estructura "dinode", lo que permite almacenar los permisos en disco para que estos persistan entre reinicios del sistema.
- Se modificó la estructura del inode en memoria, la cual se encuentra en el archivo "file.h". Se agregó el mismo campo de permisos "int perm" a la estructura "inode", lo que permite almacenar los permisos en memoria para poder acceder y modificar los permisos durante la ejecución del sistema.
- Se modificó la estructura "ialloc" en el archivo "fs.c". Se agregó "dip->perm = 3" para asegurarnos de que cada vez que se cree un nuevo archivo se establezcan los permisos de lectura y escritura por defecto. 

### Modificación en las operaciones de apertura, lectura y escritura:

Todas estas modificaciones se realizaron en el archivo "sysfile.c".
- Se modificó la función "sys_open" para que al abrir un archivo, el sistema verifique que el modo de apertura "omode" sea compatible con los permisos asignados al inode. Si el archivo es de sólo escritura, no debería poder abrirse en modo escritura, y viceversa.
A las funciones "sys_read" y "sys_write" se les agregó una verificación de que sólo se ejecuten si el archivo es legible y escribible respectivamente.

### Creación de llamada a sistema chmod(archivo:char*, modo:int):

- En el archivo "syscall.h" se agregó la llamada a sistema "SYS_chmod" en la tabla de llamadas y se le asignó el número disponible "22".
- En el archivo "syscall.c" se agregó el prototipo para la función que maneja la llamada a sistema "extern uint64 sys_chmod(void)", y también se agregó en el array que mapea la llamada con el número establecido en "syscall.h".
- En el archivo "sysfile.c" se agregó la implementación de "sys_chmod" la cual nos permite modificar los permisos de un archivo después de su creación dado su nombre y el nuevo modo.
- En el archivo "user.h" se declaró la función "int chmod(char *path, int mode)", y en el archivo "usys.pl" se agregó la entrada para chmod "entry("chmod")". Estas modificaciones permiten que el compilador sepa que la función chmod puede ser utilizada por los programas de usuario, y se asocia con la llamada a sistema implementada en el kernel.

### Creación del programa de prueba:

- Se creó el programa "chmodtest.c" para verificar que las modificaciones funcionan correctamente. Este programa realiza operaciones de creación, lectura, escritura y cambio de permisos en un archivo. Va paso a paso probando que los permisos se establezcan y se respeten correctamente.

## Modificaciones segunda parte

### Agregar un permiso especial (número 5) que marque el archivo como inmutable:

Todas las modificaciones se realizaron en el archivo "sysfile.c".
- Se modificó la función "sys_open" donde se agregó una verificación de si se intenta abrir un archivo inmutable en modo escritura. Genera un error si esto ocurre.
- Se modificó la función "sys_write" donde se agregó una verificación para que no permita escribir en archivo inmutables. Genera un error si esto ocurre.
- Se modificó la función "sys_chmod" donde se agregó una verificación para que no se pueda cambiar los permisos de un archivo inmutable. Genera un error si esto ocurre.

### Modificaciones en el programa de prueba:

Se modificó el archivo "chmodtest.c" para que después de la segunda escritura en el archivo, se cambiaran los permisos a inmutable y legible (número 5). También que se probara tanto la escritura (abriendo el archivo en modo escritura), como la restauración de permisos de vuelta a lectura y escritura, donde ambos deben fallar. Finalmente se lee el contenido del archivo como pasaba en el programa de prueba inicial.

## Problemas encontrados y cómo se solucionaron

El primer problema que tuve fue que luego de hacer todas las modificaciones, a la hora de intentar compilar xv6, este producía un error, el cual, después de un poco de investigación, me di cuenta que se debía a que como se agregó un campo de permisos en la estructura del inode en disco, ahora el tamaño de dinode era de 68 bytes lo que producía el error, ya que el tamaño del inode debe ser divisor exacto del tamaño del bloque, que en este caso es de 1024 bytes. Para solucionar esto, se agregó un campo adicional de padding en la estructura "dinode" con un valor de 60 bytes. Con esto agregamos el relleno necesario para que el inode tenga un valor total de 128 bytes, el cual es divisor exacto de 1024.

El segundo problema que tuve vino justo después de solucionar el anterior. Este problema fue que a la hora de compilar xv6, este no diferenciaba entre dispositivos y archivos al modificar los permisos de estos, por lo que al hacer una llamada a sistema, o cualquier otra acción, el sistema operativo no realizaba nada. Para solucionar este problema, en la función "sys_open" se añadió una verificación previa a la verificación de los permisos, la cual verificaba si el tipo era un dispositivo, y de ser así, que no se apliquen las restricciones de permisos. Además, se estableció que al iniciar los permisos de un archivo recién creado, estos fueran de lectura y escritura.