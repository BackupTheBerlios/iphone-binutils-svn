/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import "NSObject.h"

@class NSString, UIView;

@interface UIViewAnimationState : NSObject
{
    UIViewAnimationState *_nextState;
    NSString *_animationID;
    void *_context;
    id _delegate;
    double _duration;
    double _delay;
    double _frameInterval;
    double _start;
    int _curve;
    float _repeatCount;
    int _transition;
    UIView *_transitionView;
    SEL _willStartSelector;
    SEL _didEndSelector;
    int _didEndCount;
    struct CGPoint _position;
    unsigned int _willStartSent:1;
    unsigned int _useCurrentLayerState:1;
    unsigned int _cacheTransition:1;
    unsigned int _autoreverses:1;
    unsigned int _roundsToInteger:1;
    unsigned int _reserved:27;
}

+ (void)popAnimationState;	// IMP=0x323c2b4c
+ (void)pushViewAnimationState:(id)fp8 context:(void *)fp12;	// IMP=0x323c2a1c
- (void)animationDidStart:(id)fp8;	// IMP=0x323c3288
- (void)animationDidStop:(id)fp8 finished:(BOOL)fp12;	// IMP=0x323c3554
- (void)dealloc;	// IMP=0x323c3024
- (void)sendDelegateAnimationDidStop:(id)fp8 finished:(BOOL)fp12;	// IMP=0x323c3390
- (void)setAnimationAttributes:(id)fp8;	// IMP=0x323c308c

@end

