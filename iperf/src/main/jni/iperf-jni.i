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

// Try force Java GC before destroying the lib:
// - to avoid late destroy of SB objects by GC
// - to avoid destruction of SB objects from a non-registered GC thread
#ifdef SWIGJAVA
%rename(libDestroy_) sb::Endpoint::libDestroy;
%typemap(javacode) sb::Endpoint %{
  public void libDestroy(long prmFlags) throws java.lang.Exception {
	Runtime.getRuntime().gc();
	libDestroy_(prmFlags);
  }

  public void libDestroy() throws java.lang.Exception {
	Runtime.getRuntime().gc();
	libDestroy_();
  }
%}
#endif

%feature("director");

/* Let's just grab the original header file here */
%include "iperf-jni.h"
