#include "MapToolPropertys.hpp"

#include "wx/filename.h"
#include "wx/filedlg.h"
#include "wx/tokenzr.h"
#include "wx/msgdlg.h"

#include "InputUIntDialog.h"

bool FileNameEditorDialogAdapter::DoShowDialog(wxPropertyGrid* propGrid, wxPGProperty* property)
{
    wxString path;

    wxFileDialog dlg( propGrid->GetPanel(),
                      property->GetAttribute(wxS("DialogTitle"), _("Choose a file")),
                      property->GetAttribute(wxS("InitialPath"), path),
                      wxEmptyString,
                      property->GetAttribute(wxPG_FILE_WILDCARD, wxALL_FILES),
                      property->GetAttributeAsLong(wxPG_FILE_DIALOG_STYLE, 0),
                      wxDefaultPosition );



    if ( dlg.ShowModal() == wxID_OK )
    {
		wxFileName filename = dlg.GetPath();
        SetValue( filename.GetFullName() );
        return true;
    }
    return false;
}
wxPGEditorDialogAdapter* FileNameProperty::GetEditorDialog() const
{
	return new FileNameEditorDialogAdapter();
}

void FileNameProperty::OnSetValue()
{

}

bool AddMagicToListDialogAdapter::DoShowDialog(wxPropertyGrid* propGrid, wxPGProperty* property)
{
    wxFileDialog dlg( propGrid->GetPanel(),
                      property->GetAttribute(wxS("DialogTitle"), _("Choose a file")),
                      property->GetAttribute(wxS("InitialPath"), wxT("")),
                      wxEmptyString,
                      property->GetAttribute(wxPG_FILE_WILDCARD, wxALL_FILES),
                      property->GetAttributeAsLong(wxPG_FILE_DIALOG_STYLE, 0),
                      wxDefaultPosition );



    if ( dlg.ShowModal() == wxID_OK )
    {
		InputUIntDialog uintDlg(nullptr);
		unsigned int radius = 1;
		if( uintDlg.ShowModal() == wxID_OK)
		{
			radius = uintDlg.GetValue();
		}

    	wxFileName filename = dlg.GetPath();
    	property->AppendChild(new MagicFileWithRadius(wxString::Format(wxT("武功%d"), property->GetChildCount()+1),
													wxPG_LABEL,
													filename.GetFullName(),
													radius));
		propGrid->RefreshProperty(property);

        return false;
    }

    return false;
}

wxPGEditorDialogAdapter* MagicFileWithRadiusList::GetEditorDialog() const
{
	return new AddMagicToListDialogAdapter();
}
void MagicFileWithRadiusList::OnSetValue()
{

}



