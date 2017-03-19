#include "YesNoAllDialog.h"

#include "wx/settings.h"

//(*InternalHeaders(YesNoAllDialog)
#include <wx/string.h>
//*)

//(*IdInit(YesNoAllDialog)
const long YesNoAllDialog::ID_STATICTEXT1 = wxNewId();
const long YesNoAllDialog::ID_BUTTON1 = wxNewId();
const long YesNoAllDialog::ID_BUTTON2 = wxNewId();
const long YesNoAllDialog::ID_BUTTON3 = wxNewId();
const long YesNoAllDialog::ID_BUTTON4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(YesNoAllDialog,wxDialog)
	//(*EventTable(YesNoAllDialog)
	//*)
END_EVENT_TABLE()

YesNoAllDialog::YesNoAllDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(YesNoAllDialog)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("id"));
	SetClientSize(wxSize(307,67));
	Move(wxDefaultPosition);
	m_message = new wxStaticText(this, ID_STATICTEXT1, wxEmptyString, wxPoint(8,8), wxSize(288,24), 0, _T("ID_STATICTEXT1"));
	m_bYes = new wxButton(this, ID_BUTTON1, _T("是"), wxPoint(5,38), wxSize(65,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	m_bYes->SetToolTip(_T("修改当前"));
	m_bNo = new wxButton(this, ID_BUTTON2, _T("否"), wxPoint(84,38), wxSize(65,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	m_bNo->SetToolTip(_T("不修改当前"));
	m_bAll = new wxButton(this, ID_BUTTON3, _T("后面全是"), wxPoint(162,38), wxSize(65,23), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	m_bCancle = new wxButton(this, ID_BUTTON4, _T("后面全否"), wxPoint(238,38), wxSize(65,23), 0, wxDefaultValidator, _T("ID_BUTTON4"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&YesNoAllDialog::Onm_bYesClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&YesNoAllDialog::Onm_bNoClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&YesNoAllDialog::Onm_bAllClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&YesNoAllDialog::Onm_bCancleClick);
	//*)
}

void YesNoAllDialog::Set(const wxString &message, const wxString &title, const wxPoint& pos)
{
	m_message->SetLabel(message);
	SetTitle(title);
	SetPosition(pos);
}

YesNoAllDialog::~YesNoAllDialog()
{
	//(*Destroy(YesNoAllDialog)
	//*)
}

void YesNoAllDialog::SetPosition(const wxPoint& position)
{
	int maxX, maxY, width, height, minX, minY;
	wxWindow *parent = this->GetParent();
	if(parent)
	{
		parent->GetScreenPosition(&minX, &minY);
		parent->GetSize(&maxX, &maxY);
		maxX += minX;
		maxY += minY;
	}
	else
	{
		maxX = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
		maxX = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
		minX = 0;
		minY = 0;
	}

	//Add some margin
	maxX -= 10;
	maxY -= 10;

	GetSize(&width, &height);
	wxPoint pos = position;
	if(pos.x + width > maxX)
	{
		pos.x = maxX - width;
	}
	if(pos.x < minX)
	{
		pos.x = minX;
	}
	if(pos.y + height > maxY)
	{
		pos.y = maxY - height;
	}
	if(pos.y < minY)
	{
		pos.y = minY;
	}
	Move(pos);
}

void YesNoAllDialog::Onm_bYesClick(wxCommandEvent& event)
{
	EndModal(YES);
}

void YesNoAllDialog::Onm_bNoClick(wxCommandEvent& event)
{
	EndModal(NO);
}

void YesNoAllDialog::Onm_bAllClick(wxCommandEvent& event)
{
	EndModal(ALL);
}

void YesNoAllDialog::Onm_bCancleClick(wxCommandEvent& event)
{
	EndModal(CANCLE);
}
