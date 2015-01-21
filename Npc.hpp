#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include <string>
#include <list>
#include "wx/string.h"
#include "wx/arrstr.h"
#include "wx/stdpaths.h"
#include "wx/filename.h"
#include "wx/textfile.h"

#include "AsfDecode.hpp"

struct NpcItem
{
    long Action;
    long Attack;
    long AttackLevel;
    long AttackRadius;
    wxString BodyIni;
    wxString DeathScript;
    long Defend;
    long DialogRadius;
    long Dir;
    long Evade;
    long Exp;
    long ExpBonus;
    wxString FixedPos;
    wxString FlyIni;
    wxString FlyIni2;
    long Idle;
    long Kind;
    long Level;
    long LevelUpExp;
    long Life;
    long LifeMax;
    long Lum;
    long Mana;
    long ManaMax;
    long MapX;
    long MapY;
    wxString Name;
    wxString NpcIni;
    long PathFinder;
    long Relation;
    wxString ScriptFile;
    wxString ShowName;
    long State;
    long Thew;
    long ThewMax;
    long VisionRadius;
    long WalkSpeed;
    AsfDecode *NpcStand; //Point to AsfImgList item, don't free

	NpcItem() {NpcStand = NULL;}
    void CopyFrom(const NpcItem *item);
};
struct ObjItem
{
    long Damage;
    long Dir;
    long Frame;
    long Height;
    long Kind;
    long Lum;
    long MapX;
    long MapY;
    wxString ObjFile;
    wxString ObjName;
    long OffX;
    long OffY;
    wxString ScriptFile;
    wxString WavFile;
    AsfDecode *ObjCommon; //Point to AsfObjImgList item, don't free

	ObjItem(){ObjCommon = NULL;}
    void CopyFrom(const ObjItem *item);

    int GetOffX() { return OffX == -1 ? 0 : OffX; }
    int GetOffY() { return OffY == -1 ? 0 : OffY; }
};
struct AsfImg
{
    wxString path;
    AsfDecode *asfdec;
};

typedef std::list<AsfImg*> AsfImgList;
typedef std::list<AsfImg*>::iterator AsfImgListIterator;
// if in , outasf is the asf data
bool IsAsfFileIn(wxString path, AsfImgList *list, AsfDecode **outasf);
// Add img to list
AsfImg* AddImg(AsfImgList &list, const wxString &asfFilePath);
//delete list item AsfImg*, AsfImg::asfdec, and clear list
void FreeAsfImgList(AsfImgList *list);

// find [stand] asf file in npcres ini file
std::string FindAsfInIni(const std::string FilePath, const std::string match);
// find [stand] asf and buffer its data
//exepath : the end contain path seprator
//asfdec : out  , get a point of AsfDecode data
//asflist : AsfDecode manager list
bool FindAndBufferAsf(const wxString &exepath,
                      const wxString &inifilename,
                      const wxString &match,
                      AsfDecode **asfdec,
                      AsfImgList *asflist = NULL);

// init NpcItem to default vaule
void InitNpcItem(NpcItem *item);
void InitObjItem(ObjItem *item);
// Read a npc ini file and initializing item
//exepath : the end contain path seprator
//list : if not null, npc's asfdata is add to list
bool ReadIni(const wxString &exepath,
             const wxString &filePath,
             NpcItem *npcitem = NULL,
             ObjItem *objitem = NULL,
             AsfImgList *list = NULL);
bool SaveIni(const wxString &filePath, NpcItem *npcitem = NULL, ObjItem *objitem = NULL);
void SaveNpcItem(wxTextFile &file, NpcItem *item);
void SaveObjItem(wxTextFile &file, ObjItem *item);
void AssignNpcItem(const wxString &name, const wxString &value, long n_value, NpcItem *item);
void AssignObjItem(const wxString &name, const wxString &value, long n_value, ObjItem *item);
bool GetNameValue(const wxString &line, wxString &name, wxString &value, long *n_value);

template<class T>
class ItemList
{
public:
	typedef typename std::list<T>::iterator iterator;
    ItemList()
    {
        exepath = wxStandardPaths::Get().GetExecutablePath();
        exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    }
    ~ItemList()
    {
        FreeData();
    }

    T GetItem(int index)
    {
        if(index < 0 || index >= (int)m_list.size()) return T(NULL);
        typename std::list<T>::iterator it = m_list.begin();
        for(int i = 0; i < index; i++, it++);
        return *it;
    }

	//Returned array must deleted use delete[]
    T* GetAllItem()
    {
    	size_t num = m_list.size();
    	T *items = new T[num + 1];
    	items[num] = NULL;
    	long pos = 0;
    	for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it)
		{
			items[pos++] = *it;
		}
		return items;
    }

    T GetItem(long mapx, long mapy, long *index = NULL)
    {
        long pos = 0;
        for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it, pos++)
        {
            if((*it)->MapX == mapx && (*it)->MapY == mapy)
            {
                if(index != NULL) (*index) = pos;
                return *it;
            }
        }
        return T(NULL);
    }

    void AddItem(T item)
    {
        if(item == NULL) return;
        m_list.push_back(item);
    }

    void DeleteItem(long mapx, long mapy)
    {
        for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it)
        {
            if((*it)->MapX == mapx && (*it)->MapY == mapy)
            {
                delete *it;
                m_list.erase(it);
                return;
            }
        }
    }
    void DeleteItem(int index)
    {
    	if(index < 0 || index >= (int)m_list.size()) return;
        typename std::list<T>::iterator it = m_list.begin();
        for(int i = 0; i < index; i++, it++);
    	delete *it;
		m_list.erase(it);
    }

    int getCounts()
    {
        return m_list.size();
    }

    bool HasItem(long mapx, long mapy)
    {
        for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it)
        {
            if((*it)->MapX == mapx && (*it)->MapY == mapy)
            {
                return true;
            }
        }
        return false;
    }

    iterator begin()
    {
    	return m_list.begin();
    }

    iterator end()
    {
    	return m_list.end();
    }

    void Clear()
    {
        FreeData();
    }

private:
    void FreeData()
    {
        if(m_list.empty()) return;
        for(typename std::list<T>::iterator it = m_list.begin();
                it != m_list.end(); ++it)
        {
            delete *it;
        }
        m_list.clear();
    }


    std::list<T> m_list;
    wxString exepath;
};

typedef ItemList<NpcItem*> NpcList;
bool NpcListImport(const wxString &exepath, const wxString &path, NpcList *list, AsfImgList *asflist);
bool NpcListSave(const wxString path, const wxString mapName, NpcList *list);

typedef ItemList<ObjItem*> ObjList;
bool ObjListImport(const wxString &exepath, const wxString &path, ObjList *list, AsfImgList *asflist);
bool ObjListSave(const wxString path, const wxString mapName, ObjList *list);

#endif // NPC_HPP_INCLUDED
