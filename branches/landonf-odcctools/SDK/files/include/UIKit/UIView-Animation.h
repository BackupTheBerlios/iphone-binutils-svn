/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@interface UIView (Animation)
+ (void)beginAnimations:(id)fp8;	// IMP=0x323c5e2c
+ (void)beginAnimations:(id)fp8 context:(void *)fp12;	// IMP=0x323c5e4c
+ (void)disableAnimation;	// IMP=0x323c5f3c
+ (void)enableAnimation;	// IMP=0x323c5f54
+ (void)endAnimations;	// IMP=0x323c5e74
+ (void)setAnimationAutoreverses:(BOOL)fp8;	// IMP=0x323c600c
+ (void)setAnimationCurve:(int)fp8;	// IMP=0x323c5fdc
+ (void)setAnimationDelay:(double)fp8;	// IMP=0x323c5f88
+ (void)setAnimationDelegate:(id)fp8;	// IMP=0x323c5e9c
+ (void)setAnimationDidStopSelector:(SEL)fp8;	// IMP=0x323c60d8
+ (void)setAnimationDuration:(double)fp8;	// IMP=0x323c5f6c
+ (void)setAnimationFrameInterval:(double)fp8;	// IMP=0x323c5fa4
+ (void)setAnimationFromCurrentState:(BOOL)fp8;	// IMP=0x323c5ef4
+ (void)setAnimationPosition:(struct CGPoint)fp8;	// IMP=0x323c5f20
+ (void)setAnimationRepeatCount:(float)fp8;	// IMP=0x323c5ff4
+ (void)setAnimationRoundsToInteger:(BOOL)fp8;	// IMP=0x323c6038
+ (void)setAnimationStartTime:(double)fp8;	// IMP=0x323c5fc0
+ (void)setAnimationTransition:(int)fp8 forView:(id)fp12 cache:(BOOL)fp16;	// IMP=0x323c6064
+ (void)setAnimationWillStartSelector:(SEL)fp8;	// IMP=0x323c60c0
- (void)addAnimation:(id)fp8 forKey:(id)fp12;	// IMP=0x323c60f0
@end

