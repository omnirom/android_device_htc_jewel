#ifndef _SETFSTAB_H
#define _SETFSTAB_H

#include <android/log.h>
#include <sys/types.h>

#define logi(...) __android_log_print(ANDROID_LOG_INFO , "setfstab", __VA_ARGS__)
#define loge(...) __android_log_print(ANDROID_LOG_ERROR , "setfstab", __VA_ARGS__)

#endif
