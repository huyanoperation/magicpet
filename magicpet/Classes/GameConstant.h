#pragma once

#include "cocos2d.h"
using namespace cocos2d;
#include "CChineseCode.h"
#include "platform/CDevice.h"

//static const char *g_szgamelevelpath = "data/levels/";
//static const char *g_szgameimagespath = "data/images/";


extern int g_iCurLanguageIndex;
extern int g_iGameMode;


static string GetGameLevelPath()
{
	//static const char *g_szgamelevelpath = "data/levels/";
	string str( "data/levels/" );

	return str;
}

static string GetGameImagesPath()
{
	//static const char *g_szgamelevelpath = "data/levels/";
    string str("data/images/");
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    if( CCDirector::sharedDirector()->getWinSize().width == 1024 || 2 == CC_CONTENT_SCALE_FACTOR() )
//    {
//        str = ( "data/images/" );
//    }
//    else
//    {
//         str = ( "data/images_low/" );
//    }
//#endif  
	return str;
}

static string GetGameDayAndTime()
{
	char buffer[128];
    time_t timer; 
    struct tm *t_tm; 
    time(&timer); 
    t_tm = localtime(&timer); 
	sprintf(buffer,"%02d/%02d %02d:%02d", 
           t_tm->tm_mon+1, t_tm->tm_mday, t_tm->tm_hour, t_tm->tm_min );

	string str = buffer;
	return str;
}

static string GetGameParticlePath()
{
	string str( "data/Particle_effects/" );
	return str;
}

static time_t GetCurrentSeconds()
{
	time_t m_currentTime = 0;
	time(&m_currentTime);
	return m_currentTime;
}

static void GetHourMiniSecond( int a, int &h, int &m, int &s )
{
	h =int( a / 3600 );
	m =( a % 3600 ) / 60;
	s =( a % 3600 ) % 60; 
}

enum  {
 
    FONT_TINY,
    FONT_SMALL,
    FONT_THIN,
    FONT_MED,
    FONT_TALL,
    FONT_LARGE,
    FONT_GRAND,
    FONT_NUM,
};

typedef struct 
{
    char fontName[32];
    int  fontSize;
    ccColor3B color;
    int strokeSize;
    ccColor3B strokeColor;
}tTTFFont;


static const ccColor3B ccWhite = {255,255,255};
static const ccColor3B ccBlack = {0,0,0};
static const ccColor3B ccBrown = {117,95,58};
static const ccColor3B ccYellowTitle = {255,247,23};

const tTTFFont kFontSystem[] = {

    {"Verdana-BoldItalic",10,ccWhite,1,ccBlack},
    {"Verdana-BoldItalic",12,ccWhite,1,ccBlack},
    {"Verdana-BoldItalic",14,ccWhite,1,ccBlack},
    {"Verdana-BoldItalic",16,ccWhite,1,ccBlack},
    {"STHeitiJ-Medium",18,ccWhite,1,ccBlack},
    {"STHeitiJ-Medium",20,ccWhite,1,ccBlack},
    {"Verdana-BoldItalic",22,ccYellowTitle,1,ccBlack},
};

static CCRenderTexture* createStroke(CCLabelTTF* label, float size ,ccColor3B cor)
{  
   
    float RealSize = size;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( CCDirector::sharedDirector()->getWinSize().width >= 1024 )
    {
        RealSize = size * 2;
    }
#endif 
    CCRenderTexture* rt = CCRenderTexture::renderTextureWithWidthAndHeight(label->getTexture()->getContentSize().width + RealSize*2, label->getTexture()->getContentSize().height + RealSize*2);
    CCPoint originalPos = label->getPosition();
    ccColor3B originalColor = label->getColor();
    bool originalVisibility = label->getIsVisible();
    label->setColor(cor);
    label->setIsVisible(true);
    ccBlendFunc originalBlend = label->getBlendFunc();
    ccBlendFunc tBlendFunc = {GL_SRC_ALPHA, GL_ONE };
    label->setBlendFunc(tBlendFunc);
    CCPoint bottomLeft = ccp(label->getTexture()->getContentSize().width * label->getAnchorPoint().x + RealSize, label->getTexture()->getContentSize().height * label->getAnchorPoint().y + RealSize);
    CCPoint positionOffset = ccp(label->getTexture()->getContentSize().width * label->getAnchorPoint().x - label->getTexture()->getContentSize().width/2, label->getTexture()->getContentSize().height * label->getAnchorPoint().y - label->getTexture()->getContentSize().height/2);
    CCPoint position = ccpSub(originalPos, positionOffset);
    
    rt->begin();
    for (int i=0; i<360; i+=30)
    {
        label->setPosition(ccp(bottomLeft.x + sin(CC_DEGREES_TO_RADIANS(i))*RealSize, bottomLeft.y + cos(CC_DEGREES_TO_RADIANS(i))*RealSize));
        label->visit(); 
    }
    rt->end();
    
    
    label->setPosition(originalPos);
    label->setColor(originalColor);
    label->setBlendFunc(originalBlend);
    label->setIsVisible(originalVisibility);
    rt->setPosition(position);
    return rt;
}
