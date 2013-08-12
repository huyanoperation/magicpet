#include "InputTextLayer.h"
#include "../GameConstant.h"



bool CInputTextLayer::m_sActive = false;

const int  kTagFontLayer = 0;


CInputTextLayer::CInputTextLayer()
{
  CCSize s = CCDirector::sharedDirector()->getWinSize();
  m_txtPos = ccp(s.width / 2, s.height / 2 + 80);
  m_sColor = ccc3(255, 255, 255);

  setIsTouchEnabled( true );
}

CInputTextLayer::~CInputTextLayer(void)
{
}

void CInputTextLayer::onEnter()
{
	CCLayer::onEnter();

	//darkenLayer( true );

	//CCRect MondRect;

	//CGameSprite *m_pNameBG =  new CGameSprite( ARCHETYPE__command, ANIMATION_command_anim_name );
	//m_pNameBG->setPosition( m_txtPos );
	//m_pNameBG->GetCollisionRect(TAG_ear_low, MondRect);
	//addChild( m_pNameBG );
    
    ccColor4B color ={ 0,0,0,200 };
    CCLayerColor* pLayerColor = CCLayerColor::layerWithColor(color);
    
    addChild( pLayerColor );
    

    m_sActive = true;

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    m_nodeBoss = CCNode::node();
    addChild(m_nodeBoss, 0);

    m_pTestLayer = new TextFieldTTFActionTest();
    m_pTestLayer->m_nCharLimit = 5;
    m_pTestLayer->m_ptTextPos = m_txtPos;
    m_pTestLayer->m_bMessage = false;
    m_pTestLayer->m_callBack = m_callBack;
    m_pTestLayer->m_sColor = m_sColor;

	m_pTestLayer->autorelease();
    addChild(m_pTestLayer, 100);


}

void CInputTextLayer::SetTestInputPos( CCPoint pos )
{
	m_pTestLayer->setPosition( pos );
}

void CInputTextLayer::SetFunction( SelectorProtocol *psele, InputTouchCallBack fun) 
{
	m_callBack = fun;
	m_pTestLayer->m_callBack = m_callBack;
	m_pTestLayer->m_pSelectorTarget = psele;
}


void CInputTextLayer::closeCallback(CCObject * pSender)
{
   m_pTestLayer->m_pTextField->detachWithIME();
  (this->*m_callBack)( (CCNode*)0 ,(void*)NULL);
}

void CInputTextLayer::sendCallback(CCObject * pSender)
{
    m_pTestLayer->m_pTextField->detachWithIME();
    (this->*m_callBack)( (CCNode*)0 ,(void*)(m_pTestLayer->m_pTextField->getString()));
}


void CInputTextLayer::onExit()
{
    m_sActive = false;
	m_pTestLayer->m_pTextField->detachWithIME();
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}

void CInputTextLayer::registerWithTouchDispatcher()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

bool CInputTextLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CInputTextLayer::darkenLayer(bool enable) 
{
    if (enable)
    {   
        CCNode* node = getChildByTag(kTagFontLayer);
        if (NULL == node)
        {   
            CCSize s = CCDirector::sharedDirector()->getWinSize();
            CCLayerColor* layer1 = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 180), s.width, s.height );            
            addChild(layer1,-1,kTagFontLayer);
        }
    }
    else
    {
        removeChildByTag(kTagFontLayer,true);
    }
}

static CCRect getRect(CCNode * pNode)
{
    CCRect rc;
    rc.origin = pNode->getPosition();
    rc.size = pNode->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

KeyboardNotificationLayer::KeyboardNotificationLayer()
: m_pTrackNode(0)
{
    setIsTouchEnabled(true);
}

void KeyboardNotificationLayer::registerWithTouchDispatcher()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

void KeyboardNotificationLayer::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    CCLOG("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)",
        info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);

    if (! m_pTrackNode)
    {
        return;
    }

    CCRect rectTracked = getRect(m_pTrackNode);
    CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
        rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);

    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (! CCRect::CCRectIntersectsRect(rectTracked, info.end))
    {
        return;
    }

    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = CCRect::CCRectGetMaxY(info.end) - CCRect::CCRectGetMinY(rectTracked);
    CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);

    // move all the children node of KeyboardNotificationLayer
    CCArray * children = getChildren();
    CCNode * node = 0;
    int count = children->count();
    CCPoint pos;
    for (int i = 0; i < count; ++i)
    {
        node = (CCNode*)children->objectAtIndex(i);
        pos = node->getPosition();
        pos.y += adjustVert;
        node->setPosition(pos);
    }
}

// CCLayer function

bool KeyboardNotificationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
    m_beginPos = pTouch->locationInView(pTouch->view());	
    m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);
    return true;
}

void KeyboardNotificationLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    return;
    if (! m_pTrackNode)
    {
        return;
    }

    CCPoint endPos = pTouch->locationInView(pTouch->view());	
    endPos = CCDirector::sharedDirector()->convertToGL(endPos);

    float delta = 5.0f;
    if (::abs(endPos.x - m_beginPos.x) > delta
        || ::abs(endPos.y - m_beginPos.y) > delta)
    {
        // not click
        m_beginPos.x = m_beginPos.y = -1;
        return;
    }

    // decide the trackNode is clicked.
    CCRect rect;
    CCPoint point = convertTouchToNodeSpaceAR(pTouch);
    CCLOG("KeyboardNotificationLayer:clickedAt(%f,%f)", point.x, point.y);

    rect = getRect(m_pTrackNode);
    CCLOG("KeyboardNotificationLayer:TrackNode at(origin:%f,%f, size:%f,%f)",
        rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);

    this->onClickTrackNode(CCRect::CCRectContainsPoint(rect, point));
    CCLOG("----------------------------------");
}

void TextFieldTTFActionTest::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
   //if (m_bMessage)
   //{
       KeyboardNotificationLayer::ccTouchEnded(pTouch, pEvent);
   //}   
}

void TextFieldTTFActionTest::onClickTrackNode(bool bClicked)
{
    CCTextFieldTTF * pTextField = (CCTextFieldTTF*)m_pTrackNode;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFActionTest:CCTextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFActionTest:CCTextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}

void TextFieldTTFActionTest::onEnter()
{
    KeyboardNotificationLayer::onEnter();

    //if (m_nCharLimit < 8)
    //{
    //   m_nCharLimit = 8;
    //}
     //m_nCharLimit = 6;

    m_pTextFieldAction = CCRepeatForever::actionWithAction(
        (CCActionInterval*)CCSequence::actions(
        CCFadeOut::actionWithDuration(0.25),
        CCFadeIn::actionWithDuration(0.25),
        0
        ));
    m_pTextFieldAction->retain();
    m_bAction = false;

    // add CCTextFieldTTF
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    m_pTextField = CCTextFieldTTF::textFieldWithPlaceHolder("petName",
		kFontSystem[FONT_TALL].fontName,kFontSystem[FONT_TALL].fontSize);

    m_pTextField->setColor(ccWhite);

    addChild(m_pTextField);
    
    m_pTextField->setDelegate(this);
   // m_pTextField->m_nMaxChar = 10;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
    // on android, CCTextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position
    m_pTextField->setPosition(ccp(s.width / 2, s.height/2 + 50));
#else
    m_pTextField->setPosition(m_ptTextPos);
#endif

    m_pTrackNode = m_pTextField;

    m_pTextField->attachWithIME();
}

void TextFieldTTFActionTest::onExit()
{
    KeyboardNotificationLayer::onExit();
	//if( NULL != m_pTextFieldAction )
	//{
	//	m_pTextFieldAction->release();
	//}
}

// CCTextFieldDelegate protocol
bool TextFieldTTFActionTest::onTextFieldAttachWithIME(CCTextFieldTTF * pSender)
{
    if (! m_bAction)
    {
        m_pTextField->runAction(m_pTextFieldAction);
        m_bAction = true;
    }
    return false;
}

bool TextFieldTTFActionTest::onTextFieldDetachWithIME(CCTextFieldTTF * pSender)
{
    if (m_bAction)
    {
        m_pTextField->stopAction(m_pTextFieldAction);
        m_pTextField->setOpacity(255);
        m_bAction = false;
    }
    return false;
}

bool TextFieldTTFActionTest::onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen)
{
    // if insert enter, treat as default to detach with ime
    
    if ('\n' == *text)
    {
        string str = m_pTextField->getString();
        if (str.empty())
        {
            return true;
        }

        if (!m_bMessage)
        {
            m_pTextField->detachWithIME();
            (m_pSelectorTarget ->*m_callBack)( (CCNode*)0 ,(void*)(m_pTextField->getString()));

            return true;
        }
        else
           return false;
    }

    // if the textfield's char count more than m_nCharLimit, doesn't insert text anymore.
    if (pSender->getCharCount() >= m_nCharLimit)
    {
        return true;
    }

    // create a insert text sprite and do some action
     CCLabelTTF * label = CCLabelTTF::labelWithString(text,  kFontSystem[FONT_MED].fontName, 30);
     this->addChild(label);
     ccColor3B color = { 255, 255, 255};
     label->setColor(color);
 
     // move the sprite from top to position
     CCPoint endPos = pSender->getPosition();
     if (pSender->getCharCount())
     {
         endPos.x += pSender->getContentSize().width / 2;
     }
     CCSize  inputTextSize = label->getContentSize();
     CCPoint beginPos(endPos.x, CCDirector::sharedDirector()->getWinSize().height - inputTextSize.height * 2); 
 
     ccTime duration = 0.5;
     label->setPosition(beginPos);
     label->setScale(8);
 
     CCAction * seq = CCSequence::actions(
         CCSpawn::actions(
         CCMoveTo::actionWithDuration(duration, endPos),
         CCScaleTo::actionWithDuration(duration, 1),
         CCFadeOut::actionWithDuration(duration),
         0),
         CCCallFuncN::actionWithTarget(this, callfuncN_selector(TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction)),
         0);
     label->runAction(seq);
    return false;
}

bool TextFieldTTFActionTest::onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen)
{
    // create a delete text sprite and do some action
    CCLabelTTF * label = CCLabelTTF::labelWithString(delText, kFontSystem[FONT_MED].fontName, 30);
    this->addChild(label);   

    // move the sprite to fly out
     CCPoint beginPos = pSender->getPosition();
     CCSize textfieldSize = pSender->getContentSize();
     CCSize labelSize = label->getContentSize();
     beginPos.x += (textfieldSize.width - labelSize.width) / 2.0f;
 
     CCSize winSize = CCDirector::sharedDirector()->getWinSize();
     CCPoint endPos(- winSize.width / 4.0f, winSize.height * (0.5 + (float)rand() / (2.0f * RAND_MAX)));
 
     ccTime duration = 1;
     ccTime rotateDuration = 0.2f;
     int repeatTime = 5; 
     label->setPosition(beginPos);
 
     CCAction * seq = CCSequence::actions(
         CCSpawn::actions(
         CCMoveTo::actionWithDuration(duration, endPos),
         CCRepeat::actionWithAction(
         CCRotateBy::actionWithDuration(rotateDuration, (rand()%2) ? 360 : -360),
         repeatTime),
         CCFadeOut::actionWithDuration(duration),
         0),
         CCCallFuncN::actionWithTarget(this, callfuncN_selector(TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction)),
         0);
     label->runAction(seq);
    return false;
}

bool TextFieldTTFActionTest::onDraw(CCTextFieldTTF * pSender)
{
    return false;
}

void TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction(CCNode * pNode)
{
    this->removeChild(pNode, true);
}