#include <android/log.h>
#include <utility>
#include "iperf-jni.h"

#define IPERF_TAG "IPERF-JNI"

#define  logd(...)  __android_log_print(ANDROID_LOG_DEBUG, IPERF_TAG, __VA_ARGS__)
#define  logi(...)  __android_log_print(ANDROID_LOG_INFO, IPERF_TAG, __VA_ARGS__)
#define  logw(...)  __android_log_print(ANDROID_LOG_WARN, IPERF_TAG, __VA_ARGS__)

static const char role = 'c';

static int internal_log(const char *format, ...) {
    int r;
    va_list args;
    va_start(args, format);
    r = __android_log_vprint(ANDROID_LOG_DEBUG, IPERF_TAG, format, args);
    va_end(args);
    return r;
}

static void reporter_callback(struct iperf_test *test) {
    auto *iperf_tester = (iPerf *) test->test_owner;
    if(iperf_tester->isDebug()) {
        logi("reporter state: %d", test->state);
    }
    switch (test->state) {
        case TEST_RUNNING:
        case STREAM_RUNNING:{
                /* print interval results for each stream */
                struct iperf_result result = iperf_intermediate_result(test);
            iperf_tester->updatedIntermediateResult(result.bandwidth_in_bit);
        }
            break;
        case TEST_END:
        case DISPLAY_RESULTS:{
            struct iperf_result result = iperf_intermediate_result(test);
            iperf_tester->updatedIntermediateResult(result.bandwidth_in_bit);
            iperf_tester->updateSummaryResult();
        }
            break;
        default:break;
    }
}

iPerf::iPerf(char *hostname, int port, char *streamTemplate, int duration,
             int interval, bool download) : hostname(hostname), port(port),
                                          streamTemplate(streamTemplate), duration(duration),
                                          interval(interval), download(download) {
    this->deInit();
}

iPerf::~iPerf() {
    this->deInit();
}

void iPerf::deInit() {
    if (this->test != nullptr) {
        iperf_free_test(this->test);
    }
    this->results.clear();
}

void iPerf::init() {
    this->test = iperf_new_test();
    this->test->test_owner = (void *) this;
    this->results.clear();

    // load defaults
    iperf_defaults(this->test);

    iperf_set_test_json_output(test, 0);
    iperf_set_verbose(test, 1);

    // set role as client
    iperf_set_test_role(this->test, role);

    // set download-> 1/ upload-> 0
    iperf_set_test_reverse(this->test, download ? 1 : 0);

    iperf_set_test_server_hostname(this->test, (char *) this->hostname.c_str());
    iperf_set_test_server_port(this->test, this->port);

    iperf_set_test_duration(this->test, this->duration);
    iperf_set_test_reporter_interval(this->test, this->interval);
    iperf_set_test_stats_interval(this->test, 1);

    iperf_set_test_template(this->test, (char *) this->streamTemplate.c_str());
    iperf_set_test_num_streams(this->test, 1);

    if (this->debug) {
        this->test->icb.log = internal_log;
    }

    iperf_set_verbose(test, this->debug? 1:0);

    this->test->reporter_callback = reporter_callback;
}

void iPerf::run() {
    iperf_run_client(this->test);
}

const char *iPerf::getHostname() {
    return hostname.c_str();
}

void iPerf::setHostname(char *hostname) {
    iPerf::hostname = std::string(hostname);
}

int iPerf::getPort() const {
    return port;
}

void iPerf::setPort(int port) {
    iPerf::port = port;
}

const char *iPerf::getStreamTemplate() {
    return streamTemplate.c_str();
}

void iPerf::setStreamTemplate(char *streamTemplate) {
    iPerf::streamTemplate = std::string(streamTemplate);
}

int iPerf::getDuration() const {
    return download;
}

void iPerf::setDuration(int duration) {
    iPerf::download = duration;
}

int iPerf::getInterval() const {
    return interval;
}

void iPerf::setInterval(int interval) {
    iPerf::interval = interval;
}

bool iPerf::isDebug() const {
    return debug;
}

void iPerf::setDebug(bool debug) {
    iPerf::debug = debug;
}

bool iPerf::isDownload() const {
    return download;
}

void iPerf::setDownload(bool download) {
    iPerf::download = download;
}

void iPerf::updatedIntermediateResult(double bits) {
    this->results.push_back(bits);
    this->onIntermediateResult(bits, this->download);
}

void iPerf::updateSummaryResult() {
    double total = 0;
    for(auto& item: this->results){
        total+= item;
    }
    this->onSummaryResult(total, this->results.size(), this->download);
}

iPerfResultCallbacks::~iPerfResultCallbacks() = default;
