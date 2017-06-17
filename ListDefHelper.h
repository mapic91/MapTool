#ifndef LISTDEFREADER_H
#define LISTDEFREADER_H

#include <functional>

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
		wxString GetItemType(const wxString &itemName);

		/**\brief Create property struct form json settings.
		 *
		 *\param
		 *\return
		 *
		 */
		void InitPropGrid(wxPropertyGridInterface *propGrid, const wxString& mapName);
		void BindEvent(wxEvtHandler *eventHandler);

		void OnItemDClick(wxPropertyGridEvent &event);

	protected:

	private:
		void AddItem(wxPropertyGridInterface *propGrid, const wxString& mapName, const json& item);

		void OpenRelateFile(const wxString& relatePath);
		bool IsRelateFileExist(const wxString& relatePath);
		void ShowFileNotExistMsg(const wxString& path);

	private:
		json m_json;
		wxString m_mapName;
};

#endif // LISTDEFREADER_H
