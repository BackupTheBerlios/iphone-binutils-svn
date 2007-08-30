/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <Foundation/NSObject.h>

@interface LKTransaction : NSObject {}
+ (void)begin;
+ (void)commit;
+ (void)flush;

@end

@interface LKTransaction (LKTransactionPrivate)
+ (BOOL)beginWithoutBlocking;	// IMP=0x30afc674
+ (unsigned int)currentState;	// IMP=0x30afc584
+ (void)synchronize;	// IMP=0x30afc500
@end
