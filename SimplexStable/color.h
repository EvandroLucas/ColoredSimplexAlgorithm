#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#ifndef COLOR_H
#define COLOR_H

/*COLOR.H versão 2.2
Nessa biblioteca, você pode:
 1 - Invocar qualquer funcão do tipo "printf_cor",
onde cor está em ingles,
para imprimir o fundo em cores no terminal.
 2 - Invocar qualquer funcão do tipo "printf_super_cor",
onde cor está em ingles,
para imprimir o background em cores no terminal.
 3 - Chamar a função "rainbow()", que imprime um arco-iris,
 util paa calibrar as cores do seu terminal

 Essas funcoes sao todas modificacoes da funcao printf e funcionam
 da mesma forma: inserem as tags de abertura e fechamento para cores.

 OBS: em caso de erro na execução de seu programa, existe a possivbilidade
 de que todo o output assuma a mesma cor. Entao evite errar!

*/

#define RED     "\x1b[91m"
#define GREEN   "\x1b[92m"
#define YELLOW  "\x1b[93m"
#define BLUE    "\x1b[94m"
#define MAGENTA "\x1b[95m"
#define CYAN    "\x1b[96m"
#define WHITE   "\x1b[97m"
#define BLINK   "\x1b[5m"
#define RESET   "\x1b[0m"

#define SUPER_RED     "\x1b[41m"
#define SUPER_GREEN   "\x1b[42m"
#define SUPER_YELLOW  "\x1b[43m"
#define SUPER_BLUE    "\x1b[44m"
#define SUPER_MAGENTA "\x1b[45m"
#define SUPER_CYAN    "\x1b[46m"
#define SUPER_WHITE   "\x1b[47m"


int printf_red (const char *format, ...){
   printf("%s",RED);
   va_list arg;
   int done;

   va_start (arg, format);
   done = vfprintf (stdout, format, arg);
   va_end (arg);
   printf("%s",RESET);

   return done;
}

int printf_green (const char *format, ...){
   printf("%s",GREEN);
   va_list arg;
   int done;

   va_start (arg, format);
   done = vfprintf (stdout, format, arg);
   va_end (arg);
   printf("%s",RESET);

   return done;
}

int printf_yellow (const char *format, ...){
   printf("%s",YELLOW);
   va_list arg;
   int done;

   va_start (arg, format);
   done = vfprintf (stdout, format, arg);
   va_end (arg);
   printf("%s",RESET);

   return done;
}

int printf_blue (const char *format, ...){
   printf("%s",BLUE);
   va_list arg;
   int done;

   va_start (arg, format);
   done = vfprintf (stdout, format, arg);
   va_end (arg);
   printf("%s",RESET);

   return done;
}

int printf_magenta (const char *format, ...){
   printf("%s",MAGENTA);
   va_list arg;
   int done;

   va_start (arg, format);
   done = vfprintf (stdout, format, arg);
   va_end (arg);
   printf("%s",RESET);

   return done;
}

int printf_cyan (const char *format, ...){
   printf("%s",CYAN);
   va_list arg;
   int done;

   va_start (arg, format);
   done = vfprintf (stdout, format, arg);
   va_end (arg);
   printf("%s",RESET);

   return done;
}

int printf_white (const char *format, ...){
    printf("%s",WHITE);
    va_list arg;
    int done;
 
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    printf("%s",RESET);
 
    return done;
 }
 
 int printf_blink (const char *format, ...){
    printf("%s",BLINK);
    va_list arg;
    int done;
 
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    printf("%s",RESET);
 
    return done;
 }

int printf_super_red (const char *format, ...){
    printf("%s",SUPER_RED);
    va_list arg;
    int done;
 
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    printf("%s",RESET);
    return done;
 }
 
 int printf_super_green (const char *format, ...){
    printf("%s",SUPER_GREEN);
    va_list arg;
    int done;
 
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    printf("%s",RESET);
 
    return done;
 }
 
 int printf_super_yellow (const char *format, ...){
    printf("%s",SUPER_YELLOW);
    va_list arg;
    int done;
 
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    printf("%s",RESET);
 
    return done;
 }
 
 int printf_super_blue (const char *format, ...){
    printf("%s",SUPER_BLUE);
    va_list arg;
    int done;
 
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    printf("%s",RESET);
    printf("%s",RESET);
    return done;
 }
 
 int printf_super_magenta (const char *format, ...){
    printf("%s",SUPER_MAGENTA);
    va_list arg;
    int done;
 
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    printf("%s",RESET);
 
    return done;
 }
 
 int printf_super_cyan (const char *format, ...){
    printf("%s",SUPER_CYAN);
    va_list arg;
    int done;
 
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    printf("%s",RESET);
 
    return done;
 }

 int printf_super_white (const char *format, ...){
    printf("%s",SUPER_WHITE);
    va_list arg;
    int done;
 
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    printf("%s",RESET);
 
    return done;
 }

 void rainbow(){
    printf("\n\n\n");
    printf_super_red(" RED                                                           \n");
    printf_super_red(" RED                                                           \n");
    printf_super_yellow(" YELLOW                                                        \n");
    printf_super_yellow(" YELLOW                                                        \n");
    printf_super_green(" GREEN                                                         \n");
    printf_super_green(" GREEN                                                         \n");
    printf_super_cyan(" CYAN                                                          \n");
    printf_super_cyan(" CYAN                                                          \n");
    printf_super_blue(" BLUE                                                          \n");
    printf_super_blue(" BLUE                                                          \n");
    printf_super_magenta(" MAGENTA                                                       \n");
    printf_super_magenta(" MAGENTA                                                       \n");
    printf_super_white(" WHITE                                                         \n");
    printf_super_white(" WHITE                                                         \n");
    printf("\n");
    //melhor tema: argonaut

    return;
 }


 #endif