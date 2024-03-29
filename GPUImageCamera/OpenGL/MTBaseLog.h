//
// Created by HW on 2020/4/20.
//

#ifndef MTBASELOG_H
#define MTBASELOG_H

#include "MTBaseDefine.h"

#define LOG_TAG "rayyy"

/**
 * log等级
 */
#define BASE_LOG_LEVEL_ALL      0
#define BASE_LOG_LEVEL_VERBOSE  1
#define BASE_LOG_LEVEL_DEBUG    2
#define BASE_LOG_LEVEL_INFO     3
#define BASE_LOG_LEVEL_WARN     4
#define BASE_LOG_LEVEL_ERROR    5
#define BASE_LOG_LEVEL_FATAL    6
#define BASE_LOG_LEVEL_OFF      7

/**
 * 设置log等级
 */
extern "C" void BASE_SetLogLevel(int level);

/**
 * 获取log等级
 */
extern "C" int BASE_GetLogLevel();

#define  BASE_TARGET_LOG_LEVEL   (BASE_GetLogLevel())

#if defined(PLATFORM_ANDROID)

#include <android/log.h>

#define  LOGV(...)  do { if (BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_VERBOSE) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__); } while(0)
#define  LOGD(...)  do { if (BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_DEBUG) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__); } while(0)
#define  LOGI(...)  do { if (BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_INFO) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__); } while(0)
#define  LOGW(...)  do { if (BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_WARN) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__); } while(0)
#define  LOGE(...)  do { if (BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_ERROR) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__); } while(0)
#define  LOGF(...)  do { if (BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_FATAL) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__); } while(0)

#else // !PLATFORM_ANDROID

#define  LOGV(...)  do { if(BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_VERBOSE) {printf("[" LOG_TAG "] VERBOSE:> ");printf(__VA_ARGS__);printf("\n");} } while(0)
#define  LOGD(...)  do { if(BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_DEBUG) {printf("<[" LOG_TAG "] DEBUG:> ");printf(__VA_ARGS__);printf("\n");} } while(0)
#define  LOGI(...)  do { if(BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_INFO) {printf("<[" LOG_TAG "] INFO:> ");printf(__VA_ARGS__);printf("\n");} } while(0)
#define  LOGW(...)  do { if(BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_WARN) {printf("<[" LOG_TAG "] WARN:> ");printf(__VA_ARGS__);printf("\n");} } while(0)
#define  LOGE(...)  do { if(BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_ERROR) {printf("<[" LOG_TAG "] ERROR:> ");printf(__VA_ARGS__);printf("\n");} } while(0)
#define  LOGF(...)  do { if(BASE_TARGET_LOG_LEVEL <= BASE_LOG_LEVEL_FATAL) {printf("<[" LOG_TAG "] FATAL:> ");printf(__VA_ARGS__);printf("\n");} } while(0)

#endif // !PLATFORM_ANDROID


#endif //MTBASELOG_H
