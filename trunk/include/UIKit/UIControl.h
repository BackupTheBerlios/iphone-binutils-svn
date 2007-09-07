#import <Foundation/Foundation.h>
#import <GraphicsServices/GraphicsServices.h>
#import <UIKit/UIView.h>

typedef enum {
    kUIControlEventMouseDown = 1 << 0,
    kUIControlEventMouseMovedInside = 1 << 2, // mouse moved inside control target
    kUIControlEventMouseMovedOutside = 1 << 3, // mouse moved outside control target
    kUIControlEventMouseUpInside = 1 << 6, // mouse up inside control target
    kUIControlEventMouseUpOutside = 1 << 7, // mouse up outside control target
    kUIControlAllEvents = (kUIControlEventMouseDown | kUIControlEventMouseMovedInside | kUIControlEventMouseMovedOutside | kUIControlEventMouseUpInside | kUIControlEventMouseUpOutside)
} UIControlEventMask;

@interface UIControl : UIView {}

- (BOOL)beginTrackingAt:(CGPoint)point withEvent:(GSEventRef)event;	
- (BOOL)cancelMouseTracking;	
- (BOOL)continueTrackingAt:(CGPoint)point previous:(CGPoint)previousPoint withEvent:(GSEventRef)event;
- (void)endTrackingAt:(CGPoint)point previous:(CGPoint)previousPoint withEvent:(GSEventRef)event;

- (BOOL)isTracking;
- (BOOL)shouldTrack;
- (void)setRequiresDisplayOnTracking:(BOOL)requiresDisplayOnTracking;

- (BOOL)mouseInside;

- (void)addTarget:(id)target action:(SEL)action forEvents:(UIControlEventMask)eventMask;
- (void)removeTarget:(id)target forEvents:(UIControlEventMask)eventMask;
- (void)sendAction:(SEL)action toTarget:(id)target forEvent:(GSEventRef)event;	
- (void)sendActionsForEvents:(UIControlEventMask)eventMask;
- (BOOL)hasOneOrMoreTargets;

- (BOOL)isHighlighted;
- (void)setHighlighted:(BOOL)highlighted;

- (void)_didMoveFromWindow:(id)window toWindow:(id)otherWindow;	

@end

@interface UIControl (Static)
- (void)_sendDelayedActions:(BOOL)sendDelayedActions;
- (void)_unhighlight;	
@end

@interface UIControl (Internal)
- (BOOL)pointMostlyInside:(CGPoint)point forEvent:(GSEventRef)event;
- (void)_cancelDelayedActions;
- (void)_delayActions;
- (BOOL)_hasActionForEventMask:(UIControlEventMask)eventMask;	
- (void)_sendActionsForEventMask:(UIControlEventMask)eventMask withEvent:(GSEventRef)event;	
- (void)_sendDelayedActions;	
- (void)_setHighlightOnMouseDown:(BOOL)highlightOnMouseDown;
@end
