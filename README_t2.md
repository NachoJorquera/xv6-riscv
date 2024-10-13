Introducción:

En este informe se detalla el funcionamiento y la lógica detrás del sistema de prioridades junto con la explicación de las modificaciones realizadas en el código de xv6 para la implementación de este, y la implementación de un programa para probar el funcionamiento del scheduler basado en prioridades.


Funcionamiento y lógica:

El scheduler utiliza un sistema de prioridades donde cada proceso tiene un valor que determina su orden de ejecución. Los procesos recién creados inician con una prioridad de 0 y un boost de 1. A medida que se van creando procesos, las prioridades de los procesos existentes se van ajustando sumando el valor del boost. Si la prioridad alcanza 9, el boost se cambia a -1, y si llega a 0, el boost vuelve a ser 1.
El scheduler selecciona el proceso con el número de prioridad más bajo (mayor prioridad) para ser ejecutado. Esto asegura que los procesos con mayor necesidad de CPU tengan preferencia de ejecución.


Modificaciones:

Lo primero que se realizó fue agregar los campos priority y boost en el archivo "proc.h". Específicamente en la definición de "struct proc", ya que necesitamos almacenar la prioridad y el boost de cada proceso.

A continuación, se modificó el archivo "proc.c". Acá la primera modificación se realizó en la función "allocproc()" para iniciar la prioridad en 0 y el boost en 1, y además se definió que cada vez que ingresara un proceso nuevo, se actualizara la prioridad de todos los procesos existentes, siguiendo la lógica prioridad += boost y el ajuste del boost cuando la prioridad llega a 9 o 0.

Luego, en el mismo archivo, se modificó la función scheduler. En esta modificación se definieron las variables "highest_p" como un puntero al proceso con la mayor prioridad encontrado, y "highest_priority" como una variable int para almacenar la mayor prioridad (menor número) encontrado hasta ese momento.
Posteriormente, se inicializaron estas variables. "highest_p = 0", para inicializar un puntero nulo, y "highest_priority = 10" para asegurar que todas las posibilidades de prioridades (número de 0 a 9) sean consideradas.
Siguiente a eso, se creó el ciclo for para recorrer toda la tabla de procesos y encontrar el proceso RUNNABLE con mayor prioridad. Para esto lo primero fue adquirir el lock del proceso sobre el cual se está iterando, con el fin de poder revisar su estado de manera segura. Luego se usó una serie de condicionales para verificar el estado del proceso, actualizar las variables en caso de encontrar procesos con mayor prioridad y liberar los locks cuando sea necesario.
Finalmente, se utilizó otro if para seleccionar el proceso a ejecutar. Acá se selecciona el proceso con la mayor prioridad encontrada, cambiamos su estado a RUNNING y hacemos el cambio de contexto para este proceso.

Por último, se modificó la función fork para asegurar que el proceso hijo herede el boost de su padre.


Programa de prueba:

Se creó el programa de prueba "testprio.c", el cual crea 20 procesos hijos utilizando fork, donde cada proceso muestra un mensaje al ejecutarse con su número y su pid. Además se utiliza sleep para detener el proceso unos segundos y wait para que el proceso padre espere a que todos los procesos hijos terminen su ejecución.
Se modificó el archivo "Makefile" para incluir el programa de prueba en la lista de programas de usuario para compilarlo y poder utilizarlo.


Dificultades encontradas y soluciones implementadas:

El único problema que tuve fue a la hora de utilizar el programa de prueba, ya que este en un principio mostraba los mensajes de los procesos entrelazados, lo que producía que no se viera bien el funcionamiento del scheduler con sistema de prioridades. Luego de un poco de investigación, encontré que cuando hay múltiples procesos intentando escribir simultáneamente en la consola se puede producir un problema de concurrencia (problema que estaba sufriendo mi programa).
Para solucionar esto, el procedimiento fue sencillo. Lo único que había que agregar era un "wait(0)" al final del ciclo for en el main, así el programa asegura que cada proceso hijo pueda mostrar su mensaje en consola sin interferencias de los otros procesos. 