#ifndef GRAPHICSSERVICES_H
#define GRAPHICSSERVICES_H

struct __GSEvent;
typedef struct __GSEvent GSEvent;

int GSEventIsChordingHandEvent(GSEvent *ev);
int GSEventGetClickCount(GSEvent *ev);

#endif

