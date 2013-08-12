#include "CCXMLLayer.h"
#include "CCFileUtils.h"
#include "CCString.h"
#include "Sprite/AdvanceSprite.h"
#include "GameConstant.h"

//--------------------------------------------------------------------------------------------------------------------
//
//
CCMenu* CreatGameUIButton( string filePath, SEL_MenuHandler callback, int Id, SelectorProtocol *target, CCPoint pos )
{
	CCSprite *spriteNormal = CCSprite::spriteWithFile( filePath.c_str() );
	CCSprite *spriteSelected = CCSprite::spriteWithFile( filePath.c_str() );
	spriteSelected->setColor(ccc3( 100,100,100 ));

	CCMenuItemSprite* item1 = CCMenuItemSprite::itemFromNormalSprite(spriteNormal, spriteSelected, target, callback );
	item1->setTag( Id );


	CCMenu* pMenu = CCMenu::menuWithItems( item1, NULL);
	pMenu->setPosition( pos );
	return pMenu;  
}
//--------------------------------------------------------------------------------------------------------------------
//
//
CCMenu* CreatGameUIWithSprite( CCSprite* pSprite, SEL_MenuHandler callback, int Id, SelectorProtocol *target, CCPoint pos )
{
	CCSprite *spriteNormal = new CCSprite();
	spriteNormal->initWithTexture(pSprite->getTexture());
	spriteNormal->autorelease();
    
	CCSprite *spriteSelected = new CCSprite();
	spriteSelected->initWithTexture(spriteNormal->getTexture());
	spriteSelected->autorelease();
    
	spriteSelected->setColor(ccc3( 100,100,100 ));
    
	CCMenuItemSprite* item1 = CCMenuItemSprite::itemFromNormalSprite(spriteNormal, spriteSelected, target, callback );
	item1->setTag( Id );
        
	CCMenu* pMenu = CCMenu::menuWithItems( item1, NULL);
	pMenu->setPosition( pos );
	return pMenu;  
}

CCXMLLayer::CCXMLLayer()
{
	
}//------------------------------------------------------
//
//
CCXMLLayer::~CCXMLLayer()
{
	m_vNodeObject.clear();
}
//---------------------------------------------------------------
//
//
void CCXMLLayer::onEnter()
{
	CCLayer::onEnter();
}
//----------------------------------------------------------------
//
//
void CCXMLLayer::onExit()
{
	CCLayer::onExit();
}
//----------------------------------------------------------------
//
//
const char * CCXMLLayer::valueForKey(const char *key, CCDictionary<std::string, CCObject*> *dict)
{
	if (dict)
	{
		CCString *pString = (CCString*)dict->objectForKey(std::string(key));
		return pString ? pString->m_sString.c_str() : "";
	}
	return "";
}
//-------------------------------------------------------------------
//
//
void CCXMLLayer::TurnitorqueWorldToCoco2d( float &x, float &y, float &layer )
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	x += ( s.width * CC_CONTENT_SCALE_FACTOR() ) / 2;
	y += ( s.height * CC_CONTENT_SCALE_FACTOR() ) / 2;

	y = s.height * CC_CONTENT_SCALE_FACTOR() - y;

	layer = MAXTORQUELAYERNUM - layer;
}
//----------------------------------------------------------------
//
//
CCNode* CCXMLLayer::GetXMLNodeFromKey( string key )
{
	for( int i = 0; i < m_vNodeObject.size(); i++ )
	{
		string *strKey = ( string* )m_vNodeObject[i]->getUserData();
		if( 0 == strcmp( strKey->c_str(), key.c_str() ) )
		{
			return m_vNodeObject[i];
		}
	}
	return NULL;
}
//-----------------------------------------------------------------
//
//
void CCXMLLayer::RemoveNode( CCNode* pNode )
{
	for(vector<CCNode*>::iterator it=m_vNodeObject.begin(); it!=m_vNodeObject.end(); it++)
	{
		if( *it== pNode )
		{
			m_vNodeObject.erase( it );
			break;
		}
	}

	removeChild( pNode, true );
}
//-----------------------------------------------------------------
//
//
void CCXMLLayer::LoadPlist( const char *pList )
{
	string strPath = GetGameLevelPath();
	strPath = strPath + pList;

	std::string fullpath(CCFileUtils::fullPathFromRelativePath(strPath.c_str())); 
    
	CCDictionary<std::string, CCObject*> *dict = CCFileUtils::dictionaryWithContentsOfFile(fullpath.c_str());

	CCDictionary<std::string, CCObject*> *imagesDict = (CCDictionary<std::string, CCObject*>*)dict->objectForKey(std::string("images"));

	m_vNodeObject.clear();

	imagesDict->begin();
	std::string key = "";
	CCDictionary<std::string, CCObject*> *imageDict = NULL;
	while( (imageDict = (CCDictionary<std::string, CCObject*>*)imagesDict->next(&key)) )
	{
		float x = (float)atof(valueForKey("x", imageDict));
		float y = (float)atof(valueForKey("y", imageDict));
		float w = (float)atof(valueForKey("width", imageDict));
		float h = (float)atof(valueForKey("height", imageDict));
//        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        if( CCDirector::sharedDirector()->getWinSize().width == 1024 || 2 == CC_CONTENT_SCALE_FACTOR() )
//        {
//            
//        }
//        else
//        {
//            x = x / 2;
//            y = y / 2;
//            w = w / 2;
//            h = h / 2;
//        }
//        #endif    
        
		float layer = (float)atof(valueForKey("layer", imageDict));
		bool flipX = (bool)atoi(valueForKey("FlipX", imageDict));
		bool flipY = (bool)atoi(valueForKey("FlipY", imageDict));
		TurnitorqueWorldToCoco2d( x, y, layer );

		if( strstr( key.c_str(), "t2dSceneObject_") )
		{
			CCNode *pNode = CCNode::node();
			//pNode->setPositionInPixels( ccp( x, y ) );
			pNode->setPosition( ccp( x / CC_CONTENT_SCALE_FACTOR(), y / CC_CONTENT_SCALE_FACTOR() ) );
			CCSize size( w / CC_CONTENT_SCALE_FACTOR(), h / CC_CONTENT_SCALE_FACTOR() );
			pNode->setContentSize( size );
			
			string *pNameKey = new string( key );
			pNode->setUserData( pNameKey );

			addChild( pNode, (int)layer );
			m_vNodeObject.push_back( pNode );
		}
		else if( strstr( key.c_str(), "t2dAnimatedSprite_") )
		{
			AdvanceSprite *m_pAnimation = new AdvanceSprite();
			
			string plist = (valueForKey("plist", imageDict));
			m_pAnimation->addFramesFromiT2D( plist.c_str() );
            m_pAnimation->autorelease();
			//m_pAnimation->setPositionInPixels( ccp( x, y ) );
			m_pAnimation->setPosition( ccp( x / CC_CONTENT_SCALE_FACTOR(), y / CC_CONTENT_SCALE_FACTOR() ) );
			m_pAnimation->setScaleX( ( w / CC_CONTENT_SCALE_FACTOR() ) / m_pAnimation->getContentSize().width );
			m_pAnimation->setScaleY( ( h / CC_CONTENT_SCALE_FACTOR() ) / m_pAnimation->getContentSize().height );
			m_pAnimation->setFlipX( flipX );
			m_pAnimation->setFlipY( flipY );
		    string *pNameKey = new string( key );
			m_pAnimation->setUserData( pNameKey );
			

			int startFrameIndex = (int)atoi(valueForKey("startframe", imageDict));
			int endFrameIndex   = (int)atoi(valueForKey("endframe", imageDict));
			float time = (float)atof(valueForKey("animationtime", imageDict));

			m_pAnimation->startAnimation( startFrameIndex, endFrameIndex, -1, NULL, this, (float)( ( endFrameIndex - startFrameIndex + 1 ) / time ), false, false  );
			addChild( m_pAnimation, (int)layer );

			m_vNodeObject.push_back( m_pAnimation );
		}
		else
		{
			const char *image = valueForKey("image", imageDict);

			string ImagePath = GetGameImagesPath();
			ImagePath = ImagePath + image;

			CCSprite *pSprite = CCSprite::spriteWithFile( ImagePath.c_str() );
			//pSprite->setPositionInPixels( ccp( x, y ) );
			pSprite->setPosition( ccp( x / CC_CONTENT_SCALE_FACTOR(), y / CC_CONTENT_SCALE_FACTOR() ) );
			pSprite->setScaleX( ( w / CC_CONTENT_SCALE_FACTOR() ) / pSprite->getTextureRect().size.width );
			pSprite->setScaleY( ( h / CC_CONTENT_SCALE_FACTOR() ) / pSprite->getTextureRect().size.height );
			pSprite->setFlipX( flipX );
			pSprite->setFlipY( flipY );
		    string *pNameKey = new string( key );
			pSprite->setUserData( pNameKey );
			addChild( pSprite, (int)layer );

			m_vNodeObject.push_back( pSprite );
		}
	}
}

