#include "ClipBoard.h"

ClipBoard::ClipBoard()
{
}

ClipBoard::~ClipBoard()
{
	Clear();
}

void ClipBoard::Clear()
{
	m_npcItmes.clear();
	m_objItems.clear();
}

void ClipBoard::ClearNpcs()
{
	m_npcItmes.clear();
}

void ClipBoard::ClearObjs()
{
	m_objItems.clear();
}


void ClipBoard::Add(const NpcItem* item)
{
	if(item == nullptr) return;
	NpcItem ni;
	ni.CopyFrom(item);
	ni.NpcStand = nullptr;
	ni.FixedPos = wxEmptyString;
	m_npcItmes.push_back(ni);
}

void ClipBoard::Add(const ObjItem* item)
{
	if(item == nullptr) return;
	ObjItem oi;
	oi.CopyFrom(item);
	oi.ObjCommon = nullptr;
	m_objItems.push_back(oi);
}

bool ClipBoard::HasNpc()
{
	return m_npcItmes.size() > 0;
}

bool ClipBoard::HasObj()
{
	return m_objItems.size() > 0;
}

size_t ClipBoard::NpcCount()
{
	return m_npcItmes.size();
}

size_t ClipBoard::ObjCount()
{
	return m_objItems.size();
}

std::vector<NpcItem>&  ClipBoard::GetNpcItems()
{
	return m_npcItmes;
}

std::vector<ObjItem>& ClipBoard::GetObjItems()
{
	return m_objItems;
}

wxPoint ClipBoard::GetMinNpcTilePosition()
{
	int x = 0, y = 0;
	if(m_npcItmes.size() > 0)
	{
		x = m_npcItmes.front().MapX;
		y = m_npcItmes.front().MapY;
		for(auto it = m_npcItmes.begin(); it != m_npcItmes.end(); it++)
		{
			if((it->MapX - x) + (it->MapY - y) < 0)
			{
				x = it->MapX;
				y = it->MapY;
			}
		}
	}

	return wxPoint(x, y);
}

wxPoint ClipBoard::GetMinObjTilePostion()
{
	int x = 0, y = 0;
	if(m_objItems.size() > 0)
	{
		x = m_objItems.front().MapX;
		y = m_objItems.front().MapY;
		for(auto it = m_objItems.begin(); it != m_objItems.end(); it++)
		{
			if((it->MapX - x) + (it->MapY - y) < 0)
			{
				x = it->MapX;
				y = it->MapY;
			}
		}
	}
	return wxPoint(x, y);
}

