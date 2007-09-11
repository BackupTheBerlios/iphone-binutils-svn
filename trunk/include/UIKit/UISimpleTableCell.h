/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <Foundation/Foundation.h>
#import <GraphicsServices/GraphicsServices.h>
#import <UIKit/UITableCell.h>
#import <UIKit/UITextLabel.h>

@class UIImageView;

@interface UISimpleTableCell : UITableCell {	 
    GSFontRef _font;
    UIImageView *_iconImageView;	 
    unsigned int _indentationLevel;
    NSString *_title;
    int _titleColor;	 
}

+ (GSFontRef)defaultFont;

- (id)initWithFrame:(CGRect)frame;

- (void)drawContentInRect:(CGRect)rect selected:(BOOL)selected;	
- (void)drawTitleInRect:(CGRect)rect selected:(BOOL)selected;

- (UITextLabelEllipsisStyle)ellipsisStyle;

- (id)icon;
- (void)setIcon:(id)icon;
- (id)iconImageView;	

- (void)layoutSubviews;	

- (void)setEnabled:(BOOL)enabled;

- (GSFontRef)font;
- (void)setFont:(GSFontRef)font;

- (unsigned int)indentationLevel;
- (void)setIndentationLevel:(unsigned int)indentationLevel;

- (id)title;
- (void)setTitle:(id)title;
- (void)setTitleColor:(int)titleColor; // enum needed

@end

