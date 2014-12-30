#include "FpsDialog.h"

//(*InternalHeaders(FpsDialog)
#include <wx/string.h>
//*)

//(*IdInit(FpsDialog)
const long FpsDialog::ID_STATICTEXT1 = wxNewId();
const long FpsDialog::ID_SPINCTRL1 = wxNewId();
const long FpsDialog::ID_BUTTON1 = wxNewId();
const long FpsDialog::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(FpsDialog,wxDialog)
	//(*EventTable(FpsDialog)
	//*)
END_EVENT_TABLE()

FpsDialog::FpsDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(FpsDialog)
	Create(parent, id, _T("设置FPS"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(227,86));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("帧率(FPS)："), wxPoint(16,16), wxSize(80,16), 0, _T("ID_STATICTEXT1"));
	m_sbFps = new wxSpinCtrl(this, ID_SPINCTRL1, _T("60"), wxPoint(90,12), wxDefaultSize, 0, 1, 200, 60, _T("ID_SPINCTRL1"));
	m_sbFps->SetValue(_T("60"));
	m_sbFps->SetToolTip(_T("帧率越高，画面更新越快，CPU使用越高，软件越卡\n如果软件较卡，请调低帧率"));
	m_bOk = new wxButton(this, ID_BUTTON1, _T("确定"), wxPoint(24,50), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	m_bCancle = new wxButton(this, ID_BUTTON2, _T("取消"), wxPoint(128,50), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FpsDialog::Onm_bOkClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FpsDialog::Onm_bCancleClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&FpsDialog::OnClose);
	//*)
}

FpsDialog::~FpsDialog()
{
	//(*Destroy(FpsDialog)
	//*)
}

void FpsDialog::Onm_bOkClick(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void FpsDialog::Onm_bCancleClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void FpsDialog::OnClose(wxCloseEvent& event)
{
	EndModal(wxID_CANCEL);
}

int FpsDialog::GetFps()
{
	return m_sbFps->GetValue();
}

void FpsDialog::SetFpsMilliseconds(int milliseconds)
{
	if(milliseconds < 1) return;
	m_sbFps->SetValue(1000/milliseconds);
}

