package com.microchip.apps.ezbl;

import java.io.PrintStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;




























public class GLDMemories
  implements Serializable, Cloneable
{
  public List<MemoryRegion> all = null;
  public List<MemoryRegion> nonvolatile = null;
  public List<MemoryRegion> data = null;
  public List<MemoryRegion> execution = null;
  public List<MemoryRegion> vectors = null;
  public List<MemoryRegion> eeprom = null;
  public List<MemoryRegion> other = null;
  public List<MemoryRegion> test = null;
  




  public GLDMemories(String gldFileData)
  {
    gldFileData = gldFileData.replaceAll("((?s)/[*].*?[*]/)|(//[^\n]*?\n)", "");
    

    Pattern p = Pattern.compile("\n[\\s]*MEMORY.*?\\{[\\s]*([\\s\\S]*?)\\}", 32);
    Matcher m = p.matcher(gldFileData);
    if (!m.find())
    {
      System.err.println("Couldn't find MEMORY region in linker script.");
      return;
    }
    
    all = new ArrayList();
    data = new ArrayList();
    nonvolatile = new ArrayList();
    execution = new ArrayList();
    vectors = new ArrayList();
    eeprom = new ArrayList();
    other = new ArrayList();
    test = new ArrayList();
    

    String[] memoryStrings = m.group(1).trim().split("\n");
    for (String memoryString : memoryStrings)
    {
      MemoryRegion mem = new MemoryRegion(memoryString);
      if (name != null)
      {
        all.add(mem);
      }
    }
    

    for (MemoryRegion mem : all)
    {
      if (attribA < 0)
      {
        data.add(mem);
      }
      else if (attribX > 0)
      {
        nonvolatile.add(mem);
        execution.add(mem);
      }
      else if ((startAddr < 2048L) || (name.equals("reset")) || (name.equals("ivt")) || (name.equals("aivt")) || (name.equals("_reserved")))
      {
        nonvolatile.add(mem);
        vectors.add(mem);
      }
      else if (name.equalsIgnoreCase("eeprom"))
      {
        nonvolatile.add(mem);
        eeprom.add(mem);
      }
      else if ((startAddr & 0x800000) != 0L)
      {
        nonvolatile.add(mem);
        test.add(mem);
      }
      else
      {
        nonvolatile.add(mem);
        other.add(mem);
      }
    }
  }
  

  public String toString()
  {
    String s = "\nMEMORY\n{\n";
    

    for (MemoryRegion mem : all)
    {
      s = s + mem.toString() + "\n";
    }
    s = s + "}\n";
    
    return s;
  }
  



  public List<MemoryRegion> getAll()
  {
    return all;
  }
  



  public void setAll(List<MemoryRegion> all)
  {
    this.all = all;
  }
  



  public List<MemoryRegion> getNonvolatile()
  {
    return nonvolatile;
  }
  



  public void setNonvolatile(List<MemoryRegion> nonvolatile)
  {
    this.nonvolatile = nonvolatile;
  }
  



  public List<MemoryRegion> getData()
  {
    return data;
  }
  



  public void setData(List<MemoryRegion> data)
  {
    this.data = data;
  }
  



  public List<MemoryRegion> getExecution()
  {
    return execution;
  }
  



  public void setExecution(List<MemoryRegion> execution)
  {
    this.execution = execution;
  }
  



  public List<MemoryRegion> getVectors()
  {
    return vectors;
  }
  



  public void setVectors(List<MemoryRegion> vectors)
  {
    this.vectors = vectors;
  }
  



  public List<MemoryRegion> getEeprom()
  {
    return eeprom;
  }
  



  public void setEeprom(List<MemoryRegion> eeprom)
  {
    this.eeprom = eeprom;
  }
  



  public List<MemoryRegion> getOther()
  {
    return other;
  }
  



  public void setOther(List<MemoryRegion> other)
  {
    this.other = other;
  }
  



  public List<MemoryRegion> getTest()
  {
    return test;
  }
  



  public void setTest(List<MemoryRegion> test)
  {
    this.test = test;
  }
}
