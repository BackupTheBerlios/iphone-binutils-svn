/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIControl.h>

#import "UITextTraitsClientProtocol.h"

@class NSString, UIImageView, UIPushButton, UITextFieldBackground, UITextFieldLabel, UITextLabel, UITextTraits;

@interface UITextField : UIControl <UITextTraitsClient>
{
    struct __GSFont *_font;
    float _fullFontSize;
    float _paddingLeft;
    float _paddingTop;
    float _paddingRight;
    float _paddingBottom;
    float _marginTop;
    NSString *_text;
    NSString *_textFont;
    struct CGColor *_textColor;
    struct CGColor *_caretColor;
    struct _NSRange _selectionRange;
    int _scrollXOffset;
    int _scrollYOffset;
    NSString *_placeholder;
    float _progress;
    UITextTraits *_traits;
    struct {
        unsigned int borderStyle:2;
        unsigned int isEditing:1;
        unsigned int secureTextChanged:1;
        unsigned int guard:1;
        unsigned int delegateRespondsToHandleKeyDown:1;
        unsigned int autosizeTextToFit:1;
        unsigned int verticallyCenterText:1;
        unsigned int horizontallyCenterText:1;
        unsigned int isAnimating:4;
        unsigned int inactiveHasDimAppearance:1;
    } _textFieldFlags;
    NSString *_style;
    NSString *_placeholderStyle;
    id _delegate;
    int _selectionBehavior;
    int _clearButtonStyle;
    UIPushButton *_clearButton;
    struct CGSize _clearButtonOffset;
    int _rightButtonStyle;
    UIPushButton *_rightButton;
    struct CGSize _rightButtonOffset;
    double _mouseDownTime;
    UITextFieldLabel *_textLabel;
    UIImageView *_iconView;
    UITextFieldBackground *_backgroundView;
    UITextLabel *_label;
    float _labelOffset;
}

- (void)_clearButtonClicked:(id)fp8;	// IMP=0x323f579c
- (void)_computeStyleForPlaceholder:(BOOL)fp8;	// IMP=0x323f5dd8
- (struct __GSFont *)_copyFont:(struct __GSFont *)fp8 newSize:(float)fp12 maxSize:(float)fp16;	// IMP=0x323f29dc
- (void)_endedEditing;	// IMP=0x323f67f0
- (void)_initializeTraits;	// IMP=0x323f270c
- (void)_invalidateTextLabel;	// IMP=0x323f64b4
- (id)_placeholderText;	// IMP=0x323f64ac
- (void)_releaseStyles;	// IMP=0x323f5608
- (id)_rightButton;	// IMP=0x323f5bfc
- (void)_rightButtonClicked:(id)fp8;	// IMP=0x323f5bb0
- (struct CGRect)_rightButtonRect;	// IMP=0x323f7474
- (BOOL)_sendInitialMouseEvents;	// IMP=0x323f4870
- (BOOL)_shouldEndEditing;	// IMP=0x323f461c
- (BOOL)_showsClearButton:(BOOL)fp8;	// IMP=0x323f3ec0
- (BOOL)_showsRightButton;	// IMP=0x323f3fd8
- (id)_style;	// IMP=0x323f4be4
- (struct CGRect)_textRectForEditing:(BOOL)fp8;	// IMP=0x323f7278
- (void)_updateAutosizeStyleIfNeeded;	// IMP=0x323f2db8
- (void)_updateBackgroundViewFrame;	// IMP=0x323f3670
- (void)_updateButtons;	// IMP=0x323f5834
- (void)_updateForStyleChange;	// IMP=0x323f2eb0
- (void)_updateStyleBasedOnTextSizeWithFieldEditor:(id)fp8 forPlaceholder:(BOOL)fp12;	// IMP=0x323f2a50
- (void)_updateTextLabelFrame;	// IMP=0x323f64e0
- (void)_useBackgroundView;	// IMP=0x323f376c
- (void)attachFieldEditor:(id)fp8;	// IMP=0x323f4fc8
- (unsigned int)becomeFirstResponder;	// IMP=0x323f46d0
- (BOOL)canBecomeFirstResponder;	// IMP=0x323f45ac
- (BOOL)canResignFirstResponder;	// IMP=0x323f46b0
- (unsigned int)characterOffsetAtPoint:(struct CGPoint)fp8;	// IMP=0x323f3ccc
- (struct CGSize)clearButtonOffset;	// IMP=0x323f4184
- (struct CGRect)clearButtonRect;	// IMP=0x323f7588
- (void)dealloc;	// IMP=0x323f25fc
- (id)delegate;	// IMP=0x323f4460
- (void)drawBorder:(struct CGRect)fp8;	// IMP=0x323f4364
- (void)drawRect:(struct CGRect)fp8;	// IMP=0x323f41d4
- (void)drawText:(struct CGRect)fp8;	// IMP=0x323f4228
- (struct CGRect)editRect;	// IMP=0x323f4194
- (BOOL)fieldEditor:(id)fp8 shouldInsertText:(id)fp12 replacingRange:(struct _NSRange)fp16;	// IMP=0x323f5440
- (BOOL)fieldEditor:(id)fp8 shouldReplaceWithText:(id)fp12;	// IMP=0x323f54b4
- (struct _NSRange)fieldEditor:(id)fp8 willChangeSelectionFromCharacterRange:(struct _NSRange)fp12 toCharacterRange:(struct _NSRange)fp20;	// IMP=0x323f5350
- (void)fieldEditorDidBecomeFirstResponder:(id)fp8;	// IMP=0x323f5050
- (void)fieldEditorDidBeginEditing:(id)fp8;	// IMP=0x323f528c
- (void)fieldEditorDidChange:(id)fp8;	// IMP=0x323f51b0
- (void)fieldEditorDidChangeSelection:(id)fp8;	// IMP=0x323f53f4
- (void)fieldEditorDidEndEditing:(id)fp8;	// IMP=0x323f52e4
- (void)fieldEditorDidResignFirstResponder:(id)fp8;	// IMP=0x323f50f0
- (BOOL)fieldEditorShouldEndEditing:(id)fp8;	// IMP=0x323f4690
- (void)forwardInvocation:(id)fp8;	// IMP=0x323f6988
- (BOOL)hasMarkedText;	// IMP=0x323f4b90
- (id)hitTest:(struct CGPoint)fp8 forEvent:(struct __GSEvent *)fp16;	// IMP=0x323f6ffc
- (struct CGRect)iconRect;	// IMP=0x323f40d8
- (id)initWithFrame:(struct CGRect)fp8;	// IMP=0x323f27d0
- (BOOL)isActivelyEditing;	// IMP=0x323f4b3c
- (BOOL)isEditing;	// IMP=0x323f4b2c
- (BOOL)isSecure;	// IMP=0x323f4540
- (void)layoutSubviews;	// IMP=0x323f6e90
- (id)methodSignatureForSelector:(SEL)fp8;	// IMP=0x323f6a44
- (void)mouseDown:(struct __GSEvent *)fp8;	// IMP=0x323f489c
- (void)mouseDragged:(struct __GSEvent *)fp8;	// IMP=0x323f4910
- (void)mouseUp:(struct __GSEvent *)fp8;	// IMP=0x323f4a94
- (float)paddingBottom;	// IMP=0x323f3238
- (float)paddingLeft;	// IMP=0x323f31e8
- (float)paddingRight;	// IMP=0x323f3260
- (float)paddingTop;	// IMP=0x323f3210
- (id)placeholder;	// IMP=0x323f395c
- (id)placeholderStyleForFieldEditor:(id)fp8;	// IMP=0x323f6460
- (id)placeholderTextForFieldEditor:(id)fp8;	// IMP=0x323f6458
- (BOOL)resignFirstResponder;	// IMP=0x323f47f0
- (void)selectAll:(id)fp8;	// IMP=0x323f3b90
- (void)selectAllFromFieldEditor:(id)fp8;	// IMP=0x323f5330
- (int)selectionBehavior;	// IMP=0x323f3cc4
- (struct _NSRange)selectionRange;	// IMP=0x323f3bf0
- (void)setAnimating:(BOOL)fp8;	// IMP=0x323f65c4
- (void)setAutoresizesTextToFit:(BOOL)fp8;	// IMP=0x323f55e8
- (void)setBorderStyle:(int)fp8;	// IMP=0x323f388c
- (void)setClearButtonOffset:(struct CGSize)fp8;	// IMP=0x323f5b60
- (void)setClearButtonStyle:(int)fp8;	// IMP=0x323f5b84
- (void)setDelegate:(id)fp8;	// IMP=0x323f4458
- (void)setEnabled:(BOOL)fp8;	// IMP=0x323f3ac8
- (void)setFont:(struct __GSFont *)fp8;	// IMP=0x323f30dc
- (void)setFont:(struct __GSFont *)fp8 fullFontSize:(float)fp12;	// IMP=0x323f3168
- (void)setFrame:(struct CGRect)fp8;	// IMP=0x323f6624
- (void)setHorizontallyCenterText:(BOOL)fp8;	// IMP=0x323f56a8
- (void)setIcon:(id)fp8;	// IMP=0x323f3d50
- (void)setInactiveHasDimAppearance:(BOOL)fp8;	// IMP=0x323f36e8
- (void)setLabel:(id)fp8;	// IMP=0x323f4e4c
- (void)setLabelOffset:(float)fp8;	// IMP=0x323f4d84
- (void)setPaddingBottom:(float)fp8;	// IMP=0x323f3218
- (void)setPaddingLeft:(float)fp8;	// IMP=0x323f31c8
- (void)setPaddingRight:(float)fp8;	// IMP=0x323f3240
- (void)setPaddingTop:(float)fp8;	// IMP=0x323f31f0
- (void)setPaddingTop:(float)fp8 paddingLeft:(float)fp12;	// IMP=0x323f31a4
- (void)setPlaceholder:(id)fp8;	// IMP=0x323f3964
- (void)setProgress:(float)fp8;	// IMP=0x323f4570
- (void)setRightButtonImage:(id)fp8;	// IMP=0x323f5d44
- (void)setRightButtonOffset:(struct CGSize)fp8;	// IMP=0x323f5d20
- (void)setRightButtonStyle:(int)fp8;	// IMP=0x323f5cf4
- (void)setRightPressedButtonImage:(id)fp8;	// IMP=0x323f5da4
- (void)setSecure:(BOOL)fp8;	// IMP=0x323f4468
- (void)setSelectionBehavior:(int)fp8;	// IMP=0x323f3cbc
- (void)setSelectionRange:(struct _NSRange)fp8;	// IMP=0x323f3c64
- (void)setText:(id)fp8;	// IMP=0x323f2f90
- (void)setTextAutorresizesToFit:(BOOL)fp8;	// IMP=0x323f5648
- (void)setTextCentersHorizontally:(BOOL)fp8;	// IMP=0x323f56e8
- (void)setTextCentersVertically:(BOOL)fp8;	// IMP=0x323f5744
- (void)setTextColor:(struct CGColor *)fp8;	// IMP=0x323f3588
- (void)setTextFont:(id)fp8;	// IMP=0x323f3268
- (void)setVerticallyCenterText:(BOOL)fp8;	// IMP=0x323f56c8
- (void)takeTraitsFrom:(id)fp8;	// IMP=0x323f6928
- (id)text;	// IMP=0x323f2920
- (struct CGColor *)textColor;	// IMP=0x323f3668
- (id)textLabel;	// IMP=0x323f4e24
- (struct CGRect)textRect;	// IMP=0x323f4144
- (id)textTraits;	// IMP=0x323f68dc
- (BOOL)webView:(id)fp8 shouldInsertText:(id)fp12 replacingDOMRange:(id)fp16 givenAction:(int)fp20;	// IMP=0x323f552c
- (void)willAttachFieldEditor:(id)fp8;	// IMP=0x323f4c18
- (void)willDetachFieldEditor:(id)fp8;	// IMP=0x323f4fe4

@end

