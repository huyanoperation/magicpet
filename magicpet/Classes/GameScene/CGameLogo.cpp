#include "CGameLogo.h"
#include "../Sprite/CPet.h"
#include "../loader/CClientLoader.h"
#include "../platform/CNet.h"
#include "../sql/CSqliteManager.h"
#include "CGameMenu.h"
#include "../md5.h"
#include "CGameScene.h"
#include "../Platform/CCNd91Mng.h"
using namespace cocos2d;

int g_iCurLanguageIndex = 0;
CCScene* CGameLogo::scene()
{
    CGameScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = new CGameScene();
        scene->autorelease();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CGameLogo *layer = CGameLogo::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}
// on "init" you need to initialize your instance
bool CGameLogo::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(CGameLogo::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
       // CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
       // pMenu->setPosition(CCPointZero);
       // CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        //this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".

		//string outText("hi");
		//CCLabelTTF* pLabel = CCLabelTTF::labelWithString( outText.c_str(), kFontSystem[0].fontName, 30 );
  //      CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
       // pLabel->setPosition(ccp(size.width / 2, size.height - 20));

        // Add the label to HelloWorld layer as a child layer.
       // this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::spriteWithFile("doformatlogo.png");
        CC_BREAK_IF(! pSprite);
        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        if( CCDirector::sharedDirector()->getWinSize().width == 1024 || 2 == CC_CONTENT_SCALE_FACTOR() )
//        {
//            
//        }
//        else
//        {
//            pSprite->setScale(0.5f);
//        }
//#endif  

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);
        
        bRet = true;
    } while (0);

    return bRet;
}


//------------------------------------------------------------------
//
//
void CGameLogo::onEnter()
{
	CCLayer::onEnter();
    
	schedule(schedule_selector(CGameLogo::nextScreen),2.0f);
	schedule(schedule_selector(CGameLogo::nextScreen2),4.0f);
    
    
   // printf(result.c_str());
//    const char *pszPath = CCFileUtils::fullPathFromRelativePath("shapedefs.plist");
//    
//    CCDictionary<std::string, CCObject*> *dict = CCFileUtils::dictionaryWithContentsOfFile(pszPath);
//    CCDictionary<std::string, CCObject*> *bodies = (CCDictionary<std::string, CCObject*>*)dict->objectForKey(std::string("bodies"));
//    
//    CCDictionary<std::string, CCObject*> *drink = (CCDictionary<std::string, CCObject*>*)bodies->objectForKey( std::string("drink") );
//    
//    std::string key = "";
//    drink->begin();
//    
//    while((CCDictionary<std::string, CCObject*>*)drink->next(&key))
//    {
//        string str("fixtures");
//        if( key == str )
//        {
//            CCObject *pObject= (CCObject*)drink->objectForKey(std::string(key));
//            CCMutableArray< CCObject* >* pArray = ( CCMutableArray< CCObject* >*) pObject;
//            int  c = pArray->count();      
//        }
//    }
//    

}
//------------------------------------------------------------------
//
//
void CGameLogo::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}
//---------------------------------------------------
//
//
void CGameLogo::nextScreen( ccTime dt  )
{
	unschedule(schedule_selector(CGameLogo::nextScreen));

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSprite* pSprite = CCSprite::spriteWithFile("doformatlogo.png");
	pSprite->setPosition(ccp(size.width/2, size.height/2));
	addChild(pSprite, 1);
    
    CNd91Mng::checkNewVersion();
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    if( CCDirector::sharedDirector()->getWinSize().width == 1024 || 2 == CC_CONTENT_SCALE_FACTOR() )
//    {
//        
//    }
//    else
//    {
//        pSprite->setScale(0.5f);
//    }
//#endif  
    
    nextScreen2(0);
	
}
//----------------------------------------------------------
//
//
void CGameLogo::nextScreen2( ccTime dt )
{
	unschedule(schedule_selector(CGameLogo::nextScreen2));

	CGameMenu::Show();

	g_iCurLanguageIndex = CCApplication::sharedApplication().getCurrentLanguage();
    g_iCurLanguageIndex = 1;
    
	g_pClientLoader = new CClientLoader();
	g_pClientLoader->Init();
    
	CSqliteManager::GetInstance()->CreateBaseDataDB();

	CCUserDefault::sharedUserDefault()->setFloatForKey("version", 1.1);
    
    
}

