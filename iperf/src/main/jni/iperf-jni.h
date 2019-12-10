#ifndef IPERF_H
#define IPERF_H

#include <jni.h>

#include <vector>
#include <string>
#include <cstring>
#include <cstdio>

extern "C" {
#include <iperf.h>
#include <iperf_api.h>
}

/**
 * @def UNUSED_ARG(arg)
 * @param arg   The argument name.
 * UNUSED_ARG prevents warning about unused argument in a function.
 */
#define UNUSED_ARG(arg)  (void)arg

class iPerfResultCallbacks {
public:
    iPerfResultCallbacks() {}

    virtual ~iPerfResultCallbacks();

    virtual void onIntermediateResult(double bits, bool download) {
        UNUSED_ARG(bits);
    }

    virtual void onSummaryResult(double totalBits, int count, bool download) {
        UNUSED_ARG(totalBits);
        UNUSED_ARG(count);
    }
};

class iPerf : public iPerfResultCallbacks {
public:

    iPerf(char *hostname, int port, char *streamTemplate, int duration,
          int interval, bool download);

    ~iPerf();

    void deInit();

    void init();

    void run();

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

    void updatedIntermediateResult(double bits);

    void updateSummaryResult();

private:
    struct iperf_test *test = nullptr;
    std::string hostname;
    int port;
    std::string streamTemplate;
    int duration;
    int interval;
    bool download;

    bool debug = false;
public:
    std::vector<double> results;
};


#endif //IPERF_H
