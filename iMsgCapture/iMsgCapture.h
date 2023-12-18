#pragma once

#include <sys/socket.h>
#include <sys/stat.h>

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <algorithm>
#include <set>

#include <android/log.h>

#include "iMsgEvent.h"

// use vm_read in case memcpy causes crashes when scanning memory
// might be slower
#define iMSG_USE_VMREAD 0

#define iMSG_EINTR_RETRY(exp) ({         \
    __typeof__(exp) _rc;                   \
    do {                                   \
        _rc = (exp);                       \
    } while (_rc == -1 && errno == EINTR); \
    _rc; })

#define iMSG_DEBUG

#define iMSG_LOG_TAG "iMsgCapture"
#define iMSG_LOGI(fmt, ...) ((void)__android_log_print(ANDROID_LOG_INFO, iMSG_LOG_TAG, fmt, ##__VA_ARGS__))
#define iMSG_LOGE(fmt, ...) ((void)__android_log_print(ANDROID_LOG_ERROR, iMSG_LOG_TAG, fmt, ##__VA_ARGS__))
#define iMSG_LOGW(fmt, ...) ((void)__android_log_print(ANDROID_LOG_WARN, iMSG_LOG_TAG, fmt, ##__VA_ARGS__))

enum EIMSG_STATUS : int32_t {
    IMSG_STATUS_NONE,
    IMSG_STATUS_UNINITIALIZED,
    IMSG_STATUS_EMPTY,
    IMSG_STATUS_OK,
    IMSG_STATUS_INVALID,
    IMSG_STATUS_WOULD_BLOCK,
    IMSG_STATUS_EOF,
    IMSG_STATUS_DEAD,
    IMSG_STATUS_TIMEOUT
};

const char* iMsgStatusString(EIMSG_STATUS status);


class iMsgCapture {
    friend iMotionEvent;

private:
    int _consumerFd;
    iMsgEvent *_pConsumerInputMsg;
    std::vector<char> _iMsgBuf;
    std::function<void(iMsgEvent* iMsg)> _callback;
    int _updateInterval;

public:
    iMsgCapture();

    inline static iMsgCapture& instance() { static iMsgCapture im{}; return im; }

    inline const std::function<void(iMsgEvent* iMsg)>& getCallback() { return _callback; }
    inline void setCallback(const std::function<void(iMsgEvent* iMsg)>& callback) { _callback = callback; }

    inline void setUpdateInterval(int micros) { _updateInterval = micros; }
    inline int getUpdateInterval() const { return _updateInterval; }

    inline float getTouchRatioX() { return iMotionEvent::TouchRatioX(); }
    inline float getTouchRatioY() { return iMotionEvent::TouchRatioY(); }

    inline void setTouchRatioX(float ratio) { iMotionEvent::TouchRatioX(ratio); }
    inline void setTouchRatioY(float ratio) { iMotionEvent::TouchRatioY(ratio); }

    EIMSG_STATUS runLoop(const std::vector<std::string>& gameActivites, bool retryOnFailure, int retrySleepSec);
    void runThread(const std::vector<std::string>& gameActivites, bool retryOnFailure, int retrySleepSec);

    std::set<int> getAnonUnixSockets() const;

    bool findByMallocScan(const std::set<int>& sockets, const std::vector<std::string>& gameActivites, int *outFd, iMsgEvent **outMsgPtr) const;
};