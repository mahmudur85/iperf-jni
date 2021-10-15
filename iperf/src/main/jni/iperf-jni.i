%module(directors = "1") libiperf

//
// Suppress few warnings
//
#pragma SWIG nowarn=312        // 312: nested struct (in types.h, sip_auth.h)

%{
#include "iperf-jni.h"

using namespace std;
%}

%{
#include <stdexcept>
#include "jni.h"

#ifdef SWIGJAVA
extern JavaVM *jni_jvm;

static JNIEnv *JNU_GetEnv() {
    JNIEnv *env;
    jint rc = jni_jvm->GetEnv((void **) &env, JNI_VERSION_1_4);
    if (rc == JNI_EDETACHED)
        throw std::runtime_error("current thread not attached");
    if (rc == JNI_EVERSION)
        throw std::runtime_error("jni version not supported");
    return env;
}
#endif

%}

// Constants from libraries
//%include "symbols.i"

//
// STL stuff.
//
%include "std_string.i"
%include "std_vector.i"

%template(StringVector) std::vector<std::string>;
%template(IntVector) std::vector<int>;

namespace std {
    %template(VecDouble) vector<double>;
}
#ifdef SWIGJAVA
%include "enumtypeunsafe.swg"
%javaconst(1);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("iperf-jni");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Failed to load native library 'traceroutelib'\n" + e);
        }
    }
%}
#endif

// Try force Java GC before destroying the lib:
// - to avoid late destroy of SB objects by GC
// - to avoid destruction of SB objects from a non-registered GC thread

%feature("director");

/* Let's just grab the original header file here */
%include "iperf-jni.h"
