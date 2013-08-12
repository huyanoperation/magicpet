#pragma once

#include "cocos2d.h"
using namespace cocos2d;


typedef void (SelectorProtocol::*InputTouchCallBack)(CCNode*, void*);
#define InputCallBack( _FUNHANDLE ) (InputTouchCallBack)( &_FUNHANDLE )

class KeyboardNotificationLayer;
class TextFieldTTFActionTest;

enum
{
    InputLay_Normal,
    InputLay_Send,
    InputLay_Player_Rename,
    InputLay_Weibo,
};

class CInputTextLayer : public CCLayer
{
public:
    
    CInputTextLayer();
    ~CInputTextLayer(void);
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void darkenLayer(bool enable);
    
    void onEnter();
    void onExit();
    
    void SetTitle(string title) { m_title = title;};
    void SetTxtPos(CCPoint pos) { m_txtPos = pos;}
    void SetTxtLimit(int limit) { m_txtLimit = limit;};
    void SetType(int type) {m_eType = type;}
	void SetTestInputPos( CCPoint pos );
    
    void SetFunction( SelectorProtocol *psele, InputTouchCallBack fun);

    void closeCallback(CCObject * pSender);
    void sendCallback(CCObject * pSender);
//     static CInputTextLayer* GetInstance();
//     static CInputTextLayer*	m_pLayer;
    static bool m_sActive;

public:
    CCNode* m_nodeBoss;
public:  
    TextFieldTTFActionTest* m_pTestLayer;
    string     m_title;
    CCPoint    m_txtPos;
    int        m_txtLimit;
    int       m_eType;
    ccColor3B  m_sColor;
    InputTouchCallBack m_callBack;

};


class KeyboardNotificationLayer : public CCLayer, public CCIMEDelegate
{
public:
    KeyboardNotificationLayer();

    virtual void onClickTrackNode(bool bClicked) = 0;

    virtual void registerWithTouchDispatcher();
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);

    // CCLayer
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    CCNode * m_pTrackNode;
    CCPoint  m_beginPos;
};

//////////////////////////////////////////////////////////////////////////
// TextFieldTTFActionTest
//////////////////////////////////////////////////////////////////////////

class TextFieldTTFActionTest : public KeyboardNotificationLayer, public CCTextFieldDelegate
{
public:
    CCTextFieldTTF *    m_pTextField;
    CCAction *          m_pTextFieldAction;
    bool                m_bAction;
    int                 m_nCharLimit;       // the textfield max char limit
    CCPoint             m_ptTextPos;
    bool                m_bMessage;
    ccColor3B           m_sColor;
    InputTouchCallBack  m_callBack;
	SelectorProtocol* m_pSelectorTarget;

public:
    void callbackRemoveNodeWhenDidAction(CCNode * pNode);

    // KeyboardNotificationLayer
    virtual void onClickTrackNode(bool bClicked);

    // CCLayer
    virtual void onEnter();
    virtual void onExit();
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
    virtual bool onDraw(CCTextFieldTTF * pSender);
};
