#pragma once

#include "cocos2d.h"

class CBaseDataItem
{

public:
	CBaseDataItem();
	~CBaseDataItem();

	int		itemid;	string	itemname;	string	itemicon;	string  itemSound;	int		itemindex;		int		itemkind;		float	itemcd;		int		itemtype;		int		itemexp;		int		itemskillexp;		int		itemmoney;		int		itemlimit;		int		itemfunction;
};