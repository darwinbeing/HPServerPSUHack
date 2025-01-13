package com.microchip.apps.ezbl;

import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;

































public class Main
{
  public Main() {}
  
  public static ClassLoader LoadRuntimeJar(String filename)
  {
    if (!Multifunction.FileExists(filename)) {
      return null;
    }
    try
    {
      Method method = URLClassLoader.class.getDeclaredMethod("addURL", new Class[] { URL.class });
      


      method.setAccessible(true);
      return (ClassLoader)ClassLoader.class.cast(method.invoke(ClassLoader.getSystemClassLoader(), new Object[] { new File(filename).getCanonicalFile().toURI().toURL() }));


    }
    catch (IllegalAccessException|IllegalArgumentException|NoSuchMethodException|SecurityException|InvocationTargetException|MalformedURLException ex)
    {

      Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
      System.err.printf("ezbl_tools: could not load '%1$s': %2$s\n", new Object[] { filename, ex.getMessage() });
      return null;
    }
    catch (IOException ex)
    {
      System.err.printf("ezbl_tools: could not load '%1$s': %2$s\n", new Object[] { filename, ex.getMessage() });
      Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex); }
    return null;
  }
  

  public static void main(String[] args)
  {
    for (int inIndex = 0; inIndex < args.length; inIndex++)
    {

      if (args[inIndex].startsWith("--"))
      {
        args[inIndex] = args[inIndex].substring(1);
      }
      String token = args[inIndex].toLowerCase();
      
      String[] newArgs = (String[])Arrays.copyOfRange(args, 1, args.length);
      
      if (token.startsWith("-blobber"))
      {
        Blobber.main(newArgs);
        return;
      }
      if (token.startsWith("-gldbuilder"))
      {
        GLDBuilder.main(newArgs);
        return;
      }
      if (token.startsWith("-communicator"))
      {
        Communicator.main(newArgs);
        return;
      }
      if (token.startsWith("-make_editor"))
      {
        MakeEditor.main(newArgs);
        return;
      }
      if (token.startsWith("-dump_parser"))
      {
        DumpParser.main(newArgs);
        return;
      }
      if (token.startsWith("-objdump_filter"))
      {
        ObjDumpFilter.main(newArgs);
        return;
      }
      if (token.startsWith("-analyze"))
      {
        Bl2b artifact = Bl2b.GetBl2bFromFile(newArgs[0]);
        String report = PCTraceLogger.StaticAnalysis(artifact);
        if (report == null)
        {
          System.err.print("Unable to read \"" + newArgs[0] + "\"\n");
          System.exit(-50);
        }
        System.out.print(report);
        return;
      }
      if (token.startsWith("-peripheralheadergenerator"))
      {
        String partNumber = "dsPIC33EP512MU810";
        String outputFilename = Multifunction.FixSlashes("../internal_files_do_not_distribute/pic_abstraction.h");
        for (String newArg : newArgs)
        {
          if (newArg.startsWith("-mcpu="))
          {
            partNumber = newArg.substring(6);
          }
          else if (newArg.startsWith("-output="))
          {
            outputFilename = newArg.substring(8);
          }
        }
        

        EDCReader.GenerateDifferentialSFRList("C:\\Work\\PIC Files\\vDefault", outputFilename);
        



        return;
      }
      if (token.startsWith("-makepartdb"))
      {
        System.exit(SQLitePartDatabase.main(args));
      }
      

      System.out.print("\r\n" + Multifunction.FormatHelpText(79, 0, "Usage:") + Multifunction.FormatHelpText(79, 3, "java -jar ezbl_tools.jar --engine [engine argument(s)]") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Inputs:") + Multifunction.FormatHelpText(79, 6, "Specify '--blobber' to invoke the .hex blobber tool.") + Multifunction.FormatHelpText(79, 6, "Specify '--gldbuilder' to invoke the gld linker script builder tool.") + Multifunction.FormatHelpText(79, 6, "Specify '--communicator' to invoke the communicator tool.") + Multifunction.FormatHelpText(79, 6, "Specify '--make_editor' to invoke the MPLAB X Makefile auto-editor tool.") + Multifunction.FormatHelpText(79, 6, "Specify '--dump_parser' to invoke the xc16-objdump parser and data extractor tool.") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Options:") + Multifunction.FormatHelpText(79, 6, "Options are arguments directly passed to the associated engine. Execute an engine with no extra arguments to get its command line help.") + "\r\n");
    }
  }
}
