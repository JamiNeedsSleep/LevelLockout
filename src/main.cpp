/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/binding/PlatformToolbox.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/binding/ColorSelectPopup.hpp>
/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
#include <Geode/ui/LazySprite.hpp>
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
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LevelAreaInnerLayer.hpp>
#include <Geode/cocos/cocoa/CCObject.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
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
				g_attemptcount = 1;
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
                	if (playBtn) {
                    	playBtn->setVisible(false);
                    	menu->updateLayout(true);
                	}
            	}
        	}
		}

        return true;
    }
};
class $modify(MyCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init())
            return false;
		auto lockoutchoice = Mod::get()->getSettingValue<bool>("shouldlockout");
		if (lockoutchoice == true) {
			if (g_levelLocked) {
            	auto buttonsmenu = this->getChildByID("creator-buttons-menu");
				auto searchBtn = buttonsmenu->getChildByID("search-button");
				auto searchbutn = typeinfo_cast<CCMenuItemSpriteExtra*>(searchBtn);
				auto featured = buttonsmenu->getChildByID("search-button");
				auto featuredBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(featured);
				auto lists = buttonsmenu->getChildByID("lists-button");
				auto listsBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(lists);
				auto mappacks = buttonsmenu->getChildByID("map-packs-button");
				auto mappacksBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(mappacks);
				auto saved = buttonsmenu->getChildByID("saved-button");
				auto savedBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(saved);
				auto map = buttonsmenu->getChildByID("map-button");
				auto mapBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(map);
				auto color = ColorSelectPopup::hexToColor("808080");
				searchbutn->m_bEnabled = false;
				searchbutn->m_colorEnabled = false;
				searchbutn->setColor(color);
				featuredBtn->m_bEnabled = false;
				featuredBtn->m_colorEnabled = false;
				featuredBtn->setColor(color);
				listsBtn->m_bEnabled = false;
				listsBtn->m_colorEnabled = false;
				listsBtn->setColor(color);
				mappacksBtn->m_bEnabled = false;
				mappacksBtn->m_colorEnabled = false;
				mappacksBtn->setColor(color);
				savedBtn->m_bEnabled = false;
				savedBtn->m_colorEnabled = false;
				savedBtn->setColor(color);
				auto newSprite = CCSprite::create("Btn_001.png"_spr);
				mapBtn->setSprite(newSprite);
        	}
		}

        return true;
    }
	void onAdventureMap(CCObject* target) {
		if (g_levelLocked) {
			Mod::get()->setSettingValue("reset", true);
			auto alert = FLAlertLayer::create(
            	"Reset Complete",
            	"Reset the lockout successfully!",
            	"OK"
        	);
			alert->m_scene = this;
			alert->show();
		}
	}
};
class $modify (MyLevelAreaInner, LevelAreaInnerLayer) {
	bool init() {
		if (!LevelAreaInnerLayer::init(true))
            return false;
		return true;
	}
	void onNextFloor(CCObject* target) {
		auto alert = FLAlertLayer::create(
            "Not Yet! (Self Promo)",
            "This wont be available until a possible upcoming mod im making... Or 2.21 comes out!",
            "OK"
        );
		alert->m_scene = this;
		alert->show();
	}
};