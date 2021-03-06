#include <jni.h>
#include <string.h>
#include <stdbool.h>

#define LOG_TAG "WIREGUARD_JNI"
#include "log.h"

static struct {
    jclass sessionStatClass;
    jmethodID sessionStatCtor;
    int tun_fd;
} module;

static bool create_session_stat(JNIEnv* env)
{
    jclass tmp;

    tmp = (*env)->FindClass(env, "io/wireguard/SessionStat");
    if (!tmp) {
        LOGE("create_session_stat: Could not find SessionStat class");
        (*env)->ExceptionClear(env);
        return false;
    }

    module.sessionStatClass = (*env)->NewGlobalRef(env, tmp);
    module.sessionStatCtor = (*env)->GetMethodID(env, module.sessionStatClass, "<init>", "(JJJJJJ)V");
    if (!module.sessionStatCtor) {
        LOGE("create_session_stat: Could not find SessionStat constructor");
        (*env)->ExceptionClear(env);
        return false;
    }

    return true;
}

jint JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    LOGV(__func__);

    JNIEnv *env = NULL;

    if ((*jvm)->GetEnv(jvm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGV("jni error");
        return JNI_ERR;
    }

    module.tun_fd = -1;

    if (!create_session_stat(env)) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

JNIEXPORT jboolean JNICALL
Java_io_wireguard_Native_enable(JNIEnv *env, jclass type)
{
    LOGV(__func__);
    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_io_wireguard_Native_disable(JNIEnv *env, jclass type)
{
    LOGV(__func__);
}

JNIEXPORT void JNICALL
Java_io_wireguard_Native_setTunFd(JNIEnv *env, jclass type, jint fd)
{
    LOGV(__func__);
}

/* TODO: get real session stat */
JNIEXPORT jobject JNICALL
Java_io_wireguard_Native_sessionStat(JNIEnv *env, jclass type)
{
    LOGV(__func__);

    jobject ret;

    static jlong pt = 0, pr = 0, pd = 0;
    static jlong bt = 0, br = 0, bd = 0;

    ret = (*env)->NewObject(env, module.sessionStatClass, module.sessionStatCtor,
            pt, pr, pd,
            bt, br, bd);

    pt += 2;
    pr += 3;
    pd += 1;

    bt += 15;
    br += 25;
    bd += 5;

    return ret;
}

