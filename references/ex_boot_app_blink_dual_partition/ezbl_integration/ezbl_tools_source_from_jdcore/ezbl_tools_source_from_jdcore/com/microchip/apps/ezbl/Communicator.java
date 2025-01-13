package com.microchip.apps.ezbl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;





















public class Communicator
{
  ReadCOMThread readCOMThread = null;
  WriteCOMThread writeCOMThread = null;
  volatile boolean timeToClose = false;
  volatile Process comTool = null;
  volatile FileInputStream in = null;
  volatile FileOutputStream out = null;
  final ConcurrentLinkedQueue<byte[]> binaryAppToCOMData = new ConcurrentLinkedQueue();
  final ConcurrentLinkedQueue<byte[]> binaryCOMToAppData = new ConcurrentLinkedQueue();
  byte[] mainThreadRXResidual = null;
  EZBLState state = null;
  String comPort = null;
  int baudRate = 115200;
  String logFilePath = null;
  boolean keepLog = false;
  boolean skipHandshake = false;
  
  long milliTimeout;
  
  public static Integer doBootload(String ezblCommExecPath, String artifactFilePath, String comPort, int baudRate, int i2cSlaveAddr, long milliTimeout, String logFile)
    throws IOException
  {
    Integer returnCode = null;
    
    if (!new File(artifactFilePath).exists())
    {
      artifactFilePath = artifactFilePath + ".bl2";
      if (!new File(artifactFilePath).exists()) {
        return null;
      }
    }
    if (logFile == null)
    {
      File f = File.createTempFile("EZBLComLog", ".txt");
      String newTempFileName = f.getParent();
      f.delete();
      if (newTempFileName == null)
        newTempFileName = "";
      if (!newTempFileName.endsWith(File.separator))
        newTempFileName = newTempFileName + File.separator;
      newTempFileName = newTempFileName + "ezbl_comm_log.txt";
      f = new File(newTempFileName);
      logFile = f.getCanonicalPath();
    }
    
    if (ezblCommExecPath == null)
    {
      File file = new File(System.getProperty("java.class.path"));
      ezblCommExecPath = file.getParent();
      if (ezblCommExecPath == null)
        ezblCommExecPath = ".";
      ezblCommExecPath = ezblCommExecPath + File.separatorChar + "ezbl_comm.exe";
      if (!new File(ezblCommExecPath).exists())
      {
        if (ezblCommExecPath.contains(";"))
          ezblCommExecPath = "ezbl_integration" + File.separatorChar + "ezbl_comm.exe";
      }
    }
    if (!new File(ezblCommExecPath).exists())
    {
      if (logFile != null)
        Multifunction.UpdateFileIfDataDifferent(logFile, "Could not find ezbl_comm.exe needed for UART/I2C communications\n", true);
      return null;
    }
    
    ProcessBuilder comPB = new ProcessBuilder(new String[] { ezblCommExecPath, String.format("-artifact=%s", new Object[] { artifactFilePath }), String.format("-com=%s", new Object[] { comPort }), String.format("-baud=%s", new Object[] { Integer.valueOf(baudRate) }), String.format("-slave_address=0x%02X", new Object[] { Integer.valueOf(i2cSlaveAddr) }), String.format("-timeout=%d", new Object[] { Long.valueOf(milliTimeout) }), String.format("-log=%s", new Object[] { logFile }) });
    





    comPB.inheritIO();
    Process comProcess = comPB.start();
    
    long startTime = System.currentTimeMillis();
    while ((returnCode == null) && (startTime - System.currentTimeMillis() < milliTimeout))
    {
      try
      {
        returnCode = Integer.valueOf(comProcess.exitValue());
      }
      catch (IllegalThreadStateException ex)
      {
        try
        {
          Thread.sleep(10L);
        }
        catch (InterruptedException localInterruptedException) {}
      }
    }
    


    if (returnCode == null)
    {
      comProcess.destroy();
      returnCode = Integer.valueOf(35233);
    }
    
    System.err.println();
    return returnCode;
  }
  
  public static void main(String[] args)
  {
    COMResult ret = new COMResult();
    
    boolean errorReturn = true;
    boolean displayPostUploadFeedback = false;
    boolean handshakeDone = false;
    boolean legacyProtocol = false;
    
    String preCommands = null;
    String readArtifact = null;
    String writeArtifact = null;
    String readArtifact2 = null;
    String postCommands = null;
    String alternateArtifact = null;
    String comInPath = null;
    String comOutPath = null;
    int exitCode = 0;
    String[] artifactExts = { "", ".bl2", ".blob", ".hex" };
    




    EZBLState state = CommandAndBuildState.ReadArgs(null, args);
    

    if (args.length == 0)
    {
      System.out.print("\r\n" + Multifunction.FormatHelpText(79, 0, "Uploads a .hex/.bl2/.blob firmware file to an EZBL Bootloader or EZBL ICSP Programmer.") + "\r\n" + Multifunction.FormatHelpText(79, 0, "Usage:") + Multifunction.FormatHelpText(79, 3, "java -jar ezbl_tools.jar -communicator -artifact=\"app_firmware.bl2\" -com=COMx [-baud=x] [-i2c_address=x] [-timeout=x] [-options]") + "\r\n" + Multifunction.FormatHelpText(79, 3, "Inputs:") + Multifunction.FormatHelpText(79, 6, "-com=COMx") + Multifunction.FormatHelpText(79, 9, "Required parameter where, COMx, is the communications port or OS specific file path that the bootloader or EZBL ICSP programmer is connected to. In the case of Windows COM ports, the specified COM port is internally prefixed as necessary with \"\\\\.\\\" to ensure the target represents a communications port rather than a file on the local filesystem.") + "\r\n" + Multifunction.FormatHelpText(79, 9, "If targeting an MCP2221 device acting as an I2C master, specify -com=I2C. If there is more than one MCP2221 connected to your system, you can specify different instances of the MCP2221 by appending a number (from 0 to n) onto the I2C token. For example, if there are two MCP2221 devices attached to the system, try, -com=I2C0, and if that does not work, instead try, -com=I2C1.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "[-timeout=x]") + Multifunction.FormatHelpText(79, 9, "Optional parameter specifying the communications timeout to use. 'x' specifies how long to wait for a response before terminating, in milliseconds. For most operations, the internal timeout counter is reset anytime data is transmitted back to us, so the optimal timeout to specify depends primarily on the latency characteristics of the communications channel and target device. The default timeout when this parameter is not specified is 1000ms. When using a high latency communications link, such as a Bluetooth SPP channel, there can be appreciable connection initiation delay and retransmission latency for lost packets. A timeout of 10000 ms or higher may be needed for such hardware.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "[-baud=x]") + Multifunction.FormatHelpText(79, 9, "Where, x, is the communications port baud rate that communications should be attempted at. For UARTs, auto-baud will attempt to set the target Bootloader or EZBL ICSP programmer to match this chosen buad rate. For COM ports, this parameter is optional and will default to 115200 baud if omitted. For an MCP2221 I2C interface, the default I2C baud rate is 400kHz.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "[-i2c_address=x]") + Multifunction.FormatHelpText(79, 9, "When using I2C, this option is required. x is the 7-bit I2C target slave address to communicate with. For example, if the slave is at 0x08, you would specify, -slave_address=0x08.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "[-save_temps=com-log.txt]") + Multifunction.FormatHelpText(79, 9, "Optional parameter specifying that all communications data should be logged to a file called 'com-log.txt'.") + "\r\n" + Multifunction.FormatHelpText(79, 6, "[-artifact=app_firmware[.bl2|.blob|.hex]]") + Multifunction.FormatHelpText(79, 9, "Where, app_firmware[.bl2|.blob|.hex], is the file to send to the target hardware for programming. If a file extension is excluded, an attempt is made to auto-detect the most optimal available file extension by searching the parent folder for a file with an exact name match (i.e. no extension), a file with the \".bl2\" extension, a file with the \".blob\" extension, or a file with the \".hex\" extension. .hex files are converted internally to a .blob format before COM transmission.") + "\r\n" + "\r\n");
      



































      exitCode = -1;
      return;
    }
    
    try
    {
      doBootload(null, artifactPath, comPort, Integer.decode(baudRate).intValue(), slaveAddress, milliTimeout, saveTempsFile);
      System.exit(0);
    }
    catch (IOException ex)
    {
      System.exit(35232);
    }
    

    List<String> argTokens = new ArrayList();
    
    argTokens.addAll(Arrays.asList(args));
    for (String token : argTokens)
    {
      String lowerToken = token.toLowerCase();
      
      if (lowerToken.startsWith("-auto_baud_retries="))
      {
        legacyProtocol = true;
        autoBaudRetryLimit = Integer.decode(Multifunction.TrimQuotes(token.substring("-auto_baud_retries=".length()))).intValue();
      }
      else if (lowerToken.startsWith("-com_in="))
      {
        comInPath = Multifunction.TrimQuotes(token.substring("-com_in=".length()));
      }
      else if (lowerToken.startsWith("-com_out="))
      {
        comOutPath = Multifunction.TrimQuotes(token.substring("-com_out=".length()));
      }
      else if ((lowerToken.startsWith("-save-temps")) || (lowerToken.startsWith("-save_temps")) || (lowerToken.startsWith("-log")))
      {
        saveTemps = true;
        if ((lowerToken.startsWith("-save-temps=")) || (lowerToken.startsWith("-save_temps=")))
        {
          saveTempsFile = Multifunction.TrimQuotes(token.substring("-save_temps=".length()));
        }
        else if (lowerToken.startsWith("-log="))
        {
          saveTempsFile = Multifunction.TrimQuotes(token.substring("-log=".length()));
        }
      }
      else if (lowerToken.startsWith("-command_prefix="))
      {
        legacyProtocol = true;
        commandPrefix = Multifunction.TrimQuotes(token.substring("-command_prefix=".length()));
      }
      else if (lowerToken.startsWith("-skip_handshake"))
      {
        legacyProtocol = true;
        skipComHandshake = true;
      }
      else if (lowerToken.startsWith("-precommands="))
      {
        legacyProtocol = true;
        preCommands = Multifunction.TrimQuotes(token.substring("-precommands=".length()));
        if (preCommands.isEmpty())
        {
          preCommands = null;
        }
      }
      else if (lowerToken.startsWith("-read="))
      {
        legacyProtocol = true;
        readArtifact = Multifunction.TrimQuotes(token.substring("-read=".length()));
        if (readArtifact.isEmpty())
        {
          readArtifact = null;
        }
      }
      else if (lowerToken.startsWith("-artifact="))
      {
        writeArtifact = Multifunction.TrimQuotes(token.substring("-artifact=".length()));
      }
      else if (lowerToken.startsWith("-read2="))
      {
        legacyProtocol = true;
        readArtifact2 = Multifunction.TrimQuotes(token.substring("-read2=".length()));
        if (readArtifact2.isEmpty())
        {
          readArtifact2 = null;
        }
      }
      else if (lowerToken.startsWith("-postcommands="))
      {
        legacyProtocol = true;
        postCommands = Multifunction.TrimQuotes(token.substring("-postcommands=".length()));
        if (postCommands.isEmpty())
        {
          postCommands = null;
        }
      }
      else if (lowerToken.startsWith("-v1protocol"))
      {
        legacyProtocol = true;
      }
      else if (lowerToken.startsWith("-feedback"))
      {
        displayPostUploadFeedback = true;
      }
      else if (!lowerToken.startsWith("-"))
      {
        File f = new File(token);
        if (f.canRead())
        {
          alternateArtifact = token;
        }
      }
    }
    

    if ((writeArtifact != null) && (writeArtifact.isEmpty()))
    {
      writeArtifact = null;
    }
    if ((writeArtifact == null) && (alternateArtifact != null))
    {
      writeArtifact = alternateArtifact;
    }
    


    if ((writeArtifact != null) && (!writeArtifact.isEmpty()))
    {
      for (String ext : artifactExts)
      {
        File f = new File(writeArtifact + ext);
        if (f.canRead())
        {
          writeArtifact = writeArtifact + ext;
          break;
        }
      }
    }
    artifactPath = writeArtifact;
    
    if ((!legacyProtocol) && ((writeArtifact == null) || (writeArtifact.isEmpty())))
    {
      System.err.println("EZBL communicator error: missing required -artifact=[file.bl2|file.blob|file.hex] parameter");
      System.exit(-1);
      return;
    }
    

    System.setOut(System.err);
    

    if ((comPort != null) && (comPort.startsWith("COM")) && (comPort.length() >= 5))
    {
      comPort = ("\\\\.\\" + comPort);
    }
    if ((comInPath != null) && (comInPath.startsWith("COM")) && (comInPath.length() >= 5))
    {
      comInPath = "\\\\.\\" + comInPath;
    }
    if ((comOutPath != null) && (comOutPath.startsWith("COM")) && (comOutPath.length() >= 5))
    {
      comOutPath = "\\\\.\\" + comOutPath;
    }
    
    Communicator comPort = null;
    
    try
    {
      if (comPort == null)
      {
        exitCode = -2;
        throw new IOException("missing required -com=[target] parameter"); }
      int baud;
      int baud;
      if (baudRate != null) {
        baud = Integer.decode(baudRate).intValue(); } else { int baud;
        if (comPort.toLowerCase().contains("i2c")) {
          baud = 400000;
        } else
          baud = 115200; }
      comPort = new Communicator(null, null, comPort, baud, slaveAddress, saveTempsFile);
      skipHandshake = ((skipComHandshake) || (!legacyProtocol));
      keepLog = saveTemps;
      milliTimeout = milliTimeout;
      state = state;
      
      if (comInPath != null)
      {
        readCOMThread.fileIOPath = comInPath;
      }
      if (comOutPath != null)
      {
        writeCOMThread.fileIOPath = comOutPath;
      }
      long threadStartNanoTime = System.nanoTime();
      readCOMThread.startNanoTimeRef = threadStartNanoTime;
      writeCOMThread.startNanoTimeRef = threadStartNanoTime;
      readCOMThread.start();
      writeCOMThread.start();
      
      if (legacyProtocol)
      {
        handshakeDone = skipHandshake;
        if (!skipHandshake)
        {
          ret = comPort.HandshakeWithTarget();
          handshakeDone = !error;
          if (error)
          {
            exitCode = -3;
            throw new IOException(responseText);
          }
          System.out.println(responseText);
        }
        

        if (preCommands != null)
        {
          ret = comPort.SendCommand(preCommands, 0, 0L);
          if (error)
          {
            exitCode = -4;
            throw new IOException(responseText);
          }
        }
        

        if (undecodedOptions.contains("-precrc"))
        {
          ret = comPort.GetTargetCRC(0, handshakeDone | skipHandshake);
          if (error)
          {
            exitCode = -5;
            throw new IOException(responseText);
          }
          System.out.println("CRC32 returned from target: " + responseText);
        }
        
        if (readArtifact != null)
        {
          ret = comPort.ReadDeviceFlash(readArtifact, handshakeDone | skipHandshake);
          if (error)
          {
            exitCode = -6;
            throw new IOException(responseText);
          }
          handshakeDone = true;
        }
      }
      

      if (writeArtifact != null)
      {
        if (legacyProtocol)
        {
          ret = comPort.Bootload(artifactPath, handshakeDone | skipHandshake);
        }
        else
        {
          ret = comPort.BootloadBL2(artifactPath, true);
        }
        if (error)
        {
          exitCode = -7;
          throw new IOException(responseText);
        }
        handshakeDone = false;
      }
      

      if (legacyProtocol)
      {
        if (readArtifact2 != null)
        {
          ret = comPort.ReadDeviceFlash(readArtifact2, handshakeDone | skipHandshake);
          if (error)
          {
            exitCode = -8;
            throw new IOException(responseText);
          }
          handshakeDone = true;
        }
        

        if (postCommands != null)
        {
          ret = comPort.SendCommand(postCommands, 0, 0L);
          if (error)
          {
            exitCode = -9;
            throw new IOException(responseText);
          }
        }
        


        if (undecodedOptions.contains("-postcrc"))
        {
          ret = comPort.GetTargetCRC(0, handshakeDone | skipHandshake);
          if (error)
          {
            exitCode = -10;
            throw new IOException(responseText);
          }
          System.out.println("CRC32 returned from target: " + responseText);
        }
      }
    }
    catch (IOException ex)
    {
      errorReturn = !fileError;
      System.err.print("EZBL communications error: " + ex.getMessage());
      if (!ex.getMessage().endsWith("\n"))
      {
        System.err.println();
      }
      if (exitCode == 0)
      {
        exitCode = -100;
      }
      if ((comPort != null) && (logFilePath != null))
      {
        if (new File(logFilePath).canRead())
        {
          System.err.printf("%1$-13sLog saved to: %2$s\n", new Object[] { " ", logFilePath });
        }
      }
    }
    




    if ((comPort != null) && (displayPostUploadFeedback))
    {
      long startTime = System.currentTimeMillis();
      long lastNewLine = System.currentTimeMillis();
      while ((System.currentTimeMillis() - startTime < milliTimeout) && (System.currentTimeMillis() - lastNewLine < 100L))
      {
        ret = comPort.ReadResponse(1, 100L);
        if ((error) || (asyncClose)) {
          break;
        }
        

        if (responseBinary[0] != 0)
        {
          System.out.print(String.valueOf((char)responseBinary[0]));
        }
        if (responseBinary[0] == 10)
        {
          lastNewLine = System.currentTimeMillis();
        }
      }
    }
    
    if (comPort != null)
    {
      comPort.Close(errorReturn);
    }
  }
  
  private class ReadCOMThread
    extends Thread
  {
    String fileIOPath = "\\\\.\\pipe\\ezbl_pipe_in_from_com";
    long startNanoTimeRef;
    
    private ReadCOMThread() {}
    
    public void run() {
      long nanoTimeout = 1000000000L;
      long startTime = System.nanoTime();
      byte[] data = new byte['Ȁ'];
      





















      while ((!timeToClose) && (in == null))
      {
        try
        {
          in = new FileInputStream(fileIOPath);
        }
        catch (FileNotFoundException ex)
        {
          if (System.nanoTime() - startTime >= nanoTimeout)
          {
            timeToClose = true;
            return;
          }
          


          try
          {
            comTool.exitValue();
            

            timeToClose = true;
            return;
          }
          catch (IllegalThreadStateException localIllegalThreadStateException) {}
        }
        



        try
        {
          Thread.sleep(2L);
        }
        catch (InterruptedException localInterruptedException) {}
      }
      


      try
      {
        while (!timeToClose)
        {
          int len = in.available();
          if (len <= 0)
          {

            try
            {
              Thread.sleep(1L);









            }
            catch (InterruptedException localInterruptedException1) {}









          }
          else
          {









            if (len > data.length)
            {
              len = data.length;
            }
            len = in.read(data, 0, len);
            if (len < 0)
            {
              timeToClose = true;
              break;
            }
            
            synchronized (binaryCOMToAppData)
            {
              binaryCOMToAppData.add(Arrays.copyOf(data, len));
              binaryCOMToAppData.notifyAll();
            }
            
          }
        }
      }
      catch (IOException ex)
      {
        timeToClose = true;
      }
    }
  }
  
























  private class WriteCOMThread
    extends Thread
  {
    String fileIOPath = "\\\\.\\pipe\\ezbl_pipe_out_to_com";
    long startNanoTimeRef;
    
    private WriteCOMThread() {}
    
    public void run() {
      long nanoTimeout = 1000000000L;
      long startTime = System.nanoTime();
      


      while (out == null)
      {
        try
        {
          out = new FileOutputStream(fileIOPath);
        }
        catch (FileNotFoundException ex)
        {
          if (System.nanoTime() - startTime >= nanoTimeout)
          {
            timeToClose = true;
            return;
          }
          


          try
          {
            comTool.exitValue();
            

            timeToClose = true;
            return;
          }
          catch (IllegalThreadStateException localIllegalThreadStateException) {}
        }
        



        try
        {
          Thread.sleep(2L);
        }
        catch (InterruptedException localInterruptedException1) {}
        

        if (timeToClose)
        {
          return;
        }
      }
      




















      while (!timeToClose)
      {
        synchronized (binaryAppToCOMData)
        {
          if (binaryAppToCOMData.isEmpty())
          {
            try
            {
              binaryAppToCOMData.wait();
            }
            catch (InterruptedException ex) {}
            
            continue;
          }
        }
        

        try
        {
          byte[] writeData = (byte[])binaryAppToCOMData.poll();
          if (writeData != null)
          {


            out.write(writeData);
          }
        }
        catch (IOException ex)
        {
          timeToClose = true;
        }
      }
    }
  }
  
























  public static void PrintCOMPorts(PrintStream out)
  {
    try
    {
      List<String> comPorts = OSSpecific.EnumDosDevices();
      System.out.println("   QueryDosDevice() Windows API reports these COM ports:");
      Collections.sort(comPorts);
      for (int i = 0; i < comPorts.size(); i++)
      {
        String s = (String)comPorts.get(i);
        if (!s.contains("COM"))
        {
          comPorts.remove(i--);
        }
        else {
          out.print(Multifunction.FormatHelpText(79, 6, s));
        }
      }
    }
    catch (RuntimeException localRuntimeException) {}
  }
  

















  public static List<String> EnumCOMPorts()
  {
    List<String> comPorts = null;
    try
    {
      comPorts = OSSpecific.EnumDosDevices();
      if (comPorts == null)
      {
        return null;
      }
      Collections.sort(comPorts);
      for (int i = 0; i < comPorts.size(); i++)
      {
        String s = (String)comPorts.get(i);
        if (!s.contains("COM"))
        {
          comPorts.remove(i--);
        }
      }
    }
    catch (RuntimeException localRuntimeException) {}
    




    return comPorts;
  }
  
  public Communicator(String ezblCommExecPath, String artifactFilePath, String comPort, int baudRate, int i2cSlaveAddr, String logFile)
    throws IOException
  {
    this.comPort = comPort;
    this.baudRate = baudRate;
    
    logFilePath = logFile;
    if (logFilePath == null)
    {
      File f = File.createTempFile("EZBLComLog", ".txt");
      String newTempFileName = f.getParent();
      f.delete();
      if (newTempFileName == null)
      {
        newTempFileName = "";
      }
      if (!newTempFileName.endsWith(File.separator))
      {
        newTempFileName = newTempFileName + File.separator;
      }
      newTempFileName = newTempFileName + "ezbl_comm_log.txt";
      f = new File(newTempFileName);
      logFilePath = f.getCanonicalPath();
    }
    
    String execPath = ezblCommExecPath;
    if (ezblCommExecPath == null)
    {
      File file = new File(System.getProperty("java.class.path"));
      execPath = file.getParent();
      if (execPath == null)
      {
        execPath = ".";
      }
      execPath = execPath + File.separatorChar + "ezbl_comm.exe";
      if (!new File(execPath).exists())
      {
        if (execPath.contains(";"))
        {
          execPath = "ezbl_integration" + File.separatorChar + "ezbl_comm.exe";
        }
      }
    }
    if (!new File(execPath).exists())
    {
      Multifunction.UpdateFileIfDataDifferent(logFilePath, "Could not find ezbl_comm.exe needed for UART/I2C communications\n", true);
      throw new IOException("Could not find ezbl_comm.exe needed for UART/I2C communications");
    }
    
    String[] cmdArgs = new String[2];
    cmdArgs[0] = execPath;
    cmdArgs[1] = ((artifactFilePath != null ? "-artifact=" + artifactFilePath : "") + " -com=" + comPort + (baudRate != 0 ? " -baud=" + String.valueOf(baudRate) : "") + " -slave_address=" + String.valueOf(i2cSlaveAddr) + " -log=\"" + logFilePath + "\"");
    comTool = Runtime.getRuntime().exec(cmdArgs[0] + " " + cmdArgs[1]);
    comTool.getErrorStream().close();
    comTool.getInputStream().close();
    comTool.getOutputStream().close();
    
    readCOMThread = new ReadCOMThread(null);
    writeCOMThread = new WriteCOMThread(null);
    try
    {
      readCOMThread.setPriority(8);
      writeCOMThread.setPriority(8);
    }
    catch (Exception localException) {}
  }
  


  @Deprecated
  public Communicator(EZBLState state)
    throws IOException
  {
    this.state = state;
    comPort = comPort;
    
    if (baudRate != null) {
      baudRate = Integer.decode(baudRate).intValue();
    } else if (comPort.toLowerCase().contains("i2c")) {
      baudRate = 400000;
    } else {
      baudRate = 115200;
    }
    logFilePath = saveTempsFile;
    if (logFilePath == null)
    {
      File f = File.createTempFile("EZBLComLog", ".txt");
      String newTempFileName = f.getParent();
      f.delete();
      if (newTempFileName == null)
      {
        newTempFileName = "";
      }
      if (!newTempFileName.endsWith(File.separator))
      {
        newTempFileName = newTempFileName + File.separator;
      }
      newTempFileName = newTempFileName + "ezbl_comm_log.txt";
      f = new File(newTempFileName);
      logFilePath = f.getCanonicalPath();
    }
    
    File file = new File(System.getProperty("java.class.path"));
    String execPath = file.getParent();
    if (execPath == null)
    {
      execPath = ".";
    }
    execPath = execPath + File.separatorChar + "ezbl_comm.exe";
    if (execPath.contains(";"))
    {
      execPath = "ezbl_integration" + File.separatorChar + "ezbl_comm.exe";
    }
    
    String[] cmdArgs = new String[2];
    cmdArgs[0] = execPath;
    cmdArgs[1] = ("-artifact=" + artifactPath + " -com=" + comPort + (baudRate != null ? " -baud=" + baudRate : "") + " -slave_address=" + String.valueOf(slaveAddress) + " -log=\"" + logFilePath + "\"");
    comTool = Runtime.getRuntime().exec(cmdArgs[0] + " " + cmdArgs[1]);
    comTool.getErrorStream().close();
    comTool.getInputStream().close();
    comTool.getOutputStream().close();
    
    readCOMThread = new ReadCOMThread(null);
    writeCOMThread = new WriteCOMThread(null);
    try
    {
      readCOMThread.setPriority(8);
      writeCOMThread.setPriority(8);
    }
    catch (Exception localException) {}
  }
  


  public final void finalize()
    throws Throwable
  {
    Close(false);
    if ((!keepLog) && (logFilePath != null))
    {
      new File(logFilePath).deleteOnExit();
    }
    super.finalize();
  }
  










  public int Close(boolean printErrorCodes)
  {
    int ret = 0;
    
    timeToClose = true;
    
    if (in != null)
    {
      try
      {
        readCOMThread.interrupt();
        in.getChannel().close();
        in.close();
      }
      catch (IOException localIOException) {}
    }
    

    if (out != null)
    {
      try
      {
        writeCOMThread.interrupt();
        out.getChannel().close();
        out.close();
      }
      catch (IOException localIOException1) {}
    }
    

    if (comTool != null)
    {
      long startTime = System.currentTimeMillis();
      while (System.currentTimeMillis() - startTime < milliTimeout / 2L)
      {
        try
        {
          ret = comTool.exitValue();
          comTool = null;

        }
        catch (IllegalThreadStateException ex)
        {

          try
          {
            Thread.sleep(10L);
          }
          catch (InterruptedException localInterruptedException) {}
        }
      }
      


      if (comTool != null)
      {











        comTool.destroy();
        ret = 62405;
        comTool = null;
      }
      
      if ((printErrorCodes) && (ret != 0))
      {
        String errorReturnText = String.format("ezbl_comm.exe returned error code %1$d:\n", new Object[] { Integer.valueOf(ret) });
        switch (ret)
        {
        case -3: 
        case -1: 
          errorReturnText = errorReturnText + Multifunction.FormatHelpText(150, 4, "Prior instance of ezbl_comm.exe is already running.\n");
          break;
        case 1: 
          errorReturnText = errorReturnText + Multifunction.FormatHelpText(150, 4, String.format("Could not open \"%1$s\". Ensure the communications port exists and is not already in use by another application.\n", new Object[] { comPort.replace("\\\\.\\COM", "COM") }));
          break;
        case 2: 
          errorReturnText = errorReturnText + Multifunction.FormatHelpText(150, 4, String.format("Specified I2Cx port not found (check -com=\"%1$s\" parameter)\n", new Object[] { comPort }));
          break;
        case 3: 
        case 4: 
        case 5: 
          errorReturnText = errorReturnText + Multifunction.FormatHelpText(150, 4, "MCP2221DLL API error return\n");
          break;
        case -3131: 
          errorReturnText = Multifunction.FormatHelpText(150, 4, "ezbl_comm.exe did not close normally. The selected COM port may be invalid or requires a longer timeout.");
          break;
        }
        
        System.err.printf(errorReturnText, new Object[0]);
      }
    }
    in = null;
    out = null;
    try
    {
      readCOMThread.join(500L);
    }
    catch (InterruptedException localInterruptedException1) {}
    

    try
    {
      writeCOMThread.join(500L);
    }
    catch (InterruptedException localInterruptedException2) {}
    

    readCOMThread = null;
    writeCOMThread = null;
    binaryAppToCOMData.clear();
    binaryCOMToAppData.clear();
    return ret;
  }
  
  public COMResult SendCommand(String command, int responseBytesExpected, long milliTimeout)
  {
    return SendCommand(command, null, responseBytesExpected, milliTimeout);
  }
  
  public COMResult SendCommand(String command, String responseTerminator, long milliTimeout)
  {
    return SendCommand(command, responseTerminator, 0, milliTimeout);
  }
  
  public COMResult SendCommand(String command, String responseTerminator, int responseBytesExpected, long milliTimeout)
  {
    SendBinary((state.commandPrefix + command).getBytes());
    return ReadResponse(responseTerminator, responseBytesExpected, milliTimeout);
  }
  
  public void SendBinary(byte[] dataToTransmit)
  {
    synchronized (binaryAppToCOMData)
    {
      binaryAppToCOMData.add(dataToTransmit);
      binaryAppToCOMData.notifyAll();
    }
  }
  
  public void SendBinary(byte[] dataToTransmit, int startIndex, int length)
  {
    byte[] stageData = Arrays.copyOfRange(dataToTransmit, startIndex, startIndex + length);
    
    synchronized (binaryAppToCOMData)
    {
      binaryAppToCOMData.add(stageData);
      binaryAppToCOMData.notifyAll();
    }
  }
  
  public COMResult ReadLine(long milliTimeout)
  {
    return ReadResponse("\n", milliTimeout);
  }
  

















  public COMResult ReadResponse(int responseByteCount, long milliTimeout)
  {
    return ReadResponse(null, responseByteCount, milliTimeout);
  }
  






















  public COMResult ReadResponse(String responseTerminator, long milliTimeout)
  {
    return ReadResponse(responseTerminator, 0, milliTimeout);
  }
  
  public COMResult ReadResponse(String responseTerminator, int responseByteCount, long milliTimeout)
  {
    int writeIndex = 0;
    
    COMResult ret = new COMResult("", false, false, false);
    responseBinary = new byte[responseByteCount];
    long startTime = System.currentTimeMillis();
    

    if ((responseByteCount == 0) && (responseTerminator == null))
    {
      return ret;
    }
    


    if (mainThreadRXResidual != null)
    {
      if (responseTerminator != null)
      {
        responseText = new String(mainThreadRXResidual);
        

        int findIndex = responseText.indexOf(responseTerminator);
        if (findIndex >= 0)
        {

          findIndex += responseTerminator.length();
          if (findIndex != responseText.length())
          {

            mainThreadRXResidual = Arrays.copyOfRange(mainThreadRXResidual, findIndex, mainThreadRXResidual.length);
            responseText = responseText.substring(0, findIndex);
          }
          else
          {
            mainThreadRXResidual = null;
          }
          
          return ret;
        }
        mainThreadRXResidual = null;
      }
      else
      {
        int copyLen = mainThreadRXResidual.length;
        if (writeIndex + mainThreadRXResidual.length > responseByteCount)
        {
          copyLen = responseByteCount - writeIndex;
        }
        System.arraycopy(mainThreadRXResidual, 0, responseBinary, writeIndex, copyLen);
        if (copyLen != mainThreadRXResidual.length)
        {
          mainThreadRXResidual = Arrays.copyOfRange(mainThreadRXResidual, copyLen, mainThreadRXResidual.length);
        }
        else
        {
          mainThreadRXResidual = null;
        }
        writeIndex += copyLen;
        

        if (writeIndex == responseByteCount)
        {
          return ret;
        }
      }
    }
    


    for (;;)
    {
      boolean newData = false;
      

      if (binaryCOMToAppData.isEmpty())
      {
        timedOut = (System.currentTimeMillis() - startTime >= milliTimeout);
        if (timedOut) {
          break;
        }
      }
      


      if (timeToClose)
      {
        asyncClose = true;
        break;
      }
      

      while (!binaryCOMToAppData.isEmpty())
      {
        byte[] data = (byte[])binaryCOMToAppData.poll();
        newData = true;
        
        if (responseTerminator != null)
        {
          responseText += new String(data);
          

          int findIndex = responseText.indexOf(responseTerminator);
          if (findIndex >= 0)
          {

            findIndex += responseTerminator.length();
            if (findIndex != responseText.length())
            {

              mainThreadRXResidual = Arrays.copyOfRange(data, responseText.length() - findIndex, data.length);
              responseText = responseText.substring(0, findIndex);
            }
            
            return ret;
          }
        }
        else
        {
          int copyLen = data.length;
          if (writeIndex + data.length > responseByteCount)
          {
            copyLen = responseByteCount - writeIndex;
          }
          System.arraycopy(data, 0, responseBinary, writeIndex, copyLen);
          if (copyLen != data.length)
          {
            mainThreadRXResidual = Arrays.copyOfRange(data, copyLen, data.length);
          }
          writeIndex += copyLen;
          

          if (writeIndex == responseByteCount)
          {
            return ret;
          }
        }
      }
      
      if (!newData)
      {

        try
        {


          synchronized (binaryCOMToAppData)
          {
            if (binaryCOMToAppData.isEmpty())
            {
              binaryCOMToAppData.wait(milliTimeout);
            }
          }
        }
        catch (InterruptedException localInterruptedException) {}
      }
    }
    



    if (responseByteCount != writeIndex)
    {
      responseBinary = Arrays.copyOf(responseBinary, writeIndex);
    }
    error = true;
    return ret;
  }
  


  public static String FormatBinaryHelp(byte[] binaryData, int indent, int bytesPerLine, int bytesPerWordGroup, int bytesPerAsciiGroup)
  {
    int bytesLeft = binaryData.length;
    int i = 0;
    ArrayList<String> s = new ArrayList();
    

    String indentString = String.format("\n%1$-" + String.valueOf(indent) + "s", new Object[] { "" });
    while (bytesLeft > 0)
    {
      s.add(indentString);
      int lineBytesLeft = bytesPerLine > bytesLeft ? bytesLeft : bytesPerLine;
      int padLineBytes = bytesPerLine > bytesLeft ? bytesPerLine - bytesLeft : 0;
      String decodedChars = "";
      while (lineBytesLeft > 0)
      {
        int wordBytesLeft = bytesPerWordGroup > lineBytesLeft ? lineBytesLeft : bytesPerWordGroup;
        int spaceLeftInAsciiGroup = bytesPerAsciiGroup;
        while (wordBytesLeft > 0)
        {
          byte c = binaryData[(i++)];
          s.add(String.format("%1$02X", new Object[] { Byte.valueOf(c) }));
          if ((c < 32) || (c >= 126))
          {
            c = 46;
          }
          
          decodedChars = decodedChars + String.format("%1$c", new Object[] { Character.valueOf((char)c) });
          bytesLeft--;
          lineBytesLeft--;
          wordBytesLeft--;
          spaceLeftInAsciiGroup--; if (spaceLeftInAsciiGroup == 0)
          {
            spaceLeftInAsciiGroup = bytesPerAsciiGroup;
            decodedChars = decodedChars + " ";
          }
        }
        s.add(" ");
      }
      int padChars = padLineBytes * 2 + (padLineBytes + bytesPerWordGroup - 1) / bytesPerWordGroup + 3;
      s.add(String.format(new StringBuilder().append("%1$-").append(String.valueOf(padChars)).append("s").toString(), new Object[] { "" }) + decodedChars);
    }
    
    return Multifunction.CatStringList(s);
  }
  
  public static String FormatBinaryHelp(byte[] binaryData, int indent, int bytesPerLine, int bytesPerWordGroup)
  {
    return FormatBinaryHelp(binaryData, indent, bytesPerLine, bytesPerWordGroup, bytesPerWordGroup);
  }
  
  @Deprecated
  private COMResult HandshakeWithTarget()
  {
    return HandshakeWithTarget110();
  }
  


  @Deprecated
  private COMResult HandshakeWithTarget110()
  {
    COMResult ret = null;
    byte[] testTxData = new byte[39];
    











    if (skipHandshake)
    {
      return new COMResult("handshake skipped by command line", false, false, false);
    }
    


    long startTime = System.currentTimeMillis();
    while (System.currentTimeMillis() - startTime < milliTimeout)
    {
      if ((out != null) || (in != null) || (!readCOMThread.isAlive()) || (!writeCOMThread.isAlive())) {
        break;
      }
      
      try
      {
        Thread.sleep(1L);
      }
      catch (InterruptedException localInterruptedException) {}
    }
    


    if (((out == null) && (in == null)) || (!readCOMThread.isAlive()) || (!writeCOMThread.isAlive()))
    {
      String port = port = comPort.startsWith("\\\\.\\") ? comPort.substring(4) : comPort;
      ret = new COMResult(String.format("cannot open '%1$s'. Ensure the port is valid and not already in use by another application.", new Object[] { port }), true, true, true);
      










      responseBinary = new byte[0];
      return ret;
    }
    


    ReadResponse(8192, 0L);
    
    byte[] txOutStream = new byte['က'];
    int txOutStreamElements = 0;
    byte[] receivedStream = new byte['က'];
    int receivedStreamElements = 0;
    byte[] expectStream = new byte['က'];
    int expectStreamElements = 0;
    Calendar rtcc = Calendar.getInstance();
    long fullStartTime = System.currentTimeMillis();
    startTime = fullStartTime;
    int txGroupInterval = 50 + (int)Math.pow(milliTimeout, 0.47619047619047616D);
    long lastComActivity = System.currentTimeMillis() - txGroupInterval;
    int txGroupIntervalLimit = 50 + (int)milliTimeout / 2;
    while (System.currentTimeMillis() - startTime < milliTimeout)
    {
      long now = System.currentTimeMillis();
      if (now - lastComActivity >= txGroupInterval)
      {
        if (expectStreamElements + 20 > expectStream.length) {
          break;
        }
        
        if (txGroupInterval < txGroupIntervalLimit)
        {
          txGroupInterval = (int)(txGroupInterval * (1.05D + Math.random() * 0.25D));
          if (txGroupInterval > txGroupIntervalLimit)
          {
            txGroupInterval = txGroupIntervalLimit;
          }
        }
        

        testTxData[0] = 85;
        expectStream[(expectStreamElements++)] = testTxData[0];
        testTxData[1] = 85;
        expectStream[(expectStreamElements++)] = testTxData[1];
        testTxData[2] = 85;
        expectStream[(expectStreamElements++)] = testTxData[2];
        testTxData[3] = 77;
        testTxData[4] = 67;
        testTxData[5] = 85;
        expectStream[(expectStreamElements++)] = testTxData[5];
        testTxData[6] = 80;
        testTxData[7] = 72;
        testTxData[8] = 67;
        testTxData[9] = 77;
        testTxData[10] = 69;
        if (baudRate != 0)
        {
          testTxData[11] = ((byte)(baudRate & 0xFF));
          testTxData[12] = ((byte)(baudRate >> 8 & 0xFF));
          testTxData[13] = ((byte)(baudRate >> 16 & 0xFF));
          testTxData[14] = ((byte)(baudRate >> 24 & 0xFF));
        }
        long epochMilliseconds = System.currentTimeMillis();
        
        byte sec10 = (byte)(rtcc.get(13) / 10);
        byte sec1 = (byte)(rtcc.get(13) % 10);
        byte min10 = (byte)(rtcc.get(12) / 10);
        byte min1 = (byte)(rtcc.get(12) % 10);
        byte hr10 = (byte)(rtcc.get(11) / 10);
        byte hr1 = (byte)(rtcc.get(11) % 10);
        byte day10 = (byte)(rtcc.get(5) / 10);
        byte day1 = (byte)(rtcc.get(5) % 10);
        byte month10 = (byte)(rtcc.get(2) / 10);
        byte month1 = (byte)(rtcc.get(2) % 10);
        byte year10 = (byte)(rtcc.get(1) % 100 / 10);
        byte year1 = (byte)(rtcc.get(1) % 100 % 10);
        byte weekday1 = (byte)(rtcc.get(7) - 1);
        
        long epochSec = epochMilliseconds / 1000L;
        
        testTxData[15] = ((byte)(int)(epochSec & 0xFF));
        testTxData[16] = ((byte)(int)(epochSec >> 8 & 0xFF));
        testTxData[17] = ((byte)(int)(epochSec >> 16 & 0xFF));
        testTxData[18] = ((byte)(int)(epochSec >> 24 & 0xFF));
        

        testTxData[19] = ((byte)(sec10 << 4 | sec1));
        testTxData[20] = ((byte)(min10 << 4 | min1));
        testTxData[21] = ((byte)(hr10 << 4 | hr1));
        testTxData[22] = ((byte)(int)((epochMilliseconds % 1000L & 0x3) << 4 | weekday1));
        testTxData[23] = ((byte)(day10 << 4 | day1));
        testTxData[24] = ((byte)(month10 << 4 | month1));
        testTxData[25] = ((byte)(year10 << 4 | year1));
        
        testTxData[26] = ((byte)(int)(epochMilliseconds % 1000L >> 2));
        testTxData[27] = 85;
        
        SendBinary(testTxData, 0, 28);
        long lastTxTime = epochMilliseconds;
        lastComActivity = lastTxTime;
        System.arraycopy(testTxData, 0, txOutStream, txOutStreamElements, 28);
        txOutStreamElements += 28;
        System.arraycopy(testTxData, 11, expectStream, expectStreamElements, 17);
        expectStreamElements += 17;
      }
      
      ret = ReadResponse(18, 1L);
      if ((asyncClose) || (fileError)) {
        break;
      }
      
      if (responseBinary.length != 0)
      {


        lastComActivity = System.currentTimeMillis();
        if (receivedStreamElements + responseBinary.length > receivedStream.length) {
          break;
        }
        
        System.arraycopy(responseBinary, 0, receivedStream, receivedStreamElements, responseBinary.length);
        receivedStreamElements += responseBinary.length;
        
        if (receivedStreamElements >= 18)
        {



          boolean matches = true;
          for (int i = 0; i < 18; i++)
          {
            if (receivedStream[(receivedStreamElements - 1 - i)] != expectStream[(expectStreamElements - 1 - i)])
            {
              matches = false;
              break;
            }
          }
          if (matches)
          {


            ret = SendCommand("#", 13, milliTimeout);
            

            if ((!error) && (responseBinary != null) && (responseBinary.length != 0) && (responseBinary[0] == 43))
            {


              System.arraycopy(responseBinary, 0, receivedStream, receivedStreamElements, responseBinary.length);
              receivedStreamElements += responseBinary.length;
              responseBinary = new byte[receivedStreamElements];
              System.arraycopy(receivedStream, 0, responseBinary, 0, receivedStreamElements);
              int devID = responseBinary[(receivedStreamElements - 12)] & 0xFF | (responseBinary[(receivedStreamElements - 11)] & 0xFF) << 8 | (responseBinary[(receivedStreamElements - 10)] & 0xFF) << 16 | (responseBinary[(receivedStreamElements - 9)] & 0xFF) << 24;
              int devREV = responseBinary[(receivedStreamElements - 8)] & 0xFF | (responseBinary[(receivedStreamElements - 7)] & 0xFF) << 8 | (responseBinary[(receivedStreamElements - 6)] & 0xFF) << 16 | (responseBinary[(receivedStreamElements - 5)] & 0xFF) << 24;
              int remoteWindow = responseBinary[(receivedStreamElements - 4)] & 0xFF | (responseBinary[(receivedStreamElements - 3)] & 0xFF) << 8 | (responseBinary[(receivedStreamElements - 2)] & 0xFF) << 16 | (responseBinary[(receivedStreamElements - 1)] & 0xFF) << 24;
              responseText = String.format("Connected: DEVID, DEVREV: %1$08X, %2$08X; Buffering: %3$d bytes", new Object[] { Integer.valueOf(devID), Integer.valueOf(devREV), Integer.valueOf(remoteWindow) });
              return ret;
            }
          }
        } } }
    if (ret == null)
    {
      ret = new COMResult();
    }
    timedOut |= System.currentTimeMillis() - fullStartTime >= milliTimeout;
    responseText = ("handshake with target device " + (timedOut ? "timed out" : "failed") + String.format(" (TX %1$d bytes, RX %2$d bytes)", new Object[] { Integer.valueOf(txOutStreamElements), Integer.valueOf(receivedStreamElements) }));
    responseBinary = new byte[receivedStreamElements];
    System.arraycopy(receivedStream, 0, responseBinary, 0, receivedStreamElements);
    return ret;
  }
  


  @Deprecated
  private COMResult HandshakeWithTarget109()
  {
    int handshakeTxAttempts = 0;
    byte[] testTxData = new byte[39];
    

    if (skipHandshake)
    {
      return new COMResult("Handshake skipped by command line", false, false, false);
    }
    


    long startTime = System.currentTimeMillis();
    while (System.currentTimeMillis() - startTime < milliTimeout)
    {
      if ((out != null) && (in != null)) {
        break;
      }
      
      try
      {
        Thread.sleep(1L);
      }
      catch (InterruptedException localInterruptedException) {}
    }
    





    ReadResponse(8192, 0L);
    
    startTime = System.currentTimeMillis();
    for (; (handshakeTxAttempts++ < 3) || (System.currentTimeMillis() - startTime < milliTimeout); 
        


















































        goto 515)
    {
      testTxData[0] = 85;
      testTxData[1] = 85;
      testTxData[2] = 85;
      testTxData[3] = 69;
      if (baudRate != 0)
      {
        testTxData[4] = ((byte)(baudRate & 0xFF));
        testTxData[5] = ((byte)(baudRate >> 8 & 0xFF));
        testTxData[6] = ((byte)(baudRate >> 16 & 0xFF));
        testTxData[7] = ((byte)(baudRate >> 24 & 0xFF));
      }
      testTxData[8] = 15;
      testTxData[9] = 31;
      testTxData[10] = 63;
      testTxData[11] = Byte.MAX_VALUE;
      testTxData[12] = -1;
      testTxData[13] = -91;
      testTxData[14] = 90;
      testTxData[15] = 85;
      testTxData[16] = -86;
      testTxData[17] = Byte.MIN_VALUE;
      testTxData[18] = ((byte)(int)System.currentTimeMillis());
      testTxData[19] = ((byte)(int)(System.currentTimeMillis() >> 8));
      testTxData[20] = 85;
      testTxData[21] = 69;
      if (baudRate != 0)
      {
        testTxData[22] = ((byte)(baudRate & 0xFF));
        testTxData[23] = ((byte)(baudRate >> 8 & 0xFF));
        testTxData[24] = ((byte)(baudRate >> 16 & 0xFF));
        testTxData[25] = ((byte)(baudRate >> 24 & 0xFF));
      }
      for (int i = 26; i < 38; i += 4)
      {
        long t = (Math.random() * 4.294967295E9D);
        testTxData[(i + 0)] = ((byte)(int)t);
        testTxData[(i + 1)] = ((byte)(int)(t >> 8));
        testTxData[(i + 2)] = ((byte)(int)(t >> 16));
        testTxData[(i + 3)] = ((byte)(int)(t >> 24));
      }
      testTxData[38] = 35;
      
      startTime = System.currentTimeMillis();
      SendBinary(testTxData);
      int expectIndex = 4;
      boolean commsLookingGood = false;
      int decodePhase = 0;
      int bytesGoodSoFar = 0;
      int attemptTimeout = (int)(milliTimeout * (0.1D + Math.random() * 0.4D));
      int timeBlock = 0; if ((bytesGoodSoFar < 36) && (timeBlock < attemptTimeout))
      {
        COMResult ret = ReadResponse(1, 1L);
        if ((asyncClose) || (fileError))
        {
          responseText = "read error, ";
          return ret;
        }
        
        if (responseBinary.length != 0)
        {
          switch (decodePhase)
          {
          case 0: 
            if (responseBinary[0] == 85)
            {
              bytesGoodSoFar++;
              decodePhase = 1; }
            break;
          

          case 1: 
            if (responseBinary[0] == testTxData[expectIndex])
            {
              bytesGoodSoFar++;
              expectIndex++;
              if (expectIndex > 19)
              {
                decodePhase = 2; } }
            break;
          



          case 2: 
            if (responseBinary[0] == 85)
            {
              bytesGoodSoFar++;
              decodePhase = 3;
              expectIndex = 22; }
            break;
          


          case 3: 
            if (responseBinary[0] == testTxData[expectIndex])
            {
              bytesGoodSoFar++;
              expectIndex++;
              if (expectIndex > 37)
              {
                commsLookingGood = true;
              }
            }
            break;
          }
          
          if (commsLookingGood)
          {
            long fullEchoRoundTripTime = System.currentTimeMillis() - startTime;
            
            ret = ReadResponse(13, milliTimeout / 4L + 2L * fullEchoRoundTripTime);
            if ((responseBinary.length == 13) && (responseBinary[0] == 43))
            {
              int devID = responseBinary[1] & 0xFF | (responseBinary[2] & 0xFF) << 8 | (responseBinary[3] & 0xFF) << 16 | (responseBinary[4] & 0xFF) << 24;
              int devREV = responseBinary[5] & 0xFF | (responseBinary[6] & 0xFF) << 8 | (responseBinary[7] & 0xFF) << 16 | (responseBinary[8] & 0xFF) << 24;
              int remoteWindow = responseBinary[9] & 0xFF | (responseBinary[10] & 0xFF) << 8 | (responseBinary[11] & 0xFF) << 16 | (responseBinary[12] & 0xFF) << 24;
              responseText = String.format("Connected: DEVID, DEVREV: %1$08X, %2$08X; Buffering: %3$d bytes", new Object[] { Integer.valueOf(devID), Integer.valueOf(devREV), Integer.valueOf(remoteWindow) });
              return ret;
            }
          }
        }
        timeBlock++;
      }
    }
    








































































    return new COMResult("Handshake failure", true, false, false);
  }
  








  @Deprecated
  private COMResult HandshakeWithTarget101()
  {
    if (skipHandshake)
    {
      return new COMResult("Handshake skipped by command line", false, false, false);
    }
    


    long startTime = System.currentTimeMillis();
    while (System.currentTimeMillis() - startTime < milliTimeout)
    {
      if ((out != null) && (in != null)) {
        break;
      }
      
      try
      {
        Thread.sleep(1L);
      }
      catch (InterruptedException localInterruptedException) {}
    }
    





    ReadResponse(8192, 10L);
    




    long estimatedRoundTripTime = milliTimeout / 20L < 10L ? 10L : milliTimeout / 20L;
    int autoBaudCharsEchoed = 0;
    startTime = System.currentTimeMillis();
    long lastTime = startTime;
    int autoBaudBackoff = 3;
    int charactersReceived = 0;
    do
    {
      byte[] txData = new byte[1];
      txData[0] = 85;
      SendBinary(txData);
      ret = ReadResponse(1, autoBaudBackoff);
      if (responseBinary.length >= 1)
      {
        charactersReceived++;
        if (responseBinary[0] == 85)
        {
          autoBaudCharsEchoed++;
          

          long now = System.currentTimeMillis();
          if (now - lastTime > estimatedRoundTripTime)
          {
            estimatedRoundTripTime = now - lastTime;
            lastTime = now;
          }
        }
        else
        {
          autoBaudCharsEchoed = 0;
          error = true;
        }
      }
      

      if ((timedOut) && (autoBaudBackoff < 100))
      {
        autoBaudBackoff = (int)(autoBaudBackoff * 1.34D);
      }
    } while ((autoBaudCharsEchoed < 3) && (!asyncClose) && (System.currentTimeMillis() - startTime < milliTimeout));
    

    do
    {
      ret = ReadResponse(10, estimatedRoundTripTime + autoBaudBackoff);
    } while ((responseBinary.length != 0) && (!asyncClose) && (System.currentTimeMillis() - startTime < milliTimeout));
    

    COMResult ret = SendCommand("#", 1, estimatedRoundTripTime * 2L);
    
    do
    {
      if (responseBinary.length >= 1)
      {
        if (responseBinary[0] == 85)
        {

          estimatedRoundTripTime += autoBaudBackoff;
        } else {
          if (responseBinary[0] == 43) {
            break;
          }
          




          error = true;
          responseText = String.format("discovery/handshake stage, expecting 0x%1$02X ('%2$c') in response to '#' device query command, but received:", new Object[] { Byte.valueOf(43), Character.valueOf('+') }); COMResult 
            tmp408_407 = ret;408407responseText = (408407responseText + "\n{" + FormatBinaryHelp(responseBinary, 4, 32, 1) + "\n}");
          

          return ret;
        }
      }
      ret = ReadResponse(1, estimatedRoundTripTime * 2L);
    } while ((timedOut) && (!asyncClose) && (System.currentTimeMillis() - startTime < milliTimeout));
    
    if (asyncClose)
    {

      return ret;
    }
    if (error)
    {
      if (charactersReceived == 0)
      {
        responseText = Multifunction.FormatHelpText(100, 0, "discovery/handshake failure, did not receive any bytes from the communications channel. Check communications port, baud rate, physical connections and power.");
      }
      else
      {
        responseText = String.format("discovery/handshake failure, did not receive command acknowledgement in response to '#' device query command before the specified timeout (%1$dms).", new Object[] { Long.valueOf(milliTimeout) });
        if (responseBinary.length != 0)
        {
          COMResult tmp565_564 = ret;565564responseText = (565564responseText + String.format("\nLast %1$d bytes received:", new Object[] { Integer.valueOf(responseBinary.length) }) + "\n{" + FormatBinaryHelp(responseBinary, 4, 32, 1) + "\n}");
        }
        



        responseText = Multifunction.FormatHelpText(100, 0, responseText);
        return ret;
      }
    }
    
    ret = ReadResponse(12, estimatedRoundTripTime * 16L > milliTimeout ? milliTimeout : estimatedRoundTripTime * 16L);
    if (error)
    {
      responseText = String.format("discovery/handshake stage, did not receive device info in response to '#' device query command (expecting 12 bytes)", new Object[0]);
      if (responseBinary.length != 0)
      {
        COMResult tmp713_712 = ret;713712responseText = (713712responseText + String.format("\nLast %1$d bytes received:", new Object[] { Integer.valueOf(responseBinary.length) }) + "\n{" + FormatBinaryHelp(responseBinary, 4, 32, 1) + "\n}");
      }
      



      responseText = Multifunction.FormatHelpText(100, 0, responseText);
      return ret;
    }
    

    byte[] testData = new byte[17];
    testData[0] = 69;
    testData[1] = 0;
    testData[2] = 1;
    testData[3] = 3;
    testData[4] = 7;
    testData[5] = 15;
    testData[6] = 31;
    testData[7] = 63;
    testData[8] = Byte.MAX_VALUE;
    testData[9] = -1;
    testData[10] = -91;
    testData[11] = 90;
    testData[12] = 85;
    testData[13] = -86;
    testData[14] = Byte.MIN_VALUE;
    testData[15] = ((byte)(int)System.currentTimeMillis());
    testData[16] = ((byte)(int)(System.currentTimeMillis() >> 8));
    SendBinary(testData);
    ret = ReadResponse(16, 10L + estimatedRoundTripTime * 16L);
    if (error)
    {
      responseText = "handshake echo read error. Expecting 16 bytes. Check hardware and try a lower communications bit rate.";
      if (responseBinary.length != 0)
      {
        COMResult tmp968_967 = ret;968967responseText = (968967responseText + String.format("\nReceived %1$d bytes:", new Object[] { Integer.valueOf(responseBinary.length) }) + "\n{" + FormatBinaryHelp(responseBinary, 4, 32, 1) + "\n}");
      }
      



      responseText = Multifunction.FormatHelpText(100, 0, responseText);
      return ret;
    }
    for (int i = 1; i < testData.length; i++)
    {
      if (responseBinary[(i - 1)] != testData[i])
      {
        error = true;
        responseText = "handshake echo data wrong. Check hardware and try a different communications rate.";
        responseText += String.format("\n    Received: %1$02X %2$02X %3$02X %4$02X %5$02X %6$02X %7$02X %8$02X %9$02X %10$02X %11$02X %12$02X %13$02X %14$02X %15$02X %16$02X", new Object[] { Byte.valueOf(responseBinary[0]), Byte.valueOf(responseBinary[1]), Byte.valueOf(responseBinary[2]), Byte.valueOf(responseBinary[3]), Byte.valueOf(responseBinary[4]), Byte.valueOf(responseBinary[5]), Byte.valueOf(responseBinary[6]), Byte.valueOf(responseBinary[7]), Byte.valueOf(responseBinary[8]), Byte.valueOf(responseBinary[9]), Byte.valueOf(responseBinary[10]), Byte.valueOf(responseBinary[11]), Byte.valueOf(responseBinary[12]), Byte.valueOf(responseBinary[13]), Byte.valueOf(responseBinary[14]), Byte.valueOf(responseBinary[15]) });
        responseText += String.format("\n    Expected: %1$02X %2$02X %3$02X %4$02X %5$02X %6$02X %7$02X %8$02X %9$02X %10$02X %11$02X %12$02X %13$02X %14$02X %15$02X %16$02X", new Object[] { Byte.valueOf(testData[1]), Byte.valueOf(testData[2]), Byte.valueOf(testData[3]), Byte.valueOf(testData[4]), Byte.valueOf(testData[5]), Byte.valueOf(testData[6]), Byte.valueOf(testData[7]), Byte.valueOf(testData[8]), Byte.valueOf(testData[9]), Byte.valueOf(testData[10]), Byte.valueOf(testData[11]), Byte.valueOf(testData[12]), Byte.valueOf(testData[13]), Byte.valueOf(testData[14]), Byte.valueOf(testData[15]), Byte.valueOf(testData[16]) });
        return ret;
      }
    }
    

    COMResult test = ReadResponse(64, 1L + estimatedRoundTripTime);
    if (responseBinary.length != 0)
    {
      error = true;
      timedOut = false;
      responseText = "unexpected extra data received during handshaking in response to '#' device query command.";
      if (responseBinary.length != 0)
      {
        COMResult tmp1612_1611 = ret;16121611responseText = (16121611responseText + String.format("\nReceived %1$d extra bytes:", new Object[] { Integer.valueOf(responseBinary.length) }) + "\n{" + FormatBinaryHelp(responseBinary, 4, 32, 1) + "\n}");
      }
      



      responseText = Multifunction.FormatHelpText(100, 0, responseText);
      return ret;
    }
    

    return new COMResult("Handshake failure", true, false, false);
  }
  



























  @Deprecated
  public COMResult GetTargetCRC(int crcType, boolean suppressHandshake)
  {
    if ((crcType < 0) || (crcType > 2))
    {
      return new COMResult("Incorrect crcType parameter", true, false, false);
    }
    if (!suppressHandshake)
    {
      COMResult ret = HandshakeWithTarget();
      if (error)
      {
        return ret;
      }
    }
    
    COMResult ret = SendCommand(crcType == 1 ? "d" : crcType == 0 ? "c" : "e", 5, milliTimeout * 2L);
    if (error)
    {
      return ret;
    }
    if (responseBinary[0] != 43)
    {
      responseText = String.format("CRC feature unavailable: 0x%1$02X\n", new Object[] { Byte.valueOf(responseBinary[0]) });
      error = true;
      return ret;
    }
    
    long crc = responseBinary[1] & 0xFF | (responseBinary[2] & 0xFF) << 8 | (responseBinary[3] & 0xFF) << 16 | (responseBinary[4] & 0xFF) << 24;
    responseText = String.format("0x%1$08X", new Object[] { Long.valueOf(crc) });
    return ret;
  }
  












  @Deprecated
  public COMResult ReadDeviceFlash(String outputArtifactPath, boolean suppressHandshake)
  {
    long startTime = System.currentTimeMillis();
    


    if (!suppressHandshake)
    {
      COMResult ret = HandshakeWithTarget();
      if (error)
      {
        return ret;
      }
      

      ret = SendCommand("#", 13, milliTimeout);
      if (error)
      {
        responseText = Multifunction.FormatHelpText(100, 0, "after handshaking, did not receive a valid response to the '#' device ID read command");
        return ret;
      }
      int devID = responseBinary[1] & 0xFF | (responseBinary[2] & 0xFF) << 8 | (responseBinary[3] & 0xFF) << 16 | (responseBinary[4] & 0xFF) << 24;
      int devREV = responseBinary[5] & 0xFF | (responseBinary[6] & 0xFF) << 8 | (responseBinary[7] & 0xFF) << 16 | (responseBinary[8] & 0xFF) << 24;
      int remoteWindow = responseBinary[9] & 0xFF | (responseBinary[10] & 0xFF) << 8 | (responseBinary[11] & 0xFF) << 16 | (responseBinary[12] & 0xFF) << 24;
      System.out.format("Connected: DEVID, DEVREV: %1$08X, %2$08X; Buffering: %3$d bytes\n", new Object[] { Integer.valueOf(devID), Integer.valueOf(devREV), Integer.valueOf(remoteWindow) });
    }
    

    COMResult ret = SendCommand("F", 8, milliTimeout);
    if (error)
    {
      responseText = String.format("Did not receive image file length from target. Device read-back may not be enabled in the device firmware.\n", new Object[0]);
      return ret;
    }
    if (responseBinary[0] == 43)
    {

      if ((responseBinary[5] == 66) && (responseBinary[6] == 76))
      {

        byte[] trimmedData = new byte[responseBinary.length - 1];
        System.arraycopy(responseBinary, 1, trimmedData, 0, trimmedData.length);
        responseBinary = trimmedData;
      }
    }
    
    int blobSize = responseBinary[0] & 0xFF;
    blobSize |= (responseBinary[1] & 0xFF) << 8;
    blobSize |= (responseBinary[2] & 0xFF) << 16;
    blobSize |= (responseBinary[3] & 0xFF) << 24;
    int bytesRemaining = blobSize - responseBinary.length;
    
    byte[] buffer = new byte[blobSize > 1048576 ? 1048576 : blobSize];
    System.arraycopy(responseBinary, 0, buffer, 0, responseBinary.length);
    int bufferIndex = responseBinary.length;
    int fileIndex = 0;
    

    int chunkLen = 1024;
    int lastPercentDone = 0;
    System.out.print("Read progress:   |0%         25%         50%         75%        100%|\n");
    System.out.print("                 |");
    System.out.flush();
    while (bytesRemaining != 0)
    {
      if (chunkLen > bytesRemaining)
      {
        chunkLen = bytesRemaining;
      }
      if (chunkLen > buffer.length - bufferIndex)
      {
        chunkLen = buffer.length - bufferIndex;
      }
      ret = ReadResponse(chunkLen, milliTimeout);
      if (responseBinary != null)
      {
        System.arraycopy(responseBinary, 0, buffer, bufferIndex, responseBinary.length);
        bufferIndex += responseBinary.length;
        if (bufferIndex >= buffer.length)
        {
          Multifunction.WriteFile(outputArtifactPath, buffer, fileIndex != 0);
          fileIndex += bufferIndex;
          bufferIndex = 0;
        }
      }
      if (error)
      {

        if (bufferIndex != buffer.length)
        {
          buffer = Arrays.copyOf(buffer, bufferIndex);
        }
        Multifunction.WriteFile(outputArtifactPath, buffer, fileIndex != 0);
        
        System.out.println();
        responseText = String.format("Error reading data from target at offset %1$d\n", new Object[] { Integer.valueOf(fileIndex + bufferIndex) });
        return ret;
      }
      bytesRemaining -= chunkLen;
      

      int percentDone = (blobSize - bytesRemaining) * 100 / blobSize;
      while (percentDone - lastPercentDone >= 2)
      {
        System.out.append('.');
        System.out.flush();
        lastPercentDone += 2;
      }
    }
    

    long totalTime = System.currentTimeMillis() - startTime;
    System.out.print("|\n                  ");
    
    String status = String.format("%1$d bytes received in %2$1.3fs (%3$1.0f bytes/second)", new Object[] { Integer.valueOf(fileIndex + bufferIndex), Double.valueOf(totalTime / 1000.0D), Double.valueOf(fileIndex + bufferIndex / (totalTime / 1000.0D + 1.0E-6D)) });
    System.out.println(status);
    

    if (bufferIndex != buffer.length)
    {
      buffer = Arrays.copyOf(buffer, bufferIndex);
    }
    Multifunction.WriteFile(outputArtifactPath, buffer, fileIndex != 0);
    
    error = false;
    timedOut = false;
    asyncClose = false;
    responseText = "Read-back completed successfully";
    return ret;
  }
  
  public static String GetEZBLBootloaderErrorText(int errorCode, int EZBLReleaseVersion)
  {
    if (EZBLReleaseVersion >= 200)
    {
      switch (errorCode)
      {
      case 1: 
        return "Operation completed successfully.";
      case 2: 
        return "Operation completed successfully, but the programmed image did not have an APPID_VER_MAJOR.APPID_VER_MINOR field that was +1 (minor code) from the existing application.";
      case 3: 
        return "Offered firmware image already matches the existing target firmware.";
      case -20: 
        return "Bootloader signaled communications timeout while waiting for image data.";
      case -21: 
        return "Bootloader rejected firmware as malformed or of unsupported type. Possible communications error.";
      case -22: 
        return "Bootloader rejected firmware as incompatible.";
      case -23: 
        return "Bootloader rejected firmware as out of the required programming order.";
      case -24: 
        return "Bootloader read-back verification failure.";
      case -25: 
        return "Bootloader read-back verification mismatch in reserved address range.";
      case -26: 
        return "Bootloader computed CRC mismatch with CRC contained in firmware image. Probable communications error.";
      case -27: 
        return "Bootloader or running application rejected the offered image.";
      }
      
      return null;
    }
    

    if (EZBLReleaseVersion < 200)
    {
      switch (errorCode)
      {
      case 1: 
        return "Operation completed successfully.";
      
      case -1: 
        return "Communications timeout attempting to read the first 4 bytes of the .blob file (where the file's length is contained).";
      case -2: 
        return "Communications timeout attempting to read a record header from the .blob file.";
      case -3: 
        return "Communications corruption occurred or the .blob file contains an illegally long length field in a data record or the overall .blob header.";
      case -4: 
        return "Communications corruption occurred or the .blob file contains an illegally high record address.";
      case -5: 
        return "Communications timeout trying to read .blob record data.";
      case -6: 
        return "Communications timeout reading last 4 byte CRC field.";
      case -7: 
        return "CRC of received .blob contents mismatch with CRC contained in .blob. Probable communications corruption.";
      case -8: 
        return "Read-back verification mismatch. Probable configuration error or write protected memory.";
      case -9: 
        return "Read-back verification mismatch. All programming completed, but data in the existing bootloader does not match the bootloader copy in the uploaded image. Make sure you transmitted a correct .hex/.blob file that exactly matches and was built for the installed bootloader. The Application must be compiled with _merge.s and _merge.gld files generated when the bootloader was originally built and deployed.";
      }
      return null;
    }
    

    return null;
  }
  

















  @Deprecated
  public COMResult Bootload(String artifactPath, boolean suppressHandshake)
  {
    int bytesUploaded = 0;
    



    int remoteWindow = Integer.MAX_VALUE;
    




    long startTime = System.currentTimeMillis();
    
    try
    {
      Blob blob = null;
      byte[] blobData; if (artifactPath.toLowerCase().endsWith(".hex"))
      {
        String hexData = Multifunction.ReadFile(artifactPath, true);
        if (hexData == null)
        {
          return new COMResult("could not read " + artifactPath, true, false, true);
        }
        blob = new Blob(hexData, true);
        DataRecord.CoalesceRecords(records, true, 2L, 2L);
        blob.ReorderForSequentialAccess();
        blobData = blob.GetBytes();
      }
      else
      {
        byte[] blobData = Multifunction.ReadFileBinary(artifactPath);
        if (blobData == null)
        {
          return new COMResult("could not read " + artifactPath, true, false, true);
        }
        


        if ((state.targetPartition != 0) || (state.devSpecialConf.FBOOTValue != 16777215))
        {
          blob = new Blob(blobData, true);
        }
      }
    }
    catch (Exception ex)
    {
      return new COMResult("could not parse " + artifactPath + ". " + ex.getMessage(), true, false, true);
    }
    byte[] blobData;
    Blob blob;
    if ((state.targetPartition != 0) || (state.devSpecialConf.FBOOTValue != 16777215))
    {
      byte[] FBOOTbytes = new byte[3];
      
      FBOOTbytes[0] = ((byte)(state.devSpecialConf.FBOOTValue & 0xFF));
      FBOOTbytes[1] = ((byte)((state.devSpecialConf.FBOOTValue & 0xFF00) >> 8));
      FBOOTbytes[2] = ((byte)((state.devSpecialConf.FBOOTValue & 0xFF0000) >> 16));
      
      if ((state.targetPartition >= 1) && (state.devSpecialConf.FBOOTValue == 16777215))
      {
        FBOOTbytes[0] = -2;
        FBOOTbytes[1] = -1;
        FBOOTbytes[2] = -1;
      }
      
      if (state.devSpecialConf.FBOOTAddr == 0)
      {






        records.add(0, new DataRecord(8392704L, FBOOTbytes, state.MCU16Mode));
        records.add(0, new DataRecord(8394752L, FBOOTbytes, state.MCU16Mode));
      }
      else
      {
        records.add(0, new DataRecord(state.devSpecialConf.FBOOTAddr, FBOOTbytes, state.MCU16Mode));
      }
      blobData = blob.GetBytes();
    }
    


    if (!suppressHandshake)
    {
      COMResult ret = HandshakeWithTarget();
      if (error)
      {
        return ret;
      }
      

      ret = SendCommand("#", 13, milliTimeout);
      if (error)
      {
        responseText = "after handshaking, did not receive a valid response to the '#' device ID read command.";
        if (responseBinary.length != 0)
        {
          responseText += String.format("\nReceived %1$d bytes:\n{\n    ", new Object[] { Integer.valueOf(responseBinary.length) });
          


          for (int i = 0; i < responseBinary.length; i++)
          {
            if ((i + 1) % 32 == 0)
            {
              responseText += "\n    ";
            }
            responseText += String.format("%1$02X ", new Object[] { Byte.valueOf(responseBinary[i]) });
          }
          responseText += "\n}";
          responseText = Multifunction.FormatHelpText(100, 0, responseText);
        }
        
        return ret;
      }
      int devID = responseBinary[1] & 0xFF | (responseBinary[2] & 0xFF) << 8 | (responseBinary[3] & 0xFF) << 16 | (responseBinary[4] & 0xFF) << 24;
      int devREV = responseBinary[5] & 0xFF | (responseBinary[6] & 0xFF) << 8 | (responseBinary[7] & 0xFF) << 16 | (responseBinary[8] & 0xFF) << 24;
      remoteWindow = responseBinary[9] & 0xFF | (responseBinary[10] & 0xFF) << 8 | (responseBinary[11] & 0xFF) << 16 | (responseBinary[12] & 0xFF) << 24;
      System.out.format("Connected: DEVID, DEVREV: %1$08X, %2$08X; Buffering: %3$d bytes\n", new Object[] { Integer.valueOf(devID), Integer.valueOf(devREV), Integer.valueOf(remoteWindow) });
    }
    

    COMResult ret = SendCommand("MCHPb", 1, milliTimeout);
    if ((error) || (responseBinary[0] != 43))
    {
      if (error)
      {
        responseText = "no ACK to bootload start command";
      }
      else
      {
        error = true;
        responseText = String.format("bad bootload request response 0x%1$02X ('" + String.valueOf((char)responseBinary[0]) + "')", new Object[] { Byte.valueOf(responseBinary[0]) });
      }
      return ret;
    }
    










    System.out.print("Erase");
    for (;;)
    {
      ret = ReadResponse(1, milliTimeout);
      if (error)
      {
        System.out.println();
        responseText = ("erase stage " + (timedOut ? "timeout" : ""));
        return ret;
      }
      if (responseBinary[0] == 45)
      {
        System.out.println();
        responseText = "erase stage (NACK)";
        error = true;
        return ret;
      }
      if (responseBinary[0] == 43)
      {
        System.out.println();
        break;
      }
      System.out.append(String.valueOf((char)responseBinary[0]));
      System.out.flush();
    }
    



    int lastPercentDone = 0;
    System.out.print("Upload progress: |0%         25%         50%         75%        100%|\n");
    System.out.print("                 |");
    System.out.flush();
    
    int chunkLen = blobData.length / 100;
    for (;;)
    {
      if (!state.hardwareFlowControl)
      {
        ret = ReadResponse(2, milliTimeout);
        if (error)
        {
          System.out.println();
          responseText += String.format("did not receive file chunk request near offset %1$d (of %2$d)", new Object[] { Integer.valueOf(bytesUploaded), Integer.valueOf(blobData.length) });
          return ret;
        }
        

        chunkLen = responseBinary[0] & 0xFF | responseBinary[1] << 8 & 0xFF00;
      }
      else
      {
        ret = ReadResponse(2, blobData.length - bytesUploaded == 0 ? milliTimeout : 0L);
        if ((!error) && (!timedOut))
        {

          chunkLen = responseBinary[0] & 0xFF | responseBinary[1] << 8 & 0xFF00;
        }
      }
      


      if ((chunkLen > remoteWindow) && (!state.hardwareFlowControl))
      {
        System.out.println();
        error = true;
        responseText += Multifunction.FormatHelpText(100, 0, String.format("remote node requested more data than their reported buffer size (%1$d requested, %2$d buffer size)", new Object[] { Integer.valueOf(chunkLen), Integer.valueOf(remoteWindow) }));
        return ret;
      }
      

      if (chunkLen == 0) {
        break;
      }
      


      if (chunkLen > blobData.length - bytesUploaded)
      {
        chunkLen = blobData.length - bytesUploaded;
      }
      

      byte[] dataToTransmit = new byte[chunkLen];
      System.arraycopy(blobData, bytesUploaded, dataToTransmit, 0, chunkLen);
      SendBinary(dataToTransmit);
      bytesUploaded += chunkLen;
      

      int percentDone = bytesUploaded * 100 / blobData.length;
      while (percentDone - lastPercentDone >= 2)
      {
        System.out.append('.');
        System.out.flush();
        lastPercentDone += 2;
      }
    }
    


    ret = ReadResponse(2, milliTimeout);
    if (error)
    {
      System.out.println();
      responseText = String.format("no return code from target near file offset %1$d (of %2$d)", new Object[] { Integer.valueOf(bytesUploaded), Integer.valueOf(blobData.length) });
      return ret;
    }
    
    int errorCode = responseBinary[0] & 0xFF;
    errorCode |= responseBinary[1] << 8;
    if ((errorCode <= 0) || (bytesUploaded != blobData.length))
    {
      System.out.println();
      responseText = String.format("remote node aborted with error %1$d around file offset %2$d (of %3$d)\n", new Object[] { Integer.valueOf(errorCode), Integer.valueOf(bytesUploaded), Integer.valueOf(blobData.length) });
      
      String errorString = GetEZBLBootloaderErrorText(errorCode, 100);
      if (errorString == null)
      {

        COMResult targetGeneratedErrorMessage = ReadLine(100L);
        while (!responseText.isEmpty())
        {
          COMResult tmp1858_1856 = ret;18581856responseText = (18581856responseText + "\n       " + responseText);
          targetGeneratedErrorMessage = ReadLine(100L);
        }
      }
      else {
        responseText += Multifunction.FormatHelpText(110, 4, errorString);
      }
      error = true;
      return ret;
    }
    

    long totalTime = System.currentTimeMillis() - startTime;
    
    System.out.printf("|\n                  %1$d bytes sent in %2$1.3fs (%3$1.0f bytes/second)\n", new Object[] { Integer.valueOf(bytesUploaded), Double.valueOf(totalTime / 1000.0D), Double.valueOf(bytesUploaded / (totalTime / 1000.0D + 1.0E-6D)) });
    




    error = false;
    timedOut = false;
    asyncClose = false;
    responseText = "Bootload complete";
    return ret;
  }
  


























  public COMResult BootloadBL2(String artifactPath, boolean softFlowControl)
  {
    COMResult ret = new COMResult(new byte[2], true, false, false);
    



    int maxKeepAlives = 8192;
    long allUploadedTime = 0L;
    try
    {
      byte[] imgBytes;
      if (artifactPath.matches(".*?\\.[hH][eE][xX]$"))
      {
        String hexData = Multifunction.ReadFile(artifactPath, true);
        if (hexData == null)
        {
          return new COMResult("could not read " + artifactPath, true, false, true);
        }
        Bl2b bl2b = new Bl2b(hexData);
        bl2b.Coalesce(true, 0, 0);
        bl2b.ReorderForSequentialAccess();
        imgBytes = bl2b.GetBytes();
      }
      else
      {
        byte[] imgBytes = Multifunction.ReadFileBinary(artifactPath);
        if (imgBytes == null)
        {
          return new COMResult("could not read " + artifactPath, true, false, true);
        }
      }
    }
    catch (Exception ex)
    {
      return new COMResult("could not parse " + artifactPath + ". " + ex.getMessage(), true, false, true);
    }
    
    byte[] imgBytes;
    
    long startTime = System.currentTimeMillis();
    while (System.currentTimeMillis() - startTime < milliTimeout)
    {
      if (((out != null) && (in != null)) || (!writeCOMThread.isAlive()) || (!readCOMThread.isAlive())) {
        break;
      }
      
      try
      {
        Thread.sleep(2L);
      }
      catch (InterruptedException localInterruptedException) {}
    }
    


    if ((out == null) || (!writeCOMThread.isAlive()))
    {
      String port = port = comPort.startsWith("\\\\.\\") ? comPort.substring(4) : comPort;
      ret = new COMResult(String.format("cannot open '%1$s'. Ensure the port is valid, not already in use by another application and that ezbl_comm.exe is not blocked by antivirus software.", new Object[] { port }), true, true, true);
      










      responseBinary = new byte[0];
      return ret;
    }
    





    ReadResponse(65536, 1L);
    

    SendBinary(imgBytes, 0, imgBytes.length >= 64 ? 64 : imgBytes.length);
    int bytesUploaded = imgBytes.length >= 64 ? 64 : imgBytes.length;
    if (softFlowControl)
    {
      ret = ReadResponse(2, milliTimeout);
      if (timedOut)
      {
        responseText = "no target response";
        return ret;
      }
    }
    int lastPercentDone = 0;
    System.out.print("Upload progress: |0%         25%         50%         75%        100%|\n");
    System.out.print("                 |");
    System.out.flush();
    boolean firstRead = true;
    
    boolean doneSending = false;
    for (;;) {
      int chunkLen;
      if (softFlowControl)
      {
        if (!firstRead)
        {
          ret = ReadResponse(2, milliTimeout);
          if (error)
          {
            System.out.println();
            responseText += String.format("did not receive file chunk request near offset %1$d (of %2$d)", new Object[] { Integer.valueOf(bytesUploaded), Integer.valueOf(imgBytes.length) });
            return ret;
          }
        }
        firstRead = false;
        int chunkLen = responseBinary[0] & 0xFF | responseBinary[1] << 8;
        

        if (chunkLen == 0) {
          break;
        }
        

        if ((chunkLen < 0) || (chunkLen == 4369) || (chunkLen == 4883))
        {
          maxKeepAlives--;
          if (maxKeepAlives != 0)
            continue;
          break;
        }
        



        if (chunkLen > imgBytes.length - bytesUploaded)
        {
          chunkLen = imgBytes.length - bytesUploaded;
        }
      }
      else {
        chunkLen = imgBytes.length - bytesUploaded >= 1024 ? 1024 : imgBytes.length - bytesUploaded;
      }
      
      SendBinary(imgBytes, bytesUploaded, chunkLen);
      bytesUploaded += chunkLen;
      if ((!doneSending) && (bytesUploaded == imgBytes.length))
      {
        doneSending = true;
        allUploadedTime = System.currentTimeMillis();
      } else {
        if ((doneSending) && 
        
          (System.currentTimeMillis() - allUploadedTime >= milliTimeout)) {
          break;
        }
      }
      


      int percentDone = bytesUploaded * 100 / imgBytes.length;
      while (percentDone - lastPercentDone >= 2)
      {
        System.out.append('.');
        System.out.flush();
        lastPercentDone += 2;
      }
      
      if (!writeCOMThread.isAlive()) {
        break;
      }
    }
    



    ret = ReadResponse(2, milliTimeout);
    if (error)
    {
      System.out.println();
      responseText = String.format("no return code from target near file offset %1$d (of %2$d)", new Object[] { Integer.valueOf(bytesUploaded), Integer.valueOf(imgBytes.length) });
      return ret;
    }
    
    int errorCode = responseBinary[0] & 0xFF;
    errorCode |= responseBinary[1] << 8;
    if ((errorCode <= 0) || (bytesUploaded != imgBytes.length))
    {
      System.out.println();
      responseText = String.format("remote node aborted with error %1$d around file offset %2$d (of %3$d)\n", new Object[] { Integer.valueOf(errorCode), Integer.valueOf(bytesUploaded), Integer.valueOf(imgBytes.length) });
      
      String errorString = GetEZBLBootloaderErrorText(errorCode, 204);
      if (errorString == null)
      {

        COMResult targetGeneratedErrorMessage = ReadLine(100L);
        if ((responseText != null) && (!responseText.isEmpty()) && (responseText.matches("[\\x01-\\x7E]*$")))
        {


          COMResult tmp999_997 = ret;999997responseText = (999997responseText + "\n       " + responseText);
        }
        else
        {
          responseText += "\n       Invalid error code (communications corrupt or bootloader not executing)";
        }
      }
      else {
        responseText += Multifunction.FormatHelpText(110, 4, errorString);
      }
      error = true;
      return ret;
    }
    

    long totalTime = System.currentTimeMillis() - startTime;
    
    System.out.printf("|\n                  %1$d bytes sent in %2$1.3fs (%3$1.0f bytes/second)\n", new Object[] { Integer.valueOf(bytesUploaded), Double.valueOf(totalTime / 1000.0D), Double.valueOf(bytesUploaded / (totalTime / 1000.0D + 1.0E-6D)) });
    




    error = false;
    timedOut = false;
    asyncClose = false;
    responseText = "Bootload complete";
    return ret;
  }
}
