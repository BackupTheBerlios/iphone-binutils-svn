/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

struct CGAffineTransform {
    float _field1;
    float _field2;
    float _field3;
    float _field4;
    float _field5;
    float _field6;
};

struct CGColor;

struct CGContext;

struct CGPath;

struct CGPoint {
    float x;
    float y;
};

struct CGRect {
    struct CGPoint _field1;
    struct CGSize _field2;
};

struct CGSBounds {
    int x;
    int y;
    int w;
    int h;
};

struct CGSize {
    float width;
    float height;
};

struct LKTimingFunctionPrivate {
    float _field1[2];
    float _field2[2];
};

struct LKTransform {
    float _field1;
    float _field2;
    float _field3;
    float _field4;
    float _field5;
    float _field6;
    float _field7;
    float _field8;
    float _field9;
    float _field10;
    float _field11;
    float _field12;
    float _field13;
    float _field14;
    float _field15;
    float _field16;
};

struct _LKAttrList;

struct _LKPropertyDescription;

struct _LKRenderAnimation {
    CDAnonymousStruct1 _field1;
    float _field2;
    struct _LKRenderTiming *_field3;
    struct _LKRenderVector *_field4;
    union _LKRenderKeyPath _field5;
    unsigned int _field6;
    union {
        struct _LKRenderArray *_field1;
        union _LKRenderKeyPath _field2;
        struct {
            union _LKRenderKeyPath _field1;
            void *_field2;
            void *_field3;
            void *_field4;
        } _field3;
        struct {
            union _LKRenderKeyPath _field1;
            struct _LKRenderArray *_field2;
            struct _LKRenderVector *_field3;
            struct _LKRenderPath *_field4;
        } _field4;
        struct {
            unsigned int _field1;
            unsigned int _field2;
            float _field3;
            float _field4;
            struct _LKRenderFilter *_field5;
            void *_field6;
            unsigned int _field7;
            unsigned int _field8;
        } _field5;
    } _field7;
};

struct _LKRenderArray;

struct _LKRenderContext {
    struct {
        int _field1;
    } _field1;
    unsigned int _field2;
    struct __CFDictionary *_field3;
    unsigned int _field4;
    struct _opaque_pthread_mutex_t _field5;
    unsigned int _field6;
    struct _LKRenderHandle *_field7;
    struct __CFDictionary *_field8;
    struct x_list_struct *_field9;
    float _field10;
    struct _LKRenderFence *_field11;
    double _field12;
    int *_field13;
    struct CGSBounds _field14;
    unsigned int _field15;
    unsigned int _field16;
};

struct _LKRenderFence;

struct _LKRenderFilter;

struct _LKRenderHandle;

struct _LKRenderPath;

struct _LKRenderProxy {
    CDAnonymousStruct1 _field1;
    unsigned long long _field2;
};

struct _LKRenderTiming;

struct _LKRenderVector;

struct _NSZone;

struct __CFArray;

struct __CFDictionary;

struct _opaque_pthread_mutex_t {
    long __sig;
    char __opaque[40];
};

struct x_list_struct {
    void *_field1;
    struct x_list_struct *_field2;
};

typedef struct {
    int x;
} CDAnonymousStruct1;

union _LKRenderKeyPath {
    unsigned int _field1;
    unsigned int *_field2;
};

