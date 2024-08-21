Introducción:
Este informe detalla los pasos seguidos para la instalación de xv6 en mi entorno local, los problemas que se encontraron durante el proceso, las soluciones aplicadas, y la confirmación de que xv6 funcionara correctamente.

Preparativos Iniciales:
Se comenzó creando una carpeta local para almacenar los archivos del programa xv6.
Se realizó un fork del repositorio: https://github.com/otrab/xv6-riscv.git.
Se utilizó el comando "git clone" para clonar el repositorio en la carpeta local.

Compilación Inicial:
Se ejecutó el comando "make qemu" para compilar xv6 y probar su funcionamiento.
Durante este proceso se produjo el error: "Couldn't find a riscv64 version of GCC/binutils.", el cual indica que no se pudo encontrar una versión de GCC/binutils para riscv6.

Solución al Problema de Compilación:
Se realizó una consulta del problema a GPT, dónde se determinó que faltaba el toolchain para realizar una compilación correcta del programa en el sistema.
Se utilizó Homebrew ("brew install riscv-gnu-toolchain") para instalar la herramienta de compilación GNU para RISC-V.
Posteriormente, se modificó el archivo "Makefile" para asegurar el uso del compilador correcto durante el proceso de compilación.

Compilación y Pruebas de Funcionamiento:
Después de realizar los cambios mencionados, se ejecutó nuevamente el comando "make qemu".
Esta vez, la compilación se completó exitosamente sin errores.
Se ejecutaron los comandos: ls, echo "Hola xv6", y cat README, dentro de xv6 para verificar el correcto funcinamiento.
Todos los comandos funcionaron correctamente, confirmando que que xv6 se instaló y funcionó como se esperaba.