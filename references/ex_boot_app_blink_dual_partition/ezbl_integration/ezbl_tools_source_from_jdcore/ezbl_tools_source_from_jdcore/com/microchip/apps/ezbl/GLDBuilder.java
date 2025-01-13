package com.microchip.apps.ezbl;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;


























public class GLDBuilder
{
  public GLDBuilder() {}
  
  public static void main(String[] args)
  {
    TextBlock deviceGLD = null;
    String dynamicSectionsHeader = "";String dynamicSections = "";String dynamicSectionsFooter = "";
    
    String equDefineSectionTagsRegexStart = "AUTOMATIC EQU/DEFINE FILL SECTION[^\n]*?\n";
    String equDefineSectionTagsRegexEnd = "(?<=\n)[ \t]*?/[*][^E\n]*END OF EQU/DEFINE FILL SECTION";
    String deviceSectionFillSectionTagsRegexStart = "AUTOMATIC DEVICE SECTION FILL SECTION[^\n]*?\n";
    String deviceSectionFillSectionTagsRegexEnd = "(?<=\n)[ \t]*?/[*][^E\n]*END OF DEVICE SECTION FILL SECTION";
    

    String stateSavePath = CommandAndBuildState.GetStateFilePath(args);
    EZBLState state = EZBLState.ReadFromFile(stateSavePath);
    state = CommandAndBuildState.ReadArgs(state, args);
    
    if (undecodedOptions.contains("-make_non_ezbl_gld"))
    {
      System.exit(CreateNonEZBLLinkerScripts(args));
    }
    

    if (!parseOkay)
    {
      System.out.print("\r\n" + Multifunction.FormatHelpText(79, 0, "Usage:") + Multifunction.FormatHelpText(79, 3, "java -jar ezbl_tools.jar --gldbuilder -mcpu=PIC [-options] \"path to XC16 bins\" \"linker_script.gld\"") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Inputs:") + Multifunction.FormatHelpText(79, 6, "Target PIC part number: ex: -mcpu=33EP64MC506, -mcpu=24FJ128GA010,-mcpu=33FJ256GP710A") + "\r\n" + Multifunction.FormatHelpText(79, 6, "Path to the XC16 bin installation folder for device .gld searching. Ex: \"C:\\Program Files (x86)\\Microchip\\xc16\\v1.33\\bin\"") + "\r\n" + Multifunction.FormatHelpText(79, 6, "Preexisting linker script (ex: ezbl_build_standalone.gld) to insert parsed device specific linker script information into.") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Options:") + Multifunction.FormatHelpText(79, 6, "-ignore=0x002000,0x3000") + Multifunction.FormatHelpText(79, 9, "Address range to drop from the device Flash MEMORY regions in the linker script. Typically, this is set to a bootloader start address for the first paramter and a bootloader end address for the second end parameter. The second parameter is exclusive, or 1 address greater than the actual ignore range. This option may be repeated if multiple address ranges should be ignored.") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Outputs:") + Multifunction.FormatHelpText(79, 6, "Appropriately tagged sections in the preexisting ezbl .gld linker script are updated to reflect the information in the current device .gld file.") + "\r\n");
      


















      return;
    }
    

    if ((partNumber == null) || ((!MCU16Mode) && (linkScriptPath == null)) || ((MCU16Mode) && (compilerLinkScriptPath == null) && (linkScriptPath == null)))
    {
      System.err.println("ezbl_tools: Missing required input parameter: need a device part number, compiler linker script to start from, and output linker script file specified.");
      System.exit(-1);
    }
    

    String gldContents = Multifunction.ReadFile(linkScriptPath, true);
    if (gldContents == null)
    {
      System.err.println("ezbl_tools: Unable to open \"" + linkScriptPath + "\". Ensure project contains valid EZBL .gld or .ld linker script.");
      System.exit(-2);
    }
    






    TextBlock outGLD = new TextBlock(gldContents);
    

    if (compilerLinkScriptPath != null)
    {
      gldContents = Multifunction.ReadFile(compilerLinkScriptPath, true);
      if (gldContents == null)
      {
        System.err.println("ezbl_tools: Unable to read \"" + compilerLinkScriptPath + "\"");
        System.exit(-3);
      }
      deviceGLD = new TextBlock(gldContents);
      compilerGLDContents = gldContents;
    }
    
    if (deviceGLD.Find("#define __EZBL_TOOLS_COMPUTED_TABLE_SIZE_CORRECTION_FILLER[^\n]{2,}?[\n]"))
    {
      deviceGLD.ReplaceOuter("");
    }
    

    while ((deviceGLD != null) && (deviceGLD.Find("SECTIONS[^{]*", "\\{", "\\}", "\n")))
    {
      if (dynamicSectionsHeader.isEmpty())
      {
        dynamicSectionsHeader = deviceGLD.GetOuterLeft(false);
        dynamicSectionsFooter = deviceGLD.GetOuterRight(false);
      }
      TextBlock sectionBlock = new TextBlock(deviceGLD.GetInner(true));
      

      if (sectionBlock.Find("\\s[.]text[.]EZBLTableSizeCorrectionFiller[^:]*:[^{]*", "\\{", "\\}", "\n"))
      {
        sectionBlock.ReplaceOuter("\n");
      }
      

      erasableIGT = "";
      if ((remapISRThroughIGT) && (MCU16Mode))
      {
        if (sectionBlock.Find("\\s[.]ivt[^:]*:[^{]*", "\\{", "\\}", "\n"))
        {
          List<String> ivtLines = new ArrayList();
          List<String> igtLines = new ArrayList();
          igtLines.add("\n      #define GOTO(address)        LONG(0x040000 | (ABSOLUTE(address) & 0x00FFFF)); LONG(0x000000 | ((ABSOLUTE(address) & 0x7F0000)>>16))");
          igtLines.add("\n      #define BRA(short_address)   LONG(0x370000 | (((ABSOLUTE(short_address) - ABSOLUTE(. + 2))>>1) & 0x00FFFF))");
          

          int longestVectorName = 0;
          int longestVectorDesc = 0;
          for (int i = 0; i < ivtVectors.size(); i++)
          {
            InterruptVector vector = (InterruptVector)ivtVectors.get(Integer.valueOf(i));
            if (name.length() > longestVectorName)
              longestVectorName = name.length();
            if (desc.length() > longestVectorDesc)
              longestVectorDesc = desc.length();
          }
          if (longestVectorName > 31)
            longestVectorName = 31;
          if (longestVectorDesc > 40)
            longestVectorDesc = 40;
          String vecNamePad = String.valueOf(longestVectorName);
          String vecDescPad = String.valueOf(longestVectorDesc);
          

          int outputIndex = 0;
          for (int i = 0; i < ivtVectors.size(); i++)
          {
            InterruptVector vector = (InterruptVector)ivtVectors.get(Integer.valueOf(i));
            String formattedVectorName = String.format("_%1$-" + vecNamePad + "s", new Object[] { name });
            String vecComment = String.format("/* IRQ %2$3d, Vector %3$3d: %1$-" + vecDescPad + "s */", new Object[] { desc, Integer.valueOf(i - 8), Integer.valueOf(i) });
            if (i < 8) {
              vecComment = String.format("/* Trap %2$2d, Vector %3$3d: %1$-" + vecDescPad + "s */", new Object[] { desc, Integer.valueOf(i), Integer.valueOf(i) });
            }
            if (!implemented)
            {
              String s = String.format("\n      LONG(ABSOLUTE(_EZBL_IGT_BASE) + 0x%1$04X);", new Object[] { Integer.valueOf(ivtVectorsImplemented * igtSpacing) });
              ivtLines.add(String.format("%1$-" + String.valueOf(longestVectorName * 2 + 100) + "s  %2$s", new Object[] { s, vecComment }));
            }
            else
            {
              igtLines.add(String.format("\n      %1$s(DEFINED(_%2$s) ? ABSOLUTE(_%2$s) : ABSOLUTE(__DefaultInterrupt));", new Object[] { igtSpacing == 2 ? "BRA" : "GOTO", formattedVectorName }));
              

              ivtLines.add(String.format("\n      LONG(DEFINED(EZBL_Dispatch%1$s) ? ABSOLUTE(EZBL_Dispatch%1$s) : ABSOLUTE(_EZBL_IGT_BASE) + 0x%2$04X);  %3$s", new Object[] { formattedVectorName, Integer.valueOf(outputIndex++ * igtSpacing), vecComment }));
            }
          }
          

          igtLines.add(String.format("\n      %1$s(ABSOLUTE(__DefaultInterrupt));", new Object[] { igtSpacing == 2 ? "BRA" : "GOTO" }));
          

          String newIVT = sectionBlock.GetOuterLeft(true).replaceAll("\n(?! )", "\n  ") + "{" + Multifunction.CatStringList(ivtLines) + "\n  " + sectionBlock.GetOuterRight(false);
          

          sectionBlock.ReplaceOuter(newIVT);
          erasableIGT = Multifunction.CatStringList(igtLines);
        }
      }
      
      if (deleteResetSection)
      {

        if (sectionBlock.Find("\\s[.]reset[^:]*:[^{]*", "\\{", "\\}", "\n"))
        {
          sectionBlock.ReplaceOuter("\n");
        }
      }
      dynamicSections = dynamicSections + sectionBlock.GetFullBlock();
      deviceGLD.DeleteOuter();
    }
    



    if (encryptionKey != null)
    {
      String saltString = "";
      for (int i = 0; i < 16; i++)
      {
        saltString = saltString + String.format("%1$02X", new Object[] { Byte.valueOf(encryptionSalt[i]) });
      }
      
      dynamicSections = dynamicSections + String.format("\n  /* Encryption password: %1$s */ ", new Object[] { encryptionPassword }) + "\n  /* Encryption salt: " + saltString + " */" + "\n  EZBL_CryptKey :" + "\n  {" + "\n      _EZBL_CryptKey = ABSOLUTE(.);" + "\n      ";
      




      for (int i = 0; i < 15; i += 3)
      {
        dynamicSections = dynamicSections + String.format("LONG(0x%3$02X%2$02X%1$02X); ", new Object[] { Byte.valueOf(encryptionKey[i]), Byte.valueOf(encryptionKey[(i + 1)]), Byte.valueOf(encryptionKey[(i + 2)]) });
      }
      dynamicSections = dynamicSections + String.format("LONG(0x%3$02X%2$02X%1$02X); ", new Object[] { Byte.valueOf(encryptionKey[15]), Integer.valueOf(0), Integer.valueOf(0) });
      dynamicSections = dynamicSections + "\n  } >" + mainFlashRegion.name + "\n";
    }
    


    dynamicSections = dynamicSectionsHeader + dynamicSections + dynamicSectionsFooter;
    

    if ((MCU16Mode) && (outGLD.Find(equDefineSectionTagsRegexStart, equDefineSectionTagsRegexEnd)))
    {
      List<String> deviceParameters = new ArrayList();
      deviceParameters.add(String.format("_EZBL_ADDRESSES_PER_SECTOR      = 0x%1$06X;\n", new Object[] { Integer.valueOf(state.getEraseBlockSizeAddresses()) }));
      deviceParameters.add(String.format("_EZBL_MAIN_FLASH_BASE           = 0x%1$06X;\n", new Object[] { Long.valueOf(mainFlashRegion.startAddr) }));
      deviceParameters.add(String.format("_EZBL_MAIN_FLASH_END_ADDRESS    = 0x%1$06X;\n", new Object[] { Long.valueOf(mainFlashRegion.endAddr) }));
      deviceParameters.add(String.format("_EZBL_CONFIG_BASE               = 0x%1$06X;\n", new Object[] { Long.valueOf(configWordsRegion == null ? 0L : configWordsRegion.startAddr) }));
      deviceParameters.add(String.format("_EZBL_CONFIG_END_ADDRESS        = 0x%1$06X;\n", new Object[] { Long.valueOf(configWordsRegion == null ? 0L : configWordsRegion.endAddr) }));
      deviceParameters.add(String.format("_EZBL_DEVID_ADDRESS             = 0x%1$06X;\n", new Object[] { Long.valueOf(devIDAddr) }));
      deviceParameters.add(String.format("_EZBL_DEVID_MASK                = 0x%1$06X;\n", new Object[] { Long.valueOf(devIDMask) }));
      deviceParameters.add(String.format("_EZBL_DEVID_VALUE               = 0x%1$06X;\n", new Object[] { Long.valueOf(devIDValue) }));
      deviceParameters.add(String.format("_EZBL_REVID_ADDRESS             = 0x%1$06X;\n", new Object[] { Long.valueOf(devRevAddr) }));
      deviceParameters.add(String.format("_EZBL_REVID_MASK                = 0x%1$06X;\n", new Object[] { Long.valueOf(devRevMask) }));
      deviceParameters.add(String.format("_EZBL_RESERVED_BIT_ADDRESS      = 0x%1$06X;   %2$s\n", new Object[] { Long.valueOf(devSpecialConf.reservedBitAddr), "/* " + devSpecialConf.reserveBitConfigName + " */" }));
      deviceParameters.add(String.format("_EZBL_RESERVED_BIT_MASK         = 0x%1$06X;\n", new Object[] { Long.valueOf(devSpecialConf.reservedBitMask) }));
      deviceParameters.add(String.format("_EZBL_CODE_PROTECT_ADDRESS      = 0x%1$06X;   %2$s\n", new Object[] { Long.valueOf(devSpecialConf.codeProtectAddr), "/* " + devSpecialConf.codeProtectConfigName + " */" }));
      deviceParameters.add(String.format("_EZBL_CODE_PROTECT_MASK         = 0x%1$06X;\n", new Object[] { Long.valueOf(devSpecialConf.codeProtectMask) }));
      deviceParameters.add(String.format("_EZBL_BACKBUG_ADDRESS           = 0x%1$06X;   %2$s\n", new Object[] { Long.valueOf(devSpecialConf.BACKBUGAddr), "/* " + devSpecialConf.BACKBUGConfigName + " */" }));
      deviceParameters.add(String.format("_EZBL_BACKBUG_MASK              = 0x%1$06X;\n", new Object[] { Long.valueOf(devSpecialConf.BACKBUGMask) }));
      
      for (MemoryRegion p : devConfigWordsByAddr.values())
      {
        deviceParameters.add(String.format("_EZBL_%1$-25s = 0x%2$06X;\n", new Object[] { name, Long.valueOf(startAddr) }));
      }
      

      outGLD.ReplaceInner(deviceGLD.GetFullBlock() + "\n" + "\n" + Multifunction.CatStringList(deviceParameters) + "\n" + String.format("#define __EZBL_PROGRAM_BASE       0x%1$06X\n", new Object[] { Long.valueOf(mainFlashRegion.startAddr) }) + String.format("#define __EZBL_PROGRAM_LENGTH     0x%1$06X\n", new Object[] { Long.valueOf(mainFlashRegion.endAddr - mainFlashRegion.startAddr) }) + String.format("#define __EZBL_PROGRAM_ERASE_SIZE 0x%1$06X\n", new Object[] { Integer.valueOf(eraseBlockSizeAddresses) }) + String.format("#define __EZBL_BASE_ADDRESS       0x%1$06X\n", new Object[] { Long.valueOf(baseAddress) }) + String.format("#define __EZBL_IGT_ADDRESSES      0x%1$06X", new Object[] { Integer.valueOf(remapISRThroughIGT ? 4 * (ivtVectorsImplemented + 1) : 0) }) + " /* Number of Flash addresses needed to store all Interrupt Goto Table entries, including one for the __DefaultInterrupt */\n" + "\n");












    }
    else if (MCU16Mode)
    {

      outGLD.ResetFind();
      System.err.println("ezbl_tools: Unable to find AUTOMATIC EQU/DEFINE FILL SECTION in " + linkScriptPath + "; skipping insertion of this section. Ensure correct linker script in use and marker lines are present.");
    }
    


    if ((MCU16Mode) && (outGLD.Find(deviceSectionFillSectionTagsRegexStart, deviceSectionFillSectionTagsRegexEnd)))
    {
      outGLD.ReplaceInner(dynamicSections);
    }
    else if (MCU16Mode)
    {

      outGLD.ResetFind();
      System.err.println("ezbl_tools: Unable to find AUTOMATIC DEVICE SECTION FILL SECTION in " + linkScriptPath + "; skipping insertion of this section. Ensure correct linker script in use and marker lines are present.");
    }
    

    if (Multifunction.UpdateFileIfDataDifferent(linkScriptPath, outGLD.GetFullBlock(), true) < 0)
    {
      System.err.println("ezbl_tools: failed to write to \"" + linkScriptPath + "\"");
    }
    
    state.SaveToFile();
  }
  


  public static String makeMCU16LinkerScript(EZBLState state)
  {
    List<String> ivtLines = new ArrayList();
    List<String> igtLines = new ArrayList();
    List<String> equates = new ArrayList();
    List<String> macros = new ArrayList();
    

    if ((partNumber == null) || ((!MCU16Mode) && (linkScriptPath == null)) || ((MCU16Mode) && (compilerLinkScriptPath == null) && (linkScriptPath == null)))
    {
      System.err.println("ezbl_tools: Missing required input parameter: need a device part number, compiler linker script to start from, and output linker script file specified.");
      System.exit(-1);
    }
    

    String ret = Multifunction.ReadFile(linkScriptPath, true);
    if (ret == null)
    {
      System.err.println("ezbl_tools: Unable to open \"" + linkScriptPath + "\". Ensure project contains valid EZBL linker script.");
      System.exit(-2);
    }
    

    String compilerGLD = Multifunction.ReadFile(compilerLinkScriptPath, true);
    if (compilerGLD == null)
    {
      System.err.println("ezbl_tools: Unable to read \"" + compilerLinkScriptPath + "\"");
      System.exit(-3);
    }
    

    ivtLines.add("  /* EZBL Bootloader project: redirect interrupt vectors to Application Interrupt Goto Table */\n  .ivt __IVT_BASE : \n  {");
    

    igtLines.add("#else\n  /* EZBL Application project: receive interrupts from Bootloader and branch to ISR */\n  .igt _EZBL_IGT_BASE : \n  {");
    




    int longestVectorName = 0;
    int longestVectorDesc = 0;
    for (int i = 0; i < ivtVectors.size(); i++)
    {
      InterruptVector vector = (InterruptVector)ivtVectors.get(Integer.valueOf(i));
      if (name.length() > longestVectorName)
        longestVectorName = name.length();
      if (desc.length() > longestVectorDesc)
        longestVectorDesc = desc.length();
    }
    if (longestVectorName > 31)
      longestVectorName = 31;
    if (longestVectorDesc > 40)
      longestVectorDesc = 40;
    String vecNamePad = String.valueOf(longestVectorName);
    String vecDescPad = String.valueOf(longestVectorDesc);
    

    int outputIndex = 0;
    for (int i = 0; i < ivtVectors.size(); i++)
    {
      InterruptVector vector = (InterruptVector)ivtVectors.get(Integer.valueOf(i));
      String formattedVectorName = String.format("_%-" + vecNamePad + "s", new Object[] { name });
      String vecComment = String.format("/* IRQ %2$3d, Vector %3$3d: %1$-" + vecDescPad + "s */", new Object[] { desc, Integer.valueOf(i - 8), Integer.valueOf(i) });
      if (i < 8) {
        vecComment = String.format("/* Trap %2$2d, Vector %3$3d: %1$-" + vecDescPad + "s */", new Object[] { desc, Integer.valueOf(i), Integer.valueOf(i) });
      }
      if (!implemented)
      {
        String s = String.format("    LONG(ABSOLUTE(_EZBL_IGT_BASE) + 0x%04X);", new Object[] { Integer.valueOf(ivtVectorsImplemented * igtSpacing) });
        ivtLines.add(String.format("%-" + String.valueOf(longestVectorName * 2 + 98) + "s %s", new Object[] { s, vecComment }));
      }
      else
      {
        igtLines.add(String.format("    %1$s(DEFINED(_%2$s) ? ABSOLUTE(_%2$s) : ABSOLUTE(__DefaultInterrupt));", new Object[] { igtSpacing == 2 ? "BRA" : "GOTO", formattedVectorName }));
        

        ivtLines.add(String.format("    LONG(DEFINED(EZBL_Dispatch%1$s) ? ABSOLUTE(EZBL_Dispatch%1$s) : ABSOLUTE(_EZBL_IGT_BASE) + 0x%2$04X);  %3$s", new Object[] { formattedVectorName, Integer.valueOf(outputIndex++ * igtSpacing), vecComment }));
      }
    }
    ivtLines.add("  } > ivt");
    

    igtLines.add(String.format("    %1$s(ABSOLUTE(__DefaultInterrupt));", new Object[] { igtSpacing == 2 ? "BRA" : "GOTO" }));
    igtLines.add("  } > program\n#endif");
    


    erasableIGT = Multifunction.CatStringList(igtLines);
    
    macros.add("/**\n * EZBL Generated Information\n */");
    

    macros.add("#define GOTO(address)             LONG(0x040000 | (ABSOLUTE(address) & 0x00FFFF)); LONG(0x000000 | ((ABSOLUTE(address) & 0x7F0000)>>16))");
    macros.add("#define BRA(short_addr)           LONG(0x370000 | (((ABSOLUTE(short_addr) - ABSOLUTE(. + 2))>>1) & 0x00FFFF))");
    macros.add("#define BTSC(address, bit)        LONG(0xAF0000 | (ABSOLUTE(address) & 0x1FFE) | ((ABSOLUTE(bit) & 0x7)<<13) | ((ABSOLUTE(bit) & 0x8)>>3))");
    macros.add("\n\n\n");
    macros.add(String.format("#define __EZBL_PROGRAM_BASE       0x%06X", new Object[] { Long.valueOf(mainFlashRegion.startAddr) }));
    macros.add(String.format("#define __EZBL_PROGRAM_LENGTH     0x%06X", new Object[] { Long.valueOf(mainFlashRegion.endAddr - mainFlashRegion.startAddr) }));
    macros.add(String.format("#define __EZBL_PROGRAM_ERASE_SIZE 0x%06X", new Object[] { Integer.valueOf(eraseBlockSizeAddresses) }));
    macros.add(String.format("#define __EZBL_BASE_ADDRESS       0x%06X", new Object[] { Long.valueOf(baseAddress) }));
    macros.add(String.format("#define __EZBL_IGT_ADDRESSES      0x%06X", new Object[] { Integer.valueOf(remapISRThroughIGT ? 4 * (ivtVectorsImplemented + 1) : 0) }));
    

    equates.add(String.format("_EZBL_appBootloadState          = 0x%06X;", new Object[] { Long.valueOf(baseAddressOfAppErasable) }));
    equates.add(String.format("_EZBL_APP_RESET_BASE            = 0x%06X;", new Object[] { Long.valueOf(baseAddressOfGotoReset) }));
    equates.add(String.format("_EZBL_IGT_BASE                  = 0x%06X;", new Object[] { Long.valueOf(baseAddressOfIGT) }));
    equates.add(String.format("_EZBL_ADDRESSES_PER_SECTOR      = 0x%06X;", new Object[] { Integer.valueOf(state.getEraseBlockSizeAddresses()) }));
    equates.add(String.format("_EZBL_MAIN_FLASH_BASE           = 0x%06X;", new Object[] { Long.valueOf(mainFlashRegion.startAddr) }));
    equates.add(String.format("_EZBL_MAIN_FLASH_END_ADDRESS    = 0x%06X;", new Object[] { Long.valueOf(mainFlashRegion.endAddr) }));
    if (configWordsRegion != null)
    {
      equates.add(String.format("_EZBL_CONFIG_BASE               = 0x%06X;", new Object[] { Long.valueOf(configWordsRegion.startAddr) }));
      equates.add(String.format("_EZBL_CONFIG_END_ADDRESS        = 0x%06X;", new Object[] { Long.valueOf(configWordsRegion.endAddr) }));
    }
    equates.add(String.format("_EZBL_DEVID_ADDRESS             = 0x%06X;", new Object[] { Long.valueOf(devIDAddr) }));
    equates.add(String.format("_EZBL_DEVID_MASK                = 0x%06X;", new Object[] { Long.valueOf(devIDMask) }));
    equates.add(String.format("_EZBL_DEVID_VALUE               = 0x%06X;", new Object[] { Long.valueOf(devIDValue) }));
    equates.add(String.format("_EZBL_REVID_ADDRESS             = 0x%06X;", new Object[] { Long.valueOf(devRevAddr) }));
    equates.add(String.format("_EZBL_REVID_MASK                = 0x%06X;", new Object[] { Long.valueOf(devRevMask) }));
    if (devSpecialConf.reserveBitConfigName != null)
    {
      equates.add(String.format("_EZBL_RESERVED_BIT_ADDRESS      = 0x%1$06X;   /* %2$s */", new Object[] { Long.valueOf(devSpecialConf.reservedBitAddr), devSpecialConf.reserveBitConfigName }));
      equates.add(String.format("_EZBL_RESERVED_BIT_MASK         = 0x%1$06X;", new Object[] { Long.valueOf(devSpecialConf.reservedBitMask) }));
    }
    equates.add(String.format("_EZBL_CODE_PROTECT_ADDRESS      = 0x%1$06X;   /* %2$s */", new Object[] { Long.valueOf(devSpecialConf.codeProtectAddr), devSpecialConf.codeProtectConfigName }));
    equates.add(String.format("_EZBL_CODE_PROTECT_MASK         = 0x%1$06X;", new Object[] { Long.valueOf(devSpecialConf.codeProtectMask) }));
    equates.add(String.format("_EZBL_BACKBUG_ADDRESS           = 0x%1$06X;   /* %2$s */", new Object[] { Long.valueOf(devSpecialConf.BACKBUGAddr), devSpecialConf.BACKBUGConfigName }));
    equates.add(String.format("_EZBL_BACKBUG_MASK              = 0x%1$06X;", new Object[] { Long.valueOf(devSpecialConf.BACKBUGMask) }));
    for (MemoryRegion p : devConfigWordsByAddr.values())
    {
      equates.add(String.format("_EZBL_%1$-25s = 0x%2$06X;", new Object[] { name, Long.valueOf(startAddr) }));
    }
    
    ret = compilerGLD;
    

    Pattern p = Pattern.compile("(?m-is)^[\\s]*?SECTIONS[^{}]*?\\{");
    Matcher m = p.matcher(ret);
    if (!m.find())
    {
      System.err.printf("ezbl_tools.jar: unable to locate SECTIONS {...} in '%s'\n", new Object[] { linkScriptPath });
      System.exit(-4);
    }
    ret = ret.substring(0, m.start()) + "\n\n" + Multifunction.CatStringList(macros, "\n") + "\n\n" + Multifunction.CatStringList(equates, "\n") + "\n\n\n" + ret.substring(m.start());
    







    PairWithText secMapping = findOutputSectionMapping(ret, ".ivt");
    if (secMapping == null) {
      System.out.printf("ezbl_tools.jar: did not locate .ivt section in '%s'\n", new Object[] { linkScriptPath });
    } else {
      ret = ret.substring(0, (int)first) + ret.substring((int)second);
    }
    secMapping = findOutputSectionMapping(ret, ".aivt");
    if (secMapping != null) {
      ret = ret.substring(0, (int)first) + ret.substring((int)second);
    }
    
    secMapping = findOutputSectionMapping(ret, ".reset");
    if (secMapping == null)
    {
      System.err.printf("ezbl_tools.jar: unable to locate .reset section in '%s'\n", new Object[] { linkScriptPath });
      System.exit(-5);
    }
    ret = ret.substring(0, (int)first) + "#if defined(EZBL_BOOT_PROJECT)" + ret.substring((int)first, (int)second).replaceAll("(?m)^#", "  #").replaceFirst("(?m)(?<=\\{)[^}]+(?=\\})", "\n    GOTO(__reset)\n  ") + "\n" + Multifunction.CatStringList(ivtLines, "\n") + "\n" + Multifunction.CatStringList(igtLines, "\n") + "\n" + ret.substring((int)second);
    








    state.SaveToFile();
    

    if (Multifunction.UpdateFileIfDataDifferent(linkScriptPath, ret, false) < 0)
    {
      System.err.println("ezbl_tools: failed to write to \"" + linkScriptPath + "\"");
      System.exit(-6);
    }
    
    return ret;
  }
  











  public static PairWithText findOutputSectionMapping(String searchText, String outputSectionName)
  {
    if (!searchText.contains(outputSectionName)) {
      return null;
    }
    

    char firstLetter = outputSectionName.charAt(0);
    String secName = "\\Q" + outputSectionName + "\\E";
    
    Pattern p = Pattern.compile("(?m-is)(?<=(\n\n))([^\\" + firstLetter + "{}]+?" + secName + "[^:]*?:[^{]*\\{[^}]*\\}[^>]*>[^\n]+\n([^\n}]+\n)*)");
    Matcher m = p.matcher(searchText);
    
    if (m.find())
    {
      return new PairWithText(m.start(), m.end(), m.group(2));
    }
    
    p = Pattern.compile("(?m-is)(?:(([\\s]*?\\{[^\n]*?\n(?!\n))|(\n\n)))([^\\" + firstLetter + "{}]+?" + secName + "[^:]*?:[^{]*\\{[^}]*\\}[^>]*>[^\n]+\n([^\n}]+\n)*)");
    m = p.matcher(searchText);
    if (m.find())
    {
      return new PairWithText(m.start(), m.end(), m.group(4));
    }
    
    return null;
  }
  
















  public static String[] CreateMergeScript(EZBLState state, Bl2b bl2)
  {
    TreeMap<String, Symbol> romSymbols = new TreeMap();
    List<Symbol> ramSymbols = new ArrayList();
    List<Symbol> exportableSymbols = new ArrayList();
    List<Symbol> symbolsConsumed = new ArrayList();
    


    List<PairWithText> gldPairs = new ArrayList();
    List<PairWithText> gldRAMSectionAllocations = new ArrayList();
    



    List<String> symbolTable = new ArrayList();
    List<String> exportTable = new ArrayList();
    List<String> asmMerge = new ArrayList();
    MemoryRegion mrEZBL_ForwardBootloaderISR = null;
    List<Section> romNoLoadSections = new ArrayList();
    

    String[] retGLDASM = new String[2];
    
    if ((elfDump == null) || (state.romUseRegions == null))
    {

      if ((partNumber == null) || ((compilerGLDContents == null) && (compilerLinkScriptPath == null) && (MCU16Mode)))
      {
        if (!silent)
        {
          System.err.println("ezbl_tools: Missing required input parameter. Device part number and compiler linker script path required.");
        }
        System.exit(-10);
      }
    }
    

    List<MemoryRegion> romUseRegions = state.romUseRegions;
    if (MCU16Mode)
    {
      MemoryRegion lastInstrWord = ((MemoryRegion)state.romUseRegions.get(0)).clone();
      startAddr = (mainExecutionRegion.endAddr - 2L);
      endAddr = mainExecutionRegion.endAddr;
      romUseRegions = MemoryRegion.SubtractRegions(state.romUseRegions, lastInstrWord, false);
    }
    



    for (MemoryRegion mr : romUseRegions)
    {
      bl2.FillData(mr, (byte)-1);
    }
    

    for (int i = 0; i < records.size(); i++)
    {
      DataRecord d = (DataRecord)records.get(i);
      d.normalizePIC32Addresses();
      

      Section dSec = new Section();
      loadMemoryAddress = address;
      size = data.length;
      isROM = true;
      data = d;
      flags.CODE = true;
      MemoryRegion mr = dSec.mapToDeviceRegion(devProgramSpaces, MemoryRegion.Partition.values()[targetPartition]);
      mr.alignToProgSize();
      comment = comment;
      if ((address >= startAddr) && (address < endAddr))
      {
        if (d.getEndAddress() > endAddr)
        {

          DataRecord splitRecord = d.SplitAtAddress(endAddr);
          records.add(i + 1, splitRecord);
        }
        assignedMemory = name;
      }
      else if ((d.getEndAddress() > startAddr) && (address <= startAddr))
      {

        DataRecord splitRecord = d.SplitAtAddress(startAddr);
        records.add(i + 1, splitRecord);
      }
      


      byte sum = -1;
      for (byte dByte : data)
      {
        sum = (byte)(sum & dByte);
      }
      if (sum == -1)
      {
        records.remove(d);
        i--;
      }
    }
    


    if (elfDump != null)
    {
      if (coreType == EZBLState.CPUClass.mm)
      {
        elfDump.normalizePIC32Addresses();
        Symbol gp = (Symbol)elfDump.symbolsByName.get("_gp");
        if (gp == null) {
          System.err.println("ezbl_tools.jar: could not determine Bootloader's Global Pointer (_gp) address. Application projects will not be able to safely call Bootloader functions.");
        } else {
          exportableSymbols.add(gp);
        }
      }
      


      elfDump.removeSections("[.]text[.]EZBL_AppReserved.*");
      

      for (Symbol s : elfDump.symbols)
      {
        if (s.isExportSuitable())
        {

          exportableSymbols.add(s);
        }
      }
      
      List<String> noSymExportNames = Arrays.asList(new String[] { "__Size_block", "__Aldata", "__reset", "__resetPRI", "__psv_init" });
      




      List<String> noSymExportNameStarts = Arrays.asList(new String[] { "__crt_start_mode", "__data_init", "_persist_", "_sbss_", "_sdata_", "__pic32_data_init", "_dinit", "_on_reset", "_on_bootstrap", "_nmi_handler", "_reset", "_startup", "__preinit_array", "__init_array", "__fini_array" });
      
















      List<String> noSymExportRegEx = new ArrayList();
      Section noSymExports = (Section)elfDump.sectionsByName.get(".info.EZBL_NoExportSYM");
      if ((noSymExports != null) && (data != null) && (data.data.length != 0))
      {
        noSymExportRegEx = Arrays.asList(new String(data.data).split("\000"));
        for (int i = 0; i < noSymExportRegEx.size(); i++)
        {
          String noExport = (String)noSymExportRegEx.get(i);
          try
          {
            for (int j = 0; j < exportableSymbols.size(); j++)
            {
              Symbol s = (Symbol)exportableSymbols.get(j);
              if (name.matches(noExport))
              {
                if (verbose)
                  System.out.printf("ezbl_tools.jar: suppressing export of '%s' due to .info.EZBL_NoExportSYM(\"%s\");\n", new Object[] { name, noExport });
                exportableSymbols.remove(s);
                j--;
              }
            }
          }
          catch (PatternSyntaxException ex)
          {
            System.err.printf("ezbl_tools.jar: found unrecognized regular expression '%s' in .info.EZBL_NoExportSYM, ignoring\n", new Object[] { noExport });
            noSymExportRegEx.remove(i--);
          }
        }
      }
      

      Section noSectionExports = (Section)elfDump.sectionsByName.get(".info.EZBL_NoExportSection");
      List<String> noSectionExportRegEx = new ArrayList();
      int i; if ((noSectionExports != null) && (data != null) && (data.data.length != 0))
      {
        noSectionExportRegEx = Arrays.asList(new String(data.data).split("\000"));
        for (i = 0; i < noSectionExportRegEx.size();)
        {
          String noSectionExport = (String)noSectionExportRegEx.get(i);
          try
          {
            if ("".matches(noSectionExport)) {}
            i++;




          }
          catch (PatternSyntaxException ex)
          {




            System.err.printf("ezbl_tools.jar: found unrecognized regular expression '%s' in .info.EZBL_NoExportSection, ignoring\n", new Object[] { noSectionExport });
            noSectionExportRegEx.remove(i--);
          }
        }
      }
      

      for (Section sec : elfDump.sections)
      {

        if (((isRAM) || (isROM)) && 
        

          (!sec.nameMatchesRegEx(noSectionExportRegEx)))
        {


          if ((isROM) && (flags.ALLOC) && (flags.NEVER_LOAD))
          {
            if (((!name.startsWith("reserve_")) || ((loadMemoryAddress != 3217032336L) && (loadMemoryAddress != 2680161424L) && (loadMemoryAddress != 532677776L))) && ((!name.startsWith("reserve_")) || ((loadMemoryAddress != 134217728L) && (loadMemoryAddress != 167772160L) && (loadMemoryAddress != 0L))))
            {




              romNoLoadSections.add(sec);
            }
          } else {
            sec.LoadSymbols(exportableSymbols);
            for (int i = 0; i < symbols.size(); i++)
            {
              Symbol sym = (Symbol)symbols.get(i);
              
              if ((isRAM) && (name.endsWith("EZBL_ForwardBootloaderISR"))) {
                mrEZBL_ForwardBootloaderISR = sec.mapToDeviceRegion(devMemories, null);
              }
              if ((sym.nameMatchesRegEx(noSymExportRegEx)) || (sym.nameMatches(noSymExportNames)) || (sym.nameStartsWith(noSymExportNameStarts)))
              {
                exportableSymbols.remove(sym);
              }
              else
              {
                if (section.startsWith("."))
                {
                  if ((section.equals(".init")) || (section.equals(".fini")) || (section.equals(".preinit_array")) || (section.equals(".init_array")) || (section.equals(".fini_array")) || (section.equals(".ctors")) || (section.equals(".dtors")))
                  {






                    exportableSymbols.remove(sym);
                    continue;
                  }
                }
                else if (name.startsWith("__vector_dispatch_")) {
                  name = name.replace("__vector_dispatch_", "EZBL_BOOT_vector_dispatch_");
                } else if (name.equals("_general_exception_context")) {
                  name = "EZBL_BOOT_general_exception_context";
                } else if (name.equals("_general_exception_handler")) {
                  name = "EZBL_BOOT_general_exception_handler";
                } else if (name.equals("EZBL_general_exception")) {
                  name = "EZBL_BOOT_general_exception";
                } else if (name.equals("EZBL_TrapHandler")) {
                  name = "EZBL_BOOT_TrapHandler";
                } else if ((name.equals("malloc")) || (name.equals("calloc")) || (name.equals("realloc")) || (name.equals("free"))) {
                  name = ("EZBL_BOOT_" + name);
                } else if (name.startsWith("__alloc")) {
                  name = ("EZBL_BOOT" + name);
                } else if (name.equals("_bootstrap_exception_handler")) {
                  name = "EZBL_BOOT__bootstrap_exception_handler";
                }
                
                if (isRAM) {
                  ramSymbols.add(sym);
                } else if (isROM) {
                  romSymbols.put(name, sym);
                } else if ((!name.startsWith("EZBL")) && (!name.startsWith("_EZBL")))
                  exportableSymbols.remove(sym);
              }
            }
          } }
      }
      Collections.sort(ramSymbols, new SymbolNameComparator());
    }
    

    asmMerge.add("/**\n * EZBL Bootloader Code and RAM Allocation\n *\n * Automatically generated file - not intended for manual editing. If changes \n * are made here, they will normally be overwritten when you rebuild your \n * Bootloader. If necessary, maintain a backup copy and manually merge your \n * customizations back in.\n * \n * Built for:\n *     " + fullPartNumber + (dualPartitionMode ? " (Dual Partition mode)" : "") + "\n * From:" + "\n *     " + artifactPath + "\n * Using build configuration:" + "\n *     " + conf + "\n * " + "\n * Tool paths:" + "\n *     " + Multifunction.GetCanonicalPath(new StringBuilder().append(compilerFolder).append("/..").toString()) + "\n *     " + Multifunction.GetCanonicalPath(new StringBuilder().append(pathToIDEBin).append("/../..").toString()) + "\n */" + "\n" + "\n ; Validate matching target processors between Bootloader and Application projects." + "\n ; If you get this error and wish to force the two dissimilar targets together anyway," + "\n ; you can comment out these .error statements. However, something will likely be" + "\n ; broken at run time, so do so only if you are sure of what you are doing." + (MCU16Mode ? "\n    .ifndef __" + partNumber + "\n    .error \"Bootloader's " + projectName + ".merge.S/.gld files were generated for a different target processor.\"" + "\n    .error \"Recompile this Application project or the " + projectName + " Bootloader project for the same hardware.\"" + "\n    .endif" : new StringBuilder().append("\n#if !defined(__").append(partNumber).append("__)").append("\n    .error \"Bootloader's ").append(projectName).append(".merge.S file was generated for a different target processor.\"").append("\n    .error \"Recompile this Application project or the ").append(projectName).append(" Bootloader project for the same hardware.\"").append("\n#endif").toString()) + "\n" + "\n");
    



































    asmMerge.add(String.format("\n;----Target Bootloader ID and this Application Version meta data for .bl2 file header----\n    .pushsection    .info.EZBL_metaParameters, info, keep\n    .weak   _BOOTID_HASH0\n    .weak   _BOOTID_HASH1\n    .weak   _BOOTID_HASH2\n    .weak   _BOOTID_HASH3\n    .weak   _APPID_VER_BUILD\n    .weak   _APPID_VER_MINOR\n    .weak   _APPID_VER_MAJOR\n_BOOTID_HASH0 = 0x%1$08X\n_BOOTID_HASH1 = 0x%2$08X\n_BOOTID_HASH2 = 0x%3$08X\n_BOOTID_HASH3 = 0x%4$08X\nEZBL_metaAppIDVerBuild:\n    .long   _APPID_VER_BUILD\nEZBL_metaAppIDVerMinor:\n    .short  _APPID_VER_MINOR\nEZBL_metaAppIDVerMajor:\n    .short  _APPID_VER_MAJOR\n    .popsection\n", new Object[] { Integer.valueOf(bootIDHash[0]), Integer.valueOf(bootIDHash[1]), Integer.valueOf(bootIDHash[2]), Integer.valueOf(bootIDHash[3]) }));
    





















    if (mrEZBL_ForwardBootloaderISR != null)
    {
      gldPairs.add(new PairWithText(startAddr, endAddr, String.format("\n#if defined(EZBL_HIDE_BOOT_SYMBOLS) /* EZBL_ForwardBootloaderISR exists here and cannot be hidden */\n  EZBL_RAM_AT_0x%1$04X 0x%1$04X :\n  {\n    *(EZBL_RAM_AT_0x%1$04X); /* [0x%1$04X, 0x%2$04X), contains %3$d bytes */\n  } >%4$s\n#endif\n", new Object[] { Long.valueOf(startAddr), Long.valueOf(endAddr), Long.valueOf(endAddr - startAddr), name })));
      







      asmMerge.add("\n;----Bootloader reserved static RAM----\n#if defined(EZBL_HIDE_BOOT_SYMBOLS)" + String.format("\n    ; Bootloader RAM block intended for %1$s region\n    ; 0x%2$04X to 0x%3$04X, length 0x0004 (4 bytes)\n    .pushsection    EZBL_RAM_AT_0x%2$04X, address(0x%2$04X), persist, keep", new Object[] { name, Long.valueOf(startAddr), Long.valueOf(endAddr - startAddr) }) + "\n    .global     _EZBL_ForwardBootloaderISR" + "\n    .type       _EZBL_ForwardBootloaderISR, @object" + "\n_EZBL_ForwardBootloaderISR: ; This variable cannot be hidden since ISR dispatch code uses it" + "\n    .space      0x4" + "\n    .size       _EZBL_ForwardBootloaderISR, . - _EZBL_ForwardBootloaderISR" + "\n    .popsection" + "\n#endif" + "\n");
    }
    











    Collections.sort(ramUseRegions);
    
    if ((MCU16Mode) && (linkedAsDebugImage))
    {
      MemoryRegion mr = (MemoryRegion)ramUseRegions.get(0);
      if (startAddr + 80L != endAddr)
      {
        mr = mr.clone();
        startAddr -= 80L;
        endAddr = (startAddr + 80L);
        ramUseRegions.add(0, mr);
      }
    }
    
    asmMerge.add("\n#if !defined(EZBL_HIDE_BOOT_SYMBOLS)");
    for (Iterator i$ = ramUseRegions.iterator(); i$.hasNext();) { mr = (MemoryRegion)i$.next();
      
      asmMerge.add(mr.toASMString());
      gldRAMSectionAllocations.add(new PairWithText(startAddr, endAddr, mr.toLinkerString()));
      

      for (Symbol s : ramSymbols)
      {
        if ((address >= startAddr) && (address < endAddr))
        {
          symbolsConsumed.add(s);
          symbolTable.add(String.format("\n    .equ        %1$s, 0x%2$06X", new Object[] { name, Long.valueOf(address) }));
          if (name.equals("_EZBL_ForwardBootloaderISR"))
          {
            exportTable.add(String.format("\n    .global     %1$s", new Object[] { name }));
          }
          else
          {
            exportTable.add(String.format("\n    .weak       %1$s", new Object[] { name }));
          }
          if (flags.function)
          {
            exportTable.add(String.format("\n    .type       %1$s, @function", new Object[] { name }));
          }
          else if (flags.object)
          {
            exportTable.add(String.format("\n    .type       %1$s, @object", new Object[] { name })); }
        }
      }
    }
    MemoryRegion mr;
    asmMerge.add("\n#endif");
    

    if (MCU16Mode)
    {
      asmMerge.add("\n\n;----App erasable items that the Bootloader knows about----");
      asmMerge.add(String.format("\n    .pushsection    EZBL_AppErasable, address(0x%1$06X), code, keep\n    ; EZBL_appBootloadState - Base address of EZBL_INSTALLED_FLAGS structure (18 bytes/6 instruction words)\n    .pword      0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x12CDFF\n\n    ; _EZBL_APP_RESET_BASE\n    .extern     __reset\n    goto        __reset\n\n    ; Interrupt Goto Table follows for 0x%2$X addresses (0x%3$06X to 0x%4$06X)\n    ; The .igt section is defined in the linker script since this is an application\n    ; erasable/updatable structure.\n    .popsection", new Object[] { Long.valueOf(baseAddressOfAppErasable), Integer.valueOf(sizeOfIGT), Long.valueOf(baseAddressOfIGT), Long.valueOf(baseAddressOfIGT + sizeOfIGT) }));
      














      gldPairs.add(new PairWithText(baseAddressOfAppErasable, baseAddressOfIGT, String.format("\n  EZBL_AppErasable 0x%1$06X :\n  {\n    *(EZBL_AppErasable); /* [0x%1$06X, 0x%2$06X), contains %3$d bytes */\n  } >program\n", new Object[] { Long.valueOf(baseAddressOfAppErasable), Long.valueOf(baseAddressOfAppErasable + sizeOfAppErasable), Integer.valueOf(sizeOfAppErasable / 2 * 3) })));
      






      if ((erasableIGT != null) && (!erasableIGT.isEmpty()))
      {
        gldPairs.add(new PairWithText(baseAddressOfIGT, baseAddressOfIGT + sizeOfIGT, String.format("\n  /* Application's erasable Interrupt Goto Table */\n  .igt 0x%1$06X :\n  { /* [0x%1$06X, 0x%3$06X), contains %4$d bytes */%2$s\n  } >program\n", new Object[] { Long.valueOf(baseAddressOfIGT), erasableIGT, Long.valueOf(baseAddressOfIGT + sizeOfIGT), Integer.valueOf(sizeOfIGT / 2 * 3) })));


      }
      


    }
    else
    {


      Symbol sym = (Symbol)elfDump.symbolsByName.get("EZBL_BOOTLOADER_SIZE");
      if (sym != null)
      {
        asmMerge.add(String.format("\n\n;----Bootloader reserved flash space propagated from the bootloader project----\n    .global EZBL_BOOTLOADER_SIZE\nEZBL_BOOTLOADER_SIZE = 0x%08X\n\n", new Object[] { Long.valueOf(address) }));
      }
      




      sym = (Symbol)elfDump.symbolsByName.get("EZBL_appBootloadState");
      asmMerge.add(String.format("\n\n;----App erasable items that the Bootloader knows about----\n    .pushsection    .EZBL_appBootloadState, address(0x%08X), code, keep\n    ; EZBL_appBootloadState - Base address of EZBL_INSTALLED_FLAGS structure (24 bytes/6 instruction words)\n    .global EZBL_appBootloadState\n    .global EZBL_appIDVer\n    .type   EZBL_appBootloadState, @object\n    .type   EZBL_appIDVer, @object\nEZBL_appBootloadState:\nEZBL_appIDVer:\n    .long   _APPID_VER_BUILD    ; EZBL_appIDVer.appIDVerBuild\n    .short  _APPID_VER_MINOR    ; EZBL_appIDVer.appIDVerMinor\n    .short  _APPID_VER_MAJOR    ; EZBL_appIDVer.appIDVerMajor\n    .size   EZBL_appIDVer, . - EZBL_appIDVer\n    .long   0xFFFFFFFF          ; hash32 - not defined at compile time, set when run-time programmed\n    .long   0xFFFFFFFF          ; CRC32  - not defined at compile time, set when run-time programmed\n    .long   0xFFFF12CD          ; appInstalled == EZBL_APP_INSTALLED (0x12CD)\n    .long   0xFFFFFFFF          ; padding to fit 3x flash double words\n    .size   EZBL_appBootloadState, . - EZBL_appBootloadState\n    .popsection\n", new Object[] { Long.valueOf(address) }));
      



















      gldPairs.add(new PairWithText(baseAddressOfAppErasable, baseAddressOfIGT, String.format("\n  EZBL_appBootloadState 0x%1$08X :\n  {\n    KEEP(*(.EZBL_appBootloadState*)); /* [0x%1$08X, 0x%2$08X), contains %3$d bytes */\n  } >kseg0_program_mem\n", new Object[] { Long.valueOf(baseAddressOfAppErasable), Long.valueOf(baseAddressOfAppErasable + sizeOfAppErasable), Integer.valueOf(sizeOfAppErasable) })));
    }
    






    if ((MCU16Mode) && (elfDump != null))
    {
      Section constSect = (Section)elfDump.romSectionMapByName.get(".const");
      





      if (constSect != null)
      {
        MemoryRegion constRegion = new MemoryRegion(mainExecutionRegion.startAddr, (mainExecutionRegion.startAddr & 0xFFFFFFFFFFFF8000) + 32768L);
        type = MemoryRegion.MemType.ROM;
        startAddr = (loadMemoryAddress & 0xFFFFFFFFFFFF8000);
        endAddr = (startAddr + 32768L);
        constRegion.copyMetaData(constSect.mapToDeviceRegion(devProgramSpaces, MemoryRegion.Partition.values()[targetPartition]));
        if (endAddr > mainExecutionRegion.endAddr) {
          endAddr = mainExecutionRegion.endAddr;
        }
        List<MemoryRegion> usedSpaces = new ArrayList();
        




        for (DataRecord dr : records)
        {
          usedSpaces.add(dr.mapToDeviceRegion(devProgramSpaces, MemoryRegion.Partition.values()[targetPartition]));
        }
        usedSpaces = MemoryRegion.alignToEraseSize(usedSpaces);
        usedSpaces = MemoryRegion.coalesce(usedSpaces, 0, 0, false);
        MemoryRegion appErasableRegion = new MemoryRegion(baseAddressOfAppErasable, baseAddressOfAppErasable + sizeOfAppErasable);
        type = MemoryRegion.MemType.ROM;
        usedSpaces.add(appErasableRegion);
        List<MemoryRegion> constPageHoles = MemoryRegion.SubtractRegions(constRegion.getAsList(), usedSpaces, false);
        
        TreeMap<Long, MemoryRegion> holesMappedByWidth = new TreeMap();
        for (MemoryRegion constHole : constPageHoles)
        {
          holesMappedByWidth.put(Long.valueOf(endAddr - startAddr), constHole);
        }
        MemoryRegion biggestConstHole = constRegion.clone();
        endAddr = startAddr;
        if (holesMappedByWidth.lastKey() != null)
          biggestConstHole = (MemoryRegion)holesMappedByWidth.lastEntry().getValue();
        long constPageBiggestHoleSize = endAddr - startAddr;
        
        gldPairs.add(new PairWithText(startAddr, endAddr, String.format("\n#if !defined(EZBL_HIDE_BOOT_SYMBOLS)\n  /*\n  ** .const compatibility with Bootloader .const section\n  ** \n  ** Explicitly locate the .const section so the largest possible PSV \n  ** window is available for the Application project without using a \n  ** different PSVPAG/DSRPAG value than the Bootloader project requires \n  ** for API compatibility. \n  */\n  .const 0x%1$06X : AT(0x%2$06X)\n  {\n    LONG(0xFFFFFF);/* Dummy word to avoid empty .const section and prevent linker de-initializing CORCON<PSV>/DSRPAG when App starts */\n    *(.const);     /* 0x%3$06X (%3$d) available addresses/bytes for auto PSV constants */\n  } >program\n#endif\n", new Object[] { Long.valueOf(0x8000 | startAddr & 0x7FFF), Long.valueOf(startAddr), Long.valueOf(constPageBiggestHoleSize) })));
        
















        if ((constPageBiggestHoleSize < minFreePSVSpace) || (constPageBiggestHoleSize < warnFreePSVSpace))
        {
          if (!silent)
          {
            long spaceNeeded = Math.max(Math.max(warnFreePSVSpace, minFreePSVSpace), eraseBlockSizeAddresses);
            long alignedAppReservedEnd = loadMemoryAddress + (32768L - loadMemoryAddress % 32768L);
            long alignedAppReservedStart = loadMemoryAddress + size;
            if (alignedAppReservedStart % eraseBlockSizeAddresses != 0L)
              alignedAppReservedStart += eraseBlockSizeAddresses - alignedAppReservedStart % eraseBlockSizeAddresses;
            for (MemoryRegion mr : devProgramSpaces)
            {
              if ((loadMemoryAddress >= startAddr) && (loadMemoryAddress + size <= endAddr) && (alignedAppReservedEnd > endAddr))
              {
                alignedAppReservedEnd = endAddr - 2L;
                break;
              }
            }
            long spaceMakable = alignedAppReservedEnd - alignedAppReservedStart;
            while (spaceMakable > spaceNeeded + 4096L)
            {
              alignedAppReservedStart += eraseBlockSizeAddresses;
              spaceMakable = alignedAppReservedEnd - alignedAppReservedStart;
            }
            
            if (spaceMakable < spaceNeeded)
            {
              long alignedAppReservedEnd2 = loadMemoryAddress & (eraseBlockSizeAddresses ^ 0xFFFFFFFF);
              long alignedAppReservedStart2 = loadMemoryAddress & 0xFFFFFFFFFFFF7FFF;
              if (alignedAppReservedStart2 < eraseBlockSizeAddresses)
                alignedAppReservedStart2 = eraseBlockSizeAddresses;
              long spaceMakable2 = alignedAppReservedEnd2 - alignedAppReservedStart2;
              if (spaceMakable2 > spaceMakable)
              {
                while (spaceMakable2 > spaceNeeded + 4096L)
                {
                  alignedAppReservedEnd2 -= eraseBlockSizeAddresses;
                  spaceMakable2 = alignedAppReservedEnd2 - alignedAppReservedStart2;
                  if (spaceMakable2 > spaceNeeded + 4096L)
                  {
                    alignedAppReservedStart2 += eraseBlockSizeAddresses;
                    spaceMakable2 = alignedAppReservedEnd2 - alignedAppReservedStart2;
                  }
                }
                alignedAppReservedStart = alignedAppReservedStart2;
                alignedAppReservedEnd = alignedAppReservedEnd2;
                spaceMakable = spaceMakable2;
              }
            }
            
            if (spaceMakable < spaceNeeded)
            {
              alignedAppReservedEnd = loadMemoryAddress + (32768L - loadMemoryAddress % 32768L) + 32768L;
              alignedAppReservedStart = loadMemoryAddress + size - 2L;
              alignedAppReservedStart -= alignedAppReservedStart % eraseBlockSizeAddresses;
              while (alignedAppReservedEnd - alignedAppReservedStart > spaceNeeded + 4096L)
              {
                alignedAppReservedEnd -= eraseBlockSizeAddresses;
              }
              for (MemoryRegion mr : devProgramSpaces)
              {
                if ((loadMemoryAddress >= startAddr) && (loadMemoryAddress + size <= endAddr) && (alignedAppReservedEnd > endAddr))
                {
                  alignedAppReservedEnd = endAddr - 2L;
                  break;
                }
              }
              spaceMakable = alignedAppReservedEnd - alignedAppReservedStart;
            }
            
            if (spaceMakable < spaceNeeded)
            {
              alignedAppReservedStart = eraseBlockSizeAddresses;
              alignedAppReservedEnd = alignedAppReservedStart + (spaceNeeded % eraseBlockSizeAddresses == 0L ? spaceNeeded : spaceNeeded + (eraseBlockSizeAddresses - spaceNeeded % eraseBlockSizeAddresses));
              for (MemoryRegion mr : devProgramSpaces)
              {
                if ((loadMemoryAddress >= startAddr) && (loadMemoryAddress + size <= endAddr) && (alignedAppReservedEnd > endAddr))
                {
                  alignedAppReservedEnd = endAddr - 2L;
                  break;
                }
              }
            }
            
            System.err.print("EZBL check " + (constPageBiggestHoleSize < minFreePSVSpace ? "error:\n" : "warning:\n") + Multifunction.FormatHelpText(String.format(".const found at 0x%1$06X; %2$d bytes free on PSV/EDS page\n\nIf calling Bootloader APIs from an Application project, all constants must reside on the same PSV/EDS page. To avoid running out of PSV addresses in future Application projects, recompile this Bootloader project with space explicitly reserved for the Application using the EZBL_SetAppReservedHole() macro, ex:\n    EZBL_SetAppReservedHole(0x%3$06X, 0x%4$06X);\nTo change the checking thresholds for this diagnostic, specify the -min_free_psv=x and/or -warn_free_psv=y command line options.", new Object[] { Long.valueOf(loadMemoryAddress), Long.valueOf(constPageBiggestHoleSize), Long.valueOf(alignedAppReservedStart), Long.valueOf(alignedAppReservedEnd) }), 110, 4, 0, false));
            







            System.err.flush();
          }
          if (constPageBiggestHoleSize < minFreePSVSpace)
          {
            System.exit(-10);
          }
        }
      }
    }
    
    if (!romNoLoadSections.isEmpty())
    {
      asmMerge.add("\n\n\n;----Bootloader defined noload holes in program memory----");
      for (Section sec : romNoLoadSections)
      {
        long byteLen = size;
        MemoryRegion mr = sec.mapToDeviceRegion(devProgramSpaces, MemoryRegion.Partition.values()[targetPartition]);
        if (MCU16Mode)
        {
          byteLen = byteLen / 2L * 3L;
        }
        asmMerge.add(String.format("\n\n    ; Bootloader noload attributed hole intended for program region '%5$s'\n    ; 0x%1$06X to 0x%2$06X, length 0x%3$06X (%4$d bytes; needs %6$d pages)\n    .pushsection    %7$s, address(0x%1$06X), code, keep, noload\n    .skip 0x%3$06X\n    .popsection", new Object[] { Long.valueOf(loadMemoryAddress), Long.valueOf(loadMemoryAddress + size), Long.valueOf(size), Long.valueOf(byteLen), name, Long.valueOf((byteLen - 1L) / eraseBlockSizeBytes), name }));
        





        gldPairs.add(new PairWithText(loadMemoryAddress, loadMemoryAddress + size, mr.toLinkerString(new String[] { "(NOLOAD)" })));
        

        for (Symbol s : symbols)
        {
          symbolsConsumed.add(s);
          symbolTable.add(String.format("\n    .equ        %1$s, 0x%2$06X", new Object[] { name, Long.valueOf(address) }));
          exportTable.add(String.format("\n    .weak       %1$s", new Object[] { name }));
          if (flags.function) {
            exportTable.add(String.format("\n    .type       %1$s, @function", new Object[] { name }));
          } else if (flags.object) {
            exportTable.add(String.format("\n    .type       %1$s, @object", new Object[] { name }));
          }
        }
      }
    }
    asmMerge.add("\n\n\n;----Bootloader reserved program memory----");
    for (DataRecord d : records)
    {
      d.normalizePIC32Addresses();
      MemoryRegion mr = d.mapToDeviceRegion(devProgramSpaces, MemoryRegion.Partition.values()[targetPartition]);
      String secName = String.format("EZBL_ROM_AT_0x%06X", new Object[] { Long.valueOf(startAddr) });
      if ((type.equals(MemoryRegion.MemType.BYTEFUSE)) || (type.equals(MemoryRegion.MemType.FLASHFUSE)))
        secName = "EZBL_BTLDR_CONFIG_WORD_" + name;
      asmMerge.add(String.format("\n\n    ; Bootloader code block intended for program region '%5$s'\n    ; 0x%1$06X to 0x%2$06X, length 0x%3$06X (%4$d bytes; needs %6$d pages)\n    .pushsection    " + secName + ", address(0x%1$06X), code, keep", new Object[] { Long.valueOf(address), Long.valueOf(d.getEndAddress()), Long.valueOf(d.getEndAddress() - address), Integer.valueOf(data.length), assignedMemory, Integer.valueOf((data.length - 1) / eraseBlockSizeBytes) }));
      




      gldPairs.add(new PairWithText(address, d.getEndAddress(), mr.toLinkerString()));
      

      List<Symbol> functionsHere = new ArrayList();
      for (Symbol s : romSymbols.values())
      {
        if ((address >= address) && (address < d.getEndAddress()))
        {
          symbolsConsumed.add(s);
          if (flags.function)
          {
            if (MCU16Mode)
            {
              symbolTable.add(String.format("\n    .equ        %s, 0x%06X", new Object[] { name, Long.valueOf(address) }));
              exportTable.add(String.format("\n    .type       %s, @function", new Object[] { name }));
            }
            functionsHere.add(s);
          }
          else
          {
            symbolTable.add(String.format("\n    .equ        %s, 0x%06X", new Object[] { name, Long.valueOf(address) }));
            if (flags.object)
              exportTable.add(String.format("\n    .type       %s, @object", new Object[] { name }));
          }
          exportTable.add(String.format("\n    .weak       %s", new Object[] { name }));
        }
      }
      

      asmMerge.add("\n" + d.getDataAsASMCode(functionsHere));
      asmMerge.add("\n    .popsection");
    }
    

    exportableSymbols.removeAll(symbolsConsumed);
    Collections.sort(exportableSymbols, new SymbolNameComparator());
    for (Symbol s : exportableSymbols)
    {
      symbolTable.add(String.format("\n    .equ        %1$s, 0x%2$06X", new Object[] { name, Long.valueOf(address) }));
      exportTable.add(String.format("\n    .weak       %1$s", new Object[] { name }));
      if (flags.function)
      {
        exportTable.add(String.format("\n    .type       %1$s, @function", new Object[] { name }));
      }
      else if (flags.object)
      {
        exportTable.add(String.format("\n    .type       %1$s, @object", new Object[] { name }));
      }
    }
    
    asmMerge.add("\n\n\n#if !defined(EZBL_HIDE_BOOT_SYMBOLS)");
    if (!exportTable.isEmpty())
    {
      asmMerge.add("\n\n;----Bootloader symbol export table----" + Multifunction.CatStringList(exportTable) + "\n");
    }
    

    if (!symbolTable.isEmpty())
    {
      asmMerge.add("\n\n\n;----Bootloader symbol addresses----" + Multifunction.CatStringList(symbolTable) + "\n");
    }
    

    asmMerge.add("\n#endif");
    asmMerge.add("\n");
    

    String gldContents = compilerGLDContents;
    if (gldContents == null)
    {
      String referenceLinkerFile = MCU16Mode ? compilerLinkScriptPath : linkScriptPath;
      gldContents = Multifunction.ReadFile(referenceLinkerFile, true);
      if (gldContents == null)
      {
        if (!silent)
        {
          System.err.println("ezbl_tools: Unable to read \"" + referenceLinkerFile + "\"");
        }
        System.exit(-2);
      }
    }
    
    TextBlock outGLD = new TextBlock(gldContents);
    String dynamicSectionsHeader = "";String dynamicSections = "";String dynamicSectionsFooter = "";
    



    if (!MCU16Mode)
      Collections.sort(gldPairs, new PairWithText.PIC32MMBootFirstAddrComparator());
    String gldROMSectionAllocation = "";
    String gldRAMSectionAllocation = "";
    for (PairWithText p : gldRAMSectionAllocations)
    {
      gldRAMSectionAllocation = gldRAMSectionAllocation + text;
    }
    for (PairWithText p : gldPairs)
    {


      gldROMSectionAllocation = gldROMSectionAllocation + text;
    }
    



    while (outGLD.Find("SECTIONS[^{]*?", "\\{", "\\}", "\n"))
    {
      if (dynamicSectionsHeader.isEmpty())
      {
        dynamicSectionsHeader = outGLD.GetOuterLeft(false);
        dynamicSectionsFooter = outGLD.GetOuterRight(false);
      }
      TextBlock sectionBlock = new TextBlock(outGLD.GetInner(true));
      
      if (MCU16Mode)
      {






        if (sectionBlock.Find("\\s[.]reset[^:]*?:[^{]*?", "\\{", "\\}", "\n"))
        {
          sectionBlock.ReplaceOuter(gldRAMSectionAllocation + "\n" + gldROMSectionAllocation + "\n");
        }
        

        if (sectionBlock.Find("\\s[.]ivt[^:]*?:[^{]*?", "\\{", "\\}", "\n"))
          sectionBlock.ReplaceOuter("\n");
        if (sectionBlock.Find("\\s[.]aivt[^:]*?:[^{]*?", "\\{", "\\}", "\n")) {
          sectionBlock.ReplaceOuter("\n");
        }
      }
      else {
        if (sectionBlock.Find("\\s\\.reset[^:]*?:[^{]*?", "\\{", "\\}", "\n"))
          sectionBlock.InsertInner(gldROMSectionAllocation + "\n", true, true);
        if (sectionBlock.Find("\\s\\.dbg_data[^:]*?:[^{]*?", "\\{", "\\}", "\n")) {
          sectionBlock.InsertInner(gldRAMSectionAllocation + "\n", false, false);
        }
      }
      dynamicSections = dynamicSections + sectionBlock.GetFullBlock();
      outGLD.DeleteOuter();
    }
    

    dynamicSections = dynamicSections.replace("/*\n** Interrupt Vector Table\n*/\n", "");
    

    dynamicSections = dynamicSectionsHeader + dynamicSections + dynamicSectionsFooter;
    retGLDASM[0] = (outGLD.GetFullBlock().replaceFirst("(?dmus)(/\\*.*?Generic linker script.*?\\*/\\s+?)", new StringBuilder().append("$1\n\n#if !defined(__").append(partNumber).append("__)\n").append("#define __").append(partNumber).append("__  1\n").append("#endif\n").toString()) + "\n\n" + dynamicSections);
    





    retGLDASM[1] = Multifunction.CatStringList(asmMerge);
    if (!MCU16Mode) {
      retGLDASM[1] = retGLDASM[1].replace(';', '#').replaceAll("bytes\\#", Matcher.quoteReplacement("bytes;"));
    }
    return retGLDASM;
  }
  











  public static int CreateNonEZBLLinkerScripts(String[] args)
  {
    EZBLState state = CommandAndBuildState.ReadArgs(null, args);
    List<String> deviceParameters = new ArrayList();
    List<String> commonSections = new ArrayList();
    List<String> bootSections = new ArrayList();
    List<String> appSections = new ArrayList();
    List<String> appIGT = new ArrayList();
    List<String> ivt = new ArrayList();
    List<String> bootISRDispatchers = new ArrayList();
    
    int appIGTSpacing = 4;
    

    String gldOriginalSectionsHeader = "";String gldOriginalSections = "";String gldOriginalSectionsFooter = "";
    

    if (!parseOkay)
    {
      System.out.print("\r\n" + Multifunction.FormatHelpText(79, 0, "Usage:") + Multifunction.FormatHelpText(79, 3, "java -jar ezbl_tools.jar --gldbuilder -make_non_ezbl_glds -mcpu=PIC [-options] -compiler_folder=\"path to XC16 bins\"") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Inputs:") + Multifunction.FormatHelpText(79, 6, "Target PIC part number: ex: -mcpu=33EP64MC506, -mcpu=24FJ128GA010,-mcpu=33FJ256GP710A") + "\r\n" + Multifunction.FormatHelpText(79, 6, "Path to the XC16 bin installation folder for device .gld searching. Ex: \"C:\\Program Files (x86)\\Microchip\\xc16\\v1.xx\\bin\"") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Outputs:") + Multifunction.FormatHelpText(79, 6, "Generated .gld linker script file for two App project with interrupt multiplexing printed to stdout.") + "\r\n");
      












      return -1;
    }
    

    if ((partNumber == null) || (compilerLinkScriptPath == null))
    {
      System.err.println("ezbl_tools: Missing required input parameter: need a device part number, compiler linker script to start from, and output linker script file specified.");
      return -2;
    }
    

    String gldContents = Multifunction.ReadFile(compilerLinkScriptPath, true);
    if (gldContents == null)
    {
      System.err.println("ezbl_tools: Unable to read \"" + compilerLinkScriptPath + "\"");
      return -3;
    }
    
    TextBlock deviceGLD = new TextBlock(gldContents);
    
    while (deviceGLD.Find("SECTIONS[^{]*", "\\{", "\\}", "\n"))
    {
      if (gldOriginalSectionsHeader.isEmpty())
      {
        gldOriginalSectionsHeader = deviceGLD.GetOuterLeft(false);
        gldOriginalSectionsFooter = deviceGLD.GetOuterRight(false);
      }
      TextBlock sectionBlock = new TextBlock(deviceGLD.GetInner(true));
      





      if (sectionBlock.Find("((?ms)\\s/[*].*[*]/\\s\\.ivt)|((?-ms)\\s\\.ivt)[^:]*:[^{]*[^:]*:[^{]*", "\\{", "\\}", "\n"))
      {

        String gldParse = sectionBlock.GetInner(true);
        
        gldParse = gldParse.replaceAll("__DefaultInterrupt", "");
        gldParse = gldParse.replaceAll("LONG[\\s]*?[(]", "");
        gldParse = gldParse.replaceAll("ABSOLUTE[\\s]*?[(]", "");
        gldParse = gldParse.replaceAll(":[\\s\\S]*?;", "");
        gldParse = gldParse.replaceAll("DEFINED[\\s]*?[(][\\s\\S]*?[?]", "");
        gldParse = gldParse.replaceAll("[)]", "");
        gldParse = gldParse.replaceAll("[\\s]+", "\n");
        gldParse = gldParse.trim();
        




        int implementedVectorCount = 0;
        for (int i = 0; i < ivtVectors.size(); i++)
        {
          if (!ivtVectors.get(Integer.valueOf(i))).name.startsWith("__Interrupt"))
          {


            implementedVectorCount++;
          }
        }
        
        int outputIndex = 0;
        

        for (int i = 0; i < ivtVectors.size(); i++)
        {
          if (ivtVectors.get(Integer.valueOf(i))).name.startsWith("__Interrupt"))
          {
            ivt.add(String.format("%1$-54s/* Vector at 0x%2$06X, IRQ %3$3d, Vector %4$3d, target _APP_RESET_VECTOR + 0x%5$03X, %6$s coalesces to _DefaultInterrupt() */", new Object[] { "LONG(ABSOLUTE(_Dispatch__DefaultInterrupt));", Integer.valueOf(4 + 2 * i), Integer.valueOf(i), Integer.valueOf(i + 8), Integer.valueOf(4 + implementedVectorCount * appIGTSpacing), ivtVectors.get(Integer.valueOf(i))).name }));
          }
          else
          {
            String formattedVectorName = String.format("%1$-18s", new Object[] { ivtVectors.get(Integer.valueOf(i))).name });
            String igtCode = "GOTO(DEFINED(" + formattedVectorName + ") ? " + formattedVectorName + " : __DefaultInterrupt);";
            appIGT.add(String.format("%1$-84s/* IRQ %2$3d, Vector %3$3d, _APP_RESET_VECTOR + 0x%4$03X */", new Object[] { igtCode, Integer.valueOf(i), Integer.valueOf(i + 8), Integer.valueOf(4 + outputIndex * appIGTSpacing) }));
            ivt.add(String.format("%1$-54s/* Vector at 0x%2$06X, IRQ %3$3d, Vector %4$3d, target _APP_RESET_VECTOR + 0x%5$03X */", new Object[] { "LONG(ABSOLUTE(_Dispatch" + ivtVectors.get(Integer.valueOf(i))).name + "));", Integer.valueOf(4 + 2 * i), Integer.valueOf(i), Integer.valueOf(i + 8), Integer.valueOf(4 + outputIndex * appIGTSpacing) }));
            bootISRDispatchers.add(String.format("_Dispatch%1$18s /* IRQ %2$3d, Vector %3$3d */", new Object[] { ivtVectors.get(Integer.valueOf(i))).name + " = ABSOLUTE(.);", Integer.valueOf(i), Integer.valueOf(i + 8) }));
            bootISRDispatchers.add("    BTSC(_IVTMuxTarget, 0);");
            bootISRDispatchers.add("    BRA(_APP1_RESET_VECTOR + 0x" + String.format("%1$04X);", new Object[] { Integer.valueOf(4 + outputIndex * appIGTSpacing) }));
            bootISRDispatchers.add("    BRA(_APP2_RESET_VECTOR + 0x" + String.format("%1$04X);", new Object[] { Integer.valueOf(4 + outputIndex++ * appIGTSpacing) }));
          }
        }
        
        appIGT.add(String.format("%1$-84s/* _APP_RESET_VECTOR + 0x%2$03X, Unimplemented/coalesced IRQs */", new Object[] { "GOTO(__DefaultInterrupt);", Integer.valueOf(4 + implementedVectorCount * appIGTSpacing) }));
        bootISRDispatchers.add(String.format("_Dispatch%1$18s /* Unimplemented/coalesced IRQs */", new Object[] { "__DefaultInterrupt = ABSOLUTE(.);" }));
        bootISRDispatchers.add("    BTSC(_IVTMuxTarget, 0);");
        bootISRDispatchers.add("    BRA(_APP1_RESET_VECTOR + 0x" + String.format("%1$04X);", new Object[] { Integer.valueOf(4 + implementedVectorCount * appIGTSpacing) }));
        bootISRDispatchers.add("    BRA(_APP2_RESET_VECTOR + 0x" + String.format("%1$04X);", new Object[] { Integer.valueOf(4 + implementedVectorCount * appIGTSpacing) }));
      }
      

      sectionBlock.ReplaceOuter("");
      


      if (sectionBlock.Find("\\s[.]reset[^:]*:[^{]*", "\\{", "\\}", "\n"))
      {
        sectionBlock.ReplaceOuter("");
      }
      
      gldOriginalSections = gldOriginalSections + sectionBlock.GetFullBlock();
      deviceGLD.DeleteOuter();
    }
    

    gldOriginalSections = gldOriginalSectionsHeader + gldOriginalSections + gldOriginalSectionsFooter;
    

    deviceParameters.add(deviceGLD.GetFullBlock());
    

    deviceParameters.add("\n\n");
    deviceParameters.add(String.format("\n_EZBL_ADDRESSES_PER_SECTOR      = 0x%1$06X;", new Object[] { Integer.valueOf(eraseBlockSizeAddresses) }));
    deviceParameters.add(String.format("\n_EZBL_MAIN_FLASH_BASE           = 0x%1$06X;", new Object[] { Long.valueOf(mainFlashRegion.startAddr | (dualPartitionMode ? 4194304 : 0)) }));
    deviceParameters.add(String.format("\n_EZBL_MAIN_FLASH_END_ADDRESS    = 0x%1$06X;", new Object[] { Long.valueOf(mainFlashRegion.endAddr | (dualPartitionMode ? 4194304 : 0)) }));
    deviceParameters.add(String.format("\n_EZBL_CONFIG_BASE               = 0x%1$06X;", new Object[] { Long.valueOf(configWordsRegion == null ? 0L : configWordsRegion.startAddr | (dualPartitionMode ? 4194304 : 0)) }));
    deviceParameters.add(String.format("\n_EZBL_CONFIG_END_ADDRESS        = 0x%1$06X;", new Object[] { Long.valueOf(configWordsRegion == null ? 0L : configWordsRegion.endAddr | (dualPartitionMode ? 4194304 : 0)) }));
    deviceParameters.add(String.format("\n_EZBL_DEVID_ADDRESS             = 0x%1$06X;", new Object[] { Long.valueOf(devIDAddr) }));
    deviceParameters.add(String.format("\n_EZBL_DEVID_MASK                = 0x%1$06X;", new Object[] { Long.valueOf(devIDMask) }));
    deviceParameters.add(String.format("\n_EZBL_DEVID_VALUE               = 0x%1$06X;", new Object[] { Long.valueOf(devIDValue) }));
    deviceParameters.add(String.format("\n_EZBL_REVID_ADDRESS             = 0x%1$06X;", new Object[] { Long.valueOf(devRevAddr) }));
    deviceParameters.add(String.format("\n_EZBL_REVID_MASK                = 0x%1$06X;", new Object[] { Long.valueOf(devRevMask) }));
    deviceParameters.add(String.format("\n_EZBL_RESERVED_BIT_ADDRESS      = 0x%1$06X;   %2$s", new Object[] { Long.valueOf(devSpecialConf.reservedBitAddr | (dualPartitionMode ? 4194304 : 0)), "/* " + devSpecialConf.reserveBitConfigName + " */" }));
    deviceParameters.add(String.format("\n_EZBL_RESERVED_BIT_MASK         = 0x%1$06X;", new Object[] { Long.valueOf(devSpecialConf.reservedBitMask) }));
    deviceParameters.add(String.format("\n_EZBL_CODE_PROTECT_ADDRESS      = 0x%1$06X;   %2$s", new Object[] { Long.valueOf(devSpecialConf.codeProtectAddr | (dualPartitionMode ? 4194304 : 0)), "/* " + devSpecialConf.codeProtectConfigName + " */" }));
    deviceParameters.add(String.format("\n_EZBL_CODE_PROTECT_MASK         = 0x%1$06X;", new Object[] { Long.valueOf(devSpecialConf.codeProtectMask) }));
    deviceParameters.add(String.format("\n_EZBL_BACKBUG_ADDRESS           = 0x%1$06X;   %2$s", new Object[] { Long.valueOf(devSpecialConf.BACKBUGAddr | (dualPartitionMode ? 4194304 : 0)), "/* " + devSpecialConf.BACKBUGConfigName + " */" }));
    deviceParameters.add(String.format("\n_EZBL_BACKBUG_MASK              = 0x%1$06X;", new Object[] { Long.valueOf(devSpecialConf.BACKBUGMask) }));
    deviceParameters.add("\n\n/* Define macros for generating Reset Vector, IVT, IGT and run-time multiplexing dispatch code */\n#define BTSC(address, bit)   LONG(0xAF0000 | (ABSOLUTE(address) & 0x1FFE) | ((ABSOLUTE(bit) & 0x7)<<13) | ((ABSOLUTE(bit) & 0x8)>>3))\n#define BRA(short_address)   LONG(0x370000 | (((ABSOLUTE(short_address) - ABSOLUTE(. + 2))>>1) & 0x00FFFF))\n#define GOTO(address)        LONG(0x040000 | (ABSOLUTE(address) & 0x00FFFF)); LONG(0x000000 | ((ABSOLUTE(address) & 0x7F0000)>>16))\n\n");
    






    commonSections.add("\nSECTIONS\n{\n  /* INTRecipient global RAM variable allocated in all projects to select ISR to execute when HW Interrupt occurs */\n  .data.IVTMuxTarget (__DATA_BASE + 0x50) (NOLOAD) :\n  {\n    _IVTMuxTarget = .;\n    . += 2;\n  } >data\n");
    









    bootSections.add("\n\n/* Flash contents for Bootloader or other project that owns the hardware vectors */\n#if !defined(APP_RESET_VECTOR)    \n  #if !defined(__CORESIDENT) || defined(__DEFINE_RESET)\n  .reset :\n  {\n    GOTO(__reset);\n  } >reset\n  #endif\n\n\n  .ivt __IVT_BASE :\n  {\n    " + Multifunction.CatStringList(ivt, "\n    ") + "\n  } >ivt" + "\n" + "\n" + "\n  /**" + "\n   * Program code to run time select which ISR executes on hardware interrupt." + "\n   *   IVTMuxTarget<0> == '0' means App 1 ISRs execute." + "\n   *   IVTMuxTarget<0> == '1' means App 2 ISRs execute." + "\n   * Access the IVTMuxTarget variable using this declaration:" + "\n   *   extern volatile unsigned int __attribute__((near, keep, noload)) IVTMuxTarget;" + "\n   */" + "\n  .text.ISRDispatchers :" + "\n  {" + "\n    " + Multifunction.CatStringList(bootISRDispatchers, "\n    ") + "\n  } >program" + "\n");
    




























    appSections.add("\n#else    /* APP_RESET_VECTOR macro defined in project and set to the base program address for the Application project */\n\n  _APP_RESET_VECTOR = ABSOLUTE(APP_RESET_VECTOR);\n\n  .reset (ABSOLUTE(_APP_RESET_VECTOR) + 0x0) :\n  {\n    GOTO(__reset);\n  } >program\n\n\n  .igt (ABSOLUTE(_APP_RESET_VECTOR) + 0x4) :\n  {\n    " + Multifunction.CatStringList(appIGT, "\n    ") + "\n  } >program" + "\n#endif" + "\n}" + "\n" + "\n" + "\n");
    


















    System.out.print(Multifunction.CatStringList(deviceParameters) + Multifunction.CatStringList(commonSections) + Multifunction.CatStringList(bootSections) + Multifunction.CatStringList(appSections) + gldOriginalSections);
    return 0;
  }
}
