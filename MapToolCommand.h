#ifndef MAPTOOLCOMMAND_H
#define MAPTOOLCOMMAND_H

#include "wx/cmdproc.h"
#include "wx/string.h"
#include "Npc.hpp"

#include <map>
#include <vector>

class MapTool;

void MTC_Init(MapTool *mapTool, wxString exepath, NpcList *npcList, ObjList *objList);
void MTC_Uninit();

class MTC_Change_Npc : public wxCommand
{
public:
	MTC_Change_Npc(size_t index);
	~MTC_Change_Npc();
	void OldFrom(NpcItem *item);
	void NewFrom(NpcItem *item);
    virtual bool Do();
    virtual bool Undo();

private:
	size_t mIndex;
	NpcItem *mOldValue = nullptr;
	NpcItem *mNewValue = nullptr;
};

class MTC_Move_Npc : public wxCommand
{
public:
	MTC_Move_Npc(size_t index);
	~MTC_Move_Npc();
	void OldPos(int x, int y);
	void NewPos(int x, int y);
	virtual bool Do();
	virtual bool Undo();

	bool IsNewOldEqual();

private:
	size_t mIndex;
	int mOldX, mOldY;
	int mNewX, mNewY;
};

class MTC_Move_Npcs : public wxCommand
{
public:
	MTC_Move_Npcs(wxString name);
	~MTC_Move_Npcs();
	void Move(size_t index, int oldX, int oldY, int newX, int newY);
	virtual bool Do();
	virtual bool Undo();

private:
	struct History
	{
		size_t Index;
		int OldX;
		int OldY;
		int NewX;
		int NewY;
	};
	std::vector<History> mHistory;
};

class MTC_Change_Npcs : public wxCommand
{
public:
	MTC_Change_Npcs();
	~MTC_Change_Npcs();
	void OldFrom(size_t index, NpcItem *item);
	void NewFrom(size_t index, NpcItem *item);
	virtual bool Do();
	virtual bool Undo();

	size_t size();

private:
	struct History
	{
		size_t Index;
		NpcItem oldValue;
		NpcItem newValue;
	};

	std::map<size_t, History> mHistorys;
};

class MTC_Add_Npcs : public wxCommand
{
public:
	MTC_Add_Npcs(wxString name);
	~MTC_Add_Npcs();
	void Add(NpcItem *item);
	void DeleteAndAdd(size_t deletedIndex, NpcItem *deletedItem, NpcItem *addedItem);
	virtual bool Do();
	virtual bool Undo();

	size_t Count() { return mHistorys.size(); };

private:
	struct History
	{
		bool HasDelete;
		size_t DeletedIndex;
		NpcItem DeletedItem;
		NpcItem AddedItem;
	};

	std::vector<History> mHistorys;
};

class MTC_Delete_Npc : public wxCommand
{
public:
	MTC_Delete_Npc(size_t index);
	~MTC_Delete_Npc();
	void DeletedItem(NpcItem *item);
	virtual bool Do();
	virtual bool Undo();

private:
	size_t mIndex;
	NpcItem *mDeletedItem = nullptr;
};

class MTC_Delete_Npcs : public wxCommand
{
public:
	MTC_Delete_Npcs(wxString name = wxT("批量删除Npc"));
	~MTC_Delete_Npcs();
	void DeleteItem(size_t index, NpcItem *item);
	virtual bool Do();
	virtual bool Undo();

	size_t Count() { return mHistorys.size(); };

private:
	struct History
	{
		size_t Index;
		NpcItem Item;
	};

	std::vector<History> mHistorys;
};

class MTC_Change_Obj : public wxCommand
{
public:
	MTC_Change_Obj(size_t index);
	~MTC_Change_Obj();
	void OldFrom(ObjItem *item);
	void NewFrom(ObjItem *item);
	virtual bool Do();
	virtual bool Undo();

private:
	size_t mIndex;
	ObjItem *mOldValue = nullptr;
	ObjItem *mNewValue = nullptr;
};

class MTC_Move_Obj : public wxCommand
{
public:
	MTC_Move_Obj(size_t index);
	~MTC_Move_Obj();
	void OldPos(int x, int y);
	void NewPos(int x, int y);
	virtual bool Do();
	virtual bool Undo();

	bool IsNewOldEqual();

private:
	size_t mIndex;
	int mOldX, mOldY;
	int mNewX, mNewY;
};

class MTC_Move_Objs : public wxCommand
{
public:
	MTC_Move_Objs(wxString name);
	~MTC_Move_Objs();
	void Move(size_t index, int oldX, int oldY, int newX, int newY);
	virtual bool Do();
	virtual bool Undo();

private:
	struct History
	{
		size_t Index;
		int OldX;
		int OldY;
		int NewX;
		int NewY;
	};
	std::vector<History> mHistory;
};

class MTC_Change_Objs : public wxCommand
{
public:
	MTC_Change_Objs();
	~MTC_Change_Objs();
	void OldFrom(size_t index, ObjItem *item);
	void NewFrom(size_t index, ObjItem *item);
	virtual bool Do();
	virtual bool Undo();

	size_t size();

private:
	struct History
	{
		size_t Index;
		ObjItem oldValue;
		ObjItem newValue;
	};

	std::map<size_t, History> mHistorys;
};

class MTC_Delete_Obj : public wxCommand
{
public:
	MTC_Delete_Obj(size_t index);
	~MTC_Delete_Obj();
	void DeletedItem(ObjItem *item);
	virtual bool Do();
	virtual bool Undo();

private:
	size_t mIndex;
	ObjItem *mDeletedItem = nullptr;
};

class MTC_Add_Objs : public wxCommand
{
public:
	MTC_Add_Objs(wxString name);
	~MTC_Add_Objs();
	void Add(ObjItem *item);
	void DeleteAndAdd(size_t deletedIndex, ObjItem *deletedItem, ObjItem *addedItem);
	virtual bool Do();
	virtual bool Undo();

	size_t Count() { return mHistorys.size(); };

private:
	struct History
	{
		bool HasDelete;
		size_t DeletedIndex;
		ObjItem DeletedItem;
		ObjItem AddedItem;
	};

	std::vector<History> mHistorys;
};

class MTC_Delete_Objs : public wxCommand
{
public:
	MTC_Delete_Objs(wxString name = wxT("批量删除Obj"));
	~MTC_Delete_Objs();
	void DeleteItem(size_t index, ObjItem *item);
	virtual bool Do();
	virtual bool Undo();

	size_t Count() { return mHistorys.size(); };

private:
	struct History
	{
		size_t Index;
		ObjItem Item;
	};

	std::vector<History> mHistorys;
};

#endif // MAPTOOLCOMMAND_H
