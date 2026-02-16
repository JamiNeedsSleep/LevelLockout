/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/binding/PlatformToolbox.hpp>
/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;
static bool g_levelLocked = false;
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
class $modify(MyPlayerObject, PlayerObject) {
    void playerDestroyed(bool p0) {
        PlayerObject::playerDestroyed(p0);

        g_levelLocked = true;

        auto playLayer = PlayLayer::get();
        if (playLayer) {
            playLayer->removeFromParentAndCleanup(true);
            CCDirector::sharedDirector()->replaceScene(MenuLayer::scene(false));
			PlatformToolbox::showCursor();
			PlatformToolbox::toggleLockCursor(false);
        }
    }
};
#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {

    bool init() {
        if (!MenuLayer::init())
            return false;

        if (g_levelLocked) {
            auto menu = this->getChildByID("main-menu");
			auto createmenu = this->getChildByID("main-menu");
            if (menu) {
                auto playBtn = menu->getChildByID("play-button");
				auto createBtn = menu->getChildByID("editor-button");
                if (playBtn) {
                    playBtn->setVisible(false);
                    menu->updateLayout(true);
                }
				if (createBtn) {
                    createBtn->setVisible(false);
                    menu->updateLayout(true);
                }
            }
        }

        return true;
    }
};