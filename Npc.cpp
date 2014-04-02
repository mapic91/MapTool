#include "Npc.hpp"

#include <fstream>
#include <cstdlib>
#include <cstring>

#include "wx/textfile.h"
#include "wx/msgdlg.h"

using namespace std;

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
bool FindAndBufferStandAsf(const wxString &exepath,const wxString &inifilename,AsfDecode *asfdec)
{
    string NpcIniPath(exepath.char_str());
    NpcIniPath += "ini\\npcres\\";
    NpcIniPath += inifilename.char_str();

    wxString asffilename(wxString(FindStandAsf(NpcIniPath).c_str()));
    wxString asfpath(exepath +
                     wxT("asf\\character\\") +
                     asffilename);

    //if fail, try another floder
    if(!asfdec->ReadAsfFile(asfpath))
    {
        asfpath = exepath + wxT("asf\\interlude\\") + asffilename;
        if(!asfdec->ReadAsfFile(asfpath)) return false;
    }
    asfdec->BufferData();
    return true;
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

bool ReadNpcIni(const wxString &exepath, const wxString &filePath, NpcItem *item)
{
    if(item == NULL) return false;

    wxTextFile file;
    file.Open(filePath, wxConvLibc);
    if(!file.IsOpened()) return false;

    InitNpcItem(item);

    wxString line, name, value;
    int pos;
    long n_value;
    for(line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine())
    {
        pos = line.find(wxT("="));
        if(pos == wxNOT_FOUND) continue;
        name = line.Mid(0, pos);
        value = line.Mid(pos + 1);
        if(!value.ToLong(&n_value)) n_value = -1;

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

    FindAndBufferStandAsf(exepath, item->NpcIni, &(item->NpcStand));

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

        file.AddLine(wxT("Name=") + item->Name);

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

        file.AddLine(wxT(""));
    }

    file.Write(wxTextFileType_None, wxConvLibc);
    file.Close();

    return true;
}





