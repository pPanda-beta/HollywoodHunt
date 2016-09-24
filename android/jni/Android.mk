LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := tgui-example


LOCAL_CPPFLAGS  :=  -fpermissive -w

LOCAL_SRC_FILES := src/main.cpp
LOCAL_SRC_FILES += src/SceneHandler.cpp
LOCAL_SRC_FILES += src/TGUIScene.cpp
LOCAL_SRC_FILES += src/login_signup_sc.cpp
LOCAL_SRC_FILES += src/Menu.cpp

LOCAL_SRC_FILES += src/db_core.cpp
LOCAL_SRC_FILES += src/fetchurl.cpp
LOCAL_SRC_FILES += src/GameCore.cpp
LOCAL_SRC_FILES += src/InGame_sc.cpp
LOCAL_SRC_FILES += src/sever_core.cpp
LOCAL_SRC_FILES += src/myWord_sc.cpp
LOCAL_SRC_FILES += src/GameEnd_sc.cpp
LOCAL_SRC_FILES += src/High_score_sc.cpp
LOCAL_SRC_FILES += src/ErrorHandler.cpp
LOCAL_SRC_FILES += src/Intro_sc.cpp

LOCAL_SRC_FILES += src/sqlite3.c


LOCAL_SHARED_LIBRARIES := sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_SHARED_LIBRARIES += tgui
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main

include $(BUILD_SHARED_LIBRARY)

$(call import-module,tgui)
