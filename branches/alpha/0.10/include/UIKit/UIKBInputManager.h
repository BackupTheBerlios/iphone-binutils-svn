/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import "NSObject.h"

@interface UIKBInputManager : NSObject
{
}

+ (id)activeInstance;	// IMP=0x3249f34c
+ (void)clearCentroids;	// IMP=0x3249f294
+ (void)registerCentroid:(struct CGPoint)fp8 forKey:(id)fp16;	// IMP=0x3249f498
+ (void)releaseSharedInstance;	// IMP=0x3249f4f4
+ (void)removeAllDynamicDictionaries;	// IMP=0x3249f2a4
+ (id)sharedInstance;	// IMP=0x3249f56c
- (void)acceptInput;	// IMP=0x3249fec8
- (void)acceptMarkedInput;	// IMP=0x3249fe40
- (void)addInput:(id)fp8 flags:(unsigned int)fp12 point:(struct CGPoint)fp16;	// IMP=0x324a0128
- (id)autocorrection;	// IMP=0x3249f904
- (void)clearDynamicDictionary;	// IMP=0x324a02b4
- (void)clearInput;	// IMP=0x3249ff0c
- (void)clearMarkedInput;	// IMP=0x3249fe84
- (void)clearTypingHistory;	// IMP=0x3249fb48
- (void)dealloc;	// IMP=0x324a072c
- (void)deleteFromInput;	// IMP=0x3249ff50
- (id)generateSuggestions;	// IMP=0x3249f800
- (id)generateSuggestions:(BOOL)fp8;	// IMP=0x3249f5fc
- (id)init;	// IMP=0x3249f35c
- (void)initializeAutocorrectStems;	// IMP=0x3249f3d0
- (int)inputCount;	// IMP=0x3249fd5c
- (unsigned int)inputIndex;	// IMP=0x3249fda8
- (id)inputString;	// IMP=0x3249fcf4
- (int)lookupMode;	// IMP=0x324a0430
- (void)nextCharacterProbabilities:(float **)fp8;	// IMP=0x3249f96c
- (void)noteUserDeletedWord;	// IMP=0x3249fc38
- (void)noteUserDeletedWord:(id)fp8;	// IMP=0x3249fbd0
- (void)noteUserTypedWord:(id)fp8 weight:(int)fp12;	// IMP=0x3249fc7c
- (void)selectionChanged;	// IMP=0x3249fb8c
- (void)setAutoCorrecting:(BOOL)fp8;	// IMP=0x324a0394
- (void)setAutoShiftFlag:(BOOL)fp8;	// IMP=0x3249fa30
- (void)setCalculatesNextCharacterProbabilities:(BOOL)fp8;	// IMP=0x324a033c
- (void)setInput:(id)fp8;	// IMP=0x3249ff94
- (void)setInputIndex:(unsigned int)fp8;	// IMP=0x3249fdf4
- (void)setLookupMode:(int)fp8;	// IMP=0x324a03e4
- (void)setShift:(BOOL)fp8;	// IMP=0x3249fa80
- (void)setTestMode;	// IMP=0x324a02f8
- (void)setTextDomain:(int)fp8;	// IMP=0x324a047c
- (id)shadowTyping;	// IMP=0x3249fad8
- (int)textDomain;	// IMP=0x324a06e0

@end

