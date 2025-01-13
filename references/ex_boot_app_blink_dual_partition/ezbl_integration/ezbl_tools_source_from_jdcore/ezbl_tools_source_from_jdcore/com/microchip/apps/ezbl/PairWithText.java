package com.microchip.apps.ezbl;

import java.io.Serializable;
import java.util.Comparator;


























public class PairWithText
  implements Serializable, Cloneable, Comparable<PairWithText>
{
  public long first = 0L;
  public long second = 0L;
  public String text = null;
  
  public PairWithText(AddressRange ar)
  {
    first = startAddr;
    second = endAddr;
  }
  
  public PairWithText(MemoryRegion mr)
  {
    first = mr.getStartAddr();
    second = mr.getEndAddr();
    text = name;
  }
  
  public PairWithText(AddressRange ar, String text)
  {
    first = startAddr;
    second = endAddr;
    this.text = text;
  }
  
  public PairWithText(long first, long second, String text)
  {
    this.first = first;
    this.second = second;
    this.text = text;
  }
  
  public PairWithText(long first, String text)
  {
    this.first = first;
    this.text = text;
  }
  
  public PairWithText(long first, long second)
  {
    this.first = first;
    this.second = second;
  }
  
  public PairWithText(String text)
  {
    this.text = text;
  }
  





  public PairWithText() {}
  




  public String toString()
  {
    return String.format("[0x%08X, 0x%08X] \"%s\"", new Object[] { Long.valueOf(first), Long.valueOf(second), text });
  }
  
  public long getFirst()
  {
    return first;
  }
  
  public void setFirst(long first)
  {
    this.first = first;
  }
  
  public long getSecond()
  {
    return second;
  }
  
  public void setSecond(long second)
  {
    this.second = second;
  }
  
  public String getText()
  {
    return text;
  }
  
  public void setText(String text)
  {
    this.text = text;
  }
  









  public int compareTo(PairWithText y)
  {
    return first == first ? 0 : first < first ? -1 : 1;
  }
  









  public int compareTo(AddressRange y)
  {
    return first == startAddr ? 0 : first < startAddr ? -1 : 1;
  }
  
  public static class PIC32MMBootFirstAddrComparator
    implements Comparator<PairWithText>
  {
    public PIC32MMBootFirstAddrComparator() {}
    
    public int compare(PairWithText x, PairWithText y)
    {
      long xStart = first & 0x1FFFFFFF;
      long yStart = first & 0x1FFFFFFF;
      

      if ((xStart & 0xFFFFFFFFFFC00000) == (yStart & 0xFFFFFFFFFFC00000)) {
        return first == first ? 0 : first < first ? -1 : 1;
      }
      
      if ((xStart & 0x1FC00000) == 528482304L)
        return -1;
      if ((yStart & 0x1FC00000) == 528482304L) {
        return 1;
      }
      if ((xStart & 0x1FC00000) == 0L)
        return -1;
      if ((yStart & 0x1FC00000) == 0L) {
        return 1;
      }
      if ((xStart & 0x1FC00000) == 532676608L)
        return -1;
      if ((yStart & 0x1FC00000) == 532676608L) {
        return 1;
      }
      if ((xStart & 0x1FC00000) == 486539264L)
        return -1;
      if ((yStart & 0x1FC00000) == 486539264L) {
        return 1;
      }
      
      return first == first ? 0 : first < first ? -1 : 1;
    }
  }
}
