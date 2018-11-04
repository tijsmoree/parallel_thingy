/* HRGRAPH.H    :  Grafische header file voor TEKTRONIX 4014 (emulaties) */
/* Kees Lemmens :  Juli 1991   , 1e toepassing voor ATARI ST             */

/* Modified April KL 2002: synchronized with Java graphic library
 * (Color numbers exchanged)
 */

 #ifndef __HRGRAPH__
#define __HRGRAPH__

/* define a few very common colors as a MACRO (using RGB) */

#define WHITE   0xffffff
#define BLACK   0x000000
#define RED     0xff0000
#define GREEN   0x00ff00
#define BLUE    0x0000ff
#define YELLOW  0xffff00
#define GREY    0x807080
#define CYAN    0x00ffff
#define MAGENTA 0xff00ff
#define ORANGE  0xff8700 

/* next definitions are used to set object colors, line sand styles */

#define BACKGR '1'
#define DOT    '2'
#define LINE   '3'
#define CIRCLE '4'
#define TEXT   '5'

#define SOLID      'a'
#define DOTTED     'b'
#define DOTDASHED  'c'
#define DASHED     'd'
#define LONGDASHED 'e'

/* for a High Resolution Screen */
/*
#define XMAX 1279
#define YMAX 1023
#define XRES 1280
#define YRES 1024
*/

/* for a Medium Resolution Screen */

#define XMAX 1023
#define YMAX 767
#define XRES 1024
#define YRES 768

void graphic(void);
void endgraph(void);
void graphon(void);
void graphoff(void);
void clrmap(void);
void linestyle(int style,int thickness);
void setwinscale(double size);
void setcol(int objecttype,long rgbcolor);
void plot(int x,int y);
void setplot(int x,int y);  /* dark vector */	
void dot(int x,int y);
void line(int x1,int y1,int x2,int y2);
void circle(int x,int y,int r);
void text(int x,int y,char *string,int height,int width,double angle);
void getaspectratio(int *x,int *y);

#endif
