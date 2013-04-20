LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := glwt-test
LOCAL_SRC_FILES := ../../glwt_test_app.c
LOCAL_C_INCLUDES := $(abspath $(LOCAL_PATH)/../../../include)
LOCAL_CFLAGS += -g
LOCAL_CFLAGS += -DGLWT_TEST_GLES
LOCAL_STATIC_LIBRARIES := glwt
LOCAL_STATIC_LIBRARIES += glxw_es2
LOCAL_LDLIBS=-llog
LOCAL_LDLIBS+=-lGLESv2

include $(BUILD_SHARED_LIBRARY)

$(call import-module,glwt)
