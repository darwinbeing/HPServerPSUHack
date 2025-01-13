package com.microchip.apps.ezbl;

import java.io.Serializable;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Collection;








































public class InterruptVector
  implements Serializable, Comparable<InterruptVector>
{
  int vectorNum = 0;
  int irqNum = -8;
  String name = null;
  String desc = null;
  boolean trap = false;
  boolean implemented = false;
  long remapAddress = -1L;
  

  public InterruptVector() {}
  

  public InterruptVector(ResultSet dbData)
  {
    try
    {
      String parts = dbData.getString("parts").toLowerCase();
      vectorNum = dbData.getInt("vector");
      name = dbData.getString("name");
      implemented = dbData.getBoolean("implemented");
      trap = dbData.getBoolean("trap");
      desc = dbData.getString("desc");
      irqNum = ((parts.contains("dspic")) || (parts.contains("pic24")) ? vectorNum - 8 : vectorNum);
      remapAddress = -1L;
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  







  public static InterruptVector fromCSVLine(String[] csvFields)
  {
    InterruptVector ret = new InterruptVector();
    vectorNum = Integer.decode(csvFields[2]).intValue();
    irqNum = ((csvFields[1].contains("dsPIC")) || (csvFields[1].contains("PIC24")) ? vectorNum - 8 : vectorNum);
    name = (csvFields[3].isEmpty() ? null : Multifunction.TrimQuotes(csvFields[3]));
    implemented = (!csvFields[4].equals("0"));
    trap = (!csvFields[5].equals("0"));
    desc = (csvFields[6].isEmpty() ? null : Multifunction.TrimQuotes(csvFields[6]));
    return ret;
  }
  
  public static InterruptVector fromCSVLine(String csvLine)
  {
    return fromCSVLine(csvLine.split("[,]"));
  }
  
  public InterruptVector(String name, int vectorNum, boolean implemented, boolean trap, String description)
  {
    desc = description;
    this.name = name;
    this.implemented = implemented;
    remapAddress = -1L;
    this.trap = trap;
    irqNum = (vectorNum - 8);
    this.vectorNum = vectorNum;
  }
  







  public int compareTo(InterruptVector vec)
  {
    return vectorNum < vectorNum ? -1 : vectorNum > vectorNum ? 1 : 0;
  }
  


  public static int GetImplementedVectorCount(Collection<InterruptVector> vectors)
  {
    int ret = 0;
    for (InterruptVector iv : vectors)
    {
      if (implemented)
        ret++;
    }
    return ret;
  }
}
