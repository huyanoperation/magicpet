/*
 *  CCScrollLayerExt.cpp
 *  SkeletonX
 *
 *  Created by mac on 11-11-18.
 *  Copyright 2011 GeekStudio. All rights reserved.
 *
 */

#include "CCScrollLayerExt.h"


CCScrollLayerExt::CCScrollLayerExt(CCRect mRect)
{
	this->setClipsToBounds(true);
	this->setTouchDispatch(false);
	this->setIsTouchEnabled(true);
	this->setLockVertical(false);
	this->setLockHorizontal(false);
	this->setContentSize(mRect.size);
	this->setPosition(mRect.origin);
	this->setShowsHorizontalScrollIndicator(true);
	this->setShowsVerticalScrollIndicator(true);
	this->setScrollDelegate(NULL);
	
	contentLayer = CCLayerColor::layerWithColorWidthHeight(ccc4(255, 0, 0, 255), mRect.size.width, mRect.size.height);
	contentLayer->setAnchorPoint(ccp(0.5,1.0));
	contentLayer->setIsRelativeAnchorPoint(true);
	contentLayer->setPosition(ccp(mRect.size.width/2.0,mRect.size.height));
	this->addChild(contentLayer);

	verticalScrollIndicator = NULL;
	if (verticalScrollIndicator == NULL)
	{
		CCSize vSize = CCSizeMake(4, mRect.size.height * mRect.size.height / contentLayer->getContentSize().height);
		verticalScrollIndicator = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 180), vSize.width,vSize.height);
		verticalScrollIndicator->setIsRelativeAnchorPoint(true);
		verticalScrollIndicator->setPosition(ccp(this->getContentSize().width - vSize.width/2.0,this->getContentSize().height-vSize.height/2.0));
		verticalScrollIndicator->setIsTouchEnabled(false);
		this->addChild(verticalScrollIndicator);
	}
	
	horizontalScrollIndicator = NULL;
	if (horizontalScrollIndicator == NULL)
	{
		CCSize hSize = CCSizeMake(mRect.size.width * mRect.size.width / contentLayer->getContentSize().width, 4);
		horizontalScrollIndicator = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 180), hSize.width,hSize.height);
		horizontalScrollIndicator->setIsRelativeAnchorPoint(true);
		horizontalScrollIndicator->setPosition(ccp(hSize.width/2.0,hSize.height/2.0));
		horizontalScrollIndicator->setIsTouchEnabled(false);
		this->addChild(horizontalScrollIndicator);
	}
}

CCScrollLayerExt::~CCScrollLayerExt()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	this->removeAllChildrenWithCleanup(true);
}


void CCScrollLayerExt::constraintContent()
{
	bool isLimitX = false;
	bool isLimitY = false;
	
	float curX = contentLayer->getPosition().x;
	float curY = contentLayer->getPosition().y;
	float width = contentLayer->getContentSize().width;
	float height = contentLayer->getContentSize().height;
	float anchorY = contentLayer->getAnchorPoint().y;
	float properX = curX> width/2.0 ? (isLimitX=true,width/2.0) :
					(curX< (this->getContentSize().width - width/2.0) ? (isLimitX=true,this->getContentSize().width - width/2.0) : curX) ;
	float properY = curY > height * anchorY ? (isLimitY=true,height * anchorY) : 
					(curY < (this->getContentSize().height) ? (isLimitY=true,this->getContentSize().height) : curY) ;
	
	//contentLayer->setPosition(ccp( properX,properY));
	
	if (isLimitX ||  isLimitY) {
		contentLayer->stopAllActions();
		
		
		if (scrollDelegate) {
			scrollDelegate->scrllViewWillBeginDecelerating(this);
		}
		
		CCEaseOut * fadeOut = CCEaseOut::actionWithAction(CCMoveTo::actionWithDuration(0.3, ccp( properX,properY)), 3);
		CCCallFunc * callBack = CCCallFunc::actionWithTarget(this, callfunc_selector(CCScrollLayerExt::scrollingEnd));
		contentLayer->runAction(CCSequence::actionOneTwo(fadeOut, callBack));
	}
	else {
		contentLayer->setPosition(ccp( properX,properY));
		
		if (scrollDelegate) {
			scrollDelegate->scrollViewDidEndMoving(this);
		}
	}
}

void CCScrollLayerExt::scrollingEnd()
{
	contentLayer->stopAllActions();
	this->setIsScrolling(false);
	
	if (scrollDelegate) {
		scrollDelegate->scrollViewDidEndDecelerating(this);
	}
}

void CCScrollLayerExt::setContentLayerSize(CCSize mSize)
{
	this->contentLayer->setContentSize(mSize);
}

void CCScrollLayerExt::setContentOffset(CCPoint mOffset,bool mAnimate)
{
	if (mAnimate) {
		CCEaseOut * fadeOut = CCEaseOut::actionWithAction(CCMoveTo::actionWithDuration(0.3,mOffset), 3);
		CCCallFunc * callBack = CCCallFunc::actionWithTarget(this, callfunc_selector(CCScrollLayerExt::constraintContent));
		contentLayer->runAction(CCSequence::actionOneTwo(fadeOut, callBack));
	}
	else {
		contentLayer->setPosition(mOffset);
		this->constraintContent();
	}
}

/////Menu - Events
void CCScrollLayerExt::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority, false);
}

bool CCScrollLayerExt::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (touchDispatch) {
		return !touchDispatch;
	}
	
	beginTouch = touch;
	CCPoint touchLocation = touch->locationInView(touch->view());
	firstTouchLocation = touchOnLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	
	touchLocation = this->convertToNodeSpace(touchOnLocation);
	CCRect r;
	r.origin = CCPointZero;
	r.size   = this->getContentSize();
	if (!CCRect::CCRectContainsPoint(r, touchLocation))
		return false;
	
	contentLayer->stopAllActions();
	
	CCTime::gettimeofdayCocos2d(&beginTime, NULL);
	this->setIsTouch(true);
	return !touchDispatch;
}

void CCScrollLayerExt::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
	CCPoint touchLocation = touch->locationInView(touch->view());
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	if (touch == beginTouch) {
		cc_timeval resultTime;
		cc_timeval endTime;
		CCTime::gettimeofdayCocos2d(&endTime, NULL);
		float delta = ccpDistance(touchLocation, firstTouchLocation);
		CCTime::timersubCocos2d(&resultTime, &beginTime, &endTime);
		float microSeconds	= resultTime.tv_sec * 1000.0f + resultTime.tv_usec/1000.0f;
		float seconds		= resultTime.tv_sec + resultTime.tv_usec/1000000.0f;
		//printf("time(%ld,%ld) ms=%f,second=%f\n",resultTime.tv_sec,resultTime.tv_usec,microSeconds,seconds);
		float speed = delta/(microSeconds);
		
		//CCPoint resultPoint = ccpAdd(contentLayer->getPosition(), delta);
		//this->constraintContent();
		
		if (speed >= 0.6f) { //速度
			contentLayer->stopAllActions();
			CCPoint deltaPos = ccpSub(touchLocation, firstTouchLocation);
			
			if (this->getLockVertical()) deltaPos.y = 0.0f;
			else if (this->getLockHorizontal()) deltaPos.x =0.0f;
			
			if (scrollDelegate) {
				scrollDelegate->scrllViewWillBeginDecelerating(this);
			}
			
			CCEaseOut * fadeOut = CCEaseOut::actionWithAction(CCMoveBy::actionWithDuration(seconds, deltaPos),3);
			CCCallFunc * callBack = CCCallFunc::actionWithTarget(this, callfunc_selector(CCScrollLayerExt::constraintContent));
			contentLayer->runAction(CCSequence::actionOneTwo(fadeOut, callBack));
			this->setIsScrolling(true);
			
			//printf("end (%f,%f) delta =%f,speed =%f deltapos(%f,%f)\n",touchLocation.x,touchLocation.y,delta,speed,deltaPos.x,deltaPos.y);
		}else {
			this->constraintContent();
		}
	}
	
	this->setIsTouch(false);
	touchOnLocation = CCPointZero;
}

void CCScrollLayerExt::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
	this->constraintContent();
	touchOnLocation = CCPointZero;
}

void CCScrollLayerExt::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	this->setIsTouch(true);
	CCPoint touchLocation = touch->locationInView(touch->view());
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	if (touch == beginTouch) {
		CCPoint delta = ccpSub(touchLocation, touchOnLocation);
		
		if (this->getLockVertical()) delta.y = 0.0f;
		else if (this->getLockHorizontal()) delta.x =0.0f;
		
		CCPoint resultPoint = ccpAdd(contentLayer->getPosition(), delta);
		contentLayer->setPosition(resultPoint);
		
		//printf("move (%f,%f) \n",touchLocation.x,touchLocation.y);
//		CCEaseOut * fadeOut = CCEaseOut::actionWithAction(CCMoveTo::actionWithDuration(0.3, resultPoint), 3);
//		contentLayer->runAction(fadeOut);
		
		this->setIsScrolling(true);
		//this->constraintContent();
		touchOnLocation = touchLocation;
		
		if (scrollDelegate) {
			scrollDelegate->scrollViewTouchMoving(this);
		}
	}
}

void CCScrollLayerExt::visit() 
{
	if (this->getClipsToBounds())
		this->preVisitWithClippingRect(CCRectMake(-this->getContentSize().width * this->getAnchorPoint().x,
												  -this->getContentSize().height* this->getAnchorPoint().y,
												  this->getContentSize().width * this->getScaleX(),
												  this->getContentSize().height*this->getScaleY()));
	CCLayerColor::visit();
	if (this->getClipsToBounds())
		this->postVisit();
	
	this->horizontalScrollIndicator->setIsVisible(this->getShowsHorizontalScrollIndicator());
	this->verticalScrollIndicator->setIsVisible(this->getShowsVerticalScrollIndicator());
	
	CCSize m_tScrollSize = contentLayer->getContentSize();
	CCSize m_tViewSize = this->getContentSize();
	
	if (this->getShowsVerticalScrollIndicator())
	{
		CCSize vScrollIndSize = CCSizeMake(4, m_tViewSize.height * m_tViewSize.height / m_tScrollSize.height);
		verticalScrollIndicator->setContentSize(vScrollIndSize);
		float fIndScaleOfV = (contentLayer->getPosition().y - m_tViewSize.height) * vScrollIndSize.height / m_tViewSize.height;
		CCPoint vIndPos = verticalScrollIndicator->getPosition();
		CCPoint vScrollIndPos = ccp(vIndPos.x, -fIndScaleOfV+ m_tViewSize.height - vScrollIndSize.height/2.0);
		verticalScrollIndicator->setPosition(vScrollIndPos);
	}
	
	if (this->getShowsHorizontalScrollIndicator())
	{
		CCSize hScrollIndSize = CCSizeMake(m_tViewSize.width * m_tViewSize.width / m_tScrollSize.width, 4);
		horizontalScrollIndicator->setContentSize(hScrollIndSize);
		float fIndScaleOfH = (contentLayer->getPosition().x - m_tViewSize.width) * hScrollIndSize.width / m_tViewSize.width;
		CCPoint hIndPos = horizontalScrollIndicator->getPosition();
		CCPoint hScrollIndPos = ccp(-fIndScaleOfH + m_tViewSize.width/2.0  - hScrollIndSize.width/2.0, hIndPos.y);
		horizontalScrollIndicator->setPosition(hScrollIndPos);
	}
}

void CCScrollLayerExt::preVisitWithClippingRect(CCRect clipRect)
{
    if (!this->getIsVisible())
        return;
	
    glEnable(GL_SCISSOR_TEST);
	
    CCDirector *director = CCDirector::sharedDirector();
    CCSize size =director->getWinSize();
    CCPoint origin = this->convertToWorldSpaceAR(clipRect.origin);
    CCPoint topRight =this->convertToWorldSpaceAR(ccpAdd(clipRect.origin, ccp(clipRect.size.width, clipRect.size.height)));
    CCRect scissorRect = CCRectMake(origin.x, origin.y, topRight.x-origin.x, topRight.y-origin.y);
	
    // transform the clipping rectangle to adjust to the current screen
    // orientation: the rectangle that has to be passed into glScissor is
    // always based on the coordinate system as if the device was held with the
    // home button at the bottom. the transformations account for different
    // device orientations and adjust the clipping rectangle to what the user
    // expects to happen.
    ccDeviceOrientation orientation =director->getDeviceOrientation();
    switch (orientation) {
        case kCCDeviceOrientationPortrait:
            break;
        case kCCDeviceOrientationPortraitUpsideDown:
            scissorRect.origin.x = size.width-scissorRect.size.width-scissorRect.origin.x;
            scissorRect.origin.y = size.height-scissorRect.size.height-scissorRect.origin.y;
            break;
        case kCCDeviceOrientationLandscapeLeft:
        {
            float tmp = scissorRect.origin.x;
            scissorRect.origin.x = scissorRect.origin.y;
            scissorRect.origin.y = size.width-scissorRect.size.width-tmp;
            tmp = scissorRect.size.width;
            scissorRect.size.width = scissorRect.size.height;
            scissorRect.size.height = tmp;
        }
            break;
        case kCCDeviceOrientationLandscapeRight:
        {
            float tmp = scissorRect.origin.y;
            scissorRect.origin.y = scissorRect.origin.x;
            scissorRect.origin.x = size.height-scissorRect.size.height-tmp;
            tmp = scissorRect.size.width;
            scissorRect.size.width = scissorRect.size.height;
            scissorRect.size.height = tmp;
        }
            break;
    }
	
    // Handle Retina
    scissorRect = CC_RECT_POINTS_TO_PIXELS(scissorRect);
	
    glScissor((GLint) scissorRect.origin.x, (GLint) scissorRect.origin.y,
              (GLint) scissorRect.size.width, (GLint) scissorRect.size.height);
}

void CCScrollLayerExt::postVisit()
{
    glDisable(GL_SCISSOR_TEST);
}