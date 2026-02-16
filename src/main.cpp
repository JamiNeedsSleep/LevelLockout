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
int g_attemptcount = 0;
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/LevelTools.hpp>
#include <Geode/binding/GameManager.hpp>
class $modify(MyPlayerObject, PlayerObject) {
    void playerDestroyed(bool p0) {
		auto gm = GameManager::get();
		auto playLayer = PlayLayer::get();
		if (g_attemptcount<=9) {
			log::debug("test");
			g_attemptcount++;
		} else {
			PlayerObject::playerDestroyed(p0);

        	g_levelLocked = true;

        	if (playLayer) {
        		playLayer->removeFromParentAndCleanup(true);
            	CCDirector::sharedDirector()->replaceScene(MenuLayer::scene(false));
				PlatformToolbox::showCursor();
				PlatformToolbox::toggleLockCursor(false);
				g_attemptcount = 0;
        	}
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
				auto createBtn = menu->getChildByID("editor-button");
				if (createBtn) {
                    createBtn->setVisible(false);
                    menu->updateLayout(true);
                }
            }
        }

        return true;
    }
};