#pragma once

#include <stdint.h>
#include <inttypes.h>

#include <android/input.h>
#include <android/keycodes.h>

#include "BitSet.h"

using nsecs_t = int64_t; // nano-seconds

/*
 * Maximum number of pointers supported per motion event.
 * Smallest number of pointers is 1.
 * (We want at least 10 but some touch controllers obstensibly configured for 10 pointers
 * will occasionally emit 11.  There is not much harm making this constant bigger.)
 */
#define MAX_POINTERS 16
/*
 * Maximum number of samples supported per motion event.
 */
#define MAX_SAMPLES UINT16_MAX
/*
 * Maximum pointer id value supported in a motion event.
 * Smallest pointer id is 0.
 * (This is limited by our use of BitSet32 to track pointer assignments.)
 */
#define MAX_POINTER_ID 31

/*
 * Pointer coordinate data.
 */

struct PointerCoords {
    enum { MAX_AXES = 30 }; // 30 so that sizeof(PointerCoords) == 128
    // Bitfield of axes that are present in this structure.
    uint64_t bits __attribute__((aligned(8)));
    // Values of axes that are stored in this structure packed in order by axis id
    // for each axis that is present in the structure according to 'bits'.
    float values[MAX_AXES];

    inline float getAxisValue(int32_t axis) const { return (axis < 0 || axis > 63 || !BitSet64::hasBit(bits, axis)) ? 0 : values[BitSet64::getIndexOfBit(bits, axis)]; }
    inline float getX() const { return getAxisValue(AMOTION_EVENT_AXIS_X); }
    inline float getY() const { return getAxisValue(AMOTION_EVENT_AXIS_Y); }
};

/*
 * Pointer property data.
 */
struct PointerProperties {
    // The id of the pointer.
    int32_t id;
    // The pointer tool type.
    int32_t toolType;

    inline bool operator==(const PointerProperties& other) const { return id == other.id && toolType == other.toolType; }
    inline bool operator!=(const PointerProperties& other) const { return id != other.id || toolType != other.toolType; }
};

/**
 * Portion of FrameMetrics timeline of interest to input code.
 */
namespace GraphicsTimeline {
    /** Time when the app sent the buffer to SurfaceFlinger. */
    static constexpr size_t GPU_COMPLETED_TIME = 0;

    /** Time when the frame was presented on the display */
    static constexpr size_t PRESENT_TIME = 1;

    /** Total size of the 'GraphicsTimeline' array. Must always be last. */
    static constexpr size_t SIZE = 2;
};