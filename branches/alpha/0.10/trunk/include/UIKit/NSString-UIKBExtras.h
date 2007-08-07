/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import "NSString.h"

@interface NSString (UIKBExtras)
+ (id)stringWithUnichar:(unsigned short)fp8;	// IMP=0x324a0800
- (BOOL)endsSentence;	// IMP=0x324a0a98
- (BOOL)endsWord;	// IMP=0x324a1144
- (BOOL)isDottedURLDomain;	// IMP=0x324a0b38
- (BOOL)isLeftAssociative;	// IMP=0x324a10b0
- (BOOL)isSpace;	// IMP=0x324a11e4
- (BOOL)isSpaceOrReturn;	// IMP=0x324a1240
- (BOOL)isTripledPunctuation;	// IMP=0x324a0be0
- (BOOL)looksLikeEmailAddress;	// IMP=0x324a0c64
- (BOOL)looksLikeNumberInput;	// IMP=0x324a129c
- (BOOL)looksLikeURL;	// IMP=0x324a0cb8
- (id)stringByReplacingCharacter:(unsigned short)fp8 withCharacter:(unsigned short)fp12;	// IMP=0x324a09ac
- (id)stringByReplacingCharactersInSet:(struct __CFCharacterSet *)fp8 withCharacter:(unsigned short)fp12;	// IMP=0x324a08a4
- (id)stringByTrimmingCharactersInCFCharacterSet:(struct __CFCharacterSet *)fp8;	// IMP=0x324a0d4c
- (id)stringByTrimmingLastCharacter;	// IMP=0x324a085c
@end

