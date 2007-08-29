/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@class NSString, UINavigationBar;

@interface UINavBarPrompt : UIView
{
    NSString *_prompt;
    UINavigationBar *_navBar;
}

- (void)dealloc;	// IMP=0x323d66a8
- (void)drawRect:(struct CGRect)fp8;	// IMP=0x323d6834
- (id)initWithPrompt:(id)fp8 navBar:(id)fp12;	// IMP=0x323d6608
- (id)prompt;	// IMP=0x323d675c
- (struct CGRect)promptBounds;	// IMP=0x323d6764
- (void)setFrame:(struct CGRect)fp8;	// IMP=0x323d84a8
- (void)setPrompt:(id)fp8;	// IMP=0x323d6700

@end

