/////////////////////////////////////////////////////////////////////////////
// Name:        wx/aui/tabmdi.h
// Purpose:     Generic MDI (Multiple Document Interface) classes
// Author:      Hans Van Leemputten
// Modified by: Benjamin I. Williams / Kirix Corporation
// Created:     29/07/2002
// Copyright:   (c) Hans Van Leemputten
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_AUITABMDI_H_
#  define _WX_AUITABMDI_H_
#  if  wxUSE_AUI && wxUSE_MDI
// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#    include "wx/frame.h"
#    include "wx/panel.h"
#    include "wx/notebook.h"
#    include "wx/icon.h"
#    include "wx/mdi.h"
#    include "wx/aui/auibook.h"
//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
class WXDLLIMPEXP_FWD_AUI wxAuiMDIParentFrame;
class WXDLLIMPEXP_FWD_AUI wxAuiMDIClientWindow;
class WXDLLIMPEXP_FWD_AUI wxAuiMDIChildFrame;
//-----------------------------------------------------------------------------
// wxAuiMDIParentFrame
//-----------------------------------------------------------------------------
class WXDLLIMPEXP_AUI wxAuiMDIParentFrame : public wxFrame
{
public:
  wxAuiMDIParentFrame();
  wxAuiMDIParentFrame(wxWindow* parent, wxWindowID winid, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL, const wxString& name = wxASCII_STR(wxFrameNameStr));
  ~wxAuiMDIParentFrame();
  bool Create(wxWindow* parent, wxWindowID winid, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL, const wxString& name = wxASCII_STR(wxFrameNameStr));
  void SetArtProvider(wxAuiTabArt* provider);
  wxAuiTabArt* GetArtProvider();
  wxAuiNotebook* GetNotebook() const;
#    if  wxUSE_MENUS
  wxMenu* GetWindowMenu() const
  {
    return m_pWindowMenu;
  }
  void SetWindowMenu(wxMenu* pMenu);
  void SetMenuBar(wxMenuBar* pMenuBar) override;
#    endif
  void SetChildMenuBar(wxAuiMDIChildFrame* pChild);
  wxAuiMDIChildFrame* GetActiveChild() const;
  void SetActiveChild(wxAuiMDIChildFrame* pChildFrame);
  wxAuiMDIClientWindow* GetClientWindow() const;
  virtual wxAuiMDIClientWindow* OnCreateClient();
  virtual void Cascade()
  {
  }
  virtual void Tile(wxOrientation orient = wxHORIZONTAL);
  virtual void ArrangeIcons()
  {
  }
  virtual void ActivateNext();
  virtual void ActivatePrevious();
protected:
  wxAuiMDIClientWindow* m_pClientWindow;
  wxEvent* m_pLastEvt;
#    if  wxUSE_MENUS
  wxMenu* m_pWindowMenu;
  wxMenuBar* m_pMyMenuBar;
#    endif
  void Init();
#    if  wxUSE_MENUS
  void RemoveWindowMenu(wxMenuBar* pMenuBar);
  void AddWindowMenu(wxMenuBar* pMenuBar);
  void DoHandleMenu(wxCommandEvent& event);
  void DoHandleUpdateUI(wxUpdateUIEvent& event);
#    endif
  bool ProcessEvent(wxEvent& event) override;
  void DoGetClientSize(int* width, int* height) const override;
private:
  void OnClose(wxCloseEvent& event);
    // close all children, return false if any of them vetoed it
  bool CloseAll();
  wxDECLARE_EVENT_TABLE();
  wxDECLARE_DYNAMIC_CLASS(wxAuiMDIParentFrame);
};
//-----------------------------------------------------------------------------
// wxAuiMDIChildFrame
//-----------------------------------------------------------------------------
class WXDLLIMPEXP_AUI wxAuiMDIChildFrame : public wxTDIChildFrame
{
public:
  wxAuiMDIChildFrame();
  wxAuiMDIChildFrame(wxAuiMDIParentFrame* parent, wxWindowID winid, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxASCII_STR(wxFrameNameStr));
  virtual ~wxAuiMDIChildFrame();
  bool Create(wxAuiMDIParentFrame* parent, wxWindowID winid, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxASCII_STR(wxFrameNameStr));
#    if  wxUSE_MENUS
  void SetMenuBar(wxMenuBar* menuBar) override;
  wxMenuBar* GetMenuBar() const override;
#    endif
  void SetTitle(const wxString& title) override;
  void SetIcons(const wxIconBundle& icons) override;
  void Activate() override;
  bool Destroy() override;
  bool Show(bool show = true) override;
  void OnMenuHighlight(wxMenuEvent& evt);
  void SetMDIParentFrame(wxAuiMDIParentFrame* parent);
  wxAuiMDIParentFrame* GetMDIParentFrame() const;
protected:
  void Init();
public:
    // This function needs to be called when a size change is confirmed,
    // we needed this function to prevent anybody from the outside
    // changing the panel... it messes the UI layout when we would allow it.
  void ApplyMDIChildFrameRect();
protected:
  wxAuiMDIParentFrame* m_pMDIParentFrame;
  bool m_activateOnCreate;
#    if  wxUSE_MENUS
  wxMenuBar* m_pMenuBar;
#    endif
  wxDECLARE_DYNAMIC_CLASS(wxAuiMDIChildFrame);
  wxDECLARE_EVENT_TABLE();
private:
  friend class wxAuiMDIClientWindow;
};
//-----------------------------------------------------------------------------
// wxAuiMDIClientWindow
//-----------------------------------------------------------------------------
class WXDLLIMPEXP_AUI wxAuiMDIClientWindow : public wxAuiNotebook
{
public:
  wxAuiMDIClientWindow();
  wxAuiMDIClientWindow(wxAuiMDIParentFrame* parent, long style = 0);
  virtual bool CreateClient(wxAuiMDIParentFrame* parent, long style = wxVSCROLL | wxHSCROLL);
  virtual wxAuiMDIChildFrame* GetActiveChild();
  virtual void SetActiveChild(wxAuiMDIChildFrame* pChildFrame)
  {
    SetSelection(GetPageIndex(pChildFrame));
  }
protected:
  void PageChanged(int oldSelection, int newSelection);
  void OnPageClose(wxAuiNotebookEvent& evt);
  void OnPageChanged(wxAuiNotebookEvent& evt);
  wxDECLARE_DYNAMIC_CLASS(wxAuiMDIClientWindow);
  wxDECLARE_EVENT_TABLE();
};
#  endif
#endif
