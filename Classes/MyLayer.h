#pragma once

#include "cocos2d.h"

class MyLayer : public cocos2d::LayerColor
{
private:
    cocos2d::Label* text_label;
    const int x_margin = 50;
    const int y_margin = 20;

public:
    void addText(const std::string& text);
    void setText(const std::string& text);
    void clearText();

    static MyLayer* create(const cocos2d::Color4B& color, float width, float height);

    CC_CONSTRUCTOR_ACCESS :
    MyLayer();
    virtual ~MyLayer();

    virtual bool init() override;

};
