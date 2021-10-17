#include <android/log.h>
#include <utility>
#include "iperf-jni.h"

#define OUTPUT_LENGTH  10240

iPerfNative *iPerfNative::_instance = nullptr;

static const char role = 'c';

int printf(const char *fmt, ...) {
    va_list argptr;
    int cnt;
    va_start(argptr, fmt);
    char *buffer = (char *) malloc(OUTPUT_LENGTH);
    memset(buffer, OUTPUT_LENGTH, 0);
    cnt = vsnprintf(buffer, OUTPUT_LENGTH, fmt, argptr);
    buffer[cnt] = '\0';
    if (iPerfNative::instance().isDebug()) {
        logi("ifref message(printf): %s", buffer);
    }
    iPerfNative::instance().onAppendResult(buffer);
    free(buffer);
    va_end(argptr);
    return cnt;
}

int fprintf(FILE *fp, const char *fmt, ...) {
    va_list argptr;
    int cnt;
    va_start(argptr, fmt);
    char *buffer = (char *) malloc(OUTPUT_LENGTH);
    memset(buffer, OUTPUT_LENGTH, 0);
    cnt = vsnprintf(buffer, OUTPUT_LENGTH, fmt, argptr);
    buffer[cnt] = '\0';
    if (iPerfNative::instance().isDebug()) {
        logi("ifref message(fprintf): %s", buffer);
    }
    iPerfNative::instance().onAppendResult(buffer);
    free(buffer);
    va_end(argptr);
    return cnt;
}

int vfprintf(FILE *fp, const char *fmt, va_list args) {
    int cnt;
    char *buffer = (char *) malloc(OUTPUT_LENGTH);
    memset(buffer, OUTPUT_LENGTH, 0);
    cnt = vsnprintf(buffer, OUTPUT_LENGTH, fmt, args);
    buffer[cnt] = '\0';
    if (iPerfNative::instance().isDebug()) {
        logi("ifref message(vfprintf): %s", buffer);
    }
    iPerfNative::instance().onAppendResult(buffer);
    free(buffer);
    return cnt;
}

void perror(const char *msg) {
    loge("ifref error message(perror): %s", msg);
}

iPerfNative::iPerfNative() {
    if (_instance != nullptr) {
        throw "Instance already exists";
    }
    _instance = this;
    this->deInit();
}


iPerfNative::~iPerfNative() {
    this->deInit();
    _instance = nullptr;
}

iPerfNative &iPerfNative::instance() {
    if (_instance == nullptr) {
        throw "Instance does not exists";
    }
    return *_instance;
}

void iPerfNative::deInit() {
    if (this->test != nullptr) {
        iperf_free_test(this->test);
    }
}

void iPerfNative::init(char *hostname, int port, char *streamTemplate, int duration,
                       int interval, bool download, bool useUDP, bool json) {
    this->hostname = hostname;
    this->port = port;
    this->streamTemplate = streamTemplate;
    this->duration = duration;
    this->interval = interval;
    this->download = download;
    this->useUDP = useUDP;
    this->json = json;

    this->test = iperf_new_test();

    // load defaults
    iperf_defaults(this->test);


    iperf_set_test_json_output(test, this->json ? 1 : 0);
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

    if(!useUDP) {
        set_protocol(this->test, Ptcp);
    }else{
        set_protocol(this->test, Pudp);
    }

    // need this to get the output in local printf
    iperf_set_verbose(test, 1);
    test->get_server_output = 1;
}

static int run(struct iperf_test *test) {
    int ret = iperf_run_client(test);
    logd("iperf_run_client() result: %d", ret);
    return ret;
}

int iPerfNative::execute() {
    onClearResult();
    if(json){
        onAppendResult("Accumulating results in JSON format...\n");
    }
    auto run_future = std::async(std::launch::async, run, this->test);
    auto result = run_future.get();
    logd("finish iperf request, status: %d", result);
    return result;
}

const char *iPerfNative::getHostname() {
    return hostname.c_str();
}

void iPerfNative::setHostname(char *hostname) {
    iPerfNative::hostname = std::string(hostname);
}

int iPerfNative::getPort() const {
    return port;
}

void iPerfNative::setPort(int port) {
    iPerfNative::port = port;
}

const char *iPerfNative::getStreamTemplate() {
    return streamTemplate.c_str();
}

void iPerfNative::setStreamTemplate(char *streamTemplate) {
    iPerfNative::streamTemplate = std::string(streamTemplate);
}

int iPerfNative::getDuration() const {
    return download;
}

void iPerfNative::setDuration(int duration) {
    iPerfNative::download = duration;
}

int iPerfNative::getInterval() const {
    return interval;
}

void iPerfNative::setInterval(int interval) {
    iPerfNative::interval = interval;
}

bool iPerfNative::isDebug() const {
    return debug;
}

void iPerfNative::setDebug(bool debug) {
    iPerfNative::debug = debug;
}

bool iPerfNative::isDownload() const {
    return download;
}

void iPerfNative::setDownload(bool download) {
    iPerfNative::download = download;
}

bool iPerfNative::isJson() const {
    return json;
}

void iPerfNative::setJson(bool json) {
    iPerfNative::json = json;
}

bool iPerfNative::isUseUDP() const {
    return useUDP;
}

void iPerfNative::setUseUDP(bool udp) {
    iPerfNative::useUDP = udp;
}

iPerfNativeCallbacks::~iPerfNativeCallbacks() = default;
