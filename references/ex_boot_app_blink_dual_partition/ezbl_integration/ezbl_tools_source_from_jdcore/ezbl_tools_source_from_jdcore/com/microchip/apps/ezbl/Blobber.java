package com.microchip.apps.ezbl;

import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.zip.CRC32;



































public class Blobber
{
  public Blobber() {}
  
  public static void main(String[] args)
  {
    String outputFilename = null;
    String outputExtension = ".bl2";
    
    List<DataRecord> rawInputs = new ArrayList();
    





    boolean binOut = false;
    boolean rowCompressed = false;
    boolean addPhantoms = false;
    boolean showHelp = false;
    ELFDumpParser metaParser = null;
    List<AddressRange> voidRanges = new ArrayList();
    List<AddressRange> fillRanges = new ArrayList();
    List<Byte> fillValues = new ArrayList();
    List<AddressRange> padRanges = new ArrayList();
    List<Byte> padValues = new ArrayList();
    List<AddressRange> crcRanges = new ArrayList();
    List<Long> crcDestinations = new ArrayList();
    List<AddressRange> movRanges = new ArrayList();
    List<Long> movDestinations = new ArrayList();
    
    EZBLState state = new EZBLState();
    

    for (String arg : args)
    {
      if (arg.toLowerCase().contains("-generate_merge"))
      {
        String stateSavePath = CommandAndBuildState.GetStateFilePath(args);
        state = EZBLState.ReadFromFile(stateSavePath);
        break;
      }
      if (arg.toLowerCase().matches("(--help)|(-help)|(/\\?)|(-h)|(--h)"))
        showHelp = true;
    }
    state = CommandAndBuildState.ReadArgs(state, args);
    

    boolean generate_merge = undecodedOptions.contains("-generate_merge");
    boolean dump = undecodedOptions.contains("-dump");
    





    if ((dump) && (artifactPath != null) && (artifactPath.matches(".*?[.](([hH][eE][xX])|([bB][lL]2)|([bB][lL][oO][bB]))$")))
    {
      String dumpText = "";
      byte[] fileBytes = Multifunction.ReadFileBinary(hexPath);
      
      if (fileBytes == null)
      {
        System.err.println("ezbl_tools: Unable to read \"" + hexPath + "\"");
        System.exit(-2);
      }
      
      if (undecodedOptions.contains("-dump_hex"))
      {
        dumpText = Bl2b.HexToReadable(artifactPath);
      }
      else if (artifactPath.matches(".*?[.][hH][eE][xX]$"))
      {
        dumpText = Bl2b.GetObjDumpView(new String(fileBytes));
      }
      else if (artifactPath.matches(".*?[.][bB][lL]2$"))
      {
        dumpText = Bl2b.GetObjDumpView(fileBytes);
      }
      else
      {
        dumpText = Blob.GetObjDumpView(fileBytes, MCU16Mode);
      }
      System.out.print(dumpText);
      return;
    }
    

    if ((!parseOkay) || (showHelp))
    {
      System.out.print("\r\n" + Multifunction.FormatHelpText(79, 0, "Usage:") + Multifunction.FormatHelpText(79, 3, "java -jar ezbl_tools.jar --blobber [-options] -artifact=\"input.elf\"") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Inputs:") + Multifunction.FormatHelpText(79, 6, "-artifact='input.[elf/bl2/blob/hex]' specifies the input filename. .elf files are recommened as they contain the BOOTID_HASH and APPID_VER meta data that is needed for the .bl2 file output headers. However, a suitable xc16-objdump or xc32-objdump executable, as applicable, must be in the system path for .elf parsing. Other formats supported are .hex, .bl2, and (older) .blob files (do not require objdump in path).") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Outputs:") + Multifunction.FormatHelpText(79, 6, "Input file decoded from .hex/.elf format and reencoded as a binary .bl2 file. The output filename is input.bl2.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-dump") + Multifunction.FormatHelpText(79, 9, "Decodes and prints the decoded contents of the specified input artifact to stdout in human readable ASCII format.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-dump_hex") + Multifunction.FormatHelpText(79, 9, "Alternate raw decoding of .hex file contents in human readable form. This option should only be used for .hex file inputs.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-bin") + Multifunction.FormatHelpText(79, 9, "Writes a raw binary raw binary output file without any addressing information contained in it. Record addresses, with all padding/filler/adjustments completed, are sorted numerically before stripping addressing information.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-row_compressed") + Multifunction.FormatHelpText(79, 9, "Reorders bytes within data records for compressed format row programming. For each 0x4 address aligned block of data normally stored as a byte array with byte ordering {Instruction1<7:0>, Instruction1<15:8>, Instruction1<23:16>, Instruction2<7:0>, Instruction2<15:8>, Instruction2<23:16>}, this option will change the out ordering to {Instruction1<7:0>, Instruction1<15:8>, Instruction1<23:16>, Instruction2<23:16>, Instruction2<7:0>, Instruction2<15:8>}. Specifying this option will automatically generate left and right 0xFF padding so all data starts and ends on 0x4 address boundaries.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-add_phantoms") + Multifunction.FormatHelpText(79, 9, "Aligns and pads all records with 0xFF bytes to start and end on a perfect 0x2 address boundary, doubles all record start addresses, then adds 0x00 phantom bytes after every 3 bytes of record data (every 0x2 original addresses).") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-raw=address:filename.ext[,addr2:filename2.ext,addr3:filename3.ext,...]") + Multifunction.FormatHelpText(79, 9, "Inserts the binary file contents of the specified file starting at the specified address. If data already exists within the given address range required, it is overwritten.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-void=StartAddr,EndAddr[,StartAddr2,EndAddr2,...]") + Multifunction.FormatHelpText(79, 9, "Makes data, if any, dissappear from the specified starting address up to, but not including, the specified end address before generating the final output.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-fill=StartAddr,EndAddr,byteVal[,StartAddr2,EndAddr2,byteVal2,...]") + Multifunction.FormatHelpText(79, 9, "Generates a specified byte value over all locations between the specified start address (inclusive) and end address (exclusive). If input data is already present in the address range, it is overwritten with fill bytes.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-pad=StartAddr,EndAddr,byteVal[,StartAddr2,EndAddr2,byteVal2,...]") + Multifunction.FormatHelpText(79, 9, "Generates a specified byte value over all locations between the specified start address (inclusive) and end address (exclusive). If input data is already present in the address range, the existing data is retained with pad bytes only inserted into addressing gaps.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-crc32=StartAddr,EndAddr,InsertAddr[,StartAddr2,EndAddr2,InsertAddr2,...]") + Multifunction.FormatHelpText(79, 9, "Generates a CRC32 (IEEE 802.3 Ethernet version) over data within the specified address range, then inserts this value at a specific address.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-mov=SrcStartAddr,EndAddr,DestStartAddr[,SrcStartAddr2,EndAddr2,DestStartAddr2,...]") + Multifunction.FormatHelpText(79, 9, "Moves data from a given address range to a new block location. If existing data resides on the destination range, and there is source range data defined for the same target location, the existing destination data is overwritten.") + "\r\n");
      
























































      return;
    }
    
    List<String> tokens = new ArrayList();
    tokens.addAll(Arrays.asList(args));
    for (String token : tokens)
    {
      String lowerToken = token.toLowerCase();
      
      if (lowerToken.startsWith("-raw="))
      {
        String[] rawParams = Multifunction.TrimQuotes(token.substring("-raw=".length())).split(",");
        for (String raw : rawParams)
        {
          DataRecord dr = new DataRecord(true);
          String[] addrFile = Multifunction.TrimQuotes(raw).split(":", 2);
          String errorString = String.format("\nezbl_tools: Invalid format for 'raw' parameter. Use:\n  -raw=address:filename.ext[,addr2:filename2.ext,addr3:filename3.ext,...]:\n  \"%1$s\"\n", new Object[] { raw });
          



          try
          {
            address = Long.decode(Multifunction.TrimQuotes(addrFile[0])).longValue();
          }
          catch (NumberFormatException ex)
          {
            System.err.print(errorString);
            continue;
          }
          String filePath = Multifunction.FixSlashes(Multifunction.TrimQuotes(addrFile[1]));
          File f = new File(filePath);
          if (!f.canRead())
          {
            System.err.printf("ezbl_tools: Can't read \"%1$s\"\n", new Object[] { addrFile[1] });
          }
          else {
            data = Multifunction.ReadFileBinary(f.getPath());
            rawInputs.add(dr);
            if (outputFilename == null)
            {
              outputFilename = f.getPath();
            }
          }
        }
      } else if (lowerToken.startsWith("-bin"))
      {
        binOut = true;
        outputExtension = ".bin";
      }
      else if (lowerToken.startsWith("-row_compressed"))
      {
        rowCompressed = true;
      }
      else if (lowerToken.startsWith("-add_phantoms"))
      {
        addPhantoms = true;
      }
      else if (lowerToken.startsWith("-void="))
      {
        String[] params = Multifunction.TrimQuotes(token.substring("-void=".length())).split(",");
        int i = 0;
        AddressRange ar = new AddressRange(-1L, -1L);
        for (String param : params)
        {
          i++;
          String errorString = String.format("\nezbl_tools: Invalid format for 'void' parameter. Use:\n  -void=StartAddr,EndAddr[,StartAddr2,EndAddr2,...]:\n  \"%1$s\"\n", new Object[] { param });
          


          try
          {
            if ((i & 0x1) == 1)
            {
              startAddr = Long.decode(Multifunction.TrimQuotes(param)).longValue();
            }
            else
            {
              endAddr = Long.decode(Multifunction.TrimQuotes(param)).longValue();
              if (startAddr != -1L)
              {
                voidRanges.add(ar);
                ar = new AddressRange(-1L, -1L);
              }
            }
          }
          catch (Exception ex)
          {
            System.err.print(errorString);
          }
          
        }
      }
      else if (lowerToken.startsWith("-fill="))
      {
        String[] params = Multifunction.TrimQuotes(token.substring("-fill=".length())).split(",");
        int i = 0;
        AddressRange ar = new AddressRange(-1L, -1L);
        
        for (String param : params)
        {
          i++;
          String errorString = String.format("\nezbl_tools: Invalid format for 'fill' parameter. Use:\n  -fill=StartAddr,EndAddr,byteVal[,StartAddr2,EndAddr2,byteVal2,...]:\n  \"%1$s\"\n", new Object[] { param });
          


          try
          {
            switch (i % 3)
            {
            case 1: 
              startAddr = Long.decode(Multifunction.TrimQuotes(param)).longValue();
              break;
            
            case 2: 
              endAddr = Long.decode(Multifunction.TrimQuotes(param)).longValue();
              break;
            
            default: 
              byte fillVal = Integer.decode(Multifunction.TrimQuotes(param)).byteValue();
              if ((startAddr != -1L) && (endAddr != -1L))
              {
                fillRanges.add(ar);
                ar = new AddressRange(-1L, -1L);
                fillValues.add(Byte.valueOf(fillVal));
              }
              break;
            }
          }
          catch (NumberFormatException ex)
          {
            System.err.print(errorString);
          }
        }
      }
      else if (lowerToken.startsWith("-pad="))
      {
        String[] params = Multifunction.TrimQuotes(token.substring("-pad=".length())).split(",");
        int i = 0;
        AddressRange ar = new AddressRange(-1L, -1L);
        
        for (String param : params)
        {
          i++;
          String errorString = String.format("\nezbl_tools: Invalid format for 'pad' parameter. Use:\n  -pad=StartAddr,EndAddr,byteVal[,StartAddr2,EndAddr2,byteVal2,...]:\n  \"%1$s\"\n", new Object[] { param });
          


          try
          {
            switch (i % 3)
            {
            case 1: 
              startAddr = Long.decode(Multifunction.TrimQuotes(param)).longValue();
              break;
            
            case 2: 
              endAddr = Long.decode(Multifunction.TrimQuotes(param)).longValue();
              break;
            
            default: 
              byte padVal = Integer.decode(Multifunction.TrimQuotes(param)).byteValue();
              if ((startAddr != -1L) && (endAddr != -1L))
              {
                padRanges.add(ar);
                ar = new AddressRange(-1L, -1L);
                padValues.add(Byte.valueOf(padVal));
              }
              break;
            }
          }
          catch (NumberFormatException ex)
          {
            System.err.print(errorString);
          }
        }
      }
      else if (lowerToken.startsWith("-crc32="))
      {
        String[] params = Multifunction.TrimQuotes(token.substring("-crc32=".length())).split(",");
        int i = 0;
        AddressRange ar = new AddressRange(-1L, -1L);
        
        for (String param : params)
        {
          i++;
          String errorString = String.format("\nezbl_tools: Invalid format for 'crc32' parameter. Use:\n  -crc32=StartAddr,EndAddr,InsertAddr[,StartAddr2,EndAddr2,InsertAddr2,...]:\n  \"%1$s\"\n", new Object[] { param });
          


          try
          {
            convert = Long.decode(Multifunction.TrimQuotes(param)).longValue();
          }
          catch (NumberFormatException ex) {
            long convert;
            System.err.print(errorString);
            continue; }
          long convert;
          switch (i % 3)
          {
          case 1: 
            startAddr = convert;
            break;
          case 2: 
            endAddr = convert;
            break;
          default: 
            if ((startAddr != -1L) && (endAddr != -1L))
            {
              crcDestinations.add(Long.valueOf(convert));
              crcRanges.add(ar);
              ar = new AddressRange(-1L, -1);
            }
            break;
          }
        }
      }
      else if (lowerToken.startsWith("-mov="))
      {
        String[] params = Multifunction.TrimQuotes(token.substring("-mov=".length())).split(",");
        int i = 0;
        AddressRange ar = new AddressRange(-1L, -1L);
        for (String param : params)
        {
          i++;
          String errorString = String.format("\nezbl_tools: Invalid format for 'mov' parameter. Use:\n  -mov=SrcStartAddr,EndAddr,DestStartAddr[,SrcStartAddr2,EndAddr2,DestStartAddr2,...]:\n  \"%1$s\"\n", new Object[] { param });
          


          try
          {
            switch (i % 3)
            {
            case 1: 
              startAddr = Long.decode(Multifunction.TrimQuotes(param)).longValue();
              break;
            
            case 2: 
              endAddr = Long.decode(Multifunction.TrimQuotes(param)).longValue();
              break;
            
            default: 
              long destAddr = Long.decode(Multifunction.TrimQuotes(param)).longValue();
              if ((startAddr != -1L) && (endAddr != -1L))
              {
                movRanges.add(ar);
                ar = new AddressRange(-1L, -1L);
                movDestinations.add(Long.valueOf(destAddr));
              }
              break;
            }
          }
          catch (NumberFormatException ex)
          {
            System.err.print(errorString);
          }
        }
      }
    }
    

    if ((artifactPath == null) && (rawInputs.isEmpty()))
    {
      System.err.println("ezbl_tools: Missing input filename. Specify -artifact=input.hex/.elf/.blob/.bl2 or -raw parameter.");
      System.exit(-1);
    }
    

    if ((artifactPath != null) && (artifactPath.matches(".*?[.][eE][lL][fF]$")))
    {
      artifactPath = Multifunction.TrimQuotes(Multifunction.FixSlashes(artifactPath.trim()));
      int elfMachine = ELFReader.Machine(artifactPath);
      String toolPrefix = "elf";
      metaParser = new ELFDumpParser();
      List<String> cmdLine = new ArrayList();
      
      if (elfMachine == ELFReader.e_machine.EM_MIPS)
      {
        MCU16Mode = false;
        toolPrefix = "xc32";
        if (hexAlign == 0) {
          hexAlign = (state.hexAlign = 8);
        }
      } else if (elfMachine == ELFReader.e_machine.EM_PIC30)
      {
        MCU16Mode = true;
        toolPrefix = "xc16";
      }
      else if (elfMachine == ELFReader.e_machine.EM_XC8)
      {
        System.err.printf("ezbl_tools.jar: PIC16/PIC18 .elf files are not supported as blobber input\n                artifacts. Supply a .hex file instead.\n", new Object[0]);
        
        System.exit(-18);
      }
      cmdLine.add((compilerFolder != null ? compilerFolder + File.separator : "") + toolPrefix + "-objdump");
      cmdLine.add("--all-headers");
      if (!MCU16Mode)
        cmdLine.add("--full-contents");
      cmdLine.add("--syms");
      cmdLine.add(artifactPath);
      if (metaParser.startObjDump(cmdLine) < 0)
      {
        metaParser = null;
        if (!silent)
        {
          System.err.printf("    \"" + Multifunction.CatStringList(cmdLine, " ") + "\"\n" + "    Ensure %s's bin folder is in the system path, set via the -compiler_folder= command line option, or assigned\n" + "    using the MP_CC_PATH environment variable.\n", new Object[] { toolPrefix.toUpperCase() });
          

          System.exit(elfMachine == ELFReader.e_machine.EM_MIPS ? -32 : elfMachine == ELFReader.e_machine.EM_PIC30 ? -16 : 63904);
        }
      }
      if ((MCU16Mode) && (metaParser != null))
      {
        cmdLine.remove("--syms");
        cmdLine.remove("--all-headers");
        cmdLine.add("--full-contents");
        metaParser.startObjDump(cmdLine);
      }
      
      hexPath = artifactPath.replaceFirst("[.][eE][lL][fF]$", ".hex");
      File eFile = new File(artifactPath);
      File hFile = new File(hexPath);
      if (hFile.lastModified() < eFile.lastModified())
      {
        cmdLine.clear();
        cmdLine.add((compilerFolder != null ? compilerFolder + File.separator : "") + toolPrefix + "-bin2hex");
        cmdLine.add("-a");
        if (MCU16Mode)
        {
          cmdLine.add("-omf=elf");
        }
        cmdLine.add(artifactPath);
        ProcessBuilder proc = new ProcessBuilder(cmdLine);
        try
        {
          proc.redirectErrorStream(true);
          Process ps = proc.start();
          ps.waitFor();
        }
        catch (IOException e)
        {
          if (!silent)
          {
            System.err.println("ezbl_tools: " + e.getMessage());
          }
          System.exit(-1);
          return;
        }
        catch (InterruptedException localInterruptedException) {}
      }
    }
    


    if (hexAlign == 0)
    {
      hexAlign = flashWordSize; }
    Bl2b bl2;
    Bl2b bl2;
    if (hexPath == null)
    {
      bl2 = new Bl2b(MCU16Mode);

    }
    else
    {

      int extensionOffset = hexPath.toLowerCase().lastIndexOf(".hex");
      if (extensionOffset >= 0)
      {
        String fileData = Multifunction.ReadFile(hexPath, true);
        if (fileData == null)
        {
          System.err.println("ezbl_tools: Unable to open \"" + hexPath + "\"");
          System.exit(-3);
        }
        
        Bl2b bl2 = new Bl2b(fileData);
        MCU16Mode = architecture16Bit;
        

        outputFilename = hexPath.substring(0, extensionOffset);
      }
      else
      {
        byte[] binaryFileData = Multifunction.ReadFileBinary(hexPath);
        if (binaryFileData == null)
        {
          System.err.println("ezbl_tools: Unable to open \"" + hexPath + "\"");
          System.exit(-4);
          return;
        }
        try
        {
          Bl2b bl2;
          if (hexPath.toLowerCase().lastIndexOf(".blob") >= 0)
          {
            Blob b = new Blob(binaryFileData, MCU16Mode);
            bl2 = new Bl2b(b, null, 0, 0, 0);
          }
          else
          {
            bl2 = new Bl2b(binaryFileData, null);
          }
        }
        catch (Exception ex) {
          Bl2b bl2;
          System.err.println("ezbl_tools: " + ex.toString());
          System.exit(-5);
          return;
        }
        
        outputFilename = hexPath.replaceAll("([.]([hH][eE][xX])|([bB][lL][oO][bB])|([bB][lL]2))$", "");
      }
    }
    


    if (metaParser != null)
    {



      metaParser.parseAllObjOutput(0);
      metaParser.parseAllObjOutput(0);
      elfDump = metaParser;
      
      Section sec = (Section)romSectionMapByName.get(".text.EZBL_bootIDHash");
      if ((sec != null) && (data != null) && (data.data != null)) {
        bootIDHash = Multifunction.BytesToInt32s(data.data, 0, data.data.length);
      }
      else {
        Symbol s = (Symbol)symbolsByName.get("BOOTID_HASH0");
        if (s != null)
        {
          bootIDHash[0] = ((int)address);
          bootIDHash[1] = ((int)symbolsByName.get("BOOTID_HASH1")).address);
          bootIDHash[2] = ((int)symbolsByName.get("BOOTID_HASH2")).address);
          bootIDHash[3] = ((int)symbolsByName.get("BOOTID_HASH3")).address);
        }
        else
        {
          s = (Symbol)symbolsByName.get("_BOOTID_HASH0");
          if (s != null)
          {
            bootIDHash[0] = ((int)address);
            bootIDHash[1] = ((int)symbolsByName.get("_BOOTID_HASH1")).address);
            bootIDHash[2] = ((int)symbolsByName.get("_BOOTID_HASH2")).address);
            bootIDHash[3] = ((int)symbolsByName.get("_BOOTID_HASH3")).address);
          }
        }
      }
      
      Symbol s = (Symbol)symbolsByName.get("APPID_VER_MAJOR");
      if (s != null)
      {
        appIDVerMajor = ((int)address);
        appIDVerMinor = ((int)symbolsByName.get("APPID_VER_MINOR")).address);
        appIDVerBuild = ((int)symbolsByName.get("APPID_VER_BUILD")).address);
      }
      else
      {
        s = (Symbol)symbolsByName.get("_APPID_VER_MAJOR");
        if (s != null)
        {
          appIDVerMajor = ((int)address);
          appIDVerMinor = ((int)symbolsByName.get("_APPID_VER_MINOR")).address);
          appIDVerBuild = ((int)symbolsByName.get("_APPID_VER_BUILD")).address);
        }
      }
    }
    

    if (dump)
    {
      System.out.print(bl2.GetObjDumpView());
      return;
    }
    





    String outputMergeFilename = outputFilename.replaceFirst("([.]production$)|([.]debug$)", "") + ".merge";
    String outputMergeLinkerExt = MCU16Mode ? ".gld" : ".ld";
    

    bl2.Coalesce(true, hexAlign, hexAlign);
    



    if (!rawInputs.isEmpty())
    {
      bl2.RemoveIgnoreRangeByDR((DataRecord)rawInputs.get(0));
      for (int i = 0; i < rawInputs.size(); i++)
      {
        DataRecord dr = (DataRecord)rawInputs.get(i);
        records.add(dr);
        bl2.RemoveIgnoreRangesByDR(rawInputs.subList(i + 1, rawInputs.size()));
      }
      
      bl2.Coalesce(true, hexAlign, hexAlign);
    }
    
    if (generate_merge)
    {


      String[] mergeScripts = GLDBuilder.CreateMergeScript(state, bl2);
      if (mergeScripts.length != 2)
      {
        System.exit(-6);
        return;
      }
      

      if (mergeScripts[0] != null)
      {
        if (Multifunction.UpdateFileIfDataDifferent(outputMergeFilename + outputMergeLinkerExt, mergeScripts[0], true) < 0)
        {
          System.err.println("ezbl_tools: failed to write to \"" + outputMergeFilename + outputMergeLinkerExt + "\"");
          System.exit(-7);
          return;
        }
      }
      

      if (mergeScripts[1] != null)
      {
        if (Multifunction.UpdateFileIfDataDifferent(outputMergeFilename + ".S", mergeScripts[1], true) < 0)
        {
          System.err.println("ezbl_tools: failed to write to \"" + outputMergeFilename + ".S\"");
          System.exit(-8);
          return;
        }
      }
    }
    else
    {
      bl2.Coalesce(true, hexAlign, hexAlign);
    }
    
    if (ignoreROMRegions != null)
    {
      bl2.RemoveIgnoreRanges(ignoreROMRegions);
      bl2.Coalesce(true, hexAlign, hexAlign);
    }
    

    for (int i = 0; i < fillRanges.size(); i++)
    {
      AddressRange ar = (AddressRange)fillRanges.get(i);
      DataRecord.ExtractRange(records, startAddr, endAddr);
      DataRecord dr = new DataRecord();
      address = startAddr;
      architecture16Bit = architecture16Bit;
      int fillSize = architecture16Bit ? (int)(endAddr - startAddr) * 3 / 2 : (int)(endAddr - startAddr);
      data = new byte[fillSize];
      Arrays.fill(data, ((Byte)fillValues.get(i)).byteValue());
      records.add(dr);
    }
    
    for (int i = 0; i < padRanges.size(); i++)
    {
      AddressRange ar = (AddressRange)padRanges.get(i);
      List<DataRecord> drs = DataRecord.ExtractRanges(records, startAddr, endAddr);
      if ((drs == null) || (drs.isEmpty()))
      {
        DataRecord dr = new DataRecord();
        address = startAddr;
        architecture16Bit = architecture16Bit;
        int padSize = architecture16Bit ? (int)(endAddr - startAddr) * 3 / 2 : (int)(endAddr - startAddr);
        data = new byte[padSize];
        Arrays.fill(data, ((Byte)padValues.get(i)).byteValue());
        records.add(dr);
      }
      else {
        Collections.sort(drs);
        DataRecord.CoalesceRecords(drs);
        for (int drIndex = 0; drIndex < drs.size(); drIndex++)
        {
          DataRecord dr = (DataRecord)drs.get(drIndex);
          int leftPadSize = architecture16Bit ? (int)((address - startAddr) * 3L / 2L) : (int)(address - startAddr);
          int rightPadSize = 0;
          if (drIndex == drs.size() - 1)
          {
            rightPadSize = architecture16Bit ? (int)((endAddr - dr.getEndAddress()) * 3L / 2L) : (int)(endAddr - dr.getEndAddress());
          }
          
          byte[] newData = new byte[data.length + leftPadSize + rightPadSize];
          Arrays.fill(newData, 0, leftPadSize, ((Byte)padValues.get(i)).byteValue());
          Arrays.fill(newData, leftPadSize + data.length, newData.length, ((Byte)padValues.get(i)).byteValue());
          System.arraycopy(data, 0, newData, leftPadSize, data.length);
          data = newData;
          address = startAddr;
          records.add(dr);
          startAddr = dr.getEndAddress();
        }
      }
    }
    
    for (AddressRange ar : voidRanges)
    {
      DataRecord.ExtractRange(records, startAddr, endAddr);
    }
    AddressRange ar;
    for (int i = 0; i < movRanges.size(); i++)
    {
      ar = (AddressRange)movRanges.get(i);
      List<DataRecord> drs = DataRecord.ExtractRanges(records, startAddr, endAddr);
      for (DataRecord dr : drs)
      {
        address -= startAddr - ((Long)movDestinations.get(i)).longValue();
        records.add(dr);
      }
    }
    
    for (int i = 0; i < crcRanges.size(); i++)
    {
      AddressRange ar = (AddressRange)crcRanges.get(i);
      DataRecord crcDR = new DataRecord();
      address = ((Long)crcDestinations.get(i)).longValue();
      architecture16Bit = architecture16Bit;
      List<DataRecord> drs = DataRecord.ExtractRanges(records, startAddr, endAddr);
      data = new byte[architecture16Bit ? 6 : 4];
      CRC32 myCRC = new CRC32();
      
      if (data.length > 4)
      {
        data[4] = -1;
        data[5] = -1;
      }
      if ((drs == null) || (drs.isEmpty()))
      {
        long crcValue = myCRC.getValue();
        data[0] = ((byte)(int)(crcValue & 0xFF));
        data[1] = ((byte)(int)(crcValue >> 8 & 0xFF));
        data[2] = ((byte)(int)(crcValue >> 16 & 0xFF));
        data[3] = ((byte)(int)(crcValue >> 24 & 0xFF));
        DataRecord.ExtractRange(records, address, address + 4L);
        records.add(crcDR);
      }
      else {
        Collections.sort(drs);
        DataRecord.CoalesceRecords(drs);
        for (DataRecord dr : drs)
        {
          myCRC.update(data);
          records.add(dr);
        }
        long crcValue = myCRC.getValue();
        data[0] = ((byte)(int)(crcValue & 0xFF));
        data[1] = ((byte)(int)(crcValue >> 8 & 0xFF));
        data[2] = ((byte)(int)(crcValue >> 16 & 0xFF));
        data[3] = ((byte)(int)(crcValue >> 24 & 0xFF));
        DataRecord.ExtractRange(records, address, address + 4L);
        records.add(crcDR);
        if (!silent)
        {
          System.out.printf("    CRC32 from 0x%1$08X to 0x%2$08X = 0x%3$08X (stored at 0x%4$08X)\n", new Object[] { Long.valueOf(startAddr), Long.valueOf(endAddr), Long.valueOf(crcValue), Long.valueOf(address) });
        }
      }
    }
    if ((!fillRanges.isEmpty()) || (!padRanges.isEmpty()) || (!movRanges.isEmpty()) || (!crcRanges.isEmpty()))
    {
      bl2.CoalesceAdjacentDataRecords();
    }
    
    if ((addPhantoms) && (records != null) && (!records.isEmpty()) && (records.get(0)).architecture16Bit))
    {
      DataRecord.CoalesceRecords(records, true, 2L, 2L);
      for (DataRecord dr : records)
      {
        byte[] phantomPaddedData = new byte[data.length / 3 * 4];
        int phantomIndex = 0;
        for (int i = 0; i < data.length; i += 3)
        {
          phantomPaddedData[(phantomIndex++)] = data[(i + 0)];
          phantomPaddedData[(phantomIndex++)] = data[(i + 1)];
          phantomPaddedData[(phantomIndex++)] = data[(i + 2)];
          phantomIndex++;
        }
        data = phantomPaddedData;
        architecture16Bit = false;
        address *= 2L;
      }
    }
    
    if ((rowCompressed) && (records != null) && (!records.isEmpty()) && (records.get(0)).architecture16Bit))
    {
      DataRecord.CoalesceRecords(records, true, 4L, 4L);
      for (DataRecord dr : records)
      {
        for (int i = 0; i < data.length; i += 6)
        {
          byte b = data[(i + 5)];
          data[(i + 5)] = data[(i + 4)];
          data[(i + 4)] = data[(i + 3)];
          data[(i + 3)] = b;
        }
      }
    }
    
    if (binOut)
    {
      Collections.sort(records);
      byte[] buffer = bl2.GetBinBytes();
      if (Multifunction.UpdateFileIfDataDifferent(outputFilename + outputExtension, buffer) < 0)
      {
        System.err.println("ezbl_tools: Failed to write processed data to \"" + outputFilename + outputExtension + "\"");
        System.exit(-10);
        return;
      }
      return;
    }
    


    bl2.ReorderForSequentialAccess();
    

    byte[] buffer = bl2.GetBytes();
    


    if ((encryptionPassword != null) && (saveTemps))
    {
      if (Multifunction.UpdateFileIfDataDifferent(outputFilename + "_no_encrypt" + outputExtension, buffer) < 0)
      {
        System.err.println("ezbl_tools: Failed to write processed data to \"" + outputFilename + "_no_encrypt" + outputExtension + "\"");
        System.exit(-9);
        return;
      }
    }
    

    if (encryptionKey != null)
    {
      byte[] encryptedBuffer = Blob.EncryptBlobBytes(state, buffer);
      buffer = encryptedBuffer;
    }
    

    if (Multifunction.UpdateFileIfDataDifferent(outputFilename + outputExtension, buffer) < 0)
    {
      System.err.println("ezbl_tools: Failed to write processed data to \"" + outputFilename + outputExtension + "\"");
      System.exit(-10);
      return;
    }
    

    if (!silent)
    {
      int i = buffer.length - 4 - 32;
      String hashString = String.format("%1$02x%2$02x%3$02x%4$02x%5$02x%6$02x%7$02x%8$02x%9$02x%10$02x%11$02x%12$02x%13$02x%14$02x%15$02x%16$02x%17$02x%18$02x%19$02x%20$02x%21$02x%22$02x%23$02x%24$02x%25$02x%26$02x%27$02x%28$02x%29$02x%30$02x%31$02x%32$02x", new Object[] { Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]) });
      

      System.out.printf("    BL2 content SHA-256 hash + CRC32 (stored at offset 0x%1$08X) is: %2$s, 0x%6$02X%5$02X%4$02X%3$02X\n", new Object[] { Integer.valueOf(buffer.length - 4 - 32), hashString, Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]), Byte.valueOf(buffer[(i++)]) });
      System.out.printf("    Successfully wrote %1$d bytes to %2$s\n", new Object[] { Integer.valueOf(buffer.length), outputFilename + outputExtension });
    }
  }
}
