#include <jni.h>
#include "android_native/app_core.h"

extern "C" {

JNIEXPORT void JNICALL
Java_com_conch_app_NativeLib_initializeApp(JNIEnv* env, jobject /* this */) {
    android_native::initialize_app();
}

JNIEXPORT void JNICALL
Java_com_conch_app_NativeLib_startMeeting(JNIEnv* env, jobject /* this */) {
    android_native::start_meeting();
}

}  // extern "C"
