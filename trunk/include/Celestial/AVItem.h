/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import <Foundation/Foundation.h>

@interface AVItem : NSObject
{}

+ (id)avItem;
+ (id)avItemWithPath:(NSString *)path error:(NSError **)error;
- (id)init;
- (id)initWithError:(NSError **)error;
- (id)initWithPath:(NSString *)path error:(NSError **)error;

- (double)duration;

- (CGSize)naturalSize;
- (id)path;

- (id)attributeForKey:(id)key;
- (BOOL)setAttribute:(id)attribute forKey:(id)key error:(NSError **)error;

- (int)eqPreset; // enum needed
- (void)setEQPreset:(int)fp8;

- (BOOL)setPath:(NSString *)path error:(NSError **)error;

- (float)volume;
- (void)setVolume:(float)volume;

- (int)_instantiateItem;

@end

