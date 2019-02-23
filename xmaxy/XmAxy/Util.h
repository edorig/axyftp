#include<X11/Xlib.h>

#ifndef Max
#define Max(x,y)		 ((x)>(y)?(x):(y))
#endif

#ifndef Min
#define Min(x,y)		 ((x)<(y)?(x):(y))
#endif

#ifndef Abs
#define Abs(x) ((x)>0?(x):(-(x)))
#endif


void axyRectanglesIntersection(XRectangle*,XRectangle*,XRectangle*);
int axyRectangleIsEmpty(XRectangle*);
int axyRectanglesIntersect(XRectangle*,XRectangle*);

