#pragma once
#include "GameConstant.h"
#include "Box2D/Box2D.h"
#include "../CCXMLLayer.h"


enum
{
	CPETATTRIBUTELAYER_BUTTON_BACK,
	CPETATTRIBUTELAYER_BUTTON_EVOLUTION
};


class CPetAttributeLayer : public CCXMLLayer
{
public:
	CPetAttributeLayer();
	~CPetAttributeLayer();
    
	virtual void onEnter();
	virtual void onExit();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	void menuCallback(CCObject * pSender);
	void UpData( ccTime tt );

	CCSprite *m_pSpriteSkillExp;
	float	m_skillExpWidth;
	CCLabelTTF	*m_pTTFSkillExp;
	CCSprite    *m_pButtonAddSkill[3];

	CCLabelTTF	*m_pTTFSkillItem[3];
	CCLabelTTF	*m_pTTFSkillLevel[3];

	int m_iAddSkillExpIndex;
    
};
