package com.microchip.apps.ezbl;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;

























public class DumpParser
{
  private static EZBLState state = null;
  private static String stateSavePath = null;
  private static ProcessBuilder relinkProcBuilder = null;
  private static Process relinkProc = null;
  private static String linkCommandLine = null;
  
  private static String debuggableArtifactPath = null;
  private static String objDumpExecutable = null;
  private static int hexCharWidth = 8;
  private static String linkerFileData = null;
  
  private static final String firstPassResultsSectionRegexStart = "AUTOMATIC FIRST PASS RESULTS SECTION[^\n]*?\n";
  
  private static final String firstPassResultsSectionRegexEnd = "(?<=\n)[ \t]*?/[*][^E\n]*END OF FIRST PASS RESULTS SECTION";
  private static final String forwardBootloaderFlagsVariableName = "EZBL_ForwardBootloaderISR";
  
  public DumpParser() {}
  
  public static void main(String[] args)
  {
    List<DataRecord> eraseRestoreTable = new ArrayList();
    


    boolean bootContentOnConfigPage = false;
    String ivtDispatcher = "";
    String ivtMasks = "";
    String ivtVectorPointers = "";
    String ivtVectorSave = "";
    String bootReservedRAMMacro = "";
    int sizeOfAppGotoReset = 4;
    List<String> gldOutput = new ArrayList();
    

    int genROMDataAddrSize = 0;
    int genISRDispatchStubs = 0;
    

    if (state == null)
    {
      stateSavePath = CommandAndBuildState.GetStateFilePath(args);
      state = EZBLState.ReadFromFile(stateSavePath);
      state = CommandAndBuildState.ReadArgs(state, args);
      

      if (!stateparseOkay)
      {
        System.out.print("\r\n" + Multifunction.FormatHelpText(79, 0, "Usage:") + Multifunction.FormatHelpText(79, 3, "java -jar ezbl_tools.jar --dump_parser -pass=1 -elf_artifact=input.elf -linkscript=,--script=\"boot_proj_linker_file.[gld/ld]\"") + "\r\n");
        



        return;
      }
      
      if (stateMCU16Mode) {
        hexCharWidth = 6;
      }
      if ((stateartifactPath == null) || (statelinkScriptPath == null))
      {
        if (!statesilent)
          System.err.println("ezbl_tools: Missing required input parameter.");
        System.exit(-1);
      }
      
      debuggableArtifactPath = stateelfPath != null ? stateelfPath : stateartifactPath;
      if (!debuggableArtifactPath.toLowerCase().endsWith(".elf"))
      {
        debuggableArtifactPath = debuggableArtifactPath.replaceFirst("\\.[hH][eE][xX]$", ".elf");
      }
      if (!new File(debuggableArtifactPath).exists())
      {
        System.err.printf("ezbl_tools: \"%s\" could not be found\n", new Object[] { debuggableArtifactPath });
        System.exit(-5);
      }
      
      objDumpExecutable = "xc16-objdump";
      if (ELFReader.Machine(debuggableArtifactPath) == ELFReader.e_machine.EM_MIPS) {
        objDumpExecutable = "xc32-objdump";
      }
      
      linkerFileData = Multifunction.ReadFile(statelinkScriptPath, true);
      if (linkerFileData == null)
      {
        System.err.println("ezbl_tools: could not read \"" + statelinkScriptPath + "\"");
        System.exit(-5);
      }
    }
    

    stateelfDump = new ELFDumpParser();
    List<String> cmdLine;
    List<String> cmdLine; if (objDumpExecutable.contains("xc32-objdump"))
    {
      cmdLine = Arrays.asList(new String[] { statecompilerFolder + File.separator + objDumpExecutable, "--section-headers", "--syms", "--full-contents", debuggableArtifactPath });
    }
    else
    {
      cmdLine = Arrays.asList(new String[] { statecompilerFolder + File.separator + objDumpExecutable, "--section-headers", "--syms", debuggableArtifactPath });
      if (stateverbose)
        System.out.println("ezbl_tools: " + Multifunction.CatStringList(cmdLine, " "));
      if (stateelfDump.startObjDump(cmdLine) < 0)
      {
        System.err.printf("ezbl_tools: failed to execute '%1$s'\n", new Object[] { Multifunction.CatStringList(cmdLine, " ") });
        System.exit(-5);
      }
      cmdLine = Arrays.asList(new String[] { statecompilerFolder + File.separator + objDumpExecutable, "--full-contents", debuggableArtifactPath });
    }
    if (stateverbose)
      System.out.println("ezbl_tools: " + Multifunction.CatStringList(cmdLine, " "));
    if (stateelfDump.startObjDump(cmdLine) < 0)
    {
      System.err.printf("ezbl_tools: failed to execute '%1$s'\n", new Object[] { Multifunction.CatStringList(cmdLine, " ") });
      System.exit(-3);
    }
    

    stateromUseRegions = new ArrayList();
    stateramUseRegions = new ArrayList();
    stateramUseSections = new ArrayList();
    stateotherSections = new ArrayList();
    stateramSections = new ArrayList();
    stateromSections = new ArrayList();
    statenoProgramRegions = new ArrayList();
    statenoEraseRegions = new ArrayList();
    statenoVerifyRegions = new ArrayList();
    List<MemoryRegion> noAppSpaceRanges = new ArrayList();
    







    if ((statecoreType == EZBLState.CPUClass.mm) && (statebaseAddress == 0L))
    {
      statebaseAddress = 2634022912L;
    }
    
    String elfDumpText = stateelfDump.parseAllObjOutput(0);
    elfDumpText = elfDumpText + stateelfDump.parseAllObjOutput(0);
    
    boolean newDualPartitionMode = stateelfDump.symbolsByName.containsKey("__DUAL_PARTITION");
    if (statedualPartitionMode != newDualPartitionMode)
    {
      state.LoadEDCData();
    }
    
    if (statecoreType == EZBLState.CPUClass.mm) {
      stateelfDump.normalizePIC32Addresses();
    }
    if (statesaveTemps) {
      Multifunction.WriteFile(statetemporariesPath + "ezbl_objdump.txt", elfDumpText.getBytes(), false);
    } else {
      Multifunction.DeleteFile(statetemporariesPath + "ezbl_objdump.txt");
    }
    


    stateelfDump.removeSections("[.]text[.]EZBL_AppReserved.*");
    
    if ((statehasFlashConfigWords) && (stateconfigWordsRegion != null))
    {
      MemoryRegion configWordPageFreespace = stateconfigWordsRegion.clone().alignToEraseSize();
      MemoryRegion.SubtractRegion(configWordPageFreespace.getAsList(), stateconfigWordsRegion.clone().alignToProgSize());
      for (long addr = startAddr; addr < endAddr; addr += 2L)
      {
        if (stateelfDump.romSectionMapByAddr.containsKey(Long.valueOf(addr)))
        {
          Section lastPageSec = (Section)stateelfDump.romSectionMapByAddr.get(Long.valueOf(addr));
          if ((!name.startsWith("reserve_")) && (!name.startsWith(".dbg_code")) && (loadMemoryAddress != 3217032336L))
          {
            bootContentOnConfigPage = true;
            break;
          }
        }
      }
    }
    




    if ((!stateMCU16Mode) && (stateelfDump.symbolsByName.containsKey("_heap")) && (!stateelfDump.sectionsByName.containsKey(".heap")))
    {
      Section heap = new Section();
      alignment = 4;
      name = ".heap";
      flags = new SectionFlags();
      symbols = new ArrayList();
      symbolsByAddr = new HashMap();
      symbolsByName = new HashMap();
      heap.addSymbol((Symbol)stateelfDump.symbolsByName.get("_heap"));
      loadMemoryAddress = stateelfDump.symbolsByName.get("_heap")).address;
      virtualMemoryAddress = loadMemoryAddress;
      size = 0L;
      if (stateelfDump.symbolsByName.containsKey("__MIN_HEAP_SIZE"))
      {
        heap.addSymbol((Symbol)stateelfDump.symbolsByName.get("__MIN_HEAP_SIZE"));
        size = stateelfDump.symbolsByName.get("__MIN_HEAP_SIZE")).address;
      }
      if (stateelfDump.symbolsByName.containsKey("_min_heap_size"))
      {
        heap.addSymbol((Symbol)stateelfDump.symbolsByName.get("_min_heap_size"));
        size = stateelfDump.symbolsByName.get("_min_heap_size")).address;
      }
      if (stateelfDump.symbolsByName.containsKey("_eheap"))
      {
        size = (stateelfDump.symbolsByName.get("_eheap")).address - virtualMemoryAddress);
        heap.addSymbol((Symbol)stateelfDump.symbolsByName.get("_eheap"));
      }
      isRAM = true;
      flags.ALLOC = true;
      flags.NEVER_LOAD = true;
      stateelfDump.addSection(heap);
    }
    



    for (int secIndex = 0; secIndex < stateelfDump.sections.size(); secIndex++)
    {
      Section sec = (Section)stateelfDump.sections.get(secIndex);
      

      if ((name.equals(".icd")) || (name.startsWith("reserve_data_")) || (name.equals("reserve_boot_")) || ((statecoreType == EZBLState.CPUClass.mm) && (loadMemoryAddress == 3217032336L))) {
        statelinkedAsDebugImage = true;
      } else if (name.equals("EZBL_ICD_RAM_RESERVE")) {
        statelinkedAsDebugImage = false;
      }
      if (isDebug)
      {
        stateotherSections.add(sec);
      }
      else
      {
        MemoryRegion mr = sec.mapToDeviceRegion(statedevMemories, statedualPartitionMode ? MemoryRegion.Partition.partition1 : MemoryRegion.Partition.single);
        if (data != null)
          data.assignedMemory = name;
        mappedMemoryRegion = mr.clone();
        
        if (isROM)
        {
          mr.alignToProgSize();
          
          if ((statedevSpecialConf.FBOOTAddr > 0) && (loadMemoryAddress == statedevSpecialConf.FBOOTAddr))
          {
            statenoVerifyRegions.add(mr.clone());
          }
          

          stateromSections.add(sec);
          






          if ((!stateallowLastPageErase) || (stateconfigWordsRegion == null) || ((stateallowLastPageErase) && (endAddr <= stateconfigWordsRegion.startAddr) && (type != MemoryRegion.MemType.FLASHFUSE)) || ((stateMCU16Mode) && ((startAddr & 0xFF800000) == 0L) && ((!stateallowFirstPageErase) || ((stateallowFirstPageErase) && (startAddr >= stateeraseBlockSizeAddresses) && (statebaseAddress >= stateeraseBlockSizeAddresses)))))
          {



            if (stateMCU16Mode)
            {
              if (type == MemoryRegion.MemType.FLASHFUSE)
              {
                if ((bootContentOnConfigPage) || (!stateallowLastPageErase)) {
                  mr.alignToEraseSize();
                } else {
                  mr.alignToProgSize();
                }
              } else {
                mr.alignToEraseSize();
              }
            }
            else {
              mr.alignToEraseSizeWithoutOverlap(stateconfigWordsRegion.getAsList());
            }
          }
          



          MemoryRegion regionClone = mr.clone();
          if (comment == null)
            comment = "";
          comment += String.format(" %s: [0x%08X, 0x%08X)", new Object[] { name, Long.valueOf(loadMemoryAddress), Long.valueOf(loadMemoryAddress + size) });
          
          if (flags.NEVER_LOAD) {
            noAppSpaceRanges.add(regionClone);
          } else
            stateromUseRegions.add(regionClone);
          statenoProgramRegions.add(regionClone.clone());
          if ((stateconfigWordsRegion == null) || (bootContentOnConfigPage) || (endAddr <= stateconfigWordsRegion.startAddr) || (startAddr >= stateconfigWordsRegion.endAddr)) {
            statenoEraseRegions.add(regionClone.clone());
          }
          

        }
        else if (isRAM)
        {
          stateramSections.add(sec);
          

          if ((!name.startsWith("EZBL_ICD_RAM_RESERVE")) && (!name.equals("EZBL_APP_RAM_RESERVE")) && (!name.contains("EZBL_AppReservedRAMHole")) && (!name.equals(".stack")) && (!name.equals(".dbg_data")))
          {

            stateramUseRegions.add(mr.clone());
            stateramUseSections.add(sec);
          }
        }
        else
        {
          if (stateverbose)
          {
            System.err.println("ezbl_tools: Don't know what memory region the \"" + name + "\" section belongs in, so bootloader handling may be incorrect. This EZBL version may not be compatible with your compiler version.");
          }
          stateotherSections.add(sec);
        }
      }
    }
    Collections.sort(stateromSections);
    Collections.sort(stateramSections);
    

    List<MemoryRegion> bootloaderSpaces = new ArrayList();
    bootloaderSpaces.addAll(stateromUseRegions);
    bootloaderSpaces.addAll(noAppSpaceRanges);
    if ((stateallowLastPageErase) && (!bootContentOnConfigPage))
      bootloaderSpaces = MemoryRegion.SubtractRegions(bootloaderSpaces, stateconfigWordsRegion, false);
    bootloaderSpaces = MemoryRegion.coalesce(bootloaderSpaces, get0eraseAlign, get0eraseAlign, false);
    List<MemoryRegion> appSpaceRegions = MemoryRegion.SubtractRegions(statemainFlashRegion, bootloaderSpaces, false);
    long newAddressOfAppErasable = get0startAddr;
    if (!stateMCU16Mode)
      bootloaderSpaces = MemoryRegion.SubtractRegions(statemainFlashRegion, appSpaceRegions, false);
    long sizeOfBootloaderOccupiedSpace = 0L;
    for (MemoryRegion mr : bootloaderSpaces)
    {
      sizeOfBootloaderOccupiedSpace += endAddr - startAddr;
    }
    









    if ((statepass == 2) && (statebaseAddressOfAppErasable != newAddressOfAppErasable))
    {
      try
      {
        if (System.in.available() != 0)
        {
          InputStreamReader in = new InputStreamReader(System.in);
          char[] linkCommandsChars = new char[System.in.available()];
          in.read(linkCommandsChars);
          linkCommandLine = String.valueOf(linkCommandsChars);
        }
        if ((linkCommandLine != null) && (!linkCommandLine.isEmpty()))
        {
          System.out.printf("\nezbl_tools.jar: Bootloader changed geometry between pass %d and pass %d linking. A %s linking step will be invoked.\n", new Object[] { Integer.valueOf(statepass - 1), Integer.valueOf(statepass), state.getPassNumStr(statepass + 1) });
          System.out.println(linkCommandLine.trim());
          relinkProcBuilder = new ProcessBuilder(Multifunction.ParseCommandLineArguments(linkCommandLine));
          relinkProcBuilder.inheritIO();
        }
      }
      catch (IOException ex)
      {
        Logger.getLogger(DumpParser.class.getName()).log(Level.SEVERE, null, ex);
      }
      if (relinkProcBuilder == null)
      {
        System.err.printf("\nezbl_tools.jar: Bootloader changed geometry between pass %d and pass %d linking. Linking a %s time is required.\n\n", new Object[] { Integer.valueOf(statepass - 1), Integer.valueOf(statepass), state.getPassNumStr(statepass + 1) });
        System.exit(65235);
      }
    }
    else if ((statepass >= 3) && (statebaseAddressOfAppErasable != newAddressOfAppErasable))
    {
      System.err.printf("\nezbl_tools.jar: Linking a %s time did not successfully resolve the geometry. Try adding or removing a few bytes of code/flash constants.\n\n", new Object[] { state.getPassNumStr() });
      System.exit(65236);
    }
    statesizeOfIGT = (stateremapISRThroughIGT ? (stateivtVectorsImplemented + 1) * stateigtSpacing : 0);
    statesizeOfAppErasable = (state.getAppBootloadStateSize() + sizeOfAppGotoReset + statesizeOfIGT);
    statebaseAddressOfAppErasable = newAddressOfAppErasable;
    statebaseAddressOfGotoReset = (statebaseAddressOfAppErasable + state.getAppBootloadStateSize());
    statebaseAddressOfIGT = (statebaseAddressOfGotoReset + sizeOfAppGotoReset);
    




    MemoryRegion appBootloadStateRange = new MemoryRegion(statebaseAddressOfAppErasable, statebaseAddressOfAppErasable + state.getAppBootloadStateSize());
    appBootloadStateRange.copyMetaData(statemainExecutionRegion);
    eraseAlign = programAlign;
    statenoVerifyRegions.add(appBootloadStateRange);
    statenoProgramRegions.add(appBootloadStateRange);
    

    stateramUseRegions = MemoryRegion.coalesce(stateramUseRegions, 1, 1, true);
    stateromUseRegions = MemoryRegion.coalesce(stateromUseRegions, true, stateconfigWordsRegion.getAsList(), false);
    
    if (statecoreType == EZBLState.CPUClass.mm)
    {
      for (int i = 0; i < 256; i++)
      {
        Symbol vecSym = (Symbol)stateelfDump.symbolsByName.get(String.format("__vector_dispatch_%d", new Object[] { Integer.valueOf(i) }));
        if (vecSym != null)
        {

          String vecDesc = stateivtVectors.get(Integer.valueOf(i)) != null ? "" : stateivtVectors.get(Integer.valueOf(i))).desc != null ? "/* RAM pointer to " + stateivtVectors.get(Integer.valueOf(i))).desc + " ISR */" : "";
          




          long ISRAddr = 0L;
          String ISRFunctionName = "???";
          
          Section vectorSection = (Section)stateelfDump.romSectionMapByAddr.get(Long.valueOf(address));
          if (vectorSection != null)
          {
            ISRAddr = data.GetIntDataAtAddress(address);
            ISRAddr = ((ISRAddr >> 16 & 0xFFFF | ISRAddr << 16) & 0x3FFFFFF) << 1 | 0x9D000001;
            Symbol ISRFunctionSym = (Symbol)stateelfDump.functionSymsByAddr.get(Long.valueOf(ISRAddr - 1L));
            if (ISRFunctionSym != null)
              ISRFunctionName = name;
            if (ISRFunctionName.contains("_DefaultInterrupt")) {}
          }
          else
          {
            ivtVectorPointers = ivtVectorPointers + String.format("\n    %-92s", new Object[] { String.format("EZBL_Vector%dISRPtr = .; %s", new Object[] { Integer.valueOf(i), vecDesc }) });
            ivtVectorPointers = ivtVectorPointers + String.format("\n    %-92s", new Object[] { String.format(". += 4; /* = 0x%08X for Boot %s() ISR or App ISR address for IRQ %d */", new Object[] { Long.valueOf(ISRAddr), ISRFunctionName, Integer.valueOf(i) }) });
            
            ivtDispatcher = ivtDispatcher + String.format("\n    %1$-92s", new Object[] { String.format(".vector_%1$d_EZBL_RAMRedirect _ebase_address + 0x200 + ((_vector_spacing << 3) * %1$d) :", new Object[] { Integer.valueOf(i) }) });
            ivtDispatcher = ivtDispatcher + String.format("\n    %1$-92s", new Object[] { String.format("{", new Object[0]) });
            ivtDispatcher = ivtDispatcher + String.format("\n    %1$-92s", new Object[] { String.format("  LW32(26, 28, ABSOLUTE(EZBL_Vector%1$dISRPtr) - ABSOLUTE(_gp)); /* k0 = 26, gp = 28 */", new Object[] { Integer.valueOf(i) }) });
            ivtDispatcher = ivtDispatcher + String.format("\n    %1$-92s", new Object[] { String.format("  JRC16(26);", new Object[0]) });
            ivtDispatcher = ivtDispatcher + String.format("\n    %1$-92s", new Object[] { String.format("  NOP16();", new Object[0]) });
            ivtDispatcher = ivtDispatcher + String.format("\n    %1$-92s", new Object[] { String.format("} > kseg0_program_mem", new Object[0]) });
            
            ivtVectorSave = ivtVectorSave + String.format("\n    %1$-92s", new Object[] { String.format("  LONG(%d);", new Object[] { Integer.valueOf(i) }) });
            ivtVectorSave = ivtVectorSave + String.format("\n    %1$-92s", new Object[] { String.format("  EZBL_Vector%dOriginal = ABSOLUTE(.);", new Object[] { Integer.valueOf(i) }) });
            ivtVectorSave = ivtVectorSave + String.format("\n    %1$-92s", new Object[] { String.format("  KEEP(*(.vector_%d));", new Object[] { Integer.valueOf(i) }) });
            genROMDataAddrSize += 12;
          }
        } }
      bootReservedRAMMacro = "";
      for (MemoryRegion r : stateramUseRegions)
      {
        bootReservedRAMMacro = bootReservedRAMMacro + String.format("\n    %-92s", new Object[] { String.format("  EZBL_RAM_AT_0x%1$08X 0x%1$08X :", new Object[] { Long.valueOf(startAddr) }) });
        bootReservedRAMMacro = bootReservedRAMMacro + String.format("\n    %-92s", new Object[] { String.format("  {", new Object[0]) });
        bootReservedRAMMacro = bootReservedRAMMacro + String.format("\n    %-92s", new Object[] { String.format("    *(EZBL_RAM_AT_0x%1$08X); /* [0x%1$08X, 0x%3$08X), contains %2$d bytes */", new Object[] { Long.valueOf(startAddr), Long.valueOf(endAddr - startAddr), Long.valueOf(endAddr) }) });
        bootReservedRAMMacro = bootReservedRAMMacro + String.format("\n    %-92s", new Object[] { String.format("  } > %s", new Object[] { name }) });

      }
      

    }
    else if (stateremapISRThroughIGT)
    {
      int igtIndex = 0;
      
      for (int i = 0; i < stateivtVectors.size(); i++)
      {
        InterruptVector vector = (InterruptVector)stateivtVectors.get(Integer.valueOf(i));
        Symbol sym = (Symbol)stateelfDump.symbolsByName.get("__" + name);
        
        int igtAddress = (int)statebaseAddressOfIGT + stateigtSpacing * igtIndex;
        if (!implemented) {
          igtAddress = (int)statebaseAddressOfIGT + stateigtSpacing * stateivtVectorsImplemented;
        }
        if ((sym == null) || (genISRDispatchStubs > 32))
        {
          if ((sym != null) && (genISRDispatchStubs > 32))
          {
            System.err.print(Multifunction.FormatHelpText(110, 0, String.format("ezbl_tools: warning: There are more than 32 interrupts defined in your bootloader. EZBL only supports up to 32 run-time choosable ISRs in your Bootloader. Building will proceed by not hooking the Bootloader's %1$s() function up in the IVT and will instead always forward this Interrupt to the Application IGT.", new Object[] { name })));

          }
          


        }
        else
        {


          String shortIntName = name.replace("Interrupt", "").replaceFirst("_", "");
          ivtMasks = ivtMasks + String.format("\n%1$-32s = 0x%2$08X;\n%3$-32s = %4$d;\n%5$-32s = %6$d;", new Object[] { "_EZBL_FORWARD_MASK_" + shortIntName, Integer.valueOf(1 << genISRDispatchStubs), "_EZBL_FORWARD_POS_" + shortIntName, Integer.valueOf(genISRDispatchStubs), "_EZBL_FORWARD_IRQ_" + shortIntName, Integer.valueOf(irqNum) });
          





          ivtDispatcher = ivtDispatcher + String.format("%1$-96s", new Object[] { "\n    EZBL_Dispatch_" + name + " = ABSOLUTE(.);" }) + String.format("%1$-49s", new Object[] { " /* EZBL_Dispatch_" + name + ":" }) + " */ " + String.format("%1$-96s", new Object[] { String.format("\n    BTSC(ABSOLUTE(_EZBL_ForwardBootloaderISR) + 0x%1$X, %2$d);", new Object[] { Integer.valueOf(genISRDispatchStubs / 16 * 2), Integer.valueOf(genISRDispatchStubs % 16) }) }) + String.format("%1$-49s", new Object[] { String.format(" /*    btsc    _EZBL_ForwardBootloaderISR, #%1$d", new Object[] { Integer.valueOf(genISRDispatchStubs) }) }) + " */ " + String.format("%1$-96s", new Object[] { String.format("\n    %1$s(ABSOLUTE(0x%2$06X));", new Object[] { stateigtSpacing == 2 ? "BRA" : "GOTO", Integer.valueOf(igtAddress) }) }) + String.format("%1$-49s", new Object[] { String.format(" /*    %1$s    ApplicationIGT(__%2$s)", new Object[] { stateigtSpacing == 2 ? "bra" : "goto", name }) }) + " */ " + String.format("%1$-96s", new Object[] { String.format("\n    %1$s(DEFINED(__" + name + ") ? ABSOLUTE(__" + name + ") : ABSOLUTE(0x%2$06X));", new Object[] { stateigtSpacing == 2 ? "BRA" : "GOTO", Integer.valueOf(igtAddress) }) }) + String.format("%1$-49s", new Object[] { String.format(" /*    %1$s    BootloaderISR(__%2$s)", new Object[] { stateigtSpacing == 2 ? "bra" : "goto", name }) }) + " */ ";
          


          genISRDispatchStubs++;
        }
        


        if (stateivtVectors.get(Integer.valueOf(i))).implemented) {
          igtIndex++;
        }
      }
      genROMDataAddrSize += (2 + 2 * stateigtSpacing) * genISRDispatchStubs;
    }
    
    ivtDispatcher = ivtDispatcher.replaceAll("\n", Matcher.quoteReplacement("\\\n"));
    ivtVectorPointers = ivtVectorPointers.replaceAll("\n", Matcher.quoteReplacement("\\\n"));
    ivtVectorSave = ivtVectorSave.replaceAll("\n", Matcher.quoteReplacement("\\\n"));
    bootReservedRAMMacro = bootReservedRAMMacro.replaceAll("\n", Matcher.quoteReplacement("\\\n"));
    






    for (Section sec : stateromSections)
    {

      if ((stateMCU16Mode) && (stateallowFirstPageErase))
      {
        if (loadMemoryAddress < 0L + stateeraseBlockSizeAddresses)
        {
          if (data == null)
          {
            System.err.print("ezbl_tools: failed to get section data from object dump.\n");
            System.exit(-4);
          }
          
          DataRecord restoreRecord = data.Clone();
          comment = "";
          assignedMemory = "";
          
          if (restoreRecord.getEndAddress() > 0L + stateeraseBlockSizeAddresses)
            restoreRecord = restoreRecord.SplitAtAddress(stateeraseBlockSizeAddresses);
          eraseRestoreTable.add(restoreRecord);
        }
      }
      
      if ((statehasFlashConfigWords) && (stateallowLastPageErase) && (!bootContentOnConfigPage) && (stateconfigWordsRegion != null))
      {

        if ((loadMemoryAddress + size > stateconfigWordsRegion.startAddr) && (loadMemoryAddress < stateconfigWordsRegion.endAddr))
        {
          if (data == null)
          {
            System.err.print("ezbl_tools: failed to get section data from object dump.\n");
            System.exit(-4);
          }
          DataRecord restoreRecord = data.Clone();
          if (data.address < stateconfigWordsRegion.startAddr)
            restoreRecord = restoreRecord.SplitAtAddress(stateconfigWordsRegion.startAddr);
          eraseRestoreTable.add(restoreRecord);
          continue;
        }
      }
      

      if ((!statehasFlashConfigWords) && (loadMemoryAddress >= 8388608L))
      {
        if (data == null)
        {
          System.err.print("ezbl_tools: failed to get section data from object dump.\n");
          System.exit(-4);
        }
        eraseRestoreTable.add(data.Clone());
      }
    }
    


    if ((statelinkedAsDebugImage) && (statedevSpecialConf.BACKBUGAddr != 0L))
    {
      MemoryRegion specialCfgWord = ((MemoryRegion)statedevConfigWordsByName.get(statedevSpecialConf.BACKBUGConfigName)).clone().alignToProgSize();
      eraseAlign = programAlign;
      DataRecord cfgRecord = DataRecord.ExtractRange(eraseRestoreTable, startAddr, endAddr);
      if (cfgRecord == null)
      {
        cfgRecord = new DataRecord();
        address = startAddr;
        architecture16Bit = stateMCU16Mode;
        assignedMemory = (stateMCU16Mode ? statedevSpecialConf.BACKBUGConfigName : "configsfrs");
        comment = "Generated section for debugging after bootloader erase of Config words flash page";
        data = new byte[(int)(stateMCU16Mode ? (endAddr - startAddr) / 2L * 3L : endAddr - startAddr)];
        Arrays.fill(data, (byte)-1);
      }
      int i = (int)(statedevSpecialConf.BACKBUGAddr - startAddr);
      if (stateMCU16Mode)
        i = i / 2 * 3;
      int tmp6032_6031 = (i + 0); byte[] tmp6032_6025 = data;tmp6032_6025[tmp6032_6031] = ((byte)(tmp6032_6025[tmp6032_6031] & (byte)(int)((statedevSpecialConf.BACKBUGMask ^ 0xFFFFFFFFFFFFFFFF) & 0xFF))); int 
        tmp6065_6064 = (i + 1); byte[] tmp6065_6058 = data;tmp6065_6058[tmp6065_6064] = ((byte)(tmp6065_6058[tmp6065_6064] & (byte)(int)(((statedevSpecialConf.BACKBUGMask ^ 0xFFFFFFFFFFFFFFFF) & 0xFF00) >> 8))); int 
        tmp6101_6100 = (i + 2); byte[] tmp6101_6094 = data;tmp6101_6094[tmp6101_6100] = ((byte)(tmp6101_6094[tmp6101_6100] & (byte)(int)(((statedevSpecialConf.BACKBUGMask ^ 0xFFFFFFFFFFFFFFFF) & 0xFF0000) >> 16)));
      if (!stateMCU16Mode) {
        int tmp6146_6145 = (i + 3); byte[] tmp6146_6139 = data;tmp6146_6139[tmp6146_6145] = ((byte)(tmp6146_6139[tmp6146_6145] & (byte)(int)(((statedevSpecialConf.BACKBUGMask ^ 0xFFFFFFFFFFFFFFFF) & 0xFF000000) >> 24))); }
      if (!bootContentOnConfigPage)
        eraseRestoreTable.add(cfgRecord);
      statenoVerifyRegions.add(specialCfgWord);
      stateromUseRegions.add(specialCfgWord);
      noAppSpaceRanges.add(specialCfgWord);
      if (statecoreType == EZBLState.CPUClass.mm)
      {
        statenoProgramRegions.add(specialCfgWord);
        statenoEraseRegions.add(specialCfgWord);
      }
    }
    

    DataRecord.CoalesceRecords(eraseRestoreTable, true, stateflashWordSize, stateflashWordSize);
    

    if (statedevSpecialConf.reservedBitAddr != 0L)
    {
      MemoryRegion specialCfgWord = ((MemoryRegion)statedevConfigWordsByName.get(statedevSpecialConf.reserveBitConfigName)).clone().alignToProgSize();
      eraseAlign = programAlign;
      DataRecord cfgRecord = DataRecord.ExtractRange(eraseRestoreTable, startAddr, endAddr);
      if (cfgRecord == null)
      {
        cfgRecord = new DataRecord();
        address = startAddr;
        architecture16Bit = stateMCU16Mode;
        assignedMemory = statedevSpecialConf.reserveBitConfigName;
        comment = "Generated section for clearing Reserved Bit";
        data = new byte[(int)(stateMCU16Mode ? (endAddr - startAddr) / 2L * 3L : endAddr - startAddr)];
        Arrays.fill(data, (byte)-1);
      }
      int i = (int)(statedevSpecialConf.reservedBitAddr - startAddr);
      if (stateMCU16Mode)
        i = i / 2 * 3;
      int tmp6512_6511 = (i + 0); byte[] tmp6512_6505 = data;tmp6512_6505[tmp6512_6511] = ((byte)(tmp6512_6505[tmp6512_6511] & (byte)(int)((statedevSpecialConf.reservedBitMask ^ 0xFFFFFFFFFFFFFFFF) & 0xFF))); int 
        tmp6545_6544 = (i + 1); byte[] tmp6545_6538 = data;tmp6545_6538[tmp6545_6544] = ((byte)(tmp6545_6538[tmp6545_6544] & (byte)(int)(((statedevSpecialConf.reservedBitMask ^ 0xFFFFFFFFFFFFFFFF) & 0xFF00) >> 8))); int 
        tmp6581_6580 = (i + 2); byte[] tmp6581_6574 = data;tmp6581_6574[tmp6581_6580] = ((byte)(tmp6581_6574[tmp6581_6580] & (byte)(int)(((statedevSpecialConf.reservedBitMask ^ 0xFFFFFFFFFFFFFFFF) & 0xFF0000) >> 16)));
      if (!stateMCU16Mode) {
        int tmp6626_6625 = (i + 3); byte[] tmp6626_6619 = data;tmp6626_6619[tmp6626_6625] = ((byte)(tmp6626_6619[tmp6626_6625] & (byte)(int)(((statedevSpecialConf.reservedBitMask ^ 0xFFFFFFFFFFFFFFFF) & 0xFF000000) >> 24))); }
      if (stateMCU16Mode)
      {
        if (!bootContentOnConfigPage) {
          eraseRestoreTable.add(cfgRecord);
        }
        DataRecord.CoalesceRecords(eraseRestoreTable, false);
        stateromUseRegions.add(specialCfgWord);
      }
      statenoVerifyRegions.add(specialCfgWord);
      noAppSpaceRanges.add(specialCfgWord);
      if (statecoreType == EZBLState.CPUClass.mm)
      {
        statenoProgramRegions.add(specialCfgWord);
        statenoEraseRegions.add(specialCfgWord);
      }
    }
    

    if ((statehasFlashConfigWords) && (stateconfigWordsRegion != null) && ((!stateallowLastPageErase) || (bootContentOnConfigPage)))
    {
      statenoVerifyRegions.addAll(MemoryRegion.SubtractRegions(stateconfigWordsRegion.getAsList(), Section.getMappedMemoryRegions(stateromSections), false));
    }
    
    if (stateverbose)
    {
      List<MemoryRegion> paddingAlignmentRegions = MemoryRegion.SubtractRegions(stateromUseRegions, Section.getMappedMemoryRegions(stateromSections), false);
      long totalPad = 0L;
      for (MemoryRegion mr : paddingAlignmentRegions)
      {
        System.err.printf("    Padding Range: [0x%06X, 0x%06X)\n", new Object[] { Long.valueOf(startAddr), Long.valueOf(endAddr) });
        totalPad += endAddr - startAddr;
      }
      System.err.printf("    Total Padding/Alignment space: 0x%06X (%d bytes)\n", new Object[] { Long.valueOf(totalPad), Long.valueOf(totalPad / 2L * 3L) });
    }
    

    noAppSpaceRanges = MemoryRegion.coalesce(noAppSpaceRanges, 0, 0, false);
    statenoProgramRegions = MemoryRegion.coalesce(statenoProgramRegions, true, (stateallowLastPageErase) && (!bootContentOnConfigPage) ? stateconfigWordsRegion.getAsList() : null, false);
    statenoEraseRegions = MemoryRegion.coalesce(statenoEraseRegions, true, (stateallowLastPageErase) && (!bootContentOnConfigPage) ? stateconfigWordsRegion.getAsList() : null, false);
    statenoVerifyRegions = MemoryRegion.coalesce(statenoVerifyRegions, true, (stateallowLastPageErase) && (!bootContentOnConfigPage) ? stateconfigWordsRegion.getAsList() : null, false);
    

    List<MemoryRegion> appSpaceGeometry = MemoryRegion.SubtractRegions(statedevNVGeometry, stateromUseRegions, false);
    MemoryRegion.alignToEraseSize(noAppSpaceRanges, !bootContentOnConfigPage ? stateconfigWordsRegion.getAsList() : null);
    appSpaceGeometry = MemoryRegion.SubtractRegions(appSpaceGeometry, noAppSpaceRanges, false);
    

    for (DataRecord dr : eraseRestoreTable)
    {
      genROMDataAddrSize = (int)(genROMDataAddrSize + ((stateMCU16Mode ? 4 : 8) + (dr.getEndAddress() - address)));
    }
    genROMDataAddrSize += (stateMCU16Mode ? 4 : 8) * (appSpaceGeometry.size() + statenoProgramRegions.size() + statenoEraseRegions.size() + statenoVerifyRegions.size());
    























    gldOutput.add("/* EZBL bootloader data generated during historical project linking passes */\n" + String.format("\n/* ezbl_tools generated rom size = 0x%06X */\n", new Object[] { Integer.valueOf(genROMDataAddrSize) }));
    
    gldOutput.add(String.format("\n%-32s =   0x%08X;", new Object[] { (stateMCU16Mode ? "_" : "") + "EZBL_BOOTLOADER_SIZE", Long.valueOf(sizeOfBootloaderOccupiedSpace) }));
    
    if (stateMCU16Mode)
    {
      gldOutput.add(String.format("\n%-32s =   0x%08X;", new Object[] { "_EZBL_appBootloadState", Long.valueOf(statebaseAddressOfAppErasable) }) + String.format("\n%-32s =   0x%08X;", new Object[] { "_EZBL_APP_RESET_BASE", Long.valueOf(statebaseAddressOfGotoReset) }) + String.format("\n%-32s =   0x%08X;", new Object[] { "_EZBL_IGT_BASE", Long.valueOf(statebaseAddressOfIGT) }) + "\n");
      


      if (stateremapISRThroughIGT)
      {
        gldOutput.add("\n/* Bit mask flags for EZBL_ForwardBootloaderISR */" + ivtMasks + "\n" + "\n");
      }
    }
    

    gldOutput.add("\n");
    
    if (stateMCU16Mode)
      gldOutput.add(genLinkerMacro("__EZBL_TOOLS_COMPUTED_BOOT_RAM_USE /* FYI - not stored/used */", stateramUseRegions, hexCharWidth));
    gldOutput.add(genLinkerMacro("__EZBL_TOOLS_COMPUTED_APP_SPACE_GEOMETRY", appSpaceGeometry, hexCharWidth));
    gldOutput.add(genLinkerMacro("__EZBL_TOOLS_COMPUTED_NO_PROGRAM_RANGES", statenoProgramRegions, hexCharWidth));
    gldOutput.add(genLinkerMacro("__EZBL_TOOLS_COMPUTED_NO_ERASE_RANGES", statenoEraseRegions, hexCharWidth));
    gldOutput.add(genLinkerMacro("__EZBL_TOOLS_COMPUTED_NO_VERIFY_RANGES", statenoVerifyRegions, hexCharWidth));
    gldOutput.add(genLinkerMacro("__EZBL_TOOLS_COMPUTED_ERASE_RESTORE_TABLE", eraseRestoreTable));
    



    gldOutput.add(String.format("\n%-96s", new Object[] { "#define __EZBL_IVT_DISPATCH_CODE " }) + ivtDispatcher + "\n");
    


    if (!stateMCU16Mode)
    {
      gldOutput.add(String.format("\n%-96s", new Object[] { "#define __EZBL_RAM_POINTERS_TO_ISRS " }) + ivtVectorPointers + "\n");
      


      gldOutput.add(String.format("\n%-96s", new Object[] { "#define __EZBL_IVT_VECTOR_SAVE " }) + ivtVectorSave + "\n");
      


      gldOutput.add(String.format("\n%-96s", new Object[] { "#define __EZBL_TOOLS_COMPUTED_BOOTLOADER_RESERVED_RAM " }) + bootReservedRAMMacro + "\n");
    }
    



    TextBlock outGLD = new TextBlock(linkerFileData);
    
    if (!outGLD.Find("AUTOMATIC FIRST PASS RESULTS SECTION[^\n]*?\n", "(?<=\n)[ \t]*?/[*][^E\n]*END OF FIRST PASS RESULTS SECTION"))
    {
      System.err.println("ezbl_tools: could not find 'AUTOMATIC FIRST PASS RESULTS SECTION' to write first pass linking results in \"" + statelinkScriptPath + "\"");
      System.exit(-6);
    }
    
    String outputValuesString = Multifunction.CatStringList(gldOutput);
    outGLD.ReplaceInner(outputValuesString);
    

    if (Multifunction.UpdateFileIfDataDifferent(statelinkScriptPath, outGLD.GetFullBlock(), true) < 0)
    {
      System.err.println("ezbl_tools: failed to write to \"" + statelinkScriptPath + "\"");
      System.exit(-7);
    }
    

    if (relinkProcBuilder != null)
    {
      int linkReturn = 65234;
      try
      {
        relinkProc = relinkProcBuilder.start();
        linkReturn = relinkProc.waitFor();
        relinkProc = null;
        relinkProcBuilder = null;
        if (linkReturn == 0)
        {
          statepass += 1;
          main(args);
          return;
        }
      }
      catch (InterruptedException ex)
      {
        Logger.getLogger(DumpParser.class.getName()).log(Level.SEVERE, null, ex);
      }
      catch (IOException ex)
      {
        System.err.printf("\nezbl_tools.jar: failed to launch %s linking process.\n", new Object[] { state.getPassNumStr() });
        System.exit(65235);
      }
      System.exit(linkReturn);
    }
    

    state.SaveToFile();
  }
  
  private static String genLinkerMacro(String elementNameAndComment, List<MemoryRegion> regions, int longCharWidth)
  {
    int lineFormatWidth = 80;
    List<String> ret = new ArrayList();
    int indentWidth = 4;
    String numFormat = "0x%X";
    String lineFormat = "\n%-" + String.valueOf(lineFormatWidth) + "s";
    String indentedLineFormat = "\\\n    %-" + String.valueOf(lineFormatWidth - 4) + "s";
    
    if (longCharWidth > 0)
      numFormat = "0x%0" + String.valueOf(longCharWidth) + "X";
    if (elementNameAndComment.length() > lineFormatWidth)
    {
      lineFormatWidth = elementNameAndComment.length() + 16 - elementNameAndComment.length() % 16;
      lineFormat = "\n%-" + String.valueOf(lineFormatWidth) + "s";
      indentedLineFormat = String.format(new StringBuilder().append("\\\n%").append(String.valueOf(indentWidth)).append("s").toString(), new Object[] { "" }) + "%-" + String.valueOf(lineFormatWidth - indentWidth) + "s";
    }
    
    ret.add(String.format(lineFormat, new Object[] { String.format("#define " + elementNameAndComment, new Object[0]) }));
    for (MemoryRegion r : regions)
    {
      ret.add(String.format(indentedLineFormat, new Object[] { String.format("LONG(" + numFormat + "); LONG(" + numFormat + ");", new Object[] { Long.valueOf(startAddr), Long.valueOf(endAddr) }) }));
    }
    
    if (ret.size() == 1) {
      ret.set(ret.size() - 1, "\n" + ((String)ret.get(ret.size() - 1)).trim());
    } else
      ret.set(ret.size() - 1, ((String)ret.get(ret.size() - 1)).trim());
    ret.add("\n");
    return Multifunction.CatStringList(ret);
  }
  
  private static String genLinkerMacro(String elementNameAndComment, List<DataRecord> records)
  {
    int lineFormatWidth = 96;
    List<String> ret = new ArrayList();
    int indentWidth = 4;
    
    String lineFormat = "\n%-" + String.valueOf(lineFormatWidth) + "s";
    String indentedLineFormat = "\\\n    %-" + String.valueOf(lineFormatWidth - 4) + "s";
    
    int longCharWidth = 4;
    if (!records.isEmpty())
      longCharWidth = get0architecture16Bit ? 6 : 8;
    String numFormat = "0x%0" + String.valueOf(longCharWidth) + "X";
    
    if (elementNameAndComment.length() > lineFormatWidth)
    {
      lineFormatWidth = elementNameAndComment.length() + 16 - elementNameAndComment.length() % 16;
      lineFormat = "\n%-" + String.valueOf(lineFormatWidth) + "s";
      indentedLineFormat = String.format(new StringBuilder().append("\\\n%-").append(String.valueOf(indentWidth)).append("s").toString(), new Object[] { "" }) + "%-" + String.valueOf(lineFormatWidth) + "s";
    }
    
    ret.add(String.format(lineFormat, new Object[] { String.format("#define " + elementNameAndComment, new Object[0]) }));
    
    for (DataRecord r : records)
    {
      String restoreLine = String.format("LONG(" + numFormat + "); LONG(" + numFormat + "); /* Start address, byte length [" + numFormat + ", " + numFormat + ") */ ", new Object[] { Long.valueOf(address), Integer.valueOf(data.length), Long.valueOf(address), Long.valueOf(r.getEndAddress()) });
      ret.add(String.format(indentedLineFormat, new Object[] { restoreLine }));
      restoreLine = "";
      for (long addr = address; addr < r.getEndAddress(); addr += (architecture16Bit ? 2 : 4))
      {
        if (((addr - address) % (architecture16Bit ? 10 : 16) == 0L) && (!restoreLine.isEmpty()))
        {
          ret.add(String.format(indentedLineFormat, new Object[] { restoreLine }));
          restoreLine = "";
        }
        restoreLine = restoreLine + String.format(new StringBuilder().append("LONG(").append(numFormat).append("); ").toString(), new Object[] { Integer.valueOf(r.GetIntDataAtAddress(addr)) });
      }
      if (!restoreLine.isEmpty()) {
        ret.add(String.format(indentedLineFormat, new Object[] { restoreLine }));
      }
    }
    if (ret.size() == 1) {
      ret.set(ret.size() - 1, "\n" + ((String)ret.get(ret.size() - 1)).trim());
    } else
      ret.set(ret.size() - 1, ((String)ret.get(ret.size() - 1)).trim());
    ret.add("\n");
    return Multifunction.CatStringList(ret);
  }
  
  private static String genLinkerMacro(String elementNameAndComment, boolean architecture16Bit, int[] literalDataToStore)
  {
    int longCharWidth = architecture16Bit ? 6 : 8;
    int lineFormatWidth = 80;
    List<String> ret = new ArrayList();
    int indentWidth = 4;
    String numFormat = "0x%0" + String.valueOf(longCharWidth) + "X";
    String lineFormat = "\n%-" + String.valueOf(lineFormatWidth) + "s";
    String indentedLineFormat = "\\\n    %-" + String.valueOf(lineFormatWidth - 4) + "s";
    int wordsPerLine = 4;
    
    if (elementNameAndComment.length() > lineFormatWidth)
    {
      lineFormatWidth = elementNameAndComment.length() + 16 - elementNameAndComment.length() % 16;
      lineFormat = "\n%-" + String.valueOf(lineFormatWidth) + "s";
      indentedLineFormat = String.format(new StringBuilder().append("\\\n%").append(String.valueOf(indentWidth)).append("s").toString(), new Object[] { "" }) + "%-" + String.valueOf(lineFormatWidth - indentWidth) + "s";
    }
    
    ret.add(String.format(lineFormat, new Object[] { String.format("#define " + elementNameAndComment, new Object[0]) }));
    for (int i = 0; i < literalDataToStore.length; i += wordsPerLine)
    {
      String s = "";
      for (int j = 0; j < wordsPerLine; j++)
      {
        if (i + j >= literalDataToStore.length)
          break;
        s = s + String.format(new StringBuilder().append("LONG(").append(numFormat).append("); ").toString(), new Object[] { Integer.valueOf(literalDataToStore[(i + j)]) });
      }
      ret.add(String.format(indentedLineFormat, new Object[] { s }));
    }
    
    if (ret.size() == 1) {
      ret.set(ret.size() - 1, "\n" + ((String)ret.get(ret.size() - 1)).trim());
    } else
      ret.set(ret.size() - 1, ((String)ret.get(ret.size() - 1)).trim());
    ret.add("\n");
    return Multifunction.CatStringList(ret);
  }
}
