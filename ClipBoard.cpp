#include "ClipBoard.h"

ClipBoard::ClipBoard()
{
	Clear();
}

ClipBoard::~ClipBoard()
{
	Clear();
}

void ClipBoard::Clear()
{
	m_isNpcItemSet = false;
	m_isObjItemSet = false;
}

NpcItem* ClipBoard::GetNpcItem()
{
	if(m_isNpcItemSet)
		return &m_npcItem;
	else
		return NULL;
}

void ClipBoard::Copy(const NpcItem* item)
{
	m_isNpcItemSet = (bool)item;
	m_npcItem.CopyFrom(item);
	m_npcItem.FixedPos = wxEmptyString;
}

void ClipBoard::Copy(const ObjItem* item)
{
	m_isObjItemSet = (bool) item;
	m_objItem.CopyFrom(item);
}

ObjItem* ClipBoard::GetObjItem()
{
	if(m_isObjItemSet)
		return &m_objItem;
	else
		return NULL;
}

