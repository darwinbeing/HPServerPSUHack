package com.microchip.apps.ezbl;

import java.io.File;
import java.io.PrintStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;






































public class MakeEditor
{
  public MakeEditor() {}
  
  public static void main(String[] args)
  {
    boolean keepGoing = false;
    for (int i = 0; i < args.length; i++)
    {
      String arg = args[i];
      String token = arg.trim().toLowerCase();
      if (token.startsWith("-updateid="))
      {
        IncIDAndBuildNumber(arg.trim().substring("-updateid=".length()));
        args[i] = "";

      }
      else if (token.startsWith("-conf="))
      {
        keepGoing = true;
      }
      else if (token.startsWith("-makefile="))
      {
        keepGoing = true;
      }
    }
    if (!keepGoing)
    {
      return;
    }
    

    String stateSavePath = CommandAndBuildState.GetStateFilePath(args);
    EZBLState state = EZBLState.ReadFromFile(stateSavePath);
    if ((argsFromMakeEditor == null) || (!Arrays.equals(argsFromMakeEditor, args)))
    {
      state = CommandAndBuildState.ReadArgs(state, args);
    }
    

    if (!parseOkay)
    {
      System.out.print("\r\n" + Multifunction.FormatHelpText(79, 0, "Usage:") + Multifunction.FormatHelpText(79, 3, "java -jar ezbl_tools.jar --make_editor -makefile=\"nbproject/Makefile-${CONF}.mk\" [options]") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Inputs:") + Multifunction.FormatHelpText(79, 6, "-conf=${CONF}") + Multifunction.FormatHelpText(79, 9, "or") + Multifunction.FormatHelpText(79, 6, "-makefile=\"nbproject/Makefile-${CONF}.mk\"") + Multifunction.FormatHelpText(79, 9, "MPLAB X current project configuration Makefile (not the user Makefile) to modify for EZBL linking/merging.") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Outputs:") + Multifunction.FormatHelpText(79, 6, "Project Makefile-${CONF}.mk is edited to invoke the ezbl_tools.jar at appropriate times during the build process so that EZBL can be linked with the application and contain all needed project/device specific data.") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Options:") + Multifunction.FormatHelpText(79, 6, "-save-temps") + Multifunction.FormatHelpText(79, 9, "Option to keep any intermediate files generating by ezbl_tools.jar during the make process.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "-temp-folder=\"temporaries_path\"") + Multifunction.FormatHelpText(79, 9, "Path to write intermediate output files. This option is ignored if -save-temps is unspecified. If this option is unspecified, the same folder that this .jar file is in will be used.") + "\r\n");
      




















      return;
    }
    
    if (makefilePath == null)
    {
      System.err.println("ezbl_tools: Missing required -conf=${CONF} or -makefile=\"nbproject/Makefile-${CONF}.mk\" parameter.");
      System.exit(-1);
    }
    

    elfPath = (temporariesPath + "ezbl_partial_link.elf");
    

    String expandedPassThroughOptions = undecodedOptions;
    if (!expandedPassThroughOptions.contains("-conf="))
    {
      expandedPassThroughOptions = expandedPassThroughOptions + " -conf=$(CONF)";
    }
    if (!expandedPassThroughOptions.contains("-mcpu="))
    {
      expandedPassThroughOptions = expandedPassThroughOptions + " -mcpu=$(MP_PROCESSOR_OPTION)";
    }
    if (!expandedPassThroughOptions.contains("-artifact="))
    {
      expandedPassThroughOptions = expandedPassThroughOptions + " -artifact=\"$(FINAL_IMAGE)\"";
    }
    if (!expandedPassThroughOptions.contains("-linkscript="))
    {
      expandedPassThroughOptions = expandedPassThroughOptions + " -linkscript=$(MP_LINKER_FILE_OPTION)";
    }
    if (!expandedPassThroughOptions.contains("-compiler_folder="))
    {
      expandedPassThroughOptions = expandedPassThroughOptions + " -compiler_folder=$(MP_CC_DIR)";
    }
    if (!expandedPassThroughOptions.contains("-java"))
    {
      expandedPassThroughOptions = expandedPassThroughOptions + " -java=$(MP_JAVA_PATH)";
    }
    if (!expandedPassThroughOptions.contains("-project_name="))
    {
      expandedPassThroughOptions = expandedPassThroughOptions + " -project_name=\"$(PROJECTNAME)\"";
    }
    if (!expandedPassThroughOptions.contains("-state="))
    {
      expandedPassThroughOptions = expandedPassThroughOptions + " -state=\"dist" + File.separator + "$(CND_CONF)" + File.separator + "$(IMAGE_TYPE)" + File.separator + "ezbl_build_state.bin\"";
    }
    

    String makeContents = Multifunction.ReadFile(makefilePath, true);
    if (makeContents == null)
    {
      System.err.println("ezbl_tools: Unable to read \"" + makefilePath + "\"");
      System.exit(-2);
    }
    

    makeContents = makeContents.replaceAll("(?sm)^###\\<EZBL MODIFIED 1\\>###\n.*?###\\</EZBL MODIFIED 1\\>###\n", "");
    makeContents = makeContents.replaceAll("(?sm)^###\\<EZBL MODIFIED 0\\>###\n.*?###\\</EZBL MODIFIED 0\\>###\n", "");
    

    TextBlock makeData = new TextBlock(makeContents);
    boolean found = false;
    makeData.ResetFind();
    while (makeData.Find("(?m)^[\t][^\\$]*[\\$]([{]|[(])MP_CC([}]|[)]).*?-Wl[^\n]*", "\n"))
    {
      found = true;
      String linkCommand = makeData.GetInner(false);
      linkCommand = linkCommand.substring(0, linkCommand.length() - 1);
      linkCommand = linkCommand.replaceAll("[.][$]([{]|[(])OUTPUT_SUFFIX([}]|[)])", Matcher.quoteReplacement(".$(DEBUGGABLE_SUFFIX)"));
      

      boolean partitionOptionFound = linkCommand.contains("--partition");
      if (dualPartitionMode != partitionOptionFound)
      {

        edcInfoLoaded = false;
        dualPartitionMode = partitionOptionFound;
        if (linkCommand.contains("--partition,1")) {
          targetPartition = 1;
        } else if (linkCommand.contains("--partition,2"))
          targetPartition = 2;
      }
      if (!partitionOptionFound) {
        targetPartition = 0;
      }
      TextBlock commandParser = new TextBlock(linkCommand);
      if (!commandParser.Find("-o[\\s]+[\\\"]", "[\\\"][\\s]"))
      {
        commandParser.ResetFind();
        commandParser.Find("-o[\\s]+", "[\\s]");
      }
      String originalLinkOutputFile = commandParser.GetInner(true);
      
      saveTemps |= linkCommand.toLowerCase().contains("-save-temps");
      if (!linkCommand.contains("-D__$(MP_PROCESSOR_OPTION)__=1"))
        linkCommand = linkCommand.replaceFirst("-Wl", Matcher.quoteReplacement("-Wl,-D__$(MP_PROCESSOR_OPTION)__=1"));
      if (!linkCommand.contains("-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT"))
        linkCommand = linkCommand.replaceFirst("-Wl", Matcher.quoteReplacement("-Wl,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT"));
      if (!linkCommand.contains("--defsym=EZBL_BOOT_PROJECT=1")) {
        linkCommand = linkCommand.replaceFirst("-Wl", Matcher.quoteReplacement("-Wl,--defsym=EZBL_BOOT_PROJECT=1"));
      }
      String firstLinkCommand = linkCommand.replaceAll("[\\s]+-o[\\s]+[\\S]+[\\s]", Matcher.quoteReplacement(new StringBuilder().append(" -o \"").append(elfPath).append("\" ").toString())).replaceAll("--check-sections", Matcher.quoteReplacement("--no-check-sections")) + " 1>" + EZBLState.nullFile();
      if (!firstLinkCommand.contains("--no-check-sections"))
        firstLinkCommand = firstLinkCommand.replaceFirst("-Wl", Matcher.quoteReplacement("-Wl,--no-check-sections"));
      if (!firstLinkCommand.contains("-DEZBL_PASS_1_LINK"))
        firstLinkCommand = firstLinkCommand.replaceFirst("-Wl", Matcher.quoteReplacement("-Wl,-DEZBL_PASS_1_LINK"));
      String dump1Command = javaLaunchString + " --dump_parser -pass=1 -elf_artifact=\"" + elfPath + "\" " + expandedPassThroughOptions;
      String dump2Command = javaLaunchString + " --dump_parser -pass=2 -elf_artifact=\"" + originalLinkOutputFile + "\" " + expandedPassThroughOptions;
      
      dump2Command = "@echo " + dump2Command + "\n\t" + "@echo " + linkCommand + " | " + dump2Command;
      
      String genMergeCommand = javaLaunchString + " --blobber -generate_merge " + expandedPassThroughOptions.replace("-artifact=\"$(FINAL_IMAGE)\"", new StringBuilder().append("-artifact=\"").append(originalLinkOutputFile).append("\"").toString());
      
      String newLink1Command = "###<EZBL MODIFIED 0>###\n";
      if (MCU16Mode)
      {
        newLink1Command = newLink1Command + "\t@echo EZBL: Updating automatic sections in linker script\n\t" + javaLaunchString + " --gldbuilder " + expandedPassThroughOptions + "\n";
      }
      
      newLink1Command = newLink1Command + "\t@echo EZBL: Starting linking pass 1 of 2\n" + firstLinkCommand + "\n";
      
      List<String> newCommands = new ArrayList();
      newCommands.add(newLink1Command);
      newCommands.add("\t" + dump1Command + "\n");
      newCommands.add("\t@echo EZBL: Starting linking pass 2 of 2\n");
      newCommands.add("###</EZBL MODIFIED 0>###\n");
      newCommands.add(linkCommand + "\n");
      newCommands.add("###<EZBL MODIFIED 0>###\n");
      newCommands.add("#\tRun the ezbl_tools.jar dump processor again so we can test if the Bootloader geometry changed\n");
      newCommands.add("\t" + dump2Command + "\n");
      newCommands.add("\t" + genMergeCommand + "\n");
      if (!saveTemps)
        newCommands.add("\t@test -e \"" + elfPath + "\" && rm \"" + elfPath + "\" || true\n");
      newCommands.add("###</EZBL MODIFIED 0>###\n");
      String str = Multifunction.CatStringList(newCommands);
      
      makeData.ReplaceOuter(str);
    }
    if (!found)
    {
      System.err.println("ezbl_tools: Could not find linking command line location(s) in " + makefilePath + ". This EZBL tool version will not work properly with this MPLAB X IDE version.");
      System.exit(-3);
    }
    


    List<String> newCommands = new ArrayList();
    newCommands.add("###<EZBL MODIFIED 1>###\n");
    newCommands.add("\t" + javaLaunchString + " --blobber -generate_merge " + expandedPassThroughOptions.replaceFirst("[$][{(]FINAL_IMAGE[})]", Matcher.quoteReplacement("$(subst .hex,.elf,$(FINAL_IMAGE))")) + "\n");
    if (!saveTemps)
      newCommands.add("\t@test -e \"" + elfPath + "\" && rm \"" + elfPath + "\" || true\n");
    newCommands.add("###</EZBL MODIFIED 1>###\n");
    String str = Multifunction.CatStringList(newCommands);
    makeData.ResetFind();
    while (makeData.Find("(?m)^[\t][^\n]*?hexmate[\\s\".]*", "-[oO][\"]?", "[\\s\"]", "\n"))
    {
      makeData.ReplaceOuter(makeData.GetOuter() + str);
    }
    

    String newMakeContents = makeData.GetFullBlock();
    int writeResult = Multifunction.UpdateFileIfDataDifferent(makefilePath, newMakeContents, false);
    if (writeResult < 0)
    {
      System.err.println("ezbl_tools: failed to write to \"" + makefilePath + "\"");
      System.exit(-4);
    }
    

    pass = 0;
    state.SaveToFile();
  }
  

  static void IncIDAndBuildNumber(String ezblProductIDFilePath)
  {
    List<String> findStrings = new ArrayList();
    
    boolean errorReturn = false;
    String fileContents = Multifunction.ReadFile(ezblProductIDFilePath, true);
    
    if (fileContents == null)
    {
      System.err.printf("ezbl_tools: could not read '%1$s' for updating BOOTID_HASH/APPID_VER_BUILD data\n", new Object[] { ezblProductIDFilePath });
      System.exit(-1);
    }
    



    try
    {
      MessageDigest hashComputer = MessageDigest.getInstance("SHA-256");
      
      findStrings.add("VENDOR");
      findStrings.add("MODEL");
      findStrings.add("NAME");
      findStrings.add("OTHER");
      
      for (String fstr : findStrings)
      {
        Matcher m = Pattern.compile("^ *?BOOTID_" + fstr + "[ ]*?=[\\s]*?", 9).matcher(fileContents);
        
        if (m.find())
        {
          int fieldStartIndex = m.end() + 1;
          int fieldEndIndex = fileContents.indexOf('\n', fieldStartIndex + 1);
          String fieldContents = fileContents.substring(fieldStartIndex, fieldEndIndex);
          int firstQuote = fieldContents.indexOf('"');
          if (firstQuote >= 0)
          {
            int secondQuote = fieldContents.indexOf('"', firstQuote + 1);
            if (secondQuote >= 0)
            {
              fieldContents = fieldContents.substring(firstQuote, secondQuote + 1);
            }
          }
          
          hashComputer.update(fieldContents.getBytes());
        }
      }
      
      byte[] computedHash = hashComputer.digest();
      String BOOTID_HASH = String.format("%1$02X%2$02X%3$02X%4$02X%5$02X%6$02X%7$02X%8$02X%9$02X%10$02X%11$02X%12$02X%13$02X%14$02X%15$02X%16$02X%17$02X%18$02X%19$02X%20$02X", new Object[] { Byte.valueOf(computedHash[0]), Byte.valueOf(computedHash[1]), Byte.valueOf(computedHash[2]), Byte.valueOf(computedHash[3]), Byte.valueOf(computedHash[4]), Byte.valueOf(computedHash[5]), Byte.valueOf(computedHash[6]), Byte.valueOf(computedHash[7]), Byte.valueOf(computedHash[8]), Byte.valueOf(computedHash[9]), Byte.valueOf(computedHash[10]), Byte.valueOf(computedHash[11]), Byte.valueOf(computedHash[12]), Byte.valueOf(computedHash[13]), Byte.valueOf(computedHash[14]), Byte.valueOf(computedHash[15]), Byte.valueOf(computedHash[16]), Byte.valueOf(computedHash[17]), Byte.valueOf(computedHash[18]), Byte.valueOf(computedHash[19]) });
      



      for (int i = 20; i < computedHash.length; i += 4)
      {
        BOOTID_HASH = BOOTID_HASH + String.format("%1$02X%2$02X%3$02X%4$02X", new Object[] { Byte.valueOf(computedHash[i]), Byte.valueOf(computedHash[(i + 1)]), Byte.valueOf(computedHash[(i + 2)]), Byte.valueOf(computedHash[(i + 3)]) });
      }
      
      fileContents = fileContents.replaceFirst("(?mu)^[ ]*?BOOTID_HASH[ ]*?=.*?$", "BOOTID_HASH  = " + BOOTID_HASH);
      int j = BOOTID_HASH.length();
      for (int i = 0; i < computedHash.length; j -= 8)
      {
        String index = String.valueOf(i / 4);
        fileContents = fileContents.replaceFirst("(?mu)^[ ]*?BOOTID_HASH" + index + "[ ]*?=.*?$", "BOOTID_HASH" + index + " = 0x" + BOOTID_HASH.substring(j - 8, j));i += 4;
      }
      
    }
    catch (NoSuchAlgorithmException ex)
    {
      System.err.println("Missing SHA-256 java hashing library. BOOTID not computed/updated.");
      errorReturn = true;
    }
    

    int increment = 0;
    long lastBuild = 0L;
    int buildIndex = -1;int buildEndIndex = -1;
    

    Matcher m = Pattern.compile("^ *?APPID_VER_BUILD_INC[ ]*?=[ ]*?", 9).matcher(fileContents);
    if (m.find())
    {
      int fieldStartIndex = m.end() + 1;
      int fieldEndIndex = fileContents.indexOf('\n', fieldStartIndex + 1);
      String fieldContents = fileContents.substring(fieldStartIndex, fieldEndIndex);
      increment = Integer.decode(fieldContents).intValue();
      

      m = Pattern.compile("^ *?APPID_VER_BUILD[ ]*?=[ ]*?", 9).matcher(fileContents);
      if (m.find())
      {
        buildIndex = m.end() + 1;
        buildEndIndex = fileContents.indexOf('\n', buildIndex + 1);
        fieldContents = fileContents.substring(buildIndex, buildEndIndex);
        lastBuild = Long.decode(fieldContents).longValue();
        

        fileContents = fileContents.substring(0, buildIndex) + String.valueOf(lastBuild + increment) + fileContents.substring(buildEndIndex);
      }
    }
    

    System.exit(Multifunction.UpdateFileIfDataDifferent(ezblProductIDFilePath, fileContents, false));
    
    if (errorReturn)
    {
      System.exit(-1);
    }
  }
}
