/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@class NSString;

@interface UIAutocorrectTextView : UIView
{
    NSString *m_string;
    int m_type;
    int m_edgeType;
    struct __GSFont *m_textFont;
    BOOL m_animating;
}

- (void)dealloc;	// IMP=0x324a2270
- (void)drawRect:(struct CGRect)fp8;	// IMP=0x324a34c8
- (id)initWithFrame:(struct CGRect)fp8 string:(id)fp24 type:(int)fp28 edgeType:(int)fp32;	// IMP=0x324a2024
- (void)setAnimating:(BOOL)fp8;	// IMP=0x324a22d8
- (void)setEdgeType:(int)fp8;	// IMP=0x324a2bf8

@end

