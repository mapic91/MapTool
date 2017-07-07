#ifndef LISTDEFREADER_H
#define LISTDEFREADER_H

#include <functional>
#include <map>

#include "wx/string.h"
#include "wx/propgrid/propgrid.h"
#include "wx/propgrid/advprops.h"

#include "mydefs.hpp"

class wxPropertyGridInterface;

#include "json.hpp"

using json = nlohmann::json;

class ListDefHelper
{
	public:
		ListDefHelper();

		bool Read(const wxString &jsonFilePath);

		const json& GetJson() { return m_json; }

		void GetItem(const wxString &itemName, const std::function<void(const json&)> &callbackOnItem);
		wxVariant GetItemDefaultValue(const wxString &itemName);
		wxString GetCategoryShowName(const wxString &itemName);
		wxString GetItemType(const wxString &itemName);

		/**\brief Create property struct form json settings.
		 *
		 *\param
		 *\return
		 *
		 */
		void InitPropGrid(wxPropertyGridInterface *propGrid, const wxString& mapName);
		void BindEvent(wxEvtHandler *eventHandler,
						std::function<void(const wxString &propertyName, const wxString& typeName)> onItemBtnClick,
						std::function<void(const wxString &propertyName, const wxString& typeName, const wxVariant& value)> onItemValueChanged);
		void UnbindEvent(wxEvtHandler *eventHandler);
		void SetValue(wxPropertyGridInterface *propGrid, const wxString& key, const wxVariant &value);
		void SetValues(wxPropertyGridInterface *propGrid, const std::map<wxString, wxVariant> &keyValues);
		void AssignValues(wxPropertyGridInterface *propGrid, std::map<wxString, wxVariant> &keyValues, bool noEmpty = false);
		void ClearAllValues(wxPropertyGridInterface *propGrid);

		void OnItemDClick(wxPropertyGridEvent &event);
		void OnItemValueChanged(wxPropertyGridEvent &event);
		void OnItemBtnClick(wxCommandEvent &event);

	protected:

	private:
		void AddItem(wxPropertyGridInterface *propGrid, const wxString& mapName, const json& item);

		void OpenRelateFile(const wxString& relatePath);
		bool IsRelateFileExist(const wxString& relatePath);
		void ShowFileNotExistMsg(const wxString& path);

		wxPGProperty* GetCategory(wxPropertyGridInterface *propGrid, const wxString& name);

	private:
		json m_json;
		wxString m_mapName;

		std::function<void(const wxString &propertyName, const wxString& typeName)> m_onPropertyBtnClicked;
		std::function<void(const wxString &propertyName, const wxString& typeName, const wxVariant& value)> m_onItemValueChanged;
		wxPropertyGridInterface* m_targetGridInterface = nullptr;
};

#endif // LISTDEFREADER_H
