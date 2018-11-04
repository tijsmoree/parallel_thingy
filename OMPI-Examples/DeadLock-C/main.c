/* Kees Lemmens; January 2002:

   This part: main.c
 */

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

extern int master(int argc, char *argv[]);
extern int slave(void);

void debug(char *format,...)
{  
#if (DEBUGLEVEL > 0)
   va_list p;

   va_start(p,format);
   vprintf(format, p);
   va_end(p);

#else
   ;
#endif
}

int main(int argc, char *argv[])
{
   int procid;
   int exitstatus;
   
   MPI_Init(&argc, &argv);
   
   MPI_Comm_rank(MPI_COMM_WORLD, &procid);  // get our process nr
   
   switch(procid)
   {
    case 0 : // Master
      debug("Starting master on %d ...\n",procid);
      exitstatus = master(argc,argv);
      break;
    default: // Slave
      debug("Starting slave on  %d ...\n",procid);
      exitstatus = slave();
      break;
   }
   
   MPI_Finalize();
   exit(exitstatus);
}

