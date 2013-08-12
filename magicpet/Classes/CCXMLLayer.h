#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

CCMenu* CreatGameUIButton( string filePath, SEL_MenuHandler callback, int Id, SelectorProtocol *target, CCPoint pos );
CCMenu* CreatGameUIWithSprite( CCSprite* pSprite, SEL_MenuHandler callback, int Id, SelectorProtocol *target, CCPoint pos );

#define MAXTORQUELAYERNUM 100

class CCXMLLayer : public CCLayer
{
public:
	CCXMLLayer();
	~CCXMLLayer();

	void onEnter();
	void onExit();
	void LoadPlist( const char *pList );
	void TurnitorqueWorldToCoco2d( float &x, float &y, float &layer );
	const char *valueForKey(const char *key, CCDictionary<std::string, CCObject*> *dict);
	vector< CCNode* > m_vNodeObject;
	CCNode* GetXMLNodeFromKey( string key );
	void RemoveNode( CCNode* pNode );

};