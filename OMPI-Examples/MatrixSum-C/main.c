/* Kees Lemmens; January 2002:

  Last mod Oct 2012

  This part: main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <mpi.h>

#include "main.h"

void ldebug(int level, char *format, ...)
{
   va_list p;
   
   if (DEBUGLEVEL < level)
     return;
   va_start(p,format);
   vfprintf(stderr,format, p);
   va_end(p);
}

int main(int argc, char *argv[])
{
   int procid;
   int dim = N;
   int exitstatus;
   
   MPI_Init(&argc, &argv);

   if(argc >=2 )
     sscanf(argv[1],"%d",&dim);
   
   MPI_Comm_rank(MPI_COMM_WORLD, &procid);  // get our process nr
   
   switch(procid)
   {
    case 0 : // Master
      ldebug(1,"Starting master on %d ...\n",procid);
      exitstatus = master(dim);
      break;
    default: // Slave
      ldebug(1,"Starting slave on  %d ...\n",procid);
      exitstatus = slave(dim);
      break;
   }
   
   MPI_Finalize();
   exit(exitstatus);
}

