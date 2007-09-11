#import <Foundation/Foundation.h>
#import <GraphicsServices/GraphicsServices.h>
#import <UIKit/UIView.h>

@class UINavBarButton, UINavBarPrompt;

typedef enum {
    kUINavigationBarBlue = 0,
	kUINavigationBarBlack = 1
} UINavigationBarStyle;

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

+ (GSFontRef)defaultPromptFont;	
+ (CGSize)defaultSize;	
+ (CGSize)defaultSizeWithPrompt;

- (id)createButtonWithContents:(id)contents width:(float)width barStyle:(UINavigationBarStyle)style buttonStyle:(int)buttonStyle isRight:(BOOL)isRight; // button style enum needed

- (unsigned int)animationDisabledCount;	
- (CGRect)availableTitleArea;	

- (void)enableAnimation;
- (void)disableAnimation;
- (BOOL)isAnimationEnabled;

- (void)drawBackButtonBackgroundInRect:(CGRect)rect withStyle:(UINavigationBarStyle)style pressed:(BOOL)pressed;	
- (void)drawBackgroundInRect:(CGRect)fp8 withStyle:(UINavigationBarStyle)style;
- (void)drawRect:(CGRect)rect;

- (void)setAccessoryView:(id)accessoryView;	
- (void)setAccessoryView:(id)accessoryView animate:(BOOL)animate removeOnPop:(BOOL)removeOnPop;

- (void)setBarStyle:(UINavigationBarStyle)style;

- (id)delegate;
- (void)setDelegate:(id)delegate;

- (void)pushNavigationItem:(id)navigationItem;
- (void)popNavigationItem;
- (id)topItem;
- (id)navigationItemAtPoint:(CGPoint)point;	
- (NSArray *)navigationItems;
- (void)setNavigationItems:(NSArray *)navigationItems;

- (id)prompt;
- (void)setPrompt:(id)fp8;
- (CGRect)promptBounds;	
- (id)promptView;

- (void)setRightMargin:(float)fp8;

- (void)setButton:(int)fp8 enabled:(BOOL)fp12;
- (void)showBackButton:(BOOL)fp8 animated:(BOOL)fp12;	
- (void)showButtonsWithLeft:(id)fp8 right:(id)fp12 leftBack:(BOOL)fp16;	
- (void)showButtonsWithLeftTitle:(id)fp8 rightTitle:(id)fp12;	
- (void)showButtonsWithLeftTitle:(id)fp8 rightTitle:(id)fp12 leftBack:(BOOL)fp16;	
- (void)showLeftButton:(id)fp8 withStyle:(int)fp12 rightButton:(id)fp16 withStyle:(int)fp20;
- (void)hideButtons;

- (BOOL)_canHandleStatusBarMouseEvents;	
- (void)_hideButtonsAnimationDidStop:(id)fp8 finished:(id)fp12 context:(void *)fp16;	
- (void)_navigationAnimationDidFinish;	
- (void)_showLeftRightButtonsAnimationDidStop:(id)fp8 finished:(id)fp12;	

@end

@interface UINavigationBar (Static)
- (void)_adjustVisibleItemsByDelta:(float)fp8;	
- (void)_backgroundFadeDidFinish:(id)fp8 finished:(id)fp12 context:(void *)fp16;	
- (int)_barStyle:(BOOL)fp8;	
- (float)_barWidth;	
- (struct CGRect)_boundsForPrompt:(id)fp8 inRect:(struct CGRect)fp12 withFont:(struct __GSFont *)fp28 barStyle:(int)fp32;	
- (void)_broadcastNewTopToItems:(int)fp8 complete:(BOOL)fp12;	
- (id)_currentBackButton;	
- (void)_drawPrompt:(id)fp8 inRect:(struct CGRect)fp12 withFont:(struct __GSFont *)fp28 barStyle:(int)fp32;	
- (void)_fadeViewOut:(id)fp8;	
- (void)_fadeViewsIn:(id)fp8;	
- (void)_fadeViewsOut:(id)fp8;	
- (void)_getBackButtonRect:(struct CGRect *)fp8 titleRect:(struct CGRect *)fp12;	
- (BOOL)_hasBackButton;	
- (void)_navBarButtonPressed:(id)fp8;	
- (void)_removeAccessoryView;	
- (void)_removeItemsFromSuperview:(id)fp8;	
- (void)_startBarStyleAnimation:(int)fp8;	
- (void)_startPopAnimation;	
- (void)_startPushAnimation;	
- (void)layoutSubviews;	
@end
