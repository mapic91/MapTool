#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include <string>
#include <list>
#include "wx/string.h"
#include "wx/arrstr.h"
#include "wx/stdpaths.h"
#include "wx/filename.h"

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
    long State;
    long Thew;
    long ThewMax;
    long VisionRadius;
    long WalkSpeed;
    AsfDecode *NpcStand; //Point to AsfImgList item, don't free
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
//delete list item AsfImg*, AsfImg::asfdec, and clear list
void FreeAsfImgList(AsfImgList *list);

// find [stand] asf file in npcres ini file
std::string FindStandAsf(std::string FilePath);
// find [stand] asf and buffer its data
//exepath : the end contain path seprator
//asfdec : out  , get a point of AsfDecode data
//asflist : AsfDecode manager list
bool FindAndBufferStandAsf(const wxString &exepath, const wxString &inifilename, AsfDecode **asfdec, AsfImgList *asflist = NULL);

//return ini file content without head
wxString ReadNpcIni(wxString FilePath);

// init NpcItem to default vaule
void InitNpcItem(NpcItem *item);
// Read a npc ini file and initializing item
//exepath : the end contain path seprator
//list : if not null, npc's asfdata is add to list
bool ReadNpcIni(const wxString &exepath, const wxString &filePath, NpcItem *item, AsfImgList *list = NULL);

template<class T>
class ItemList
{
public:
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
        if(index < 0 || index >= m_list.size()) return T(NULL);
        typename std::list<T>::iterator it = m_list.begin();
        for(int i = 0; i < index; i++, it++);
        return *it;
    }
    T GetItem(long mapx, long mapy)
    {
        for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it)
        {
            if((*it)->MapX == mapx && (*it)->MapY == mapy)
            {
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
            }
        }
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

    void Clear(){FreeData();}

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
bool NpcListSave(const wxString path, const wxString mapName, NpcList *list);

#endif // NPC_HPP_INCLUDED
