#ifndef CLIPBOARD_H
#define CLIPBOARD_H
#include "Npc.hpp"

class ClipBoard
{
	public:
		ClipBoard();
		virtual ~ClipBoard();

	public:
		void Clear();
		void Copy(const NpcItem *item);
		void Copy(const ObjItem *item);
		NpcItem* GetNpcItem();
		ObjItem* GetObjItem();

	protected:
	private:
		NpcItem m_npcItem;
		ObjItem m_objItem;
		bool m_isNpcItemSet;
		bool m_isObjItemSet;
};

#endif // CLIPBOARD_H
