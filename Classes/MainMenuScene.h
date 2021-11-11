#pragma once

#ifndef __MainMenu_SCENE_H__
#define __MainMenu_SCENE_H__

#include "cocos2d.h"

class MainMenuScene: public cocos2d::Scene
{
private:
    cocos2d::MenuItemLabel* continue_item = nullptr;

    //init labels on main menu
    bool init_game_labels();
    // init menu and menu items
    bool init_menu();

public:
    void enableContinue();
    void disableContinue();

    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
};

#endif // __MainMenu_SCENE_H__
