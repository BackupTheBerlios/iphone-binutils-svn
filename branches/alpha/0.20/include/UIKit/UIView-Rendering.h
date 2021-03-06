/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@interface UIView (Rendering)
+ (void)flush;	// IMP=0x323c5e04
- (void)_enableLayerKitPatternDrawing:(BOOL)fp8;	// IMP=0x323c5dec
- (float)alpha;	// IMP=0x323c5a10
- (struct CGColor *)backgroundColor;	// IMP=0x323c5960
- (BOOL)clipsSubviews;	// IMP=0x323c55f4
- (int)controlTint;	// IMP=0x323c5ce4
- (struct CGImage *)createSnapshotWithRect:(struct CGRect)fp8;	// IMP=0x323c7fe0
- (void)drawRect:(struct CGRect)fp8;	// IMP=0x323c54bc
- (void)forceDisplayIfNeeded;	// IMP=0x323c5d30
- (BOOL)isOpaque;	// IMP=0x323c5ae8
- (BOOL)needsDisplay;	// IMP=0x323c5564
- (BOOL)needsDisplayOnBoundsChange;	// IMP=0x323c5588
- (void)recursivelyForceDisplayIfNeeded;	// IMP=0x323c5d50
- (void)setAlpha:(float)fp8;	// IMP=0x323c59f0
- (void)setBackgroundColor:(struct CGColor *)fp8;	// IMP=0x323c5650
- (void)setClearsContext:(BOOL)fp8;	// IMP=0x323c5af8
- (void)setClipsSubviews:(BOOL)fp8;	// IMP=0x323c55d0
- (void)setContentsPosition:(int)fp8;	// IMP=0x323c5b1c
- (void)setControlTint:(int)fp8;	// IMP=0x323c5d10
- (void)setFixedBackgroundPattern:(BOOL)fp8;	// IMP=0x323c59d8
- (void)setNeedsDisplay;	// IMP=0x323c5508
- (void)setNeedsDisplayInRect:(struct CGRect)fp8;	// IMP=0x323c7e2c
- (void)setNeedsDisplayOnBoundsChange:(BOOL)fp8;	// IMP=0x323c55ac
- (void)setOpaque:(BOOL)fp8;	// IMP=0x323c5a30
- (struct CGRect)visibleBounds;	// IMP=0x323c54cc
@end

