package com.microchip.apps.ezbl;

import java.io.File;
import java.io.PrintStream;
import java.io.Serializable;
import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.net.URLDecoder;
import java.security.CodeSource;
import java.security.ProtectionDomain;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;


























public class CommandAndBuildState
  implements Serializable
{
  public static final byte[] defaultEncryptionSalt = { -107, 15, -21, -39, -41, -70, -114, -51, -92, -120, 83, 96, 124, -28, -7, 75 };
  









  public CommandAndBuildState() {}
  








  public static EZBLState ReadArgs(EZBLState stateTemplate, String[] args)
  {
    String raw_mcpu = null;
    EZBLState originalState = null;
    EZBLState s = stateTemplate;
    

    if (s == null)
    {
      s = new EZBLState();
    }
    originalState = s.clone();
    
    for (int inIndex = 0; inIndex < args.length; inIndex++)
    {
      if ((args[inIndex] != null) && (args[inIndex].length() != 0))
      {




        if (args[inIndex].startsWith("--"))
        {
          args[inIndex] = args[inIndex].substring(1);
        }
        String token = args[inIndex].toLowerCase();
        String newOption = args[inIndex];
        int lastEqualIndex = newOption.lastIndexOf('=');
        if (lastEqualIndex >= 0)
        {
          newOption = newOption.substring(0, lastEqualIndex + 1) + "\"" + newOption.substring(lastEqualIndex + 1) + "\"";
        }
        
        if (token.startsWith("-state="))
        {
          ezblStateSavePath = Multifunction.TrimQuotes(args[inIndex].substring("-state=".length()));
        }
        else if ((token.matches("-16")) || (token.startsWith("-pic24")) || (token.startsWith("-dspic")))
        {
          MCU16Mode = true;
        }
        else if ((token.matches("-32")) || (token.startsWith("-pic32")))
        {
          MCU16Mode = false;
        }
        else if (token.startsWith("-noivtremap"))
        {
          remapISRThroughIGT = false;
        }
        else if (token.startsWith("-ivtremap"))
        {
          remapISRThroughIGT = true;
        }
        else if (token.startsWith("-delete.reset="))
        {
          deleteResetSection = Boolean.parseBoolean(Multifunction.TrimQuotes(args[inIndex].substring("-delete.reset=".length())));
        }
        else if (token.startsWith("-delete.reset"))
        {
          deleteResetSection = true;
        }
        else if (token.startsWith("-timeout="))
        {
          milliTimeout = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-timeout=".length()))).intValue();
        }
        else if ((token.startsWith("-save-temps")) || (token.startsWith("-save_temps")) || (token.startsWith("-log")))
        {
          saveTemps = true;
          if ((token.startsWith("-save-temps=")) || (token.startsWith("-save_temps=")))
            saveTempsFile = Multifunction.TrimQuotes(args[inIndex].substring("-save-temps=".length()));
          if (token.startsWith("-log=")) {
            saveTempsFile = Multifunction.TrimQuotes(args[inIndex].substring("-log=".length()));
          }
        } else if (token.startsWith("-base="))
        {
          baseAddress = Long.decode(Multifunction.TrimQuotes(args[inIndex].substring("-base=".length()))).longValue();
        }
        else if (token.startsWith("-pass="))
        {
          pass = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-pass=".length()))).intValue();
        }
        else if (token.startsWith("-ignore="))
        {
          ignoreROMRegions.add(new AddressRange(Multifunction.TrimQuotes(args[inIndex].substring("-ignore=".length()))).toMemoryRegion());
        }
        else if ((token.startsWith("-temp-folder=")) || (token.startsWith("-temp_folder=")))
        {
          temporariesPath = Multifunction.TrimQuotes(args[inIndex].substring("-temp-folder=".length()));
        }
        else if (token.startsWith("-password="))
        {
          encryptionPassword = Multifunction.TrimQuotes(args[inIndex].substring("-password=".length()));
        }
        else if (token.startsWith("-salt="))
        {
          encryptionSaltString = Multifunction.TrimQuotes(args[inIndex].substring("-salt=".length()));
        }
        else if (token.startsWith("-conf="))
        {
          conf = Multifunction.TrimQuotes(args[inIndex].substring("-conf=".length()));
        }
        else if (token.startsWith("-partition="))
        {
          targetPartition = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-partition=".length()))).intValue();
          dualPartitionMode = (targetPartition != 0);
        }
        else if (token.startsWith("-fboot="))
        {
          devSpecialConf.FBOOTValue = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-fboot=".length()))).intValue();
        }
        else if (token.startsWith("-makefile="))
        {
          makefilePath = Multifunction.TrimQuotes(args[inIndex].substring("-makefile=".length()));
        }
        else if (token.startsWith("-elf_artifact="))
        {
          elfPath = Multifunction.TrimQuotes(args[inIndex].substring("-elf_artifact=".length()));
        }
        else if (token.startsWith("-mcpu="))
        {
          raw_mcpu = args[inIndex];
          partNumber = Multifunction.TrimQuotes(args[inIndex].substring("-mcpu=".length()));
          partNumber = partNumber.replaceFirst(Matcher.quoteReplacement("([dD][sS][pP][iI][cC])|([pP][iI][cC])"), "");
          if ((partNumber != null) && (!partNumber.equals(partNumber)))
          {




            if (verbose)
            {
              System.out.print("EZBL: Different -mcpu value provided; ignoring stale " + ezblStateSavePath + " state file contents.\n");
            }
            EZBLState ret = ReadArgs(null, args);
            argsFromMakeEditor = args;
            return ret;
          }
          if ((partNumber.startsWith("33")) || (partNumber.startsWith("30")) || (partNumber.startsWith("24"))) {
            MCU16Mode = true;
          } else if (partNumber.startsWith("32"))
            MCU16Mode = false;
          if ((partNumber.startsWith("33")) || (partNumber.startsWith("30"))) {
            fullPartNumber = ("dsPIC" + partNumber);
          } else {
            fullPartNumber = ("PIC" + partNumber);
          }
        } else if (token.startsWith("-artifact="))
        {
          artifactPath = Multifunction.TrimQuotes(args[inIndex].substring("-artifact=".length()));
          hexPath = artifactPath;
        }
        else if (token.startsWith("-project_name="))
        {
          projectName = Multifunction.TrimQuotes(args[inIndex].substring("-project_name=".length()));
        }
        else if (token.startsWith("-linkscript="))
        {
          linkScriptPath = Multifunction.GetCanonicalPath(Multifunction.FixSlashes(Multifunction.TrimQuotes(args[inIndex].substring("-linkscript=".length()))).replace(",--script=", ""));
        }
        else if (token.startsWith("-compiler_folder="))
        {
          compilerFolder = Multifunction.GetCanonicalPath(Multifunction.FixSlashes(Multifunction.TrimQuotes(args[inIndex].substring("-compiler_folder=".length()))));
        }
        else if (token.startsWith("-java="))
        {
          javaPath = Multifunction.GetCanonicalPath(Multifunction.TrimQuotes(args[inIndex].substring("-java=".length())));
        }
        else if (token.startsWith("-path_to_ide_bin="))
        {
          pathToIDEBin = Multifunction.GetCanonicalPath(Multifunction.TrimQuotes(args[inIndex].substring("-path_to_ide_bin=".length())));
        }
        else if (token.startsWith("-last_page_erasable"))
        {
          allowLastPageErase = true;
        }
        else if (token.startsWith("-nolast_page_erasable"))
        {
          allowLastPageErase = false;
        }
        else if (token.startsWith("-first_page_erasable"))
        {
          allowFirstPageErase = true;
        }
        else if (token.startsWith("-version"))
        {
          System.out.printf("%s %s v%s\n", new Object[] { EZBLState.class.getPackage().getSpecificationVendor(), EZBLState.class.getPackage().getImplementationTitle(), EZBLState.class.getPackage().getImplementationVersion() });
        }
        else if (token.startsWith("-verbose"))
        {
          verbose = true;
        }
        else if (token.startsWith("-silent"))
        {
          silent = true;
        }
        else if (token.startsWith("-com="))
        {
          comPort = Multifunction.TrimQuotes(args[inIndex].substring("-com=".length()));
          if (comPort.toLowerCase().startsWith("i2c")) {
            hardwareFlowControl = true;
          }
        } else if (token.startsWith("-baud="))
        {
          baudRate = Multifunction.TrimQuotes(args[inIndex].substring("-baud=".length()));
        }
        else if (token.startsWith("-slave_address="))
        {
          slaveAddress = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-slave_address=".length()))).intValue();
        }
        else if (token.startsWith("-i2c_address="))
        {
          slaveAddress = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-i2c_address=".length()))).intValue();
        }
        else if (token.startsWith("-auto_baud_retries="))
        {
          autoBaudRetryLimit = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-auto_baud_retries=".length()))).intValue();
        }
        else if (token.startsWith("-command_prefix="))
        {
          commandPrefix = Multifunction.TrimQuotes(args[inIndex].substring("-command_prefix=".length()));
        }
        else if (token.startsWith("-skip_handshake"))
        {
          skipComHandshake = true;
        }
        else if (token.startsWith("-min_free_psv="))
        {
          minFreePSVSpace = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-min_free_psv=".length()))).intValue();
        }
        else if (token.startsWith("-warn_free_psv="))
        {
          warnFreePSVSpace = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-warn_free_psv=".length()))).intValue();
        }
        else if (token.startsWith("-align="))
        {
          hexAlign = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-align=".length()))).intValue();
        }
        else if (token.startsWith("-"))
        {
          if (!undecodedOptions.contains(newOption)) {
            EZBLState tmp2019_2017 = s;20192017undecodedOptions = (20192017undecodedOptions + " " + newOption + " ");
          }
        }
        else {
          System.err.println("    EZBL: Command token \"" + args[inIndex] + "\" unrecognized. Ignoring.");
        }
      }
    }
    
    if (ezblStateSavePath == null) {
      ezblStateSavePath = GetStateFilePath(args);
    }
    if ((!silent) && (System.getProperty("java.class.path", "ezbl_tools.jar").contains("ezbl_tools\\build\\classes"))) {
      verbose = true;
    }
    
    if ((makefilePath == null) && (conf != null)) {
      makefilePath = ("nbproject" + File.separator + "Makefile-" + conf + ".mk");
    }
    

    if ((partNumber != null) && (fullPartNumber != null))
    {
      if ((partNumber.startsWith("30")) || (partNumber.toUpperCase().startsWith("33F")) || (partNumber.toUpperCase().startsWith("24H")) || (partNumber.toUpperCase().startsWith("24F"))) {
        coreType = EZBLState.CPUClass.f;
      } else if ((partNumber.toUpperCase().startsWith("33E")) || (partNumber.toUpperCase().startsWith("24E"))) {
        coreType = EZBLState.CPUClass.e;
      } else if (partNumber.toUpperCase().startsWith("33C")) {
        coreType = EZBLState.CPUClass.c;
      } else if (partNumber.toUpperCase().startsWith("33B")) {
        coreType = EZBLState.CPUClass.b;
      } else if (partNumber.toUpperCase().startsWith("33A")) {
        coreType = EZBLState.CPUClass.a;
      } else if (partNumber.toUpperCase().startsWith("32MM"))
        coreType = EZBLState.CPUClass.mm;
      if (coreType != EZBLState.CPUClass.other) {
        MCU16Mode = ((coreType == EZBLState.CPUClass.f ? 1 : 0) | (coreType == EZBLState.CPUClass.e ? 1 : 0) | (coreType == EZBLState.CPUClass.c ? 1 : 0) | (coreType == EZBLState.CPUClass.b ? 1 : 0));
      }
      if ((compilerFolder == null) || (compilerFolder == null) || (!compilerFolder.equals(compilerFolder)))
      {
        compilerLinkScriptPath = null;
        compilerGLDContents = null;
      }
      

      if (MCU16Mode)
      {
        if (compilerLinkScriptPath == null)
        {

          File f = new File("p" + partNumber + ".gld");
          if (f.exists()) {
            compilerLinkScriptPath = f.getName();
          }
          else {
            f = new File(EZBLState.ezblToolsExecPath() + "/p" + partNumber + ".gld");
            if (f.exists()) {
              compilerLinkScriptPath = Multifunction.GetCanonicalPath(f.getPath());
            }
          }
          if ((compilerFolder != null) && (compilerLinkScriptPath == null))
          {
            String familyPrefix = "PIC24F";
            try
            {
              familyPrefix = fullPartNumber.startsWith("ds") ? fullPartNumber.substring(0, 8) : fullPartNumber.substring(0, 6);
            }
            catch (IndexOutOfBoundsException localIndexOutOfBoundsException) {}
            

            compilerLinkScriptPath = Multifunction.FixSlashes(compilerFolder + "/../support/" + familyPrefix + "/gld/p" + partNumber + ".gld");
            if (!new File(compilerLinkScriptPath).exists())
            {
              compilerLinkScriptPath = null;
            }
          }
          if ((compilerFolder != null) && (compilerLinkScriptPath == null))
          {
            List<File> gldMatches = Multifunction.FindFiles(Multifunction.FixSlashes(compilerFolder + "/../support/"), "p" + partNumber + ".gld", true);
            if ((gldMatches != null) && (gldMatches.size() >= 1))
              compilerLinkScriptPath = Multifunction.GetCanonicalPath(((File)gldMatches.get(0)).getPath());
          }
          if ((compilerFolder != null) && (compilerLinkScriptPath == null))
          {
            List<File> gldMatches = Multifunction.FindFilesRegEx(Multifunction.FixSlashes(compilerFolder + "/../"), "." + partNumber + "\\.[gG][lL][dD]", true);
            if ((gldMatches != null) && (gldMatches.size() >= 1))
              compilerLinkScriptPath = Multifunction.GetCanonicalPath(((File)gldMatches.get(0)).getPath());
          }
          if ((compilerFolder != null) && (compilerLinkScriptPath == null))
          {
            System.err.println("EZBL: could not find .gld linker script for '" + raw_mcpu + "'. Ensure '-compiler_folder=' option is set to point to a valid XC16/XC32 bin folder and a valid target processor for the compiler is selected, this device is supported by EZBL and ezbl_tools.jar make/command parameters are correct.");
          }
        }
      }
    }
    


    if (temporariesPath == null)
    {
      temporariesPath = CommandAndBuildState.class.getProtectionDomain().getCodeSource().getLocation().getPath();
      try
      {
        temporariesPath = URLDecoder.decode(temporariesPath, "UTF-8");
      }
      catch (UnsupportedEncodingException ex)
      {
        Logger.getLogger(CommandAndBuildState.class.getName()).log(Level.SEVERE, null, ex);
      }
      temporariesPath = new File(temporariesPath).getParent();
      

      if (temporariesPath == null)
      {
        temporariesPath = "";
      }
    }
    


    if (!temporariesPath.isEmpty())
    {

      if (!temporariesPath.substring(temporariesPath.length() - 1).equals(File.separator))
      {
        temporariesPath += File.separator;
      }
    }
    

    if (((fullPartNumber != null) && (fullPartNumber.matches("[^S]*?S[0-9]$"))) || (coreType == EZBLState.CPUClass.mm)) {
      remapISRThroughIGT = false;
    }
    s.LoadEDCData();
    

    if (encryptionPassword != null)
    {




      encryptionKey = new byte[16];
      
      byte[] passBytes;
      
      try
      {
        passBytes = encryptionPassword.getBytes("UTF-8");
      }
      catch (UnsupportedEncodingException ex) {
        byte[] passBytes;
        passBytes = encryptionPassword.getBytes();
      }
      int i = 0;
      while (i < encryptionKey.length)
      {
        int chunkSize = encryptionKey.length - i;
        if (chunkSize > passBytes.length)
        {
          chunkSize = passBytes.length;
        }
        System.arraycopy(passBytes, 0, encryptionKey, i, chunkSize);
        i += chunkSize;
      }
      

      if (encryptionSaltString == null)
        encryptionSalt = defaultEncryptionSalt;
      for (i = 0; i < encryptionKey.length; i++)
      {
        int tmp3466_3464 = i; byte[] tmp3466_3461 = encryptionKey;tmp3466_3461[tmp3466_3464] = ((byte)(tmp3466_3461[tmp3466_3464] ^ encryptionSalt[i]));
      }
    }
    
    parseOkay = true;
    return s;
  }
  
  public static String GetStateFilePath(String[] args)
  {
    String ret = "ezbl_integration" + File.separator + "ezbl_build_state.bin";
    
    if (args != null)
    {
      for (int inIndex = 0; inIndex < args.length; inIndex++)
      {

        if (args[inIndex].startsWith("--"))
        {
          args[inIndex] = args[inIndex].substring(1);
        }
        String token = args[inIndex].toLowerCase();
        
        if (token.startsWith("-state="))
        {
          return Multifunction.TrimQuotes(args[inIndex].substring("-state=".length()));
        }
        if (token.startsWith("-artifact="))
        {
          String artifact = Multifunction.TrimQuotes(args[inIndex].substring("-artifact=".length()));
          File f = new File(artifact);
          String parentFolder = f.getParent();
          if (parentFolder != null)
          {
            ret = parentFolder + File.separator + "ezbl_build_state.bin";
          }
        }
      }
    }
    
    return ret;
  }
}
