/***************************************************************
 * Name:      App.h
 * Purpose:   Defines Application Class
 * Author:
 * Created:
 * Copyright:
 * License:
 **************************************************************/

#ifndef WXWIDGETS_TEMPLATEAPP_H
#define WXWIDGETS_TEMPLATEAPP_H

#include <wx/app.h>
class MapTool;

class wxWidgets_templateApp : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();

	private:
		virtual int FilterEvent(wxEvent &event);

	private:
		MapTool *m_mapTool;
};

#endif // WXWIDGETS_TEMPLATEAPP_H
