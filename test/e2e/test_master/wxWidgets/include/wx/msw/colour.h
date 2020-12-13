/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/colour.h
// Purpose:     wxColour class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_COLOUR_H_
#  define _WX_COLOUR_H_
#  include "wx/object.h"
// ----------------------------------------------------------------------------
// Colour
// ----------------------------------------------------------------------------
class WXDLLIMPEXP_CORE wxColour : public wxColourBase
{
public:
    // constructors
    // ------------
  DEFINE_STD_WXCOLOUR_CONSTRUCTORS bool IsOk() const override
  {
    return m_isInit;
  }
  unsigned char Red() const override
  {
    return m_red;
  }
  unsigned char Green() const override
  {
    return m_green;
  }
  unsigned char Blue() const override
  {
    return m_blue;
  }
  unsigned char Alpha() const override
  {
    return m_alpha;
  }
    // comparison
  bool operator==(const wxColour& colour) const
  {
    return m_isInit == colour.m_isInit && m_red == colour.m_red && m_green == colour.m_green && m_blue == colour.m_blue && m_alpha == colour.m_alpha;
  }
  bool operator!=(const wxColour& colour) const
  {
    return !(*this == colour);
  }
  WXCOLORREF GetPixel() const
  {
    return m_pixel;
  }
  WXCOLORREF m_pixel;
protected:
    // Helper function
  void Init();
  void InitRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) override;
private:
  bool m_isInit;
  unsigned char m_red;
  unsigned char m_blue;
  unsigned char m_green;
  unsigned char m_alpha;
  wxDECLARE_DYNAMIC_CLASS(wxColour);
};
#endif
