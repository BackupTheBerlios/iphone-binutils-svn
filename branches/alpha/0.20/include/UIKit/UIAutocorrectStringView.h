/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@class NSString;

@interface UIAutocorrectStringView : UIView
{
    NSString *m_string;
    struct __GSFont *m_font;
}

- (void)dealloc;	// IMP=0x324a3c84
- (void)drawRect:(struct CGRect)fp8;	// IMP=0x324a3cdc
- (id)initWithFrame:(struct CGRect)fp8 string:(id)fp24 font:(struct __GSFont *)fp28;	// IMP=0x324a3bdc

@end

