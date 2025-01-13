package com.microchip.apps.ezbl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InvalidClassException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintStream;
import java.io.Serializable;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.security.CodeSource;
import java.security.ProtectionDomain;
import java.util.ArrayList;
import java.util.List;
import java.util.SortedMap;
import java.util.TreeMap;


















public class EZBLState
  implements Serializable, Cloneable
{
  static final long serialVersionUID = 211L;
  public String ezblStateSavePath = null;
  public int pass = 0;
  public boolean parseOkay = false;
  public boolean MCU16Mode = true;
  

  public static enum CPUClass
  {
    other, 
    f, 
    e, 
    c, 
    b, 
    a, 
    mm;
    private CPUClass() {} }
  public CPUClass coreType = CPUClass.other;
  public boolean verbose = false;
  public boolean silent = false;
  public boolean linkedAsDebugImage = false;
  public int flashWordSize = 4;
  public int hexAlign = 0;
  public boolean remapISRThroughIGT = true;
  public boolean saveTemps = false;
  public boolean allowLastPageErase = true;
  public boolean allowFirstPageErase = false;
  public boolean deleteResetSection = false;
  public boolean dualPartitionMode = false;
  public String[] argsFromMakeEditor = null;
  public String conf = null;
  public String makefilePath = null;
  public String artifactPath = null;
  public String projectName = null;
  public String linkScriptPath = null;
  public String compilerFolder = null;
  public String compilerLinkScriptPath = null;
  public String saveTempsFile = null;
  public String temporariesPath = null;
  public String partNumber = null;
  public String fullPartNumber = null;
  public String javaPath = null;
  public String pathToIDEBin = null;
  public String hexPath = null;
  public String elfPath = null;
  public String undecodedOptions = "";
  public String passThroughOptions = "";
  public String comPort = null;
  public String baudRate = null;
  public int autoBaudRetryLimit = 4;
  public boolean hardwareFlowControl = false;
  public String commandPrefix = "";
  public boolean skipComHandshake = false;
  public int slaveAddress = 0;
  public int milliTimeout = 1100;
  public String javaLaunchString = null;
  public String encryptionPassword = null;
  public String encryptionSaltString = null;
  public byte[] encryptionSalt = null;
  public byte[] encryptionKey = null;
  
  public long baseAddress = 0L;
  public long endAddressOfBootloader = 0L;
  
  public long baseAddressOfAppErasable;
  
  public long baseAddressOfGotoReset;
  public long baseAddressOfIGT;
  public int sizeOfIGT;
  public int sizeOfAppErasable;
  public boolean hasFlashConfigWords = false;
  public boolean edcInfoLoaded = false;
  public List<MemoryRegion> devNVGeometry = null;
  public List<MemoryRegion> devMemories = new ArrayList();
  public List<MemoryRegion> devProgramSpaces = new ArrayList();
  public TreeMap<Long, MemoryRegion> devConfigWordsByAddr = new TreeMap();
  public TreeMap<String, MemoryRegion> devConfigWordsByName = new TreeMap();
  public MemoryRegion mainExecutionRegion = null;
  public MemoryRegion mainFlashRegion = null;
  public MemoryRegion configWordsRegion = null;
  protected int eraseBlockSize = 512;
  protected int eraseBlockSizeBytes = eraseBlockSize * 3;
  protected int eraseBlockSizeAddresses = eraseBlockSize * 2;
  public long devIDAddr = 0L;
  public long devIDMask = 0L;
  public long devIDValue = 0L;
  public long devRevAddr = 0L;
  public long devRevMask = 0L;
  
  public static class SpecialConfigFields implements Serializable {
    public SpecialConfigFields() {}
    
    long BACKBUGAddr = 0L;
    long BACKBUGMask = 0L;
    String BACKBUGConfigName = null;
    long codeProtectAddr = 0L;
    long codeProtectMask = 0L;
    String codeProtectConfigName = null;
    long reservedBitAddr = 0L;
    long reservedBitMask = 0L;
    String reserveBitConfigName = null;
    public int FBOOTAddr = -1;
    public int FBOOTValue = 16777215;
     }
  public SpecialConfigFields devSpecialConf = new SpecialConfigFields();
  public ELFDumpParser elfDump = null;
  public GLDMemories gldMemories = null;
  public SortedMap<Integer, InterruptVector> ivtVectors = null;
  public int ivtVectorsImplemented = 0;
  public String ivtSectionRemapped = null;
  public String erasableIGT = null;
  public int igtSpacing = 4;
  public String compilerGLDContents = null;
  public List<MemoryRegion> romUseRegions = new ArrayList();
  public List<MemoryRegion> ramUseRegions = new ArrayList();
  public List<Section> ramUseSections = new ArrayList();
  public List<MemoryRegion> ignoreROMRegions = new ArrayList();
  public List<MemoryRegion> noEraseRegions = null;
  public List<MemoryRegion> noProgramRegions = null;
  public List<MemoryRegion> noVerifyRegions = null;
  
  public int minFreePSVSpace = 0;
  public int warnFreePSVSpace = 0;
  
  public List<Section> ramSections = null;
  public List<Section> romSections = null;
  public List<Section> otherSections = null;
  

  public int targetPartition = 0;
  
  public static final String nullFile()
  {
    String os = System.getProperty("os.name");
    if ((os == null) || (os.startsWith("Windows"))) {
      return "NUL";
    }
    return "/dev/null";
  }
  
  public static final String ezblToolsExecPath()
  {
    String execPath = null;
    try
    {
      CodeSource codeSource = Main.class.getProtectionDomain().getCodeSource();
      File jarFile = new File(codeSource.getLocation().toURI().getPath());
      execPath = jarFile.getAbsolutePath();
    }
    catch (URISyntaxException ex)
    {
      execPath = Main.class.getProtectionDomain().getCodeSource().getLocation().getPath();
    }
    try
    {
      File f = new File(execPath).getParentFile();
      if (f != null) {
        execPath = f.getCanonicalPath();
      }
    }
    catch (IOException ex) {
      execPath = "";
    }
    if (execPath == null) {
      return "";
    }
    return execPath;
  }
  
  public static final String ourJarPath()
  {
    try
    {
      CodeSource codeSource = Main.class.getProtectionDomain().getCodeSource();
      File jarFile = new File(codeSource.getLocation().toURI().getPath());
      return jarFile.getAbsolutePath();
    }
    catch (URISyntaxException ex) {}
    
    return Main.class.getProtectionDomain().getCodeSource().getLocation().getPath();
  }
  

  public EZBLState()
  {
    javaLaunchString = ("$(MP_JAVA_PATH)java -cp \"" + ourJarPath() + "\" com.microchip.apps.ezbl.Main");
    

    compilerFolder = Multifunction.GetCanonicalPath(Multifunction.FixSlashes(Multifunction.TrimQuotes(System.getenv("MP_CC_DIR"))));
    projectName = Multifunction.FixSlashes(Multifunction.TrimQuotes(System.getenv("PROJECTNAME")));
    conf = Multifunction.FixSlashes(System.getenv("CONF"));
    javaPath = Multifunction.FixSlashes(Multifunction.TrimQuotes(System.getenv("MP_JAVA_PATH")));
    if (projectName != null)
    {
      elfPath = (Multifunction.FixSlashes(Multifunction.TrimQuotes(System.getenv("DISTDIR"))) + File.separator + projectName + "." + Multifunction.TrimQuotes(System.getenv("IMAGE_TYPE")) + ".elf");
      hexPath = (Multifunction.FixSlashes(Multifunction.TrimQuotes(System.getenv("DISTDIR"))) + File.separator + projectName + "." + Multifunction.TrimQuotes(System.getenv("IMAGE_TYPE")) + ".hex");
    }
    if (elfPath != null)
      artifactPath = elfPath;
    pathToIDEBin = Multifunction.GetCanonicalPath(Multifunction.FixSlashes(Multifunction.TrimQuotes(System.getenv("PATH_TO_IDE_BIN"))));
    partNumber = Multifunction.TrimQuotes(System.getenv("MP_PROCESSOR_OPTION"));
    if (partNumber != null)
    {
      fullPartNumber = ("PIC" + partNumber.toUpperCase()); }
    linkScriptPath = Multifunction.FixSlashes(Multifunction.TrimQuotes(System.getenv("MP_LINKER_FILE_OPTION")));
    if (linkScriptPath != null)
      linkScriptPath = Multifunction.GetCanonicalPath(Multifunction.FixSlashes(Multifunction.TrimQuotes(linkScriptPath.replace(",--script=", ""))));
    verbose = Boolean.getBoolean(Multifunction.TrimQuotes(System.getenv("EZBL_VERBOSE")));
    if (verbose)
    {
      System.out.printf("    javaLaunchString = %s\n", new Object[] { javaLaunchString });
      System.out.printf("    compilerFolder = %s\n", new Object[] { compilerFolder });
      System.out.printf("    projectName = %s\n", new Object[] { projectName });
      System.out.printf("    conf = %s\n", new Object[] { conf });
      System.out.printf("    javaPath = %s\n", new Object[] { javaPath });
      System.out.printf("    elfPath = %s\n", new Object[] { elfPath });
      System.out.printf("    hexPath = %s\n", new Object[] { hexPath });
      System.out.printf("    artifactPath = %s\n", new Object[] { artifactPath });
      System.out.printf("    pathToIDEBin = %s\n", new Object[] { pathToIDEBin });
      System.out.printf("    partNumber = %s\n", new Object[] { partNumber });
      System.out.printf("    fullPartNumber = %s\n", new Object[] { fullPartNumber });
      System.out.printf("    linkScriptPath = %s\n", new Object[] { linkScriptPath });
    }
  }
  

  public EZBLState clone()
  {
    try
    {
      return (EZBLState)super.clone();

    }
    catch (CloneNotSupportedException ex)
    {

      ex.printStackTrace();
    }
    return null;
  }
  

  void SaveToFile()
  {
    undecodedOptions = undecodedOptions.replaceAll("-dump[\\s]*?", "");
    undecodedOptions = undecodedOptions.replaceAll("-generate_merge[\\s]*?", "");
    

    if (elfDump != null)
      elfDump.symbols = null;
    if (pass < 2)
    {
      elfDump = null;
      romUseRegions = null;
      ramUseRegions = null;
      ramUseSections = null;
      otherSections = null;
      ramSections = null;
      romSections = null;
    }
    gldMemories = null;
    devConfigWordsByName = null;
    
    File f = new File(ezblStateSavePath);
    File parentFolder = f.getParentFile();
    if (parentFolder != null)
    {
      if (!parentFolder.exists())
      {
        parentFolder.mkdirs();
      }
    }
    



    try
    {
      FileOutputStream fileOut = new FileOutputStream(ezblStateSavePath, false);
      ObjectOutputStream objectOut = new ObjectOutputStream(fileOut);
      objectOut.writeObject(this);
      objectOut.close();
      fileOut.close();
    }
    catch (IOException ex)
    {
      if (!silent) {
        System.err.printf("ezbl_tools: Unable to save state information\n    %1$s\n", new Object[] { ex.getMessage() });
      }
    }
  }
  
  public static EZBLState ReadFromFile(String filename)
  {
    if (!new File(filename).exists())
    {
      return new EZBLState();
    }
    



    try
    {
      FileInputStream in = new FileInputStream(filename);
      ObjectInputStream objectIn = new ObjectInputStream(in);
      EZBLState ret = (EZBLState)objectIn.readObject();
      objectIn.close();
      in.close();
      

      if ((elfDump != null) && (elfDump.symbols == null) && (elfDump.symbolTableAsString != null))
      {
        elfDump.symbols = new ArrayList();
        int lastIndex = -1;
        int curIndex;
        while ((curIndex = elfDump.symbolTableAsString.indexOf('\n', lastIndex + 1)) >= 0)
        {
          elfDump.symbols.add(new Symbol(elfDump.symbolTableAsString.substring(lastIndex + 1, curIndex)));
          lastIndex = curIndex;
        }
      }
      
      if (devConfigWordsByAddr != null)
      {
        devConfigWordsByName = new TreeMap();
        for (MemoryRegion mr : devConfigWordsByAddr.values())
        {
          devConfigWordsByName.put(name, mr);
        }
      }
      



      return ret;
    }
    catch (InvalidClassException ex)
    {
      System.out.println("ezbl_tools: " + filename + " contains incompatible state data; using new state data.");
      return new EZBLState();
    }
    catch (IOException|ClassNotFoundException ex)
    {
      ex.printStackTrace();
    }
    return null;
  }
  



  public boolean LoadEDCData()
  {
    if (partNumber != null)
    {
      SQLitePartDatabase db = new SQLitePartDatabase();
      EDCProperties dev = db.getDeviceProperties(fullPartNumber);
      if (dev == null) {
        return false;
      }
      hasFlashConfigWords = hasFlashConfigWords;
      flashWordSize = programBlockSize;
      setEraseBlockSizeAddresses(eraseBlockSize);
      devIDAddr = (MCU16Mode ? devIDAddr : Multifunction.normalizePIC32Addr(devIDAddr));
      devIDValue = devID;
      devIDMask = devIDMask;
      devRevAddr = (MCU16Mode ? revIDAddr : Multifunction.normalizePIC32Addr(revIDAddr));
      devRevMask = revIDMask;
      ivtVectors = db.getInterrupts(partNumber);
      ivtVectorsImplemented = InterruptVector.GetImplementedVectorCount(ivtVectors.values());
      devSpecialConf.BACKBUGAddr = (MCU16Mode ? BACKBUGAddr : Multifunction.normalizePIC32Addr(BACKBUGAddr));
      devSpecialConf.BACKBUGMask = BACKBUGMask;
      devSpecialConf.BACKBUGConfigName = BACKBUGRegName;
      devSpecialConf.reservedBitAddr = (MCU16Mode ? ReservedBitAddr : Multifunction.normalizePIC32Addr(ReservedBitAddr));
      devSpecialConf.reservedBitMask = (ReservedBitAddr == 0L ? 0L : 1L << ReservedBitPos);
      devSpecialConf.reserveBitConfigName = ReservedBitRegName;
      devSpecialConf.codeProtectAddr = (MCU16Mode ? CodeProtectAddr : Multifunction.normalizePIC32Addr(CodeProtectAddr));
      devSpecialConf.codeProtectMask = CodeProtectMask;
      devSpecialConf.codeProtectConfigName = CodeProtectRegName;
      
      devProgramSpaces = new ArrayList();
      devMemories = new ArrayList();
      devConfigWordsByAddr = new TreeMap();
      devConfigWordsByName = new TreeMap();
      devNVGeometry = new ArrayList();
      mainExecutionRegion = null;
      configWordsRegion = null;
      
      List<MemoryRegion> gldRegions = db.getMemories(SQLitePartDatabase.MemoryTable.GLDMemories, partNumber, null, MemoryRegion.Partition.single);
      for (MemoryRegion mr : gldRegions)
      {
        if (name.equals("FBOOT"))
        {
          devSpecialConf.FBOOTAddr = ((int)startAddr);
          devProgramSpaces.add(mr.clone());
        }
      }
      
      if (coreType != CPUClass.mm)
      {
        gldRegions = db.getMemories(SQLitePartDatabase.MemoryTable.GLDMemories, partNumber, null, MemoryRegion.Partition.values()[targetPartition]);
        for (MemoryRegion mr : gldRegions)
        {
          devProgramSpaces.add(mr.clone());
        }
      }
      List<MemoryRegion> configRanges = db.getMemories(SQLitePartDatabase.MemoryTable.DecodedConfigMemories, partNumber, null, MemoryRegion.Partition.values()[targetPartition]);
      for (MemoryRegion mr : configRanges)
      {
        if (coreType == CPUClass.mm)
          mr.normalizePIC32Addresses();
        devConfigWordsByAddr.put(Long.valueOf(startAddr), mr);
        devConfigWordsByName.put(name, mr);
      }
      List<MemoryRegion> bootloadableRanges = db.getMemories(SQLitePartDatabase.MemoryTable.BootloadableRanges, partNumber, null, MemoryRegion.Partition.values()[targetPartition]);
      for (MemoryRegion mr : bootloadableRanges)
      {
        if ((type != MemoryRegion.MemType.OTP) && (type != MemoryRegion.MemType.TEST) && (!name.equalsIgnoreCase("bootcfg")) && (!name.equalsIgnoreCase("customerotp")))
        {
          if (coreType == CPUClass.mm) {
            mr.normalizePIC32Addresses();
          }
          if (((name.equals("program")) || (name.equals("code"))) && (type == MemoryRegion.MemType.ROM))
            mainFlashRegion = mr.clone();
          devNVGeometry.add(mr.clone());
        } }
      List<MemoryRegion> specialRanges = db.getMemories(SQLitePartDatabase.MemoryTable.DeviceMemories, partNumber, null, MemoryRegion.Partition.values()[targetPartition]);
      for (MemoryRegion mr : specialRanges)
      {
        devMemories.add(mr);
        if (coreType == CPUClass.mm)
        {
          mr.normalizePIC32Addresses();
          if (name.equals("code"))
            name = "kseg0_program_mem";
          if (name.equals("bootconfig"))
            name = "kseg1_boot_mem";
          if (type == MemoryRegion.MemType.FLASHFUSE)
            name = "configsfrs";
          devProgramSpaces.add(mr.clone());
        }
        

        if ((!name.equals("bootcfg")) && ((type == MemoryRegion.MemType.BYTEFUSE) || (type == MemoryRegion.MemType.FLASHFUSE) || (name.equals("cfgmem")) || (name.equals("configsfrs")) || (name.equals("config")) || (name.equals("altconfig"))))
        {
          if (configWordsRegion == null) {
            configWordsRegion = mr.clone().alignToProgSize();
          }
          else {
            if (startAddr < configWordsRegion.startAddr)
              configWordsRegion.startAddr = startAddr;
            if (endAddr > configWordsRegion.endAddr)
              configWordsRegion.endAddr = endAddr;
          }
        }
        if (((name.equals("program")) || (name.equals("kseg0_program_mem"))) && (type == MemoryRegion.MemType.ROM))
        {
          mainExecutionRegion = mr.clone();
        }
      }
      

      if (targetPartition != 0)
      {
        if (devConfigWordsByName.containsKey(BACKBUGRegName))
          devSpecialConf.BACKBUGAddr = devConfigWordsByName.get(BACKBUGRegName)).startAddr;
        if (devConfigWordsByName.containsKey(ReservedBitRegName))
          devSpecialConf.reservedBitAddr = devConfigWordsByName.get(ReservedBitRegName)).startAddr;
        if (devConfigWordsByName.containsKey(CodeProtectRegName)) {
          devSpecialConf.codeProtectAddr = devConfigWordsByName.get(CodeProtectRegName)).startAddr;
        }
      }
      if (!hasFlashConfigWords)
        allowLastPageErase = true;
      igtSpacing = (mainExecutionRegion.endAddr < 65536L ? 2 : 4);
      edcInfoLoaded = true;
      



      long maxPossiblePSVSpace = mainFlashRegion != null ? mainFlashRegion.endAddr - mainFlashRegion.startAddr : 32768L;
      if (maxPossiblePSVSpace > 32768L)
      {
        maxPossiblePSVSpace = 32768L;
      }
      int reasonablePSVMin = (int)(maxPossiblePSVSpace - 6144L) / 4;
      if (reasonablePSVMin < eraseBlockSizeAddresses)
        reasonablePSVMin = eraseBlockSizeAddresses;
      if (minFreePSVSpace == 0)
      {
        minFreePSVSpace = reasonablePSVMin;
      }
      if (warnFreePSVSpace == 0)
      {
        minFreePSVSpace = (reasonablePSVMin * 2);
      }
      if (minFreePSVSpace > (int)(maxPossiblePSVSpace - 6144L))
        minFreePSVSpace = ((int)(maxPossiblePSVSpace - 6144L));
      if (warnFreePSVSpace > (int)(maxPossiblePSVSpace - 4096L))
        warnFreePSVSpace = ((int)(maxPossiblePSVSpace - 4096L));
      edcInfoLoaded = true;
      return true;
    }
    
    return false;
  }
  



























  public static List<AddressRange> computeSimpleUnion(List<Section> sections, List<DataRecord> dataRecords, List<AddressRange> addressRanges)
  {
    List<AddressRange> unionRanges = new ArrayList();
    
    if (sections != null)
    {
      for (Section sec : sections)
      {
        unionRanges.add(new AddressRange(loadMemoryAddress, loadMemoryAddress + size));
      }
    }
    if (dataRecords != null)
    {
      for (DataRecord rec : dataRecords)
      {
        unionRanges.add(new AddressRange(address, rec.getEndAddress()));
      }
    }
    if (addressRanges != null)
    {
      for (AddressRange ar : addressRanges)
      {
        unionRanges.add(new AddressRange(startAddr, endAddr));
      }
    }
    
    AddressRange.SortAndReduce(unionRanges);
    
    return unionRanges;
  }
  
  public int getEraseBlockSize()
  {
    return eraseBlockSize;
  }
  
  public void setEraseBlockSize(int instructionsPerEraseBlock) {
    eraseBlockSize = instructionsPerEraseBlock;
    eraseBlockSizeBytes = (MCU16Mode ? instructionsPerEraseBlock * 3 : instructionsPerEraseBlock * 4);
    eraseBlockSizeAddresses = (MCU16Mode ? eraseBlockSizeBytes / 3 * 2 : eraseBlockSizeBytes);
  }
  
  public int getEraseBlockSizeBytes() {
    return eraseBlockSizeBytes;
  }
  
  public void setEraseBlockSizeBytes(int eraseBlockSizeBytes) {
    this.eraseBlockSizeBytes = eraseBlockSizeBytes;
    eraseBlockSizeAddresses = (MCU16Mode ? eraseBlockSizeBytes / 3 * 2 : eraseBlockSizeBytes);
    eraseBlockSize = (MCU16Mode ? eraseBlockSizeBytes / 3 : eraseBlockSizeBytes / 4);
  }
  
  public int getEraseBlockSizeAddresses() {
    return eraseBlockSizeAddresses;
  }
  
  public void setEraseBlockSizeAddresses(int eraseBlockSizeAddresses) {
    this.eraseBlockSizeAddresses = eraseBlockSizeAddresses;
    eraseBlockSizeBytes = (MCU16Mode ? eraseBlockSizeAddresses / 2 * 3 : eraseBlockSizeAddresses);
    eraseBlockSize = (MCU16Mode ? eraseBlockSizeBytes / 3 : eraseBlockSizeBytes / 4);
  }
  
  public int getAppBootloadStateSize()
  {
    if (MCU16Mode)
      return 12;
    return 24;
  }
  







  public String getPassNumStr()
  {
    return getPassNumStr(pass);
  }
  
  public String getPassNumStr(int passNumber) {
    return String.format("%d%s", new Object[] { Integer.valueOf(passNumber), passNumber == 3 ? "rd" : passNumber == 2 ? "nd" : passNumber == 1 ? "st" : "th" });
  }
}
