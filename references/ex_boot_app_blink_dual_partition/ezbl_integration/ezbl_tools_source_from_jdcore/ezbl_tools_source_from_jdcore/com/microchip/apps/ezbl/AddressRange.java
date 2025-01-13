package com.microchip.apps.ezbl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;























public class AddressRange
  implements Serializable, Cloneable, Comparable<AddressRange>
{
  static final long serialVersionUID = 1L;
  public long startAddr;
  public long endAddr;
  
  public AddressRange()
  {
    startAddr = 0L;
    endAddr = 0L;
  }
  
  public AddressRange(MemoryRegion mr)
  {
    startAddr = mr.getStartAddr();
    endAddr = mr.getEndAddr();
  }
  
  public AddressRange(long startAddr_inclusive, long endAddress_exclusive)
  {
    startAddr = startAddr_inclusive;
    endAddr = endAddress_exclusive;
  }
  
  public AddressRange(long startAddr_inclusive, int length)
  {
    startAddr = startAddr_inclusive;
    endAddr = (startAddr + length);
  }
  



  public AddressRange(String startAndEndAddr)
  {
    startAndEndAddr = Multifunction.TrimQuotes(startAndEndAddr);
    

    if (startAndEndAddr.contains("="))
    {
      startAndEndAddr = startAndEndAddr.replaceFirst(".*[=]", "");
    }
    

    startAndEndAddr = Multifunction.TrimQuotes(startAndEndAddr);
    

    startAndEndAddr = startAndEndAddr.replace("[", "");
    startAndEndAddr = startAndEndAddr.replace("(", "");
    startAndEndAddr = startAndEndAddr.replace("{", "");
    startAndEndAddr = startAndEndAddr.replace("]", "");
    startAndEndAddr = startAndEndAddr.replace(")", "");
    startAndEndAddr = startAndEndAddr.replace("}", "");
    

    startAndEndAddr = startAndEndAddr.trim();
    

    String[] addresses = startAndEndAddr.split("(\\s*,\\s*)|,|(\\s+)", 2);
    startAddr = Long.decode(addresses[0].trim()).longValue();
    endAddr = Long.decode(addresses[1].trim()).longValue();
  }
  

  public int compareTo(AddressRange y)
  {
    return startAddr > startAddr ? 1 : startAddr == startAddr ? 0 : -1;
  }
  

  public AddressRange clone()
  {
    try
    {
      return (AddressRange)super.clone();
    }
    catch (CloneNotSupportedException ex) {}
    
    return new AddressRange(startAddr, endAddr);
  }
  


  public String toString()
  {
    return String.format("[0x%08X, 0x%08X)", new Object[] { Long.valueOf(startAddr), Long.valueOf(endAddr) });
  }
  
  public MemoryRegion toMemoryRegion()
  {
    return new MemoryRegion(this);
  }
  
  public AddressRange PadAlign(int alignAddrSize)
  {
    if (alignAddrSize == 0)
      return this;
    startAddr -= startAddr % alignAddrSize;
    endAddr += (endAddr % alignAddrSize != 0L ? alignAddrSize - endAddr % alignAddrSize : 0L);
    return this;
  }
  
  public static void PadAlign(AddressRange ar, int alignAddrSize)
  {
    startAddr -= startAddr % alignAddrSize;
    endAddr += (endAddr % alignAddrSize != 0L ? alignAddrSize - endAddr % alignAddrSize : 0L);
  }
  
  public static List<AddressRange> PadAlign(Collection<AddressRange> addrRanges, int alignAddrSize)
  {
    for (AddressRange ar : addrRanges)
    {
      PadAlign(ar, alignAddrSize);
    }
    return (List)addrRanges;
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
  






  static void SortAndReduce(List<? extends AddressRange> list)
  {
    Collections.sort(list);
    for (int i = 0; i < list.size() - 1; i++)
    {
      AddressRange ar = (AddressRange)list.get(i);
      if (startAddr == endAddr)
      {
        list.remove(i--);
      }
      else {
        AddressRange ar2 = (AddressRange)list.get(i + 1);
        if ((startAddr >= startAddr) && (startAddr <= endAddr))
        {
          if (endAddr > endAddr)
          {
            endAddr = endAddr;
          }
          list.remove(i + 1);
          i--;

        }
        else if (startAddr == endAddr) {
          list.remove(ar2);
        }
      }
    }
  }
  













  static void SubtractRange(List<AddressRange> rangeList, AddressRange subtractRange)
  {
    for (int i = 0; i < rangeList.size(); i++)
    {
      AddressRange ar = (AddressRange)rangeList.get(i);
      if ((endAddr > startAddr) && (startAddr < endAddr))
      {
        if ((startAddr >= startAddr) && (endAddr < endAddr))
        {
          rangeList.add(i + 1, new AddressRange(endAddr, endAddr));
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
    }
  }
  




















  static List<AddressRange> SubtractRanges(List<AddressRange> referenceRangeList, List<AddressRange> subtractRangeList)
  {
    List<AddressRange> ret = new ArrayList();
    


    for (int i = 0; i < referenceRangeList.size(); i++)
    {
      ret.add(new AddressRange(getstartAddr, getendAddr));
    }
    
    Collections.sort(ret);
    for (int i = 0; i < ret.size(); i++)
    {
      AddressRange ar = (AddressRange)ret.get(i);
      for (int j = 0; j < subtractRangeList.size(); j++)
      {
        AddressRange ar2 = (AddressRange)subtractRangeList.get(j);
        if ((endAddr > startAddr) && (startAddr < endAddr))
        {
          if ((startAddr >= startAddr) && (endAddr < endAddr))
          {
            ret.add(i + 1, new AddressRange(endAddr, endAddr));
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
      }
    }
    

    SortAndReduce(ret);
    return ret;
  }
  



















  static List<AddressRange> SectionAddressUnion(List<AddressRange> addressRangeList, List<Section> sectionList)
  {
    List<AddressRange> sectionRanges = new ArrayList();
    
    for (Section sec : sectionList)
    {
      sectionRanges.add(new AddressRange(loadMemoryAddress, size));
    }
    
    return SubtractRanges(addressRangeList, sectionRanges);
  }
}
