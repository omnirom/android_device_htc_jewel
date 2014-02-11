LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_SETS_FSTAB),true)

include $(CLEAR_VARS)
LOCAL_MODULE            := setfstab
LOCAL_MODULE_TAGS       := optional eng

LOCAL_C_INCLUDES        := system/core \
                           system/core/include \
                           bionic/libc/include

LOCAL_STATIC_LIBRARIES  := libcutils \
                           libc \
                           liblog

LOCAL_SRC_FILES         := setfstab.c \
                           ../../../../../system/core/init/util.c

LOCAL_MODULE_CLASS      := EXECUTABLES
LOCAL_MODULE_PATH       := $(TARGET_ROOT_OUT)
include $(BUILD_EXECUTABLE)

endif
