#include<XmAxy/Util.h>

void axyRectanglesIntersection(XRectangle* r1,XRectangle* r2,XRectangle* r){
  XRectangle t;
  int i;
  t.x=Max(r1->x,r2->x);
  t.y=Max(r1->y,r2->y);
  i=(int)Min(r1->x+r1->width,r2->x+r2->width)-(int)t.x;
  if(i<=0) t.width=0;
  else t.width=i;
  i=(int)Min(r1->y+r1->height,r2->y+r2->height)-(int)t.y;
  if(i<=0) t.height=0;
  else t.height=i;
  r->x=t.x;r->y=t.y;r->width=t.width;r->height=t.height;
}

int axyRectangleIsEmpty(XRectangle* r){
  if((r->width==0) || (r->height==0)) return 1;
  else return 0;
}

int axyRectanglesIntersect(XRectangle* r1,XRectangle* r2){
  if(Max(r1->x,r2->x)<Min(r1->x+r1->width,r2->x+r2->width) &&
     Max(r1->y,r2->y)<Min(r1->y+r1->height,r2->y+r2->height) )
    return 1;
  else return 0;
}

