package com.microchip.apps.ezbl;

import java.io.Serializable;
import java.sql.ResultSet;
import java.sql.SQLException;








































public class EDCProperties
  implements Serializable, Cloneable
{
  String partNumber;
  EZBLState.CPUClass coreType = EZBLState.CPUClass.other;
  
  int programBlockSize;
  
  int eraseBlockSize;
  long devIDAddr;
  long devIDMask;
  long devID;
  long revIDAddr;
  long revIDMask;
  long dsNum;
  long dosNum;
  boolean hasFlashConfigWords;
  String BACKBUGRegName;
  long BACKBUGAddr;
  int BACKBUGPos;
  long BACKBUGMask;
  String BACKBUGAdjAffectedRegName;
  String ReservedBitRegName;
  long ReservedBitAddr;
  int ReservedBitPos;
  String ReservedBitAdjAffectedRegName;
  String CodeProtectRegName;
  long CodeProtectAddr;
  long CodeProtectMask;
  String dataOriginFile;
  long dataOriginDate;
  
  public EDCProperties() {}
  
  public EDCProperties(ResultSet dbData)
  {
    try
    {
      partNumber = dbData.getString("part");
      coreType = EZBLState.CPUClass.values()[dbData.getInt("CPUClass")];
      programBlockSize = dbData.getInt("programBlockSize");
      eraseBlockSize = dbData.getInt("eraseBlockSize");
      devIDAddr = dbData.getLong("devIDAddr");
      devIDMask = dbData.getLong("devIDMask");
      devID = dbData.getLong("devID");
      revIDAddr = dbData.getLong("revIDAddr");
      revIDMask = dbData.getLong("revIDMask");
      dsNum = dbData.getLong("dsNum");
      dosNum = dbData.getLong("dosNum");
      hasFlashConfigWords = dbData.getBoolean("hasFlashConfigWords");
      BACKBUGRegName = dbData.getString("BACKBUGRegName");
      BACKBUGAddr = dbData.getLong("BACKBUGAddr");
      BACKBUGPos = dbData.getInt("BACKBUGPos");
      BACKBUGMask = dbData.getLong("BACKBUGMask");
      BACKBUGAdjAffectedRegName = dbData.getString("BACKBUGAdjAffectedRegName");
      ReservedBitRegName = dbData.getString("ReservedBitRegName");
      ReservedBitAddr = dbData.getLong("ReservedBitAddr");
      ReservedBitPos = dbData.getInt("ReservedBitPos");
      ReservedBitAdjAffectedRegName = dbData.getString("ReservedBitAdjAffectedRegName");
      CodeProtectRegName = dbData.getString("CodeProtectRegName");
      CodeProtectAddr = dbData.getLong("CodeProtectAddr");
      CodeProtectMask = dbData.getLong("CodeProtectMask");
      dataOriginFile = dbData.getString("dataOriginFile");
      dataOriginDate = dbData.getLong("dataOriginDate");
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  
  public static EDCProperties fromCSVLine(String[] csvFields)
  {
    int i = 0;
    EDCProperties ret = new EDCProperties();
    

























    partNumber = (csvFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(csvFields[(i - 1)]));
    coreType = EZBLState.CPUClass.values()[Integer.decode(csvFields[(i++)]).intValue()];
    programBlockSize = Integer.decode(csvFields[(i++)]).intValue();
    eraseBlockSize = Integer.decode(csvFields[(i++)]).intValue();
    devIDAddr = Long.decode(csvFields[(i++)]).longValue();
    devIDMask = Long.decode(csvFields[(i++)]).longValue();
    devID = Long.decode(csvFields[(i++)]).longValue();
    revIDAddr = Long.decode(csvFields[(i++)]).longValue();
    revIDMask = Long.decode(csvFields[(i++)]).longValue();
    dsNum = Long.decode(csvFields[(i++)]).longValue();
    dosNum = Long.decode(csvFields[(i++)]).longValue();
    hasFlashConfigWords = (!csvFields[(i++)].equals("0"));
    BACKBUGRegName = (csvFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(csvFields[(i - 1)]));
    BACKBUGAddr = Long.decode(csvFields[(i++)]).longValue();
    BACKBUGPos = Integer.decode(csvFields[(i++)]).intValue();
    BACKBUGMask = Long.decode(csvFields[(i++)]).longValue();
    BACKBUGAdjAffectedRegName = (csvFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(csvFields[(i - 1)]));
    ReservedBitRegName = (csvFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(csvFields[(i - 1)]));
    ReservedBitAddr = Long.decode(csvFields[(i++)]).longValue();
    ReservedBitPos = Integer.decode(csvFields[(i++)]).intValue();
    ReservedBitAdjAffectedRegName = (csvFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(csvFields[(i - 1)]));
    CodeProtectRegName = (csvFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(csvFields[(i - 1)]));
    CodeProtectAddr = Long.decode(csvFields[(i++)]).longValue();
    CodeProtectMask = Long.decode(csvFields[(i++)]).longValue();
    dataOriginFile = (csvFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(csvFields[(i - 1)]));
    dataOriginDate = Long.decode(csvFields[(i++)]).longValue();
    return ret;
  }
  
  public static EDCProperties fromCSVLine(String csvData)
  {
    return fromCSVLine(csvData.split(","));
  }
  

  public EDCProperties clone()
  {
    try
    {
      return (EDCProperties)EDCProperties.class.cast(super.clone());
    }
    catch (CloneNotSupportedException e)
    {
      throw new RuntimeException(e);
    }
  }
}
