#include "iMsgCapture.h"

struct InputChannel5_8
{
    uintptr_t refbase1 = 0, refbase2 = 0;
    const char *name = nullptr;
    int fd = 0;
};

struct InputChannel9_11
{
    uintptr_t refbase1 = 0, refbase2 = 0;
    std::string name;
    int fd = 0;
};

struct InputChannel12_14
{
    uintptr_t parcel = 0;
    std::string name;
    int fd = 0;
};

struct map_info_t {
    uintptr_t start, end, offset;
    std::string perms;
    std::string name;
    map_info_t()
        : start(0)
        , end(0)
        , offset(0)
    {
    }
    map_info_t(uintptr_t _start, uintptr_t _end, uintptr_t _offset, const std::string &_perms, const std::string &_name)
        : start(_start)
        , end(_end)
        , offset(_offset)
        , perms(_perms)
        , name(_name)
    {
    }

    inline bool operator==(const map_info_t& rhs) const { return start == rhs.start && end == rhs.end; }
    inline bool operator!=(const map_info_t& rhs) const { return start != rhs.start || end != rhs.end; }
};

struct ref_info_t {
    map_info_t map_info;
    uintptr_t ptr;
    ref_info_t()
        : ptr(0)
    {
    }
    ref_info_t(const map_info_t& _map_info, uintptr_t _ptr)
        : map_info(_map_info)
        , ptr(_ptr)
    {
    }

    inline bool operator==(const ref_info_t& rhs) const { return ptr == rhs.ptr; }
    inline bool operator!=(const ref_info_t& rhs) const { return ptr != rhs.ptr; }
};

static inline bool is_page_valid(void *p)
{
    const size_t page_size = sysconf(_SC_PAGE_SIZE);
    unsigned char vec[2] = { 0 };
    return (p != nullptr && mincore((void*)(uintptr_t(p) & ~(page_size-1)), page_size, vec) == 0 && (vec[0] & 1) != 0);
}

int8_t property_get_bool(const char* key, int8_t default_value);

std::string _mallocPathname();

void _walkMaps(const std::function<bool(const map_info_t &map)> &callback);
std::vector<ref_info_t> _mallocScan(const void* data, size_t size);

size_t vm_read(uintptr_t offset, void *buffer, size_t len);

std::vector<int*> _scanChannelFds(const std::set<int>& sockets, const std::string &activity);
uintptr_t _scanInputMessage(int* pFd);

const char* iMsgStatusString(EIMSG_STATUS status)
{
    switch (status) {
    case IMSG_STATUS_NONE:
        return "NONE";
    case IMSG_STATUS_UNINITIALIZED:
        return "UNINITIALIZED";
    case IMSG_STATUS_EMPTY:
        return "EMPTY";
    case IMSG_STATUS_OK:
        return "OK";
    case IMSG_STATUS_INVALID:
        return "INVALID";
    case IMSG_STATUS_WOULD_BLOCK:
        return "WOULD_BLOCK:";
    case IMSG_STATUS_EOF:
        return "EOF";
    case IMSG_STATUS_DEAD:
        return "DEAD";
    case IMSG_STATUS_TIMEOUT:
        return "TIMEOUT";
    }
    return "UNKNOWN";
}

iMsgCapture::iMsgCapture()
{
    _consumerFd = 0;
    _pConsumerInputMsg = nullptr;

    _callback = nullptr;

    _iMsgBuf = std::vector<char>(AInputMsg_Size(), 0);

    _updateInterval = 1000;
}

std::set<int> iMsgCapture::getAnonUnixSockets() const
{
    // SO_DOMAIN = AF_UNIX
    // SO_TYPE = SOCK_SEQPACKET
    // SO_PROTOCOL = 0
    // (SO_RCVBUF == SO_SNDBUF)

    std::set<int> socketsFds;

    char cFdDir[0xff] = { 0 };
    snprintf(cFdDir, sizeof(cFdDir), "/proc/%d/fd/", getpid());
    if (auto dir = opendir(cFdDir))
    {
        while (auto f = readdir(dir))
        {
            if (f->d_name[0] == '.' || f->d_type != DT_LNK)
                continue;

            std::string link = cFdDir;
            link += f->d_name;
            char cRealPath[0xff] = { 0 };
            if (readlink(link.c_str(), cRealPath, 0xff) == -1 || !strstr(cRealPath, "socket:["))
                continue;

            int fd = std::atoi(f->d_name);

            int opt_val = 0;
            socklen_t opt_len = sizeof(int);

            ((void)getsockopt(fd, SOL_SOCKET, SO_DOMAIN, &opt_val, &opt_len));
            if (opt_val != AF_UNIX)
                continue;

            ((void)getsockopt(fd, SOL_SOCKET, SO_TYPE, &opt_val, &opt_len));
            if (opt_val != SOCK_SEQPACKET)
                continue;

            ((void)getsockopt(fd, SOL_SOCKET, SO_PROTOCOL, &opt_val, &opt_len));
            if (opt_val != 0)
                continue;

            // SO_SNDBUF == SO_RCVBUF

            ((void)getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &opt_val, &opt_len));
            int bufSz = opt_val;

            ((void)getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &opt_val, &opt_len));
            if (opt_val != bufSz)
                continue;

            socketsFds.insert(fd);
        }

        closedir(dir);
    }

    return socketsFds;
}

bool iMsgCapture::findByMallocScan(const std::set<int>& sockets, const std::vector<std::string> &gameActivites, int *outFd, iMsgEvent **outMsgPtr) const
{
    if (gameActivites.empty())
        return 0;

    // Malloc Scan
    // regions rw-p [anon:libc_malloc]
    // find ref to string "$hash_hex $app_pkg/$app_main_activity (client)"
    // Example "c1ec9d3 com.kiloo.subwaysurf/com.sybogames.chili.multidex.ChiliMultidexSupportActivity (client)"
    // ref + sizeof(uintptr_t) = fd ptr

    const static std::string mallocPathname = _mallocPathname();
    iMSG_LOGI("mallocScan: Pathname = \"%s\"", mallocPathname.c_str());

    for (const auto& act : gameActivites)
    {
        auto pFds = _scanChannelFds(sockets, act);
        if (pFds.empty())
        {
            iMSG_LOGI("mallocScan: Couldn't find any socket for activity \"%s\".", act.c_str());
            continue;
        }

        for (const auto &pFd : pFds)
        {
            iMSG_LOGI("mallocScan: Scanning for InputMessage with fd (%d)...", *pFd);
            auto pIMsg = _scanInputMessage(pFd);
            if (pIMsg > 0)
            {
                *outFd = *pFd;
                *outMsgPtr = reinterpret_cast<iMsgEvent*>(pIMsg);
                return true;
            }
        }
    }

    return false;
}

EIMSG_STATUS iMsgCapture::runLoop(const std::vector<std::string>& gameActivites, bool retryOnFailure, int retrySleepSec)
{
    EIMSG_STATUS lastStatus = IMSG_STATUS_NONE;
    char b = 0;
    int n = 0;
    for (;;)
    {
        if (lastStatus != IMSG_STATUS_NONE && lastStatus != IMSG_STATUS_OK && lastStatus != IMSG_STATUS_WOULD_BLOCK)
        {
            if (!retryOnFailure)
                break;

            if (retrySleepSec > 0)
            {
                iMSG_LOGI("runLoop: Retrying after %d seconds...", retrySleepSec);
                sleep(retrySleepSec);
            }

            _consumerFd = 0;
            _pConsumerInputMsg = nullptr;
        }

        if (!_consumerFd)
        {
            iMSG_LOGI("runLoop: Initializing...");

            auto sockets = getAnonUnixSockets();
            iMSG_LOGI("runLoop: Unix sockets count %zd.", sockets.size());

            // try the more accurate method first
            iMSG_LOGI("runLoop: Trying to find InputComsumer by malloc scan...");

            if (!findByMallocScan(sockets, gameActivites, &_consumerFd, &_pConsumerInputMsg))
            {
                iMSG_LOGE("runLoop: MallocScan failed.");
                lastStatus = IMSG_STATUS_UNINITIALIZED;
                continue;
            }

            iMSG_LOGI("runLoop: Consumer socket %d.", _consumerFd);
            iMSG_LOGI("runLoop: Consumer InputMessage address %p.", _pConsumerInputMsg);
        }

        usleep(_updateInterval);

        // peek one byte just to test connection before reading InputMessage
        errno = 0;
        n = iMSG_EINTR_RETRY(recv(_consumerFd, &b, 1, MSG_PEEK | MSG_DONTWAIT));
        if (n < 0)
        {
            int err = errno;
            if (err != EAGAIN && err != EWOULDBLOCK)
            {
                iMSG_LOGE("runLoop: Consumer socket(%d) error(%d): \"%s\".", _consumerFd, err, strerror(err));
                lastStatus = IMSG_STATUS_DEAD;
                continue;
            }
        }
        else if (n == 0)
        {
            iMSG_LOGE("runLoop: Consumer socket(%d) EOF.", _consumerFd);
            lastStatus = IMSG_STATUS_EOF;
            continue;
        }

        // don't read same event
        if (memcmp(_pConsumerInputMsg, _iMsgBuf.data(), _iMsgBuf.size()))
        {
            auto type = _pConsumerInputMsg->type();

            if (_callback && type >= 0 && type <= 7)
                _callback(_pConsumerInputMsg);

            memcpy(_iMsgBuf.data(), _pConsumerInputMsg, _iMsgBuf.size());
        }

        lastStatus = IMSG_STATUS_OK;
    }

    iMSG_LOGE("runLoop: Thread completed with status \"%s\".", iMsgStatusString(lastStatus));
    return lastStatus;
}

void iMsgCapture::runThread(const std::vector<std::string>& gameActivites, bool retryOnFailure, int retrySleepSec)
{
    std::thread([](iMsgCapture *self, const std::vector<std::string>& gameActivites, bool retryOnFailure, int retrySleepSec)
    {
        EIMSG_STATUS status = self->runLoop(gameActivites, retryOnFailure, retrySleepSec);

        iMSG_LOGE("runLoop: Thread completed with status \"%s\".", iMsgStatusString(status));

    }, this, gameActivites, retryOnFailure, retrySleepSec).detach();
}

int8_t property_get_bool(const char* key, int8_t default_value)
{
    if (!key) return default_value;

    int8_t result = default_value;
    char buf[0xff] = { 0 };

    int len = __system_property_get(key, buf);
    if (len == 1) {
        char ch = buf[0];
        if (ch == '0' || ch == 'n') {
            result = false;
        } else if (ch == '1' || ch == 'y') {
            result = true;
        }
    } else if (len > 1) {
        if (!strcmp(buf, "no") || !strcmp(buf, "false") || !strcmp(buf, "off")) {
            result = false;
        } else if (!strcmp(buf, "yes") || !strcmp(buf, "true") || !strcmp(buf, "on")) {
            result = true;
        }
    }

    return result;
}

std::string _mallocPathname()
{
    void *n = malloc(sizeof(void*));

    if (auto fMaps = fopen("/proc/self/maps", "r"))
    {
        char cLine[512] = { 0 };
        while (fgets(cLine, sizeof(cLine), fMaps) != nullptr)
        {
            unsigned long long start = 0, end = 0;
            char pathanme[0xff] = { 0 };
            sscanf(cLine, "%llx-%llx %*s %*s %*s %*s %s", &start, &end, pathanme);
            if (uintptr_t(n) >= start && uintptr_t(n) < end) return pathanme;
        }
        fclose(fMaps);
    }

    free(n);

    return "";
}

#if iMSG_USE_VMREAD

static ssize_t syscall_vm_read(pid_t pid,
    const iovec* lvec, unsigned long liovcnt,
    const iovec* rvec, unsigned long riovcnt,
    unsigned long flags)
{
#if defined(__aarch64__)
    return syscall(270, pid, lvec, liovcnt, rvec, riovcnt, flags);
#elif defined(__arm__)
    return syscall(376, pid, lvec, liovcnt, rvec, riovcnt, flags);
#elif defined(__i386__)
    return syscall(347, pid, lvec, liovcnt, rvec, riovcnt, flags);
#elif defined(__x86_64__)
    return syscall(310, pid, lvec, liovcnt, rvec, riovcnt, flags);
#else
#error "Unsupported ABI"
#endif
}

inline size_t vm_read(uintptr_t address, void *buffer, size_t len)
{
    if (!address || !buffer || !len)
        return 0;

    struct iovec lvec { .iov_base = buffer, .iov_len = len };
    struct iovec rvec { .iov_base = reinterpret_cast<void*>(address), .iov_len = len };

    ssize_t n = iMSG_EINTR_RETRY(syscall_vm_read(getpid(), &lvec, 1, &rvec, 1, 0));
    return n < 0 ? 0 : n;  
}

#endif

void _walkMaps(const std::function<bool(const map_info_t &map)> &callback)
{
    if (auto fMaps = fopen("/proc/self/maps", "r"))
    {
        char cLine[512] = { 0 };
        while (fgets(cLine, sizeof(cLine), fMaps) != nullptr)
        {
            unsigned long long start = 0, end = 0, offset;
            char perms[10] = {0}, pathanme[0xff] = {0};
            sscanf(cLine, "%llx-%llx %s %llx %*s %*s %s", &start, &end, perms, &offset, pathanme);
            
            if (callback(map_info_t(start, end, offset, perms, pathanme)))
                break;
        }

        fclose(fMaps);
    }
}

std::vector<ref_info_t> _mallocScan(const void* data, size_t size)
{
    const static std::string mallocPathname = _mallocPathname();

    auto findInRange = [](uintptr_t start, uintptr_t end, const void* data, size_t dataSize) -> uintptr_t
    {
        for (size_t i = 0; (start + i + dataSize) < end; i++)
            if (memcmp(reinterpret_cast<const char*>(start + i), data, dataSize) == 0)
                return start + i;
        return 0;
    };

    std::vector<ref_info_t> ret;

    _walkMaps([&](const map_info_t& map) -> bool
    {
        if (!(((map.end - map.start) > (128 * 1024)) && map.offset == 0 && !map.perms.compare("rw-p") && !map.name.compare(mallocPathname)))
            return false;

#if iMSG_USE_VMREAD
        std::vector<char> buf(map.end - map.start, 0);
        size_t n = 0;
        if ((n = vm_read(map.start, &buf[0], buf.size())) <= 0)
            return false;

        uintptr_t curr_search_address = reinterpret_cast<uintptr_t>(buf.data());
        do {
            uintptr_t found = findInRange(curr_search_address, reinterpret_cast<uintptr_t>(buf.data())+n, data, size);
            if (!found)
                break;

            ret.emplace_back(map, map.start + (found - uintptr_t(buf.data())));

            curr_search_address = (found + size);
        } while (true);
#else
        uintptr_t curr_search_address = map.start;
        do {
            uintptr_t found = findInRange(curr_search_address, map.end, data, size);
            if (!found)
                break;

            ret.emplace_back(map, found);

            curr_search_address = (found + size);
        } while (true);
#endif

        return false;
    });

    return ret;
}

std::vector<int*> _scanChannelFds(const std::set<int>& sockets, const std::string &activity)
{
    std::vector<int*> pFds;

    // IDK the hash hex so we will start from the space after hex
    std::string data = " ";
    data += getprogname();
    data += '/';
    data += activity;
    data += " (client)\0";
    iMSG_LOGI("mallocScan: Searching for channel \"%s\"...", data.c_str());

    auto channelNamePtrs = _mallocScan(data.data(), data.size());
    if (channelNamePtrs.empty())
    {
        iMSG_LOGE("mallocScan: Couldn't find channel name in memory.");
        return pFds;
    }

    iMSG_LOGI("mallocScan: Channel name pointers found %zd.", channelNamePtrs.size());

    for (const auto& channelNamePtr : channelNamePtrs)
    {
        if (!is_page_valid((void*)(channelNamePtr.ptr)))
            continue;

        // find start of string where hash hex begin
        uintptr_t str_start = channelNamePtr.ptr;
        for (char* p = (char*)str_start; *--p && std::isxdigit(*p);)
            str_start--;

        // verify alignment
        for (uintptr_t p = str_start; (p % 4) != 0; p++)
            str_start++;

        // no valid hash hex found
        if (str_start >= channelNamePtr.ptr)
            continue;

        iMSG_LOGI("mallocScan: Scanning channel name at %p [%p - %p \"%s\"]", (void*)str_start, (void*)channelNamePtr.map_info.start, (void*)channelNamePtr.map_info.end, channelNamePtr.map_info.name.c_str());

        auto channelNameRefs = _mallocScan(&str_start, sizeof(str_start));
        if (channelNameRefs.empty())
        {
            iMSG_LOGE("mallocScan: Couldn't find any ref to channel name.");
            return pFds;
        }

        for (const auto& channelNameRef : channelNameRefs)
        {
            if (!is_page_valid((void*)(channelNameRef.ptr)))
                continue;

            int* _pFd = (int*)(channelNameRef.ptr + sizeof(uintptr_t));
            iMSG_LOGI("mallocScan: Possible fd [%p] = %d", _pFd, *_pFd);

            // verify fd
            if (*_pFd <= 0 || sockets.count(*_pFd) <= 0)
                continue;

            iMSG_LOGI("mallocScan: Found valid channel socket fd at %p [%p - %p \"%s\"]", _pFd, (void*)channelNameRef.map_info.start, (void*)channelNameRef.map_info.end, channelNameRef.map_info.name.c_str());
            
            pFds.push_back(_pFd);
        }
    }

    return pFds;
}

uintptr_t _scanInputMessage(int* pFd)
{
    const static int sdk = AMsg_GetAndroidSDK();
    const static int8_t bResampling = property_get_bool("ro.input.resampling", true);

    uintptr_t sp_channel = 0;
    if (sdk <= 27)
        sp_channel = uintptr_t(pFd) - offsetof(InputChannel5_8, fd);
    else if (sdk <= 30)
        sp_channel = uintptr_t(pFd) - offsetof(InputChannel9_11, fd);
    else
        sp_channel = uintptr_t(pFd) - offsetof(InputChannel12_14, fd);

    iMSG_LOGI("mallocScan: Scanning for sp_channel (%p) refs...", (void*)sp_channel);

    auto sp_channel_refs_tmp = _mallocScan(&sp_channel, sizeof(sp_channel));
    if (sp_channel_refs_tmp.empty())
    {
        iMSG_LOGE("mallocScan: Couldn't find any ref to sp_channel (%p).", (void*)sp_channel);
        return 0;
    }

    auto sp_channel_refs = sp_channel_refs_tmp;
    for (const auto& ref1 : sp_channel_refs_tmp)
    {
        for (const auto& ref2 : sp_channel_refs_tmp)
        {
            if (ref1 == ref2)
                continue;

            // there shouldn't be two refs in one page
            auto dist = std::abs(intptr_t(ref1.ptr) - intptr_t(ref2.ptr));
            if (dist < 0x1000)
            {
                sp_channel_refs.erase(std::remove(sp_channel_refs.begin(), sp_channel_refs.end(), ref1), sp_channel_refs.end());
                sp_channel_refs.erase(std::remove(sp_channel_refs.begin(), sp_channel_refs.end(), ref2), sp_channel_refs.end());
            }
        }
    }

    if (sp_channel_refs.empty())
    {
        iMSG_LOGE("mallocScan: Couldn't find any valid refs to sp_channel (%p).", (void*)sp_channel);
        return 0;
    }

    std::vector<iMsgEvent *> possible_imsg_ptrs;
    for (const auto& ref : sp_channel_refs)
    {
        if (!is_page_valid((void*)(ref.ptr)))
            continue;

        iMSG_LOGI("mallocScan: sp_channel ref %p [%p - %p \"%s\"]", (void*)ref.ptr, (void*)ref.map_info.start, (void*)ref.map_info.end, ref.map_info.name.c_str());

        int8_t *mResampleTouch = (int8_t*)(ref.ptr - sizeof(uintptr_t));

#ifdef __LP64__
        if (*mResampleTouch != bResampling && *(mResampleTouch+sizeof(uint32_t)) != bResampling)
            continue;
#else
        if (*mResampleTouch != bResampling)
            continue;
#endif

        iMsgEvent* msgPtrTemp = nullptr;
        if (sdk > 30)
        {
#ifdef __LP64__
            msgPtrTemp = reinterpret_cast<iMsgEvent*>(ref.ptr + sizeof(std::shared_ptr<uintptr_t>));
#else
            msgPtrTemp = reinterpret_cast<iMsgEvent*>(ref.ptr + sizeof(std::shared_ptr<uintptr_t>) + sizeof(uintptr_t));
#endif
        }
        else
        {
            msgPtrTemp = reinterpret_cast<iMsgEvent*>(ref.ptr + sizeof(uintptr_t));
        }

        // InputMessage mMsg shouldn't be a pointer
        if (is_page_valid(*(void**)msgPtrTemp))
            continue;

        void** mMsgDeferred  = (void**)(reinterpret_cast<uintptr_t>(msgPtrTemp) + AInputMsg_Size());
        void** mBatchesBegin = (void**)(reinterpret_cast<uintptr_t>(mMsgDeferred) + sizeof(uintptr_t));
        void** mBatchesEnd   = (void**)(reinterpret_cast<uintptr_t>(mBatchesBegin) + sizeof(uintptr_t));        

        // bool mMsgDeferred shouldn't be a pointer
        if (is_page_valid(*mMsgDeferred))
            continue;

        // verify std::vector<Batch> begin pointer
        if (*mBatchesBegin && !is_page_valid(*mBatchesBegin))
            continue;

        // verify std::vector<Batch> end pointer
        if (*mBatchesEnd && !is_page_valid(*mBatchesEnd))
            continue;

        char id_str16[] = { 0x69, 0, 0x64, 0 };
        if (!memcmp(reinterpret_cast<char*>(msgPtrTemp) + 2, id_str16, sizeof(id_str16)))
        {
            iMSG_LOGI("mallocScan: InputMessage = %p", msgPtrTemp);
            return uintptr_t(msgPtrTemp);
        }

        if (msgPtrTemp->type() >= 0 && msgPtrTemp->type() <= 7)
        {
            iMSG_LOGI("mallocScan: InputMessage = %p", msgPtrTemp);
            return uintptr_t(msgPtrTemp);
        }

        possible_imsg_ptrs.push_back(msgPtrTemp);
    }

    if (!possible_imsg_ptrs.empty())
    {
        iMSG_LOGI("mallocScan: Possible InputMessage = %p", possible_imsg_ptrs.front());

        return uintptr_t(possible_imsg_ptrs.front());
    }

    iMSG_LOGE("mallocScan: Failed to detect InputConsumer.");

    return 0;
}
