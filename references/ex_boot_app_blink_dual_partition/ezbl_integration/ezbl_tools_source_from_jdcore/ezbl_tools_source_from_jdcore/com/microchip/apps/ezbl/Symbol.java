package com.microchip.apps.ezbl;

import java.io.PrintStream;
import java.io.Serializable;
import java.util.List;






























public class Symbol
  implements Serializable, Cloneable, Comparable<Symbol>
{
  static final long serialVersionUID = 1L;
  public long address = -1L;
  public SymbolFlags flags = null;
  public String section = null;
  public long alignment_size = -1L;
  public String name = null;
  public String probableFile = null;
  

  public Symbol() {}
  

  public Symbol(String obj_dump_line)
  {
    if ((obj_dump_line == null) || (obj_dump_line.length() < 18))
    {
      return;
    }
    

    try
    {
      address = Long.parseLong(obj_dump_line.substring(0, 8), 16);
      flags = new SymbolFlags(obj_dump_line.substring(9, 16));
      int alignSpaceOffset = obj_dump_line.indexOf(' ', 17);
      int alignOffset = obj_dump_line.indexOf('\t', 17);
      if ((alignOffset == -1) || ((alignSpaceOffset != 1) && (alignSpaceOffset < alignOffset)))
      {
        alignOffset = alignSpaceOffset;
      }
      section = obj_dump_line.substring(17, alignOffset).trim();
      while ((obj_dump_line.charAt(alignOffset) == ' ') || (obj_dump_line.charAt(alignOffset) == '\t'))
      {
        alignOffset++;
      }
      alignment_size = Long.parseLong(obj_dump_line.substring(alignOffset, alignOffset + 8), 16);
      name = obj_dump_line.substring(alignOffset + 9);
      int extraTokenEndIndex = name.lastIndexOf(' ');
      if (extraTokenEndIndex >= 0) {
        name = name.substring(extraTokenEndIndex + 1);
      }
    }
    catch (Exception ex) {
      System.out.printf("ezbl_tools.jar: unable to convert symbol line:\n                \"%1$s\"\n", new Object[] { obj_dump_line });
    }
  }
  


  public Symbol Clone()
  {
    Symbol ret = new Symbol();
    
    address = address;
    flags = flags.Clone();
    section = section;
    alignment_size = alignment_size;
    name = name;
    probableFile = probableFile;
    
    return ret;
  }
  
  public boolean equals(Symbol symbol, boolean testSection)
  {
    boolean match = (address == address) && (flags.equals(flags)) && (alignment_size == alignment_size) && (name.equals(name));
    if (!match)
    {
      return false;
    }
    if (!testSection)
    {
      return true;
    }
    return section.equals(section);
  }
  

  public int compareTo(Symbol y)
  {
    return address > address ? 1 : address == address ? 0 : -1;
  }
  
  public void normalizePIC32Addresses()
  {
    if ((address & 0xFFC00000) == 2684354560L)
    {
      address ^= 0x20000000;
    }
    if ((address & 0xFFC00000) == 486539264L)
    {
      address ^= 0x80000000;
    }
    if ((address & 0xFFC00000) == 532676608L)
    {
      address ^= 0xA0000000;
    }
  }
  










  public boolean isExportSuitable()
  {
    if (flags.local) {
      return false;
    }
    
    if ((flags.file) || (flags.relocatableEvaluated) || (flags.warning) || (flags.dynamic) || (flags.indirectReference)) {
      return false;
    }
    
    if (section.equals("*UND*")) {
      return false;
    }
    
    if ((section.equals("*ABS*")) && (!name.contains("EZBL"))) {
      return false;
    }
    
    if (!name.matches("[A-Za-z_][A-Za-z0-9_]*")) {
      return false;
    }
    
    if ((name.equals("EZBL_BOOT_PROJECT")) || (name.equals("EZBL_appBootloadState")) || (name.startsWith("__linked_")))
    {

      return false;
    }
    return true;
  }
  

  public String toString()
  {
    return String.format("%1$08X %2$s %3$s\t%4$08X %5$s\n", new Object[] { Long.valueOf(address), flags, section, Long.valueOf(alignment_size), name });
  }
  
  public boolean nameMatchesRegEx(List<String> regExpressions)
  {
    if (regExpressions == null) {
      return false;
    }
    for (String regEx : regExpressions)
    {
      if (name.matches(regEx)) {
        return true;
      }
    }
    return false;
  }
  
  public boolean nameMatches(List<String> literalList) {
    if (literalList == null) {
      return false;
    }
    for (String testName : literalList)
    {
      if (name.matches(testName)) {
        return true;
      }
    }
    return false;
  }
  
  public boolean nameStartsWith(List<String> literalList) {
    if (literalList == null) {
      return false;
    }
    for (String prefix : literalList)
    {
      if (name.startsWith(prefix)) {
        return true;
      }
    }
    return false;
  }
}
