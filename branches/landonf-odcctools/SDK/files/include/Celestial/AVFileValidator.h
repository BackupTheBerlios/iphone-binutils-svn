/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import "NSObject.h"

@interface AVFileValidator : NSObject
{
    struct AVFileValidatorPrivate *_priv;
}

- (id)byteStream;	// IMP=0x302a28c4
- (void)cancel;	// IMP=0x302a2fc0
- (void)dealloc;	// IMP=0x302a26d8
- (id)initWithPath:(id)fp8;	// IMP=0x302a2620
- (id)path;	// IMP=0x302a27e0
- (float)progress;	// IMP=0x302a3044
- (void)refMovieLeaf:(id)fp8;	// IMP=0x302a2b48
- (void)refMovieResolved:(id)fp8;	// IMP=0x302a2a54
- (void)setPath:(id)fp8;	// IMP=0x302a2814
- (void)streamFailed:(id)fp8;	// IMP=0x302a2ddc
- (void)streamOpened:(id)fp8;	// IMP=0x302a2c08
- (void)validate;	// IMP=0x302a2ea4

@end

