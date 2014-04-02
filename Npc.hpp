#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include <string>
#include <list>
#include "wx/string.h"
#include "wx/arrstr.h"

#include "AsfDecode.hpp"

struct NpcData
{
    char Name[255];
    char NpcIni[255];
    char FlyIni[255];
    char BodyIni[255];
    char Kind;
    char Relation;
    long Life;
    long LifeMax;
    long Thew;
    long ThewMax;
    long Mana;
    long ManaMax;
    long Attack;
    long Defence;
    long Evade;
    long Exp;
    char WalkSpeed;
    char Dir;
    long Lum;
    char PathFinder;
    char DeathScript[255];
    char ScriptFile[255];
};

void ResetNpc(NpcData *npc);
bool ReadNpcIni(std::string FilePath, NpcData *npc);
std::string FindStandAsf(std::string FilePath);

//return ini file content without head
wxString ReadNpcIni(wxString FilePath);


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
    AsfDecode NpcStand;
};

void InitNpcItem(NpcItem *item);

template<class T>
class ItemList
{
    ItemList(){}
    ~ItemList(){FreeData();}

    void AddItem(T item);
    void DeleteItem(long mapx, long mapy);
    int getCounts(){return m_list.size();}
    bool HasItem(long mapx, long mapy);
private:
    void FreeData();
    std::list<T> m_list;
};

typedef ItemList<NpcItem*> NpcList;

#endif // NPC_HPP_INCLUDED
