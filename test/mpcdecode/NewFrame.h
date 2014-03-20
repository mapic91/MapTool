#ifndef NEWFRAME_H
#define NEWFRAME_H

//(*Headers(NewFrame)
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/frame.h>
//*)
#include "MpcDecode.hpp"

class NewFrame: public wxFrame
{
	public:

		NewFrame(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~NewFrame();

	private:

		//(*Declarations(NewFrame)
		wxButton* Button1;
		wxButton* Button2;
		wxButton* file;
		wxStaticBitmap* ShowBitmap;
		//*)
		MpcDecode decode;
		long index;

	protected:

		//(*Identifiers(NewFrame)
		static const long ID_STATICBITMAP1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		//*)

	public:

		//(*Handlers(NewFrame)
		void OnfileClick(wxCommandEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
