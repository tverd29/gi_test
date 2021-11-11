#include "MainMenuScene.h"

#include "Constants.h"
#include "GameScene.h"

USING_NS_CC;

const Color3B enabled(245, 245, 20);
const Color3B disabled(169, 169, 169);

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

MenuItemLabel* get_menu_label_item(std::string text, const ccMenuCallback& callback)
{
    auto continue_label = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 24);
    if(continue_label)
    {
        auto continue_item = MenuItemLabel::create(continue_label, callback);
        if(continue_item)
        {
            continue_item->setColor(enabled);
            continue_item->setDisabledColor(disabled);
            return continue_item;
        }
    }
    return nullptr;
}

bool MainMenuScene::init_game_labels()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto g_label = Label::createWithTTF("Game Insight Test", "fonts/Marker Felt.ttf", 48);
    if (g_label)
    {
        Color4B g_label_color(250, 240, 230, 200);
        g_label->setColor(Color3B(g_label_color));
        g_label->enableOutline(Color4B::BLACK, 1);
        // position the label on the center of the screen
        g_label->setPosition(Vec2(origin.x + visible_size.width / 2,
            origin.y + visible_size.height - g_label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(g_label, 1);
    }
    return true;
}

bool MainMenuScene::init_menu()
{
    auto visible_size = Director::getInstance()->getVisibleSize();

    // MenuItems
    Vector<MenuItem*> MenuItems;

    this->continue_item = get_menu_label_item("CONTINUE", [&](Ref* sender){
        //start game from the last save
        EventCustom event("gi_event");
        gi_test::MyEventType type = gi_test::MyEventType::CONTINUE;
        event.setUserData(&type);
        _eventDispatcher->dispatchEvent(&event);
    });
    if(continue_item)
    {
        continue_item->setEnabled(false);
        MenuItems.pushBack(continue_item);
    }
    else
    {
        problemLoading("continue_item");
        return false;
    }

    auto new_game_item = get_menu_label_item("NEW GAME", [&](Ref* sender) {
        // start a new game
        EventCustom event("gi_event");
        gi_test::MyEventType type = gi_test::MyEventType::NEW_GAME;
        event.setUserData(&type);
        _eventDispatcher->dispatchEvent(&event);
    });
    if(new_game_item)
    {
        MenuItems.pushBack(new_game_item);
    }
    else
    {
        problemLoading("new_game_item");
        return false;
    }

    auto close_item = get_menu_label_item("CLOSE", [&](Ref* sender) {
        Director::getInstance()->end();
    });
    if (close_item)
    {
        MenuItems.pushBack(close_item);
    }
    else
    {
        problemLoading("close_item");
        return false;
    }

    auto menu = Menu::createWithArray(MenuItems);
    if(menu)
    {
        menu->setPosition(visible_size.width / 2, visible_size.height / 2);
        menu->alignItemsVerticallyWithPadding(20);
        this->addChild(menu, 1);
    }
    else
    {
        problemLoading("menu");
        return false;
    }

    return true;
}

void MainMenuScene::enableContinue()
{
    if(continue_item)
        continue_item->setEnabled(true);
}

void MainMenuScene::disableContinue()
{
    if (continue_item)
        continue_item->setEnabled(false);
}

cocos2d::Scene* MainMenuScene::createScene()
{
     return MainMenuScene::create();
}

bool MainMenuScene::init()
{
    if(Scene::init() && init_game_labels() && init_menu())
    {
        //key event
        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {};
        listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event) {
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
            {
                Director::getInstance()->end();
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        this->setColor(Color3B(128,128,128));

        return true;
    }

    return false;
}
