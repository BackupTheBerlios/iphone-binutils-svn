/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@class NSMutableArray;

@interface UICompositeImageView : UIView
{
    NSMutableArray *m_images;
}

- (void)addImage:(id)fp8;	// IMP=0x3245d0c0
- (void)addImage:(id)fp8 operation:(int)fp12 fraction:(float)fp16;	// IMP=0x3245d334
- (void)addImage:(id)fp8 toRect:(struct CGRect)fp12 fromRect:(struct CGRect)fp28;	// IMP=0x3245d170
- (void)addImage:(id)fp8 toRect:(struct CGRect)fp12 fromRect:(struct CGRect)fp28 operation:(int)fp44 fraction:(float)fp48;	// IMP=0x3245d1fc
- (void)dealloc;	// IMP=0x3245d068
- (void)drawRect:(struct CGRect)fp8;	// IMP=0x3245d2f4
- (id)initWithFrame:(struct CGRect)fp8;	// IMP=0x3245cfc4
- (void)removeAllImages;	// IMP=0x3245d2bc

@end

