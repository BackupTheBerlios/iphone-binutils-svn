/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <UIKit/UIView.h>

@class NSMutableArray, UINavBarButton, UINavBarPrompt;

@interface UINavigationBar : UIView
{
    NSMutableArray *_itemStack;
    float _rightMargin;
    int _state;
    id _delegate;
    UINavBarButton *_rightButton;
    UINavBarButton *_leftButton;
    UINavBarPrompt *_prompt;
    UIView *_accessoryView;
    struct {
        unsigned int animate:1;
        unsigned int animationDisabledCount:10;
        unsigned int barStyle:3;
        unsigned int disableLayout:1;
        unsigned int backPressed:1;
        unsigned int animatePromptChange:1;
        unsigned int hideBackButton:1;
        unsigned int removeAccessoryOnPop:1;
        unsigned int reserved:13;
    } _navbarFlags;
}

+ (struct __GSFont *)defaultPromptFont;	// IMP=0x323d1908
+ (struct CGSize)defaultSize;	// IMP=0x323d18bc
+ (struct CGSize)defaultSizeWithPrompt;	// IMP=0x323d8c90
- (BOOL)_canHandleStatusBarMouseEvents;	// IMP=0x323d2fd0
- (void)_hideButtonsAnimationDidStop:(id)fp8 finished:(id)fp12 context:(void *)fp16;	// IMP=0x323d3ae4
- (void)_navigationAnimationDidFinish;	// IMP=0x323d2e1c
- (void)_showLeftRightButtonsAnimationDidStop:(id)fp8 finished:(id)fp12;	// IMP=0x323d3694
- (unsigned int)animationDisabledCount;	// IMP=0x323d35dc
- (struct CGRect)availableTitleArea;	// IMP=0x323d3c60
- (id)createButtonWithContents:(id)fp8 width:(float)fp12 barStyle:(int)fp16 buttonStyle:(int)fp20 isRight:(BOOL)fp24;	// IMP=0x323d9034
- (void)dealloc;	// IMP=0x323d19e4
- (id)delegate;	// IMP=0x323d3358
- (void)disableAnimation;	// IMP=0x323d3520
- (void)drawBackButtonBackgroundInRect:(struct CGRect)fp8 withStyle:(int)fp24 pressed:(BOOL)fp28;	// IMP=0x323d5ab4
- (void)drawBackgroundInRect:(struct CGRect)fp8 withStyle:(int)fp24;	// IMP=0x323d7360
- (void)drawRect:(struct CGRect)fp8;	// IMP=0x323d1a90
- (void)enableAnimation;	// IMP=0x323d3550
- (void)hideButtons;	// IMP=0x323d3828
- (id)hitTest:(struct CGPoint)fp8 forEvent:(struct __GSEvent *)fp16;	// IMP=0x323d7528
- (id)initWithFrame:(struct CGRect)fp8;	// IMP=0x323d191c
- (BOOL)isAnimationEnabled;	// IMP=0x323d358c
- (void)mouseDown:(struct __GSEvent *)fp8;	// IMP=0x323d2fd8
- (void)mouseUp:(struct __GSEvent *)fp8;	// IMP=0x323d30f0
- (id)navigationItemAtPoint:(struct CGPoint)fp8;	// IMP=0x323d2d0c
- (id)navigationItems;	// IMP=0x323d2c88
- (void)popNavigationItem;	// IMP=0x323d2354
- (id)prompt;	// IMP=0x323d346c
- (struct CGRect)promptBounds;	// IMP=0x323d349c
- (id)promptView;	// IMP=0x323d3494
- (void)pushNavigationItem:(id)fp8;	// IMP=0x323d1fb0
- (void)setAccessoryView:(id)fp8;	// IMP=0x323d1b2c
- (void)setAccessoryView:(id)fp8 animate:(BOOL)fp12 removeOnPop:(BOOL)fp16;	// IMP=0x323d1b80
- (void)setBarStyle:(int)fp8;	// IMP=0x323d3360
- (void)setButton:(int)fp8 enabled:(BOOL)fp12;	// IMP=0x323d37cc
- (void)setDelegate:(id)fp8;	// IMP=0x323d3350
- (void)setFrame:(struct CGRect)fp8;	// IMP=0x323d6e58
- (void)setNavigationItems:(id)fp8;	// IMP=0x323d2774
- (void)setPrompt:(id)fp8;	// IMP=0x323d69e8
- (void)setRightMargin:(float)fp8;	// IMP=0x323d3454
- (void)showBackButton:(BOOL)fp8 animated:(BOOL)fp12;	// IMP=0x323d3b6c
- (void)showButtonsWithLeft:(id)fp8 right:(id)fp12 leftBack:(BOOL)fp16;	// IMP=0x323d3644
- (void)showButtonsWithLeftTitle:(id)fp8 rightTitle:(id)fp12;	// IMP=0x323d35ec
- (void)showButtonsWithLeftTitle:(id)fp8 rightTitle:(id)fp12 leftBack:(BOOL)fp16;	// IMP=0x323d3618
- (void)showLeftButton:(id)fp8 withStyle:(int)fp12 rightButton:(id)fp16 withStyle:(int)fp20;	// IMP=0x323d7894
- (id)topItem;	// IMP=0x323d2704

@end

