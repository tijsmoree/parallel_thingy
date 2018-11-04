/* Kees Lemmens; January 2002:

   This part: main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <mpi.h>

#define DEBUGLEVEL 1

extern int master(long dim);
extern int slave(void);

void debug(char *format,...)
{  
#if (DEBUGLEVEL > 0)
   va_list p;

   va_start(p,format);
   vfprintf(stderr,format, p);
   va_end(p);

#else
   ;
#endif
}

int main(int argc, char *argv[])
{
   int procid;
   int exitstatus;
   long dim = 16L;       // Number of elements in arrays (problemsize)
   
   MPI_Init(&argc, &argv);
   
   if(argc >=2 )
     sscanf(argv[1],"%ld",(long *)&dim);
   
   MPI_Comm_rank(MPI_COMM_WORLD, &procid);  // get our process nr
   
   switch(procid)
   {
    case 0 : // Master
      debug("Starting master on %d ...\n",procid);
      exitstatus = master(dim);
      break;
    default: // Slave
      debug("Starting slave on  %d ...\n",procid);
      exitstatus = slave();
      break;
   }
   
   MPI_Finalize();
   exit(exitstatus);
}

