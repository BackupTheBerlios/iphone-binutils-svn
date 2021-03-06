/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIView.h>

@interface UIViewTapInfo : NSObject
{
    id _delegate;
    UIView *_view;
    int _tapDownCount;
    int _fingerCount;
    float _multiTapDelay;
    float _rejectAsTapThrehold;
    float _viewTouchPauseThreshold;
    struct CGPoint _startPosition;
    double _startTime;
    struct {
        unsigned int shouldSendTouchPauseUp:1;
        unsigned int delegateViewHandleTapWithCountEvent:1;
        unsigned int delegateViewHandleTapWithCountEventFingerCount:1;
        unsigned int delegateViewHandleTouchPauseIsDown:1;
        unsigned int reserved:28;
    } _tapInfoFlags;
}

- (BOOL)_eventLocationConsideredMovement:(struct __GSEvent *)fp8;	// IMP=0x323c7b3c
- (void)_handleTapWithCount:(int)fp8 event:(struct __GSEvent *)fp12;	// IMP=0x323c772c
- (void)_sendTouchPauseDownIfNecessary;	// IMP=0x323c7958
- (void)_sendTouchPauseUpIfNecessary;	// IMP=0x323c79b4
- (BOOL)cancelMouseTracking;	// IMP=0x323c7d90
- (void)clearTapState;	// IMP=0x323c753c
- (void)handleDoubleTapEvent:(struct __GSEvent *)fp8;	// IMP=0x323c7814
- (void)handleSingleTapEvent:(struct __GSEvent *)fp8;	// IMP=0x323c77f0
- (id)initWithDelegate:(id)fp8 view:(id)fp12;	// IMP=0x323c7480
- (void)mouseDown:(struct __GSEvent *)fp8;	// IMP=0x323c79f4
- (void)mouseDragged:(struct __GSEvent *)fp8;	// IMP=0x323c7bbc
- (void)mouseUp:(struct __GSEvent *)fp8;	// IMP=0x323c7c3c
- (void)releaseAndClearWeakRefs;	// IMP=0x323c74ec
- (void)scheduleSingleTapHandlerForEvent:(struct __GSEvent *)fp8;	// IMP=0x323c7838
- (void)setDelegate:(id)fp8;	// IMP=0x323c758c

@end

