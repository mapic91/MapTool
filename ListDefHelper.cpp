#include "ListDefHelper.h"

#include "wx/msgdlg.h"
#include "wx/stdpaths.h"
#include "wx/tokenzr.h"

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

wxVariant ListDefHelper::GetItemDefaultValue(const wxString &itemName)
{
	std::unique_ptr<wxVariant> ret;
	try{
		GetItem(itemName, [&ret](const json& item) {
			if(item.count("Default") == 0) return;
			if(item["Default"].is_number())
			{
				ret.reset(new wxVariant(item["Default"].get<int>()));
			}
			else
			{
				ret.reset(new wxVariant(towxStr(item["Default"])));
			}
		});

		if(ret == nullptr)
		{
			return wxVariant();
		}
	}
	catch(std::exception &e)
	{
		wxMessageBox(itemName + wxT(":") + wxString(e.what()));
	}

	return wxVariant(*ret.get());
}

wxString ListDefHelper::GetCategoryShowName(const wxString &itemName)
{
    if(m_json.count("Order") == 0) return wxT("");
    for(auto &categoryName : m_json["Order"])
    {
        auto categroy = m_json[categoryName.get<std::string>().c_str()];
        for(auto &item : categroy["Items"])
        {
            if(towxStr(item["Name"]) == itemName)
            {
                return towxStr(categroy["ShowName"]);
            }
        }
    }
    return wxT("");
}

wxString ListDefHelper::GetItemType(const wxString& itemName)
{
    wxString type;
    try
    {
        std::function<void(const json&)> callback = [&type](const json& item)
        {
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
    m_targetGridInterface = propGrid;

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

void ListDefHelper::BindEvent(wxEvtHandler* eventHandler,
							std::function<void(const wxString &propertyName, const wxString& typeName)> onItemBtnClick,
							std::function<void(const wxString &propertyName, const wxString& typeName, const wxVariant& value)> onItemValueChanged)
{
    eventHandler->Bind(wxEVT_PG_DOUBLE_CLICK, &ListDefHelper::OnItemDClick, this);
    eventHandler->Bind(wxEVT_PG_CHANGED, &ListDefHelper::OnItemValueChanged, this);
    eventHandler->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ListDefHelper::OnItemBtnClick, this);
    m_onPropertyBtnClicked = onItemBtnClick;
    m_onItemValueChanged = onItemValueChanged;
}

void ListDefHelper::UnbindEvent(wxEvtHandler* eventHandler)
{
    eventHandler->Unbind(wxEVT_PG_DOUBLE_CLICK, &ListDefHelper::OnItemDClick, this);
    eventHandler->Unbind(wxEVT_PG_CHANGED, &ListDefHelper::OnItemValueChanged, this);
    eventHandler->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &ListDefHelper::OnItemBtnClick, this);
}

void ListDefHelper::SetValue(wxPropertyGridInterface* propGrid, const wxString& key, const wxVariant& value)
{
    wxString categoryName = GetCategoryShowName(key);
    if(categoryName.IsEmpty()) return;
    wxPGProperty *pgCategory = propGrid->GetProperty(categoryName);
    if(pgCategory != nullptr && pgCategory->IsCategory())
    {
        std::function<void(const json&)> callback = [pgCategory,&key, &value, propGrid](const json& item)
        {
            std::string type = item["Type"];
            wxPGProperty *pgItem = pgCategory->GetPropertyByName(key);
            if (pgItem == nullptr) return;

            if(type == "UIntEnum")
            {
                pgItem->SetChoiceSelection(value.GetInteger());
            }
            else if (type == "MagicFileWithRadiusList")
            {
                pgItem->DeleteChildren();
                wxStringTokenizer tokn(value.GetString(), wxT(";；"));
                while(tokn.HasMoreTokens())
                {
                    wxString token = tokn.GetNextToken();
                    wxArrayString arr = wxStringTokenize(token, wxT(":："));
                    if(arr.GetCount() == 2)
                    {
                        long radius = 0;
                        arr[1].ToLong(&radius);
                        pgItem->AppendChild(new MagicFileWithRadius(wxString::Format(wxT("武功%d"), pgItem->GetChildCount()+1),
                                            wxPG_LABEL,
                                            arr[0],
                                            radius));
                    }
                }
                propGrid->Collapse(pgItem);
            }
            else
            {
                pgItem->SetValueFromString(value.GetString());
            }
        };
        GetItem(key, callback);
    }
}

void ListDefHelper::SetValues(wxPropertyGridInterface* propGrid, const std::map<wxString, wxVariant>& keyValues)
{
    for(auto &kv : keyValues)
    {
		SetValue(propGrid, kv.first, kv.second);
    }
}

void ListDefHelper::AssignValues(wxPropertyGridInterface* propGrid, std::map<wxString, wxVariant>& keyValues, bool noEmpty)
{
    if(m_json.count("Order") == 0) return;
    for(auto &categoryName : m_json["Order"])
    {
        auto category = m_json[categoryName.get<std::string>().c_str()];
        wxString showName = towxStr(category["ShowName"]);
        wxPGProperty *pgCategory = propGrid->GetProperty(showName);
        if(pgCategory == nullptr && pgCategory->IsCategory()) continue;
        for(auto &item : category["Items"])
        {
            wxString name = towxStr(item["Name"]);
            wxPGProperty *pgItem = pgCategory->GetPropertyByName(name);
            if(pgItem != nullptr)
            {
                std::string type = item["Type"];
                if (type == "MagicFileWithRadiusList")
                {
                    wxString composedValue;
                    for(unsigned int i = 0; i < pgItem->GetChildCount(); i++)
                    {
                        auto child = pgItem->GetPropertyByName(wxString::Format(wxT("武功%d"), i+1));
                        if(child != nullptr)
                        {
                            MagicFileWithRadius *pg = static_cast<MagicFileWithRadius*>(child);
                            if(pg->IsValueOk())
                            {
                                composedValue += pg->GetValueStringForIni();
                            }
                        }
                    }
                    if(!noEmpty || !composedValue.IsEmpty())
					{
						keyValues[name] = composedValue;
					}
                }
                else
                {
                    wxVariant value = pgItem->GetValue();
                    if(!noEmpty || !value.GetString().IsEmpty())
                    {
                    	keyValues[name] = value;
                    }
                }
            }
        }
    }
}

void ListDefHelper::ClearAllValues(wxPropertyGridInterface *propGrid)
{
	if(m_json.count("Order") == 0) return;
    for(auto &categoryName : m_json["Order"])
    {
        auto category = m_json[categoryName.get<std::string>().c_str()];
        wxString showName = towxStr(category["ShowName"]);
        wxPGProperty *pgCategory = propGrid->GetProperty(showName);
        if(pgCategory == nullptr && pgCategory->IsCategory()) continue;
        for(auto &item : category["Items"])
        {
            wxString name = towxStr(item["Name"]);
            wxPGProperty *pgItem = pgCategory->GetPropertyByName(name);
            if(pgItem != nullptr)
            {
                std::string type = item["Type"];
                if (type == "MagicFileWithRadiusList")
                {
                    pgItem->DeleteChildren();
                }
                else
                {
                    pgItem->SetValue(wxVariant());
                }
            }
        }
    }
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
    else if (type == "FileName")
    {
        if(value.IsEmpty())
        {
            OpenRelateFile(wxT(""));
            return;
        }

        std::function<void(const json&)> callback = [this,&value](const json& item)
        {
            wxString path = towxStr(item["StartPath"]) + wxT("\\") + value;
            if(IsRelateFileExist(path))
            {
                OpenRelateFile(path);
            }
            else
            {
                ShowFileNotExistMsg(Utils::GetFullPath(path));
            }
        };
        GetItem(property->GetBaseName(), callback);
    }
}

void ListDefHelper::OnItemValueChanged(wxPropertyGridEvent &event)
{
	if(m_onItemValueChanged != nullptr)
    {
        wxPGProperty *property = event.GetProperty();
        if(property != nullptr)
        {
            wxString type = GetItemType(property->GetBaseName());
            m_onItemValueChanged(property->GetBaseName(), type, property->GetValue());
        }
    }
}

void ListDefHelper::OnItemBtnClick(wxCommandEvent& event)
{
    if(m_onPropertyBtnClicked != nullptr)
    {
        wxPGProperty *property = m_targetGridInterface->GetSelection();
        if(property != nullptr)
        {
            wxString type = GetItemType(property->GetBaseName());
            m_onPropertyBtnClicked(property->GetBaseName(), type);
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
    else if(type == "Int")
    {
        p = propGrid->Append(new wxIntProperty(towxStr(item["Name"]), wxPG_LABEL, item["Default"].get<int>()));
    }
    else if(type == "UInt")
    {
        p = propGrid->Append(new wxUIntProperty(towxStr(item["Name"]), wxPG_LABEL, item["Default"].get<int>()));
    }
    else if(type == "MagicFileWithRadiusList")
    {
        p = propGrid->Append(new MagicFileWithRadiusList(towxStr(item["Name"]), wxPG_LABEL, Utils::GetFullPath(wxT("ini\\magic"))));
    }
    else if(type == "FileName")
    {
        p = propGrid->Append(new FileNameProperty(towxStr(item["Name"]), wxPG_LABEL, towxStr(item["Default"])));
        p->SetAttribute(wxPG_FILE_INITIAL_PATH, wxVariant(Utils::GetFullPath(towxStr(item["StartPath"]))));
        p->SetAttribute(wxPG_FILE_WILDCARD, wxVariant(towxStr(item["FileNameFilter"])));
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
    else if (type == "String")
    {
        p = propGrid->Append(new wxStringProperty(towxStr(item["Name"]), wxPG_LABEL, towxStr(item["Default"])));
    }
    else if (type == "FixedPosString")
    {
        p = propGrid->Append(new wxStringProperty(towxStr(item["Name"]), wxPG_LABEL, towxStr(item["Default"])));
        p->SetEditor(wxPGEditor_TextCtrlAndButton);
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

wxPGProperty* ListDefHelper::GetCategory(wxPropertyGridInterface *propGrid, const wxString& name)
{
    return propGrid->GetPropertyCategory(name);
}
