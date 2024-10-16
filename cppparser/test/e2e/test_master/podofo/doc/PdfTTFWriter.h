/***************************************************************************
 *   Copyright (C) 2007 by Dominik Seichter                                *
 *   domseichter@web.de                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of portions of this program with the      *
 *   OpenSSL library under certain conditions as described in each         *
 *   individual source file, and distribute linked combinations            *
 *   including the two.                                                    *
 *   You must obey the GNU General Public License in all respects          *
 *   for all of the code used other than OpenSSL.  If you modify           *
 *   file(s) with this exception, you may extend this exception to your    *
 *   version of the file(s), but you are not obligated to do so.  If you   *
 *   do not wish to do so, delete this exception statement from your       *
 *   version.  If you delete this exception statement from all source      *
 *   files in the program, then also delete it here.                       *
 ***************************************************************************/
#ifndef _PDF_TTF_WRITER_H_
#  define _PDF_TTF_WRITER_H_
#  undef error "THIS SOURCE FILE WAS REPLACED BY PdfFontTTFSubset.h !"

#  include "PdfDefines.h"
#  include "PdfRefCountedBuffer.h"
namespace PoDoFo
{
  class PdfInputDevice;
  class PdfOutputDevice;
  namespace NonPublic
  {
/** An internal class which can parse a TrueType font file
 *  and write a subset of this TrueType font back to an output device.
 *
 *  This class is used internally to do font subsetting.
 *
 *  The usual way to use this class is:
 *
 *  PdfTTFWriter writer;
 *  writer.Read   ( [an input device]  ); // read the font from a device
 *  writer.Subset (                    ); // do the subsetting
 *  writer.Write  ( [an output device] ); // write the font back to a device
 */
    class PODOFO_API PdfTTFWriter
    {
    // Some common datatypes used in TTF files
      typedef pdf_uint32 pdf_ttf_fixed;
      typedef pdf_uint16 pdf_ttf_ushort;
      typedef pdf_int16 pdf_ttf_short;
      typedef pdf_uint32 pdf_ttf_ulong;
      typedef pdf_int16 pdf_ttf_fword;
      typedef pdf_uint16 pdf_ttf_ufword;
      typedef pdf_int16 pdf_ttf_f2dot14;
#  pragma  pack(1)
    /** The table dictionary is the starting point when reading 
     *  or writing a TTF file.
     */
      struct TTableDirectory
      {
        pdf_ttf_fixed sfnt_version;
        pdf_ttf_ushort numTables;
        pdf_ttf_ushort searchRange;
        pdf_ttf_ushort entrySelector;
        pdf_ttf_ushort rangeShift;
      };
      struct TTableDirectoryEntry
      {
        pdf_ttf_ulong tag;
        pdf_ttf_ulong checkSum;
        pdf_ttf_ulong offset;
        pdf_ttf_ulong length;
      };
      typedef std::vector<TTableDirectoryEntry> TVecTableDirectoryEntries;
      typedef TVecTableDirectoryEntries::iterator TIVecTableDirectoryEntries;
      typedef TVecTableDirectoryEntries::const_iterator TCIVecTableDirectoryEntries;
      struct TTable
      {
        TTable()
          : data(NULL)
        {
        }
        ~TTable()
        {
            /*
            if( data )
                free( data );
            */
        }
        pdf_ttf_ulong tag;
        pdf_ttf_ulong length;
        char* data;
      };
      typedef std::vector<TTable> TVecTable;
      typedef TVecTable::iterator TIVecTable;
      typedef TVecTable::const_iterator TCIVecTable;
      struct TMaxP
      {
        pdf_ttf_fixed version;
        pdf_ttf_ushort numGlyphs;
        pdf_ttf_ushort maxPoints;
        pdf_ttf_ushort maxContours;
        pdf_ttf_ushort maxCompositePoints;
        pdf_ttf_ushort maxCompositeContours;
        pdf_ttf_ushort maxZones;
        pdf_ttf_ushort maxTwilightPoints;
        pdf_ttf_ushort maxStorage;
        pdf_ttf_ushort maxFunctionsDefs;
        pdf_ttf_ushort maxInstructionDefs;
        pdf_ttf_ushort maxStackElements;
        pdf_ttf_ushort maxSizeOfInstruction;
        pdf_ttf_ushort maxComponentElements;
        pdf_ttf_ushort maxComponentDepth;
      };
      struct THead
      {
        pdf_ttf_fixed version;
        pdf_ttf_fixed revision;
        pdf_ttf_ulong checkSumAdjustment;
        pdf_ttf_ulong magicNumber;
        pdf_ttf_ushort flags;
        pdf_ttf_ushort unitsPerEm;
        char created[8];
        char modified[8];
        pdf_ttf_fword xMin;
        pdf_ttf_fword yMin;
        pdf_ttf_fword xMax;
        pdf_ttf_fword yMax;
        pdf_ttf_ushort macStyle;
        pdf_ttf_ushort lowestRecPPEM;
        pdf_ttf_short fontDirectionHint;
        pdf_ttf_short indexToLocForm;
        pdf_ttf_short glyphDataFormat;
      };
      struct TCMapEntry
      {
        pdf_ttf_ushort platformId;
        pdf_ttf_ushort encodingId;
        pdf_ttf_ulong offset;
      };
    /**
     * Header of a single glyph in the glyf table
     */
      struct TGlyphHeader
      {
        pdf_ttf_short numberOfContours;
        pdf_ttf_fword xMin;
        pdf_ttf_fword yMin;
        pdf_ttf_fword xMax;
        pdf_ttf_fword yMax;
      };
#  pragma  pack()
      class PdfTTFGlyph
      {
      public:
        /** Create a new glyph object.
         *
         *  \param nIndex glyph index.
         *  \param bComposite if true, this is a composite glyph
         *                    otherwise this object is simple glyph
         */
        PdfTTFGlyph(int nIndex)
          : m_nPosition(0)
          , m_nIndex(nIndex)
          , m_bComposite(false)
          , m_nInstructionLength(0)
          , m_pInstructions(NULL)
        {
          printf("m_nIndex=%i\n", m_nIndex);
        }
        PdfTTFGlyph(const PdfTTFGlyph& rhs)
        {
          operator=(rhs);
        }
        const PdfTTFGlyph& operator=(const PdfTTFGlyph& rhs)
        {
          m_nIndex = rhs.m_nIndex;
          m_bComposite = rhs.m_bComposite;
          m_tHeader = rhs.m_tHeader;
          m_nPosition = rhs.m_nPosition;
          m_nInstructionLength = rhs.m_nInstructionLength;
          m_pInstructions = rhs.m_pInstructions;
                // simple
          vecEndPoints = rhs.vecEndPoints;
          vecXCoordinates = rhs.vecXCoordinates;
          vecYCoordinates = rhs.vecYCoordinates;
          vecFlags = rhs.vecFlags;
          vecFlagsOrig = rhs.vecFlagsOrig;
                // composite
          arg1 = rhs.arg1;
          arg2 = rhs.arg2;
          xx = rhs.xx;
          yy = rhs.yy;
          xy = rhs.xy;
          yx = rhs.yx;
          m_buffer = rhs.m_buffer;
          return *this;
        }
        inline bool IsComposite() const
        {
          return m_bComposite;
        }
        inline void SetComposite(bool b)
        {
          m_bComposite = b;
        }
        inline int GetIndex() const
        {
          return m_nIndex;
        }
        inline int GetPosition() const
        {
          return m_nPosition;
        }
        inline void SetPosition(int nPos)
        {
          m_nPosition = nPos;
        }
        inline pdf_ttf_ushort GetInstrunctionLength() const
        {
          return m_nInstructionLength;
        }
        inline const char* GetInstrunctions() const
        {
          return m_pInstructions;
        }
      public:
        int m_nPosition;
        PdfRefCountedBuffer m_buffer;
        // common
        int m_nIndex;
        bool m_bComposite;
        TGlyphHeader m_tHeader;
        pdf_ttf_ushort m_nInstructionLength;
        char* m_pInstructions;
        // simple glyph
        std::vector<pdf_ttf_ushort> vecEndPoints;
        std::vector<pdf_ttf_short> vecXCoordinates;
        std::vector<pdf_ttf_short> vecYCoordinates;
        std::vector<unsigned char> vecFlags;
        std::vector<unsigned char> vecFlagsOrig;
        // composite
        pdf_ttf_short arg1;
        pdf_ttf_short arg2;
        pdf_ttf_short xx;
        pdf_ttf_short yy;
        pdf_ttf_short xy;
        pdf_ttf_short yx;
      };
#  pragma  pack(1)
      struct TCMapFormat4
      {
        pdf_ttf_ushort format;
        pdf_ttf_ushort length;
        pdf_ttf_ushort version;
        pdf_ttf_ushort segCountX2;
        pdf_ttf_ushort searchRange;
        pdf_ttf_ushort entrySelector;
        pdf_ttf_ushort rangeShift;
      };
      struct TCMapRange
      {
        pdf_ttf_ushort nStart;
        pdf_ttf_ushort nEnd;
        pdf_ttf_short nDelta;
        pdf_ttf_ushort nOffset;
        TCMapRange()
        {
        }
        TCMapRange(const TCMapRange& rhs)
        {
          this->operator=(rhs);
        }
        const TCMapRange& operator=(const TCMapRange& rhs)
        {
          nStart = rhs.nStart;
          nEnd = rhs.nEnd;
          nDelta = rhs.nDelta;
          nOffset = rhs.nOffset;
          return *this;
        }
        bool operator<(const TCMapRange& rhs) const
        {
          return nStart < rhs.nStart;
        }
      };
      typedef std::vector<PdfTTFGlyph> TVecGlyphs;
      typedef TVecGlyphs::iterator TIVecGlyphs;
      typedef TVecGlyphs::const_iterator TCIVecGlyphs;
      typedef std::vector<pdf_ttf_ulong> TVecLoca;
      typedef TVecLoca::iterator TIVecLoca;
      typedef TVecLoca::const_iterator TCIVecLoca;
      struct THHea
      {
        pdf_ttf_fixed version;
        pdf_ttf_fword ascender;
        pdf_ttf_fword descender;
        pdf_ttf_fword linegap;
        pdf_ttf_fword advanceWidthMax;
        pdf_ttf_fword minLeftSideBearing;
        pdf_ttf_fword minRightSideBearing;
        pdf_ttf_fword xMaxExtent;
        pdf_ttf_short caretSlopeRise;
        pdf_ttf_short caretSlopeRun;
        pdf_ttf_short reserved1;
        pdf_ttf_short reserved2;
        pdf_ttf_short reserved3;
        pdf_ttf_short reserved4;
        pdf_ttf_short reserved5;
        pdf_ttf_short metricDataFormat;
        pdf_ttf_ushort numberOfHMetrics;
      };
      struct TOs2
      {
        pdf_ttf_ushort version;
        pdf_ttf_short xAvgCharWidth;
        pdf_ttf_ushort usWeightClass;
        pdf_ttf_ushort usWidthClass;
        pdf_ttf_short fsType;
        pdf_ttf_short ySubscriptXSize;
        pdf_ttf_short ySubscriptYSize;
        pdf_ttf_short ySubscriptXOffset;
        pdf_ttf_short ySubscriptYOffset;
        pdf_ttf_short ySuperscriptXSize;
        pdf_ttf_short ySuperscriptYSize;
        pdf_ttf_short ySuperscriptXOffset;
        pdf_ttf_short ySuperscriptYOffset;
        pdf_ttf_short yStrikeoutSize;
        pdf_ttf_short yStrikeoutPosition;
        pdf_ttf_short sFamilyClass;
        char panose[10];
        pdf_ttf_ulong ulUnicodeRange1;
        pdf_ttf_ulong ulUnicodeRange2;
        pdf_ttf_ulong ulUnicodeRange3;
        pdf_ttf_ulong ulUnicodeRange4;
        char achVendID[4];
        pdf_ttf_ushort fsSelection;
        pdf_ttf_ushort usFirstCharIndex;
        pdf_ttf_ushort usLastCharIndex;
        pdf_ttf_ushort sTypoAscender;
        pdf_ttf_ushort sTypoDescender;
        pdf_ttf_ushort sTypoLineGap;
        pdf_ttf_ushort usWinAscent;
        pdf_ttf_ushort usWinDescent;
        pdf_ttf_ulong ulCodePageRange1;
        pdf_ttf_ulong ulCodePageRange2;
      };
      struct TLongHorMetric
      {
        pdf_ttf_ufword advanceWidth;
        pdf_ttf_fword leftSideBearing;
      };
      struct TNameTable
      {
        // header
        pdf_ttf_ushort format;
        pdf_ttf_ushort numRecords;
        pdf_ttf_ushort offset;
        // body
        pdf_ttf_ushort platformId;
        pdf_ttf_ushort encodingId;
        pdf_ttf_ushort languageId;
        pdf_ttf_ushort nameId;
        pdf_ttf_ushort stringLength;
        pdf_ttf_ushort stringOffset;
      };
    /** The postscript table
     */
      struct TPost
      {
        pdf_ttf_fixed format;
        pdf_ttf_fixed italicAngle;
        pdf_ttf_fword underlinePosition;
        pdf_ttf_fword underlineThickness;
        pdf_ttf_ulong isFixedPitch;
        pdf_ttf_ulong minMemType42;
        pdf_ttf_ulong maxMemType42;
        pdf_ttf_ulong minMemType1;
        pdf_ttf_ulong maxMemType1;
      };
#  pragma  pack()
    public:
    /** Create a PdfTTFWriter object.
     *  For testing purposes.
     *
     *  TODO: Remove
     */
      PdfTTFWriter();
      PdfTTFWriter(const std::vector<int>& rvecGlyphs);
      ~PdfTTFWriter();
    /** Fills the internal data structures
     *  using an existing TrueType font.
     *
     *  \param pDevice the TTF is read from this device
     */
      void Read(PdfInputDevice* pDevice);
    /** Do the actual subsetting of the font data
     *  TODO
     */
      void Subset();
    /** Write a TTF font from the current internal structures
     *  to an output device.
     *
     *  \param pDevice write the font to this device
     */
      void Write(PdfOutputDevice* pDevice);
    private:
    /** Create a tag name from four characters,
     *  so that the user readable tag can be put into
     *  TTableDirectoryEntry.
     *
     *  \returns the tag as a pdf_ttf_ulong
     */
      inline pdf_ttf_ulong CreateTag(char a, char b, char c, char d) const;
    /** Calculate the checksum of a table.
     *
     *  The table is interpreted as a byte stream of unsigned longs
     *  and has to be padded to a multiple of 4 bytes
     *
     *  \param pTable pointer to the beginning of the table
     *  \param lLength length of the table
     *
     *  \returns the checksum of the table
     */
      pdf_ttf_ulong CalculateChecksum(const pdf_ttf_ulong* pTable, pdf_ttf_ulong lLength) const;
    /** Convert a pdf_ttf_ushort between big and little endian
     *
     *  \param pShort a value to swap
     */
      inline void SwapUShort(pdf_ttf_ushort* pShort) const;
    /** Convert a pdf_ttf_short between big and little endian
     *
     *  \param pShort a value to swap
     */
      inline void SwapShort(pdf_ttf_short* pShort) const;
    /** Convert a pdf_ttf_fword between big and little endian
     *
     *  \param pFword a value to swap
     */
      inline void SwapFWord(pdf_ttf_fword* pFword) const;
    /** Convert a pdf_ttf_ulong between big and little endian
     *
     *  \param pShort a value to swap
     */
      inline void SwapULong(pdf_ttf_ulong* pLong) const;
    /** Reads the table directory from the current position
     *  of the input device, handling any necessary
     *  conversion from big to little endian.
     *
     *  \param pDevice read from the current position of this device.
     *
     *  \see m_tTableDirectory 
     */
      void ReadTableDirectory(PdfInputDevice* pDevice);
    /** Reads a table directory entry from the current position
     *  of the input device, handling any necessary
     *  conversion from big to little endian.
     *
     *  \param pDevice read from the current position of this device.
     *  \param pEntry store the result at this memory location
     */
      void ReadTableDirectoryEntry(PdfInputDevice* pDevice, TTableDirectoryEntry* pEntry);
    /** Writes a table directory entry at the current position
     *  of the output device, handling any necessary
     *  conversion from big to little endian.
     *
     *  \param pDevice write at the current position of this device.
     *  \param pEntry the entry which should be written
     */
      void WriteTableDirectoryEntry(PdfOutputDevice* pDevice, TTableDirectoryEntry* pEntry);
    /** Reads the head table from the current position of 
     *  the input device, handling any necessary conversion 
     *  from big to little endian.
     *
     *  \param pDevice read from the current position of this device.
     *
     *  \see m_tHead 
     */
      void ReadHeadTable(PdfInputDevice* pDevice);
    /** Swap the endianess of the head table.
     *  \see m_tHead
     */
      void SwapHeadTable();
      void ReadMaxpTable(PdfInputDevice* pDevice);
      void ReadLocaTable(PdfInputDevice* pDevice);
      void ReadHHeaTable(PdfInputDevice* pDevice);
      void ReadCmapTable(PdfInputDevice* pDevice);
      void ReadGlyfTable(PdfInputDevice* pDevice);
      void ReadOs2Table(PdfInputDevice* pDevice);
      void ReadHmtxTable(PdfInputDevice* pDevice);
      void ReadPostTable(PdfInputDevice* pDevice);
    /** Writes the table directory at the current position
     *  of the output device, handling any necessary
     *  conversion from big to little endian.
     *
     *  \param pDevice write at the current position of this device.
     *
     *  \see m_tTableDirectory 
     */
      void WriteTableDirectory(PdfOutputDevice* pDevice);
    /** Writes the head table at the current position
     *  of the output device, handling any necessary
     *  conversion from big to little endian.
     *
     *  \param pDevice write at the current position of this device.
     *
     *  \see m_tHead
     */
      void WriteHeadTable(PdfOutputDevice* pDevice);
    /** Writes the maxp table at the current position
     *  of the output device, handling any necessary
     *  conversion from big to little endian.
     *
     *  \param pDevice write at the current position of this device.
     *
     *  \see m_tMaxp
     */
      void WriteMaxpTable(PdfOutputDevice* pDevice);
      void WriteHHeaTable(PdfOutputDevice* pDevice);
      void WriteLocaTable(PdfOutputDevice* pDevice);
      void WriteCMapTable(PdfOutputDevice* pDevice);
      void WriteGlyfTable(PdfOutputDevice* pDevice);
      void WriteOs2Table(PdfOutputDevice* pDevice);
      void WriteNameTable(PdfOutputDevice* pDevice);
      void WriteHmtxTable(PdfOutputDevice* pDevice);
      void WritePostTable(PdfOutputDevice* pDevice);
    /** 
     *  Write a table to an output device and create a table directory for it
     *  with a correctly calculated checksum.
     *
     *  \param pDevice the output device on which the table should be written
     *  \param rToc add a table directory entry to this table directory.
     *  \param tag the tag of the table (e.g. 'name' or 'os/2').
     *  \param WriteTableFunc a member function pointer to the function that actually write the data
     *
     *  \see CreateTag
     */
      void WriteTable(PdfOutputDevice* pDevice, TVecTableDirectoryEntries& rToc, pdf_ttf_ulong tag, void (*WriteTableFunc) (PdfOutputDevice*));
      void SwapGlyfHeader(TGlyphHeader* pHeader);
    /** Swap the endianess of the maxp table.
     *  \see m_tMaxp
     */
      void SwapMaxpTable();
      void SwapHHeaTable();
      void SwapOs2Table();
      void SwapPostTable();
    /** Read the glyph coordinates from an input device.
     *
     *  \param pDevice read from this device
     *  \param rvecFlags a vector of flags describing the coordinates to load
     *         For each flag ONE coordinate is read. Not more, not less.
     *  \param rvecCoordinates store all coordinates in this vector
     *  \param nFlagShort the flag to use for x and y coordinates which determines a short coordinate
     *  \param nFlag the flag to use (0x10 for x coordinates and 0x20 for y coordinates)
     */
      void ReadSimpleGlyfCoordinates(PdfInputDevice* pDevice, const std::vector<unsigned char>& rvecFlags, std::vector<pdf_ttf_short>& rvecCoordinates, int nFlagShort, int nFlag);
      void WriteSimpleGlyfCoordinates(PdfOutputDevice* pDevice, const std::vector<unsigned char>& rvecFlags, std::vector<pdf_ttf_short>& rvecCoordinates, int nFlagShort, int nFlag);
    /** Get the offset to the location of the glyphs data.
     *
     *  \param nIndex unicode index of the glyph to load
     *  \param plLength pointer to an address where the length of the glyphdata can be stored
     *  \param pDevice an input device which can be used to read the CMap table which is required for certain glyphs
     *
     *  \return the offset to the glyph data or -1 if the glyph does not exist
     */
      long GetGlyphDataLocation(unsigned int nIndex, long* plLength, PdfInputDevice* pDevice) const;
    /** Load a glyph from an input device at a certain offset
     *
     *  \param nIndex the index of the glyph to load
     *  \param lOffset the offset at which the glyph is located in the file
     *  \param pDevice the input device to read from
     *
     */
      void LoadGlyph(int nIndex, long lOffset, PdfInputDevice* pDevice);
    private:
      long m_lGlyphDataOffset;
      long m_lCMapOffset;
      std::vector<int> m_vecGlyphIndeces;
      TTableDirectory m_tTableDirectory;
      TVecTable m_vecTableData;
      TMaxP m_tMaxp;
      THead m_tHead;
      THHea m_tHHea;
      TOs2 m_tOs2;
      TPost m_tPost;
      TVecLoca m_tLoca;
      TVecLoca m_vecLoca;
      TVecGlyphs m_vecGlyphs;
      std::vector<TCMapRange> m_ranges;
      TCMapFormat4 format4;
      std::vector<pdf_ttf_short> m_vecGlyphIds;
      std::vector<TLongHorMetric> m_vecHmtx;
      PdfRefCountedBuffer* m_pRefBuffer;
    };
// -----------------------------------------------------
// 
// -----------------------------------------------------
    inline PdfTTFWriter::pdf_ttf_ulong PdfTTFWriter::CreateTag(char a, char b, char c, char d) const
    {
      return ((a << 24) | (b << 16) | (c << 8) | d);
    }
// -----------------------------------------------------
// 
// -----------------------------------------------------
    inline void PdfTTFWriter::SwapUShort(pdf_ttf_ushort* pShort) const
    {
      *pShort = ((*pShort << 8) & 0xFF00) | ((*pShort >> 8) & 0x00FF);
    }
// -----------------------------------------------------
// 
// -----------------------------------------------------
    inline void PdfTTFWriter::SwapShort(pdf_ttf_short* pShort) const
    {
      *pShort = ((*pShort << 8) & 0xFF00) | ((*pShort >> 8) & 0x00FF);
    }
// -----------------------------------------------------
// 
// -----------------------------------------------------
    inline void PdfTTFWriter::SwapFWord(pdf_ttf_fword* pFword) const
    {
      *pFword = ((*pFword << 8) & 0xFF00) | ((*pFword >> 8) & 0x00FF);
    }
// -----------------------------------------------------
// 
// -----------------------------------------------------
    inline void PdfTTFWriter::SwapULong(pdf_ttf_ulong* pLong) const
    {
      *pLong = ((*pLong << 24) & 0xFF000000) | ((*pLong << 8) & 0x00FF0000) | ((*pLong >> 8) & 0x0000FF00) | ((*pLong >> 24) & 0x000000FF);
    }
  }
}
#endif
