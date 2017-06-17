#ifndef YESNOALLDIALOG_H
#define YESNOALLDIALOG_H

//(*Headers(YesNoAllDialog)
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class YesNoAllDialog: public wxDialog
{
	public:

		YesNoAllDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		void Set(const wxString &message, const wxString &title, const wxPoint& pos=wxDefaultPosition);
		virtual ~YesNoAllDialog();

	public:
		enum {YES, NO, ALL, CANCLE};

	private:

		//(*Declarations(YesNoAllDialog)
		wxButton* m_bYes;
		wxButton* m_bCancle;
		wxButton* m_bAll;
		wxStaticText* m_message;
		wxButton* m_bNo;
		//*)

	protected:

		//(*Identifiers(YesNoAllDialog)
		static const long ID_STATICTEXT1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		//*)

	public:

		//(*Handlers(YesNoAllDialog)
		void Onm_bYesClick(wxCommandEvent& event);
		void Onm_bNoClick(wxCommandEvent& event);
		void Onm_bAllClick(wxCommandEvent& event);
		void Onm_bCancleClick(wxCommandEvent& event);
		//*)
        /** \brief Set position on screen, if dialog is out
         * of screen(or parent if presented),
         * correction is used to made it show on screen(or parent if presented).
         *
         * \param pos Screen position
         * \return void
         *
         */
		void SetPosition(const wxPoint& pos);

		DECLARE_EVENT_TABLE()
};

#endif
