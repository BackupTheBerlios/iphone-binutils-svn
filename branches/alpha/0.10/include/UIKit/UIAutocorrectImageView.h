/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@class UIImage;

@interface UIAutocorrectImageView : UIView
{
    UIImage *m_image;
}

+ (id)imageWithMaskingView:(id)fp8 maskingRect:(struct CGRect)fp12;	// IMP=0x324a3d74
- (void)dealloc;	// IMP=0x324a4050
- (void)drawRect:(struct CGRect)fp8;	// IMP=0x324a40a8
- (id)initWithFrame:(struct CGRect)fp8 image:(id)fp24;	// IMP=0x324a3e1c
- (id)initWithFrame:(struct CGRect)fp8 string:(id)fp24 font:(struct __GSFont *)fp28;	// IMP=0x324a3ec0

@end

