#import <GraphicsServices/GraphicsServices.h>
#import <UIKit/UIView.h>

typedef enum {
    kUIScrollerIndicatorBlack = 1,
    kUIScrollerIndicatorWhite = 2,
    kUIScrollerIndicatorBlackWithWhiteBorder = 3
} UIScrollerIndicatorStyle;

@interface UIScroller : UIView {}

- (id)initWithFrame:(CGRect)frame;	

- (BOOL)cancelMouseTracking;	
- (void)cancelNextContentMouseUp;	
- (void)contentMouseUpInView:(id)view withEvent:(GSEventRef)event;	

- (void)gestureChanged:(GSEventRef)event;	
- (void)gestureEnded:(GSEventRef)event;	
- (void)gestureStarted:(GSEventRef)event;

- (void)highlightView:(id)view state:(BOOL)state;

- (BOOL)adjustSmoothScrollEnd:(CDAnonymousStruct6)fp8;
- (CDAnonymousStruct6)velocity;
- (BOOL)isDecelerating;	
- (BOOL)isScrolling;

// scrolling
- (void)scrollByDelta:(CGSize)delta;	
- (void)scrollByDelta:(CGSize)delta animated:(BOOL)animated;	
- (void)scrollPointVisibleAtTopLeft:(CGPoint)topLeftPoint;	
- (void)scrollPointVisibleAtTopLeft:(CGPoint)topLeftPoint animated:(BOOL)animated;	
- (void)scrollRectToVisible:(CGRect)rect;	
- (void)scrollRectToVisible:(CGRect)rect animated:(BOOL)animated;
- (void)displayScrollerIndicators;

- (void)setDirectionalScrolling:(BOOL)directionalScrolling;	
- (void)setDirectionalScrollingAngle:(float)angle;
- (void)setScrollDecelerationFactor:(float)fp8;
- (void)setScrollHysteresis:(float)fp8;
- (double)scrollToPointAnimationDuration;
- (void)setScrollToPointAnimationDuration:(double)scrollToPointAnimationDuration;
- (void)setScrollerIndicatorStyle:(UIScrollerIndicatorStyle)style;	
- (void)setScrollerIndicatorSubrect:(CGRect)rect;	
- (void)setScrollerIndicatorsPinToContent:(BOOL)scrollerIndicatorsPinToContent;	
- (void)setScrollingEnabled:(BOOL)scrollingEnabled;	
- (void)setShowBackgroundShadow:(BOOL)showBackgroundShadow;	
- (void)setShowScrollerIndicators:(BOOL)showScrollerIndicators;
- (void)setThumbDetectionEnabled:(BOOL)thumbDetectionEnabled;

- (void)setAllowsFourWayRubberBanding:(BOOL)allowsFourWayRubberBanding;
- (void)setAllowsRubberBanding:(BOOL)allowsRubberBanding;
- (void)setRubberBand:(float)rubberBandWidth forEdges:(int)edgeMask; // enum needed
- (BOOL)releaseRubberBandIfNecessary;

- (float)bottomBufferHeight;
- (void)setBottomBufferHeight:(float)bottomBufferHeight;

- (void)setBounces:(BOOL)bounces;

- (CGSize)contentSize;
- (void)setContentSize:(CGSize)contentSize;
- (void)setAdjustForContentSizeChange:(BOOL)adjustForContentSizeChange;

- (id)delegate;
- (void)setDelegate:(id)delegate;

- (void)setEventMode:(int)eventMode; // enum needed

- (void)setGridSize:(CGSize)gridSize;	
- (void)setHighlightContentImmediately:(BOOL)highlightContentImmediately;

- (CGPoint)offset;
- (void)setOffset:(CGPoint)offset;
- (CGPoint)dragStartOffset;
- (void)setOffsetForDragOffset:(CGPoint)offset withEvent:(GSEventRef)event duration:(float)duration;

- (BOOL)canHandleSwipes;
- (int)swipe:(UIViewSwipeDirection)swipeDirection withEvent:(GSEventRef)event;

- (void)_didMoveFromWindow:(id)window toWindow:(id)otherWindow;	
- (void)_hideScrollIndicators;	
- (CGPoint)_pinnedScrollPointForPoint:(CGPoint)fp8;	
- (void)_popTrackingRunLoopMode;	
- (void)_scrollAnimationEnded;	

@end

@interface UIScroller (Static)
- (void)_adjustEndOffset;	
- (void)_adjustScrollerIndicators:(BOOL)fp8 alwaysShowingThem:(BOOL)fp12;	
- (BOOL)_continueScrollingAtOffset:(CGPoint)fp8;	
- (void)_controlMouseDown:(GSEventRef)fp8;	
- (void)_controlMouseDragged:(GSEventRef)fp8;	
- (void)_controlMouseUp:(GSEventRef)fp8;	
- (void)_doContentHighlight;	
- (BOOL)_dragging;	
- (void)_notifyDidScroll;	
- (BOOL)_passControlEvents;	
- (void)_runLoopModePopped:(id)fp8;	
- (void)_smoothScroll:(double)fp8;	
- (void)_startTimer:(BOOL)fp8;	
@end

@interface UIScroller (Internal)
+ (void)_registerForNotifications;	
+ (void)_unregisterForNotifications;	
- (void)_addScrollNotificationView:(id)fp8;	
- (void)_adjustBackgroundShadowsForContentSize:(CGSize)fp8;	
- (void)_adjustSpecialViews;	
- (BOOL)_alwaysHandleInteractionEvents;	
- (id)_bottomShadowView;	
- (id)_bottomSpecialView;	
- (void)_cancelContentHighlight;	
- (BOOL)_isUserScrolling;	
- (void)_removeScrollNotificationView:(id)fp8;	
- (void)_resetScrollingWithEvent:(GSEventRef)fp8;	
- (void)_stopScrollingNotify:(BOOL)fp8;	
- (id)_topSpecialView;	
@end
