package com.microchip.apps.ezbl;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;
import java.io.PrintStream;
import java.io.Serializable;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;





















public class Section
  implements Serializable, Cloneable, Comparable<Section>
{
  static final long serialVersionUID = 1L;
  public int id = 0;
  public String name = "";
  public String combinedName = "";
  public String sourceFile = "";
  public long size = -1L;
  public long virtualMemoryAddress = -1L;
  public long loadMemoryAddress = -1L;
  public long fileOffset = 0L;
  public int alignment = 2;
  public SectionFlags flags = new SectionFlags();
  public List<Symbol> symbols = null;
  public Map<Long, Symbol> symbolsByAddr = null;
  public Map<String, Symbol> symbolsByName = null;
  public DataRecord data = null;
  public boolean isROM = false;
  public boolean isRAM = false;
  public boolean isDebug = false;
  public MemoryRegion mappedMemoryRegion = null;
  
  public Section Clone()
  {
    Section ret = new Section();
    
    alignment = alignment;
    combinedName = combinedName;
    if (data != null)
    {
      data = data.Clone();
    }
    fileOffset = fileOffset;
    flags = flags.Clone();
    id = id;
    isDebug = isDebug;
    isRAM = isRAM;
    isROM = isROM;
    loadMemoryAddress = loadMemoryAddress;
    name = name;
    size = size;
    sourceFile = sourceFile;
    if (symbols != null)
    {
      symbols = new ArrayList(symbols.size());
      symbolsByAddr = new HashMap(symbols.size());
      symbolsByName = new HashMap(symbols.size());
      for (int i = 0; i < symbols.size(); i++)
      {
        Symbol s = ((Symbol)symbols.get(i)).Clone();
        symbolsByAddr.put(Long.valueOf(address), s);
        symbolsByName.put(name, s);
        symbols.add(s);
      }
    }
    
    virtualMemoryAddress = virtualMemoryAddress;
    mappedMemoryRegion = (mappedMemoryRegion == null ? null : mappedMemoryRegion.clone());
    
    return ret;
  }
  



  public Section() {}
  



  public Section(String obj_dump_line)
  {
    try
    {
      String[] dump = obj_dump_line.split("[\\s]+?");
      List<String> allFlags = new ArrayList();
      int i = 0;
      for (String s : dump)
      {
        if (s.length() != 0)
        {

          switch (i++)
          {
          case 0: 
            id = Integer.decode(s).intValue();
            break;
          case 1: 
            name = s;
            break;
          case 2: 
            size = Long.parseLong(s, 16);
            break;
          case 3: 
            virtualMemoryAddress = Long.parseLong(s, 16);
            break;
          case 4: 
            loadMemoryAddress = Long.parseLong(s, 16);
            break;
          case 5: 
            fileOffset = Long.parseLong(s, 16);
            break;
          case 6: 
            alignment = (0x2 ^ Integer.parseInt(s.substring(s.lastIndexOf('*') + 1)));
            break;
          default: 
            s = s.toUpperCase();
            allFlags.add(s + " ");
            if (s.endsWith(","))
            {
              s = s.substring(0, s.length() - 1);
            }
            switch (s)
            {
            case "CONTENTS": 
              flags.CONTENTS = true;
              break;
            case "ALLOC": 
              flags.ALLOC = true;
              break;
            case "LOAD": 
              flags.LOAD = true;
              break;
            case "CODE": 
              flags.CODE = true;
              break;
            case "ABSOLUTE": 
              flags.ABSOLUTE = true;
              break;
            case "DEBUGGING": 
              flags.DEBUGGING = true;
              break;
            case "DATA": 
              flags.DATA = true;
              break;
            case "NEVER_LOAD": 
              flags.NEVER_LOAD = true;
              break;
            case "PERSIST": 
              flags.PERSIST = true;
              break;
            case "PSV": 
              flags.PSV = true;
              break;
            case "PAGE": 
              flags.PAGE = true;
              break;
            case "READONLY": 
              flags.READONLY = true;
              break;
            case "EDS": 
              flags.EDS = true;
              break;
            case "RELOC": 
              flags.RELOC = true;
              break;
            case "NEAR": 
              flags.NEAR = true;
              break;
            case "REVERSE": 
              flags.REVERSE = true;
              break;
            case "SECURE": 
              flags.SECURE = true;
              break;
            case "XMEMORY": 
              flags.XMEMORY = true;
              break;
            case "YMEMORY": 
              flags.YMEMORY = true;
              break;
            case "MEMORY": 
              flags.MEMORY = true;
              break;
            case "PACKEDFLASH": 
              flags.PACKEDFLASH = true;
              break;
            case "PRESERVED": 
              flags.PRESERVED = true;
              break;
            case "UPDATE": 
              flags.UPDATE = true;
              break;
            case "LINK_ONCE_SAME_SIZE": 
              flags.LINK_ONCE_SAME_SIZE = true;
              break;
            default: 
              SectionFlags tmp1379_1376 = flags;13791376unknown = (13791376unknown + "," + s);
            }
            break;
          }
        }
      }
      if (!flags.unknown.isEmpty())
      {
        flags.unknown = flags.unknown.substring(1);
      }
      flags.wholeString = Multifunction.CatStringList(allFlags);
      if (flags.wholeString.endsWith(" "))
      {
        flags.wholeString = flags.wholeString.substring(0, flags.wholeString.length() - 1);
      }
      
      isDebug = ((flags.DEBUGGING) || ((loadMemoryAddress == 0L) && (virtualMemoryAddress == 0L) && (size == 0L)) || (name.equals(".mdebug.abi32")) || (name.equals(".gnu.attributes")) || (name.equals(".reginfo")) || (name.equals(".comment")) || (name.equals(".info.EZBL_KeepSYM")));
      





      isROM = ((!flags.DEBUGGING) && (((flags.CODE) && (flags.READONLY)) || (flags.PSV) || (flags.PAGE) || (flags.PACKEDFLASH) || ((loadMemoryAddress & 0xFFC00000) == 486539264L) || ((loadMemoryAddress & 0xFFC00000) == 532676608L) || ((loadMemoryAddress & 0xFFC00000) == 2634022912L) || ((loadMemoryAddress & 0xFFC00000) == 2680160256L)));
      







      isRAM = ((!isROM) && ((flags.DATA) || (flags.NEAR) || (flags.EDS) || (flags.MEMORY) || (flags.PERSIST) || (flags.XMEMORY) || (flags.YMEMORY) || (flags.REVERSE) || ((loadMemoryAddress & 0xFFC00000) == 2147483648L) || ((loadMemoryAddress & 0xFFC00000) == 2684354560L)));
      









      if ((!isDebug) && (!isROM) && (!isRAM) && ((flags.CODE) || (name.startsWith(".text")) || (name.startsWith(".ivt")) || (name.startsWith(".const")) || (name.startsWith("reserve_boot_"))))
        isROM = true;
      if ((!isDebug) && (!isROM) && (!isRAM) && ((name.startsWith(".bss")) || (name.startsWith(".pbss")) || (name.startsWith(".nbss")) || (name.startsWith(".data")) || (name.startsWith("reserve_data_")) || ((flags.ALLOC) && (!flags.CONTENTS))))
        isRAM = true;
      if ((!isDebug) && (!isROM) && (!isRAM)) {
        isDebug = true;
      }
      
      if ((flags.READONLY) && (((loadMemoryAddress & 0x1FC00000) == 532676608L) || ((loadMemoryAddress & 0x1FC00000) == 486539264L)))
      {
        isRAM = false;
        isROM = true;
        flags.DATA = false;
        flags.CODE = true;
      }
    }
    catch (NumberFormatException ex)
    {
      name = null;
    }
  }
  










  public int compareTo(Section y)
  {
    return loadMemoryAddress == loadMemoryAddress ? 0 : loadMemoryAddress < loadMemoryAddress ? -1 : 1;
  }
  















  public MemoryRegion mapToDeviceRegion(List<MemoryRegion> deviceRegions, MemoryRegion.Partition sectionPartition)
  {
    MemoryRegion ret = new MemoryRegion(loadMemoryAddress, loadMemoryAddress + size);
    partition = sectionPartition;
    if (data != null)
    {
      name = data.assignedMemory;
      endAddr = data.getEndAddress();
    }
    if ((isDebug) || (isRAM) || (sectionPartition == null)) {
      sectionPartition = MemoryRegion.Partition.single;
    }
    if ((startAddr & 0xFFC00000) == 2684354560L)
    {
      startAddr ^= 0x20000000;
      endAddr ^= 0x20000000;
    }
    if ((startAddr & 0xFFC00000) == 486539264L)
    {
      startAddr |= 0x80000000;
      endAddr |= 0x80000000;
    }
    if (((startAddr & 0x7FC00000) == 532676608L) || ((startAddr & 0x7FC00000) == 528482304L))
    {

      startAddr |= 0xA0000000;
      endAddr |= 0xA0000000;
    }
    
    MemoryRegion.Partition p = sectionPartition;
    for (;;)
    {
      for (MemoryRegion mr : deviceRegions)
      {
        if ((startAddr < endAddr) && (endAddr > startAddr) && (partition == p))
        {

          if (((isDebug) && (mr.isDebugSpace())) || ((isRAM) && (mr.isDataSpace())) || ((isROM) && (mr.isProgSpace())))
          {
            ret.copyMetaData(mr);
            if ((data != null) && (data.getEndAddress() > endAddr))
              endAddr = endAddr;
            if (startAddr >= startAddr)
            {
              return ret;
            }
          }
        }
      }
      for (MemoryRegion mr : deviceRegions)
      {
        if (partition == p)
        {

          if (((isDebug) && (mr.isDebugSpace())) || ((isRAM) && (mr.isDataSpace())) || ((isROM) && (mr.isProgSpace())))
          {
            if ((endAddr > startAddr) && (startAddr < endAddr))
            {
              endAddr = startAddr;
            }
            if (startAddr == endAddr)
            {
              ret.copyMetaData(mr);
              comment = "no-device-mem-defined";
            }
          }
        }
      }
      if (isDebug)
        type = MemoryRegion.MemType.DEBUG;
      if ((isRAM) && (!ret.isDataSpace()))
      {
        type = MemoryRegion.MemType.RAM;
        eraseAlign = 1;
        programAlign = 1;
      }
      if ((isROM) && (!ret.isProgSpace())) {
        type = MemoryRegion.MemType.ROM;
      }
      if (p == MemoryRegion.Partition.single)
        break;
      p = MemoryRegion.Partition.single;
    }
    return ret;
  }
  





  public void normalizePIC32Addresses()
  {
    if ((loadMemoryAddress & 0xFFC00000) == 2684354560L)
    {
      loadMemoryAddress ^= 0x20000000;
    }
    if ((loadMemoryAddress & 0x7FC00000) == 486539264L)
    {
      loadMemoryAddress |= 0x80000000;
    }
    if ((loadMemoryAddress & 0x7FC00000) == 532676608L)
    {
      loadMemoryAddress |= 0xA0000000;
    }
    if (symbols != null)
    {
      for (Symbol s : symbols)
      {
        if ((address & 0xFFC00000) == 2684354560L)
        {
          address ^= 0x20000000;
        }
        if ((address & 0x7FC00000) == 486539264L)
        {
          address |= 0x80000000;
        }
        if ((address & 0x7FC00000) == 532676608L)
        {
          address |= 0xA0000000;
        }
      }
      for (Symbol s : symbolsByAddr.values())
      {
        symbolsByAddr.put(Long.valueOf(address), s);
      }
    }
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
  









  public static List<AddressRange> convertToAddressRanges(List<Section> sections)
  {
    List<AddressRange> ret = new ArrayList();
    for (Section sec : sections)
    {
      ret.add(new AddressRange(loadMemoryAddress, loadMemoryAddress + size));
    }
    return ret;
  }
  















  public static List<DataRecord> convertToDataRecords(List<Section> sections)
  {
    List<DataRecord> ret = new ArrayList();
    boolean architectureIs16Bit = false;
    



    for (Section sec : sections)
    {
      if (data != null)
      {
        if (data.architecture16Bit)
        {
          architectureIs16Bit = true;
          break;
        }
      }
    }
    

    for (Section sec : sections)
    {


      DataRecord dr = data;
      if (dr == null)
      {
        dr = new DataRecord();
        address = loadMemoryAddress;
        architecture16Bit = architectureIs16Bit;
        data = new byte[(int)((isROM) && (architecture16Bit) ? size / 2L * 3L : size)];
        Arrays.fill(data, (byte)-1);
      }
      
      ret.add(dr);
    }
    
    return ret;
  }
  



  public long getLoadMemoryAddress()
  {
    return loadMemoryAddress;
  }
  



  public void setLoadMemoryAddress(long loadMemoryAddress)
  {
    this.loadMemoryAddress = loadMemoryAddress;
  }
  



  public DataRecord getData()
  {
    return data;
  }
  



  public void setData(DataRecord data)
  {
    this.data = data;
  }
  













  public Section Split(long splitAddress)
  {
    if (loadMemoryAddress + size > splitAddress)
    {
      Section newRight = Clone();
      
      if (data != null)
      {
        data = data.SplitAtAddress(splitAddress);
      }
      size = (loadMemoryAddress + size - splitAddress);
      size -= size;
      loadMemoryAddress += size;
      virtualMemoryAddress += size;
      return newRight;
    }
    
    return null;
  }
  
  void LoadSymbols(List<Symbol> symbols)
  {
    this.symbols = new ArrayList();
    symbolsByAddr = new TreeMap();
    symbolsByName = new TreeMap();
    for (Symbol sym : symbols)
    {
      if (((data != null) && (!data.architecture16Bit)) || ((address & 0xFF000000) != 0L)) {
        sym.normalizePIC32Addresses();
      }
      if (section.equals(name))
      {
        if (name.matches("[_A-Za-z0-9]*$"))
        {
          symbolsByAddr.put(Long.valueOf(address), sym);
          symbolsByName.put(name, sym);
          this.symbols.add(sym);
        }
      }
      if (flags.debugging)
      {
        if (sourceFile == null)
        {
          if (address == virtualMemoryAddress)
          {
            sourceFile = probableFile;
          }
        }
      }
    }
  }
  
  void addSymbol(Symbol sym)
  {
    if (!symbols.contains(sym))
      symbols.add(sym);
    symbolsByAddr.put(Long.valueOf(address), sym);
    symbolsByName.put(name, sym);
  }
  
  void addAllSymbols(Collection<Symbol> symbols)
  {
    for (Symbol s : symbols)
    {
      addSymbol(s);
    }
  }
  















  boolean LoadSectionContents(String elfSectionDump)
  {
    List<DataRecord> lines = new ArrayList();
    


















    String searchString = "Contents of section " + name + ":\n";
    int possibleSectionStartIndex = 0;
    for (;;)
    {
      possibleSectionStartIndex = elfSectionDump.indexOf(searchString, possibleSectionStartIndex);
      if (possibleSectionStartIndex < 0) {
        break;
      }
      
      possibleSectionStartIndex += searchString.length();
      

      int endOffset = elfSectionDump.indexOf("\nC", possibleSectionStartIndex);
      if (endOffset < 0)
      {
        endOffset = elfSectionDump.length();
      }
      

      String sectionContents = elfSectionDump.substring(possibleSectionStartIndex, endOffset);
      

      int startIndex = 1;
      int eolIndex = 0;
      

      while (eolIndex >= 0)
      {
        int dataSeperatorIndex = sectionContents.indexOf(' ', startIndex + 1);
        if (dataSeperatorIndex < 0) {
          break;
        }
        
        int asciiPrintIndex = sectionContents.indexOf("  ", dataSeperatorIndex + 1);
        if (asciiPrintIndex < 0) {
          break;
        }
        
        int wordByteCount = (sectionContents.indexOf(' ', dataSeperatorIndex + 1) - dataSeperatorIndex) / 2;
        eolIndex = sectionContents.indexOf('\n', asciiPrintIndex + 2);
        long address = Integer.decode("0x" + sectionContents.substring(startIndex, dataSeperatorIndex)).intValue();
        startIndex = eolIndex + 2;
        

        if ((loadMemoryAddress > address) || (loadMemoryAddress + size < address)) {
          break;
        }
        


        String encodedData = sectionContents.substring(dataSeperatorIndex + 1, asciiPrintIndex);
        encodedData = encodedData.replaceAll(" ", "");
        int dataCount = encodedData.length() / 2;
        byte[] data = new byte[dataCount];
        for (int i = 0; i < dataCount; i++)
        {
          data[i] = ((byte)(Integer.decode("0x" + encodedData.substring(i * 2, i * 2 + 2)).intValue() & 0xFF));
        }
        
        DataRecord dr = new DataRecord(address, data, wordByteCount == 3);
        lines.add(dr);
      }
    }
    DataRecord.CoalesceRecords(lines);
    if (lines.isEmpty())
    {
      return false;
    }
    this.data = ((DataRecord)lines.get(0));
    return lines.size() == 1;
  }
  







  byte[] GetHash()
  {
    Section cleanSection = Clone();
    id = 0;
    fileOffset = 0L;
    sourceFile = null;
    combinedName = null;
    
    ByteArrayOutputStream byteStream = new ByteArrayOutputStream();
    ObjectOutput objectOut = null;
    try
    {
      objectOut = new ObjectOutputStream(byteStream);
      objectOut.writeObject(this);
      byte[] thisClassBytes = byteStream.toByteArray();
      objectOut.close();
      

      try
      {
        MessageDigest hashComputer = MessageDigest.getInstance("SHA-256");
        return hashComputer.digest(thisClassBytes);
      }
      catch (NoSuchAlgorithmException ex)
      {
        System.err.println("    EZBL: ERROR! Can't find 'SHA-256' hash algorithm. Make sure your JRE includes SHA-256 support.");
        return null;
      }
      


      return null;
    }
    catch (IOException ex) {}
  }
  
  public static List<MemoryRegion> getMappedMemoryRegions(List<Section> sectionList) {
    List<MemoryRegion> ret = new ArrayList();
    for (Section s : sectionList)
    {
      ret.add(mappedMemoryRegion);
    }
    return ret;
  }
  
  public AddressRange GetLoadAddressRange()
  {
    return new AddressRange(loadMemoryAddress, loadMemoryAddress + getSize());
  }
  
  public AddressRange GetVirtualAddressRange()
  {
    return new AddressRange(virtualMemoryAddress, virtualMemoryAddress + getSize());
  }
  
  public int getId()
  {
    return id;
  }
  
  public void setId(int id)
  {
    this.id = id;
  }
  
  public String getName()
  {
    return name;
  }
  
  public void setName(String name)
  {
    this.name = name;
  }
  
  public String getCombinedName()
  {
    return combinedName;
  }
  
  public void setCombinedName(String combinedName)
  {
    this.combinedName = combinedName;
  }
  
  public String getSourceFile()
  {
    return sourceFile;
  }
  
  public void setSourceFile(String sourceFile)
  {
    this.sourceFile = sourceFile;
  }
  
  public long getSize()
  {
    return size;
  }
  
  public void setSize(long size)
  {
    this.size = size;
  }
  
  public long getVirtualMemoryAddress()
  {
    return virtualMemoryAddress;
  }
  
  public void setVirtualMemoryAddress(long virtualMemoryAddress)
  {
    this.virtualMemoryAddress = virtualMemoryAddress;
  }
  
  public long getloadMemoryAddress()
  {
    return loadMemoryAddress;
  }
  
  public void setloadMemoryAddress(long loadMemoryAddress)
  {
    this.loadMemoryAddress = loadMemoryAddress;
  }
  
  public long getFileOffset()
  {
    return fileOffset;
  }
  
  public void setFileOffset(long fileOffset)
  {
    this.fileOffset = fileOffset;
  }
  
  public int getAlignment()
  {
    return alignment;
  }
  
  public void setAlignment(int alignment)
  {
    this.alignment = alignment;
  }
  
  public SectionFlags getFlags()
  {
    return flags;
  }
  
  public void setFlags(SectionFlags flags)
  {
    this.flags = flags;
  }
  
  public List<Symbol> getSymbols()
  {
    return symbols;
  }
  
  public void setSymbols(List<Symbol> symbols)
  {
    this.symbols = symbols;
  }
  
  public boolean isIsROM()
  {
    return isROM;
  }
  
  public void setIsROM(boolean isROM)
  {
    this.isROM = isROM;
  }
  
  public boolean isIsRAM()
  {
    return isRAM;
  }
  
  public void setIsRAM(boolean isRAM)
  {
    this.isRAM = isRAM;
  }
  
  public boolean isIsDebug()
  {
    return isDebug;
  }
  
  public void setIsDebug(boolean isDebug)
  {
    this.isDebug = isDebug;
  }
}
