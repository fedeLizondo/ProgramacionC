#ifndef UTILS_H
#define UTIlS_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD_START    "\033[1m"
#define ANSI_BOLD_END	   "\033[0m"

//Posicionar el cursor pone el cursor en la línea L, columna C.
#define  CONSOLE_goto(FILA,COLUMNA)  (\033[(FILA);(COLUMNA)H)         
// Mover el cursor arriba N líneas:
#define CONSOLE_upLine(X) (\033[(X)A)
// Mover el cursor abajo N líneas:
#define CONSOLE_downLine(X)  (\033[(X)B)
// Mover el cursor hacia adelante N columnas:
#define CONSOLE_rightLine(X) (\033[(X)C)
// Mover el cursor hacia atrás N columnas:
#define CONSOLE_leftLine(X)(\033[(X)D)
// Guardar la posición del cursor:
#define CONSOLE_save() (\033[s)
// Restaurar la posición del cursor:
#define CONSOLE_restore() (\033[u)


#endif
