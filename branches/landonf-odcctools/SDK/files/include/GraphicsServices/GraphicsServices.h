#ifndef GRAPHICSSERVICES_H
#define GRAPHICSSERVICES_H

struct __GSEvent;
typedef struct __GSEvent GSEvent;
typedef GSEvent *GSEventRef;

struct CGPoint;
typedef struct CGPoint CGPoint;

int GSEventIsChordingHandEvent(GSEvent *ev);
int GSEventGetClickCount(GSEvent *ev);
CGPoint GSEventGetLocationInWindow(GSEvent *ev);
float GSEventGetDeltaX(GSEvent *ev); 
float GSEventGetDeltaY(GSEvent *ev); 
CGPoint GSEventGetInnerMostPathPosition(GSEvent *ev);
CGPoint GSEventGetOuterMostPathPosition(GSEvent *ev);

#endif

