#pragma once

// Android 11

#include <array>
#include <limits>
#include <string>

#include "../Common.h"

struct InputMsg11 {
    enum {
        TYPE_KEY,
        TYPE_MOTION,
        TYPE_FINISHED,
        TYPE_FOCUS,
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
            int32_t eventId;
            nsecs_t eventTime __attribute__((aligned(8)));
            int32_t deviceId;
            int32_t source;
            int32_t displayId;
            std::array<uint8_t, 32> hmac;
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
            int32_t eventId;
            nsecs_t eventTime __attribute__((aligned(8)));
            int32_t deviceId;
            int32_t source;
            int32_t displayId;
            std::array<uint8_t, 32> hmac;
            int32_t action;
            int32_t actionButton;
            int32_t flags;
            int32_t metaState;
            int32_t buttonState;
            uint8_t classification; // AMOTION_EVENT_CLASSIFICATION_X
            uint8_t empty2[3]; // 3 bytes to fill gap created by classification
            int32_t edgeFlags;
            nsecs_t downTime __attribute__((aligned(8)));
            float xScale;
            float yScale;
            float xOffset;
            float yOffset;
            float xPrecision;
            float yPrecision;
            float xCursorPosition;
            float yCursorPosition;
            uint32_t pointerCount;
            uint32_t empty3;
            /**
             * The "pointers" field must be the last field of the struct InputMessage.
             * When we send the struct InputMessage across the socket, we are not
             * writing the entire "pointers" array, but only the pointerCount portion
             * of it as an optimization. Adding a field after "pointers" would break this.
             */
            struct Pointer {
                PointerProperties properties;
                PointerCoords coords;
            } pointers[MAX_POINTERS] __attribute__((aligned(8)));
            inline size_t size() const { return sizeof(Motion) - sizeof(Pointer) * MAX_POINTERS + sizeof(Pointer) * pointerCount; }
            inline bool valid() const { return pointerCount > 0 && pointerCount <= MAX_POINTERS; }
        } motion;
        struct Finished {
            uint32_t seq;
            uint32_t handled; // actually a bool, but we must maintain 8-byte alignment
            inline size_t size() const { return sizeof(Finished); }
        } finished;
        struct Focus {
            uint32_t seq;
            int32_t eventId;
            uint32_t empty1;
            // The following two fields take up 4 bytes total
            uint16_t hasFocus; // actually a bool
            uint16_t inTouchMode; // actually a bool, but we must maintain 8-byte alignment
            inline size_t size() const { return sizeof(Focus); }
        } focus;
    } __attribute__((aligned(8))) body;
};
