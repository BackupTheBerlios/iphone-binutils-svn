#ifndef GRAPHICSSERVICES_H
#define GRAPHICSSERVICES_H

#import <CoreGraphics/CoreGraphics.h>

#ifdef __cplusplus
extern "C" {
#endif

// make sure to CFRelease(objectref) or [(id)objectref autorelease] the result of all GS...Create* methods to prevent leaking memory

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
int GSEventDeviceOrientation(GSEvent *ev);

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

// colors

CGColorRef GSColorCreate(CGColorSpaceRef colorspace, const float components[]);
CGColorRef GSColorCreateBlendedColorWithFraction(CGColorRef color, CGColorRef blendedColor, float fraction);
CGColorRef GSColorCreateColorWithDeviceRGBA(float red, float green, float blue, float alpha);
CGColorRef GSColorCreateColorWithDeviceWhite(float white, float alpha);
CGColorRef GSColorCreateHighlightWithLevel(CGColorRef originalColor, float highlightLevel);
CGColorRef GSColorCreateShadowWithLevel(CGColorRef originalColor, float shadowLevel);

float GSColorGetRedComponent(CGColorRef color);
float GSColorGetGreenComponent(CGColorRef color);
float GSColorGetBlueComponent(CGColorRef color);
float GSColorGetAlphaComponent(CGColorRef color);
const float *GSColorGetRGBAComponents(CGColorRef color);

// sets the color for the current context given by UICurrentContext()
void GSColorSetColor(CGColorRef color);
void GSColorSetSystemColor(CGColorRef color);

#ifdef __cplusplus
}
#endif

#endif

