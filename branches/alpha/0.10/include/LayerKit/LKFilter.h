/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import "NSObject.h"

#import "NSCodingProtocol.h"
#import "NSCopyingProtocol.h"

@class NSString;

@interface LKFilter : NSObject <NSCopying, NSCoding>
{
    unsigned int _type;
    NSString *_name;
    BOOL _enabled;
    struct _LKAttrList *_attr;
}

+ (id)filterWithName:(id)fp8;	// IMP=0x30aec7e0
- (void *)LK_copyRenderValue;	// IMP=0x30aecb10
- (id)copyWithZone:(struct _NSZone *)fp8;	// IMP=0x30aecc20
- (void)dealloc;	// IMP=0x30aeca64
- (BOOL)enabled;	// IMP=0x30aec8c0
- (void)encodeWithCoder:(id)fp8;	// IMP=0x30aecc9c
- (id)initWithCoder:(id)fp8;	// IMP=0x30aecca0
- (id)initWithName:(id)fp8;	// IMP=0x30aec824
- (id)name;	// IMP=0x30aec870
- (void)setEnabled:(BOOL)fp8;	// IMP=0x30aec8c8
- (void)setName:(id)fp8;	// IMP=0x30aec878
- (void)setValue:(id)fp8 forKey:(id)fp12;	// IMP=0x30aec90c
- (id)type;	// IMP=0x30aec85c
- (id)valueForKey:(id)fp8;	// IMP=0x30aec9e8

@end

