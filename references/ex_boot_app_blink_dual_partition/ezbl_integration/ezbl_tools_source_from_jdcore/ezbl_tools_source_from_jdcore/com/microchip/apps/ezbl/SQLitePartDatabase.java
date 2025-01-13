package com.microchip.apps.ezbl;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.nio.file.CopyOption;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map.Entry;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
















public class SQLitePartDatabase
{
  private String dbFilename;
  private Connection db;
  private Long sfrID;
  CachedSFRs sfrCache;
  
  private class CachedSFRs
  {
    private TreeMap<String, EDCReader.EZBL_SFR> sfrsByName = new TreeMap();
    private TreeMap<Long, EDCReader.EZBL_SFR> sfrsByAddr = new TreeMap();
    private String partNum = "";
    
    private CachedSFRs() {}
  }
  
  public SQLitePartDatabase()
  {
    dbFilename = null;
    db = null;
    sfrID = null;
    







    sfrCache = new CachedSFRs(null);
  }
  
  public static enum MemoryTable
  {
    DeviceMemories, 
    GLDMemories, 
    BootloadableRanges, 
    DecodedConfigMemories;
    







    private MemoryTable() {}
  }
  






  public String exportTable(String tableName, String orderBySQL)
  {
    List<String> outputText = new ArrayList();
    if (orderBySQL == null)
      orderBySQL = "";
    ResultSet rs = execQuery("SELECT * FROM " + tableName + " " + orderBySQL + ";", new Object[0]);
    try
    {
      ResultSetMetaData meta = rs.getMetaData();
      for (int i = 1; i <= meta.getColumnCount(); i++)
      {
        outputText.add(meta.getColumnLabel(i));
        outputText.add(",");
      }
      outputText.set(outputText.size() - 1, "\n");
      while (rs.next())
      {
        for (int i = 1; i <= meta.getColumnCount(); i++)
        {
          Object colData = rs.getObject(i);
          if (colData != null)
          {
            if (String.class.isInstance(colData)) {
              outputText.add("\"" + colData.toString() + "\"");
            } else
              outputText.add(colData.toString());
          }
          outputText.add(",");
        }
        outputText.set(outputText.size() - 1, "\n");
      }
      outputText.remove(outputText.size() - 1);
    }
    catch (SQLException ex)
    {
      throw new RuntimeException(ex);
    }
    
    return Multifunction.CatStringList(outputText);
  }
  











  public String extractJarResourceToTempFile(String jarResourcePath)
    throws RuntimeException
  {
    InputStream resourceStream = null;
    String tempExtractedFilePath = null;
    resourceStream = ClassLoader.getSystemClassLoader().getResourceAsStream(jarResourcePath);
    if (resourceStream == null)
    {
      throw new RuntimeException("Cannot read parts database from Jar file");
    }
    
    try
    {
      File dbLocalJarPath = new File(jarResourcePath);
      File f = File.createTempFile(dbLocalJarPath.getName(), "");
      f.deleteOnExit();
      tempExtractedFilePath = f.getCanonicalPath();
      Files.copy(resourceStream, f.toPath(), new CopyOption[] { StandardCopyOption.REPLACE_EXISTING });
      resourceStream.close();
    }
    catch (IOException e)
    {
      throw new RuntimeException(e);
    }
    
    return tempExtractedFilePath;
  }
  







  public void openDatabase(String filename)
    throws RuntimeException
  {
    if (db != null) {
      saveDatabase(true);
    }
    try
    {
      db = DriverManager.getConnection("jdbc:sqlite:");
      if ((filename != null) && (Multifunction.FileExists(filename)))
      {
        dbFilename = filename;
        exec("restore from '" + filename + "'", new Object[0]);
      }
      initTables();
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  






  public void openDatabase()
    throws RuntimeException
  {
    openDatabase(null);
  }
  











  public void saveDatabase(boolean closeAfterSave)
    throws RuntimeException
  {
    if (db == null) {
      return;
    }
    String pathToSources = "src/com/microchip/apps/ezbl/";
    
    Multifunction.UpdateFileIfDataDifferent(pathToSources + "resources/DeviceProperties.txt", exportTable("DeviceProperties", "ORDER BY part ASC"), false);
    Multifunction.UpdateFileIfDataDifferent(pathToSources + "resources/DeviceMemories.txt", exportTable("DeviceMemories", "ORDER BY part ASC, partition ASC, type ASC, startAddr ASC"), false);
    Multifunction.UpdateFileIfDataDifferent(pathToSources + "resources/DecodedConfigMemories.txt", exportTable("DecodedConfigMemories", "ORDER BY part ASC, partition ASC, type ASC, startAddr ASC"), false);
    Multifunction.UpdateFileIfDataDifferent(pathToSources + "resources/GLDMemories.txt", exportTable("GLDMemories", "ORDER BY part ASC, partition ASC, type ASC, startAddr ASC"), false);
    Multifunction.UpdateFileIfDataDifferent(pathToSources + "resources/BootloadableRanges.txt", exportTable("BootloadableRanges", "ORDER BY part ASC, partition ASC, type ASC, startAddr ASC"), false);
    Multifunction.UpdateFileIfDataDifferent(pathToSources + "resources/Interrupts.txt", exportTable("Interrupts", "ORDER BY parts ASC, vector ASC"), false);
    Multifunction.UpdateFileIfDataDifferent("../SFRs.txt", exportTable("SFRs", "ORDER BY parts ASC, address ASC, name ASC"), false);
    Multifunction.UpdateFileIfDataDifferent("../Bitfields.txt", exportTable("Bitfields", "ORDER BY sfrid ASC, position ASC, name ASC"), false);
    
    compressPartGroupings();
    
    exec("backup to '" + dbFilename + "'", new Object[0]);
    if (closeAfterSave)
    {
      try
      {
        db.close();
      }
      catch (SQLException ex)
      {
        throw new RuntimeException(ex);
      }
      db = null;
    }
  }
  
  public void compressPartGroupings() throws RuntimeException
  {
    if (db == null) {
      return;
    }
    exec("DROP TABLE IF EXISTS UniqueBitfields;", new Object[0]);
    exec("CREATE TABLE IF NOT EXISTS UniqueBitfields (id INTEGER PRIMARY KEY,SFR TEXT,BitfieldName TEXT,BitfieldPosition INTEGER,BitfieldWidth INTEGER,Parts TEXT);", new Object[0]);
    






    ResultSet rs = execQuery("SELECT * FROM AllBitfieldsUnion;", new Object[0]);
    try
    {
      while (rs.next())
      {
        String sfr = rs.getString("SFR");
        String bitfield = rs.getString("BitfieldName");
        int bitfieldPosition = rs.getInt("BitfieldPosition");
        int bitfieldWidth = rs.getInt("BitfieldWidth");
        String[] rawParts = rs.getString("Parts").split("\\|");
        List<String> parts = Arrays.asList(rawParts);
        Collections.sort(parts);
        String partsString = Multifunction.CatStringList(parts, "|");
        exec("INSERT OR REPLACE INTO UniqueBitfields(SFR,BitfieldName,BitfieldPosition,BitfieldWidth,Parts) VALUES(?,?,?,?,?);", new Object[] { sfr, bitfield, Integer.valueOf(bitfieldPosition), Integer.valueOf(bitfieldWidth), partsString });
      }
      





      rs.close();
    }
    catch (SQLException ex)
    {
      throw new RuntimeException(ex);
    }
  }
  
  public String sqlf(String sql, Object... params)
  {
    List<String> frags = new ArrayList();
    int i = 1;
    int index = -1;
    int lastIndex = -1;
    for (Object o : params)
    {
      i++;
      index = sql.indexOf('?', lastIndex + 1);
      if (index < 0) {
        break;
      }
      frags.add(sql.substring(lastIndex + 1, index));
      
      if (o == null) {
        frags.add("NULL");
      } else if (Boolean.class.isInstance(o)) {
        frags.add(((Boolean)Boolean.class.cast(o)).booleanValue() ? "1" : "0");
      } else if (Byte.class.isInstance(o)) {
        frags.add(((Byte)Byte.class.cast(o)).toString());
      } else if (Integer.class.isInstance(o)) {
        frags.add(((Integer)Integer.class.cast(o)).toString());
      } else if (Long.class.isInstance(o)) {
        frags.add(((Long)Long.class.cast(o)).toString());
      } else if (String.class.isInstance(o)) {
        frags.add("\"" + (String)String.class.cast(o) + "\"");
      } else if (Float.class.isInstance(o)) {
        frags.add(((Float)Float.class.cast(o)).toString());
      } else if (Double.class.isInstance(o)) {
        frags.add(((Double)Double.class.cast(o)).toString());
      } else {
        throw new RuntimeException("Unsupported scanf class type for parameter " + i);
      }
      lastIndex = index;
    }
    
    frags.add(sql.substring(lastIndex + 1));
    return Multifunction.CatStringList(frags);
  }
  




  public void initTables()
    throws RuntimeException
  {
    exec("CREATE TABLE IF NOT EXISTS DeviceProperties (part TEXT PRIMARY KEY,CPUClass INTEGER,programBlockSize INTEGER,eraseBlockSize INTEGER,devIDAddr INTEGER,devIDMask INTEGER,devID INTEGER,revIDAddr INTEGER,revIDMask INTEGER,dsNum INTEGER,dosNum INTEGER,hasFlashConfigWords INTEGER,BACKBUGRegName TEXT,BACKBUGAddr INTEGER,BACKBUGPos INTEGER,BACKBUGMask INTEGER,BACKBUGAdjAffectedRegName TEXT,ReservedBitRegName TEXT,ReservedBitAddr INTEGER,ReservedBitPos INTEGER,ReservedBitAdjAffectedRegName TEXT,CodeProtectRegName TEXT,CodeProtectAddr INTEGER,CodeProtectMask INTEGER,dataOriginFile TEXT,dataOriginDate INTEGER);", new Object[0]);
    

























    exec("CREATE TABLE IF NOT EXISTS DeviceMemories (id INTEGER PRIMARY KEY,part TEXT,type INTEGER,partition INTEGER,name TEXT,startAddr INTEGER,endAddr INTEGER,programAlign INTEGER,eraseAlign INTEGER);", new Object[0]);
    








    exec("CREATE TABLE IF NOT EXISTS DecodedConfigMemories (id INTEGER PRIMARY KEY,part TEXT,type INTEGER,partition INTEGER,name TEXT,startAddr INTEGER,endAddr INTEGER,programAlign INTEGER,eraseAlign INTEGER);", new Object[0]);
    









    exec("CREATE TABLE IF NOT EXISTS GLDMemories (id INTEGER PRIMARY KEY,part TEXT,type INTEGER,partition INTEGER,name TEXT,startAddr INTEGER,endAddr INTEGER,programAlign INTEGER,eraseAlign INTEGER);", new Object[0]);
    








    exec("CREATE TABLE IF NOT EXISTS BootloadableRanges (id INTEGER PRIMARY KEY,part TEXT,type INTEGER,partition INTEGER,name TEXT,startAddr INTEGER,endAddr INTEGER,programAlign INTEGER,eraseAlign INTEGER);", new Object[0]);
    








    exec("CREATE TABLE IF NOT EXISTS Interrupts (id INTEGER PRIMARY KEY,parts TEXT,vector INTEGER,name TEXT,implemented INTEGER,trap INTEGER,desc TEXT);", new Object[0]);
    






    exec("CREATE TABLE IF NOT EXISTS SFRs (sfrid INTEGER PRIMARY KEY,parts TEXT,address INTEGER,name TEXT,desc TEXT,parentmodule TEXT,hash TEXT);", new Object[0]);
    






    exec("CREATE TABLE IF NOT EXISTS Bitfields (bitfieldid INTEGER PRIMARY KEY,sfrid INTEGER NOT NULL,position INTEGER,width INTEGER,name TEXT,desc TEXT,hidden INTEGER,FOREIGN KEY (sfrid) REFERENCES SFRs(sfrid));", new Object[0]);
    








    TreeMap<String, String> IntPMDBitfieldRegEx = new TreeMap();
    IntPMDBitfieldRegEx.put("UART", "^U[0-9]+((MD)|([RT]XI.+)|(EVTI.+)|(EI.+))");
    IntPMDBitfieldRegEx.put("I2C", "^[SM]?I2C[0-9]+((MD)|(I.+)|(BCI.+))");
    IntPMDBitfieldRegEx.put("SPI", "^SPI[0-9]+((MD)|(I.+)|(EI.+)|([TR]XI.+))");
    IntPMDBitfieldRegEx.put("CAN", "^C[0-9]+((MD)|([RT]XI.+)|(I[^N]+))");
    IntPMDBitfieldRegEx.put("USB", "^USB[0-9]+((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("Timer", "^T[0-9]+((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("CCT", "^CCT[0-9]+((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("CCP", "^CCP[0-9]+((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("IC", "^IC[0-9]+((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("OC", "^OC[0-9]+((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("CLC", "^CLC[0-9]+((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("DMA", "^DMA[0-9]+((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("REFO", "^REFO[0-9]?((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("LVD", "^LVD[0-9]?((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("ADC", "^ADC?[0-9]?((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("CRC", "^CRC[0-9]?((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("CMP", "^CMP[0-9]?((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("CTMU", "^CTMU[0-9]?((MD)|(I.+))");
    IntPMDBitfieldRegEx.put("RTCC", "^RTCC[0-9]?((MD)|(I.+))");
    
    for (String peripheral : IntPMDBitfieldRegEx.keySet())
    {
      exec("CREATE VIEW IF NOT EXISTS IntPMDBitfields" + peripheral + " AS SELECT SFRs.sfrid, SFRs.parts, SFRs.address, SFRs.name, SFRs.desc, SFRs.parentmodule, Bitfields.name AS BitfieldName, position AS BitfieldPosition, width AS BitfieldWidth, Bitfields.desc AS BitfieldDesc, hidden AS BitFieldHidden FROM SFRs CROSS JOIN Bitfields WHERE SFRs.sfrid == Bitfields.sfrid AND (Bitfields.name REGEXP '" + (String)IntPMDBitfieldRegEx.get(peripheral) + "') ORDER BY Bitfields.name, SFRs.name, Bitfields.position, Bitfields.width;", new Object[0]);
      exec("CREATE VIEW IF NOT EXISTS Union" + peripheral + " AS SELECT name AS SFR, BitfieldName, BitfieldPosition, BitfieldWidth, group_concat(parts, '') AS Parts FROM IntPMDBitfields" + peripheral + " GROUP BY SFR, BitfieldName, BitfieldPosition, BitfieldWidth ORDER BY BitfieldName, BitfieldPosition, BitfieldWidth, SFR, Parts;", new Object[0]);
    }
    
    exec("CREATE VIEW IF NOT EXISTS AllBitfields           AS SELECT SFRs.sfrid, SFRs.parts, SFRs.address, SFRs.name, SFRs.desc, SFRs.parentmodule, Bitfields.name AS BitfieldName, position AS BitfieldPosition, width AS BitfieldWidth, Bitfields.desc AS BitfieldDesc, hidden AS BitFieldHidden FROM SFRs CROSS JOIN Bitfields WHERE SFRs.sfrid == Bitfields.sfrid ORDER BY Bitfields.name, SFRs.name, Bitfields.position, Bitfields.width;", new Object[0]);
    exec("CREATE VIEW IF NOT EXISTS AllBitfieldsUnion      AS SELECT name AS SFR, BitfieldName, BitfieldPosition, BitfieldWidth, group_concat(parts, '') AS Parts FROM AllBitfields GROUP BY SFR, BitfieldName, BitfieldPosition, BitfieldWidth ORDER BY BitfieldName, BitfieldPosition, BitfieldWidth, SFR, Parts;", new Object[0]);
    exec("CREATE VIEW IF NOT EXISTS AllUniquePartGroupings AS SELECT DISTINCT Parts FROM UniqueBitfields ORDER BY Parts;", new Object[0]);
  }
  











  public TreeMap<String, EDCProperties> getParts()
  {
    TreeMap<String, EDCProperties> ret = new TreeMap();
    
    if (db == null)
    {
      String[] rows = Multifunction.ReadJarResource(null, "resources/DeviceProperties.txt").split("[\n]");
      for (int i = 1; i < rows.length; i++)
      {
        EDCProperties prop = EDCProperties.fromCSVLine(rows[i]);
        ret.put(partNumber, prop);
      }
      return null;
    }
    
    try
    {
      ResultSet rs = execQuery("SELECT * FROM DeviceProperties;", new Object[0]);
      while (rs.next())
      {
        EDCProperties prop = new EDCProperties(rs);
        ret.put(partNumber, prop);
      }
      rs.close();
      
      return ret;
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  












  public void insertParts(TreeMap<String, File> parts)
  {
    for (String partNum : parts.keySet())
    {
      File f = (File)parts.get(partNum);
      String dataOriginFile = Multifunction.GetCanonicalPath(f);
      long dataOriginDate = f.lastModified();
      
      int rowsChanged = exec("UPDATE DeviceProperties SET dataOriginFile = ? WHERE part = ?;", new Object[] { dataOriginFile, partNum });
      if (rowsChanged == 0)
      {
        exec("INSERT INTO DeviceProperties(part, dataOriginFile, dataOriginDate) VALUES(?,?,?);", new Object[] { partNum, dataOriginFile, Long.valueOf(dataOriginDate) });
      }
    }
  }
  














  public EDCProperties getDeviceProperties(String partNum)
  {
    if (db == null)
    {
      partNum = "\"" + partNum + "\"";
      String[] rows = Multifunction.ReadJarResource(null, "resources/DeviceProperties.txt").split("[\n]");
      for (int i = 1; i < rows.length; i++)
      {
        if (rows[i].startsWith(partNum))
        {
          return EDCProperties.fromCSVLine(rows[i]);
        }
      }
      return null;
    }
    
    try
    {
      ResultSet rs = execQuery("SELECT * FROM DeviceProperties WHERE part = ?;", new Object[] { partNum });
      if (!rs.next())
      {
        rs.close();
        return null;
      }
      
      EDCProperties p = new EDCProperties(rs);
      rs.close();
      return p;
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  
  public void insertDeviceProperties(EDCProperties properties, boolean clearRelatedData)
  {
    if (clearRelatedData) {
      removeDevice(partNumber, clearRelatedData);
    }
    exec("INSERT OR REPLACE INTO DeviceProperties(part,CPUClass,programBlockSize,eraseBlockSize,devIDAddr,devIDMask,devID,revIDAddr,revIDMask,dsNum,dosNum,hasFlashConfigWords,BACKBUGRegName,BACKBUGAddr,BACKBUGPos,BACKBUGMask,BACKBUGAdjAffectedRegName,ReservedBitRegName,ReservedBitAddr,ReservedBitPos,ReservedBitAdjAffectedRegName,CodeProtectRegName,CodeProtectAddr,CodeProtectMask,dataOriginFile,dataOriginDate) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);", new Object[] { partNumber, Integer.valueOf(coreType.ordinal()), Integer.valueOf(programBlockSize), Integer.valueOf(eraseBlockSize), Long.valueOf(devIDAddr), Long.valueOf(devIDMask), Long.valueOf(devID), Long.valueOf(revIDAddr), Long.valueOf(revIDMask), Long.valueOf(dsNum), Long.valueOf(dosNum), Boolean.valueOf(hasFlashConfigWords), BACKBUGRegName, Long.valueOf(BACKBUGAddr), Integer.valueOf(BACKBUGPos), Long.valueOf(BACKBUGMask), BACKBUGAdjAffectedRegName, ReservedBitRegName, Long.valueOf(ReservedBitAddr), Integer.valueOf(ReservedBitPos), ReservedBitAdjAffectedRegName, CodeProtectRegName, Long.valueOf(CodeProtectAddr), Long.valueOf(CodeProtectMask), dataOriginFile, Long.valueOf(dataOriginDate) });
  }
  





















































  public void removeDevice(String partNum, boolean removeRelatedTableData)
  {
    try
    {
      exec("DELETE FROM DeviceProperties WHERE part = ?;", new Object[] { partNum });
      if (removeRelatedTableData)
      {
        exec("DELETE FROM DeviceMemories WHERE part = ?;", new Object[] { partNum });
        exec("DELETE FROM DecodedConfigMemories WHERE part = ?;", new Object[] { partNum });
        exec("DELETE FROM GLDMemories WHERE part = ?;", new Object[] { partNum });
        exec("DELETE FROM BootloadableRanges WHERE part = ?;", new Object[] { partNum });
        ResultSet rs = execQuery("SELECT id,parts FROM Interrupts WHERE parts LIKE ?;", new Object[] { "%" + partNum + "|%" });
        if (rs.next())
        {
          String parts = rs.getString("parts").replace(partNum + "|", "");
          if (parts.isEmpty()) {
            exec("DELETE FROM Interrupts WHERE id = ?;", new Object[] { Long.valueOf(rs.getLong("id")) });
          } else
            exec("UPDATE Interrupts SET parts = ? WHERE id = ?;", new Object[] { parts, Long.valueOf(rs.getLong("id")) });
        }
        rs.close();
      }
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  
  public Double getSingleDouble(String sqlStatement) throws RuntimeException
  {
    return (Double)Double.class.cast(getSingle(sqlStatement));
  }
  
  public double getSingleDouble(String sqlStatement, double defaultIfNull) throws RuntimeException {
    Double ret = (Double)Double.class.cast(getSingle(sqlStatement));
    return ret == null ? defaultIfNull : ret.doubleValue();
  }
  
  public Float getSingleFloat(String sqlStatement) throws RuntimeException {
    return (Float)Float.class.cast(getSingle(sqlStatement));
  }
  
  public float getSingleFloat(String sqlStatement, float defaultIfNull) throws RuntimeException {
    Float ret = (Float)Float.class.cast(getSingle(sqlStatement));
    return ret == null ? defaultIfNull : ret.floatValue();
  }
  
  public String getSingleString(String sqlStatement) throws RuntimeException
  {
    return (String)String.class.cast(getSingle(sqlStatement));
  }
  
  public String getSingleString(String sqlStatement, String defaultIfNull) throws RuntimeException {
    String ret = (String)String.class.cast(getSingle(sqlStatement));
    return ret == null ? defaultIfNull : ret;
  }
  
  public Integer getSingleInt(String sqlStatement) throws RuntimeException
  {
    return (Integer)Integer.class.cast(getSingle(sqlStatement));
  }
  
  public int getSingleInt(String sqlStatement, int defaultIfNull) throws RuntimeException {
    Integer ret = (Integer)Integer.class.cast(getSingle(sqlStatement));
    return ret == null ? defaultIfNull : ret.intValue();
  }
  
  public Long getSingleLong(String sqlStatement) throws RuntimeException {
    Object ret = getSingle(sqlStatement);
    if (ret == null)
      return null;
    if (Integer.class.isInstance(ret))
      return Long.valueOf(((Integer)Integer.class.cast(ret)).longValue());
    return (Long)Long.class.cast(ret);
  }
  
  public long getSingleLong(String sqlStatement, long defaultIfNull) throws RuntimeException {
    Long ret = getSingleLong(sqlStatement);
    return ret == null ? defaultIfNull : ret.longValue();
  }
  
  public Boolean getSingleBoolean(String sqlStatement) throws RuntimeException
  {
    return (Boolean)Boolean.class.cast(getSingle(sqlStatement));
  }
  
  public boolean getSingleBoolean(String sqlStatement, boolean defaultIfNull) throws RuntimeException {
    Boolean ret = (Boolean)Boolean.class.cast(getSingle(sqlStatement));
    return ret == null ? defaultIfNull : ret.booleanValue();
  }
  
  public Object getSingle(String sqlStatement, Object defaultIfNull) throws RuntimeException
  {
    Object ret = getSingle(sqlStatement);
    return ret == null ? defaultIfNull : ret;
  }
  
  public Object getSingle(String sqlStatement) throws RuntimeException
  {
    Object ret = null;
    try
    {
      Statement stmt = db.createStatement();
      ResultSet rs = stmt.executeQuery(sqlStatement);
      if (rs.next())
        ret = rs.getObject(1);
      rs.close();
      stmt.close();
      return ret;
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  













  public ResultSet execQuery(String sqlFormat, Object... args)
    throws RuntimeException
  {
    try
    {
      Statement stmt = db.createStatement(1003, 1007, 2);
      ResultSet ret = stmt.executeQuery(sqlf(sqlFormat, args));
      stmt.closeOnCompletion();
      return ret;
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  








  public int exec(String sqlFormat, Object... args)
    throws RuntimeException
  {
    try
    {
      Statement stmt = db.createStatement(1003, 1007, 2);
      boolean hasResults = stmt.execute(sqlf(sqlFormat, args));
      if (hasResults)
      {
        stmt.close();
        return -1;
      }
      
      int updateCount = stmt.getUpdateCount();
      stmt.close();
      return updateCount;
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  
  public void insertSFRs(String partNum, Collection<EDCReader.EZBL_SFR> sfrs)
  {
    for (EDCReader.EZBL_SFR sfr : sfrs)
    {
      insertSFR(partNum, sfr);
    }
  }
  


  public void insertSFR(String partNum, EDCReader.EZBL_SFR sfr)
  {
    String sfrHash = sfr.getSFRStructureHashAsString();
    try
    {
      ResultSet rs = execQuery("SELECT sfrid,parts FROM SFRs WHERE hash = ? LIMIT 1;", new Object[] { sfrHash });
      if (rs.next())
      {
        String existingParts = rs.getString("parts");
        if (existingParts.contains(partNum + "|"))
        {
          rs.close();
          return;
        }
        
        exec("UPDATE SFRs SET parts = ? WHERE sfrid = ?;", new Object[] { existingParts + partNum + "|", Long.valueOf(rs.getLong("sfrid")) });
        rs.close();
        return;
      }
      rs.close();
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
    
    if (sfrID == null) {
      sfrID = Long.valueOf(getSingleLong("SELECT MAX(sfrid) FROM SFRs;", -1L) + 1L);
    }
    Object[] tmp200_197 = new Object[7]; Long tmp215_212 = Long.valueOf(sfrID.longValue() + 1L);sfrID = tmp215_212;tmp200_197[0] = tmp215_212; Object[] tmp220_200 = tmp200_197;tmp220_200[1] = (partNum + "|"); Object[] tmp242_220 = tmp220_200;tmp242_220[2] = Long.valueOf(addr); Object[] tmp252_242 = tmp242_220;tmp252_242[3] = name; Object[] tmp259_252 = tmp252_242;tmp259_252[4] = desc; Object[] tmp266_259 = tmp259_252;tmp266_259[5] = srcModule; Object[] tmp273_266 = tmp266_259;tmp273_266[6] = sfrHash;exec("INSERT INTO SFRs(sfrid,parts,address,name,desc,parentmodule,hash) VALUES(?,?,?,?,?,?,?);", tmp273_266);
    
    long primarySFRId = sfrID.longValue();
    for (Iterator i$ = aliases.iterator(); i$.hasNext(); 
        
        exec("INSERT OR REPLACE INTO SFRs(sfrid,parts,address,name,desc,parentmodule,hash) VALUES(?,?,?,?,?,?,?);", tmp404_397))
    {
      String alias = (String)i$.next(); Object[] 
      
        tmp333_330 = new Object[7]; Long tmp348_345 = Long.valueOf(sfrID.longValue() + 1L);sfrID = tmp348_345;tmp333_330[0] = tmp348_345; Object[] tmp353_333 = tmp333_330;tmp353_333[1] = (partNum + "|"); Object[] tmp375_353 = tmp353_333;tmp375_353[2] = Long.valueOf(addr); Object[] tmp385_375 = tmp375_353;tmp385_375[3] = alias; Object[] tmp390_385 = tmp385_375;tmp390_385[4] = desc; Object[] tmp397_390 = tmp390_385;tmp397_390[5] = srcModule;tmp397_390[6] = sfrHash;
    }
    

    for (Iterator i$ = bitfields.iterator(); i$.hasNext();) { bf = (EDCReader.BitField)i$.next();
      
      exec("INSERT OR REPLACE INTO Bitfields(sfrid,position,width,name,desc,hidden) VALUES(?,?,?,?,?,?);", new Object[] { Long.valueOf(primarySFRId), Integer.valueOf(position), Integer.valueOf(width), name, desc, Boolean.valueOf(isHidden) });
      
      for (String alias : aliases)
      {
        exec("INSERT OR REPLACE INTO Bitfields(sfrid,position,width,name,desc,hidden) VALUES(?,?,?,?,?,?);", new Object[] { Long.valueOf(primarySFRId), Integer.valueOf(position), Integer.valueOf(width), alias, desc, Boolean.valueOf(isHidden) });
      }
    }
    EDCReader.BitField bf;
  }
  
  public EDCReader.EZBL_SFR getSFR(String partNum, String sfrName)
  {
    if ((sfrCache.partNum == partNum) || (db != null)) {
      return (EDCReader.EZBL_SFR)getSFRs(partNum).get(sfrName);
    }
    String sfrText = Multifunction.ReadJarResource(null, "resources/SFRs.txt");
    String bitfieldText = Multifunction.ReadJarResource(null, "resources/Bitfields.txt");
    String[] sfrRows = Multifunction.SimpleSplit(sfrText, "\n");
    String[] bitfieldRows = Multifunction.SimpleSplit(bitfieldText, "\n");
    for (int i = 1; i < sfrRows.length; i++)
    {
      if (sfrRows[i].contains(sfrName))
      {
        String[] fields = Multifunction.SimpleSplit(sfrRows[i], ",");
        if (fields[1].contains(partNum))
        {

          return new EDCReader.EZBL_SFR(fields, bitfieldRows); }
      } }
    return null;
  }
  
  public TreeMap<Long, EDCReader.EZBL_SFR> getSFRsByAddr(String partNum)
  {
    getSFRs(partNum);
    return sfrCache.sfrsByAddr;
  }
  


  public TreeMap<String, EDCReader.EZBL_SFR> getSFRs(String partNum)
  {
    if (sfrCache.partNum.equals(partNum))
      return sfrCache.sfrsByName;
    sfrCache.partNum = partNum;
    sfrCache.sfrsByAddr = new TreeMap();
    sfrCache.sfrsByName = new TreeMap();
    
    if (db == null)
    {
      partNum = partNum + "|";
      String sfrText = Multifunction.ReadJarResource(null, "resources/SFRs.txt");
      String bitfieldText = Multifunction.ReadJarResource(null, "resources/Bitfields.txt");
      if ((sfrText == null) || (bitfieldText == null))
        return sfrCache.sfrsByName;
      String[] sfrRows = Multifunction.SimpleSplit(sfrText, "\n");
      String[] bitfieldRows = Multifunction.SimpleSplit(bitfieldText, "\n");
      
      for (int i = 1; i < sfrRows.length; i++)
      {
        if (sfrRows[i].contains(partNum))
        {
          String[] fields = Multifunction.SimpleSplit(sfrRows[i], ",");
          if (fields[1].contains(partNum))
          {

            EDCReader.EZBL_SFR sfr = new EDCReader.EZBL_SFR(fields, bitfieldRows);
            if (sfrCache.sfrsByAddr.containsKey(Long.valueOf(addr)))
            {
              sfrCache.sfrsByAddr.get(Long.valueOf(addr))).aliases.add(name);
              sfrCache.sfrsByName.put(name, sfrCache.sfrsByAddr.get(Long.valueOf(addr)));
            }
            else {
              sfrCache.sfrsByName.put(name, sfr);
              sfrCache.sfrsByAddr.put(Long.valueOf(addr), sfr);
            }
          } } }
      return sfrCache.sfrsByName;
    }
    
    try
    {
      ResultSet rs = execQuery("SELECT * FROM SFRs WHERE parts LIKE ?;", new Object[] { "%" + partNum + "|%" });
      
      while (rs.next())
      {
        long sfrid = rs.getLong("sfrid");
        EDCReader.EZBL_SFR sfr = new EDCReader.EZBL_SFR();
        addr = rs.getLong("address");
        if (sfrCache.sfrsByAddr.containsKey(Long.valueOf(addr)))
        {
          sfrCache.sfrsByAddr.get(Long.valueOf(addr))).aliases.add(rs.getString("name"));
        }
        else {
          desc = rs.getString("desc");
          endAddr = (addr + 2L);
          name = rs.getString("name");
          srcModule = rs.getString("parentmodule");
          ResultSet bfRS = execQuery("SELECT * FROM Bitfields WHERE sfrid = ? ORDER BY position ASC;", new Object[] { Long.valueOf(sfrid) });
          while (bfRS.next())
          {
            EDCReader.BitField bf = new EDCReader.BitField();
            position = bfRS.getInt("position");
            width = bfRS.getInt("width");
            name = bfRS.getString("name");
            desc = bfRS.getString("desc");
            isHidden = bfRS.getBoolean("hidden");
            parentSFR = sfr;
            bitfieldByName.put(name, bf);
            bitfields.add(bf);
            endAddr = (addr + (position + width + 7) / 8);
          }
          bfRS.close();
          sfrCache.sfrsByName.put(name, sfr);
          sfrCache.sfrsByAddr.put(Long.valueOf(addr), sfr);
        } }
      rs.close();
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
    
    return sfrCache.sfrsByName;
  }
  
  public void insertMemories(MemoryTable table, String partNum, Collection<MemoryRegion> memories)
  {
    for (MemoryRegion mem : memories)
    {
      insertMemory(table, partNum, mem);
    }
  }
  
  public void insertMemory(MemoryTable table, String partNum, MemoryRegion mem) {
    exec("INSERT OR REPLACE INTO ? (part,type,partition,name,startAddr,endAddr,programAlign,eraseAlign) VALUES(?,?,?,?,?,?,?,?)", new Object[] { table.toString(), partNum, Integer.valueOf(type.ordinal()), Integer.valueOf(partition.ordinal()), name, Long.valueOf(startAddr), Long.valueOf(endAddr), Integer.valueOf(programAlign), Integer.valueOf(eraseAlign) });
  }
  
  public List<MemoryRegion> getMemories(String partNum, MemoryRegion.MemType type)
    throws RuntimeException
  {
    return getMemories(partNum, type, MemoryRegion.Partition.single);
  }
  



  public List<MemoryRegion> getMemories(String partNum, MemoryRegion.MemType type, MemoryRegion.Partition partition)
    throws RuntimeException
  {
    return getMemories(MemoryTable.DeviceMemories, partNum, type, partition);
  }
  
  public List<MemoryRegion> getMemories(MemoryTable table, String partNum, MemoryRegion.MemType type, MemoryRegion.Partition partition) throws RuntimeException
  {
    List<MemoryRegion> memories = new ArrayList();
    
    if (db == null)
    {
      String[] rows = Multifunction.ReadJarResource(null, "resources/" + table.toString() + ".txt").split("[\n]");
      if (partNum != null)
        partNum = ",\"" + partNum + "\",";
      for (int i = 1; i < rows.length; i++)
      {
        if ((partNum == null) || (rows[i].contains(partNum)))
        {
          String[] fields = rows[i].split("[,]");
          if ((type == null) || 
          
            (Integer.decode(fields[2]).intValue() == type.ordinal()))
          {

            if ((partition == null) || 
            
              (Integer.decode(fields[3]).intValue() == partition.ordinal()))
            {

              memories.add(MemoryRegion.fromCSVLine(fields)); } }
        }
      }
      Collections.sort(memories);
      return memories;
    }
    
    try
    {
      String sql = "SELECT name,partition,type,startAddr,endAddr,programAlign,eraseAlign FROM " + table.toString();
      if ((partNum != null) || (type != null) || (partition != null))
        sql = sql + " WHERE ";
      if (partNum != null)
        sql = sql + "part = \"" + partNum + "\"" + ((type != null) || (partition != null) ? " AND " : " ");
      if (type != null)
        sql = sql + "type = " + String.valueOf(type.ordinal()) + (partition != null ? " AND " : " ");
      if (partition != null)
        sql = sql + "partition = " + String.valueOf(partition.ordinal());
      sql = sql + " ORDER BY partition ASC, type ASC, startAddr ASC;";
      
      ResultSet rs = execQuery(sql, new Object[0]);
      while (rs.next())
      {
        MemoryRegion mem = new MemoryRegion();
        name = rs.getString("name");
        startAddr = rs.getLong("startAddr");
        endAddr = rs.getLong("endAddr");
        partition = MemoryRegion.Partition.values()[rs.getInt("partition")];
        type = MemoryRegion.MemType.values()[rs.getInt("type")];
        programAlign = rs.getInt("programAlign");
        eraseAlign = rs.getInt("eraseAlign");
        memories.add(mem);
      }
      rs.close();
      
      Collections.sort(memories);
      return memories;
    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  
  public void insertInterrupt(String partNum, Collection<InterruptVector> interrupts)
  {
    for (InterruptVector iv : interrupts)
    {
      insertInterrupt(partNum, iv);
    }
  }
  
  public void insertInterrupt(String partNum, InterruptVector interrupt)
  {
    try
    {
      ResultSet rs = execQuery("SELECT id, parts FROM Interrupts WHERE name = ? AND vector = ? AND implemented = ? AND trap = ? AND desc = ? LIMIT 1;", new Object[] { name, Integer.valueOf(vectorNum), Boolean.valueOf(implemented), Boolean.valueOf(trap), desc });
      if (rs.next())
      {
        String existingParts = rs.getString("parts");
        if (existingParts.contains(partNum + "|"))
        {
          rs.close();
          return;
        }
        
        exec("UPDATE Interrupts SET parts = ? WHERE id = ?;", new Object[] { existingParts + partNum + "|", Long.valueOf(rs.getLong("id")) });
        rs.close();
        return;
      }
      rs.close();
      exec("INSERT INTO Interrupts(parts,name,vector,implemented,trap,desc) VALUES(?,?,?,?,?,?);", new Object[] { partNum + "|", name, Integer.valueOf(vectorNum), Boolean.valueOf(implemented), Boolean.valueOf(trap), desc });

    }
    catch (SQLException e)
    {
      throw new RuntimeException(e);
    }
  }
  



  public SortedMap<Integer, InterruptVector> getInterrupts(String partNum)
    throws RuntimeException
  {
    SortedMap<Integer, InterruptVector> vectors = new TreeMap();
    SortedMap<Integer, InterruptVector> missingVectors = new TreeMap();
    
    if (db == null)
    {
      partNum = partNum + "|";
      String[] rows = Multifunction.ReadJarResource(null, "resources/Interrupts.txt").split("[\n]");
      for (int i = 1; i < rows.length; i++)
      {
        String[] fields = rows[i].split(",");
        if (fields[1].contains(partNum))
        {
          InterruptVector iv = InterruptVector.fromCSVLine(fields);
          vectors.put(Integer.valueOf(vectorNum), iv);
        }
      }
    }
    else
    {
      try
      {
        ResultSet rs = execQuery("SELECT * FROM Interrupts WHERE parts LIKE ? ORDER BY vector ASC, name ASC;", new Object[] { "%" + partNum + "|%" });
        while (rs.next())
        {
          InterruptVector iv = new InterruptVector(rs);
          vectors.put(Integer.valueOf(vectorNum), iv);
        }
        rs.close();
      }
      catch (SQLException e)
      {
        throw new RuntimeException(e);
      }
    }
    
    int lastSeenVec = -1;
    for (Integer v : vectors.keySet())
    {
      for (int i = lastSeenVec + 1; i < v.intValue(); i++)
      {
        missingVectors.put(Integer.valueOf(i), new InterruptVector("_Interrupt" + String.valueOf(i), i, false, (i < 8) && ((partNum.toLowerCase().startsWith("dspic")) || (partNum.toLowerCase().startsWith("pic24"))), "Reserved"));
      }
      
      InterruptVector iv = (InterruptVector)vectors.get(v);
      
      if ((name.startsWith("_Reserved")) || (name.startsWith("Reserved")))
      {
        name = ("_Interrupt" + String.valueOf(v));
        implemented = false;
      }
      lastSeenVec = v.intValue();
    }
    vectors.putAll(missingVectors);
    
    return vectors;
  }
  







  public static int main(String[] args)
  {
    String pathToMPLAB = "./";
    String outputFilename = null;
    TreeMap<String, File> picFiles = new TreeMap();
    


    EDCReader.DeviceRegs configRegs = null;
    


    long startTime = System.currentTimeMillis();
    int partsProcessed = 0;
    
    for (String arg : args)
    {
      if (arg.startsWith("-output=")) {
        outputFilename = Multifunction.GetCanonicalPath(Multifunction.TrimQuotes(arg.substring(8)));
      } else if (arg.startsWith("-path_to_mplab=")) {
        pathToMPLAB = Multifunction.GetCanonicalPath(Multifunction.TrimQuotes(arg.substring("-path_to_mplab=".length())));
      }
    }
    SQLitePartDatabase db = new SQLitePartDatabase();
    db.openDatabase();
    dbFilename = outputFilename;
    db.initTables();
    
    Collection<File> files = Multifunction.FindFilesRegEx(pathToMPLAB, ".*?(DSPIC|PIC24|PIC32MM|dspic|pic24|pic32mm)[A-Za-z0-9]+?[\\.][Pp][Ii][Cc]$", true);
    if (files.isEmpty())
    {
      System.err.printf("No .pic files found in: \"" + pathToMPLAB + "\"\n", new Object[0]);
      return -1;
    }
    
    for (File f : files)
    {
      String partNum = f.getName().substring(0, f.getName().length() - 4);
      if (partNum.toUpperCase().startsWith("DS"))
        partNum = "ds" + partNum.substring(2);
      if ((!partNum.toUpperCase().startsWith("AC")) && (!partNum.toUpperCase().contains("_AS_")) && 
      
        (!partNum.startsWith("dsPIC33CH128RA")))
      {
        picFiles.put(partNum, f); }
    }
    db.insertParts(picFiles);
    
    for (String part : picFiles.keySet())
    {
      EDCProperties p = new EDCProperties();
      List<MemoryRegion> devAllMemories = new ArrayList();
      List<MemoryRegion> devConfigDCRMemories = new ArrayList();
      List<MemoryRegion> devBootloadableRanges = new ArrayList();
      List<MemoryRegion> devGLDMemories = new ArrayList();
      List<InterruptVector> devInterrupts = new ArrayList();
      
      partNumber = part;
      dataOriginFile = Multifunction.GetCanonicalPath((File)picFiles.get(part));
      dataOriginDate = ((File)picFiles.get(part)).lastModified();
      System.out.printf("%4.3f: Processing %s (%s)\n", new Object[] { Double.valueOf((System.currentTimeMillis() - startTime) / 1000.0D), partNumber, dataOriginFile });
      startTime = System.currentTimeMillis();
      

      Document doc = PICXMLLoader.LoadPICXML((File)picFiles.get(part));
      if (doc == null)
      {
        doc = PICXMLLoader.LoadPICXML(pathToMPLAB + "/../../", partNumber);
        if (doc == null)
        {
          System.err.printf("ezbl_tools: could not load device characteristics for '%s'\n", new Object[] { partNumber });
          continue;
        }
      }
      
      coreType = (partNumber.startsWith("dsPIC33A") ? EZBLState.CPUClass.a : partNumber.startsWith("dsPIC33B") ? EZBLState.CPUClass.b : partNumber.startsWith("dsPIC33C") ? EZBLState.CPUClass.c : (partNumber.startsWith("PIC24E")) || (partNumber.startsWith("dsPIC33E")) ? EZBLState.CPUClass.e : partNumber.startsWith("PIC32MM") ? EZBLState.CPUClass.mm : (partNumber.startsWith("PIC24F")) || (partNumber.startsWith("PIC24H")) || (partNumber.startsWith("dsPIC30")) || (partNumber.startsWith("dsPIC33F")) ? EZBLState.CPUClass.f : EZBLState.CPUClass.other);
      






      EDCReader.PICDevice pic = new EDCReader.PICDevice(partNumber);
      EDCReader.DeviceRegs regs = EDCReader.IndexSFRDCRNodes(doc.getElementsByTagName("edc:SFRDef"), new String[0]);
      pic.addAll(regs);
      db.insertSFRs(partNumber, regsByAddr.values());
      

      programBlockSize = EDCReader.FindAttribAsInt(doc, "edc:CodeMemTraits", "edc:wordsize", Integer.valueOf(4)).intValue();
      if (coreType == EZBLState.CPUClass.mm)
        programBlockSize = 8;
      if ((coreType == EZBLState.CPUClass.b) && (programBlockSize < 8))
        programBlockSize = 8;
      if (((coreType == EZBLState.CPUClass.e) || (coreType == EZBLState.CPUClass.c)) && (programBlockSize < 4)) {
        programBlockSize = 4;
      } else if ((coreType == EZBLState.CPUClass.f) && (!partNumber.contains("K")) && (!partNumber.startsWith("dsPIC30")))
      {
        if ((!regsByName.containsKey("NVMADR")) && (!regsByName.containsKey("NVMADDR")) && (!regsByName.containsKey("NVMADRL")) && (!regsByName.containsKey("NVMADDRL")))
          programBlockSize = 2;
      }
      if ((coreType == EZBLState.CPUClass.f) && ((partNumber.contains("K")) || (partNumber.startsWith("dsPIC30")))) {
        programBlockSize = 64;
      }
      
      eraseBlockSize = (coreType == EZBLState.CPUClass.mm ? 1024 : (coreType == EZBLState.CPUClass.e) || (coreType == EZBLState.CPUClass.c) ? 1024 : 512);
      

      eraseBlockSize = (EDCReader.FindAttribAsInt(doc, "edc:Programming(edc:erasepagesize)", "edc:erasepagesize", Integer.valueOf(eraseBlockSize)).intValue() * (coreType == EZBLState.CPUClass.mm ? 4 : 2));
      if (EDCReader.FindNode(doc, "edc:PIC(edc:dosid==02073)") != null) {
        eraseBlockSize = 2048;
      } else if (EDCReader.FindNode(doc, "edc:PIC(edc:dosid==02786)") != null)
        eraseBlockSize = 2048;
      if ((coreType == EZBLState.CPUClass.f) && ((partNumber.contains("K")) || (partNumber.startsWith("dsPIC30")))) {
        eraseBlockSize = 64;
      }
      devIDAddr = EDCReader.FindAttribAsLong(doc, "edc:DeviceIDSector", "edc:beginaddr", Long.valueOf(16711680L)).longValue();
      devIDMask = ((coreType == EZBLState.CPUClass.c) || (coreType == EZBLState.CPUClass.e) || (coreType == EZBLState.CPUClass.f) ? 16777215L : 268435455L);
      devID = EDCReader.FindAttribAsLong(doc, "edc:DeviceIDSector", "edc:value", Long.valueOf(0L)).longValue();
      if ((coreType == EZBLState.CPUClass.b) || (coreType == EZBLState.CPUClass.c) || (coreType == EZBLState.CPUClass.e) || (coreType == EZBLState.CPUClass.f))
        devID >>= 16;
      revIDAddr = ((coreType == EZBLState.CPUClass.c) || (coreType == EZBLState.CPUClass.e) || (coreType == EZBLState.CPUClass.f) ? 16711682L : devIDAddr);
      revIDMask = ((coreType == EZBLState.CPUClass.c) || (coreType == EZBLState.CPUClass.e) || (coreType == EZBLState.CPUClass.f) ? 16777215L : -268435456L);
      dsNum = EDCReader.FindAttribAsLong(doc, "edc:PIC", "edc:dsid", Long.valueOf(0L)).longValue();
      dosNum = EDCReader.FindAttribAsLong(doc, "edc:PIC", "edc:dosid", Long.valueOf(0L)).longValue();
      
      Node node = EDCReader.FindNode(doc, "edc:ProgramSubspace(edc:partitionmode==dual,edc:id==first)");
      boolean hasFBOOT = node != null;
      for (int partitionNum = 0; partitionNum < (hasFBOOT ? 3 : 1); partitionNum++)
      {
        configRegs = new EDCReader.DeviceRegs();
        if (partitionNum == 0)
        {
          node = EDCReader.FindNode(doc, "edc:UserIDSector");
          if (node != null) {
            configRegs.addAll(EDCReader.IndexSFRDCRNodes(node.getChildNodes(), new String[0]));
          }
          node = EDCReader.FindNode(doc, "edc:WORMHoleSector(edc:regionid==bootcfg)");
          if (node != null) {
            configRegs.addAll(EDCReader.IndexSFRDCRNodes(node.getChildNodes(), new String[0]));
          }
        }
        node = EDCReader.FindNode(doc, "edc:AltConfigFuseSector");
        if (node != null)
          configRegs.addAll(EDCReader.IndexSFRDCRNodes(node.getChildNodes(), new String[0]));
        node = EDCReader.FindNode(doc, "edc:ConfigFuseSector");
        if (node != null)
          configRegs.addAll(EDCReader.IndexSFRDCRNodes(node.getChildNodes(), new String[0]));
        hasFlashConfigWords = ((coreType == EZBLState.CPUClass.mm) || (node == null));
        if (hasFlashConfigWords)
        {
          if (partitionNum == 0)
          {
            node = EDCReader.FindNode(doc, "edc:WORMHoleSector(edc:regionid==cfgmem)");
            if (node == null)
              node = EDCReader.FindNode(doc, "edc:WORMHoleSector");
            if (node != null) {
              configRegs.addAll(EDCReader.IndexSFRDCRNodes(node.getChildNodes(), new String[0]));
            }
          } else if (partitionNum == 1)
          {
            node = EDCReader.FindNode(doc, "edc:ProgramSubspace(edc:partitionmode==dual,edc:id==first)->edc:WORMHoleSector");
            if (node != null) {
              configRegs.addAll(EDCReader.IndexSFRDCRNodes(node.getChildNodes(), new String[0]));
            }
          } else if (partitionNum == 2)
          {
            node = EDCReader.FindNode(doc, "edc:ProgramSubspace(edc:partitionmode==dual,edc:id==second)->edc:WORMHoleSector");
            if (node != null) {
              configRegs.addAll(EDCReader.IndexSFRDCRNodes(node.getChildNodes(), new String[0]));
            }
          }
        }
        for (Long i : regsByAddr.keySet())
        {
          EDCReader.EZBL_SFR sfr = (EDCReader.EZBL_SFR)regsByAddr.get(i);
          MemoryRegion cfg = new MemoryRegion();
          name = name;
          startAddr = addr;
          endAddr = endAddr;
          comment = desc;
          partition = MemoryRegion.Partition.values()[partitionNum];
          type = (hasFlashConfigWords ? MemoryRegion.MemType.FLASHFUSE : MemoryRegion.MemType.BYTEFUSE);
          eraseAlign = (hasFlashConfigWords ? eraseBlockSize : (int)(endAddr - startAddr));
          programAlign = (hasFlashConfigWords ? programBlockSize : (int)(endAddr - startAddr));
          devConfigDCRMemories.add(cfg);
          devGLDMemories.add(cfg);
          if (!hasFlashConfigWords) {
            devBootloadableRanges.add(cfg);
          }
        }
        
        if ((partitionNum == 0) && (!regsByAddr.isEmpty()))
        {
          TreeMap<String, EDCReader.BitField> backbugBits = configRegs.findBitfield("BACKBUG", null, null, null, null, Integer.valueOf(1), null, Boolean.valueOf(true));
          if (backbugBits.isEmpty())
            backbugBits = configRegs.findBitfield("BKBUG", null, null, null, null, Integer.valueOf(1), null, Boolean.valueOf(true));
          if (backbugBits.isEmpty())
            backbugBits = configRegs.findBitfield("DEBUG", "FICD", null, null, null, null, null, Boolean.valueOf(true));
          if (backbugBits.isEmpty())
            backbugBits = configRegs.findBitfield("DEBUG", null, null, null, null, Integer.valueOf(1), Boolean.valueOf(true), Boolean.valueOf(true));
          if (backbugBits.isEmpty())
            backbugBits = configRegs.findBitfield(null, null, null, "Background Debug", null, Integer.valueOf(1), null, Boolean.valueOf(true));
          if (backbugBits.isEmpty()) {
            System.err.printf("ezbl_tools: Could not locate BACKBUG bit for %s (%d)\n", new Object[] { partNumber, Integer.valueOf(partitionNum) });
          } else if (backbugBits.size() > 1)
          {
            System.err.printf("ezbl_tools: Found %d config bits for BACKBUG\n", new Object[] { Integer.valueOf(backbugBits.size()) });
            for (EDCReader.BitField bf : backbugBits.values())
            {
              System.err.printf("            %s[%d] has '%s'\n", new Object[] { parentSFR.name, Integer.valueOf(position), name });
            }
          }
          else
          {
            BACKBUGRegName = firstEntrygetValueparentSFR.name;
            BACKBUGAddr = firstEntrygetValueparentSFR.addr;
            BACKBUGPos = firstEntrygetValueposition;
            BACKBUGMask = firstEntrygetValuesfrBitmask;
            if (hasFlashConfigWords)
            {
              long adjAddr = firstEntrygetValueparentSFR.addr ^ programBlockSize / 2;
              EDCReader.EZBL_SFR adjReg = (EDCReader.EZBL_SFR)regsByAddr.get(Long.valueOf(adjAddr));
              if (adjReg != null) {
                BACKBUGAdjAffectedRegName = name;
              }
            }
          }
          if (hasFlashConfigWords)
          {
            TreeMap<String, EDCReader.BitField> signBits = configRegs.findBitfield("SIGN", "FSIGN", null, null, null, Integer.valueOf(1), null, Boolean.valueOf(true));
            if (signBits.isEmpty())
              signBits = configRegs.findBitfield(null, "FSIGN", null, null, null, Integer.valueOf(1), Boolean.valueOf(true), Boolean.valueOf(true));
            if (signBits.isEmpty())
              signBits = configRegs.findBitfield("SIGN", "CONFIG1", null, null, null, Integer.valueOf(1), Boolean.valueOf(true), Boolean.valueOf(true));
            if (signBits.isEmpty())
              signBits = configRegs.findBitfield("SIGN", null, null, null, null, Integer.valueOf(1), Boolean.valueOf(true), Boolean.valueOf(true));
            if (signBits.isEmpty()) {
              signBits = configRegs.findBitfield("reserved", null, null, null, Integer.valueOf(15), Integer.valueOf(1), Boolean.valueOf(true), Boolean.valueOf(true));
            }
            if (signBits.isEmpty()) {
              System.err.printf("ezbl_tools: Could not locate reserved bit for %s (%d)\n", new Object[] { partNumber, Integer.valueOf(partitionNum) });
            } else if (signBits.size() > 1)
            {
              System.err.printf("ezbl_tools: Found %d config bits for SIGN\n", new Object[] { Integer.valueOf(signBits.size()) });
              for (EDCReader.BitField bf : signBits.values())
              {
                System.err.printf("            %s[%d] has '%s'\n", new Object[] { parentSFR.name, Integer.valueOf(position), name });
              }
            }
            else
            {
              ReservedBitRegName = firstEntrygetValueparentSFR.name;
              ReservedBitAddr = firstEntrygetValueparentSFR.addr;
              ReservedBitPos = firstEntrygetValueposition;
              long adjAddr = firstEntrygetValueparentSFR.addr ^ programBlockSize / 2;
              EDCReader.EZBL_SFR adjReg = (EDCReader.EZBL_SFR)regsByAddr.get(Long.valueOf(adjAddr));
              if (adjReg != null) {
                ReservedBitAdjAffectedRegName = name;
              }
            }
          }
          TreeMap<String, EDCReader.BitField> cpBits = configRegs.findBitfield("BSS", null, null, null, null, null, null, Boolean.valueOf(true));
          cpBits.putAll(configRegs.findBitfield("GSS", null, null, null, null, null, null, Boolean.valueOf(true)));
          cpBits.putAll(configRegs.findBitfield("CSS", null, null, null, null, null, null, Boolean.valueOf(true)));
          cpBits.putAll(configRegs.findBitfield("GCP", null, null, null, null, null, null, Boolean.valueOf(true)));
          cpBits.putAll(configRegs.findBitfield("CP", "FSEC", null, null, null, null, null, Boolean.valueOf(true)));
          if (cpBits.isEmpty()) {
            System.err.printf("ezbl_tools: Could not locate Code Protection bits for %s (%d)\n", new Object[] { partNumber, Integer.valueOf(partitionNum) });
          }
          else {
            CodeProtectRegName = firstEntrygetValueparentSFR.name;
            CodeProtectAddr = firstEntrygetValueparentSFR.addr;
            CodeProtectMask = 0L;
            for (EDCReader.BitField bf : cpBits.values())
            {
              long maskForBF = 0L;
              for (int i = 0; i < width; i++)
              {
                maskForBF = maskForBF << 1 | 1L;
              }
              CodeProtectMask |= maskForBF << position;
            }
          }
          db.insertSFRs(partNumber, regsByAddr.values());
        }
        
        DecodedNodeList nodes = new DecodedNodeList();
        if (partitionNum == 0)
        {
          node = EDCReader.FindNode(doc, "edc:DataSpace");
          if (node != null)
            nodes.putAll(EDCReader.IndexNodes(node.getChildNodes(), 1));
          node = EDCReader.FindNode(doc, "edc:ExtendedDataSpace");
          if (node != null)
            nodes.put(node);
          node = EDCReader.FindNode(doc, "edc:PhysicalSpace");
          if (node != null)
            nodes.putAll(EDCReader.IndexNodes(node.getChildNodes(), 0));
          node = EDCReader.FindNode(doc, "edc:ProgramSpace");
          if (node != null)
            nodes.putAll(EDCReader.IndexNodes(node.getChildNodes(), 0));
          node = EDCReader.FindNode(doc, "edc:ProgramSubspace(edc:partitionmode==single)");
          if (node != null) {
            nodes.putAll(EDCReader.IndexNodes(node.getChildNodes(), 0));
          }
        } else if (partitionNum == 1)
        {
          node = EDCReader.FindNode(doc, "edc:ProgramSubspace(edc:partitionmode==dual,edc:id==first)");
          if (node != null) {
            nodes.putAll(EDCReader.IndexNodes(node.getChildNodes(), 0));
          }
        } else if (partitionNum == 2)
        {
          node = EDCReader.FindNode(doc, "edc:ProgramSubspace(edc:partitionmode==dual,edc:id==second)");
          if (node != null)
            nodes.putAll(EDCReader.IndexNodes(node.getChildNodes(), 0));
        }
        if (list.size() == 0)
        {
          Logger.getLogger(SQLitePartDatabase.class.getName()).log(Level.SEVERE, "Couldn't find edc:PhysicalSpace/edc:ProgramSpace/edc:ProgramSubSpace for " + partNumber + " (" + partitionNum + ")");
        }
        else
        {
          for (DecodedNode dn : list)
          {
            if ((!tagName.equals("edc:ProgramSubspace")) && (!tagName.equals("edc:EDSWindowSector")))
            {

              MemoryRegion mem = new MemoryRegion();
              if ((attribs.containsKey("edc:beginaddr")) && (attribs.containsKey("edc:endaddr")))
              {
                startAddr = dn.getAttrAsLong("edc:beginaddr").longValue();
                endAddr = dn.getAttrAsLong("edc:endaddr").longValue();
              } else {
                if ((!attribs.containsKey("edc:xbeginaddr")) || (!attribs.containsKey("edc:xendaddr")))
                  continue;
                startAddr = dn.getAttrAsLong("edc:xbeginaddr").longValue();
                endAddr = dn.getAttrAsLong("edc:xendaddr").longValue();
              }
              


              partition = MemoryRegion.Partition.values()[partitionNum];
              name = dn.getAttrAsString("edc:regionid");
              if ((name == null) && (tagName.equals("edc:DataSpace")))
                name = "data";
              if ((name == null) && (tagName.equals("edc:ExtendedDataSpace")))
                name = "edsdata";
              if (tagName.equals("edc:SFRDataSector"))
                type = MemoryRegion.MemType.SFR;
              if ((tagName.equals("edc:DataSpace")) || (tagName.equals("edc:ExtendedDataSpace")) || (tagName.equals("edc:GPRDataSector")))
              {

                type = MemoryRegion.MemType.RAM; }
              if ((name.equals("reset")) || (tagName.equals("edc:ResetSector")) || (name.equals("ivt")) || (tagName.equals("edc:VectorSector")) || (name.equals("aivt")) || (tagName.equals("edc:AltVectorSector")) || (name.equals("program")) || (name.equals("code")) || (tagName.equals("edc:CodeSector")) || (name.equals("bootconfig")) || (tagName.equals("edc:BootConfigSector")) || (name.equals("auxflash")) || (tagName.equals("edc:AuxCodeSector")) || (name.equals("auxvector")) || (tagName.equals("edc:AuxVectorSector")) || (name.equals("auxreset")) || (tagName.equals("edc:AuxResetSector")))
              {







                type = MemoryRegion.MemType.ROM;
                eraseAlign = eraseBlockSize;
                programAlign = programBlockSize;
              }
              if (((name.equals("bootcfg")) && (tagName.equals("edc:WORMHoleSector"))) || ((name.equals("cfgmem")) && (tagName.equals("edc:WORMHoleSector"))) || ((name.equals("config")) && (tagName.equals("edc:ConfigFuseSector"))) || (name.equals("altconfig")) || (tagName.equals("edc:AltConfigFuseSector")))
              {



                type = MemoryRegion.MemType.FLASHFUSE;
                eraseAlign = eraseBlockSize;
                programAlign = programBlockSize;
              }
              if (((name.equals("cfgmem")) && (tagName.equals("edc:ConfigFuseSector"))) || (name.equals("userid")) || (tagName.equals("edc:UserIDSector")))
              {

                type = MemoryRegion.MemType.BYTEFUSE;
                eraseAlign = 2;
                programAlign = 2;
              }
              if ((name.equals("eedata")) || (tagName.equals("edc:EEDataSector")))
              {
                type = MemoryRegion.MemType.EEPROM;
                programAlign = 2;
                eraseAlign = eraseBlockSize;
              }
              if (tagName.equals("edc:UserOTPSector"))
              {
                type = MemoryRegion.MemType.OTP;
                programAlign = programBlockSize;
              }
              if ((tagName.equals("edc:DeviceIDSector")) || (tagName.equals("edc:EmulatorSector")) || (tagName.equals("edc:TestZone")) || (tagName.equals("edc:BACKBUGVectorSector")) || (tagName.equals("edc:ConfigWORMSector")) || (tagName.equals("edc:UniqueIDSector")))
              {




                type = MemoryRegion.MemType.TEST;
              }
              devAllMemories.add(mem);
              
              if ((type != MemoryRegion.MemType.DEBUG) && (type != MemoryRegion.MemType.TEST) && 
              

                (!name.equals("auxvector")) && (!name.equals("auxreset")))
              {

                if ((type == MemoryRegion.MemType.ROM) || (type == MemoryRegion.MemType.EEPROM)) {
                  devGLDMemories.add(mem);
                }
                
                if ((!name.equals("reset")) && (!name.equals("ivt")) && (!name.equals("_reserved")) && (!name.equals("aivt")) && ((!name.equals("cfgmem")) || (!tagName.equals("edc:WORMHoleSector"))) && ((!name.equals("config")) || (!tagName.equals("edc:ConfigFuseSector"))) && ((!name.equals("altconfig")) || (!tagName.equals("edc:AltConfigFuseSector"))))
                {







                  if ((type == MemoryRegion.MemType.ROM) || (type == MemoryRegion.MemType.FLASHFUSE) || (type == MemoryRegion.MemType.EEPROM) || (type == MemoryRegion.MemType.OTP))
                  {
                    devBootloadableRanges.add(mem.clone()); } }
              }
            }
          }
        }
      }
      node = EDCReader.FindNode(doc, "edc:InterruptList");
      NodeList nodeList = node.getChildNodes();
      int i = 0; for (int ivCount = 0; i < nodeList.getLength(); i++)
      {
        if (nodeList.item(i).getNodeType() == 1)
        {
          node = nodeList.item(i);
          InterruptVector iv = new InterruptVector();
          if ((coreType != EZBLState.CPUClass.mm) && (coreType != EZBLState.CPUClass.other))
          {
            irqNum = Integer.decode(EDCReader.GetAttr(node, "edc:irq", String.valueOf(ivCount - 8))).intValue();
            vectorNum = (irqNum + 8);
          }
          else
          {
            irqNum = Integer.decode(EDCReader.GetAttr(node, "edc:irq", String.valueOf(ivCount))).intValue();
            vectorNum = irqNum;
          }
          trap = node.getNodeName().contains("edc:Trap");
          name = EDCReader.GetAttr(node, "edc:cname", "_Interrupt" + String.valueOf(vectorNum));
          desc = EDCReader.GetAttr(node, "edc:desc", name).replaceAll("Resereved", "Reserved");
          implemented = ((!name.startsWith("_Interrupt")) && (!name.startsWith("Reserved")) && (!desc.equals("Reserved")) && (!name.startsWith("_Reserved")));
          devInterrupts.add(iv);
          ivCount = vectorNum + 1;
        }
      }
      devBootloadableRanges = MemoryRegion.coalesce(devBootloadableRanges, true, true);
      
      db.insertDeviceProperties(p, true);
      db.insertMemories(MemoryTable.DeviceMemories, partNumber, devAllMemories);
      db.insertMemories(MemoryTable.GLDMemories, partNumber, devGLDMemories);
      db.insertMemories(MemoryTable.BootloadableRanges, partNumber, devBootloadableRanges);
      db.insertMemories(MemoryTable.DecodedConfigMemories, partNumber, devConfigDCRMemories);
      db.insertInterrupt(partNumber, devInterrupts);
      partsProcessed++; if (partsProcessed % 32 == 0) {
        db.saveDatabase(false);
      }
    }
    System.out.printf("Saving output to '%s'\n", new Object[] { dbFilename });
    db.saveDatabase(true);
    
    return 0;
  }
}
