#include "ListDefHelper.h"

#include "wx/msgdlg.h"
#include "wx/stdpaths.h"

#include "MapToolPropertys.hpp"
#include "Utils.h"

#include <fstream>

static wxString towxStr(const json& jstr)
{
    return wxString::FromUTF8(jstr.get<std::string>().c_str());
}

ListDefHelper::ListDefHelper()
{

}

bool ListDefHelper::Read(const wxString& jsonFilePath)
{
    std::ifstream jf(jsonFilePath.char_str());
    if(jf.is_open())
    {
        try
        {
            m_json = json::parse(jf);
        }
        catch(std::exception& e)
        {
            wxMessageBox(wxString::Format(wxT("文件格式错误：%s。错误：%s"), jsonFilePath, wxString(e.what())));
            return false;
        }

        return true;
    }
    return false;
}

void ListDefHelper::GetItem(const wxString& itemName, const std::function<void(const json&)> &callbackOnItem)
{
    if(m_json.count("Order") == 0) return;
    for(auto &categoryName : m_json["Order"])
    {
        auto categroy = m_json[categoryName.get<std::string>().c_str()];
        for(auto &item : categroy["Items"])
        {
            if(towxStr(item["Name"]) == itemName)
            {
                callbackOnItem(item);
            }
        }
    }
}


wxString ListDefHelper::GetItemType(const wxString& itemName)
{
	wxString type;
	try
	{
		std::function<void(const json&)> callback = [&type](const json& item) {
			type = towxStr(item["Type"]);
		};
		GetItem(itemName, callback);
	}
	catch(std::exception &e)
	{
		wxMessageBox(itemName  + wxT("-") + e.what());
	}

    return type;
}

void ListDefHelper::InitPropGrid(wxPropertyGridInterface* propGrid, const wxString& mapName)
{
    m_mapName = mapName;

    if(m_json.count("Order") == 0) return;
    for(auto &categoryName : m_json["Order"])
    {
        auto categroy = m_json[categoryName.get<std::string>().c_str()];
        wxPGProperty *p = propGrid->Append(new wxPropertyCategory(towxStr(categroy["ShowName"])));
        p->SetHelpString(towxStr((categroy["Help"])));
        for(auto &item : categroy["Items"])
        {
            AddItem(propGrid, mapName, item);
        }
    }
}

void ListDefHelper::BindEvent(wxEvtHandler* eventHandler)
{
    eventHandler->Bind(wxEVT_PG_DOUBLE_CLICK, &ListDefHelper::OnItemDClick, this);
}

void ListDefHelper::OnItemDClick(wxPropertyGridEvent& event)
{
    wxPGProperty *property = event.GetProperty();

    wxPGProperty *parent = property;
    while(parent->GetParent() && !parent->GetParent()->IsCategory() && !parent->GetParent()->IsRoot())
	{
		parent = parent->GetParent();
	}

	wxString type = GetItemType(parent->GetBaseName());
    wxString value = property->GetValue();

    if(type == "MagicFileWithRadiusList")
	{
		if(property->GetBaseName() == MAGIC_FILE_PROPERTY_NAME)
		{
			type = "MagicFile";
		}
	}

    if(type == "ScriptFile")
    {
    	if(value.IsEmpty())
		{
			OpenRelateFile(wxT(""));
			return;
		}
		wxString path1 = wxT("script\\map\\") + m_mapName + wxT("\\") + value;
		wxString path2 = wxT("script\\common\\") + value;

		if(IsRelateFileExist(path1))
		{
			OpenRelateFile(path1);
		}
		else if(IsRelateFileExist(path2))
		{
			OpenRelateFile(path2);
		}
		else
		{
			ShowFileNotExistMsg(value);
		}
    }
	else if (type == "MagicFile")
	{
		if(value.IsEmpty())
		{
			OpenRelateFile(wxT(""));
			return;
		}
		wxString path = wxT("ini\\magic\\") + value;
		if(IsRelateFileExist(path))
		{
			OpenRelateFile(path);
		}
		else
		{
			ShowFileNotExistMsg(Utils::GetFullPath(path));
		}
	}
}

void ListDefHelper::AddItem(wxPropertyGridInterface* propGrid, const wxString& mapName, const json& item)
{
    std::string type = item["Type"];
    wxPGProperty *p = nullptr;
    if(type == "ScriptFile")
    {
        p = propGrid->Append(new FileNameProperty(towxStr(item["Name"]), wxPG_LABEL, towxStr(item["Default"])));
        p->SetAttribute(wxPG_FILE_INITIAL_PATH, wxVariant(mapName.empty() ? Utils::GetFullPath(wxT("script\\common")) : Utils::GetFullPath(wxT("script\\map\\") + mapName)));
        p->SetAttribute(wxPG_FILE_WILDCARD, wxVariant(wxT("TXT文件(*.txt)|*.txt")));
    }
    else if(type == "UINT")
    {
        p = propGrid->Append(new wxUIntProperty(towxStr(item["Name"]), wxPG_LABEL, item["Default"].get<int>()));
    }
    else if(type == "MagicFileWithRadiusList")
    {
        p = propGrid->Append(new MagicFileWithRadiusList(towxStr(item["Name"]), wxPG_LABEL, Utils::GetFullPath(wxT("ini\\magic"))));
    }
    else if(type == "MagicFile")
    {
        p = propGrid->Append(new FileNameProperty(towxStr(item["Name"]), wxPG_LABEL, towxStr(item["Default"])));
        p->SetAttribute(wxPG_FILE_INITIAL_PATH, wxVariant(Utils::GetFullPath(wxT("ini\\magic"))));
        p->SetAttribute(wxPG_FILE_WILDCARD, wxVariant(wxT("ini文件(*.ini)|*.ini")));
    }
    else if(type == "UIntRange")
    {
        p = propGrid->Append(new wxUIntProperty(towxStr(item["Name"]), wxPG_LABEL, item["Default"].get<int>()));
        p->SetAttribute(wxPG_ATTR_MIN, wxVariant(item["Range"][0].get<int>()));
        p->SetAttribute(wxPG_ATTR_MAX, wxVariant(item["Range"][1].get<int>()));
        propGrid->SetPropertyEditor(p, wxPGEditor_SpinCtrl);
    }
    else if(type == "UIntEnum")
    {
        wxArrayString keys;
        wxArrayInt values;
        for(auto &kv : item["Enums"])
        {
            keys.Add(towxStr(kv[0]));
            values.Add(kv[1].get<int>());
        }
        p = propGrid->Append(new wxEnumProperty(towxStr(item["Name"]), wxPG_LABEL, keys, values, item["Default"].get<int>()));
    }

    if(p != nullptr)
    {
        p->SetHelpString(towxStr((item["Help"])));
    }
}

void ListDefHelper::OpenRelateFile(const wxString& relatePath)
{
    if(relatePath.IsEmpty())
    {
        wxMessageBox(wxT("请先设置该属性的值"), wxT("消息"));
        return;
    }
    wxString fullPath = Utils::GetFullPath(relatePath);
    if(wxFileName::FileExists(fullPath))
    {
        wxString program = wxT("explorer");
        if(wxFileName(relatePath).GetExt() == wxT("txt") &&
                wxFileName::FileExists(Utils::GetFullPath(wxT("JxqyScriptEditor.exe"))))
            program = Utils::GetFullPath(wxT("JxqyScriptEditor.exe"));
        wxExecute(program + wxT(" \"")  + fullPath + wxT("\""));
    }
    else
    {
        wxMessageBox(relatePath + wxT("  文件不存在"), wxT("消息"));
        return;
    }
}

bool ListDefHelper::IsRelateFileExist(const wxString& relatePath)
{
	return wxFileName::FileExists(Utils::GetFullPath(relatePath));
}

void ListDefHelper::ShowFileNotExistMsg(const wxString& path)
{
	wxMessageBox(path + wxT("  文件不存在"), wxT("消息"));
}
