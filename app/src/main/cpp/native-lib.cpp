#include <jni.h>
#include <string>
#include "Calculator.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_calculator_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject,
        jstring display_str) {
    const char *expression = env->GetStringUTFChars(display_str, 0);
    string str = expression;
    delete expression;
    if (str.size() == 0) {
        return env->NewStringUTF("0");
    }
    Calculator *calculator = new Calculator(str);
    return env->NewStringUTF(calculator->solve().c_str());
}