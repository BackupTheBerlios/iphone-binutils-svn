#ifndef GRAPHICSSERVICES_H
#define GRAPHICSSERVICES_H

#import <CoreGraphics/CoreGraphics.h>

#ifdef __cplusplus
extern "C" {
#endif

// events

enum {
    kGSEventTypeOneFingerDown = 1,
    kGSEventTypeAllFingersUp = 2,
    kGSEventTypeOneFingerUp = 5,
    /* A "gesture" is either one finger dragging or two fingers down. */
    kGSEventTypeGesture = 6
} GSEventType;

struct __GSEvent;
typedef struct __GSEvent GSEvent;
typedef GSEvent *GSEventRef;

int GSEventIsChordingHandEvent(GSEvent *ev);
int GSEventGetClickCount(GSEvent *ev);
CGPoint GSEventGetLocationInWindow(GSEvent *ev);
float GSEventGetDeltaX(GSEvent *ev); 
float GSEventGetDeltaY(GSEvent *ev); 
CGPoint GSEventGetInnerMostPathPosition(GSEvent *ev);
CGPoint GSEventGetOuterMostPathPosition(GSEvent *ev);
unsigned int GSEventGetSubType(GSEvent *ev);
unsigned int GSEventGetType(GSEvent *ev);

// fonts

typedef enum {
    kGSFontTraitItalic = 1,
    kGSFontTraitBold = 2,
    kGSFontTraitBoldItalic = (kGSFontTraitBold | kGSFontTraitItalic)
} GSFontTrait;

struct __GSFont;
typedef struct __GSFont *GSFontRef;

GSFontRef GSFontCreateWithName(char *name, GSFontTrait traits, float size);
char *GSFontGetFamilyName(GSFontRef font);
char *GSFontGetFullName(GSFontRef font);
BOOL GSFontIsBold(GSFontRef font);
BOOL GSFontIsFixedPitch(GSFontRef font);
GSFontTrait GSFontGetTraits(GSFontRef font);

#ifdef __cplusplus
}
#endif

#endif

