#ifndef INPUTUINTDIALOG_H
#define INPUTUINTDIALOG_H

//(*Headers(InputUIntDialog)
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class InputUIntDialog: public wxDialog
{
	public:

		InputUIntDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~InputUIntDialog();

	private:

		//(*Declarations(InputUIntDialog)
		wxStaticText* m_MessageLabel;
		wxButton* m_ButtonOK;
		wxSpinCtrl* m_UIntValueInput;
		//*)

	protected:

		//(*Identifiers(InputUIntDialog)
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL1;
		static const long ID_BUTTON1;
		//*)

	public:

		//(*Handlers(InputUIntDialog)
		void Onm_ButtonOKClick(wxCommandEvent& event);
		//*)

		unsigned int GetValue()
		{
			return m_UIntValueInput->GetValue();
		}

		DECLARE_EVENT_TABLE()
};

#endif
