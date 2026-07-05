#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include <Geode/Enums.hpp>
#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/LevelInfoLayer.hpp>
#include <Geode/binding/LikeItemLayer.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

class $modify(EndVoting, EndLevelLayer) {
public:
    void customSetup() {
        EndLevelLayer::customSetup();

        auto playLayer = PlayLayer::get();
        if (!playLayer) return;

        auto level = playLayer->m_level;

        bool shouldLike = level->m_levelType == GJLevelType::Saved && level->m_levelID > 0;

        if (shouldLike) {
            if (
                !GameLevelManager::get()->hasLikedItem(LikeItemType::Level, level->m_levelID, true, 0) 
                && !GameLevelManager::get()->hasLikedItem(LikeItemType::Level, level->m_levelID, false, 0)
            ) {
                auto hideLayerMenu = this->getChildByID("hide-layer-menu");
                if (!hideLayerMenu) return;

                auto spr = CCSprite::createWithSpriteFrameName("GJ_like2Btn_001.png");
                spr->setScale(0.75f);

                auto voteBtn = CCMenuItemSpriteExtra::create(
                    spr,
                    this,
                    menu_selector(EndVoting::onVoteBtn)
                );
                voteBtn->setID("vote-btn"_spr);

                hideLayerMenu->addChild(voteBtn);
                hideLayerMenu->updateLayout();
            }
        }
    }

    void onVoteBtn(CCObject* sender) {
        auto playLayer = PlayLayer::get();
        if (!playLayer) return;

        int levelID = playLayer->m_level->m_levelID;

        auto likePopup = LikeItemLayer::create(LikeItemType::Level, levelID, 0);
        if (likePopup) likePopup->show();

        auto voteBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);
        if (!voteBtn) return;

        voteBtn->removeFromParent();
        voteBtn = nullptr;
    }
};