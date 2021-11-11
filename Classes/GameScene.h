#pragma once

#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "MyLayer.h"


class GameScene: public cocos2d::Scene
{
private:
    cocos2d::Vector<cocos2d::Sequence*>::iterator scenario_iter;
    cocos2d::Vector<cocos2d::Sequence*> scenarios;

    std::map<std::string, cocos2d::Map<std::string, cocos2d::Sprite*>> speakers;

    MyLayer* printer = nullptr;

    cocos2d::Label* igs_label = nullptr;
    cocos2d::ui::EditBox* text_field = nullptr;
    cocos2d::ui::Button* start_button = nullptr;
    cocos2d::ui::Button* back_button = nullptr;

    float scale_factor_X = 1;
    float scale_factor_Y = 1;
    bool is_running_scenario = false;
    bool is_parsed_scenario = false;

private:
    bool initPrinter();
    bool initStartView();
    bool initBackButton();

    bool parseScenario(const std::string& text);

    void startScenario();
    bool run();
    void nextSequence();
    void previousSequence();

public:
    void addBackgrounWithName(const std::wstring& bg_name, const std::wstring& path, int z_order = 0);
    cocos2d::Sprite* getBackgroundWithName(const std::wstring& bg_name);

    void addSpeakerWithName(const std::wstring& s_name, const std::wstring& path, int z_order = -1, float x_pos = -1, float y_pos = 0);
    cocos2d::Sprite* getSpeakerByNameAndPath(const std::wstring& s_name, const std::wstring& path);
    cocos2d::Sprite* getVisibleSpeakerByName(const std::wstring& s_name);
    cocos2d::Map<std::string, cocos2d::Sprite*> getSpeakersByName(const std::wstring& s_name);

    void showPrinter(const std::wstring& text_to_shown = L"");
    void hidePrinter();

    void showBackButton();
    void hideBackButton();

    void startScenarioButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __Game_SCENE_H__
