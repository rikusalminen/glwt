LOCAL_PATH := $(call my-dir)

PYTHON ?= python

include $(CLEAR_VARS)

LOCAL_MODULE := glxw_egl
LOCAL_SRC_FILES := ext/glxw/src/glxw_egl.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/ext/glxw/include
LOCAL_CFLAGS += -g
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ext/glxw/include
LOCAL_EXPORT_LDLIBS := -lEGL

$(LOCAL_PATH)/ext/glxw/src/glxw_egl.c: $(LOCAL_PATH)/ext/glxw/glxw_gen.py
	$(PYTHON) $< --with-platform= --api egl -o $(abspath $(dir $@)/..)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := glwt
LOCAL_SRC_FILES := \
	src/common/glwt.c \
	src/android/glwt_android.c \
	src/android/window.c \
	src/android/events.c \
	src/egl/glwt_egl.c \
	src/egl/window.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/src $(LOCAL_PATH)/ext/glxw/include
LOCAL_CFLAGS := -W -Wall -Wextra
LOCAL_CFLAGS += -g
LOCAL_LDLIBS := -landroid
LOCAL_EXPORT_LDLIBS := -landroid
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_STATIC_LIBRARIES := android_native_app_glue glxw_egl

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)
