#include <err.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <string.h>

       int
       main(void)
       {
           char    *p;
           char    buf1[20];
           char    buf2[20];
           size_t  len;

           if (sizeof(buf1) < strlen("Hello world!"))
               warnx("stpncpy: truncating character sequence");
           p = stpncpy(buf1, "Hello world!", sizeof(buf1));
           len = p - buf1;

           printf("[len = %zu]: ", len);
           printf("%.*s\n", (int) len, buf1);  // "Hello world!"

           if (sizeof(buf2) < strlen("Hello world!"))
               warnx("strncpy: truncating character sequence");
           strncpy(buf2, "Hello world!", sizeof(buf2));
           len = strnlen(buf2, sizeof(buf2));

           printf("[len = %zu]: ", len);
           printf("%.*s\n", (int) len, buf2);  // "Hello world!"

           exit(EXIT_SUCCESS);
       }