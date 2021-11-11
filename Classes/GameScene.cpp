#include "GameScene.h"

#include "Constants.h"
#include <regex>
#include <codecvt>

USING_NS_CC;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


namespace helper
{
    std::string toString(const std::wstring& wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
        return conv.to_bytes(wstr);
    }

    std::wstring toWstring(const std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
        return conv.from_bytes(str);
    }

    std::wstring getName(const std::wstring& text)
    {
        std::wstring res;
        std::wsmatch match;
        std::wregex reg(L"\\b.+?\\b");
        if(std::regex_search(text, match, reg))
            res = match[0].str();
        return res;
    }

    std::wstring getAppearance(const std::wstring& text)
    {
        std::wstring res;
        std::wsmatch match;
        std::wregex reg(L"appearance:\"(.+)\"");
        if(std::regex_search(text, match, reg) && match.size() > 1)
        {
            res = match[1].str();
        }
        return res;
    }

    int getOrder(const std::wstring& text)
    {
        std::wstring res = L"0";
        std::wsmatch match;
        std::wregex reg(L"order:([0-9]+)");
        if(std::regex_search(text, match, reg) && match.size() > 1)
        {
            res = match[1].str();
        }
        return std::stoi(res);
    }

    void getPosition(const std::wstring& text, int& x_pos, int& y_pos)
    {
        std::wsmatch match;
        std::wregex reg(L"position:([0-9]+),([0-9]+)");
        if (std::regex_search(text, match, reg) && match.size() > 2)
        {
                x_pos = std::stoi(match[1].str());
                y_pos = std::stoi(match[2].str());
        }
    }

    bool getInput(const std::wstring& text)
    {
        std::wsmatch match;
        std::wregex reg(L"input:(true|false)");
        if (std::regex_search(text, match, reg) && match.size() > 1)
        {
            return match[1].str() == L"true";
        }
        return false;
    }
}


void GameScene::addBackgrounWithName(const std::wstring& bg_name, const std::wstring& path, int z_order /*= 0*/)
{
    std::string bg_name_str = helper::toString(bg_name);
    std::string path_str = helper::toString(path);
    if (path.size())
    {
        Sprite* background = Sprite::create(path_str);
        if (background)
        {
            background->setName(bg_name_str);
            auto visible_size = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();

            scale_factor_X = visible_size.width / background->getContentSize().width;
            scale_factor_Y = visible_size.height / background->getContentSize().height;
            background->setScaleX(scale_factor_X);
            background->setScaleY(scale_factor_Y);
            background->setPosition(Vec2(visible_size.width / 2 + origin.x, visible_size.height / 2 + origin.y));
            background->setOpacity(0);
            this->addChild(background, (-1 * z_order));
        }
        else
        {
            problemLoading(path_str.c_str());
        }
    }
}

Sprite* GameScene::getBackgroundWithName(const std::wstring& bg_name)
{
    std::string bg_name_str = helper::toString(bg_name);
    auto bg = dynamic_cast<Sprite*>(this->getChildByName(bg_name_str));
    if (bg)
    {
        return bg;
    }
    return nullptr;
}

void GameScene::addSpeakerWithName(const std::wstring& s_name, const std::wstring& path, int z_order /*= 0*/, float x_pos /*= -1*/, float y_pos /*= -1*/)
{
    std::string s_name_str = helper::toString(s_name);
    std::string path_str = helper::toString(path);

    Sprite* speaker = nullptr;

    auto sp_iter = speakers.find(s_name_str);
    if(sp_iter != speakers.end())
    {
        auto it = sp_iter->second.find(path_str);
        if(it != sp_iter->second.end())
        {
            speaker = it->second;
        }
        else
        {
            Sprite* tmp_sprite = sp_iter->second.getRandomObject();
            speaker = Sprite::create(path_str);
            if (speaker != nullptr)
            {
                speaker->setName(path_str);
                speaker->setAnchorPoint(Vec2(0.5f, 0.0f));
                speaker->setPosition(tmp_sprite->getPosition());
                this->addChild(speaker, (-1 * z_order));

                sp_iter->second.insert(path_str, speaker);
            }
            else
            {
                problemLoading(path_str.c_str());
                return;
            }
        }
    }
    else
    {
        speaker = Sprite::create(path_str);
        if (speaker != nullptr)
        {
            speaker->setName(path_str);
            speaker->setAnchorPoint(Vec2(0.5f, 0.0f));
            this->addChild(speaker, (-1 * z_order));

            Map<std::string, Sprite*> sub_map;
            sub_map.insert(path_str, speaker);
            speakers.insert(std::pair<std::string, Map<std::string, Sprite*>>(s_name_str, sub_map));
        }
        else
        {
            problemLoading(path_str.c_str());
            return;
        }
    }

    auto visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if(x_pos >= 0 && y_pos >= 0)
    {
        float x = visible_size.width * (x_pos / 100) + origin.x;
        float y = visible_size.height * (y_pos / 100) + origin.y;
        Vec2 pos(x, y);
        speaker->setPosition(pos);
    }

    speaker->setOpacity(0);
    speaker->setScaleX(scale_factor_X);
    speaker->setScaleY(scale_factor_Y);
}

Sprite* GameScene::getSpeakerByNameAndPath(const std::wstring& s_name, const std::wstring& path)
{
    Sprite* res = nullptr;
    auto sp_iter = speakers.find(helper::toString(s_name));
    if (sp_iter != speakers.end())
    {
        auto it = sp_iter->second.find(helper::toString(path));
        if(it != sp_iter->second.end())
        {
            res = it->second;
        }
    }
    return res;
}

Sprite* GameScene::getVisibleSpeakerByName(const std::wstring& s_name)
{
    Sprite* res = nullptr;
    auto sp_iter = speakers.find(helper::toString(s_name));
    if(sp_iter != speakers.end())
    {
        for(const auto& it: sp_iter->second)
        {
            if (it.second->getOpacity() == 255)
            {
                res = it.second;
            }
        }
    }
    return res;
}

Map<std::string, Sprite*> GameScene::getSpeakersByName(const std::wstring& s_name)
{
    Vector<Sprite*> res;
    std::string name = helper::toString(s_name);
    auto sp_iter = speakers.find(helper::toString(s_name));
    if (sp_iter != speakers.end())
    {
        return sp_iter->second;
    }
    return Map<std::string, Sprite*>();
}

bool GameScene::initPrinter()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    Color4B text_block_bg_color(250, 240, 230, 200);
    float text_block_height_per = 0.3f;
    Vec2 right_top_layer(visible_size.width, visible_size.height * text_block_height_per);
    printer = MyLayer::create(text_block_bg_color, right_top_layer.x, right_top_layer.y);
    if(printer)
    {
        printer->setPosition(0, 0);
        printer->setName("printer");
        this->addChild(printer, 1);
        hidePrinter();
        return true;
    }
    return false;
}

void GameScene::showPrinter(const std::wstring& text_to_shown /*= ""*/)
{
    if(printer)
    {
        printer->setVisible(true);
        if(text_to_shown.size())
        {
            printer->setText(helper::toString(text_to_shown));
        }
    }
}

void GameScene::hidePrinter()
{
    if(printer)
    {
        printer->setVisible(false);
    }
}

void GameScene::showBackButton()
{
    if(back_button && !back_button->isVisible())
    {
        // не доработал
        //back_button->setVisible(true);
    }
}

void GameScene::hideBackButton()
{
    if(back_button && back_button->isVisible())
    {
        back_button->setVisible(false);
    }
}

bool GameScene::initStartView()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    igs_label = Label::createWithTTF("Insert game scenario", "fonts/arial.ttf", 48);
    if (igs_label)
    {
        igs_label->setName("igs_label");
        Color4B igs_label_color(250, 240, 230, 200);
        igs_label->setColor(Color3B(igs_label_color));
        igs_label->enableOutline(Color4B::BLACK, 1);
        igs_label->setPosition(Vec2(origin.x + visible_size.width / 2,
            origin.y + visible_size.height - igs_label->getContentSize().height));
        this->addChild(igs_label, 1);
        
        text_field = ui::EditBox::create(Size(visible_size.width - 100, visible_size.height - igs_label->getContentSize().height - 20 - 100), "res/bg/gray_bg.jpg");
        if(text_field)
        {
            text_field->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
            text_field->setFontName("Arial");
            text_field->setFontSize(18);
            text_field->setPlaceHolder("insert scenario to box");
            text_field->setPlaceholderFontColor(Color3B::GRAY);
            text_field->setPlaceholderFontSize(20);
            text_field->setInputMode(ui::EditBox::InputMode::ANY);
            text_field->setName("text_field");
            text_field->setAnchorPoint(Vec2(0.5, 1));
            text_field->setPosition(Vec2(visible_size.width / 2, igs_label->getBoundingBox().getMinY() - 20));
            //text_field->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
            //});
            this->addChild(text_field);

            start_button = ui::Button::create("", "", "");
            if(start_button)
            {
                start_button->setTitleText("Start with scenario");
                start_button->setName("start_button");
                start_button->setAnchorPoint(Vec2(0.5, 0.5));
                start_button->setTitleFontSize(24);
                start_button->setPosition(Vec2(visible_size.width / 2, text_field->getBoundingBox().getMinY() - 30));
                start_button->addTouchEventListener(CC_CALLBACK_2(GameScene::startScenarioButtonClicked,this));
                this->addChild(start_button);

                return true;
            }
        }
    }
    return false;
}

bool GameScene::initBackButton()
{
    back_button = ui::Button::create("", "", "");
    Label* back_button_label = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 36);
    if (back_button && back_button_label)
    {
        back_button_label->setName("back_button_label");
        back_button_label->setColor(Color3B(250, 240, 230));
        back_button_label->enableOutline(Color4B::BLACK, 3);

        auto visible_size = Director::getInstance()->getVisibleSize();
        back_button->setTitleLabel(back_button_label);
        back_button->setName("back");
        back_button->setAnchorPoint(Vec2(0, 0.5));
        back_button->setPosition(Vec2(10, visible_size.height/2));
        back_button->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type){
            if(type == ui::Widget::TouchEventType::ENDED)
            this->previousSequence();
        });
        hideBackButton();
        this->addChild(back_button, 1);

        return true;
    }
    return false;
}

bool GameScene::parseScenario(const std::string& text)
{
    Vector<FiniteTimeAction*> actions;

    std::wstring wtext;
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
    wtext = conv.from_bytes(text);

    std::wregex reg(L"^[^$].+");
    std::wsregex_token_iterator iter(wtext.begin(), wtext.end(), reg);
    std::wsregex_token_iterator end;
    for(;iter != end; ++iter)
    {
        bool need_input_waiting = false;
        std::wregex cmd_reg(L"^@(\\w+)\\s");
        std::wregex person_txt(L"^.+:");
        std::wsmatch match;
        std::wstring sub_str = iter->str();
        if (std::regex_search(sub_str, match, cmd_reg) && match.size() > 1)
        {
            //add command
            std::wstring cmd = match[1].str();
            std::wstring suf = match.suffix();
            if(cmd == L"back")
            {
                std::wstring bg_name = helper::getName(suf);
                std::wstring appearance = helper::getAppearance(suf);
                int order = helper::getOrder(suf);
                if(bg_name.size() && appearance.size())
                {
                    addBackgrounWithName(bg_name, appearance, order);
                    FiniteTimeAction* act = CallFunc::create([this, bg_name]() {
                        Sprite* bg = getBackgroundWithName(bg_name);
                        if (bg)
                        {
                            auto fade_in = FadeIn::create(0.25f);
                            bg->runAction(fade_in);
                        }
                    });
                    actions.pushBack(act);
                }
            }
            else if (cmd == L"hideBack")
            {
                std::wstring bg_name = helper::getName(suf);
                if (bg_name.size())
                {
                    FiniteTimeAction* act = CallFunc::create([this, bg_name]() {
                        Sprite* bg = getBackgroundWithName(bg_name);
                        if (bg)
                        {
                            auto fade_out = FadeOut::create(0.25f);
                            bg->runAction(fade_out);
                        }
                    });
                    actions.pushBack(act);
                }
            }
            else if (cmd == L"showPrinter")
            {
                FiniteTimeAction* act = CallFunc::create([this]() {
                    this->showPrinter();
                });
                actions.pushBack(act);
            }
            else if (cmd == L"hidePrinter")
            {
                FiniteTimeAction* act = CallFunc::create([this]() {
                    this->hidePrinter();
                });
                actions.pushBack(act);
            }
            else if (cmd == L"char")
            {
                std::wstring char_name = helper::getName(suf);
                std::wstring appearance = helper::getAppearance(suf);
                int order = helper::getOrder(suf);
                int x_pos = -1;
                int y_pos = -1;
                helper::getPosition(suf, x_pos, y_pos);
                if(char_name.size() && appearance.size())
                {
                    this->addSpeakerWithName(char_name, appearance, order, x_pos, y_pos);

                    FiniteTimeAction* act = CallFunc::create([this, char_name, appearance]() {
                        auto speakers = getSpeakersByName(char_name);
                        for(auto sp_iter: speakers)
                        {
                            if(sp_iter.first == helper::toString(appearance))
                            {
                                auto fade_in = FadeIn::create(0.25f);
                                sp_iter.second->runAction(fade_in);
                            }
                            else
                            {
                                auto fade_out = FadeOut::create(0.25f);
                                sp_iter.second->runAction(fade_out);
                            }
                        }
                    });
                    actions.pushBack(act);
                }
            }
            else if (cmd == L"hideChar")
            {
                std::wstring char_name = helper::getName(suf);
                if (char_name.size())
                {
                    FiniteTimeAction* hide = CallFunc::create([this, char_name]() {
                        auto speakers = getSpeakersByName(char_name);
                        for (auto sp_iter : speakers)
                        {
                            auto fade_out = FadeOut::create(0.25f);
                            sp_iter.second->runAction(fade_out);
                        }
                    });
                    actions.pushBack(hide);
                }
            }

            need_input_waiting = helper::getInput(suf);
        }
        else if (std::regex_search(sub_str, match, person_txt))
        {
            //add person text
            FiniteTimeAction* act = CallFunc::create([this, sub_str]() {
                this->showPrinter(sub_str);
            });
            actions.pushBack(act);
            need_input_waiting = true;
        }
        else
        {
            //add hero text
            FiniteTimeAction* act = CallFunc::create([this, sub_str]() {
                this->showPrinter(sub_str);
            });
            actions.pushBack(act);
            need_input_waiting = true;
        }

        if (need_input_waiting)
        {
            FiniteTimeAction* act = CallFunc::create([this, text]() {
                is_running_scenario = false;
            });
            actions.pushBack(act);
            scenarios.pushBack(Sequence::create(actions));

            actions.clear();
        }
    }

    if(actions.size())
    {
        FiniteTimeAction* act = CallFunc::create([this, text]() {
            is_running_scenario = false;
        });
        actions.pushBack(act);

        scenarios.pushBack(Sequence::create(actions));
        actions.clear();
    }
    return scenarios.size();
}

void GameScene::startScenario()
{
    scenario_iter = scenarios.begin();
    run();
}

bool GameScene::run()
{
    if (scenario_iter != scenarios.end())
    {
        is_running_scenario = true;
        this->runAction(*scenario_iter);
        return true;
    }
    return false;
}

void GameScene::nextSequence()
{
    ++scenario_iter;
    if(!run())
    {
        EventCustom event("gi_event");
        gi_test::MyEventType type = gi_test::MyEventType::END_GAME;
        event.setUserData(&type);
        _eventDispatcher->dispatchEvent(&event);
    }
    else
    {
        showBackButton();
    }
}

void GameScene::previousSequence()
{
    this->runAction((*scenario_iter)->reverse());
    --scenario_iter;
    if(scenario_iter == scenarios.begin())
    {
        hideBackButton();
    }
    run();
}

void GameScene::startScenarioButtonClicked(Ref* sender, ui::Widget::TouchEventType type)
{
    switch (type)
    {
    case ui::Widget::TouchEventType::BEGAN:
        break;
    case ui::Widget::TouchEventType::ENDED:
    {
        is_parsed_scenario = parseScenario(text_field->getText());
        if (is_parsed_scenario)
        {
            this->removeChild(igs_label);
            this->removeChild(text_field);
            this->removeChild(start_button);
        
            startScenario();
        }
        else
        {
            text_field->setText("");
            text_field->setPlaceHolder("Bad scenario. Pls insert better scenario");
        }
        break;
    }
    default:
        break;
    }
}

cocos2d::Scene* GameScene::createScene()
{
     return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    if(initStartView() && initPrinter() && initBackButton())
    {
        auto listener1 = EventListenerTouchOneByOne::create();
        listener1->onTouchBegan = [](Touch* touch, Event* event) {return true;};
        listener1->onTouchMoved = [](Touch* touch, Event* event) {};
        listener1->onTouchEnded = [=](Touch* touch, Event* event) {
            if(is_parsed_scenario && !is_running_scenario)
            {
                nextSequence();
            }
            log("touched");
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){};
        listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event){
            if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
            {
                EventCustom event("gi_event");
                gi_test::MyEventType type = gi_test::MyEventType::NEED_MENU;
                event.setUserData(&type);
                _eventDispatcher->dispatchEvent(&event);
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        return true;
    }

    return false;
}
