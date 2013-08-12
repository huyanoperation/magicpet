//
//  CMiniGameScene
//  magicpet
//
//  Created by jia huyan on 11-11-28.
//  Copyright (c) 2011ƒÍ µ¿∑Êª•∂Ø£®±±æ©£©ø∆ºº”–œﬁ‘»Œπ´Àæ. All rights reserved.
//

#include "CMiniGameCatchLayer.h"
#include "../loader/CClientLoader.h"
#include "../Sprite/CPet.h"
#include "../GameScene/CGameLoading.h"
#include "../CGameText.h"
#include "CNd91Mng_ios.h"
#define PTM_RATIO 32

enum
{
	kTagSpriteManager = 100
};

CMiniGameCatchLayer::CMiniGameCatchLayer()
{
    m_iPetActionState = MINIGAME_PET_MOVE_NONE;
    setIsTouchEnabled( true );
    m_iTime    = 60;
    m_iMaxTime = 60;
	m_bIsFinish = false;
	m_bIsHit = false;
}
//-----------------------------------------------------
//
//
CMiniGameCatchLayer::~CMiniGameCatchLayer()
{
	CC_SAFE_DELETE( world );
}
//-----------------------------------------------------
//
//
void CMiniGameCatchLayer::onEnter()
{
    CNd91Mng::RemoveBanner();
    CNd91Mng::InitadBanner();
    
	CCLayer::onEnter();

	LoadPlist("minigame1_ccessfood.plist");

	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	//UXLOG(L"Screen width %0.2f screen height %0.2f",screenSize.width,screenSize.height);

	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -1.0f);

	// Do we want to let bodies sleep?
	bool doSleep = true;

	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(gravity);
	world->SetAllowSleeping(doSleep);
	world->SetContinuousPhysics(false);

		// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(screenSize.width/2/PTM_RATIO, screenSize.height/2/PTM_RATIO); // bottom-left corner
	//
	//// Call the body factory which allocates memory for the ground body
	//// from a pool and creates the ground box shape (also from a pool).
	//// The body is also added to the world.
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	//// Define the ground box shape.
	b2PolygonShape groundBox;
    // bottom
	//groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, -screenSize.height/2/PTM_RATIO), 0);
 	//groundBody->CreateFixture(&groundBox, 0);
	//
    // top
    //groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, screenSize.height/2/PTM_RATIO), 0);
    //groundBody->CreateFixture(&groundBox, 0);

    // left
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(-screenSize.width/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);

    // right
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(screenSize.width/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
	////Set up sprite
	//
	string path = GetGameImagesPath();
	path = path + "minigame1_icon.png";

	CCSpriteBatchNode *mgr = CCSpriteBatchNode::batchNodeWithFile(path.c_str(), 100);
	addChild(mgr, MINIGAME_LAYER_BEGIN, kTagSpriteManager);
    
	schedule(schedule_selector(CMiniGameCatchLayer::UpDataCatch),0.5f);
    schedule(schedule_selector(CMiniGameCatchLayer::UpData));
    schedule(schedule_selector(CMiniGameCatchLayer::UpDataTime), 1.0f);
    
	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(g_pPetDataBlock->ResId);
    m_pBaseDataAnimation = pAnimationData; 
    
    CCNode *pNode = GetXMLNodeFromKey( "t2dSceneObject_role_position" );
    
    if( NULL != pAnimationData )
    {
        m_pPet = new CPet();
        m_pPet->setPetDataBlock( g_pPetDataBlock );
        m_pPet->autorelease();
        //string path = GetGameImagesPath();
        path = pAnimationData->m_strPlist;
        m_pPet->addFramesFromiT2D( path.c_str(), pAnimationData->m_iPlistType );
        m_pPet->setPosition( pNode->getPosition() );
        m_pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
        addChild(m_pPet, MINIGAME_LAYER_BEGIN);
    }
    scheduleUpdate();

	pNode = GetXMLNodeFromKey( "money" );

	m_plabelName = CCLabelTTF::labelWithString("00000",  kFontSystem[FONT_GRAND].fontName,kFontSystem[FONT_GRAND].fontSize);
	m_plabelName->setPosition(pNode->getPosition());
	m_plabelName->setColor( ccBlack );
	addChild(m_plabelName,1000);

    
    
    CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( "button_back" );
    CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CMiniGameCatchLayer::menuCallback), 0, this, pSprite->getPosition() );
    addChild( pMenu, pSprite->getZOrder());
    removeChild( pSprite, true );
    
    
    m_pTimeSprite= (CCSprite*)GetXMLNodeFromKey( "minigame1_time" );
    m_pTimeSprite->setAnchorPoint( ccp( 0.0f, 0.5f ) );
    
    m_pTimeSprite->setPosition( ccp( m_pTimeSprite->getPosition().x -  m_pTimeSprite->getTextureRect().size.width / 2, m_pTimeSprite->getPosition().y ) );
    m_iTimeBar = m_pTimeSprite->getTextureRect().size.width;
    
    std::string effectpath = GetGameParticlePath();
    std::string filename = effectpath + "Glitter.plist";
    CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
    //m_emitter->setPositionType( kCCPositionTypeRelative );
    pEmitter->setPosition( getPosition() );
    CCPoint pos = getPosition();
    addChild( pEmitter, 100 );
    
	m_ItemSave.clear();
}
//----------------------------------------------------------------------------
//
//
void CMiniGameCatchLayer::AddControlPlayer()
{
//	CCSpriteBatchNode* batch = (CCSpriteBatchNode*)getChildByTag(kTagSpriteManager);
//	
//	//We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
//	//just randomly picking one of the images
//	int index = (int)(CCRANDOM_0_1() * 7);
//    
//    char *FrameName[] = { "batterybig.png",
//                        "batterysmall.png",
//                        "bonestick.png",
//                        "bonestick_big.png",
//                        "brick.png",
//                        "energyball.png",
//                        "greenapple.png",
//                        "redpepper.png"
//    };
//    
//	CCSprite *sprite = CCSprite::spriteWithSpriteFrameName( FrameName[index] );
//	batch->addChild(sprite, MINIGAME_LAYER_BEGIN);
//	
//	//sprite->setPosition( CCPointMake( p.x, p.y) );
//	
//	// Define the dynamic body.
//	//Set up a 1m squared box in the physics world
//	b2BodyDef bodyDef;
//	bodyDef.type = b2_dynamicBody;
//	//bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
//	bodyDef.userData = sprite;
//	b2Body *body = world->CreateBody(&bodyDef);
//	
//	// Define another box shape for our dynamic body.
//	b2PolygonShape dynamicBox;
//	dynamicBox.SetAsBox(32.0f/ PTM_RATIO / 2.0f, 32.0f/ PTM_RATIO / 2.0f);//These are mid points for our 1m box
//	
//	// Define the dynamic body fixture.
//	b2FixtureDef fixtureDef;
//	fixtureDef.shape = &dynamicBox;	
//	fixtureDef.density = 3.0f;
//	fixtureDef.friction = 0.1f;
//	body->CreateFixture(&fixtureDef);
}
//-------------------------------------------------------------------------------
//
//
void CMiniGameCatchLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if( m_bIsHit )
	{
		return;
	}

    CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCPoint touchLocation = touch->locationInView( touch->view() );
	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if( touchLocation.x  < m_pPet->getPosition().x )
    {
         m_iPetActionState = MINIGAME_PET_MOVE_LEFT;
    }
    else if( touchLocation.x > m_pPet->getPosition().x )
    {
         m_iPetActionState = MINIGAME_PET_MOVE_RIGHT;
    }
     m_pPet->PlayAnimation( m_pBaseDataAnimation->m_move, -1, NULL, this, false, false);
   
    
    std::string effectpath = GetGameParticlePath();
    std::string filename = effectpath + "touchDownEffect.plist";
    CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
    pEmitter->setPosition( touchLocation );
    CCPoint pos = getPosition();
    addChild( pEmitter, 100 );
  
}
//-------------------------------------------------------------------------------
//
//
void CMiniGameCatchLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

}
//-------------------------------------------------------------------------------
//
//
void CMiniGameCatchLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if( m_bIsHit )
	{
		return;
	}

    m_iPetActionState = MINIGAME_PET_MOVE_NONE;
    m_pPet->PlayAnimation(m_pBaseDataAnimation->m_stand, -1, NULL, this, false, false);
}
//-----------------------------------------
//
//
void CMiniGameCatchLayer::UpData( ccTime dt )
{
	if( m_bIsHit )
	{
		return;
	}
    
    int pixScale = 1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( CCDirector::sharedDirector()->getWinSize().width >= 1024 )
    {
        pixScale = 2;
    }
#endif

    if( MINIGAME_PET_MOVE_LEFT == m_iPetActionState )
    {
        m_pPet->setPosition( ccp( m_pPet->getPosition().x - 4 * pixScale, m_pPet->getPosition().y ) );
        m_pPet->setFlipX(false);
    }
    else if( MINIGAME_PET_MOVE_RIGHT == m_iPetActionState )
    {
        m_pPet->setPosition( ccp( m_pPet->getPosition().x + 4 * pixScale, m_pPet->getPosition().y ) );
        m_pPet->setFlipX(true);
    }
    
	char buffer[32];
	sprintf( buffer, "%d", g_pPetDataBlock->petmoney );
	m_plabelName->setString( buffer );
}
//-----------------------------
//
//
void CMiniGameCatchLayer::UpDataTime( ccTime dt )
{
    m_iTime--;
    if( m_iTime <= 0 )
    {
        m_iTime = 0;
    }
    int height = m_pTimeSprite->getTextureRect().size.height;
    CCRect rect( 0, 0, ( (float)m_iTime / (float)m_iMaxTime ) * m_iTimeBar, height );
    if( NULL != m_pTimeSprite )
    {
        m_pTimeSprite->setTextureRect( rect );
    }
    
    if( m_iTime <= 0 && !m_bIsFinish)
    {
		m_bIsFinish = true;
		if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/time_up.mp3", false);
		}

		AdvanceSprite *pAdv = new AdvanceSprite();
		pAdv->addFramesFromiT2D( "time_up.plist" );
		pAdv->autorelease();
		addChild( pAdv, 100 );

		int timeup[] = { 0,3,2 };
		pAdv->PlayAnimation( timeup, 1, NULL, this, false, true );
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pAdv->setPosition( ccp( size.width/2, size.height/2 ) );

		schedule( schedule_selector( CMiniGameCatchLayer::Finsh ), 3.0f ); 
    }
    
}
//----------------------------------------------------------------------------
//
//
void CMiniGameCatchLayer::Finsh( ccTime tt)
{
	CGameLoading::Show( LOADING_CGAMEWORLD_SCENE );
}
//-----------------------------------------------------------------------------
//
//
void CMiniGameCatchLayer::UpDataCatch( ccTime dt )
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	AddCartchSprite( ccp( screenSize.width / 2 + screenSize.width / 2 * CCRANDOM_MINUS1_1(), screenSize.height + PTM_RATIO  ) );
}
//------------------------------------------------------------------------------
//
//
void CMiniGameCatchLayer::AddCartchSprite(CCPoint p)
{
	//UXLOG(L"Add sprite %0.2f x %02.f",p.x,p.y);
    
	CCSpriteBatchNode *mgr = (CCSpriteBatchNode*)getChildByTag( kTagSpriteManager );
    
    int index = 1 + (int)(CCRANDOM_0_1() * 11);
    
    int pixScale = 1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( CCDirector::sharedDirector()->getWinSize().width >= 1024 )
    {
        pixScale = 2;
    }
#endif

    CCRect rect( ( index % 4) * 32 * pixScale, index / 4 * 32 * pixScale  ,32 * pixScale, 32 * pixScale );

    
	CCSprite *sprite = CCSprite::spriteWithBatchNode( mgr, rect );
   // addChild(sprite, MINIGAME_LAYER_BEGIN);
	sprite->setPosition( CCPointMake( p.x, p.y) );
	sprite->setTag( index );
	mgr->addChild( sprite );
    //sprite->setAnchorPoint(ccp( 0,0 ));
	
	// Define the dynamic body.
	//Set up a 1m squared box in the physics world
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
	bodyDef.userData = sprite;
	b2Body *body = world->CreateBody(&bodyDef);
	
	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(32.0f/ PTM_RATIO / 2.0f, 32.0f/ PTM_RATIO / 2.0f);//These are mid points for our 1m box
	
	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;	
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);

	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	b2Vec2 d;
    srand( GetCurrentSeconds() );
	d.Set( rand() % 10 * screenSize.width / 10.f, screenSize.height + screenSize.height );

	d.Normalize();
	b2Vec2 F = 80 * d;

	body->ApplyForce(F,d);
    

}
//------------------------------------------------------
//
//
void CMiniGameCatchLayer::update(ccTime dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
	
	int velocityIterations = 8;
	int positionIterations = 1;

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);
	
	//Iterate over the bodies in the physics world
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
            
            if( CCRect::CCRectContainsPoint(CCRect( myActor->getPosition().x - myActor->getTextureRect().size.width / 2, myActor->getPosition().y - myActor->getTextureRect().size.height / 2, myActor->getTextureRect().size.width, myActor->getTextureRect().size.height), m_pPet->getPosition() ) && myActor->getIsVisible() )
            {
                myActor->setIsVisible( false );
                
                CCParticleSystemQuad *m_emitter = new CCParticleSystemQuad();
                std::string effectpath = GetGameParticlePath();
                std::string filename = effectpath + "freezeEffect.plist";
                m_emitter->initWithFile(filename.c_str());
                //m_emitter->setPositionType( kCCPositionTypeRelative );
                m_emitter->setPosition( m_pPet->getPosition() );
                addChild( m_emitter, MINIGAME_LAYER_BEGIN );
                m_emitter->release();
				
				CBaseDataItem *pDataItem= g_pClientLoader->GetBaseDataItemIndex( myActor->getTag() );
				if( NULL != pDataItem )
				{
					
					bool isAdd = false;
                    if( g_pPetDataBlock->recordsave.isNotBeginner[2] )
                    {
                            g_pPetDataBlock->addItem(pDataItem->itemid );
                           for( int j = 0; j < m_ItemSave.size(); j++ )
                           {
                               if( m_ItemSave[j].itemIndex == myActor->getTag() )
                               {
                                   m_ItemSave[j].count++;
                                   isAdd = true;
                                   break;
                               }       
                           }
                           if( !isAdd )
                           {
                               SMiniGameCatchLayer_ItemSave ItemSaveData;
                               ItemSaveData.itemIndex = myActor->getTag();
                               ItemSaveData.count = 1;
                               m_ItemSave.push_back( ItemSaveData );
                           }
                    }
                    else
                    {
                        
                    }
                    
                    int data = ( 1 + rand()%50 );
                    g_pPetDataBlock->addPetExp(data, m_pPet->getPosition() );
					
					if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
					{
						string path = "data/audio/";
						path += pDataItem->itemSound.c_str();
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(path.c_str(), false);
                        
					}
				}
				else if( 10 == myActor->getTag()  )
				{
					int money = 1 + rand()%10;
					g_pPetDataBlock->addPetMoney( money, m_pPet->getPosition() );
				}
				else if( 6 == myActor->getTag() )
				{
					m_pPet->PlayAnimation(m_pBaseDataAnimation->m_injured, 1, callfunc_selector(CMiniGameCatchLayer::HitCallBack) , this, false, false);
					 CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/Hit1.mp3", false);
                     m_iPetActionState = MINIGAME_PET_MOVE_NONE;
					 m_bIsHit = true;
				}
        
            }
            
            if( myActor->getPosition().y <= m_pPet->getPosition().y - m_pPet->getContentSize().width  /  2)
            {
                myActor->setIsVisible(false);
                //myActor->release();
            }
            
		}	
	}

	//world->c
}
//-----------------------------------------------------
//
//
void CMiniGameCatchLayer::HitCallBack()
{
	m_bIsHit = false;
}
//------------------------------------------------------
//
//
void CMiniGameCatchLayer::onExit()
{
	CCLayer::onExit();

	string utf81;
	CChineseCode::GB2312ToUTF_8( utf81, (char*)peteat_1[g_iCurLanguageIndex], 256 );
	
	for( int j = 0; j < m_ItemSave.size(); j ++ )
	{
		char temp[128];
		CBaseDataItem *pDataItem= g_pClientLoader->GetBaseDataItemIndex(m_ItemSave[j].itemIndex );
		sprintf( temp,utf81.c_str(),  m_ItemSave[j].count, pDataItem->itemname.c_str() );
		string rewcord = temp;
		g_pPetDataBlock->addGameMessage( GetGameDayAndTime().c_str(), rewcord.c_str() );
	}
    
    if( !g_pPetDataBlock->recordsave.isNotBeginner[2] )
    {
        g_pPetDataBlock->addItem(7, 10 );
        
        g_pPetDataBlock->recordsave.isNotBeginner[2] = true;
    }

	//g_pPetDataBlock->addGameMessage(  );
}
//-------------------------------------------------------
//
//
void CMiniGameCatchLayer::menuCallback(CCObject * pSender)
{
    CGameLoading::Show( LOADING_CGAMEWORLD_SCENE );
}
//-------------------------------------------------------
//
//