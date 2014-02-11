LOCAL_PATH:=		$(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE            := set_fstab
LOCAL_MODULE_TAGS       := optional
LOCAL_SRC_FILES         := set_fstab.c
LOCAL_C_INCLUDES        := \
        $(LOCAL_PATH) \
        ../../../../../system/core/init \
        ../../../../../system/core/include
LOCAL_SHARED_LIBRARIES  := libcutils liblog
LOCAL_MODULE_PATH       := $(LOCAL_PATH)

#LOCAL_MODULE_OWNER := qcom
#LOCAL_MODULE := bdAddrLoader
include $(BUILD_EXECUTABLE)
