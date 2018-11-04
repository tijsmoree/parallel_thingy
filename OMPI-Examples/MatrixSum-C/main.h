
// Test 1:
#define DEBUGLEVEL 1
#define N          4 // matrix dimension

// Test 2:
//#define DEBUGLEVEL 1
//#define N          8 // matrix dimension

// Test 3:
//#define DEBUGLEVEL 0
//#define N          512 // matrix dimension

#ifndef DEBUGLEVEL
# define DEBUGLEVEL 0
#endif

extern void ldebug(int level,char *format,...);

extern double **initMatrix(int n, int m);
extern void showMatrix(char *name, double **a, int n, int m);

extern int master(int dim);
extern int slave(int dim);
