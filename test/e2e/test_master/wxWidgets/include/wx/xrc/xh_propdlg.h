/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_propdlg.h
// Purpose:     XML resource handler for wxPropertySheetDialog
// Author:      Sander Berents
// Created:     2007/07/12
// Copyright:   (c) 2007 Sander Berents
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_XH_PROPDLG_H_
#  define _WX_XH_PROPDLG_H_
#  include "wx/xrc/xmlres.h"
#  if  wxUSE_XRC
class WXDLLIMPEXP_FWD_CORE wxPropertySheetDialog;
class WXDLLIMPEXP_XRC wxPropertySheetDialogXmlHandler : public wxXmlResourceHandler
{
  wxDECLARE_DYNAMIC_CLASS(wxPropertySheetDialogXmlHandler);
public:
  wxPropertySheetDialogXmlHandler();
  wxObject* DoCreateResource() override;
  bool CanHandle(wxXmlNode* node) override;
private:
  bool m_isInside;
  wxPropertySheetDialog* m_dialog;
};
#  endif
#endif
