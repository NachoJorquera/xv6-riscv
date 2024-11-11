Introducción:

En este informe se detalla el funcionamiento y la lógica detrás de la protección de memoria, junto con una explicación de las modificaciones realizadas para la implementación de ésta en el código fuente de xv6 RISC-V, y las dificultades encontradas en el proceso junto con la forma en que se solucionaron.

Funcionamiento y lógica:

La protección de memoria es un mecanismo fundamental que permite controlar el acceso a regiones específicas de la memoria por parte de los procesos.
En xv6, la memoria se gestiona mediante paginación. Cada proceso tiene su propia tabla de páginas y cada PTE contiene información sobre los permisos y la tabla física asociada, donde el bit W determina si una página tiene permisos de escritura.
En esta implementación la función mprotect se encarga de marcar las páginas del rango especificado como sólo lectura, deshabilitando el bit de escritura, y la función munprotect revierte lo anterior, habilitando nuevamente el bit de escritura.

Modificaciones:

- En el archivo “syscall.h” se asignaron los números únicos para las llamadas a sistema “sys_mprotect” y “sys_munprotect”, en este caso 22 y 23 respectivamente.
- En el archivo “syscall.c” se agregaron los prototipos de las funciones que manejan las llamadas a sistema:“extern uint64 sys_mprotect(void)” y “extern uint64 sys_munprotect(void)” y se agregaron en el array que mapea la llamada con el número único asignado: “[SYS_mprotect] sys_mprotect” y “[SYS_munprotect] sys_munprotect”
- En el archivo “defs.h” se declararon las funciones “mprotect” y “munprotect” para que estén disponibles para los archivos del kernel que las necesiten. “int mprotect(uint64 addr, int len)” y “int munprotect(uint64 addr, int len)” respectivamente.
- En el archivo “user.h” se declararon las referencias de las funciones “mprotect” y “munprotect” para que los programas de usuario puedan usarlas. “int mprotect(void *addr, int len)” y “int munprotect(void *addr, int len)” respectivamente.
- En el archivo “usys.pl” se agregaron las referencias que permiten que las llamadas desde el espacio de usuario invoquen las llamadas a sistema. “entry("mprotect”)” y “entry("munprotect”)” respectivamente. 
- En el archivo “vm.c” se incluyeron las definiciones de “spinlock.h” y “proc.h” en la cabecera, y se implementaron las funciones “mprotect” y “munprotect” que manipulan las tablas de páginas para cambiar los permisos de memoria.
-  En el archivo “sysproc.c” se implementaron las llamadas a sistema “sys_mprotect” y sys_munprotect” que actúan como intermediarias entre el espacio de usuario y las funciones de kernel.
- Se creó el archivo “mprotect_test.c” dónde se agregó el programa de prueba.
- Se modificó el archivo “Makefile” para que el programa este disponible al compilar xv6.

Problemas encontrados y cómo se solucionaron:

Las definiciones de mprotect y munprotect en un inicio no las tenía definidas en el archivo defs.h ni en otro archivo, lo que provocaba un error en la compilación de xv6. La primera solución que aplique para esto fue incluirlas directamente el archivo vm.c. Luego, con un poco más de investigación encontré que podía agregar las definiciones directamente en el archivo defs.h, lo que funcionó correctamente tras la compilación.
Otro problema que tuve fue que al compilar xv6 obtenía un error en el archivo vm.c que decía que el campo ‘lock’ tenía un tipo incompleto. Busqué el problema en internet y encontré que la solución era simplemente incluir la definición de “spinlock.h” en la cabecera del archivo.
Finalmente, el ultimo problema que tuve fue con el programa de prueba, el cual al compilar xv6 me daba el error que los directorios incluidos en la cabecera no existían, y revisando cómo implementé los programas de prueba en las tareas anteriores, me di cuenta que sólo había que especificar la carpeta dónde se ubicaban dichos directorios. Volví a compilar xv6 y todo funcionó correctamente. 