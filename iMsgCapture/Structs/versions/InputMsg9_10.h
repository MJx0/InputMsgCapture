#pragma once

// Android 9 to 10

#include <array>
#include <limits>
#include <string>

#include "../Common.h"

struct InputMsg9_10 {
    enum {
        TYPE_KEY = 1,
        TYPE_MOTION = 2,
        TYPE_FINISHED = 3,
    };
    struct Header {
        uint32_t type;
        // We don't need this field in order to align the body below but we
        // leave it here because InputMessage::size() and other functions
        // compute the size of this structure as sizeof(Header) + sizeof(Body).
        uint32_t padding;
    } header;
    // Body *must* be 8 byte aligned.
    union Body {
        struct Key {
            uint32_t seq;
            uint32_t empty1;
            nsecs_t eventTime __attribute__((aligned(8)));
            int32_t deviceId;
            int32_t source;
            int32_t displayId;
            int32_t action;
            int32_t flags;
            int32_t keyCode;
            int32_t scanCode;
            int32_t metaState;
            int32_t repeatCount;
            uint32_t empty2;
            nsecs_t downTime __attribute__((aligned(8)));
            inline size_t size() const { return sizeof(Key); }
        } key;
        struct Motion {
            uint32_t seq;
            uint32_t empty1;
            nsecs_t eventTime __attribute__((aligned(8)));
            int32_t deviceId;
            int32_t source;
            int32_t displayId;
            int32_t action;
            int32_t actionButton;
            int32_t flags;
            int32_t metaState;
            int32_t buttonState;
            int32_t edgeFlags;
            uint32_t empty2;
            nsecs_t downTime __attribute__((aligned(8)));
            float xOffset;
            float yOffset;
            float xPrecision;
            float yPrecision;
            uint32_t pointerCount;
            uint32_t empty3;
            // Note that PointerCoords requires 8 byte alignment.
            struct Pointer {
                PointerProperties properties;
                PointerCoords coords;
            } pointers[MAX_POINTERS];
            inline size_t size() const { return sizeof(Motion) - sizeof(Pointer) * MAX_POINTERS + sizeof(Pointer) * pointerCount; }
            inline bool valid() const { return pointerCount > 0 && pointerCount <= MAX_POINTERS; }
        } motion;
        struct Finished {
            uint32_t seq;
            bool handled;
            inline size_t size() const { return sizeof(Finished); }
        } finished;
    } __attribute__((aligned(8))) body;
};
