package com.microchip.apps.ezbl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;



























public class MemoryRegion
  extends AddressRange
  implements Serializable, Cloneable, Comparable<AddressRange>
{
  static final long serialVersionUID = 1L;
  public String name = null;
  
  public int attribR;
  
  public int attribW;
  public int attribX;
  public int attribA;
  public int attribI;
  public String comment = null;
  
  public int programAlign;
  public int eraseAlign;
  
  public static enum Partition
  {
    single, 
    partition1, 
    partition2;
    private Partition() {} }
  public Partition partition = Partition.single;
  






  public static enum MemType
  {
    DEBUG, 
    SFR, 
    RAM, 
    ROM, 
    FLASHFUSE, 
    BYTEFUSE, 
    EEPROM, 
    OTP, 
    TEST;
    private MemType() {} }
  public MemType type = MemType.DEBUG;
  
  public static boolean MemTypeIsProgSpace(MemType mem)
  {
    return (mem.equals(MemType.BYTEFUSE)) || (mem.equals(MemType.EEPROM)) || (mem.equals(MemType.FLASHFUSE)) || (mem.equals(MemType.OTP)) || (mem.equals(MemType.ROM)) || (mem.equals(MemType.TEST));
  }
  
  public static boolean MemTypeIsDataSpace(MemType mem) {
    return (mem.equals(MemType.RAM)) || (mem.equals(MemType.SFR));
  }
  
  public static boolean MemTypeIsDebugSpace(MemType mem) {
    return mem.equals(MemType.DEBUG);
  }
  
  public static boolean MemSpaceEqual(MemType mem1, MemType mem2) {
    if (MemTypeIsDataSpace(mem1))
      return MemTypeIsDataSpace(mem2);
    if (MemTypeIsProgSpace(mem1))
      return MemTypeIsProgSpace(mem2);
    return MemTypeIsDebugSpace(mem2);
  }
  
  public boolean isProgSpace() {
    return MemTypeIsProgSpace(type);
  }
  
  public boolean isDataSpace() {
    return MemTypeIsDataSpace(type);
  }
  
  public boolean isDebugSpace() {
    return MemTypeIsDebugSpace(type);
  }
  
  public boolean memSpaceEqual(MemType mem) {
    return MemSpaceEqual(type, mem);
  }
  
  public boolean memSpaceEqual(MemoryRegion region) {
    return MemSpaceEqual(type, type);
  }
  

  public MemoryRegion clone()
  {
    MemoryRegion ret = (MemoryRegion)MemoryRegion.class.cast(super.clone());
    
    name = name;
    attribR = attribR;
    attribW = attribW;
    attribX = attribX;
    attribA = attribA;
    attribI = attribI;
    comment = comment;
    partition = partition;
    type = type;
    programAlign = programAlign;
    eraseAlign = eraseAlign;
    
    return ret;
  }
  

  public MemoryRegion() {}
  

  public MemoryRegion(long startAddr, long endAddr)
  {
    this.startAddr = startAddr;
    this.endAddr = endAddr;
  }
  
  public MemoryRegion(AddressRange ar)
  {
    startAddr = startAddr;
    endAddr = endAddr;
  }
  




  public MemoryRegion(String gldLine)
  {
    gldLine = gldLine.replaceAll("[\\s]", "");
    
    Pattern p = Pattern.compile("([\\w]+\\b)([(].*?[)])??[:].*?[=]([0-9xXa-fA-F]+[MKmk]??)[,].*?[=]([0-9xXa-fA-F]+[MKmk]??)", 32);
    Matcher m = p.matcher(gldLine);
    if (!m.find())
    {
      return;
    }
    name = m.group(1);
    String parseTemp = m.group(2);
    if (parseTemp != null)
    {
      parseTemp = parseTemp.toUpperCase();
      if (parseTemp.matches(".*R.*[!].*"))
      {
        attribR = -1;
      }
      else if (parseTemp.matches(".*R.*"))
      {
        attribR = 1;
      }
      if (parseTemp.matches(".*W.*[!].*"))
      {
        attribW = -1;
      }
      else if (parseTemp.matches(".*W.*"))
      {
        attribW = 1;
      }
      if (parseTemp.matches(".*X.*[!].*"))
      {
        attribX = -1;
      }
      else if (parseTemp.matches(".*X.*"))
      {
        attribX = 1;
      }
      if (parseTemp.matches(".*A.*[!].*"))
      {
        attribA = -1;
      }
      else if (parseTemp.matches(".*A.*"))
      {
        attribA = 1;
      }
      if (parseTemp.matches(".*(I|L).*[!].*"))
      {
        attribI = -1;
      }
      else if (parseTemp.matches(".*(I|L).*"))
      {
        attribI = 1;
      }
    }
    

    for (int i = 3; i <= 4; i++)
    {
      parseTemp = m.group(i).toUpperCase();
      Long multiplier; if (parseTemp.contains("M"))
      {
        Long multiplier = Long.valueOf(1048576L);
        parseTemp = parseTemp.substring(0, parseTemp.length() - 1);
      }
      else if (parseTemp.contains("K"))
      {
        Long multiplier = Long.valueOf(1024L);
        parseTemp = parseTemp.substring(0, parseTemp.length() - 1);
      }
      else
      {
        multiplier = Long.valueOf(1L);
      }
      
      if (i == 3) {
        startAddr = (Long.decode(parseTemp).longValue() * multiplier.longValue());
      } else {
        endAddr = (startAddr + Long.decode(parseTemp).longValue() * multiplier.longValue());
      }
    }
  }
  
  public static MemoryRegion fromCSVLine(String[] csvFieldData) {
    MemoryRegion mr = new MemoryRegion();
    








    int i = 2;
    type = MemType.values()[Integer.decode(csvFieldData[(i++)]).intValue()];
    partition = Partition.values()[Integer.decode(csvFieldData[(i++)]).intValue()];
    name = (csvFieldData[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(csvFieldData[(i - 1)]));
    startAddr = Long.decode(csvFieldData[(i++)]).longValue();
    endAddr = Long.decode(csvFieldData[(i++)]).longValue();
    programAlign = Integer.decode(csvFieldData[(i++)]).intValue();
    eraseAlign = Integer.decode(csvFieldData[(i++)]).intValue();
    return mr;
  }
  
  public static MemoryRegion fromCSVLine(String csvData)
  {
    return fromCSVLine(csvData.split("[,]"));
  }
  




  public MemoryRegion copyMetaData(MemoryRegion referenceRegion, boolean includeName, boolean includeComment)
  {
    attribA = attribA;
    attribI = attribI;
    attribR = attribR;
    attribW = attribW;
    attribX = attribX;
    eraseAlign = eraseAlign;
    if (includeComment)
      comment = comment;
    if (includeName)
      name = name;
    partition = partition;
    programAlign = programAlign;
    type = type;
    return this;
  }
  




  public MemoryRegion copyMetaData(MemoryRegion referenceRegion)
  {
    return copyMetaData(referenceRegion, true, true);
  }
  











  public MemoryRegion getOverlapWith(MemoryRegion y)
  {
    if ((y == null) || (startAddr >= endAddr) || (endAddr <= startAddr)) {
      return null;
    }
    MemoryRegion ret = clone();
    if ((startAddr <= startAddr) && (endAddr >= endAddr)) {
      ret = y.clone();
    }
    if (startAddr < startAddr)
      startAddr = startAddr;
    if (endAddr > endAddr)
      endAddr = endAddr;
    return ret;
  }
  

















  public MemoryRegion align(int alignAddrSize, List<MemoryRegion> overlapTestRegions)
  {
    if (alignAddrSize == 0) {
      return this;
    }
    long originalStart = startAddr;
    long originalEnd = endAddr;
    
    startAddr -= startAddr % alignAddrSize;
    long mod = endAddr % alignAddrSize;
    if (mod != 0L) {
      endAddr += alignAddrSize - mod;
    }
    if ((overlapTestRegions == null) || (overlapTestRegions.isEmpty())) {
      return this;
    }
    for (MemoryRegion boundaryRegion : overlapTestRegions)
    {
      if ((originalStart >= endAddr) && (startAddr < endAddr))
        startAddr = endAddr;
      if ((originalEnd <= startAddr) && (endAddr > startAddr))
        endAddr = startAddr;
      if ((originalStart >= startAddr) && (startAddr < startAddr))
        startAddr = startAddr;
      if ((originalEnd <= endAddr) && (endAddr > endAddr))
        endAddr = endAddr;
    }
    return this;
  }
  
  public MemoryRegion align(int alignAddrSize) {
    return align(alignAddrSize, null);
  }
  
  public static List<MemoryRegion> align(List<MemoryRegion> regions, int alignAddrSize)
  {
    return align(regions, alignAddrSize, null);
  }
  
  public static List<MemoryRegion> align(List<MemoryRegion> regions, int alignAddrSize, List<MemoryRegion> overlapTestRegions) {
    if (regions == null)
      return null;
    if (alignAddrSize == 0)
      return regions;
    for (MemoryRegion r : regions)
    {
      r.align(alignAddrSize, overlapTestRegions);
    }
    return regions;
  }
  
  public static List<MemoryRegion> alignToProgSize(List<MemoryRegion> regions, List<MemoryRegion> overlapTestRegions)
  {
    for (MemoryRegion r : regions)
    {
      r.align(programAlign, overlapTestRegions);
    }
    return regions;
  }
  
  public static List<MemoryRegion> alignToProgSize(List<MemoryRegion> regions) {
    return alignToProgSize(regions, null);
  }
  
  public static List<MemoryRegion> alignToEraseSize(List<MemoryRegion> regions, List<MemoryRegion> overlapTestRegions)
  {
    for (MemoryRegion r : regions)
    {
      r.align(eraseAlign, overlapTestRegions);
    }
    return regions;
  }
  
  public static List<MemoryRegion> alignToEraseSize(List<MemoryRegion> regions)
  {
    return alignToEraseSize(regions, null);
  }
  
  public MemoryRegion alignToProgSizeWithoutOverlap(List<MemoryRegion> overlapTestRegions)
  {
    return align(programAlign, overlapTestRegions);
  }
  
  public MemoryRegion alignToProgSize() {
    return align(programAlign, null);
  }
  
  public MemoryRegion alignToEraseSizeWithoutOverlap(List<MemoryRegion> overlapTestRegions)
  {
    return align(eraseAlign, overlapTestRegions);
  }
  
  public MemoryRegion alignToEraseSize() {
    return align(eraseAlign, null);
  }
  
  public static void SubtractRegion(List<MemoryRegion> regionList, MemoryRegion subtractRegion)
  {
    for (int i = 0; i < regionList.size(); i++)
    {
      MemoryRegion region = (MemoryRegion)regionList.get(i);
      if (region.memSpaceEqual(subtractRegion))
      {
        if ((endAddr > startAddr) && (startAddr < endAddr))
        {
          if ((startAddr >= startAddr) && (endAddr < endAddr))
          {
            MemoryRegion rightRegion = region.clone();
            startAddr = endAddr;
            regionList.add(i + 1, rightRegion);
            endAddr = startAddr;
          }
          else if ((startAddr >= startAddr) && (endAddr >= endAddr))
          {
            endAddr = startAddr;
          }
          else if ((startAddr <= startAddr) && (endAddr > startAddr))
          {
            startAddr = endAddr;
          }
          if (startAddr >= endAddr)
          {
            regionList.remove(i--);
          }
        }
      }
    }
  }
  
  public static List<MemoryRegion> SubtractRegions(List<MemoryRegion> regionList, MemoryRegion subtractRegion, boolean strictMemTypeMatchRequired) {
    return SubtractRegions(regionList, subtractRegion.getAsList(), strictMemTypeMatchRequired);
  }
  

  public static List<MemoryRegion> SubtractRegions(MemoryRegion referenceRegion, List<MemoryRegion> subtractRegionList, boolean strictMemTypeMatchRequired)
  {
    return SubtractRegions(referenceRegion.getAsList(), subtractRegionList, strictMemTypeMatchRequired);
  }
  















  public static List<MemoryRegion> SubtractRegions(List<MemoryRegion> referenceRegionList, List<MemoryRegion> subtractRegionList, boolean memTypeMatchRequired)
  {
    List<MemoryRegion> ret = coalesce(referenceRegionList, 0, 0, memTypeMatchRequired);
    if ((subtractRegionList == null) || (subtractRegionList.isEmpty())) {
      return ret;
    }
    for (int i = 0; i < ret.size(); i++)
    {
      MemoryRegion ref = (MemoryRegion)ret.get(i);
      for (int j = 0; j < subtractRegionList.size(); j++)
      {
        MemoryRegion sub = (MemoryRegion)subtractRegionList.get(j);
        if ((!memTypeMatchRequired) || (type == type))
        {
          if (ref.memSpaceEqual(sub))
          {
            if ((endAddr > startAddr) && (startAddr < endAddr))
            {
              if ((startAddr >= startAddr) && (endAddr < endAddr))
              {
                MemoryRegion rightRegion = ref.clone();
                startAddr = endAddr;
                ret.add(i + 1, rightRegion);
                endAddr = startAddr;
              }
              else if ((startAddr >= startAddr) && (endAddr >= endAddr))
              {
                endAddr = startAddr;
              }
              else if ((startAddr <= startAddr) && (endAddr > startAddr))
              {
                startAddr = endAddr;
              }
            }
            if (startAddr >= endAddr)
            {
              ret.remove(i--);
              break;
            }
          } }
      }
    }
    return ret;
  }
  
  public static List<MemoryRegion> coalesce(List<MemoryRegion> regions, boolean align, List<MemoryRegion> overlapTestRegions, boolean strictMemTypeMatchRequired)
  {
    List<MemoryRegion> ret = new ArrayList(regions.size());
    for (MemoryRegion mr : regions)
    {
      if (startAddr != endAddr)
      {
        MemoryRegion mr2 = mr.clone();
        if (align)
        {
          if (type == MemType.ROM) {
            mr2.align(eraseAlign, overlapTestRegions);
          } else if ((type == MemType.FLASHFUSE) || (type == MemType.BYTEFUSE) || (type == MemType.TEST))
            mr2.align(programAlign, overlapTestRegions);
        }
        ret.add(mr2);
      } }
    return coalesce(ret, 0, 0, strictMemTypeMatchRequired);
  }
  

  public static List<MemoryRegion> coalesce(List<MemoryRegion> regions, int leftAlign, int rightAlign, boolean strictMemTypeMatchNeeded)
  {
    List<MemoryRegion> refList = new ArrayList();
    


    for (int i = 0; i < regions.size(); i++)
    {
      refList.add(((MemoryRegion)regions.get(i)).clone());
    }
    
    if (leftAlign == 0)
      leftAlign = 1;
    if (rightAlign == 0)
      rightAlign = 1;
    Collections.sort(refList);
    

    for (int i = 0; i < refList.size() - 1; i++)
    {
      MemoryRegion mr = (MemoryRegion)refList.get(i);
      if (startAddr == endAddr)
      {
        refList.remove(i--);
      }
      else {
        startAddr -= startAddr % leftAlign;
        endAddr += (endAddr % rightAlign == 0L ? 0L : rightAlign - (endAddr + rightAlign) % rightAlign);
        for (int j = i + 1; j < refList.size(); j++)
        {
          MemoryRegion mr2 = (MemoryRegion)refList.get(j);
          if ((!strictMemTypeMatchNeeded) || (type == type))
          {
            if (mr.memSpaceEqual(mr2))
            {
              startAddr -= startAddr % leftAlign;
              if (endAddr < startAddr)
                break;
              if (endAddr > endAddr)
                endAddr = endAddr;
              refList.remove(j--);
              endAddr += (endAddr % rightAlign == 0L ? 0L : rightAlign - (endAddr + rightAlign) % rightAlign);
            } }
        }
      } }
    return refList;
  }
  

  public static List<MemoryRegion> coalesce(List<MemoryRegion> regions, boolean align, boolean strictMemTypeMatchRequired)
  {
    return coalesce(regions, align, null, strictMemTypeMatchRequired);
  }
  
  public int compareTo(MemoryRegion y)
  {
    return startAddr > startAddr ? 1 : startAddr == startAddr ? 0 : -1;
  }
  

  public int compareTo(AddressRange y)
  {
    return startAddr > startAddr ? 1 : startAddr == startAddr ? 0 : -1;
  }
  




  public String toString()
  {
    String nameAttributes = "    " + name + " ";
    String attributes = "";
    if (attribA == -1)
    {
      attributes = attributes + "a";
    }
    if (attribI == -1)
    {
      attributes = attributes + "i";
    }
    if (attribX == -1)
    {
      attributes = attributes + "x";
    }
    if (attribR == -1)
    {
      attributes = attributes + "r";
    }
    if (attribW == -1)
    {
      attributes = attributes + "w";
    }
    if (!attributes.isEmpty())
    {
      attributes = attributes + "!";
    }
    if (attribA == 1)
    {
      attributes = attributes + "a";
    }
    if (attribI == 1)
    {
      attributes = attributes + "i";
    }
    if (attribX == 1)
    {
      attributes = attributes + "x";
    }
    if (attribR == 1)
    {
      attributes = attributes + "r";
    }
    if (attribW == 1)
    {
      attributes = attributes + "w";
    }
    if (!attributes.isEmpty())
    {
      nameAttributes = nameAttributes + "(" + attributes + ") ";
    }
    
    if (comment == null)
    {
      return String.format("%1$-33s: ORIGIN = 0x%2$06X, LENGTH = 0x%3$06X", new Object[] { nameAttributes, Long.valueOf(startAddr), Long.valueOf(endAddr - startAddr) });
    }
    return String.format("%1$-33s: ORIGIN = 0x%2$06X, LENGTH = 0x%3$06X    /* %4$s */", new Object[] { nameAttributes, Long.valueOf(startAddr), Long.valueOf(endAddr - startAddr), comment });
  }
  
  public String toDebugString()
  {
    return String.format("[%06X, %06X): %s %s %s progAlign=0x%X, eraseAlign=0x%X\n", new Object[] { Long.valueOf(startAddr), Long.valueOf(endAddr), partition.toString(), type.toString(), name, Integer.valueOf(programAlign), Integer.valueOf(eraseAlign) });
  }
  















  public String toLinkerString(String... extraAttributes)
  {
    List<String> ret = new ArrayList();
    String secTypeName = isProgSpace() ? "ROM" : isDataSpace() ? "RAM" : "DBG";
    String secName = String.format("EZBL_%s_AT_0x%04X", new Object[] { secTypeName, Long.valueOf(startAddr) });
    if ((type == MemType.BYTEFUSE) || (type == MemType.FLASHFUSE))
      secName = "EZBL_BTLDR_CONFIG_WORD_" + name;
    long byteSize = endAddr - startAddr;
    if ((isProgSpace()) && ((endAddr & 0xFF000000) == 0L)) {
      byteSize = byteSize / 2L * 3L;
    }
    if (extraAttributes == null)
      extraAttributes = new String[0];
    String earlyAttribs = "";
    String lateAttribs = "";
    for (String attrib : extraAttributes)
    {
      if (attrib.toUpperCase().startsWith("AT")) {
        lateAttribs = lateAttribs + " " + attrib;
      } else {
        earlyAttribs = earlyAttribs + attrib + " ";
      }
    }
    ret.add(String.format("%s 0x%04X %s:%s", new Object[] { secName, Long.valueOf(startAddr), earlyAttribs, lateAttribs }));
    ret.add("{");
    ret.add(String.format("  *(%s); /* [0x%04X, 0x%04X), contains %d bytes */", new Object[] { secName, Long.valueOf(startAddr), Long.valueOf(endAddr), Long.valueOf(byteSize) }));
    ret.add(String.format("} > %s", new Object[] { name }));
    return "\n  " + Multifunction.CatStringList(ret, "\n  ");
  }
  
  public String toLinkerString() {
    return toLinkerString(new String[0]);
  }
  











  public String toASMString(String extraAttributes)
  {
    List<String> ret = new ArrayList();
    String secTypeName = isProgSpace() ? "ROM" : isDataSpace() ? "RAM" : "DBG";
    String secName = String.format("EZBL_%s_AT_0x%04X", new Object[] { secTypeName, Long.valueOf(startAddr) });
    String secAsmType = isProgSpace() ? "code" : isDataSpace() ? "persist" : "info";
    if ((type == MemType.BYTEFUSE) || (type == MemType.FLASHFUSE))
      secName = "EZBL_BTLDR_CONFIG_WORD_" + name;
    if (extraAttributes == null)
      extraAttributes = "";
    if (!extraAttributes.isEmpty())
      extraAttributes = extraAttributes + ", ";
    long byteSize = endAddr - startAddr;
    if ((isProgSpace()) && ((endAddr & 0xFF000000) == 0L)) {
      byteSize = byteSize / 2L * 3L;
    }
    ret.add(String.format("; Bootloader %s block intended for '%s' region", new Object[] { secTypeName, name }));
    ret.add(String.format("; 0x%04X to 0x%04X, length 0x%04X (%d bytes)", new Object[] { Long.valueOf(startAddr), Long.valueOf(endAddr), Long.valueOf(endAddr - startAddr), Long.valueOf(byteSize) }));
    ret.add(String.format(".pushsection    %s, address(0x%04X), %s, %skeep", new Object[] { secName, Long.valueOf(startAddr), secAsmType, extraAttributes }));
    ret.add(String.format(".space      0x%X", new Object[] { Long.valueOf(endAddr - startAddr) }));
    ret.add(String.format(".popsection", new Object[0]));
    return "\n    " + Multifunction.CatStringList(ret, "\n    ");
  }
  
  public String toASMString() {
    return toASMString(null);
  }
  
  void normalizePIC32Addresses()
  {
    if (((type == MemType.RAM) && ((startAddr & 0xFFC00000) == 0L)) || (((type == MemType.ROM) || (type == MemType.FLASHFUSE)) && ((startAddr & 0x7FC00000) == 486539264L)))
    {


      startAddr |= 0x80000000;
      endAddr |= 0x80000000;
    }
    if (((type != MemType.ROM) && (type != MemType.FLASHFUSE)) || (((startAddr & 0x7FC00000) == 532676608L) || ((type == MemType.SFR) && ((startAddr & 0x7FC00000) == 528482304L))))
    {

      startAddr |= 0xA0000000;
      endAddr |= 0xA0000000;
    }
  }
  
  public List<MemoryRegion> getAsList()
  {
    List<MemoryRegion> ret = new ArrayList(1);
    ret.add(this);
    return ret;
  }
  
  public String getName()
  {
    return name;
  }
  
  public void setName(String name)
  {
    this.name = name;
  }
  
  public int getAttribR()
  {
    return attribR;
  }
  
  public void setAttribR(int attribR)
  {
    this.attribR = attribR;
  }
  
  public int getAttribW()
  {
    return attribW;
  }
  
  public void setAttribW(int attribW)
  {
    this.attribW = attribW;
  }
  
  public int getAttribX()
  {
    return attribX;
  }
  
  public void setAttribX(int attribX)
  {
    this.attribX = attribX;
  }
  
  public int getAttribA()
  {
    return attribA;
  }
  
  public void setAttribA(int attribA)
  {
    this.attribA = attribA;
  }
  
  public int getAttribI()
  {
    return attribI;
  }
  
  public void setAttribI(int attribI)
  {
    this.attribI = attribI;
  }
  
  public long getStartAddr()
  {
    return startAddr;
  }
  
  public void setStartAddr(long startAddr)
  {
    this.startAddr = startAddr;
  }
  
  public long getEndAddr()
  {
    return endAddr;
  }
  
  public void setEndAddr(long endAddr)
  {
    this.endAddr = endAddr;
  }
  
  public String getComment()
  {
    return comment;
  }
  
  public void setComment(String comment)
  {
    this.comment = comment;
  }
}
