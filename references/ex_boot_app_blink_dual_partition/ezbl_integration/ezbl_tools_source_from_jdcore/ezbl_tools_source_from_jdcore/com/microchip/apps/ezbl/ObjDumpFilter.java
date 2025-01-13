package com.microchip.apps.ezbl;

import java.io.File;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;






























public class ObjDumpFilter
{
  public ObjDumpFilter() {}
  
  public static void main(String[] args)
  {
    String elfReaderPath = null;
    

    boolean decodeRestartDataInit = false;
    boolean decodeDataInit = false;
    boolean generateFastDataInitCode = false;
    boolean generateFastDataInitCodeSummary = false;
    

    EZBLState state = new EZBLState();
    state = CommandAndBuildState.ReadArgs(state, args);
    

    if (!parseOkay)
    {
      System.out.print("\r\n" + Multifunction.FormatHelpText(79, 0, "Usage:") + Multifunction.FormatHelpText(79, 3, "java -jar ezbl_tools.jar --obj-dump_filter [options] -artifact=\"app_v2.production.elf\"[,\"app_v1.production.elf\"]") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Options and Inputs:") + Multifunction.FormatHelpText(79, 6, "-decode.dinit") + Multifunction.FormatHelpText(79, 9, "Optional parameter that turns on decoding of the .dinit RAM initializer table (normal data initialization for non-'preserved' variables)") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-decode.rdinit") + Multifunction.FormatHelpText(79, 9, "Optional parameter that turns on decoding of the .rdinit RAM initializer table (restart data initialization for 'update' variables)") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-generate.fast_init") + Multifunction.FormatHelpText(79, 9, "Optional parameter that turns on generation of fast, 'inline' initialization code to replace .dinit/.rdinit and _data_init() compiler methods.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-summarize.fast_init") + Multifunction.FormatHelpText(79, 9, "Optional parameter displaying summary of fast 'inline' initialization code.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-elf_reader=\"compiler\\path\\to\\bin\\xc16-objdump.exe\"") + Multifunction.FormatHelpText(79, 9, "Optional path to the GCC object dumping executable, i.e. xc16-objdump. If not specified, \"xc16-objdump\" will attempt to be used with OS specific path environment rules passed to the ezbl_tools.jar JVM.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-artifact=\"project_v2.production.elf\",[\"project_v1.production.elf\"]") + Multifunction.FormatHelpText(79, 9, "One or more input .elf/.o file to read and process the contents of") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Outputs:") + Multifunction.FormatHelpText(79, 6, "Summary of suspicious data object file contents moving between artifact versions + any optional output data") + "\r\n");
      



























      return;
    }
    

    if (undecodedOptions.contains("-elf_reader="))
    {
      for (int i = 0; i < args.length; i++)
      {
        if (args[i].startsWith("-elf_reader="))
        {
          elfReaderPath = Multifunction.TrimQuotes(args[i].substring("-elf_reader=".length()));
          elfReaderPath = Multifunction.FixSlashes(elfReaderPath);
          break;
        }
      }
    }
    decodeDataInit = undecodedOptions.contains("-decode.dinit");
    decodeRestartDataInit = undecodedOptions.contains("-decode.rdinit");
    generateFastDataInitCode = undecodedOptions.contains("-generate.fast_init");
    generateFastDataInitCodeSummary = undecodedOptions.contains("-summarize.fast_init");
    
    if (artifactPath == null)
    {
      if (!silent)
      {
        System.err.println("Error: Missing required input artifact. The -artifact parameter is required.");
      }
      System.exit(-1);
    }
    String[] artifacts = artifactPath.split("[,]");
    int filesPresent = 0;
    for (int i = 0; i < artifacts.length; i++)
    {
      if (new File(artifacts[i]).exists())
      {
        filesPresent++;
      }
    }
    if ((filesPresent < 2) && (!decodeDataInit) && (!decodeRestartDataInit) && (!generateFastDataInitCode) && (!generateFastDataInitCodeSummary))
    {
      if (!silent)
      {
        System.err.println("EZBL: At least two .elf files must be provided for RAM address checking; skipping...");
      }
      System.exit(-2);
    }
    
    ELFDumpParser[] parsers = new ELFDumpParser[artifacts.length];
    

    for (int i = 0; i < parsers.length; i++)
    {
      parsers[i] = new ELFDumpParser();
      if (elfReaderPath == null)
      {
        if (ELFReader.Machine(artifacts[i]) == 16) {
          elfReaderPath = "xc16-objdump";
        } else {
          elfReaderPath = "xc32-objdump";
        }
      }
      if (parsers[i].startObjDump(new String[] { elfReaderPath, "--section-header", "--full-contents", artifacts[i] }) < 0)
      {
        System.exit(-3);
      }
    }
    

    for (int i = 0; i < parsers.length; i++)
    {
      parsers[i].parseAllObjOutput(0);
      Collections.sort(symbols);
      Collections.sort(sections);
    }
    


































    List<String> secAddrErrors = new ArrayList();
    for (int p = 0; p < parsers.length; p++)
    {
      ELFDumpParser parser = parsers[p];
      
      boolean exactMatch = false;
      boolean nameMatch = false;
      boolean weakNameMatch = false;
      boolean addressMatch = false;
      boolean sizeMatch = false;
      boolean updateMatch = false;
      boolean preservedMatch = false;
      



      if ((p == 0) && ((decodeDataInit) || (decodeRestartDataInit) || (generateFastDataInitCode) || (generateFastDataInitCodeSummary)))
      {
        for (Section sec : sections)
        {
          if ((name.equals(".rdinit")) || (name.equals(".dinit")))
          {



            Blob ramInitBlob = new Blob(false);
            List<String> initCode = new ArrayList();
            List<String> dinitDecode = new ArrayList();
            int nonzeroBytesEncoded = 0;
            int zeroBytesEncoded = 0;
            int functionCallsEncoded = 0;
            int tableSize = data.data.length;
            
            for (int j = 0; j < data.data.length - 9; j += 9)
            {
              int dest = data.data[j] & 0xFF | (data.data[(j + 1)] & 0xFF) << 8 | (data.data[(j + 2)] & 0xFF) << 16;
              int recordLen = data.data[(j + 3)] & 0xFF | (data.data[(j + 4)] & 0xFF) << 8 | (data.data[(j + 5)] & 0xFF) << 16;
              int format = data.data[(j + 6)] & 0x7F;
              int page = (data.data[(j + 6)] & 0x80) >> 7 | (data.data[(j + 7)] & 0xFF) << 1;
              
              String decodedFormat = format == 31 ? "ldslvPRAM" : format == 3 ? "call func" : format == 2 ? "copy pgm3" : format == 1 ? "copy pgm2" : format == 0 ? "zero fill" : "unknown  ";
              




              byte[] recordData = new byte[recordLen];
              long realDest = dest;
              if (format == 3)
              {
                realDest |= page << 16;
              }
              else if (((dest & 0x8000) != 0) && (page != 0))
              {
                realDest ^= 0x8000 ^ page << 15;
              }
              else
              {
                realDest |= page - 1 << 16;
              }
              String initSectionName = "";
              for (Section secLookName : sections)
              {
                if (loadMemoryAddress == realDest)
                {
                  initSectionName = name;
                  break;
                }
              }
              
              if (format == 0)
              {
                zeroBytesEncoded += recordLen;
                ramInitBlob.FillData(new AddressRange(realDest, recordLen), (byte)0);
              }
              if (format == 1)
              {
                nonzeroBytesEncoded += recordLen;
                for (int k = 0; k < recordLen; k += 2)
                {
                  recordData[k] = data.data[(j + 9)];
                  j++;
                  if (data.data.length >= j + 9)
                  {
                    recordData[(k + 1)] = data.data[(j + 9)];
                  }
                  j++;
                  if (data.data.length >= j + 9)
                  {
                    j++;
                  }
                }
                ramInitBlob.AddData(realDest, recordData);
              }
              else if (format == 2)
              {
                nonzeroBytesEncoded += recordLen;
                for (int k = 0; k < recordLen; k++)
                {
                  recordData[k] = data.data[(j + 9)];
                  j++;
                }
                j += (recordLen % 3 == 0 ? 0 : 3 - recordLen % 3);
                ramInitBlob.AddData(realDest, recordData);
              }
              else if (format == 3)
              {
                functionCallsEncoded++;
                initCode.add(String.format("call 0x%1$06X ; %2$s", new Object[] { Integer.valueOf(dest), initSectionName }));
              }
              
              dinitDecode.add(String.format("%1$s: dest = 0x%2$04X, len = %3$3d, format = %4$X (%5$s), page = 0x%6$03X: %7$s", new Object[] { name, Integer.valueOf(dest), Integer.valueOf(recordLen), Integer.valueOf(format), decodedFormat, Integer.valueOf(page), initSectionName }));
              if ((recordLen != 0) && (format != 0))
              {
                String displayBytes = Communicator.FormatBinaryHelp(recordData, name.length() + 2, 16, 2, name.length() + 2);
                dinitDecode.add(String.format("{" + displayBytes + "\n}", new Object[0]));
              }
            }
            
            if (((name.equals(".dinit")) && (decodeDataInit)) || ((name.equals(".rdinit")) && (decodeRestartDataInit)))
            {
              dinitDecode.add(0, String.format("Decoded contents of %1$s data initialiation table:\n----------------------------------------------------", new Object[] { name }));
              
              System.out.print(Multifunction.CatStringList(dinitDecode, "\n") + "\n\n");
            }
            
            System.out.printf("\nTotal %1$s section size: %2$3d bytes of flash (0x%3$06X addresses)", new Object[] { name, Integer.valueOf(tableSize), Integer.valueOf((tableSize + 2) / 3 * 2) });
            System.out.printf("\n    RAM bytes written: %1$d", new Object[] { Integer.valueOf(nonzeroBytesEncoded + zeroBytesEncoded + 6 * functionCallsEncoded) });
            System.out.printf("\n    Function calls : %1$d", new Object[] { Integer.valueOf(functionCallsEncoded) });
            System.out.printf("\n    Zero fill bytes: %1$d", new Object[] { Integer.valueOf(zeroBytesEncoded) });
            System.out.printf("\n    Non-zero bytes : %1$d", new Object[] { Integer.valueOf(nonzeroBytesEncoded) });
            System.out.printf("\n    Ratio RAM written/table size: %1$1.3f (bytes/byte)\n", new Object[] { Double.valueOf((nonzeroBytesEncoded + zeroBytesEncoded + 6 * functionCallsEncoded) / tableSize) });
            
            if ((generateFastDataInitCode) || (generateFastDataInitCodeSummary))
            {
              architecture16Bit = false;
              for (DataRecord dr : records)
              {
                architecture16Bit = false;
              }
              
              ramInitBlob.CoalesceAdjacentDataRecords();
              long curAddr = -1L;
              int executionTime = 0;
              int[] regs = new int[15];
              Arrays.fill(regs, -1);
              
              for (DataRecord dr : records)
              {
                int index = 0;
                int doLen = data.length;
                



                if (curAddr != address)
                {
                  curAddr = address;
                  if (curAddr >= 32768L)
                  {
                    initCode.add(String.format("movpag  #0x%1$03X, DSWPAG", new Object[] { Long.valueOf(curAddr >> 15) }));
                    initCode.add(String.format("mov     #0x%1$04X, w0", new Object[] { Long.valueOf((curAddr | 0x8000) & 0xFFFF) }));
                    regs[0] = ((int)curAddr & 0xFFFF);
                    executionTime += 2;
                  }
                  else
                  {
                    initCode.add(String.format("mov     #0x%1$04X, w0", new Object[] { Long.valueOf(curAddr) }));
                    regs[0] = ((int)curAddr & 0xFFFF);
                    executionTime++;
                  }
                }
                
                if (((curAddr & 1L) != 0L) && (doLen != 0))
                {

                  if (data[index] == 0)
                  {
                    initCode.add("clr.b   [w0++]");
                    regs[0] = (regs[0] + 1 & 0xFFFF);
                  }
                  else
                  {
                    initCode.add(String.format("mov     #0x00%1$02X, w2", new Object[] { Byte.valueOf(data[index]) }));
                    regs[2] = (data[index] & 0xFF);
                    initCode.add("mov.b   w2, [w0++]");
                    regs[0] = (regs[0] + 1 & 0xFFFF);
                    executionTime++;
                  }
                  executionTime++;
                  index++;
                  doLen--;
                  curAddr += 1L;
                }
                while (doLen != 0)
                {

                  int aluGenCount = 0;
                  for (int tempIndex = index; tempIndex < data.length; tempIndex++)
                  {
                    if (data[tempIndex] != 0) {
                      break;
                    }
                    
                    aluGenCount++;
                  }
                  if ((aluGenCount & 0x1) != 0)
                  {
                    aluGenCount--;
                  }
                  if (aluGenCount >= 6)
                  {
                    initCode.add(String.format("repeat  #%1$d", new Object[] { Integer.valueOf(aluGenCount / 2 - 1) }));
                    initCode.add("clr     [w0++]");
                    regs[0] = (regs[0] + 2 * aluGenCount / 2 & 0xFFFF);
                    executionTime += 1 + aluGenCount / 2;
                    index += aluGenCount;
                    doLen -= aluGenCount;
                    curAddr += aluGenCount;

                  }
                  else if (aluGenCount == 4)
                  {
                    initCode.add("clr     [w0++]");
                    initCode.add("clr     [w0++]");
                    regs[0] = (regs[0] + 4 & 0xFFFF);
                    executionTime += 2;
                    index += 4;
                    doLen -= 4;
                    curAddr += 4L;

                  }
                  else if (aluGenCount == 2)
                  {
                    initCode.add("clr     [w0++]");
                    regs[0] = (regs[0] + 2 & 0xFFFF);
                    executionTime++;
                    index += aluGenCount;
                    doLen -= aluGenCount;
                    curAddr += aluGenCount;

                  }
                  else
                  {
                    aluGenCount = 0;
                    for (tempIndex = index; tempIndex < data.length; tempIndex++)
                    {
                      if (data[tempIndex] != -1) {
                        break;
                      }
                      
                      aluGenCount++;
                    }
                    if ((aluGenCount & 0x1) != 0)
                    {
                      aluGenCount--;
                    }
                    if (aluGenCount >= 6)
                    {
                      initCode.add(String.format("repeat  #%1$d", new Object[] { Integer.valueOf(aluGenCount / 2 - 1) }));
                      initCode.add("setm    [w0++]");
                      regs[0] = (regs[0] + 2 * aluGenCount / 2 & 0xFFFF);
                      executionTime += 1 + aluGenCount / 2;
                      index += aluGenCount;
                      doLen -= aluGenCount;
                      curAddr += aluGenCount;

                    }
                    else if (aluGenCount == 4)
                    {
                      initCode.add("setm    [w0++]");
                      initCode.add("setm    [w0++]");
                      regs[0] = (regs[0] + aluGenCount & 0xFFFF);
                      executionTime += 2;
                      index += aluGenCount;
                      doLen -= aluGenCount;
                      curAddr += aluGenCount;

                    }
                    else if (aluGenCount == 2)
                    {
                      initCode.add("setm    [w0++]");
                      regs[0] = (regs[0] + aluGenCount & 0xFFFF);
                      executionTime++;
                      index += aluGenCount;
                      doLen -= aluGenCount;
                      curAddr += aluGenCount;
                    }
                    else
                    {
                      int bytesAdded = 0;
                      for (int j = 2; j <= 14; j++)
                      {
                        if (doLen < 2) {
                          break;
                        }
                        
                        int wordData = data[(index + 1)] << 8 & 0xFF00 | data[(index + 0)] & 0xFF;
                        if ((wordData == 0) && (doLen >= 4) && (j % 2 == 0) && (j <= 12))
                        {
                          int nextWordData = data[(index + 3)] << 8 & 0xFF00 | data[(index + 2)] & 0xFF;
                          if (nextWordData == 0)
                          {
                            initCode.add(String.format("mul.uu  w0, #0, w%1$d", new Object[] { Integer.valueOf(j) }));
                            bytesAdded += 4;
                            executionTime++;
                            index += 4;
                            doLen -= 4;
                            curAddr += 4L;
                            j++;
                            continue;
                          }
                        }
                        initCode.add(String.format("mov     #0x%1$04X, w%2$d", new Object[] { Integer.valueOf(wordData), Integer.valueOf(j) }));
                        executionTime++;
                        bytesAdded += 2;
                        index += 2;
                        doLen -= 2;
                        curAddr += 2L;
                      }
                      
                      if (bytesAdded >= 6)
                      {
                        initCode.add("mov     #w2, w1");
                        initCode.add(String.format("repeat  #%1$d", new Object[] { Integer.valueOf(bytesAdded / 2 - 1) }));
                        initCode.add("mov     [w1++], [w0++]");
                        executionTime += 2 + bytesAdded / 2;
                        bytesAdded -= bytesAdded / 2 * 2;
                      }
                      for (int j = 2; bytesAdded >= 2; bytesAdded -= 2)
                      {
                        initCode.add(String.format("mov     w%1$d, [w0++]", new Object[] { Integer.valueOf(j++) }));
                        executionTime++;
                      }
                      if (doLen == 1)
                      {
                        if (data[index] == 0)
                        {
                          initCode.add("clr.b   [w0++]");
                          executionTime++;
                        }
                        if (data[index] == 255)
                        {
                          initCode.add("setm.b  [w0++]");
                          executionTime++;
                        }
                        else
                        {
                          initCode.add(String.format("mov     #0x00%1$02X, w2", new Object[] { Byte.valueOf(data[(index++)]) }));
                          initCode.add("mov.b   w2, [w0++]");
                          executionTime += 2;
                        }
                        doLen--;
                        curAddr += 1L;
                      }
                    }
                  }
                } }
              int initCodeBytes = 3 * initCode.size() + 3 * functionCallsEncoded;
              executionTime += 10 * functionCallsEncoded;
              
              if (generateFastDataInitCode)
              {
                System.out.print("\n\nFast initializer equivalent code:\n    " + Multifunction.CatStringList(initCode, "\n    ") + "\n\n");
              }
              

              System.out.printf("\nTotal generated code size: %1$3d bytes of flash (0x%2$06X addresses)", new Object[] { Integer.valueOf(initCodeBytes), Integer.valueOf(initCodeBytes / 3 * 2) });
              System.out.printf("\n    RAM bytes initialized: %1$d bytes (0x%1$04X)", new Object[] { Integer.valueOf(nonzeroBytesEncoded + zeroBytesEncoded + 6 * functionCallsEncoded) });
              System.out.printf("\n    Ratio RAM written/code size: %1$1.3f (bytes/byte)", new Object[] { Double.valueOf((nonzeroBytesEncoded + zeroBytesEncoded + 6 * functionCallsEncoded) / initCodeBytes) });
              System.out.printf("\n    Relative ROM improvement over %1$s table: %2$d ROM table + 270 shared ROM code bytes for data_init() (%3$1.1f%%)", new Object[] { name, Integer.valueOf(tableSize - initCodeBytes), Double.valueOf(initCodeBytes / (tableSize + 270) * 100.0D) });
              System.out.printf("\n    Approximate execution time, excluding function code: %1$d instruction cycles (%2$1.3fus @ 70 MIPS)\n", new Object[] { Integer.valueOf(executionTime), Double.valueOf(executionTime / 70.0D) });
              System.out.printf("\n\n\n\n", new Object[0]);
            }
          }
        }
      }
      for (Section sec : sections)
      {
        if ((isRAM) && (
        


          (!flags.UPDATE) || (flags.PRESERVED)))
        {
          String namePad;
          

          for (int j = p + 1; j < parsers.length; j++)
          {
            ELFDumpParser parser2 = parsers[j];
            
            exactMatch = false;
            nameMatch = false;
            addressMatch = false;
            sizeMatch = false;
            updateMatch = false;
            List<Section> partialMatches = new ArrayList();
            for (Section sec2 : sections)
            {
              if (isRAM)
              {



                if ((name.equals(name)) && (loadMemoryAddress == loadMemoryAddress) && (size == size))
                {
                  exactMatch = true;
                  preservedMatch |= flags.PRESERVED;
                  updateMatch |= flags.UPDATE;
                  partialMatches.add(sec2);
                }
                else if ((name.equals(name)) && (loadMemoryAddress == loadMemoryAddress))
                {
                  nameMatch = true;
                  addressMatch = true;
                  preservedMatch |= flags.PRESERVED;
                  updateMatch |= flags.UPDATE;
                  partialMatches.add(sec2);
                }
                else if ((size == size) && ((loadMemoryAddress == loadMemoryAddress) || (name.equals(name))))
                {
                  sizeMatch = true;
                  preservedMatch |= flags.PRESERVED;
                  updateMatch |= flags.UPDATE;
                  partialMatches.add(sec2);
                }
                else if (name.equals(name))
                {
                  nameMatch = true;
                  preservedMatch |= flags.PRESERVED;
                  updateMatch |= flags.UPDATE;
                  partialMatches.add(sec2);
                }
                else if (name.matches(".*?\\.[0-9]*?$"))
                {
                  String left = name.substring(0, name.lastIndexOf('.'));
                  String right = name;
                  if (right.matches(".*?\\.[0-9]*?$"))
                  {
                    right = right.substring(0, right.lastIndexOf('.'));
                  }
                  if (left.equals(right))
                  {
                    weakNameMatch = true;
                    partialMatches.add(sec2);
                  }
                } }
            }
            if ((partialMatches.size() != 1) || (!exactMatch) || (nameMatch) || (addressMatch) || (sizeMatch))
            {


              if (!exactMatch)
              {


                if ((partialMatches.size() != 1) || (!nameMatch) || (!updateMatch))
                {




                  namePad = "-";
                  int longestName = name.length();
                  for (Section partial : partialMatches)
                  {
                    if (name.length() > longestName)
                    {
                      longestName = name.length();
                    }
                  }
                  namePad = namePad + Integer.toString(longestName);
                  
                  secAddrErrors.add(String.format("Found: 0x%1$04X (%2$3d) %3$" + namePad + "s, %4$s\n", new Object[] { Long.valueOf(loadMemoryAddress), Long.valueOf(size), name, flags.wholeString }));
                  if (partialMatches.isEmpty())
                  {
                    secAddrErrors.add(String.format("       No likely related sections found\n", new Object[0]));
                  }
                  
                  for (Section partial : partialMatches)
                  {
                    secAddrErrors.add(String.format(" Prev: 0x%1$04X (%2$3d) %3$" + namePad + "s, %4$s\n", new Object[] { Long.valueOf(loadMemoryAddress), Long.valueOf(size), name, flags.wholeString })); }
                } }
            }
          }
        }
      }
      if ((!secAddrErrors.isEmpty()) && (p == 0))
      {
        System.err.println();
        for (String s : secAddrErrors)
        {
          System.err.print(s);
        }
        System.err.println();
      }
    }
  }
}
