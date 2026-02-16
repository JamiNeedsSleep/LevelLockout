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
int g_attemptcount = 1;
#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/LevelTools.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
$execute {
    listenForSettingChanges<bool>("shouldlockout", [](bool value) {
        g_levelLocked = false;
    });
	listenForSettingChanges<bool>("reset", [](bool value) {
        if (value == true) {
			g_levelLocked = false;
			Mod::get()->setSettingValue("reset", false);
		}
    });
}

class $modify(MyPlayerObject, PlayerObject) {
    void playerDestroyed(bool p0) {
		auto gm = GameManager::get();
		auto playLayer = PlayLayer::get();
		if (g_attemptcount<=4) {
			log::debug("{}", g_attemptcount);
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
				auto alert = FLAlertLayer::create(
    				"Well...",    // title
 					"You failed that level, try better next time.",  // content
    				"Alright"        // button
				);
				alert->m_scene = this;
				alert->show();
        	}
		}
    }
};
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;
		auto lockoutchoice = Mod::get()->getSettingValue<bool>("shouldlockout");
		if (lockoutchoice == true) {
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
		}

        return true;
    }
};