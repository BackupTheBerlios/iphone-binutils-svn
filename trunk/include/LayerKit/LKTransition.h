/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <LayerKit/LKAnimation.h>

@interface LKTransition : LKAnimation
{
}

- (id)description;	// IMP=0x30aea3d0
- (float)endProgress;	// IMP=0x30aea470
- (id)filter;	// IMP=0x30aea478
- (float)startProgress;	// IMP=0x30aea468
- (id)subtype;	// IMP=0x30aea460
- (id)type;	// IMP=0x30aea458

@end

@interface LKTransition (LKTransitionPrivate)
- (unsigned int)transitionFlags;	// IMP=0x30aea480
@end
