Para la instalación de xv6 se sigueron los pasos explicados por el profesor en clases.
Lo primero fue crear la carpeta local en la que se ubicaría el programa.
Se hizo fork al repositorio: https://github.com/otrab/xv6-riscv.git, se creo la carpeta local y se realizó la clonación del repo.
Posteriormente, se ejecutó el comando "make qemu" para compilar xv6 y probar el funcionamiento.
Se produjo el error: "Couldn't find a riscv64 version of GCC/binutils.".
A travez de GPT vi que el error se podía deber a no tener el compilador riscv correcto.
Se utilizó Homebrew para instalar "riscv-gnu-toolchain".
Se modificó el archivo "Makefile" para establecer el compilador correcto que se debía utilizar.
Se ejecuto nuevamente el comando "make qemu" y la compilación fue exitosa.
Finalmente se probaron los comandos: ls, echo "Hola xv6" y cat README, para verificar la correcta ejecución del programa.
Todos los comandos se realizaron exitosamente.