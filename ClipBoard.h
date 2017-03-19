#ifndef CLIPBOARD_H
#define CLIPBOARD_H
#include "Npc.hpp"

#include <vector>

#include "wx/gdicmn.h"

class ClipBoard
{
	public:
		ClipBoard();
		virtual ~ClipBoard();

	public:
		void Clear();
		void ClearNpcs();
		void ClearObjs();
		void Add(const NpcItem *item);
		void Add(const ObjItem *item);
		bool HasNpc();
		bool HasObj();
		size_t NpcCount();
		size_t ObjCount();
		std::vector<NpcItem>& GetNpcItems();
		std::vector<ObjItem>& GetObjItems();
		wxPoint GetMinNpcTilePosition();
		wxPoint GetMinObjTilePostion();

	protected:
	private:
		std::vector<NpcItem> m_npcItmes;
		std::vector<ObjItem> m_objItems;
};

#endif // CLIPBOARD_H
