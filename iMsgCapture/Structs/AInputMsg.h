#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "Common.h"

int AMsg_GetAndroidSDK();

class AInputMsg {};

size_t AInputMsg_Size();
bool AInputMsg_isMsgKeyType(const AInputMsg* iMsg);
bool AInputMsg_isMsgMotionType(const AInputMsg* iMsg);
int32_t AInputMsg_getMsgType(const AInputMsg* iMsg);
size_t AInputMsg_getMsgSize(const AInputMsg* iMsg);
bool AInputMsg_isMsgValid(const AInputMsg* iMsg, size_t actualSize);
std::string AInputMsg_getSourceString(const AInputMsg* iMsg);

nsecs_t AMsgKey_getEventTime(const AInputMsg* iMsg);
nsecs_t AMsgKey_getDownTime(const AInputMsg* iMsg);
int32_t AMsgKey_getDisplayId(const AInputMsg* iMsg);
int32_t AMsgKey_getDeviceId(const AInputMsg* iMsg);
int32_t AMsgKey_getSource(const AInputMsg* iMsg);
int32_t AMsgKey_getAction(const AInputMsg* iMsg);
int32_t AMsgKey_getFlags(const AInputMsg* iMsg);
int32_t AMsgKey_getKeyCode(const AInputMsg* iMsg);
int32_t AMsgKey_getScanCode(const AInputMsg* iMsg);
int32_t AMsgKey_getMetaState(const AInputMsg* iMsg);
int32_t AMsgKey_getRepeatCount(const AInputMsg* iMsg);
size_t AMsgKey_getSize(const AInputMsg* iMsg);
bool AMsgKey_isKeyDown(const AInputMsg* iMsg, int32_t AkeyCode);
bool AMsgKey_isKeyUp(const AInputMsg* iMsg, int32_t AkeyCode);
std::string AMsgKey_getActionString(const AInputMsg* iMsg);

nsecs_t AMsgMotion_getEventTime(const AInputMsg* iMsg);
nsecs_t AMsgMotion_getDownTime(const AInputMsg* iMsg);
int32_t AMsgMotion_getDisplayId(const AInputMsg* iMsg);
int32_t AMsgMotion_getDeviceId(const AInputMsg* iMsg);
int32_t AMsgMotion_getSource(const AInputMsg* iMsg);
int32_t AMsgMotion_getAction(const AInputMsg* iMsg);
void AMsgMotion_setAction(AInputMsg* iMsg, int32_t action);
int32_t AMsgMotion_getFlags(const AInputMsg* iMsg);
int32_t AMsgMotion_getEdgeFlags(const AInputMsg* iMsg);
int32_t AMsgMotion_getActionButton(const AInputMsg* iMsg);
int32_t AMsgMotion_getButtonState(const AInputMsg* iMsg);
int32_t AMsgMotion_getMetaState(const AInputMsg* iMsg);
uint32_t AMsgMotion_getPointerCount(const AInputMsg* iMsg);
int32_t AMsgMotion_getActionMasked(const AInputMsg* iMsg);
uint32_t AMsgMotion_getActionIndex(const AInputMsg* iMsg);
int32_t AMsgMotion_getActionId(const AInputMsg* iMsg, uint32_t pointerIndex);
int32_t AMsgMotion_getToolType(const AInputMsg* iMsg, uint32_t pointerIndex);
float AMsgMotion_getOffsetX(const AInputMsg* iMsg);
float AMsgMotion_getOffsetY(const AInputMsg* iMsg);
float AMsgMotion_getPrecisionX(const AInputMsg* iMsg);
float AMsgMotion_getPrecisionY(const AInputMsg* iMsg);
float AMsgMotion_getAxisValue(const AInputMsg* iMsg, uint32_t pointerIndex, int32_t axis);
float AMsgMotion_getRawX(const AInputMsg* iMsg, uint32_t pointerIndex);
float AMsgMotion_getRawY(const AInputMsg* iMsg, uint32_t pointerIndex);
float AMsgMotion_getX(const AInputMsg* iMsg, uint32_t pointerIndex);
float AMsgMotion_getY(const AInputMsg* iMsg, uint32_t pointerIndex);
float AMsgMotion_getOrientation(const AInputMsg* iMsg, uint32_t pointerIndex);
size_t AMsgMotion_getSize(const AInputMsg* iMsg);
std::string AMsgMotion_getActionString(const AInputMsg* iMsg);

std::string ToolTypeToString(int32_t type);