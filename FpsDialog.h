#ifndef FPSDIALOG_H
#define FPSDIALOG_H

//(*Headers(FpsDialog)
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class FpsDialog: public wxDialog
{
	public:

		FpsDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~FpsDialog();

	private:

		//(*Declarations(FpsDialog)
		wxButton* m_bCancle;
		wxButton* m_bOk;
		wxStaticText* StaticText1;
		wxSpinCtrl* m_sbFps;
		//*)

	protected:

		//(*Identifiers(FpsDialog)
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	public:

		//(*Handlers(FpsDialog)
		void Onm_bOkClick(wxCommandEvent& event);
		void Onm_bCancleClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

		int GetFps();
		void SetFpsMilliseconds(int milliseconds);

		DECLARE_EVENT_TABLE()
};

#endif
