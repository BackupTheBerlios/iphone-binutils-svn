/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

#import "NSObject.h"

@class NSString;

@interface _UIDateLabelCache : NSObject
{
    double _today;
    double _noon;
    double _tomorrow;
    double _previousWeek;
    struct __GSFont *_timeDesignatorFont;
    NSString *_amString;
    NSString *_pmString;
    struct CGSize _amSize;
    struct CGSize _pmSize;
    struct __CFDictionary *_dateStringCache;
    struct __CFDateFormatter *_timeDateFormatter;
    struct __CFTimeZone *_tz;
    struct __CFDictionary *_dateSizeCache;
}

- (void)_loadDesignatorStrings;	// IMP=0x3245e8a4
- (void)_significantTimeChange;	// IMP=0x3245e824
- (void)_updateTodayAndNoon;	// IMP=0x3245eb9c
- (struct CGSize)amSize;	// IMP=0x3245ea64
- (id)amString;	// IMP=0x3245e9fc
- (int)dateKeyForAbsoluteTime:(double)fp8;	// IMP=0x3245ec80
- (id)dateStringForAbsoluteTime:(double)fp8 dateKey:(int)fp16;	// IMP=0x3245ef94
- (void)dealloc;	// IMP=0x3245e664
- (id)init;	// IMP=0x3245e500
- (void)invalidateDateCache;	// IMP=0x3245e790
- (struct CGSize)mainTimeSizeForDateKey:(int)fp8;	// IMP=0x3245f1bc
- (double)noonAbsoluteTime;	// IMP=0x3245ed74
- (struct CGSize)pmSize;	// IMP=0x3245eb00
- (id)pmString;	// IMP=0x3245ea30
- (void)setMainTimeSize:(struct CGSize)fp8 forDateKey:(int)fp16;	// IMP=0x3245f234
- (struct __GSFont *)timeDesignatorFont;	// IMP=0x3245e75c
- (id)timeDesignatorForAbsoluteTime:(double)fp8;	// IMP=0x3245eddc
- (struct CGSize)timeDesignatorSizeForAbsoluteTime:(double)fp8;	// IMP=0x3245eea0
- (double)todayAbsoluteTime;	// IMP=0x3245ed3c
- (BOOL)use24HourTime;	// IMP=0x3245edac

@end

