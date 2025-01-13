package com.microchip.apps.ezbl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.TreeMap;




























public class DataRecord
  implements Serializable, Cloneable, Comparator<DataRecord>, Comparable<DataRecord>
{
  public boolean architecture16Bit = false;
  public long address = -1L;
  public byte[] data = null;
  public String assignedMemory = "";
  public String comment = "";
  
  public DataRecord Clone()
  {
    DataRecord ret = new DataRecord();
    
    address = address;
    architecture16Bit = architecture16Bit;
    assignedMemory = assignedMemory;
    comment = comment;
    if (data == null)
    {
      data = null;
    }
    else
    {
      data = Arrays.copyOf(data, data.length);
    }
    
    return ret;
  }
  

  public DataRecord() {}
  

  public DataRecord(boolean architchture16Bit)
  {
    architecture16Bit = architchture16Bit;
  }
  
  public DataRecord(long recordAddress, byte[] recordData, boolean architecture16Bit)
  {
    address = recordAddress;
    data = recordData;
    this.architecture16Bit = architecture16Bit;
  }
  











  public DataRecord(String elfDumpSectionDataLines)
  {
    int startIndex = 1;
    int eolIndex = 0;
    



    int bufUsed = 0;
    byte[] buf = new byte[elfDumpSectionDataLines.length() / 2];
    
    address = -1L;
    
    while (eolIndex >= 0)
    {
      int dataSeperatorIndex = elfDumpSectionDataLines.indexOf(' ', startIndex + 1);
      if (dataSeperatorIndex < 0) {
        break;
      }
      
      int asciiPrintIndex = elfDumpSectionDataLines.indexOf("  ", dataSeperatorIndex + 1);
      if (asciiPrintIndex < 0) {
        break;
      }
      
      int wordByteCount = (elfDumpSectionDataLines.indexOf(' ', dataSeperatorIndex + 1) - dataSeperatorIndex) / 2;
      eolIndex = elfDumpSectionDataLines.indexOf('\n', asciiPrintIndex + 2);
      if (address < 0L)
      {
        architecture16Bit = (wordByteCount == 3);
        
        try
        {
          address = Long.decode("0x" + elfDumpSectionDataLines.substring(startIndex, dataSeperatorIndex)).longValue();
        }
        catch (NumberFormatException ex)
        {
          return;
        }
      }
      startIndex = eolIndex + 2;
      

      String encodedData = elfDumpSectionDataLines.substring(dataSeperatorIndex + 1, asciiPrintIndex);
      encodedData = encodedData.replaceAll(" ", "");
      int dataCount = encodedData.length() / 2;
      if (dataCount > buf.length - bufUsed)
      {
        buf = Arrays.copyOf(buf, bufUsed + dataCount + 1024);
      }
      for (int i = 0; i < dataCount; i++)
      {
        buf[(bufUsed + i)] = ((byte)(Integer.decode("0x" + encodedData.substring(i * 2, i * 2 + 2)).intValue() & 0xFF));
      }
      bufUsed += dataCount;
    }
    
    data = Arrays.copyOf(buf, bufUsed);
  }
  



  public boolean equals(DataRecord dataRecord)
  {
    if ((architecture16Bit != architecture16Bit) || (address != address))
      return false;
    if ((data == null) && (data == null))
      return true;
    if (data == null)
      return false;
    if (data == null)
      return false;
    if (data.length != data.length) {
      return false;
    }
    return Arrays.equals(data, data);
  }
  

  public int compareTo(DataRecord y)
  {
    return (int)address > (int)address ? 1 : address == address ? 0 : -1;
  }
  

  public int compare(DataRecord x, DataRecord y)
  {
    return (int)address > (int)address ? 1 : address == address ? 0 : -1;
  }
  

  public String toString()
  {
    List<String> retStrings = new ArrayList();
    retStrings.add(String.format("16-bit: %s, Address: 0x%" + (architecture16Bit ? "06X" : "08X"), new Object[] { Boolean.valueOf(architecture16Bit), Long.valueOf(address) }));
    retStrings.add(String.format("assignedMemory: %s", new Object[] { assignedMemory }));
    retStrings.add(String.format("comment: %s", new Object[] { comment }));
    if (data != null)
    {
      retStrings.add(String.format("data: %d bytes", new Object[] { Integer.valueOf(data.length) }));
      for (int i = 0; i < data.length - 8; i += 8)
      {
        retStrings.add(String.format("%02X%02X%02X%02X %02X%02X%02X%02X", new Object[] { Byte.valueOf(data[(i + 0)]), Byte.valueOf(data[(i + 1)]), Byte.valueOf(data[(i + 2)]), Byte.valueOf(data[(i + 3)]), Byte.valueOf(data[(i + 4)]), Byte.valueOf(data[(i + 5)]), Byte.valueOf(data[(i + 6)]), Byte.valueOf(data[(i + 7)]) }));
      }
      String residual = "";
      for (int i = data.length - data.length % 8; i < data.length; i++)
      {
        residual = residual + String.format("%02X", new Object[] { Byte.valueOf(data[i]) });
        if ((i + 1) % 4 == 0)
          residual = residual + " ";
      }
      if (!residual.isEmpty()) {
        retStrings.add(residual);
      }
    }
    return Multifunction.CatStringList(retStrings, "\n");
  }
  
  public void normalizePIC32Addresses()
  {
    if (architecture16Bit) {
      return;
    }
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
  















  public MemoryRegion mapToDeviceRegion(List<MemoryRegion> deviceRegions, MemoryRegion.Partition expectedPartition)
  {
    MemoryRegion ret = new MemoryRegion(address, getEndAddress());
    type = MemoryRegion.MemType.ROM;
    if (data != null)
    {
      name = assignedMemory;
      endAddr = getEndAddress();
    }
    if (expectedPartition == null)
      expectedPartition = MemoryRegion.Partition.single;
    partition = expectedPartition;
    
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
    
    MemoryRegion.Partition p = expectedPartition;
    for (;;)
    {
      for (MemoryRegion mr : deviceRegions)
      {
        if ((startAddr < endAddr) && (endAddr > startAddr) && (partition == p))
        {

          if (mr.isProgSpace())
          {
            ret.copyMetaData(mr);
            if ((data != null) && (getEndAddress() > endAddr))
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

          if (mr.isProgSpace())
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
      if (p == MemoryRegion.Partition.single)
        break;
      p = MemoryRegion.Partition.single;
    }
    return ret;
  }
  






  public String getDataAsASMCode(List<Symbol> symExports)
  {
    if ((data == null) || (data.length == 0)) {
      return "";
    }
    List<String> ret = new ArrayList();
    long dAddr = address;
    




    char[] charForm = new char[24];
    

    if (architecture16Bit)
    {
      for (int i = 0; i <= data.length - 24; i += 24)
      {
        for (int k = 0; k < 24; k++)
        {
          charForm[k] = ((data[(i + k)] >= 32) && (data[(i + k)] <= 126) ? (char)data[(i + k)] : '.');
        }
        int[] instWords = Multifunction.BytesToInt24s(data, i, i + 24);
        ret.add(String.format("    .pword      0x%06X, 0x%06X, 0x%06X, 0x%06X, 0x%06X, 0x%06X, 0x%06X, 0x%06X  /* 0x%06X %s */", new Object[] { Integer.valueOf(instWords[0]), Integer.valueOf(instWords[1]), Integer.valueOf(instWords[2]), Integer.valueOf(instWords[3]), Integer.valueOf(instWords[4]), Integer.valueOf(instWords[5]), Integer.valueOf(instWords[6]), Integer.valueOf(instWords[7]), Long.valueOf(dAddr), new String(charForm) }));
        

        dAddr += 16L;
      }
      String residual = "    .pword      ";
      for (int j = 0; i <= data.length - 3; i += 3)
      {
        for (int k = 0; k < 3; k++)
        {
          charForm[(j++)] = ((data[(i + k)] >= 32) && (data[(i + k)] <= 126) ? (char)data[(i + k)] : '.');
        }
        residual = residual + String.format("0x%06X, ", new Object[] { Integer.valueOf(Multifunction.BytesToInt24s(data, i, i + 3)[0]) });
      }
      if (j != 0)
      {
        ret.add(String.format("%-95s /* 0x%06X %-24s */", new Object[] { residual.subSequence(0, residual.length() - 2), Long.valueOf(dAddr), new String(charForm).substring(0, j) }));
        dAddr += j / 3 * 2;
      }
      
      residual = "    .pbyte      ";
      for (j = 0; i < data.length; i++)
      {
        charForm[(j++)] = ((data[i] >= 32) && (data[i] <= 126) ? (char)data[i] : '.');
        residual = residual + String.format("0x%02X, ", new Object[] { Byte.valueOf(data[i]) });
      }
      if (j != 0) {
        ret.add(String.format("%-95s  /* 0x%06X %-24s */", new Object[] { residual.subSequence(0, residual.length() - 2), Long.valueOf(dAddr), new String(charForm).substring(0, j) }));
      }
    }
    else {
      long endAddr = getEndAddress();
      int i = 0;
      long lastSymAddr = dAddr - 1L;
      long lastSymSize = 0L;
      List<Symbol> lastSymsOpened = new ArrayList();
      Symbol lastSymEnt = null;
      TreeMap<Long, List<Symbol>> sortedSyms = new TreeMap();
      Long symStopAddr = null;
      
      if ((symExports != null) && (!symExports.isEmpty()))
      {
        for (Symbol s : symExports)
        {
          if (sortedSyms.containsKey(Long.valueOf(address)))
          {
            ((List)sortedSyms.get(Long.valueOf(address))).add(s);
          }
          else
          {
            List<Symbol> symsAtThisAddr = new ArrayList();
            symsAtThisAddr.add(s);
            sortedSyms.put(Long.valueOf(address), symsAtThisAddr);
          }
        }
      }
      
      while (dAddr < endAddr)
      {
        if ((symExports != null) && (!symExports.isEmpty()))
          symStopAddr = (Long)sortedSyms.higherKey(Long.valueOf(lastSymAddr));
        if (symStopAddr == null)
          symStopAddr = Long.valueOf(endAddr);
        if (((dAddr & 1L) == 1L) && (dAddr != symStopAddr.longValue()))
        {
          ret.add(String.format("    .byte       0x%02X", new Object[] { Byte.valueOf(data[i]) }));
          dAddr += 1L;
          i++;
        }
        if (((dAddr & 0x2) == 2L) && (dAddr != symStopAddr.longValue()))
        {
          ret.add(String.format("    .short      0x%04X", new Object[] { Integer.valueOf(Multifunction.BytesToInt16s(data, i, i + 2)[0]) }));
          dAddr += 2L;
          i += 2;
        }
        for (; dAddr <= symStopAddr.longValue() - 4L; i += 4)
        {
          ret.add(String.format("    .word       0x%08X", new Object[] { Integer.valueOf(Multifunction.BytesToInt32s(data, i, i + 4)[0]) }));dAddr += 4L;
        }
        for (; dAddr <= symStopAddr.longValue() - 2L; i += 2)
        {
          ret.add(String.format("    .short      0x%04X", new Object[] { Integer.valueOf(Multifunction.BytesToInt16s(data, i, i + 2)[0]) }));dAddr += 2L;
        }
        for (; dAddr < symStopAddr.longValue(); i++)
        {
          ret.add(String.format("    .byte       0x%02X", new Object[] { Byte.valueOf(data[i]) }));dAddr += 1L;
        }
        if ((sortedSyms != null) && (!sortedSyms.isEmpty()))
        {
          List<Symbol> symsAtAddress = (List)sortedSyms.get(symStopAddr);
          if (symsAtAddress != null)
          {
            for (Symbol lastSym : lastSymsOpened)
            {
              ret.add(String.format("    .size       %1$s, . - %1$s   # %2$d", new Object[] { name, Long.valueOf(lastSymSize) }));
              if ((lastSymEnt != null) && (lastSymEnt.equals(lastSym)))
                ret.add(String.format("    .end        %s", new Object[] { name }));
            }
            lastSymsOpened.clear();
            ret.add("");
            boolean mmNeedsPrinting = false;
            for (Symbol sym : symsAtAddress)
            {
              if (flags.function)
              {
                if (!mmNeedsPrinting)
                {
                  ret.add(String.format("    .ent        %s", new Object[] { name }));
                  lastSymEnt = sym;
                }
                ret.add(String.format("    .type       %s, @function", new Object[] { name }));
                Long symStopAddr2 = (Long)sortedSyms.higherKey(symStopAddr);
                if (symStopAddr2 == null)
                  symStopAddr2 = Long.valueOf(endAddr);
                lastSymSize = symStopAddr2.longValue() - symStopAddr.longValue();
                lastSymsOpened.add(0, sym);
                mmNeedsPrinting = true;
              }
              else if (flags.object)
              {
                ret.add(String.format("    .type       %s, @object", new Object[] { name }));
              }
              ret.add(String.format("%-31s # 0x%08X", new Object[] { name + ":", Long.valueOf(address) }));
            }
            if (mmNeedsPrinting)
            {
              ret.add(String.format("    .set        micromips", new Object[0]));
              ret.add(String.format("    .insn", new Object[0]));
            }
          }
        }
        
        lastSymAddr = symStopAddr.longValue();
      }
      
      for (Symbol lastSym : lastSymsOpened)
      {
        ret.add(String.format("    .size       %1$s, . - %1$s   # %2$d", new Object[] { name, Long.valueOf(lastSymSize) }));
        if ((lastSymEnt != null) && (lastSymEnt.equals(lastSym)))
          ret.add(String.format("    .end        %s", new Object[] { name }));
      }
      lastSymsOpened.clear();
    }
    
    return Multifunction.CatStringList(ret, "\n");
  }
  










  public DataRecord SplitAtAddress(long splitAtAddress)
  {
    DataRecord newRecord = new DataRecord(architecture16Bit);
    address = splitAtAddress;
    assignedMemory = assignedMemory;
    comment = comment;
    

    byte[] newBaseData = new byte[getDataIndexOfAddress(splitAtAddress)];
    if (newBaseData.length > data.length)
    {
      return null;
    }
    data = new byte[data.length - newBaseData.length];
    System.arraycopy(data, newBaseData.length, data, 0, data.length);
    

    System.arraycopy(data, 0, newBaseData, 0, newBaseData.length);
    data = newBaseData;
    
    return newRecord;
  }
  
  public boolean isArchitecture16Bit()
  {
    return architecture16Bit;
  }
  
  public void setArchitecture16Bit(boolean architecture16Bit)
  {
    this.architecture16Bit = architecture16Bit;
  }
  
  public long getAddress()
  {
    return address;
  }
  
  public void setAddress(long address)
  {
    this.address = address;
  }
  
  public byte[] getData()
  {
    return data;
  }
  
  public void setData(byte[] data)
  {
    this.data = data;
  }
  
  public String getAssignedMemory()
  {
    return assignedMemory;
  }
  
  public void setAssignedMemory(String assignedMemory)
  {
    this.assignedMemory = assignedMemory;
  }
  
  public String getComment()
  {
    return comment;
  }
  
  public void setComment(String comment)
  {
    this.comment = comment;
  }
  












  public static void SplitRecordsByLength(List<DataRecord> records, int maxRecordAddresses)
  {
    for (int i = 0; i < records.size(); i++)
    {
      DataRecord r = (DataRecord)records.get(i);
      if (r.getEndAddress() - address > maxRecordAddresses)
      {
        records.add(i + 1, r.SplitAtAddress(address + maxRecordAddresses));
      }
    }
  }
  
















  public static HashMap<Long, Byte[]> CombineData(List<DataRecord> records)
  {
    if (records == null) {
      return null;
    }
    HashMap<Long, Byte[]> map = new HashMap();
    for (int i = 0; i < records.size(); i++)
    {
      DataRecord dr = (DataRecord)records.get(i);
      if ((data == null) || (data.length == 0))
      {
        records.remove(i--);
      }
      else {
        int combineBlockByteLen = 4;
        int combineBlockAddrLen = 4;
        if (architecture16Bit)
        {
          combineBlockByteLen = 3;
          combineBlockAddrLen = 2;
        }
        for (long j = address; j < dr.getEndAddress(); j += combineBlockAddrLen)
        {
          Byte[] wordData = new Byte[combineBlockByteLen];
          int index = dr.getDataIndexOfAddress(j);
          for (int k = 0; k < combineBlockByteLen; k++)
          {
            if (index >= data.length)
            {
              wordData[k] = Byte.valueOf(-1);
            }
            else
              wordData[k] = Byte.valueOf(data[(index++)]);
          }
          if (map.containsKey(Long.valueOf(j)))
          {
            Byte[] existingData = (Byte[])map.get(Long.valueOf(j));
            for (int k = 0; k < existingData.length; k++)
            {
              existingData[k] = Byte.valueOf((byte)(existingData[k].byteValue() & wordData[k].byteValue()));
            }
            map.put(Long.valueOf(j), existingData);
          }
          else {
            map.put(Long.valueOf(j), wordData);
          }
        } } }
    return map;
  }
  







  public static void CoalesceRecords(List<DataRecord> records)
  {
    CoalesceRecords(records, true);
  }
  
















  public static void CoalesceRecords(List<DataRecord> records, boolean presort)
  {
    CoalesceRecords(records, presort, 0L, 0L);
  }
  



  public static void CoalesceRecords(List<DataRecord> records, boolean presort, long leftAlign, long rightAlign)
  {
    int coalesceSizeUsed = 0;
    



    if ((records == null) || (records.size() <= 1)) {
      return;
    }
    if (leftAlign == 0L)
      leftAlign = 1L;
    if (rightAlign == 0L) {
      rightAlign = 1L;
    }
    if (presort) {
      Collections.sort(records);
    }
    
    for (int i = 0; i < records.size() - 1; i++)
    {
      DataRecord dr = (DataRecord)records.get(i);
      if ((data == null) || (data.length == 0))
      {
        records.remove(i--);
      }
      else {
        long leftAlignAddrs = address % leftAlign;
        int leftAlignBytesNeeded = (int)(architecture16Bit ? leftAlignAddrs / 2L * 3L : leftAlignAddrs);
        if (leftAlignBytesNeeded != 0)
        {
          address -= leftAlignAddrs;
          data = Arrays.copyOfRange(data, leftAlignBytesNeeded, data.length + leftAlignBytesNeeded);
          Arrays.fill(data, 0, leftAlignBytesNeeded, (byte)-1);
        }
        if ((architecture16Bit) && (data.length % 3 != 0))
        {
          int oldLen = data.length;
          data = Arrays.copyOf(data, oldLen + data.length % 3);
          Arrays.fill(data, oldLen, data.length, (byte)-1);
        }
        long testNextAddr = address + (architecture16Bit ? data.length / 3 * 2 : data.length);
        long rightAlignAddrs = testNextAddr % rightAlign == 0L ? 0L : rightAlign - (testNextAddr + rightAlign) % rightAlign;
        coalesceSizeUsed = data.length;
        int bufLen = coalesceSizeUsed;
        int rightAlignBytesNeeded = (int)(architecture16Bit ? rightAlignAddrs / 2L * 3L : rightAlignAddrs);
        if (coalesceSizeUsed + rightAlignBytesNeeded > bufLen)
        {
          bufLen = coalesceSizeUsed + rightAlignBytesNeeded + 8192;
          bufLen -= bufLen % 8192;
          data = Arrays.copyOf(data, bufLen);
        }
        for (int j = i + 1; j < records.size(); j++)
        {
          DataRecord dr2 = (DataRecord)records.get(j);
          if ((architecture16Bit != architecture16Bit) || (testNextAddr + rightAlignAddrs < address - address % leftAlign))
            break;
          rightAlignAddrs = address - testNextAddr;
          if ((rightAlignAddrs < 0L) || (rightAlignAddrs > rightAlign + leftAlign))
            break;
          rightAlignBytesNeeded = (int)(architecture16Bit ? rightAlignAddrs / 2L * 3L : rightAlignAddrs);
          if (coalesceSizeUsed + rightAlignBytesNeeded + data.length > bufLen)
          {
            bufLen = coalesceSizeUsed + rightAlignBytesNeeded + data.length + 8192;
            bufLen -= bufLen % 8192;
            data = Arrays.copyOf(data, bufLen);
          }
          Arrays.fill(data, coalesceSizeUsed, coalesceSizeUsed + rightAlignBytesNeeded, (byte)-1);
          coalesceSizeUsed += rightAlignBytesNeeded;
          System.arraycopy(data, 0, data, coalesceSizeUsed, data.length);
          coalesceSizeUsed += data.length;
          records.remove(j--);
          testNextAddr = address + (architecture16Bit ? (coalesceSizeUsed + 2) / 3 * 2 : coalesceSizeUsed);
          rightAlignAddrs = testNextAddr % rightAlign == 0L ? 0L : rightAlign - (testNextAddr + rightAlign) % rightAlign;
        }
        rightAlignBytesNeeded = (int)(architecture16Bit ? rightAlignAddrs / 2L * 3L : rightAlignAddrs);
        if (rightAlignBytesNeeded > 0)
        {
          Arrays.fill(data, coalesceSizeUsed, coalesceSizeUsed + rightAlignBytesNeeded, (byte)-1);
          coalesceSizeUsed += rightAlignBytesNeeded;
        }
        
        if (bufLen != coalesceSizeUsed) {
          data = Arrays.copyOf(data, coalesceSizeUsed);
        }
      }
    }
  }
  





















  public static DataRecord ExtractRange(List<DataRecord> recordsToExtractFrom, long extractStartAddress, long extractEndAddress)
  {
    List<DataRecord> retRecords = ExtractRanges(recordsToExtractFrom, extractStartAddress, extractEndAddress);
    
    if ((retRecords == null) || (retRecords.isEmpty()))
    {
      return null;
    }
    if (retRecords.size() > 1)
    {
      Collections.sort(retRecords);
    }
    
    return (DataRecord)retRecords.get(0);
  }
  
  public static DataRecord ExtractRangeFromSections(List<Section> sectionsToExtractFrom, long extractStartAddress, long extractEndAddress)
  {
    if (sectionsToExtractFrom == null) {
      return null;
    }
    List<DataRecord> sectionData = new ArrayList();
    
    for (Section sec : sectionsToExtractFrom)
    {
      if (data != null)
      {
        sectionData.add(data);
      }
    }
    
    return ExtractRange(sectionData, extractStartAddress, extractEndAddress);
  }
  




























  public static List<DataRecord> ExtractRanges(List<DataRecord> recordsToExtractFrom, long extractStartAddress, long extractEndAddress)
  {
    List<DataRecord> retRecords = new ArrayList();
    
    if (recordsToExtractFrom == null)
    {
      return null;
    }
    
    for (int i = 0; i < recordsToExtractFrom.size(); i++)
    {
      DataRecord record = (DataRecord)recordsToExtractFrom.get(i);
      long recEnd = record.getEndAddress();
      
      if ((address < extractEndAddress) && (recEnd > extractStartAddress))
      {

        if ((address >= extractStartAddress) && (recEnd <= extractEndAddress))
        {
          recordsToExtractFrom.remove(record);
          retRecords.add(record);
          i--;

        }
        else if ((address <= extractStartAddress) && (recEnd >= extractEndAddress))
        {
          int leftBytes = architecture16Bit ? (int)((extractStartAddress - address) * 3L / 2L) : (int)(extractStartAddress - address);
          int rightBytes = architecture16Bit ? (int)((recEnd - extractEndAddress) * 3L / 2L) : (int)(recEnd - extractEndAddress);
          int midBytes = data.length - leftBytes - rightBytes;
          DataRecord right = record.Clone();
          DataRecord mid = record.Clone();
          address = extractStartAddress;
          data = Arrays.copyOfRange(data, leftBytes, leftBytes + midBytes);
          if (rightBytes > 0)
          {
            address = extractEndAddress;
            data = Arrays.copyOfRange(data, leftBytes + midBytes, leftBytes + midBytes + rightBytes);
            recordsToExtractFrom.add(right);
            i++;
          }
          if (leftBytes > 0)
          {
            data = Arrays.copyOf(data, leftBytes);
          }
          else
          {
            recordsToExtractFrom.remove(record);
            i--;
          }
          retRecords.add(mid);

        }
        else if (address < extractStartAddress)
        {
          int leftBytes = architecture16Bit ? (int)((extractStartAddress - address) * 3L / 2L) : (int)(extractStartAddress - address);
          int rightBytes = data.length - leftBytes;
          DataRecord right = record.Clone();
          data = Arrays.copyOf(data, leftBytes);
          address = extractStartAddress;
          data = Arrays.copyOfRange(data, leftBytes, leftBytes + rightBytes);
          retRecords.add(right);

        }
        else
        {
          int rightBytes = architecture16Bit ? (int)((recEnd - extractEndAddress) * 3L / 2L) : (int)(recEnd - extractEndAddress);
          int leftBytes = data.length - rightBytes;
          DataRecord left = record.Clone();
          data = Arrays.copyOfRange(data, leftBytes, leftBytes + rightBytes);
          address = extractEndAddress;
          data = Arrays.copyOfRange(data, 0, leftBytes);
          retRecords.add(left);
        }
      }
    }
    

    CoalesceRecords(retRecords);
    return retRecords;
  }
  










  public int GetIntDataAtAddress(long address)
  {
    int index = getDataIndexOfAddress(address);
    
    if (architecture16Bit)
    {
      if ((index < 0) || (index + 3 > data.length))
      {
        if (index + 2 == data.length)
          return data[index] & 0xFF | (data[(index + 1)] & 0xFF) << 8 | 0xFF0000;
        if (index + 1 == data.length)
          return data[index] & 0xFF | 0xFFFF00;
        return 16777215;
      }
      return data[index] & 0xFF | (data[(index + 1)] & 0xFF) << 8 | (data[(index + 2)] & 0xFF) << 16;
    }
    
    if ((index < 0) || (index + 4 > data.length))
    {
      if (index + 3 == data.length)
        return data[index] & 0xFF | (data[(index + 1)] & 0xFF) << 8 | (data[(index + 2)] & 0xFF) << 16 | 0xFF000000;
      if (index + 2 == data.length)
        return data[index] & 0xFF | (data[(index + 1)] & 0xFF) << 8 | 0xFFFF0000;
      if (index + 1 == data.length)
        return data[index] & 0xFF | 0xFF00;
      return -1;
    }
    return data[index] & 0xFF | (data[(index + 1)] & 0xFF) << 8 | (data[(index + 2)] & 0xFF) << 16 | (data[(index + 3)] & 0xFF) << 24;
  }
  






  public int GetShortDataAtAddress(long address)
  {
    int index = getDataIndexOfAddress(address);
    
    if ((index < 0) || (index > data.length - 2))
    {
      return 65535;
    }
    return data[index] & 0xFF | (data[(index + 1)] & 0xFF) << 8;
  }
  






  public long getEndAddress()
  {
    long ret = address + (architecture16Bit ? Blob.BytesToMCU16Addresses(data.length) : data.length);
    if ((address & 0xFFFF0000) == 4294901760L)
    {
      ret &= 0xFFFFFFFF;
    }
    return ret;
  }
  
  public int getDataIndexOfAddress(long address)
  {
    return architecture16Bit ? (int)((address - this.address) / 2L * 3L) : (int)(address - this.address);
  }
  
  public long getAddressOfDataIndex(int dataIndex)
  {
    return address + (architecture16Bit ? (dataIndex + 2) / 3 * 2 : dataIndex);
  }
  
  public AddressRange getAddressRange()
  {
    return new AddressRange(address, getEndAddress());
  }
  










  public static List<AddressRange> convertToAddressRanges(List<DataRecord> dataRecords)
  {
    List<AddressRange> ret = new ArrayList();
    for (DataRecord dr : dataRecords)
    {
      ret.add(new AddressRange(address, dr.getEndAddress()));
    }
    return ret;
  }
  

  public MemoryRegion getMemoryRegion()
  {
    MemoryRegion ret = new MemoryRegion(address, address);
    if (data != null)
    {
      endAddr += (architecture16Bit ? (data.length + 2) / 3 * 2 : data.length);
    }
    type = MemoryRegion.MemType.ROM;
    if (assignedMemory.equals("data"))
      type = MemoryRegion.MemType.RAM;
    comment = comment;
    return ret;
  }
}
