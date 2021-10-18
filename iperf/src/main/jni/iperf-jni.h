#ifndef IPERF_H
#define IPERF_H

#include <jni.h>

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <future>
#include <thread>
#include <chrono>

extern "C" {
#include <iperf.h>
#include <iperf_api.h>
}

#define TAG "IPERF-JNI"

#define  logd(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define  logi(...)  __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define  logw(...)  __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define  loge(...)  __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

/**
 * @def UNUSED_ARG(arg)
 * @param arg   The argument name.
 * UNUSED_ARG prevents warning about unused argument in a function.
 */
#define UNUSED_ARG(arg)  (void)arg

class iPerfNativeCallbacks {
public:
    iPerfNativeCallbacks() {}

    virtual ~iPerfNativeCallbacks();

    virtual void onAppendResult(std::string result) { UNUSED_ARG(result); }

    virtual void onClearResult() {}
};

class iPerfNative : public iPerfNativeCallbacks {

public:
    iPerfNative();

    ~iPerfNative();
private:
    static iPerfNative *_instance;

public:
    static iPerfNative &instance();

    void deInit();

    void init(char *hostname, int port, char *streamTemplate, int duration,
              int interval, bool download, bool useUDP, bool json);

    int execute();

    const char *getHostname();

    void setHostname(char *hostname);

    int getPort() const;

    void setPort(int port);

    const char *getStreamTemplate();

    void setStreamTemplate(char *streamTemplate);

    int getDuration() const;

    void setDuration(int duration);

    int getInterval() const;

    void setInterval(int interval);

    bool isDebug() const;

    void setDebug(bool debug);

    bool isDownload() const;

    void setDownload(bool download);

    bool isJson() const;

    void setJson(bool json);

    bool isUseUDP() const;

    void setUseUDP(bool udp);

private:
    struct iperf_test *test = nullptr;
    std::string hostname = "";
    int port = 5201;
    std::string streamTemplate = "";
    int duration = 10;
    int interval = 1;
    bool download = false;
    bool json = false;
    bool useUDP = false;

    bool debug = false;
};


#endif //IPERF_H
