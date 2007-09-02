/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@interface UIView (Hierarchy)
- (id)_findFirstSubviewWantingToBecomeFirstResponder;	// IMP=0x323c6968
- (void)_makeSubtreePerformSelector:(SEL)fp8;	// IMP=0x323c6a74
- (void)_movedToFront;	// IMP=0x323c4f58
- (void)_postMovedFromSuperview:(id)fp8;	// IMP=0x323c8cd0
- (void)_promoteDescendantToFirstResponderIfNecessary;	// IMP=0x323c53fc
- (BOOL)_shouldTryPromoteDescendantToFirstResponder;	// IMP=0x323c53f4
- (void)addSubview:(id)fp8;	// IMP=0x323c4e1c
- (void)bringSubviewToFront:(id)fp8;	// IMP=0x323c4f5c
- (BOOL)containsView:(id)fp8;	// IMP=0x323c7438
- (void)exchangeSubviewAtIndex:(unsigned int)fp8 withSubviewAtIndex:(unsigned int)fp12;	// IMP=0x323c4d50
- (void)insertSubview:(id)fp8 above:(id)fp12;	// IMP=0x323c52a8
- (void)insertSubview:(id)fp8 atIndex:(unsigned int)fp12;	// IMP=0x323c4c0c
- (void)insertSubview:(id)fp8 below:(id)fp12;	// IMP=0x323c515c
- (void)layoutIfNeeded;	// IMP=0x323c5498
- (void)layoutSubviews;	// IMP=0x323c54b8
- (void)movedFromSuperview:(id)fp8;	// IMP=0x323c5458
- (void)movedFromWindow:(id)fp8;	// IMP=0x323c5460
- (void)movedToSuperview:(id)fp8;	// IMP=0x323c545c
- (void)movedToWindow:(id)fp8;	// IMP=0x323c5464
- (void)removeFromSuperview;	// IMP=0x323c4af4
- (void)sendSubviewToBack:(id)fp8;	// IMP=0x323c5070
- (void)setNeedsLayout;	// IMP=0x323c5478
- (void)setTag:(int)fp8;	// IMP=0x323c5468
- (id)subviews;	// IMP=0x323c67b0
- (id)superview;	// IMP=0x323c8e38
- (int)tag;	// IMP=0x323c5470
- (id)viewWithTag:(int)fp8;	// IMP=0x323c6b34
@end

