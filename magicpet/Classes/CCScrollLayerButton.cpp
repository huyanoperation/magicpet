#include "CCScrollLayerButton.h"

//-----------------------------------------------------
//
//
CCScrollLayerButton* CCScrollLayerButton::buttonWithFile(const char* filename, SelectorProtocol* target, SEL_MenuHandler selector)
{
//	CCScrollLayerButton *pRet = new CCScrollLayerButton();
//	if (pRet && pRet->initWithFile(filename, target, selector))
//	{
//		pRet->autorelease();
//		return pRet;
//	}
//	CC_SAFE_DELETE(pRet)
	return NULL;
}

bool CCScrollLayerButton::initWithFile(const char* filename, SelectorProtocol* target, SEL_MenuHandler selector)
{
	if (!CCSprite::initWithFile(filename))
	{
		return false;
	}

	this->target = target;
	this->selector = selector;
	touched = false;
	return true;
}

void CCScrollLayerButton::onEnter()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
	CCSprite::onEnter();
}

void CCScrollLayerButton::onExit()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}	

CCRect CCScrollLayerButton::rect()
{
	CCSize s = getTexture()->getContentSize();
	return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool CCScrollLayerButton::containsTouchLocation(CCTouch* touch)
{	
	return CCRect::CCRectContainsPoint(rect(), convertTouchToNodeSpaceAR(touch));
}

bool CCScrollLayerButton::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	if ( containsTouchLocation(pTouch) ) 
	{
		touched = true;
		touchPoint = pTouch->locationInView(pTouch->view());
		return true;
	}
	return false;
}

void CCScrollLayerButton::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{	
	CCPoint currTouchPoint = pTouch->locationInView(pTouch->view());
	// cancel touch if dragged too much
	if (ccpDistance(touchPoint,currTouchPoint) > TAP_MAX_DRAG)
	{
		touched = false;
	}
}

void CCScrollLayerButton::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	if (touched)
	{
		(target->*selector)(this);		
		touched = false;
	}
}