/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_notbk.h
// Purpose:     XML resource handler for wxNotebook
// Author:      Vaclav Slavik
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_XH_NOTBK_H_
#  define _WX_XH_NOTBK_H_
#  include "wx/xrc/xmlres.h"
#  if  wxUSE_XRC && wxUSE_NOTEBOOK
class WXDLLIMPEXP_FWD_CORE wxNotebook;
class WXDLLIMPEXP_XRC wxNotebookXmlHandler : public wxXmlResourceHandler
{
  wxDECLARE_DYNAMIC_CLASS(wxNotebookXmlHandler);
public:
  wxNotebookXmlHandler();
  wxObject* DoCreateResource() override;
  bool CanHandle(wxXmlNode* node) override;
private:
  bool m_isInside;
  wxNotebook* m_notebook;
};
#  endif
#endif
