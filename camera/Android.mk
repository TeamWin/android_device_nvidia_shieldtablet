LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_DEVICE),shieldtablet)

include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := liblog libcutils libgui libbinder libutils

LOCAL_SRC_FILES := \
    nv_camera.cpp

LOCAL_MODULE := libcam
LOCAL_MODULE_CLASS := SHARED_LIBRARIES

include $(BUILD_SHARED_LIBRARY)

endif
