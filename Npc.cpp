#include "Npc.hpp"

#include <fstream>
#include <cstdlib>
#include <cstring>

#include "wx/msgdlg.h"

using namespace std;

string FindAsfInIni(const string FilePath, const std::string match)
{
    string asfpath;
    ifstream file(FilePath.c_str());
    if(!file.is_open()) return asfpath;

    string line;
    size_t pos;
    while(!file.eof())
    {
        getline(file, line);
        if(line.compare(match) == 0)
        {
            getline(file, line);
            pos = line.find('=');
            if(pos == string::npos) break;
            asfpath = line.substr(pos + 1);
            break;
        }
    }

    if(asfpath.empty())
    {
        file.close();
        file.open(FilePath.c_str());
        size_t posequal, posasf;
        while(!file.eof())
        {
            getline(file, line);
            posasf = line.find(".asf");
            posequal = line.find("=");
            if(posasf != string::npos && posequal != string::npos)
            {
                asfpath = line.substr(posequal + 1);
                break;
            }
        }
    }
    file.close();

    return asfpath;
}
bool FindAndBufferAsf(const wxString &exepath,
                      const wxString &inifilename,
                      const wxString &match,
                      AsfDecode **asfdec,
                      AsfImgList *asflist)
{
    if(asfdec == NULL ||
            (*asfdec == NULL && asflist == NULL) //asfdecode missing
      ) return false;

    string IniPath(exepath.char_str());
    wxString asfpath;
    if(match.CmpNoCase(wxT("[Stand]")) == 0)
    {
        IniPath += "ini\\npcres\\";
        asfpath = exepath + wxT("asf\\character\\");
    }
    else if(match.CmpNoCase(wxT("[Common]")) == 0)
    {
        IniPath += "ini\\objres\\";
        asfpath = exepath + wxT("asf\\object\\");
    }
    IniPath += inifilename.char_str();

    wxString asffilename(wxString(FindAsfInIni(IniPath, string(match.char_str())).c_str()));
    asfpath += asffilename;

    if(asflist != NULL && !IsAsfFileIn(asffilename, asflist, asfdec))
    {
        AsfImg *asf_img = new AsfImg;
        asf_img->path = asffilename;
        asf_img->asfdec = new AsfDecode;
        asflist->push_back(asf_img);

        *asfdec = asf_img->asfdec;
    }

    if((*asfdec) == NULL) return false;

    //if fail, try another floder
    if(!(*asfdec)->ReadAsfFile(asfpath))
    {
        if(match.CmpNoCase(wxT("[Stand]")) == 0)//try another path
        {
            asfpath = exepath + wxT("asf\\interlude\\") + asffilename;
            if(!(*asfdec)->ReadAsfFile(asfpath)) return false;
        }
        else return false;
    }
    (*asfdec)->BufferData();
    return true;
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
    item->ShowName.clear();
    item->State = -1;
    item->Thew = -1;
    item->ThewMax = -1;
    item->VisionRadius = -1;
    item->WalkSpeed = -1;
}
void InitObjItem(ObjItem *item)
{
    item->Damage = -1;
    item->Dir = -1;
    item->Frame = -1;
    item->Height = -1;
    item->Kind = -1;
    item->Lum = -1;
    item->MapX = -1;
    item->MapY = -1;
    item->ObjFile.clear();
    item->ObjName.clear();
    item->OffX = -1;
    item->OffY = -1;
    item->ScriptFile.clear();
    item->WavFile.clear();
}

bool ReadIni(const wxString &exepath,
             const wxString &filePath,
             NpcItem *npcitem,
             ObjItem *objitem,
             AsfImgList *list)
{
    if((npcitem == NULL && objitem == NULL)||
            (npcitem != NULL && objitem != NULL)
      ) return false;

    wxTextFile file;
    file.Open(filePath, wxConvLibc);
    if(!file.IsOpened()) return false;

    if(npcitem != NULL)
    {
        InitNpcItem(npcitem);
    }
    else if(objitem != NULL)
    {
        InitObjItem(objitem);
    }

    wxString line, name, value;
    long n_value;
    for(line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine())
    {
        if(GetNameValue(line, name, value, &n_value))
        {
            if(npcitem != NULL)
                AssignNpcItem(name, value, n_value, npcitem);
            else if(objitem != NULL)
                AssignObjItem(name, value, n_value, objitem);
        }

    }
    if(npcitem != NULL)
        FindAndBufferAsf(exepath, npcitem->NpcIni, wxT("[Stand]"), &(npcitem->NpcStand), list);
    else if(objitem != NULL)
        FindAndBufferAsf(exepath, objitem->ObjFile, wxT("[Common]"), &(objitem->ObjCommon), list);

    return true;
}

bool SaveIni(const wxString &filePath, NpcItem *npcitem, ObjItem *objitem)
{
    if(npcitem == NULL && objitem == NULL) return false;

    wxTextFile file;
    file.Create(filePath);
    if(!file.IsOpened())
    {
        file.Open(filePath);
        if(file.IsOpened()) file.Clear();
        else return false;
    }

    file.AddLine(wxT("[INIT]"));

    if(npcitem != NULL)
        SaveNpcItem(file, npcitem);
    else if(objitem != NULL)
        SaveObjItem(file, objitem);

    file.Write(wxTextFileType_None, wxConvLibc);
    file.Close();

    return true;
}

void SaveNpcItem(wxTextFile &file, NpcItem *item)
{
    file.AddLine(wxT("Name=") + item->Name);

    if(!item->ShowName.IsEmpty())
        file.AddLine(wxT("ShowName=") + item->ShowName);

    if(item->Kind != -1)
        file.AddLine(wxT("Kind=") +
                     wxString::Format(wxT("%d"), item->Kind));

    file.AddLine(wxT("NpcIni=") + item->NpcIni);

    if(item->Relation != -1)
        file.AddLine(wxT("Relation=") +
                     wxString::Format(wxT("%d"), item->Relation));

    if(item->PathFinder != -1)
        file.AddLine(wxT("PathFinder=") +
                     wxString::Format(wxT("%d"), item->PathFinder));

    if(item->State != -1)
        file.AddLine(wxT("State=") +
                     wxString::Format(wxT("%d"), item->State));

    if(item->VisionRadius != -1)
        file.AddLine(wxT("VisionRadius=") +
                     wxString::Format(wxT("%d"), item->VisionRadius));

    if(item->DialogRadius != -1)
        file.AddLine(wxT("DialogRadius=") +
                     wxString::Format(wxT("%d"), item->DialogRadius));

    if(item->AttackRadius != -1)
        file.AddLine(wxT("AttackRadius=") +
                     wxString::Format(wxT("%d"), item->AttackRadius));

    if(item->Dir != -1)
        file.AddLine(wxT("Dir=") +
                     wxString::Format(wxT("%d"), item->Dir));

    if(item->MapX != -1)
        file.AddLine(wxT("MapX=") +
                     wxString::Format(wxT("%d"), item->MapX));

    if(item->MapY != -1)
        file.AddLine(wxT("MapY=") +
                     wxString::Format(wxT("%d"), item->MapY));

    if(item->Lum != -1)
        file.AddLine(wxT("Lum=") +
                     wxString::Format(wxT("%d"), item->Lum));

    if(item->Action != -1)
        file.AddLine(wxT("Action=") +
                     wxString::Format(wxT("%d"), item->Action));

    if(item->WalkSpeed != -1)
        file.AddLine(wxT("WalkSpeed=") +
                     wxString::Format(wxT("%d"), item->WalkSpeed));

    if(item->Evade != -1)
        file.AddLine(wxT("Evade=") +
                     wxString::Format(wxT("%d"), item->Evade));

    if(item->Attack != -1)
        file.AddLine(wxT("Attack=") +
                     wxString::Format(wxT("%d"), item->Attack));

    if(item->AttackLevel != -1)
        file.AddLine(wxT("AttackLevel=") +
                     wxString::Format(wxT("%d"), item->AttackLevel));

    if(item->Defend != -1)
        file.AddLine(wxT("Defend=") +
                     wxString::Format(wxT("%d"), item->Defend));

    if(item->Exp != -1)
        file.AddLine(wxT("Exp=") +
                     wxString::Format(wxT("%d"), item->Exp));

    if(item->LevelUpExp != -1)
        file.AddLine(wxT("LevelUpExp=") +
                     wxString::Format(wxT("%d"), item->LevelUpExp));

    if(item->Level != -1)
        file.AddLine(wxT("Level=") +
                     wxString::Format(wxT("%d"), item->Level));

    if(item->Life != -1)
        file.AddLine(wxT("Life=") +
                     wxString::Format(wxT("%d"), item->Life));

    if(item->LifeMax != -1)
        file.AddLine(wxT("LifeMax=") +
                     wxString::Format(wxT("%d"), item->LifeMax));

    if(item->Thew != -1)
        file.AddLine(wxT("Thew=") +
                     wxString::Format(wxT("%d"), item->Thew));

    if(item->ThewMax != -1)
        file.AddLine(wxT("ThewMax=") +
                     wxString::Format(wxT("%d"), item->ThewMax));

    if(item->Mana != -1)
        file.AddLine(wxT("Mana=") +
                     wxString::Format(wxT("%d"), item->Mana));

    if(item->ManaMax != -1)
        file.AddLine(wxT("ManaMax=") +
                     wxString::Format(wxT("%d"), item->ManaMax));

    if(item->ExpBonus != -1)
        file.AddLine(wxT("ExpBonus=") +
                     wxString::Format(wxT("%d"), item->ExpBonus));

    if(item->Idle != -1)
        file.AddLine(wxT("Idle=") +
                     wxString::Format(wxT("%d"), item->Idle));



    if(!item->BodyIni.IsEmpty())
        file.AddLine(wxT("BodyIni=") + item->BodyIni);

    if(!item->FlyIni.IsEmpty())
        file.AddLine(wxT("FlyIni=") + item->FlyIni);

    if(!item->FlyIni2.IsEmpty())
        file.AddLine(wxT("FlyIni2=") + item->FlyIni2);

    file.AddLine(wxT("ScriptFile=") + item->ScriptFile);

    if(!item->DeathScript.IsEmpty())
        file.AddLine(wxT("DeathScript=") + item->DeathScript);

    if(!item->FixedPos.IsEmpty())
        file.AddLine(wxT("FixedPos=") + item->FixedPos);
}
void SaveObjItem(wxTextFile &file, ObjItem *item)
{
    file.AddLine(wxT("ObjName=") + item->ObjName);
    file.AddLine(wxT("ObjFile=") + item->ObjFile);
    file.AddLine(wxT("WavFile=") + item->WavFile);
    file.AddLine(wxT("ScriptFile=") + item->ScriptFile);

    if(item->Kind != -1)
        file.AddLine(wxT("Kind=") +
                     wxString::Format(wxT("%d"), item->Kind));

    if(item->Dir != -1)
        file.AddLine(wxT("Dir=") +
                     wxString::Format(wxT("%d"), item->Dir));

    if(item->Lum != -1)
        file.AddLine(wxT("Lum=") +
                     wxString::Format(wxT("%d"), item->Lum));

    if(item->MapX != -1)
        file.AddLine(wxT("MapX=") +
                     wxString::Format(wxT("%d"), item->MapX));

    if(item->MapY != -1)
        file.AddLine(wxT("MapY=") +
                     wxString::Format(wxT("%d"), item->MapY));

    if(item->OffX != -1)
        file.AddLine(wxT("OffX=") +
                     wxString::Format(wxT("%d"), item->OffX));

    if(item->OffY != -1)
        file.AddLine(wxT("OffY=") +
                     wxString::Format(wxT("%d"), item->OffY));

    if(item->Height != -1)
        file.AddLine(wxT("Height=") +
                     wxString::Format(wxT("%d"), item->Height));


    if(item->Damage != -1)
        file.AddLine(wxT("Damage=") +
                     wxString::Format(wxT("%d"), item->Damage));

    if(item->Frame != -1)
        file.AddLine(wxT("Frame=") +
                     wxString::Format(wxT("%d"), item->Frame));
}

void AssignNpcItem(const wxString &name, const wxString &value, long n_value, NpcItem *item)
{
    if(name.CmpNoCase(wxT("Action")) == 0)
        item->Action = n_value;
    else if(name.CmpNoCase(wxT("Attack")) == 0)
        item->Attack = n_value;
    else if(name.CmpNoCase(wxT("AttackLevel")) == 0)
        item->AttackLevel = n_value;
    else if(name.CmpNoCase(wxT("AttackRadius")) == 0)
        item->AttackRadius = n_value;
    else if(name.CmpNoCase(wxT("BodyIni")) == 0)
        item->BodyIni = value;
    else if(name.CmpNoCase(wxT("DeathScript")) == 0)
        item->DeathScript = value;
    else if(name.CmpNoCase(wxT("Defend")) == 0 ||
            name.CmpNoCase(wxT("Defence")) == 0)
        item->Defend = n_value;
    else if(name.CmpNoCase(wxT("DialogRadius")) == 0)
        item->DialogRadius = n_value;
    else if(name.CmpNoCase(wxT("Dir")) == 0)
        item->Dir = n_value;
    else if(name.CmpNoCase(wxT("Evade")) == 0)
        item->Evade = n_value;
    else if(name.CmpNoCase(wxT("Exp")) == 0)
        item->Exp = n_value;
    else if(name.CmpNoCase(wxT("ExpBonus")) == 0)
        item->ExpBonus = n_value;
    else if(name.CmpNoCase(wxT("FixedPos")) == 0)
        item->FixedPos = value;
    else if(name.CmpNoCase(wxT("FlyIni")) == 0)
        item->FlyIni = value;
    else if(name.CmpNoCase(wxT("FlyIni2")) == 0)
        item->FlyIni2 = value;
    else if(name.CmpNoCase(wxT("Idle")) == 0)
        item->Idle = n_value;
    else if(name.CmpNoCase(wxT("Kind")) == 0)
        item->Kind = n_value;
    else if(name.CmpNoCase(wxT("Level")) == 0)
        item->Level = n_value;
    else if(name.CmpNoCase(wxT("LevelUpExp")) == 0)
        item->LevelUpExp = n_value;
    else if(name.CmpNoCase(wxT("Life")) == 0)
        item->Life = n_value;
    else if(name.CmpNoCase(wxT("LifeMax")) == 0)
        item->LifeMax = n_value;
    else if(name.CmpNoCase(wxT("Lum")) == 0)
        item->Lum = n_value;
    else if(name.CmpNoCase(wxT("Mana")) == 0)
        item->Mana = n_value;
    else if(name.CmpNoCase(wxT("ManaMax")) == 0)
        item->ManaMax = n_value;
    else if(name.CmpNoCase(wxT("MapX")) == 0)
        item->MapX = n_value;
    else if(name.CmpNoCase(wxT("MapY")) == 0)
        item->MapY = n_value;
    else if(name.CmpNoCase(wxT("Name")) == 0)
        item->Name = value;
    else if(name.CmpNoCase(wxT("NpcIni")) == 0)
        item->NpcIni = value;
    else if(name.CmpNoCase(wxT("PathFinder")) == 0)
        item->PathFinder = n_value;
    else if(name.CmpNoCase(wxT("Relation")) == 0)
        item->Relation = n_value;
    else if(name.CmpNoCase(wxT("ScriptFile")) == 0)
        item->ScriptFile = value;
    else if(name.CmpNoCase(wxT("ShowName")) == 0)
        item->ShowName = value;
    else if(name.CmpNoCase(wxT("State")) == 0)
        item->State = n_value;
    else if(name.CmpNoCase(wxT("Thew")) == 0)
        item->Thew = n_value;
    else if(name.CmpNoCase(wxT("ThewMax")) == 0)
        item->ThewMax = n_value;
    else if(name.CmpNoCase(wxT("VisionRadius")) == 0)
        item->VisionRadius = n_value;
    else if(name.CmpNoCase(wxT("WalkSpeed")) == 0)
        item->WalkSpeed = n_value;
//        else if(name.CmpNoCase(wxT("")) == 0)
//            item-> = n_value;
}
void AssignObjItem(const wxString &name, const wxString &value, long n_value, ObjItem *item)
{
    if(name.CmpNoCase(wxT("Damage")) == 0)
        item->Damage = n_value;
    if(name.CmpNoCase(wxT("Dir")) == 0)
        item->Dir = n_value;
    if(name.CmpNoCase(wxT("Frame")) == 0)
        item->Frame = n_value;
    if(name.CmpNoCase(wxT("Height")) == 0)
        item->Height = n_value;
    if(name.CmpNoCase(wxT("Kind")) == 0)
        item->Kind = n_value;
    if(name.CmpNoCase(wxT("Lum")) == 0)
        item->Lum = n_value;
    if(name.CmpNoCase(wxT("MapX")) == 0)
        item->MapX = n_value;
    if(name.CmpNoCase(wxT("MapY")) == 0)
        item->MapY = n_value;
    if(name.CmpNoCase(wxT("ObjFile")) == 0)
        item->ObjFile = value;
    if(name.CmpNoCase(wxT("ObjName")) == 0)
        item->ObjName = value;
    if(name.CmpNoCase(wxT("OffX")) == 0)
        item->OffX = n_value;
    if(name.CmpNoCase(wxT("OffY")) == 0)
        item->OffY = n_value;
    if(name.CmpNoCase(wxT("ScriptFile")) == 0)
        item->ScriptFile = value;
    if(name.CmpNoCase(wxT("WavFile")) == 0)
        item->WavFile = value;
}

bool GetNameValue(const wxString &line, wxString &name, wxString &value, long *n_value)
{
    int pos;
    pos = line.find(wxT("="));
    if(pos == wxNOT_FOUND) return false;
    name = line.Mid(0, pos);
    value = line.Mid(pos + 1);
    if(!value.ToLong(n_value)) (*n_value) = -1;
    return true;
}
bool NpcListImport(const wxString &exepath, const wxString &path, NpcList *list, AsfImgList *asflist)
{
    if(list == NULL ||
            asflist == NULL) return false;

    wxTextFile file;
    file.Open(path, wxConvLibc);
    if(!file.IsOpened()) return false;

    wxString line, name, value;
    long n_value;
    long counts = 0;
    for(line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine())
    {
        if(GetNameValue(line, name, value, &n_value))
        {
            if(name.CmpNoCase(wxT("Count")) == 0 && n_value != -1)
            {
                counts = n_value;
                break;
            }
        }
    }

    NpcItem *item;;
    for(long i = 0; i < counts && !file.Eof(); i++, line = file.GetNextLine())
    {
        while(!file.Eof())
        {
            if(line.CmpNoCase(wxString::Format(wxT("[NPC%03d]"), i)) != 0)
                line = file.GetNextLine();
            else break;
        }

        item = new NpcItem;
        InitNpcItem(item);

        for(line = file.GetNextLine();
                !file.Eof() && GetNameValue(line, name, value, &n_value);
                line = file.GetNextLine())
        {
            AssignNpcItem(name, value, n_value, item);
        }

        list->DeleteItem(item->MapX, item->MapY);
        list->AddItem(item);
        FindAndBufferAsf(exepath, item->NpcIni, wxT("[Stand]"), &(item->NpcStand), asflist);
    }

    return true;
}
bool NpcListSave(const wxString path, const wxString mapName, NpcList *list)
{
    if(list == NULL) return false;

    wxTextFile file;
    file.Create(path);
    if(!file.IsOpened())
    {
        file.Open(path);
        if(file.IsOpened()) file.Clear();
        else return false;
    }

    int counts = list->getCounts();
    file.AddLine(wxT("[Head]"));
    file.AddLine(wxT("Map=") + mapName);
    file.AddLine(wxString::Format(wxT("Count=%d"), counts));
    file.AddLine(wxT(""));

    NpcItem *item;
    for(int i = 0; i < counts; i++)
    {
        item = list->GetItem(i);
        if(item == NULL) return false;
        file.AddLine(wxString::Format(wxT("[NPC%03d]"), i));

        SaveNpcItem(file, item);

        file.AddLine(wxT(""));
    }

    file.Write(wxTextFileType_None, wxConvLibc);
    file.Close();

    return true;
}

bool ObjListImport(const wxString &exepath, const wxString &path, ObjList *list, AsfImgList *asflist)
{
    if(list == NULL ||
            asflist == NULL) return false;

    wxTextFile file;
    file.Open(path, wxConvLibc);
    if(!file.IsOpened()) return false;

    wxString line, name, value;
    long n_value;
    long counts = 0;
    for(line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine())
    {
        if(GetNameValue(line, name, value, &n_value))
        {
            if(name.CmpNoCase(wxT("Count")) == 0 && n_value != -1)
            {
                counts = n_value;
                break;
            }
        }
    }

    ObjItem *item;;
    for(long i = 0; i < counts && !file.Eof(); i++, line = file.GetNextLine())
    {
        while(!file.Eof())
        {
            if(line.CmpNoCase(wxString::Format(wxT("[OBJ%03d]"), i)) != 0)
                line = file.GetNextLine();
            else break;
        }

        item = new ObjItem;
        InitObjItem(item);

        for(line = file.GetNextLine();
                !file.Eof() && GetNameValue(line, name, value, &n_value);
                line = file.GetNextLine())
        {
            AssignObjItem(name, value, n_value, item);
        }

        list->DeleteItem(item->MapX, item->MapY);
        list->AddItem(item);
        FindAndBufferAsf(exepath, item->ObjFile, wxT("[Common]"), &(item->ObjCommon), asflist);
    }

    return true;
}
bool ObjListSave(const wxString path, const wxString mapName, ObjList *list)
{
    if(list == NULL) return false;

    wxTextFile file;
    file.Create(path);
    if(!file.IsOpened())
    {
        file.Open(path);
        if(file.IsOpened()) file.Clear();
        else return false;
    }

    int counts = list->getCounts();
    file.AddLine(wxT("[Head]"));
    file.AddLine(wxT("Map=") + mapName);
    file.AddLine(wxString::Format(wxT("Count=%d"), counts));
    file.AddLine(wxT(""));

    ObjItem *item;
    for(int i = 0; i < counts; i++)
    {
        item = list->GetItem(i);
        if(item == NULL) return false;
        file.AddLine(wxString::Format(wxT("[OBJ%03d]"), i));

        SaveObjItem(file, item);

        file.AddLine(wxT(""));
    }

    file.Write(wxTextFileType_None, wxConvLibc);
    file.Close();

    return true;
}

bool IsAsfFileIn(wxString path, AsfImgList *list, AsfDecode **outasf)
{
    if(list == NULL) return false;
    for(AsfImgListIterator it = list->begin(); it != list->end(); it++)
    {
        if(path.CmpNoCase((*it)->path) == 0)
        {
            (*outasf) = (*it)->asfdec;
            return true;
        }
    }
    return false;
}
void FreeAsfImgList(AsfImgList *list)
{
    if(list == NULL) return;
    for(AsfImgListIterator it = list->begin(); it != list->end(); it++)
    {
        if((*it)->asfdec != NULL) delete (*it)->asfdec;
        if((*it) != NULL) delete (*it);
    }
    list->clear();
    list = NULL;
}



