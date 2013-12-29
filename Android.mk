
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := loki_flash.c
LOCAL_MODULE := loki_flash

include $(BUILD_HOST_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := loki_patch.c
LOCAL_MODULE := loki_patch
include $(BUILD_HOST_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := loki_find.c
LOCAL_MODULE := loki_find
include $(BUILD_HOST_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := loki_flash.c
LOCAL_STATIC_LIBRARIES := libc
LOCAL_MODULE := utility_loki_flash
LOCAL_MODULE_TAGS := eng
LOCAL_MODULE_STEM := loki_flash
LOCAL_MODULE_CLASS := UTILITY_EXECUTABLES
LOCAL_UNSTRIPPED_PATH := $(PRODUCT_OUT)/symbols/utilities
LOCAL_MODULE_PATH := $(PRODUCT_OUT)/utilities
LOCAL_FORCE_STATIC_EXECUTABLE := true
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := loki_patch.c
LOCAL_STATIC_LIBRARIES := libc
LOCAL_MODULE := utility_loki_patch
LOCAL_MODULE_TAGS := eng
LOCAL_MODULE_STEM := loki_patch
LOCAL_MODULE_CLASS := UTILITY_EXECUTABLES
LOCAL_UNSTRIPPED_PATH := $(PRODUCT_OUT)/symbols/utilities
LOCAL_MODULE_PATH := $(PRODUCT_OUT)/utilities
LOCAL_FORCE_STATIC_EXECUTABLE := true
include $(BUILD_EXECUTABLE)

$(call dist-for-goals,dist_files,$(LOCAL_BUILT_MODULE))

