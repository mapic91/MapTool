#include "InputUIntDialog.h"

//(*InternalHeaders(InputUIntDialog)
#include <wx/string.h>
//*)

//(*IdInit(InputUIntDialog)
const long InputUIntDialog::ID_STATICTEXT1 = wxNewId();
const long InputUIntDialog::ID_SPINCTRL1 = wxNewId();
const long InputUIntDialog::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(InputUIntDialog,wxDialog)
	//(*EventTable(InputUIntDialog)
	//*)
END_EVENT_TABLE()

InputUIntDialog::InputUIntDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(InputUIntDialog)
	Create(parent, id, _T("请输入"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(294,143));
	Move(wxDefaultPosition);
	m_MessageLabel = new wxStaticText(this, ID_STATICTEXT1, _T("请输入距离："), wxPoint(24,16), wxSize(88,16), 0, _T("ID_STATICTEXT1"));
	m_UIntValueInput = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(80,48), wxDefaultSize, 0, 0, 99999999, 0, _T("ID_SPINCTRL1"));
	m_UIntValueInput->SetValue(_T("0"));
	m_ButtonOK = new wxButton(this, ID_BUTTON1, _T("确定"), wxPoint(96,104), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&InputUIntDialog::Onm_ButtonOKClick);
	//*)
}

InputUIntDialog::~InputUIntDialog()
{
	//(*Destroy(InputUIntDialog)
	//*)
}

void InputUIntDialog::Onm_ButtonOKClick(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}
