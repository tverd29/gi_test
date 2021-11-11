#include "MyLayer.h"

USING_NS_CC;

MyLayer::MyLayer()
{
    text_label = Label::createWithTTF("", "fonts/arial.ttf", 18);
    if (text_label)
    {
        this->addChild(text_label, 1);
        text_label->setAnchorPoint(Vec2(0,1));
    }
}

MyLayer::~MyLayer(){}

bool MyLayer::init()
{
    return LayerColor::init();
}

MyLayer* MyLayer::create(const Color4B& color, float width, float height)
{
    MyLayer* layer = new (std::nothrow) MyLayer();
    if (layer && layer->initWithColor(color, width, height))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

void MyLayer::addText(const std::string& text)
{
    if(text_label)
    {
        setText(text_label->getString() + text);
    }
}

void MyLayer::setText(const std::string& text)
{
    if (text_label)
    {
        text_label->setString(text);
        text_label->setColor(Color3B::BLACK);
        // position the label on the center of the screen
        auto rect = this->getBoundingBox();
        text_label->setPosition(Vec2(rect.getMinX()+x_margin, rect.getMaxY()-y_margin));
        text_label->setMaxLineWidth(rect.getMaxX()-(2*x_margin));
    }
}

void MyLayer::clearText()
{
    if(text_label)
        text_label->setString("");
}
