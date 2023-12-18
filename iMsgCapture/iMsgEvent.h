#include "Structs/AInputMsg.h"

class iKeyEvent;
class iMotionEvent;

class iMsgEvent : public AInputMsg {
public:
    iMsgEvent() = default;

    inline size_t size() const { return AInputMsg_getMsgSize(this); }
    inline int32_t type() const { return AInputMsg_getMsgType(this); }
    inline bool isValid(size_t actualSize) const { return AInputMsg_isMsgValid(this, actualSize); }
    inline std::string getSourceString() const { return AInputMsg_getSourceString(this); }

    inline bool isKeyEvent() const { return AInputMsg_isMsgKeyType(this); }
    inline iKeyEvent* getKey() { return reinterpret_cast<iKeyEvent*>(this); }

    inline bool isMotionEvent() const { return AInputMsg_isMsgMotionType(this); }
    inline iMotionEvent* getMotion() { return reinterpret_cast<iMotionEvent*>(this); }    
};

class iKeyEvent : public iMsgEvent {
public:
    iKeyEvent() = default;

    inline nsecs_t getEventTime() const { return AMsgKey_getEventTime(this); }
    inline nsecs_t getDownTime() const { return AMsgKey_getDownTime(this); }
    inline int32_t getDisplayId() const { return AMsgKey_getDisplayId(this); }
    inline int32_t getDeviceId() const { return AMsgKey_getDeviceId(this); }
    inline int32_t getSource() const { return AMsgKey_getSource(this); }
    inline int32_t getAction() const { return AMsgKey_getAction(this); }
    inline int32_t getFlags() const { return AMsgKey_getFlags(this); }
    inline int32_t getKeyCode() const { return AMsgKey_getKeyCode(this); }
    inline int32_t getScanCode() const { return AMsgKey_getScanCode(this); }
    inline int32_t getMetaState() const { return AMsgKey_getMetaState(this); }
    inline int32_t getRepeatCount() const { return AMsgKey_getRepeatCount(this); }
    inline size_t getSize() const { return AMsgKey_getSize(this); }
    inline bool getIsDown(int32_t AkeyCode) const { return AMsgKey_isKeyDown(this, AkeyCode); }
    inline bool getIsUp(int32_t AkeyCode) const { return AMsgKey_isKeyUp(this, AkeyCode); }
    inline std::string getActionString() const { return AMsgKey_getActionString(this); }
};

class iMotionEvent : public iMsgEvent {

    friend class iMsgCapture;

private:
    static inline float& TouchRatioX() { static float _touchRatioX = 0; return _touchRatioX; }
    static inline float& TouchRatioY() { static float _touchRatioY = 0; return _touchRatioY; }
    
    static inline void TouchRatioX(float ratio) { TouchRatioX() = ratio; }
    static inline void TouchRatioY(float ratio) { TouchRatioY() = ratio; }

public:
    iMotionEvent() = default;

    inline nsecs_t getEventTime() const { return AMsgMotion_getEventTime(this); }
    inline nsecs_t getDownTime() const { return AMsgMotion_getDownTime(this); }
    inline int32_t getDisplayId() const { return AMsgMotion_getDisplayId(this); }
    inline int32_t getDeviceId() const { return AMsgMotion_getDeviceId(this); }
    inline int32_t getSource() const { return AMsgMotion_getSource(this); }
    inline int32_t getAction() const { return AMsgMotion_getAction(this); }
    inline int32_t getFlags() const { return AMsgMotion_getFlags(this); }
    inline int32_t getMetaState() const { return AMsgMotion_getMetaState(this); }
    inline size_t getSize() const { return AMsgMotion_getSize(this); }
    inline std::string getActionString() const { return AMsgMotion_getActionString(this); }
    inline int32_t getEdgeFlags() const { return AMsgMotion_getEdgeFlags(this); }
    inline int32_t getActionButton() const { return AMsgMotion_getActionButton(this); }
    inline int32_t getButtonState() const { return AMsgMotion_getButtonState(this); }
    inline uint32_t getPointerCount() const { return AMsgMotion_getPointerCount(this); }
    inline int32_t getActionMasked() const { return AMsgMotion_getActionMasked(this); }
    inline uint32_t getActionIndex() const { return AMsgMotion_getActionIndex(this); }
    inline float getOffsetX() const { return AMsgMotion_getOffsetX(this); }
    inline float getOffsetY() const { return AMsgMotion_getOffsetY(this); }
    inline float getPrecisionX() const { return AMsgMotion_getPrecisionX(this); }
    inline float getPrecisionY() const { return AMsgMotion_getPrecisionY(this); }
    inline int32_t getActionId(uint32_t pointerIndex) const { return AMsgMotion_getActionId(this, pointerIndex); }
    inline int32_t getToolType(uint32_t pointerIndex) const { return AMsgMotion_getToolType(this, pointerIndex); }
    inline std::string getToolTypeString(uint32_t pointerIndex) const { return ToolTypeToString(AMsgMotion_getToolType(this, pointerIndex)); }
    inline float getAxisValue(uint32_t pointerIndex, int32_t axis) const { return AMsgMotion_getAxisValue(this, pointerIndex, axis); }
    inline float getRawX(uint32_t pointerIndex) const { return AMsgMotion_getRawX(this, pointerIndex); }
    inline float getRawY(uint32_t pointerIndex) const { return AMsgMotion_getRawY(this, pointerIndex); }
    inline float getOrientation(uint32_t pointerIndex) const { return AMsgMotion_getOrientation(this, pointerIndex); }

    inline float getX(uint32_t pointerIndex) const
    {
        return TouchRatioX() == 0 ? AMsgMotion_getX(this, pointerIndex) : AMsgMotion_getX(this, pointerIndex) * TouchRatioX();
    }
    inline float getY(uint32_t pointerIndex) const
    {
        return TouchRatioY() == 0 ? AMsgMotion_getY(this, pointerIndex) : AMsgMotion_getY(this, pointerIndex) * TouchRatioY();
    }
};