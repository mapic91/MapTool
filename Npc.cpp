#include "Npc.hpp"

#include <fstream>
#include <cstdlib>
#include <cstring>

#include "wx/textfile.h"

using namespace std;

void ResetNpc(NpcData *npc)
{
    if(npc == NULL) return;
    memset(npc, 0, sizeof(NpcData));
}

bool ReadNpcIni(std::string FilePath, NpcData *npc)
{
    if(npc == NULL) return false;

    ifstream npcFile(FilePath.c_str());
    if(!npcFile.is_open()) return false;

    ResetNpc(npc);

    string line, name, value;
    size_t pos;
    long num;

    while(!npcFile.eof())
    {
        getline(npcFile, line);
        pos = line.find('=');
        if(pos == string::npos) continue;

        name = line.substr(0, pos);
        value = line.substr(pos + 1);
        num = atoi(value.c_str());

        if(name.compare("Name") == 0)
        {
            strcpy(npc->Name, value.c_str());
        }
        else if(name.compare("NpcIni") == 0)
        {
            strcpy(npc->NpcIni, value.c_str());
        }
        else if(name.compare("FlyIni") == 0)
        {
            strcpy(npc->FlyIni, value.c_str());
        }
        else if(name.compare("BodyIni") == 0)
        {
            strcpy(npc->BodyIni, value.c_str());
        }
        else if(name.compare("Kind") == 0)
        {
            npc->Kind = num;
        }
        else if(name.compare("Relation") == 0)
        {
            npc->Relation = num;
        }
        else if(name.compare("Life") == 0)
        {
            npc->Life = num;
        }
        else if(name.compare("LifeMax") == 0)
        {
            npc->LifeMax = num;
        }
        else if(name.compare("Thew") == 0)
        {
            npc->Thew = num;
        }
        else if(name.compare("ThewMax") == 0)
        {
            npc->ThewMax = num;
        }
        else if(name.compare("Mana") == 0)
        {
            npc->Mana = num;
        }
        else if(name.compare("ManaMax") == 0)
        {
            npc->ManaMax = num;
        }
        else if(name.compare("Attack") == 0)
        {
            npc->Attack = num;
        }
        else if(name.compare("Defence") == 0)
        {
            npc->Defence = num;
        }
        else if(name.compare("Evade") == 0)
        {
            npc->Evade = num;
        }
        else if(name.compare("Exp") == 0)
        {
            npc->Exp = num;
        }
        else if(name.compare("WalkSpeed") == 0)
        {
            npc->WalkSpeed = num;
        }
        else if(name.compare("Dir") == 0)
        {
            npc->Dir = num;
        }
        else if(name.compare("Lum") == 0)
        {
            npc->Lum = num;
        }
        else if(name.compare("PathFinder") == 0)
        {
            npc->PathFinder = (char)num;
        }
        else if(name.compare("DeathScript") == 0)
        {
            strcpy(npc->DeathScript, value.c_str());
        }
        else if(name.compare("ScriptFile") == 0)
        {
            strcpy(npc->ScriptFile, value.c_str());
        }
    }
    return true;
}

string FindStandAsf(string FilePath)
{
    string asfpath;
    ifstream file(FilePath.c_str());
    if(!file.is_open()) return asfpath;

    string line;
    size_t pos;
    while(!file.eof())
    {
        getline(file, line);
        if(line.compare("[Stand]") == 0)
        {
            getline(file, line);
            pos = line.find('=');
            if(pos == string::npos) break;
            asfpath = line.substr(pos + 1);
            break;
        }
    }

    return asfpath;
}

wxString ReadNpcIni(wxString FilePath)
{
    wxString content;
    wxTextFile file;
    if(!file.Open(FilePath)) return content;
    file.GetFirstLine();
    while(!file.Eof())
    {
        content += file.GetLastLine();
    }
    file.Close();

    return content;
}

void InitNpcItem(NpcItem *item)
{
    item->Action = -1;
    item->Attack = -1;
    item->AttackLevel = -1;
    item->AttackRadius = -1;
    item->BodyIni.clear();
    item->DeathScript.clear();
    item->Defend = -1;
    item->DialogRadius = -1;
    item->Dir = -1;
    item->Evade = -1;
    item->Exp = -1;
    item->ExpBonus = -1;
    item->FixedPos.clear();
    item->FlyIni.clear();
    item->FlyIni2.clear();
    item->Idle = -1;
    item->Kind = -1;
    item->Level = -1;
    item->LevelUpExp = -1;
    item->Life = -1;
    item->LifeMax =-1;
    item->Lum = -1;
    item->Mana = -1;
    item->ManaMax = -1;
    item->MapX = -1;
    item->MapY = -1;
    item->Name.clear();
    item->NpcIni.clear();
    item->PathFinder = -1;
    item->Relation = -1;
    item->ScriptFile.clear();
    item->State = -1;
    item->Thew = -1;
    item->ThewMax = -1;
    item->VisionRadius = -1;
    item->WalkSpeed = -1;
}

template<class T>
void ItemList<T>::FreeData()
{
    if(m_list.empty()) return;
    for(typename list<T>::iterator it = m_list.begin();
            it != m_list.end(); ++it)
    {
        delete *it;
    }
    m_list.clear();
}
template<class T>
void ItemList<T>::AddItem(T item)
{
    if(item == NULL) return;
    m_list.push_back(item);
}
template<class T>
void ItemList<T>::DeleteItem(long mapx, long mapy)
{
    for(typename list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it)
    {
        if(*it->MapX == mapx && *it->MapY == mapy)
        {
            delete *it;
            m_list.erase(it);
        }
    }
}
template<class T>
bool ItemList<T>::HasItem(long mapx, long mapy)
{
    for(typename list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it)
    {
        if(*it->MapX == mapx && *it->MapY == mapy)
        {
           return true;
        }
    }
    return false;
}





