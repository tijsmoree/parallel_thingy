/* JGRAPH.H     :  Grafische header file voor TEKTRONIX 4014 (emulaties)  */
/* Kees Lemmens :  Juli 1991   , 1e toepassing voor ATARI ST              */
/* Modified April 2002 : Adapted for Java + Swing, now use floating point */

#ifndef __JGRAPH__
#define __JGRAPH__

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

/* next definitions are used to set object colors, fonts and style */

#define BACKGRCOL  '1'
#define DOTCOL     '2'
#define LINECOL    '3'
#define CIRCLECOL  '4'
#define TEXTCOL    '5'

#define SOLID      'a'
#define DOTTED     'b'
#define DOTDASHED  'c'
#define DASHED     'd'
#define LONGDASHED 'e'

#define TEXTPLAIN  'f'
#define TEXTBOLD   'g'

// unchanged routines :
 
void graphon(void);
void graphoff(void);
void clrmap(void);

// new routines in the Java client :

void jgraphic(void);
void endjgraphic(void);
void setdatarange(double min, double max);
void setfont(int type,int size);
void setstyle(int style,int thickness);
void setcolor(int objecttype,long rgbcolor);
void jplot(double x,double y);
void jsetplot(double x,double y);  /* dark vector */	
void jdot(double x,double y);
void jline(double x1,double y1,double x2,double y2);
void jcircle(double x,double y,double r);
void jtext(double x,double y,char *string);

#endif
