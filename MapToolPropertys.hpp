#ifndef MAPTOOLPROPERTYS_HPP_INCLUDED
#define MAPTOOLPROPERTYS_HPP_INCLUDED

#include "wx/propgrid/propgrid.h"
#include "wx/propgrid/advprops.h"
#include "Utils.h"
#include "mydefs.hpp"

class FileNameEditorDialogAdapter : public wxPGEditorDialogAdapter
{
	virtual bool DoShowDialog( wxPropertyGrid* propGrid,
                               wxPGProperty* property );
};

class FileNameProperty : public wxFileProperty
{
public:
	FileNameProperty( const wxString& label = wxPG_LABEL,
                    const wxString& name = wxPG_LABEL,
                    const wxString& value = wxEmptyString)
                    :wxFileProperty(label, name, value)
	{

	}
	virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
	virtual void OnSetValue();
};

class MagicFileWithRadius : public wxStringProperty
{
public:
	MagicFileWithRadius( const wxString& label = wxPG_LABEL,
                      const wxString& name = wxPG_LABEL ,
						const wxString& filename = wxT(""),
							unsigned int radius = 0)
                      :wxStringProperty(label, name, wxT("<composed>"))
	{
		auto magicProperty = new FileNameProperty(MAGIC_FILE_PROPERTY_NAME, wxPG_LABEL, filename);
		magicProperty->SetAttribute(wxPG_FILE_INITIAL_PATH, wxVariant(Utils::GetFullPath(wxT("ini\\magic"))));
		magicProperty->SetAttribute(wxPG_FILE_WILDCARD, wxVariant(wxT("ini文件(*.ini)|*.ini")));
		magicProperty->SetHelpString(wxT("武功文件"));
		AppendChild(magicProperty);
		auto radiousProperty = new wxUIntProperty(wxT("Radius"), wxPG_LABEL, radius);
		radiousProperty->SetHelpString(wxT("使用距离"));
		AppendChild(radiousProperty);
	}
};

class AddMagicToListDialogAdapter : public wxPGEditorDialogAdapter
{
	virtual bool DoShowDialog( wxPropertyGrid* propGrid,
                               wxPGProperty* property );
};

class MagicFileWithRadiusList : public wxStringProperty
{
public:
	MagicFileWithRadiusList( const wxString& label = wxPG_LABEL,
                      const wxString& name = wxPG_LABEL ,
                      const wxString& initPath = wxT(""))
                      :wxStringProperty(label, name, wxT("<composed>"))
	{
		SetAttribute(wxPG_FILE_INITIAL_PATH, wxVariant(Utils::GetFullPath(wxT("ini\\magic"))));
		SetAttribute(wxPG_FILE_WILDCARD, wxVariant(wxT("ini文件(*.ini)|*.ini")));
		SetEditor(wxPGEditor_TextCtrlAndButton);
	}

	virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
	virtual void OnSetValue();
};

#endif // MAPTOOLPROPERTYS_HPP_INCLUDED
