#ifndef GRAPHICSSERVICES_H
#define GRAPHICSSERVICES_H
#import <UIKit/UIKit.h>

struct __GSEvent;
typedef struct __GSEvent GSEvent;
typedef GSEvent *GSEventRef;

struct CGPoint;
typedef struct CGPoint CGPoint;

int GSEventIsChordingHandEvent(GSEvent *ev);
int GSEventGetClickCount(GSEvent *ev);
CGPoint GSEventGetLocationInWindow(GSEvent *ev, UIWindow *window);
float GSEventGetDeltaX(GSEvent *ev); 
float GSEventGetDeltaY(GSEvent *ev); 
CGPoint GSEventGetInnerMostPathPosition(GSEvent *ev);
CGPoint GSEventGetOuterMostPathPosition(GSEvent *ev);

#endif

