/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@interface UIView (Gestures_Internal)
- (void)_animateToScrollPoint:(struct CGPoint)fp8;	// IMP=0x32432670
- (void)_animateZoomFailureToWindowPoint:(struct CGPoint)fp8 scale:(float)fp16 duration:(float)fp20;	// IMP=0x324327d4
- (struct CGPoint)_constrainedScrollPoint:(struct CGPoint)fp8 contentSize:(struct CGSize)fp16;	// IMP=0x32432cf4
- (void)_gestureEnded:(struct __GSEvent *)fp8;	// IMP=0x32432184
- (id)_gestureInfo;	// IMP=0x32432138
- (struct CGSize)_scrollerContentSize;	// IMP=0x32432238
- (float)_zoomAnimationDurationForScale:(float)fp8;	// IMP=0x32432520
- (float)_zoomAnimationProgress;	// IMP=0x32432884
- (void)_zoomToScrollPoint:(struct CGPoint)fp8 scale:(float)fp16 duration:(float)fp20 event:(struct __GSEvent *)fp24;	// IMP=0x32432318
- (void)_zoomToWindowPoint:(struct CGPoint)fp8 scale:(float)fp16 duration:(float)fp20 constrainScrollPoint:(BOOL)fp24 event:(struct __GSEvent *)fp28;	// IMP=0x324325b8
@end

