/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/combo.h
// Purpose:     Generic wxComboCtrl
// Author:      Jaakko Salli
// Modified by:
// Created:     Apr-30-2006
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_GENERIC_COMBOCTRL_H_
#  define _WX_GENERIC_COMBOCTRL_H_
#  if  wxUSE_COMBOCTRL
// Only define generic if native doesn't have all the features
#    if  !defined(wxCOMBOCONTROL_FULLY_FEATURED)
#      include "wx/containr.h"
// ----------------------------------------------------------------------------
// Generic wxComboCtrl
// ----------------------------------------------------------------------------
#      if  defined(__WXUNIVERSAL__)
// all actions of single line text controls are supported

// popup/dismiss the choice window
#        define wxACTION_COMBOBOX_POPUP	wxT("popup")
#        define wxACTION_COMBOBOX_DISMISS	wxT("dismiss")
#      endif
WXDLLIMPEXP_CORE extern const char wxComboBoxNameStr[];
class WXDLLIMPEXP_CORE wxGenericComboCtrl : public wxNavigationEnabled<wxComboCtrlBase>
{
public:
    // ctors and such
  wxGenericComboCtrl()
  {
    Init();
  }
  wxGenericComboCtrl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxASCII_STR(wxComboBoxNameStr))
  {
    Init();
    (void) Create(parent, id, value, pos, size, style, validator, name);
  }
  bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxASCII_STR(wxComboBoxNameStr));
  virtual ~wxGenericComboCtrl();
  void SetCustomPaintWidth(int width);
  bool IsKeyPopupToggle(const wxKeyEvent& event) const override;
  static int GetFeatures()
  {
    return wxComboCtrlFeatures::All;
  }
#      if  defined(__WXUNIVERSAL__)
    // we have our own input handler and our own actions
  virtual bool PerformAction(const wxControlAction& action, long numArg = 0l, const wxString& strArg = wxEmptyString);
#      endif
protected:
    // Dummies for platform-specific wxTextEntry implementations
#      if  defined(__WXUNIVERSAL__)
    // Looks like there's nothing we need to override here
#      elif  defined(__WXMOTIF__)
  virtual WXWidget GetTextWidget() const
  {
    return NULL;
  }
#      elif  defined(__WXGTK__)
#        if  defined(__WXGTK20__)
  GtkEditable* GetEditable() const override
  {
    return NULL;
  }
  GtkEntry* GetEntry() const override
  {
    return NULL;
  }
#        endif
#      elif  defined(__WXOSX__)
  wxTextWidgetImpl* GetTextPeer() const override;
#      endif
    // For better transparent background rendering
  bool HasTransparentBackground() override;
    // Mandatory virtuals
  void OnResize() override;
    // Event handlers
  void OnPaintEvent(wxPaintEvent& event);
  void OnMouseEvent(wxMouseEvent& event);
private:
  void Init();
  wxDECLARE_EVENT_TABLE();
  wxDECLARE_DYNAMIC_CLASS(wxGenericComboCtrl);
};
#      ifndef _WX_COMBOCONTROL_H_
// If native wxComboCtrl was not defined, then prepare a simple
// front-end so that wxRTTI works as expected.
class WXDLLIMPEXP_CORE wxComboCtrl : public wxGenericComboCtrl
{
public:
  wxComboCtrl()
    : wxGenericComboCtrl()
  {
  }
  wxComboCtrl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxASCII_STR(wxComboBoxNameStr))
    : wxGenericComboCtrl()
  {
    (void) Create(parent, id, value, pos, size, style, validator, name);
  }
  virtual ~wxComboCtrl()
  {
  }
  wxDECLARE_DYNAMIC_CLASS(wxComboCtrl);
};
#      endif
#    else 
#      define wxGenericComboCtrl	wxComboCtrl
#    endif
#  endif
#endif
    // _WX_GENERIC_COMBOCTRL_H_
