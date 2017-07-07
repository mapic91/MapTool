#include "Npc.hpp"

#include <fstream>
#include <cstdlib>
#include <cstring>

#include "wx/msgdlg.h"

#include "MapToolCommand.h"

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

void FindAndBufferAsf(const wxString &exepath, NpcItem *npcitem, ObjItem *objitem, AsfImgList *asflist)
{
	if(npcitem != NULL)
        FindAndBufferAsf(exepath, npcitem->GetString("NpcIni"), wxT("[Stand]"), &(npcitem->NpcStand), asflist);
	if(objitem != NULL)
        FindAndBufferAsf(exepath, objitem->GetString("ObjFile"), wxT("[Common]"), &(objitem->ObjCommon), asflist);
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
    if(asffilename.IsEmpty())
	{
		if(asflist) *asfdec = NULL;
		else (*asfdec)->ReadAsfFile(wxEmptyString);
		return false;
	}
    asfpath += asffilename;

	bool isInList = IsAsfFileIn(asffilename, asflist, asfdec);
    if(asflist != NULL && !isInList)
    {
        AsfImg *asf_img = new AsfImg;
        asf_img->path = asffilename;
        asf_img->asfdec = new AsfDecode;
        asflist->push_back(asf_img);

        *asfdec = asf_img->asfdec;
    }

    if((*asfdec) == NULL) return false;

    if(!isInList)
	{
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
	}
    return true;
}

void NpcItem::CopyFrom(const NpcItem* item)
{
    if(item == NULL) return;
    KeyValues = item->KeyValues;
    NpcStand = item->NpcStand;
}

void ObjItem::CopyFrom(const ObjItem* item)
{
    if(item == NULL) return;
    KeyValues = item->KeyValues;
    ObjCommon = item->ObjCommon;
}

void InitNpcItem(NpcItem *item)
{
    if(!item) return;
    item->KeyValues.clear();
}
void InitObjItem(ObjItem *item)
{
    if(!item) return;
    item->KeyValues.clear();
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
        FindAndBufferAsf(exepath, npcitem->GetString("NpcIni"), wxT("[Stand]"), &(npcitem->NpcStand), list);
    else if(objitem != NULL)
        FindAndBufferAsf(exepath, objitem->GetString("ObjFile"), wxT("[Common]"), &(objitem->ObjCommon), list);

    return true;
}

bool SaveIni(const wxString &filePath, NpcItem *npcitem, ObjItem *objitem, ListDefHelper *listDefHelper)
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
    {
        //FixedPos can't save
        wxString fixPos = npcitem->GetString("FixedPos");
        npcitem->KeyValues["FixedPos"] = wxEmptyString;
        SaveNpcItem(file, npcitem, listDefHelper);
        npcitem->KeyValues["FixedPos"] = fixPos;
    }
    else if(objitem != NULL)
        SaveObjItem(file, objitem, listDefHelper);

    file.Write(wxTextFileType_None, wxConvLibc);
    file.Close();

    return true;
}

void SaveNpcItem(wxTextFile &file, NpcItem *item, ListDefHelper *listDefHelper)
{
	for(auto &kv : item->KeyValues)
	{
		if(kv.second.GetString().IsEmpty()) continue;
		if(listDefHelper == nullptr ||
			 kv.second.GetString() != listDefHelper->GetItemDefaultValue(kv.first).GetString())
		{
			file.AddLine(kv.first + wxT("=") + kv.second.GetString());
		}
	}
}
void SaveObjItem(wxTextFile &file, ObjItem *item, ListDefHelper *listDefHelper)
{
	for(auto &kv : item->KeyValues)
	{
		if(kv.second.GetString().IsEmpty()) continue;
		if(listDefHelper == nullptr ||
			 kv.second.GetString() != listDefHelper->GetItemDefaultValue(kv.first).GetString())
		{
			file.AddLine(kv.first + wxT("=") + kv.second.GetString());
		}
	}
}

void AssignNpcItem(const wxString &name, const wxString &value, long n_value, NpcItem *item)
{
    item->KeyValues[name] = value;
}
void AssignObjItem(const wxString &name, const wxString &value, long n_value, ObjItem *item)
{
    item->KeyValues[name] = value;
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
bool NpcListImport(const wxString &exepath, const wxString &path, NpcList *list, AsfImgList *asflist, wxCommandProcessor *cmdProc)
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
    MTC_Add_Npcs *cmd = new MTC_Add_Npcs(wxT("导入NPC文件"));
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

		long index;
		NpcItem* deletedItem = list->GetItem(item->MapX(), item->MapY(), &index);
		if(deletedItem)
		{
			cmd->DeleteAndAdd(index, deletedItem, item);
		}
		else
		{
			cmd->Add(item);
		}
        list->DeleteItem(item->MapX(), item->MapY());
        list->AddItem(item);
        FindAndBufferAsf(exepath, item->GetString("NpcIni"), wxT("[Stand]"), &(item->NpcStand), asflist);
    }
    cmdProc->Store(cmd);

    return true;
}
bool NpcListSave(const wxString path, const wxString mapName, NpcList *list, ListDefHelper *listDefHelper)
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

        SaveNpcItem(file, item, listDefHelper);

        file.AddLine(wxT(""));
    }

    file.Write(wxTextFileType_None, wxConvLibc);
    file.Close();

    return true;
}

bool ObjListImport(const wxString &exepath, const wxString &path, ObjList *list, AsfImgList *asflist, wxCommandProcessor *cmdProc)
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

    ObjItem *item;
    MTC_Add_Objs *cmd = new MTC_Add_Objs(wxT("导入OBJ文件"));
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

        long index;
        ObjItem *deletedItem = list->GetItem(item->MapX(), item->MapY(), &index);
        if(deletedItem)
		{
			cmd->DeleteAndAdd(index, deletedItem, item);
		}
		else
		{
			cmd->Add(item);
		}
        list->DeleteItem(item->MapX(), item->MapY());
        list->AddItem(item);
        FindAndBufferAsf(exepath, item->GetString("ObjFile"), wxT("[Common]"), &(item->ObjCommon), asflist);
    }
    cmdProc->Store(cmd);

    return true;
}
bool ObjListSave(const wxString path, const wxString mapName, ObjList *list, ListDefHelper *listDefHelper)
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

        SaveObjItem(file, item, listDefHelper);

        file.AddLine(wxT(""));
    }

    file.Write(wxTextFileType_None, wxConvLibc);
    file.Close();

    return true;
}

bool IsAsfFileIn(wxString path, AsfImgList *list, AsfDecode **outasf)
{
    if(list == NULL || path.IsEmpty()) return false;
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
AsfImg* AddImg(AsfImgList &list, const wxString &asfFilePath)
{
    AsfDecode *decode = new AsfDecode;
    if(decode->ReadAsfFile(asfFilePath))
    {
        decode->BufferData();
    }
    AsfImg *asf_img = new AsfImg;
    asf_img->path = wxFileName::FileName(asfFilePath).GetFullName();
    asf_img->asfdec = decode;
    list.push_back(asf_img);
    return asf_img;
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



