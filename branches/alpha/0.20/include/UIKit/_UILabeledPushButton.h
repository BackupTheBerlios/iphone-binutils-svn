/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIThreePartButton.h>

@class UITextLabel;

@interface _UILabeledPushButton : UIThreePartButton
{
    UITextLabel *_textLabel;
}

- (void)dealloc;	// IMP=0x3241599c
- (void)drawTitleAtPoint:(struct CGPoint)fp8 width:(float)fp16;	// IMP=0x32415a54
- (void)layoutSubviews;	// IMP=0x324172c0
- (void)setHighlighted:(BOOL)fp8;	// IMP=0x324159f4
- (void)setLabel:(id)fp8;	// IMP=0x32417574

@end

