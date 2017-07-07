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
		m_magicProperty = new FileNameProperty(MAGIC_FILE_PROPERTY_NAME, wxPG_LABEL, filename);
		m_magicProperty->SetAttribute(wxPG_FILE_INITIAL_PATH, wxVariant(Utils::GetFullPath(wxT("ini\\magic"))));
		m_magicProperty->SetAttribute(wxPG_FILE_WILDCARD, wxVariant(wxT("ini文件(*.ini)|*.ini")));
		m_magicProperty->SetHelpString(wxT("武功文件"));
		AppendChild(m_magicProperty);
		m_radiousProperty = new wxUIntProperty(wxT("Radius"), wxPG_LABEL, radius);
		m_radiousProperty->SetHelpString(wxT("使用距离"));
		m_radiousProperty->SetEditor(wxPGEditor_SpinCtrl);
		AppendChild(m_radiousProperty);
	}

	wxString GetValueStringForIni()
	{
		return m_magicProperty->GetValue().GetString() + wxT(":") + m_radiousProperty->GetValue().GetString() + wxT(";");
	}

	bool IsValueOk()
	{
		return !m_magicProperty->GetValue().GetString().IsEmpty();
	}

private:
	wxPGProperty *m_magicProperty;
	wxPGProperty *m_radiousProperty;
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
