/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import "NSURL.h"

@interface NSURL (UIKitAdditions)
+ (id)URLWithTelephoneNumber:(id)fp8;	// IMP=0x324a9ec0
+ (id)URLWithTelephoneNumber:(id)fp8 addressBookUID:(int)fp12;	// IMP=0x324a9f34
+ (id)mapsURLWithQuery:(id)fp8;	// IMP=0x324aa228
+ (id)mapsURLWithSourceAddress:(id)fp8 destinationAddress:(id)fp12;	// IMP=0x324aa2e8
- (void)getPhoneNumber:(id *)fp8 addressBookUID:(int *)fp12;	// IMP=0x324aa054
- (BOOL)hasTelephonyScheme;	// IMP=0x324aa1b8
- (BOOL)isSpringboardHandledURL;	// IMP=0x324aa7e4
- (id)mapsURL;	// IMP=0x324aa48c
- (id)phoneNumber;	// IMP=0x324a9fb8
- (id)youTubeURL;	// IMP=0x324aa67c
@end

