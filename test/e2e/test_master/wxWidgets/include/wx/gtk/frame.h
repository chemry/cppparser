/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/frame.h
// Purpose:
// Author:      Robert Roebling
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_GTK_FRAME_H_
#  define _WX_GTK_FRAME_H_
//-----------------------------------------------------------------------------
// wxFrame
//-----------------------------------------------------------------------------
class WXDLLIMPEXP_CORE wxFrame : public wxFrameBase
{
public:
    // construction
  wxFrame()
  {
 Init();   }
  wxFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxASCII_STR(wxFrameNameStr))
  {

        Init();

        Create(parent, id, title, pos, size, style, name);
      }
  bool Create(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxASCII_STR(wxFrameNameStr));
#  if  wxUSE_STATUSBAR
  void SetStatusBar(wxStatusBar* statbar) override;
#  endif
#  if  wxUSE_TOOLBAR
  void SetToolBar(wxToolBar* toolbar) override;
#  endif
  bool ShowFullScreen(bool show, long style = wxFULLSCREEN_ALL) override;
  wxPoint GetClientAreaOrigin() const override
  { return wxPoint(0, 0); }
    // implementation from now on
    // --------------------------
  bool SendIdleEvents(wxIdleEvent& event) override;
protected:
    // override wxWindow methods to take into account tool/menu/statusbars
  void DoGetClientSize(int* width, int* height) const override;
#  if  wxUSE_MENUS_NATIVE
  void DetachMenuBar() override;
  void AttachMenuBar(wxMenuBar* menubar) override;
#  endif
private:
  void Init();
  long m_fsSaveFlag;
  wxDECLARE_DYNAMIC_CLASS(wxFrame);
};
#endif
