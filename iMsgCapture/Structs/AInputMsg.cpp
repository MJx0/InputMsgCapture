#include "AInputMsg.h"

#include <map>

#include "versions/InputMsg11.h"
#include "versions/InputMsg12.h"
#include "versions/InputMsg12L.h"
#include "versions/InputMsg13.h"
#include "versions/InputMsg14.h"
#include "versions/InputMsg5_8.h"
#include "versions/InputMsg9_10.h"


int AMsg_GetAndroidSDK()
{
    static int sdk = 0;
    if (sdk > 0)
        return sdk;

    char buf[0xff] = { 0 };
    if (__system_property_get("ro.build.version.sdk", buf))
        sdk = std::atoi(buf);

    return sdk;
}

size_t AInputMsg_Size()
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return sizeof(InputMsg5_8);
    else if (sdk < 30) // android 9-10
        return sizeof(InputMsg9_10);
    else if (sdk == 30) // android 11
        return sizeof(InputMsg11);
    else if (sdk == 31) // android 12
        return sizeof(InputMsg12);
    else if (sdk == 32) // android 12L
        return sizeof(InputMsg12L);
    else if (sdk == 33) // android 13
        return sizeof(InputMsg13);
    else if (sdk == 34) // android 14
        return sizeof(InputMsg14);
    return 0;
}

bool AInputMsg_isMsgKeyType(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->header.type == InputMsg5_8::TYPE_KEY;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->header.type == InputMsg9_10::TYPE_KEY;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->header.type == InputMsg11::TYPE_KEY;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->header.type == InputMsg12::TYPE_KEY;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->header.type == InputMsg12L::TYPE_KEY;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->header.type == InputMsg13::TYPE_KEY;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->header.type == InputMsg14::TYPE_KEY;
    return 0;
}

bool AInputMsg_isMsgMotionType(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->header.type == InputMsg5_8::TYPE_MOTION;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->header.type == InputMsg9_10::TYPE_MOTION;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->header.type == InputMsg11::TYPE_MOTION;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->header.type == InputMsg12::TYPE_MOTION;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->header.type == InputMsg12L::TYPE_MOTION;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->header.type == InputMsg13::TYPE_MOTION;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->header.type == InputMsg14::TYPE_MOTION;
    return 0;
}

int32_t AInputMsg_getMsgType(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->header.type;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->header.type;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->header.type;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->header.type;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->header.type;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->header.type;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->header.type;
    return 0;
}

size_t AInputMsg_getMsgSize(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
    {
        switch (reinterpret_cast<const InputMsg5_8*>(iMsg)->header.type) {
        case InputMsg5_8::TYPE_KEY:
            return sizeof(InputMsg5_8::Header) + reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.size();
        case InputMsg5_8::TYPE_MOTION:
            return sizeof(InputMsg5_8::Header) + reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.size();
        case InputMsg5_8::TYPE_FINISHED:
            return sizeof(InputMsg5_8::Header) + reinterpret_cast<const InputMsg5_8*>(iMsg)->body.finished.size();
        }
        return sizeof(InputMsg5_8::Header);
    } else if (sdk < 30) // android 9-10
    {
        switch (reinterpret_cast<const InputMsg9_10*>(iMsg)->header.type) {
        case InputMsg9_10::TYPE_KEY:
            return sizeof(InputMsg9_10::Header) + reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.size();
        case InputMsg9_10::TYPE_MOTION:
            return sizeof(InputMsg9_10::Header) + reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.size();
        case InputMsg9_10::TYPE_FINISHED:
            return sizeof(InputMsg9_10::Header) + reinterpret_cast<const InputMsg9_10*>(iMsg)->body.finished.size();
        }
        return sizeof(InputMsg9_10::Header);
    } else if (sdk == 30) // android 11
    {
        switch (reinterpret_cast<const InputMsg11*>(iMsg)->header.type) {
        case InputMsg11::TYPE_KEY:
            return sizeof(InputMsg11::Header) + reinterpret_cast<const InputMsg11*>(iMsg)->body.key.size();
        case InputMsg11::TYPE_MOTION:
            return sizeof(InputMsg11::Header) + reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.size();
        case InputMsg11::TYPE_FINISHED:
            return sizeof(InputMsg11::Header) + reinterpret_cast<const InputMsg11*>(iMsg)->body.finished.size();
        case InputMsg11::TYPE_FOCUS:
            return sizeof(InputMsg11::Header) + reinterpret_cast<const InputMsg11*>(iMsg)->body.focus.size();
        }
        return sizeof(InputMsg11::Header);
    } else if (sdk == 31) // android 12
    {
        switch (reinterpret_cast<const InputMsg12*>(iMsg)->header.type) {
        case InputMsg12::TYPE_KEY:
            return sizeof(InputMsg12::Header) + reinterpret_cast<const InputMsg12*>(iMsg)->body.key.size();
        case InputMsg12::TYPE_MOTION:
            return sizeof(InputMsg12::Header) + reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.size();
        case InputMsg12::TYPE_FINISHED:
            return sizeof(InputMsg12::Header) + reinterpret_cast<const InputMsg12*>(iMsg)->body.finished.size();
        case InputMsg12::TYPE_FOCUS:
            return sizeof(InputMsg12::Header) + reinterpret_cast<const InputMsg12*>(iMsg)->body.focus.size();
        case InputMsg12::TYPE_CAPTURE:
            return sizeof(InputMsg12::Header) + reinterpret_cast<const InputMsg12*>(iMsg)->body.capture.size();
        case InputMsg12::TYPE_DRAG:
            return sizeof(InputMsg12::Header) + reinterpret_cast<const InputMsg12*>(iMsg)->body.drag.size();
        case InputMsg12::TYPE_TIMELINE:
            return sizeof(InputMsg12::Header) + reinterpret_cast<const InputMsg12*>(iMsg)->body.timeline.size();
        }
        return sizeof(InputMsg12::Header);
    } else if (sdk == 32) // android 12L
    {
        switch (reinterpret_cast<const InputMsg12L*>(iMsg)->header.type) {
        case InputMsg12L::TYPE_KEY:
            return sizeof(InputMsg12L::Header) + reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.size();
        case InputMsg12L::TYPE_MOTION:
            return sizeof(InputMsg12L::Header) + reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.size();
        case InputMsg12L::TYPE_FINISHED:
            return sizeof(InputMsg12L::Header) + reinterpret_cast<const InputMsg12L*>(iMsg)->body.finished.size();
        case InputMsg12L::TYPE_FOCUS:
            return sizeof(InputMsg12L::Header) + reinterpret_cast<const InputMsg12L*>(iMsg)->body.focus.size();
        case InputMsg12L::TYPE_CAPTURE:
            return sizeof(InputMsg12L::Header) + reinterpret_cast<const InputMsg12L*>(iMsg)->body.capture.size();
        case InputMsg12L::TYPE_DRAG:
            return sizeof(InputMsg12L::Header) + reinterpret_cast<const InputMsg12L*>(iMsg)->body.drag.size();
        case InputMsg12L::TYPE_TIMELINE:
            return sizeof(InputMsg12L::Header) + reinterpret_cast<const InputMsg12L*>(iMsg)->body.timeline.size();
        }
        return sizeof(InputMsg12L::Header);
    } else if (sdk == 33) // android 13
    {
        switch (reinterpret_cast<const InputMsg13*>(iMsg)->header.type) {
        case InputMsg13::TYPE_KEY:
            return sizeof(InputMsg13::Header) + reinterpret_cast<const InputMsg13*>(iMsg)->body.key.size();
        case InputMsg13::TYPE_MOTION:
            return sizeof(InputMsg13::Header) + reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.size();
        case InputMsg13::TYPE_FINISHED:
            return sizeof(InputMsg13::Header) + reinterpret_cast<const InputMsg13*>(iMsg)->body.finished.size();
        case InputMsg13::TYPE_FOCUS:
            return sizeof(InputMsg13::Header) + reinterpret_cast<const InputMsg13*>(iMsg)->body.focus.size();
        case InputMsg13::TYPE_CAPTURE:
            return sizeof(InputMsg13::Header) + reinterpret_cast<const InputMsg13*>(iMsg)->body.capture.size();
        case InputMsg13::TYPE_DRAG:
            return sizeof(InputMsg13::Header) + reinterpret_cast<const InputMsg13*>(iMsg)->body.drag.size();
        case InputMsg13::TYPE_TIMELINE:
            return sizeof(InputMsg13::Header) + reinterpret_cast<const InputMsg13*>(iMsg)->body.timeline.size();
        case InputMsg13::TYPE_TOUCH_MODE:
            return sizeof(InputMsg13::Header) + reinterpret_cast<const InputMsg13*>(iMsg)->body.touchMode.size();
        }
        return sizeof(InputMsg13::Header);
    } else if (sdk == 34) // android 14
    {
        switch (reinterpret_cast<const InputMsg14*>(iMsg)->header.type) {
        case InputMsg14::TYPE_KEY:
            return sizeof(InputMsg14::Header) + reinterpret_cast<const InputMsg14*>(iMsg)->body.key.size();
        case InputMsg14::TYPE_MOTION:
            return sizeof(InputMsg14::Header) + reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.size();
        case InputMsg14::TYPE_FINISHED:
            return sizeof(InputMsg14::Header) + reinterpret_cast<const InputMsg14*>(iMsg)->body.finished.size();
        case InputMsg14::TYPE_FOCUS:
            return sizeof(InputMsg14::Header) + reinterpret_cast<const InputMsg14*>(iMsg)->body.focus.size();
        case InputMsg14::TYPE_CAPTURE:
            return sizeof(InputMsg14::Header) + reinterpret_cast<const InputMsg14*>(iMsg)->body.capture.size();
        case InputMsg14::TYPE_DRAG:
            return sizeof(InputMsg14::Header) + reinterpret_cast<const InputMsg14*>(iMsg)->body.drag.size();
        case InputMsg14::TYPE_TIMELINE:
            return sizeof(InputMsg14::Header) + reinterpret_cast<const InputMsg14*>(iMsg)->body.timeline.size();
        case InputMsg14::TYPE_TOUCH_MODE:
            return sizeof(InputMsg14::Header) + reinterpret_cast<const InputMsg14*>(iMsg)->body.touchMode.size();
        }
        return sizeof(InputMsg14::Header);
    }

    return 0;
}

bool AInputMsg_isMsgValid(const AInputMsg* iMsg, size_t actualSize)
{
    if (AInputMsg_getMsgSize(iMsg) != actualSize)
        return false;

    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
    {
        switch (reinterpret_cast<const InputMsg5_8*>(iMsg)->header.type) {
        case InputMsg5_8::TYPE_KEY:
            return true;
        case InputMsg5_8::TYPE_MOTION:
            return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.valid();
        case InputMsg5_8::TYPE_FINISHED:
            return true;
        }
    } else if (sdk < 30) // android 9-10
    {
        switch (reinterpret_cast<const InputMsg9_10*>(iMsg)->header.type) {
        case InputMsg9_10::TYPE_KEY:
            return true;
        case InputMsg9_10::TYPE_MOTION:
            return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.valid();
        case InputMsg9_10::TYPE_FINISHED:
            return true;
        }
    } else if (sdk == 30) // android 11
    {
        switch (reinterpret_cast<const InputMsg11*>(iMsg)->header.type) {
        case InputMsg11::TYPE_KEY:
            return true;
        case InputMsg11::TYPE_MOTION:
            return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.valid();
        case InputMsg11::TYPE_FINISHED:
        case InputMsg11::TYPE_FOCUS:
            return true;
        }
    } else if (sdk == 31) // android 12
    {
        switch (reinterpret_cast<const InputMsg12*>(iMsg)->header.type) {
        case InputMsg12::TYPE_KEY:
            return true;
        case InputMsg12::TYPE_MOTION:
            return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.valid();
        case InputMsg12::TYPE_FINISHED:
        case InputMsg12::TYPE_FOCUS:
        case InputMsg12::TYPE_CAPTURE:
        case InputMsg12::TYPE_DRAG:
            return true;
        case InputMsg12::TYPE_TIMELINE:
            return reinterpret_cast<const InputMsg12*>(iMsg)->body.timeline.valid();
        }
    } else if (sdk == 32) // android 12L
    {
        switch (reinterpret_cast<const InputMsg12L*>(iMsg)->header.type) {
        case InputMsg12L::TYPE_KEY:
            return true;
        case InputMsg12L::TYPE_MOTION:
            return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.valid();
        case InputMsg12L::TYPE_FINISHED:
        case InputMsg12L::TYPE_FOCUS:
        case InputMsg12L::TYPE_CAPTURE:
        case InputMsg12L::TYPE_DRAG:
            return true;
        case InputMsg12L::TYPE_TIMELINE:
            return reinterpret_cast<const InputMsg12L*>(iMsg)->body.timeline.valid();
        }
    } else if (sdk == 33) // android 13
    {
        switch (reinterpret_cast<const InputMsg13*>(iMsg)->header.type) {
        case InputMsg13::TYPE_KEY:
            return true;
        case InputMsg13::TYPE_MOTION:
            return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.valid();
        case InputMsg13::TYPE_FINISHED:
        case InputMsg13::TYPE_FOCUS:
        case InputMsg13::TYPE_CAPTURE:
        case InputMsg13::TYPE_DRAG:
        case InputMsg13::TYPE_TOUCH_MODE:
            return true;
        case InputMsg13::TYPE_TIMELINE:
            return reinterpret_cast<const InputMsg13*>(iMsg)->body.timeline.valid();
        }
    } else if (sdk == 34) // android 14
    {
        switch (reinterpret_cast<const InputMsg14*>(iMsg)->header.type) {
        case InputMsg14::TYPE_KEY:
            return true;
        case InputMsg14::TYPE_MOTION:
            return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.valid();
        case InputMsg14::TYPE_FINISHED:
        case InputMsg14::TYPE_FOCUS:
        case InputMsg14::TYPE_CAPTURE:
        case InputMsg14::TYPE_DRAG:
        case InputMsg14::TYPE_TOUCH_MODE:
            return true;
        case InputMsg14::TYPE_TIMELINE:
            return reinterpret_cast<const InputMsg14*>(iMsg)->body.timeline.valid();
        }
    }
    return false;
}

std::string AInputMsg_getSourceString(const AInputMsg* iMsg)
{
    int32_t source = AINPUT_SOURCE_UNKNOWN;

    if (AInputMsg_isMsgKeyType(iMsg))
        source = AMsgKey_getSource(iMsg);
    else if (AInputMsg_isMsgMotionType(iMsg))
        source = AMsgMotion_getSource(iMsg);

    if (source == AINPUT_SOURCE_UNKNOWN)
        return "UNKNOWN";

    if (source == static_cast<int32_t>(AINPUT_SOURCE_ANY))
        return "ANY";

    static const std::map<int32_t, const char*> SOURCES {
        { AINPUT_SOURCE_KEYBOARD, "KEYBOARD" },
        { AINPUT_SOURCE_DPAD, "DPAD" },
        { AINPUT_SOURCE_GAMEPAD, "GAMEPAD" },
        { AINPUT_SOURCE_TOUCHSCREEN, "TOUCHSCREEN" },
        { AINPUT_SOURCE_MOUSE, "MOUSE" },
        { AINPUT_SOURCE_STYLUS, "STYLUS" },
        { AINPUT_SOURCE_BLUETOOTH_STYLUS, "BLUETOOTH_STYLUS" },
        { AINPUT_SOURCE_TRACKBALL, "TRACKBALL" },
        { AINPUT_SOURCE_MOUSE_RELATIVE, "MOUSE_RELATIVE" },
        { AINPUT_SOURCE_TOUCHPAD, "TOUCHPAD" },
        { AINPUT_SOURCE_TOUCH_NAVIGATION, "TOUCH_NAVIGATION" },
        { AINPUT_SOURCE_JOYSTICK, "JOYSTICK" },
        { AINPUT_SOURCE_HDMI, "HDMI" },
        { AINPUT_SOURCE_SENSOR, "SENSOR" },
        { AINPUT_SOURCE_ROTARY_ENCODER, "ROTARY_ENCODER" },
    };

    std::string result;
    for (const auto& [source_entry, str] : SOURCES) {
        if ((source & source_entry) == source_entry) {
            if (!result.empty())
                result += " | ";

            result += str;
        }
    }

    if (result.empty()) {
        char buf[0xff] = { 0 };
        snprintf(buf, 0xff, "0x%08x", source);
        result = buf;
    }
    return result;
}

nsecs_t AMsgKey_getEventTime(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.eventTime;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.eventTime;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.eventTime;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.eventTime;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.eventTime;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.eventTime;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.eventTime;
    return 0;
}

nsecs_t AMsgKey_getDownTime(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.downTime;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.downTime;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.downTime;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.downTime;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.downTime;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.downTime;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.downTime;
    return 0;
}

int32_t AMsgKey_getDisplayId(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return 0;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.displayId;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.displayId;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.displayId;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.displayId;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.displayId;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.displayId;
    return 0;
}

int32_t AMsgKey_getDeviceId(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.deviceId;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.deviceId;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.deviceId;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.deviceId;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.deviceId;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.deviceId;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.deviceId;
    return 0;
}

int32_t AMsgKey_getSource(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.source;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.source;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.source;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.source;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.source;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.source;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.source;
    return 0;
}

int32_t AMsgKey_getAction(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.action;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.action;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.action;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.action;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.action;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.action;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.action;
    return 0;
}

int32_t AMsgKey_getFlags(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.flags;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.flags;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.flags;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.flags;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.flags;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.flags;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.flags;
    return 0;
}

int32_t AMsgKey_getKeyCode(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.keyCode;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.keyCode;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.keyCode;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.keyCode;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.keyCode;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.keyCode;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.keyCode;
    return 0;
}

int32_t AMsgKey_getScanCode(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.scanCode;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.scanCode;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.scanCode;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.scanCode;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.scanCode;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.scanCode;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.scanCode;
    return 0;
}

int32_t AMsgKey_getMetaState(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.metaState;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.metaState;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.metaState;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.metaState;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.metaState;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.metaState;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.metaState;
    return 0;
}

int32_t AMsgKey_getRepeatCount(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.repeatCount;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.repeatCount;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.repeatCount;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.repeatCount;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.repeatCount;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.repeatCount;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.repeatCount;
    return 0;
}

size_t AMsgKey_getSize(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.key.size();
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.key.size();
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.key.size();
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.key.size();
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.key.size();
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.key.size();
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.key.size();
    return 0;
}

bool AMsgKey_isKeyDown(const AInputMsg* iMsg, int32_t AkeyCode)
{
    return AkeyCode == AMsgKey_getKeyCode(iMsg) && AMsgKey_getAction(iMsg) == AKEY_EVENT_ACTION_DOWN;
}

bool AMsgKey_isKeyUp(const AInputMsg* iMsg, int32_t AkeyCode)
{
    return AkeyCode == AMsgKey_getKeyCode(iMsg) && AMsgKey_getAction(iMsg) == AKEY_EVENT_ACTION_UP;
}

std::string AMsgKey_getActionString(const AInputMsg* iMsg)
{
    switch (AMsgKey_getAction(iMsg)) {
    case AKEY_EVENT_ACTION_DOWN:
        return "DOWN";
    case AKEY_EVENT_ACTION_UP:
        return "UP";
    case AKEY_EVENT_ACTION_MULTIPLE:
        return "MULTIPLE";
    }
    return "UNKNOWN";
}

nsecs_t AMsgMotion_getEventTime(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.eventTime;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.eventTime;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.eventTime;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.eventTime;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.eventTime;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.eventTime;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.eventTime;
    return 0;
}

nsecs_t AMsgMotion_getDownTime(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.downTime;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.downTime;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.downTime;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.downTime;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.downTime;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.downTime;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.downTime;
    return 0;
}

int32_t AMsgMotion_getDisplayId(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return 0;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.displayId;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.displayId;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.displayId;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.displayId;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.displayId;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.displayId;
    return 0;
}

int32_t AMsgMotion_getDeviceId(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.deviceId;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.deviceId;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.deviceId;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.deviceId;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.deviceId;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.deviceId;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.deviceId;
    return 0;
}

int32_t AMsgMotion_getSource(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.source;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.source;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.source;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.source;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.source;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.source;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.source;
    return 0;
}

int32_t AMsgMotion_getFlags(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.flags;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.flags;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.flags;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.flags;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.flags;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.flags;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.flags;
    return 0;
}

int32_t AMsgMotion_getEdgeFlags(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.edgeFlags;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.edgeFlags;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.edgeFlags;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.edgeFlags;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.edgeFlags;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.edgeFlags;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.edgeFlags;
    return 0;
}

int32_t AMsgMotion_getAction(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.action;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.action;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.action;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.action;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.action;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.action;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.action;
    return 0;
}

void AMsgMotion_setAction(AInputMsg* iMsg, int32_t action)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        reinterpret_cast<InputMsg5_8*>(iMsg)->body.motion.action = action;
    else if (sdk < 30) // android 9-10
        reinterpret_cast<InputMsg9_10*>(iMsg)->body.motion.action = action;
    else if (sdk == 30) // android 11
        reinterpret_cast<InputMsg11*>(iMsg)->body.motion.action = action;
    else if (sdk == 31) // android 12
        reinterpret_cast<InputMsg12*>(iMsg)->body.motion.action = action;
    else if (sdk == 32) // android 12L
        reinterpret_cast<InputMsg12L*>(iMsg)->body.motion.action = action;
    else if (sdk == 33) // android 13
        reinterpret_cast<InputMsg13*>(iMsg)->body.motion.action = action;
    else if (sdk == 34) // android 14
        reinterpret_cast<InputMsg14*>(iMsg)->body.motion.action = action;
}

int32_t AMsgMotion_getActionButton(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.actionButton;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.actionButton;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.actionButton;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.actionButton;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.actionButton;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.actionButton;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.actionButton;
    return 0;
}

int32_t AMsgMotion_getButtonState(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.buttonState;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.buttonState;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.buttonState;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.buttonState;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.buttonState;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.buttonState;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.buttonState;
    return 0;
}

int32_t AMsgMotion_getMetaState(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.metaState;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.metaState;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.metaState;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.metaState;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.metaState;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.metaState;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.metaState;
    return 0;
}

uint32_t AMsgMotion_getPointerCount(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.pointerCount;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.pointerCount;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.pointerCount;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.pointerCount;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.pointerCount;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.pointerCount;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.pointerCount;
    return 0;
}

uint32_t AMsgMotion_getActionIndex(const AInputMsg* iMsg)
{
    return (AMsgKey_getAction(iMsg) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
}

int32_t AMsgMotion_getActionMasked(const AInputMsg* iMsg)
{
    return AMsgKey_getAction(iMsg) & AMOTION_EVENT_ACTION_MASK;
}

int32_t AMsgMotion_getActionId(const AInputMsg* iMsg, uint32_t pointerIndex)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.pointers[pointerIndex].properties.id;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.pointers[pointerIndex].properties.id;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.pointers[pointerIndex].properties.id;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.pointers[pointerIndex].properties.id;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.pointers[pointerIndex].properties.id;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.pointers[pointerIndex].properties.id;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.pointers[pointerIndex].properties.id;
    return 0;
}

int32_t AMsgMotion_getToolType(const AInputMsg* iMsg, uint32_t pointerIndex)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.pointers[pointerIndex].properties.toolType;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.pointers[pointerIndex].properties.toolType;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.pointers[pointerIndex].properties.toolType;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.pointers[pointerIndex].properties.toolType;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.pointers[pointerIndex].properties.toolType;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.pointers[pointerIndex].properties.toolType;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.pointers[pointerIndex].properties.toolType;
    return 0;
}

float AMsgMotion_getOffsetX(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.xOffset;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.xOffset;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.xOffset;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.tx;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.tx;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.tx;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.tx;
    return 0;
}

float AMsgMotion_getOffsetY(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.yOffset;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.yOffset;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.yOffset;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.ty;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.ty;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.ty;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.ty;
    return 0;
}

float AMsgMotion_getPrecisionX(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.xPrecision;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.xPrecision;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.xPrecision;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.xPrecision;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.xPrecision;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.xPrecision;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.xPrecision;
    return 0;
}

float AMsgMotion_getPrecisionY(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.yPrecision;
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.yPrecision;
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.yPrecision;
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.yPrecision;
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.yPrecision;
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.yPrecision;
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.yPrecision;
    return 0;
}

float AMsgMotion_getAxisValue(const AInputMsg* iMsg, uint32_t pointerIndex, int32_t axis)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(axis);
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(axis);
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(axis);
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(axis);
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(axis);
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(axis);
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(axis);
    return 0;
}

float AMsgMotion_getRawX(const AInputMsg* iMsg, uint32_t pointerIndex)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    return 0;
}

float AMsgMotion_getRawY(const AInputMsg* iMsg, uint32_t pointerIndex)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    return 0;
}

float AMsgMotion_getX(const AInputMsg* iMsg, uint32_t pointerIndex)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.xOffset + reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.xOffset + reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.xOffset + reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.tx + reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.tx + reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.tx + reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.tx + reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_X);
    return 0;
}

float AMsgMotion_getY(const AInputMsg* iMsg, uint32_t pointerIndex)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.yOffset + reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.yOffset + reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.yOffset + reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.ty + reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.ty + reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.ty + reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.ty + reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_Y);
    return 0;
}

float AMsgMotion_getOrientation(const AInputMsg* iMsg, uint32_t pointerIndex)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_ORIENTATION);
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_ORIENTATION);
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_ORIENTATION);
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_ORIENTATION);
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_ORIENTATION);
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_ORIENTATION);
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.pointers[pointerIndex].coords.getAxisValue(AMOTION_EVENT_AXIS_ORIENTATION);
    return 0;
}

size_t AMsgMotion_getSize(const AInputMsg* iMsg)
{
    const static int sdk = AMsg_GetAndroidSDK();
    if (sdk < 28) // android 5-8
        return reinterpret_cast<const InputMsg5_8*>(iMsg)->body.motion.size();
    else if (sdk < 30) // android 9-10
        return reinterpret_cast<const InputMsg9_10*>(iMsg)->body.motion.size();
    else if (sdk == 30) // android 11
        return reinterpret_cast<const InputMsg11*>(iMsg)->body.motion.size();
    else if (sdk == 31) // android 12
        return reinterpret_cast<const InputMsg12*>(iMsg)->body.motion.size();
    else if (sdk == 32) // android 12L
        return reinterpret_cast<const InputMsg12L*>(iMsg)->body.motion.size();
    else if (sdk == 33) // android 13
        return reinterpret_cast<const InputMsg13*>(iMsg)->body.motion.size();
    else if (sdk == 34) // android 14
        return reinterpret_cast<const InputMsg14*>(iMsg)->body.motion.size();
    return 0;
}

std::string AMsgMotion_getActionString(const AInputMsg* iMsg)
{
    switch (AMsgMotion_getActionMasked(iMsg)) {
    case AMOTION_EVENT_ACTION_DOWN:
        return "DOWN";
    case AMOTION_EVENT_ACTION_UP:
        return "UP";
    case AMOTION_EVENT_ACTION_MOVE:
        return "MOVE";
    case AMOTION_EVENT_ACTION_CANCEL:
        return "CANCEL";
    case AMOTION_EVENT_ACTION_OUTSIDE:
        return "OUTSIDE";
    case AMOTION_EVENT_ACTION_POINTER_DOWN:
        return "POINTER_DOWN(" + std::to_string(AMsgMotion_getActionIndex(iMsg)) + ")";
    case AMOTION_EVENT_ACTION_POINTER_UP:
        return "POINTER_UP(" + std::to_string(AMsgMotion_getActionIndex(iMsg)) + ")";
    case AMOTION_EVENT_ACTION_HOVER_MOVE:
        return "HOVER_MOVE";
    case AMOTION_EVENT_ACTION_SCROLL:
        return "SCROLL";
    case AMOTION_EVENT_ACTION_HOVER_ENTER:
        return "HOVER_ENTER";
    case AMOTION_EVENT_ACTION_HOVER_EXIT:
        return "HOVER_EXIT";
    case AMOTION_EVENT_ACTION_BUTTON_PRESS:
        return "BUTTON_PRESS";
    case AMOTION_EVENT_ACTION_BUTTON_RELEASE:
        return "BUTTON_RELEASE";
    }
    return std::to_string(AMsgMotion_getAction(iMsg));
}

std::string ToolTypeToString(int32_t type)
{
    switch (type) {
    case AMOTION_EVENT_TOOL_TYPE_FINGER:
        return "FINGER";
    case AMOTION_EVENT_TOOL_TYPE_STYLUS:
        return "STYLUS";
    case AMOTION_EVENT_TOOL_TYPE_MOUSE:
        return "MOUSE";
    case AMOTION_EVENT_TOOL_TYPE_ERASER:
        return "ERASER";
    case AMOTION_EVENT_TOOL_TYPE_PALM:
        return "PALM";
    }
    return "UNKNOWN";
}