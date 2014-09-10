NDKROOT := /Applications/adt-bundle-mac-x86_64-20131030/android-ndk-r9c
LOCAL_PATH := $(call my-dir)
PROJECT_ROOT_PATH := $(LOCAL_PATH)/../../../
CORE_RELATIVE_PATH := ../../../core/
CORE_OPENGLES_RELATIVE_PATH := ../../../core/opengles/
COMMON_RELATIVE_PATH := ../../../platform/common/

include $(CLEAR_VARS)

LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra -DTECHNE_GAMES_OPENGL_ES -DTECHNE_GAMES_OPENGL_ANDROID

LOCAL_SRC_FILES := platform_asset_utils.c
LOCAL_SRC_FILES += platform_log.c
LOCAL_SRC_FILES += renderer_wrapper.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/ActiveButton.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/Assets.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/BombButton.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/BombGameObject.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/BotPlayerDynamicGameObject.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/BreakableBlock.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/DPadControl.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/DynamicGameObject.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/DynamicGridGameObject.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/Explosion.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/Fire.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/Font.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/GameButton.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/GameListener.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/GameObject.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/GridGameObject.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/GameScreen.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/GameSession.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/InsideBlock.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/InterfaceOverlay.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/Line.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/MapBorder.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/MapSearchNode.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/OverlapTester.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/PathFinder.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/PlayerAvatar.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/PlayerDynamicGameObject.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/PlayerRow.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/PowerUp.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/PowerUpBarItem.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/Rectangle.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/Renderer.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/SpectatorControls.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/TextureRegion.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/TouchEvent.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/Triangle.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/Vector2D.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/WaitingForLocalSettingsInterface.cpp
LOCAL_SRC_FILES += $(CORE_RELATIVE_PATH)/WaitingForServerInterface.cpp
LOCAL_SRC_FILES += $(CORE_OPENGLES_RELATIVE_PATH)/asset_utils.c
LOCAL_SRC_FILES += $(CORE_OPENGLES_RELATIVE_PATH)/game.cpp
LOCAL_SRC_FILES += $(CORE_OPENGLES_RELATIVE_PATH)/image.c
LOCAL_SRC_FILES += $(CORE_OPENGLES_RELATIVE_PATH)/OpenGLESGameScreen.cpp
LOCAL_SRC_FILES += $(CORE_OPENGLES_RELATIVE_PATH)/OpenGLESRectangleRenderer.cpp
LOCAL_SRC_FILES += $(CORE_OPENGLES_RELATIVE_PATH)/OpenGLESRenderer.cpp
LOCAL_SRC_FILES += $(CORE_OPENGLES_RELATIVE_PATH)/SpriteBatcher.cpp
LOCAL_SRC_FILES += $(CORE_OPENGLES_RELATIVE_PATH)/texture.c
LOCAL_SRC_FILES += $(CORE_OPENGLES_RELATIVE_PATH)/Vertices2D.cpp
LOCAL_SRC_FILES += $(COMMON_RELATIVE_PATH)/platform_file_utils.c

LOCAL_C_INCLUDES := $(PROJECT_ROOT_PATH)/platform/common/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/opengles/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/3rdparty/
LOCAL_STATIC_LIBRARIES := libpng
LOCAL_LDLIBS := -lGLESv1_CM -llog -landroid

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,$(PROJECT_ROOT_PATH)/3rdparty)
$(call import-module,libpng)