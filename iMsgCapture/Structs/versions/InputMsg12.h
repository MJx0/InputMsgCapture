#pragma once

// Android 12

#include <array>
#include <limits>
#include <string>

#include "../Common.h"

struct InputMsg12 {
    enum {
        TYPE_KEY,
        TYPE_MOTION,
        TYPE_FINISHED,
        TYPE_FOCUS,
        TYPE_CAPTURE,
        TYPE_DRAG,
        TYPE_TIMELINE,
    };
    struct Header {
        uint32_t type;
        uint32_t seq;
    } header;

    // For keys and motions, rely on the fact that std::array takes up exactly as much space
    // as the underlying data. This is not guaranteed by C++, but it simplifies the conversions.
    static_assert(sizeof(std::array<uint8_t, 32>) == 32);
    // For bool values, rely on the fact that they take up exactly one byte. This is not guaranteed
    // by C++ and is implementation-dependent, but it simplifies the conversions.
    static_assert(sizeof(bool) == 1);

    // Body *must* be 8 byte aligned.
    union Body {
        struct Key {
            int32_t eventId;
            uint32_t empty1;
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
            int32_t eventId;
            uint32_t empty1;
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
            float dsdx;
            float dtdx;
            float dtdy;
            float dsdy;
            float tx;
            float ty;
            float xPrecision;
            float yPrecision;
            float xCursorPosition;
            float yCursorPosition;
            int32_t displayWidth;
            int32_t displayHeight;
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
            bool handled;
            uint8_t empty[7];
            nsecs_t consumeTime; // The time when the event was consumed by the receiving end
            inline size_t size() const { return sizeof(Finished); }
        } finished;
        struct Focus {
            int32_t eventId;
            // The following 3 fields take up 4 bytes total
            bool hasFocus;
            bool inTouchMode;
            uint8_t empty[2];
            inline size_t size() const { return sizeof(Focus); }
        } focus;
        struct Capture {
            int32_t eventId;
            bool pointerCaptureEnabled;
            uint8_t empty[3];
            inline size_t size() const { return sizeof(Capture); }
        } capture;
        struct Drag {
            int32_t eventId;
            float x;
            float y;
            bool isExiting;
            uint8_t empty[3];
            inline size_t size() const { return sizeof(Drag); }
        } drag;
        struct Timeline {
            int32_t eventId;
            uint32_t empty;
            std::array<nsecs_t, GraphicsTimeline::SIZE> graphicsTimeline;
            inline size_t size() const { return sizeof(Timeline); }
            inline bool valid() const { return graphicsTimeline[GraphicsTimeline::PRESENT_TIME] > graphicsTimeline[GraphicsTimeline::GPU_COMPLETED_TIME]; }
        } timeline;
    } __attribute__((aligned(8))) body;
};
