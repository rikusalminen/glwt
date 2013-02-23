LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := glwt-test
LOCAL_SRC_FILES := ../../glwt_simplest.c
LOCAL_C_INCLUDES := $(abspath $(LOCAL_PATH)/../../../include)
LOCAL_CFLAGS += -g
LOCAL_STATIC_LIBRARIES := android_native_app_glue glwt

include $(BUILD_SHARED_LIBRARY)

$(call import-module,glwt)
$(call import-module,android/native_app_glue)
