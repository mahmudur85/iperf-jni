%module(directors = "1") iPerfLib

//
// Suppress few warnings
//
#pragma SWIG nowarn=312        // 312: nested struct

%{
#include "iperf-jni.h"

using namespace std;
%}

%{
#include <stdexcept>
#include "jni.h"
#include "android/log.h"

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
%include "std_array.i"
%include "arrays_java.i"
%include "std_vector.i"
%include "std_list.i"

%template(StringVector) std::vector<std::string>;
%template(StringList) std::list<std::string>;
%template(IntVector) std::vector<int>;
%template(IntList) std::list<int>;

namespace std {
        %template(VecDouble) vector<double>;
        %template(ListDouble) list<double>;
}

// Try force Java GC before destroying the lib:
// - to avoid late destroy of SB objects by GC
// - to avoid destruction of SB objects from a non-registered GC thread

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

#ifdef SWIGJAVA
// byte array
%typemap(jtype) (signed char *data, int length) "byte[]"
%typemap(jstype) (signed char *data, int length) "byte[]"
%typemap(jni) (signed char *data, int length) "jbyteArray"
%typemap(javadirectorin) (signed char *data, int length) "$jniinput"
%typemap(javadirectorout) (signed char *data, int length) "$javacall"
%typemap(javain) (signed char *data, int length) "$javainput"
%typemap(in,numinputs=1) (signed char *data, int length) {
  // Note the NULL here if you don't want to be making changes visible
  $1 = JCALL2(GetByteArrayElements, jenv, $input, NULL);
  $2 = JCALL1(GetArrayLength, jenv, $input);
}
%typemap(freearg) (signed char *data, int length) {
  // Or use  0 instead of ABORT to keep changes if it was a copy
  if ($input) JCALL3(ReleaseByteArrayElements, jenv, $input, $1, JNI_ABORT);
}
%typemap(directorin, descriptor="[B", noblock=1) (signed char *data, int length) {
  $input = 0;
  if ($1) {
    $input = JCALL1(NewByteArray, jenv, (jsize)$2);
    if (!$input) return $null;
    JCALL4(SetByteArrayRegion, jenv, $input, 0, (jsize)$2, (jbyte *)$1);
  }
  Swig::LocalRefGuard $1_refguard(jenv, $input);
}
%typemap(directorargout, noblock=1) (signed char *data, int length){
  if ($input && $1){
    JCALL4(GetByteArrayRegion, jenv, $input, 0, (jsize)$2, (jbyte *)$1);
  }
}
%typemap(directorout) (signed char *data, int length) {
  $1 = 0;
  if($input){
		$result = JCALL2(GetByteArrayElements, jenv, $input, NULL);
		if(!$1)
			return $null;
    JCALL3(ReleaseByteArrayElements, jenv, $input, $result, 0);
	}
}
#endif

%feature("director");

/* Let's just grab the original header file here */
%include "iperf-jni.h"
