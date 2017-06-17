#include "MapToolCommand.h"
#include "MapTool.h"
#include "Npc.hpp"
#include "mydefs.hpp"

static MapTool *mt = nullptr;
static NpcList *nl = nullptr;
static ObjList *ol = nullptr;
wxString ep;

void MTC_Init(MapTool* mapTool, wxString exepath, NpcList *npcList, ObjList *objList)
{
	mt = mapTool;
	ep = exepath;
	nl = npcList;
	ol = objList;
}

void MTC_Uninit()
{
	mt = nullptr;
}

MTC_Change_Npc::MTC_Change_Npc(size_t index)
: wxCommand(true, wxT("修改Npc属性"))
, mIndex(index)
{

}

MTC_Change_Npc::~MTC_Change_Npc()
{
	SAFE_DELETE(mOldValue);
	SAFE_DELETE(mNewValue);
}

void MTC_Change_Npc::OldFrom(NpcItem* item)
{
	if(item)
	{
		SAFE_DELETE(mOldValue);
		mOldValue = new NpcItem;
		mOldValue->CopyFrom(item);
	}

}

void MTC_Change_Npc::NewFrom(NpcItem* item)
{
	if(item)
	{
		SAFE_DELETE(mNewValue);
		mNewValue = new NpcItem;
		mNewValue->CopyFrom(item);
	}
}

bool MTC_Change_Npc::Do()
{
	NpcItem *item = nl->GetItem(mIndex);
	item->CopyFrom(mNewValue);
	mt->ReNewNpcAsf(item);
	mt->ShowTile(item->MapX, item->MapY);
	return true;
}

bool MTC_Change_Npc::Undo()
{
	NpcItem *item = nl->GetItem(mIndex);
	item->CopyFrom(mOldValue);
	mt->ReNewNpcAsf(item);
	mt->ShowTile(item->MapX, item->MapY);
	return true;
}

MTC_Move_Npc::MTC_Move_Npc(size_t index)
: wxCommand(true, wxT("移动Npc"))
, mIndex(index)
{

}

MTC_Move_Npc::~MTC_Move_Npc()
{

}

void MTC_Move_Npc::OldPos(int x, int y)
{
	mOldX = x;
	mOldY = y;
}

void MTC_Move_Npc::NewPos(int x, int y)
{
	mNewX = x;
	mNewY = y;
}

bool MTC_Move_Npc::Do()
{
	NpcItem *item = nl->GetItem(mIndex);
	item->MapX = mNewX;
	item->MapY = mNewY;
	mt->CorrectFixedPos(item);
	mt->UpdateListItem(mIndex, MapTool::NPCLIST);
	mt->ShowTile(item->MapX, item->MapY);
	return true;
}

bool MTC_Move_Npc::Undo()
{
	NpcItem *item = nl->GetItem(mIndex);
	item->MapX = mOldX;
	item->MapY = mOldY;
	mt->CorrectFixedPos(item);
	mt->UpdateListItem(mIndex, MapTool::NPCLIST);
	mt->ShowTile(item->MapX, item->MapY);
	return true;
}

bool MTC_Move_Npc::IsNewOldEqual()
{
	return mOldX == mNewX && mOldY == mNewY;
}

MTC_Move_Npcs::MTC_Move_Npcs(wxString name)
 : wxCommand(true, name)
{

}

MTC_Move_Npcs::~MTC_Move_Npcs()
{

}

void MTC_Move_Npcs::Move(size_t index, int oldX, int oldY, int newX, int newY)
{
	mHistory.push_back(History{index, oldX, oldY, newX, newY});
}

bool MTC_Move_Npcs::Do()
{
	size_t i = 1;
	for(auto &history : mHistory)
	{
		NpcItem *item = nl->GetItem(history.Index);
		item->MapX = history.NewX;
		item->MapY = history.NewY;
		mt->CorrectFixedPos(item);
		mt->UpdateListItem(history.Index, MapTool::NPCLIST);
		if (i == mHistory.size())
		{
			mt->ShowTile(item->MapX, item->MapY);
		}
		i++;
	}
	return true;
}

bool MTC_Move_Npcs::Undo()
{
	size_t i = 1;
	for(auto &history : mHistory)
	{
		NpcItem *item = nl->GetItem(history.Index);
		item->MapX = history.OldX;
		item->MapY = history.OldY;
		mt->CorrectFixedPos(item);
		mt->UpdateListItem(history.Index, MapTool::NPCLIST);
		if (i == mHistory.size())
		{
			mt->ShowTile(item->MapX, item->MapY);
		}
		i++;
	}
	return true;
}

MTC_Change_Npcs::MTC_Change_Npcs()
: wxCommand(true, wxT("批量修改Npc属性"))
{

}

MTC_Change_Npcs::~MTC_Change_Npcs()
{

}

void MTC_Change_Npcs::OldFrom(size_t index, NpcItem* item)
{
	if (mHistorys.count(index) == 0)
	{
		History history;
		history.Index = index;
		mHistorys[index] = history;
	}
	mHistorys[index].oldValue.CopyFrom(item);
}

void MTC_Change_Npcs::NewFrom(size_t index, NpcItem* item)
{
	if (mHistorys.count(index) == 0)
	{
		History history;
		history.Index = index;
		mHistorys[index] = history;
	}
	mHistorys[index].newValue.CopyFrom(item);
}

bool MTC_Change_Npcs::Do()
{
	NpcItem *item = nullptr;
	for(auto &history : mHistorys)
	{
		item = nl->GetItem(history.second.Index);
		item->CopyFrom(&history.second.newValue);
		mt->ReNewNpcAsf(item);
	}
	if (item)
	{
		mt->ShowTile(item->MapX, item->MapY);
	}
	return true;
}

bool MTC_Change_Npcs::Undo()
{
	NpcItem *item = nullptr;
	for(auto &history : mHistorys)
	{
		item = nl->GetItem(history.second.Index);
		item->CopyFrom(&history.second.oldValue);
		mt->ReNewNpcAsf(item);
	}
	if (item)
	{
		mt->ShowTile(item->MapX, item->MapY);
	}
	return true;
}

size_t MTC_Change_Npcs::size()
{
	return mHistorys.size();
}

MTC_Add_Npcs::MTC_Add_Npcs(wxString name)
 :wxCommand(true, name)
{

}

MTC_Add_Npcs::~MTC_Add_Npcs()
{

}

void MTC_Add_Npcs::Add(NpcItem* item)
{
	History history;
	history.HasDelete = false;
	history.AddedItem.CopyFrom(item);
	mHistorys.push_back(history);
}

void MTC_Add_Npcs::DeleteAndAdd(size_t deletedIndex, NpcItem *deletedItem, NpcItem* addedItem)
{
	History history;
	history.HasDelete = true;
	history.DeletedIndex = deletedIndex;
	history.DeletedItem.CopyFrom(deletedItem);
	history.AddedItem.CopyFrom(addedItem);
	mHistorys.push_back(history);
}

bool MTC_Add_Npcs::Do()
{
	for(History &it : mHistorys)
	{
		if(it.HasDelete)
		{
			nl->DeleteItem(it.DeletedIndex);
		}
		NpcItem *item = new NpcItem;
		item->CopyFrom(&it.AddedItem);
		nl->AddItem(item);
		mt->ReNewNpcAsf(item);
	}
	mt->RefreshNpcList();
	return true;
}

bool MTC_Add_Npcs::Undo()
{
	for(std::vector<History>::reverse_iterator it = mHistorys.rbegin(); it != mHistorys.rend(); it++)
	{
        nl->DeleteBack();
        if(it->HasDelete)
		{
			NpcItem *item = new NpcItem;
			item->CopyFrom(&it->DeletedItem);
			nl->InsertItem(it->DeletedIndex, item);
			mt->ReNewNpcAsf(item);
		}
	}
	mt->RefreshNpcList();
	return true;
}


MTC_Delete_Npc::MTC_Delete_Npc(size_t index)
: wxCommand(true, wxT("删除Npc"))
{
	mIndex = index;
}

MTC_Delete_Npc::~MTC_Delete_Npc()
{
	SAFE_DELETE(mDeletedItem);
}

void MTC_Delete_Npc::DeletedItem(NpcItem* item)
{
	if(item)
	{
		SAFE_DELETE(mDeletedItem);
		mDeletedItem = new NpcItem;
		mDeletedItem->CopyFrom(item);
	}
}

bool MTC_Delete_Npc::Do()
{
	nl->DeleteItem(mIndex);
	mt->RefreshNpcList();
	mt->ShowTile(mDeletedItem->MapX, mDeletedItem->MapY);
	return true;
}

bool MTC_Delete_Npc::Undo()
{
	NpcItem *item = new NpcItem;
	item->CopyFrom(mDeletedItem);
	nl->InsertItem(mIndex, item);
	mt->ReNewNpcAsf(item);
	mt->RefreshNpcList();
	mt->ShowTile(mDeletedItem->MapX, mDeletedItem->MapY);
	return true;
}

MTC_Delete_Npcs::MTC_Delete_Npcs(wxString name)
 : wxCommand(true, name)
{

}

MTC_Delete_Npcs::~MTC_Delete_Npcs()
{

}

void MTC_Delete_Npcs::DeleteItem(size_t index, NpcItem* item)
{
	History history;
	history.Index = index;
	history.Item.CopyFrom(item);
	mHistorys.push_back(history);
}

bool MTC_Delete_Npcs::Do()
{
	for(auto &it : mHistorys)
	{
		nl->DeleteItem(it.Index);
	}
	mt->RefreshNpcList();
	return true;
}

bool MTC_Delete_Npcs::Undo()
{
	for(auto it = mHistorys.rbegin(); it != mHistorys.rend(); it++)
	{
		NpcItem *item = new NpcItem;
		item->CopyFrom(&it->Item);
		nl->InsertItem(it->Index, item);
		mt->ReNewNpcAsf(item);
	}
	mt->RefreshNpcList();
	return true;
}

MTC_Change_Obj::MTC_Change_Obj(size_t index)
: wxCommand(true, wxT("修改Obj属性"))
, mIndex(index)
{

}

MTC_Change_Obj::~MTC_Change_Obj()
{
	SAFE_DELETE(mOldValue);
	SAFE_DELETE(mNewValue);
}

void MTC_Change_Obj::OldFrom(ObjItem* item)
{
	if(item)
	{
		SAFE_DELETE(mOldValue);
		mOldValue = new ObjItem;
		mOldValue->CopyFrom(item);
	}
}

void MTC_Change_Obj::NewFrom(ObjItem* item)
{
	if(item)
	{
		SAFE_DELETE(mNewValue);
		mNewValue = new ObjItem;
		mNewValue->CopyFrom(item);
	}
}

bool MTC_Change_Obj::Do()
{
	ObjItem *item = ol->GetItem(mIndex);
	item->CopyFrom(mNewValue);
	mt->ReNewObjAsf(item);
	mt->ShowTile(item->MapX, item->MapY);
	return true;
}

bool MTC_Change_Obj::Undo()
{
	ObjItem *item = ol->GetItem(mIndex);
	item->CopyFrom(mOldValue);
	mt->ReNewObjAsf(item);
	mt->ShowTile(item->MapX, item->MapY);
	return true;
}

MTC_Move_Obj::MTC_Move_Obj(size_t index)
: wxCommand(true, wxT("移动Obj"))
, mIndex(index)
{

}

MTC_Move_Obj::~MTC_Move_Obj()
{

}

void MTC_Move_Obj::OldPos(int x, int y)
{
	mOldX = x;
	mOldY = y;
}

void MTC_Move_Obj::NewPos(int x, int y)
{
	mNewX = x;
	mNewY = y;
}

bool MTC_Move_Obj::Do()
{
	ObjItem *item = ol->GetItem(mIndex);
	item->MapX = mNewX;
	item->MapY = mNewY;
	mt->UpdateListItem(mIndex, MapTool::OBJLIST);
	mt->ShowTile(item->MapX, item->MapY);
	return true;
}

bool MTC_Move_Obj::Undo()
{
	ObjItem *item = ol->GetItem(mIndex);
	item->MapX = mOldX;
	item->MapY = mOldY;
	mt->UpdateListItem(mIndex, MapTool::OBJLIST);
	mt->ShowTile(item->MapX, item->MapY);
	return true;
}

bool MTC_Move_Obj::IsNewOldEqual()
{
	return mOldX == mNewX && mOldY == mNewY;
}

MTC_Move_Objs::MTC_Move_Objs(wxString name)
 : wxCommand(true, name)
{

}

MTC_Move_Objs::~MTC_Move_Objs()
{

}

void MTC_Move_Objs::Move(size_t index, int oldX, int oldY, int newX, int newY)
{
	mHistory.push_back(History{index, oldX, oldY, newX, newY});
}

bool MTC_Move_Objs::Do()
{
	size_t i = 1;
	for(auto &history : mHistory)
	{
		ObjItem *item = ol->GetItem(history.Index);
		item->MapX = history.NewX;
		item->MapY = history.NewY;
		mt->UpdateListItem(history.Index, MapTool::OBJLIST);
		if (i == mHistory.size())
		{
			mt->ShowTile(item->MapX, item->MapY);
		}
		i++;
	}
	return true;
}

bool MTC_Move_Objs::Undo()
{
	size_t i = 1;
	for(auto &history : mHistory)
	{
		ObjItem *item = ol->GetItem(history.Index);
		item->MapX = history.OldX;
		item->MapY = history.OldY;
		mt->UpdateListItem(history.Index, MapTool::OBJLIST);
		if (i == mHistory.size())
		{
			mt->ShowTile(item->MapX, item->MapY);
		}
		i++;
	}
	return true;
}


MTC_Change_Objs::MTC_Change_Objs()
: wxCommand(true, wxT("批量修改Obj属性"))
{

}

MTC_Change_Objs::~MTC_Change_Objs()
{

}

void MTC_Change_Objs::OldFrom(size_t index, ObjItem* item)
{
	if (mHistorys.count(index) == 0)
	{
		History history;
		history.Index = index;
		mHistorys[index] = history;
	}
	mHistorys[index].oldValue.CopyFrom(item);
}

void MTC_Change_Objs::NewFrom(size_t index, ObjItem* item)
{
	if (mHistorys.count(index) == 0)
	{
		History history;
		history.Index = index;
		mHistorys[index] = history;
	}
	mHistorys[index].newValue.CopyFrom(item);
}

bool MTC_Change_Objs::Do()
{
	ObjItem *item = nullptr;
	for(auto &history : mHistorys)
	{
		item = ol->GetItem(history.second.Index);
		item->CopyFrom(&history.second.newValue);
		mt->ReNewObjAsf(item);
	}
	if (item)
	{
		mt->ShowTile(item->MapX, item->MapY);
	}
	return true;
}

bool MTC_Change_Objs::Undo()
{
	ObjItem *item = nullptr;
	for(auto &history : mHistorys)
	{
		item = ol->GetItem(history.second.Index);
		item->CopyFrom(&history.second.oldValue);
		mt->ReNewObjAsf(item);
	}
	if (item)
	{
		mt->ShowTile(item->MapX, item->MapY);
	}
	return true;
}

size_t MTC_Change_Objs::size()
{
	return mHistorys.size();
}


MTC_Delete_Obj::MTC_Delete_Obj(size_t index)
 :wxCommand(true, wxT("删除Obj"))
{
	mIndex = index;
}

MTC_Delete_Obj::~MTC_Delete_Obj()
{
	SAFE_DELETE(mDeletedItem);
}

void MTC_Delete_Obj::DeletedItem(ObjItem* item)
{
	if(item)
	{
		SAFE_DELETE(mDeletedItem);
		mDeletedItem = new ObjItem;
		mDeletedItem->CopyFrom(item);
	}
}

bool MTC_Delete_Obj::Do()
{
	ol->DeleteItem(mIndex);
	mt->RefreshObjList();
	mt->ShowTile(mDeletedItem->MapX, mDeletedItem->MapY);
	return true;
}

bool MTC_Delete_Obj::Undo()
{
	ObjItem *item = new ObjItem;
	item->CopyFrom(mDeletedItem);
	ol->InsertItem(mIndex, item);
	mt->ReNewObjAsf(item);
	mt->RefreshObjList();
	mt->ShowTile(mDeletedItem->MapX, mDeletedItem->MapY);
	return true;
}

MTC_Add_Objs::MTC_Add_Objs(wxString name)
 : wxCommand(true, name)
{

}

MTC_Add_Objs::~MTC_Add_Objs()
{

}

void MTC_Add_Objs::Add(ObjItem* item)
{
	History history;
	history.HasDelete = false;
	history.AddedItem.CopyFrom(item);
	mHistorys.push_back(history);
}

void MTC_Add_Objs::DeleteAndAdd(size_t deletedIndex, ObjItem* deletedItem, ObjItem* addedItem)
{
	History history;
	history.HasDelete = true;
	history.DeletedIndex = deletedIndex;
	history.DeletedItem.CopyFrom(deletedItem);
	history.AddedItem.CopyFrom(addedItem);
	mHistorys.push_back(history);
}

bool MTC_Add_Objs::Do()
{
	for(History &it : mHistorys)
	{
		if(it.HasDelete)
		{
			ol->DeleteItem(it.DeletedIndex);
		}
		ObjItem *item = new ObjItem;
		item->CopyFrom(&it.AddedItem);
		ol->AddItem(item);
		mt->ReNewObjAsf(item);
	}
	mt->RefreshObjList();
	return true;
}

bool MTC_Add_Objs::Undo()
{
	for(std::vector<History>::reverse_iterator it = mHistorys.rbegin(); it != mHistorys.rend(); it++)
	{
        ol->DeleteBack();
        if(it->HasDelete)
		{
			ObjItem *item = new ObjItem;
			item->CopyFrom(&it->DeletedItem);
			ol->InsertItem(it->DeletedIndex, item);
			mt->ReNewObjAsf(item);
		}
	}
	mt->RefreshObjList();
	return true;
}

MTC_Delete_Objs::MTC_Delete_Objs(wxString name)
 : wxCommand(true, name)
{

}

MTC_Delete_Objs::~MTC_Delete_Objs()
{

}

void MTC_Delete_Objs::DeleteItem(size_t index, ObjItem* item)
{
	History history;
	history.Index = index;
	history.Item.CopyFrom(item);
	mHistorys.push_back(history);
}

bool MTC_Delete_Objs::Do()
{
	for(auto &it : mHistorys)
	{
		ol->DeleteItem(it.Index);
	}
	mt->RefreshObjList();
	return true;
}

bool MTC_Delete_Objs::Undo()
{
	for(auto it = mHistorys.rbegin(); it != mHistorys.rend(); it++)
	{
		ObjItem *item = new ObjItem;
		item->CopyFrom(&it->Item);
		ol->InsertItem(it->Index, item);
		mt->ReNewObjAsf(item);
	}
	mt->RefreshObjList();
	return true;
}
