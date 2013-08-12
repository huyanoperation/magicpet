#pragma once
#include "cocos2d.h"
#define TAP_MAX_DRAG 10
#include "GameConstant.h"
using namespace cocos2d;


class CCScrollLayerButton : public CCSprite, public CCTargetedTouchDelegate
{
public:
	bool initWithFile(const char* filename, SelectorProtocol* target, SEL_MenuHandler selector);
	SelectorProtocol* target;
	SEL_MenuHandler selector;
	bool touched;
	CCPoint touchPoint;
	static CCScrollLayerButton* buttonWithFile(const char* filename, SelectorProtocol* target, SEL_MenuHandler selector);

	// touch events
	CCRect rect();
	bool containsTouchLocation(CCTouch* touch);
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);	
};
