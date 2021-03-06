/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <Foundation/NSObject.h>

extern NSString *kLKTransactionDisableActions;
extern NSString *kLKTransactionAnimationDelegate;
extern NSString *kLKTransactionAnimationDuration;
extern NSString *kLKTransactionAnimationTimingFunction;

@interface LKTransaction : NSObject {}
+ (void)begin;
+ (void)commit;
+ (void)flush;

@end

@interface LKTransaction (LKTransactionPrivate)
+ (BOOL)beginWithoutBlocking;	
+ (unsigned int)currentState;	
+ (void)synchronize;	
@end
