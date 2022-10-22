
Trabajo realizado únicamente por David Cuervo Pallin.

En este archivo se describirá el comportamiento del programa, así como sus peculiaridades.

Para empezar, explicaré la distribución del código a lo largo de los archivos de la práctica. Todas las peticiones que recibe el shell son recibidas en 'p1.c', donde se actualiza el historial de comandos escritos, se procesa el comando y se llama a 'proc', la cual se encuentra en el archivo 'process_shell.c'. Este archivo se encargará, en lineas generales, de atender al comando ya desglosado en en el main, llamando a las funciones oportunas. Además en este archivo se encuentran las funcionalidades de 'hist','comando' y 'ayuda'. Decidí separar estas funciones del resto de comandos de la p0 ya que estas trabajan con el histórico o la lista de cmds (la cual se encuentra definida en este archivo 'process_shell'), mientras que el resto de comandos no. Desde este archivo y como bien ya mencioné antes, se llamará al resto de funciones disponibles en el shell.


El resto de funciones de este shell se encuentran en el archivo 'comandos_p0.c' (donde se definen las funcionalidades de la p0, menos las indicadas anteriormente) y 'comandos_p1.c' (todas las funciones pedidas en la p1). Dichas funciones serán llamadas para su ejecución desde 'process_shell'. Es importante mencionar el archivo 'ayudaP1.c', el cual es suministrado por vosotros para obtener los permisos de un archivo en forma de char*. Además,vemos los archivos 'list.c' y 'list.h' que implementan la lista con la que se realizó el histórico, así como funciones para recorrerla. Por último, resalta el uso de un 'Makefile' con el que se podrá ejecutar el shell. 


Las peculiaridades que se encuentran en este programa son las siguientes:
- Para los comandos 'list' y 'stat' se usaron funciones auxiliares. Las funciones 'list_cmd' y 'stat_cmd' recogen el comando y obtienen los diferentes flags de comando ('-long','-hid','-reca'...), para luego pasarlo a las funciones auxiliares 'stat_aux', 'list_aux_a' y 'list_aux_b'. Dichas funciones obtienen los flags por parámetro en forma de boolean, uno para cada flag.  Realicé esta implementación ya que era la forma más sencilla de detectar estos flags, consecuencia de que no haya un orden específico en el que se declaran estos.

- Las funciones 'comando' y 'hist' no se encuentran definido en la lista de comandos ('cmds'), ya que necesitan el histórico de comandos por parámetro. Esto las diferencia del resto de funciones y por ello están excluídas.

- La mayoría de funciones devuelven un 0, salida estandar que no tiene consecuencias en el proceso del shell. Sin embargo funciones como 'fin', 'bye' y 'salir' devuelven un valor -1, el cual indica el fin de ejecución del shell en 'p1.c'. Además, la función 'hist -c' devuelve un valor especial (Vaciar_List). Provocará vaciar el histórico en 'shell_main.c', donde se encuentra definido.
