/*
 * Copyright 2010 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef GrGlyph_DEFINED
#  define GrGlyph_DEFINED
#  include "include/gpu/GrTypes.h"
#  include "src/gpu/GrDrawOpAtlas.h"
#  include "src/gpu/geometry/GrRect.h"
#  include "include/core/SkPath.h"
#  include "include/private/SkChecksum.h"
#  include "include/private/SkFixed.h"
struct GrGlyph
{
  enum MaskStyle {
        kCoverage_MaskStyle,
        kDistance_MaskStyle
  };
  static GrMaskFormat FormatFromSkGlyph(SkMask::Format format)
  {
    switch(format)
    {
      case SkMask::kBW_Format:
      case SkMask::kSDF_Format:
      case SkMask::kA8_Format:
        return kA8_GrMaskFormat;
      case SkMask::k3D_Format:
        return kA8_GrMaskFormat;
      case SkMask::kLCD16_Format:
        return kA565_GrMaskFormat;
      case SkMask::kARGB32_Format:
        return kARGB_GrMaskFormat;
default:
      SkDEBUGFAIL("unsupported SkMask::Format");
      return kA8_GrMaskFormat;
  }
  }
  static MaskStyle MaskStyleFromSkGlyph(const SkGlyph& skGlyph)
  {
    return skGlyph.maskFormat() == SkMask::kSDF_Format ? GrGlyph::MaskStyle::kDistance_MaskStyle : GrGlyph::MaskStyle::kCoverage_MaskStyle;
  }
  GrGlyph(const SkGlyph& skGlyph)
    : fPackedID{skGlyph.getPackedID()}
    , fMaskFormat{FormatFromSkGlyph(skGlyph.maskFormat())}
    , fMaskStyle{MaskStyleFromSkGlyph(skGlyph)}
    , fBounds{GrIRect16::Make(skGlyph.iRect())}
  {
  }
  SkRect destRect(SkPoint origin)
  {
    return SkRect::MakeXYWH(SkIntToScalar(fBounds.fLeft) + origin.x(), SkIntToScalar(fBounds.fTop) + origin.y(), SkIntToScalar(fBounds.width()), SkIntToScalar(fBounds.height()));
  }
  SkRect destRect(SkPoint origin, SkScalar textScale)
  {
    if (fMaskStyle == kCoverage_MaskStyle)
    {
      return SkRect::MakeXYWH(SkIntToScalar(fBounds.fLeft) * textScale + origin.x(), SkIntToScalar(fBounds.fTop) * textScale + origin.y(), SkIntToScalar(fBounds.width()) * textScale, SkIntToScalar(fBounds.height()) * textScale);
    }
    else 
    {
      return SkRect::MakeXYWH((SkIntToScalar(fBounds.fLeft) + SK_DistanceFieldInset) * textScale + origin.x(), (SkIntToScalar(fBounds.fTop) + SK_DistanceFieldInset) * textScale + origin.y(), (SkIntToScalar(fBounds.width()) - 2 * SK_DistanceFieldInset) * textScale, (SkIntToScalar(fBounds.height()) - 2 * SK_DistanceFieldInset) * textScale);
    }
  }
  int width() const
  {
    return fBounds.width();
  }
  int height() const
  {
    return fBounds.height();
  }
  uint32_t pageIndex() const
  {
    return GrDrawOpAtlas::GetPageIndexFromID(fID);
  }
  MaskStyle maskStyle() const
  {
    return fMaskStyle;
  }
    // GetKey and Hash for the the hash table.
  static const SkPackedGlyphID& GetKey(const GrGlyph& glyph)
  {
    return glyph.fPackedID;
  }
  static uint32_t Hash(SkPackedGlyphID key)
  {
    return SkChecksum::Mix(key.hash());
  }
  const SkPackedGlyphID fPackedID;
  const GrMaskFormat fMaskFormat;
  const MaskStyle fMaskStyle;
  const GrIRect16 fBounds;
  SkIPoint16 fAtlasLocation{0, 0};
  GrDrawOpAtlas::AtlasID fID{GrDrawOpAtlas::kInvalidAtlasID};
};
#endif
