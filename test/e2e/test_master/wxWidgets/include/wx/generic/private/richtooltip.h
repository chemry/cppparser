///////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/private/richtooltip.h
// Purpose:     wxRichToolTipGenericImpl declaration.
// Author:      Vadim Zeitlin
// Created:     2011-10-18
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef _WX_GENERIC_PRIVATE_RICHTOOLTIP_H_
#  define _WX_GENERIC_PRIVATE_RICHTOOLTIP_H_
#  include "wx/icon.h"
#  include "wx/colour.h"
// ----------------------------------------------------------------------------
// wxRichToolTipGenericImpl: defines generic wxRichToolTip implementation.
// ----------------------------------------------------------------------------
class wxRichToolTipGenericImpl : public wxRichToolTipImpl
{
public:
  wxRichToolTipGenericImpl(const wxString& title, const wxString& message)
    : 
        m_title(title),
        m_message(message)
    
    {

        m_tipKind = wxTipKind_Auto;

        // This is pretty arbitrary, we could follow MSW and use some multiple
        // of double-click time here.
        m_timeout = 5000;
        m_delay = 0;
        }
  void SetBackgroundColour(const wxColour& col, const wxColour& colEnd) override;
  void SetCustomIcon(const wxIcon& icon) override;
  void SetStandardIcon(int icon) override;
  void SetTimeout(unsigned milliseconds, unsigned millisecondsDelay = 0) override;
  void SetTipKind(wxTipKind tipKind) override;
  void SetTitleFont(const wxFont& font) override;
  void ShowFor(wxWindow* win, const wxRect* rect = NULL) override;
protected:
  wxString m_title, m_message;
private:
  wxIcon m_icon;
  wxColour m_colStart, m_colEnd;
  unsigned m_timeout, m_delay;
  wxTipKind m_tipKind;
  wxFont m_titleFont;
};
#endif