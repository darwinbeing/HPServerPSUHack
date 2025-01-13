package com.microchip.apps.ezbl;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InvalidClassException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintStream;
import java.io.Serializable;
import java.io.UnsupportedEncodingException;
import java.nio.file.CopyOption;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;





































public class Multifunction
{
  public Multifunction() {}
  
  public static int[] BytesToInt16s(byte[] data, int fromDataIndex, int toDataIndex)
  {
    if (data == null)
      return null;
    if (toDataIndex <= fromDataIndex) {
      return new int[0];
    }
    int[] ret = new int[(toDataIndex - fromDataIndex + 1) / 2];
    
    int outIndex = 0;
    
    for (int i = fromDataIndex; i <= toDataIndex - 2; i += 2)
    {
      ret[(outIndex++)] = (data[i] & 0xFF | (data[(i + 1)] & 0xFF) << 8);
    }
    if (i == toDataIndex)
      return ret;
    data[i] &= 0xFF;
    return ret;
  }
  

















  public static int[] BytesToInt24s(byte[] data, int fromDataIndex, int toDataIndex)
  {
    if (data == null)
      return null;
    if (toDataIndex <= fromDataIndex) {
      return new int[0];
    }
    int[] ret = new int[(toDataIndex - fromDataIndex + 2) / 3];
    
    int outIndex = 0;
    
    for (int i = fromDataIndex; i <= toDataIndex - 3; i += 3)
    {
      ret[(outIndex++)] = (data[i] & 0xFF | (data[(i + 1)] & 0xFF) << 8 | (data[(i + 2)] & 0xFF) << 16);
    }
    if (i == toDataIndex)
      return ret;
    data[i] &= 0xFF;
    i++; if (i == toDataIndex)
      return ret;
    ret[outIndex] |= (data[i] & 0xFF) << 8;
    return ret;
  }
  



















  public static int[] BytesToInt32s(byte[] data, int fromDataIndex, int toDataIndex)
  {
    if (data == null)
      return null;
    if (toDataIndex <= fromDataIndex) {
      return new int[0];
    }
    int[] ret = new int[(toDataIndex - fromDataIndex + 3) / 4];
    
    int outIndex = 0;
    
    for (int i = fromDataIndex; i <= toDataIndex - 4; i += 4)
    {
      ret[(outIndex++)] = (data[i] & 0xFF | (data[(i + 1)] & 0xFF) << 8 | (data[(i + 2)] & 0xFF) << 16 | (data[(i + 3)] & 0xFF) << 24);
    }
    if (i == toDataIndex)
      return ret;
    data[i] &= 0xFF;
    i++; if (i == toDataIndex)
      return ret;
    ret[outIndex] |= (data[i] & 0xFF) << 8;
    i++; if (i == toDataIndex)
      return ret;
    ret[outIndex] |= (data[i] & 0xFF) << 16;
    return ret;
  }
  
















  public static long[] UInt32sToLongs(int[] data, int fromDataIndex, int toDataIndex)
  {
    if (data == null)
      return null;
    if (toDataIndex <= fromDataIndex) {
      return new long[0];
    }
    long[] ret = new long[toDataIndex - fromDataIndex];
    int outIndex = 0;
    for (int i = fromDataIndex; i < toDataIndex; i++)
    {
      ret[(outIndex++)] = (data[i] & 0xFFFFFFFF);
    }
    return ret;
  }
  










  public static boolean FileExists(String filePath)
  {
    filePath = FixSlashes(TrimQuotes(filePath));
    

    File f = new File(filePath);
    return f.exists();
  }
  













  public static boolean DeleteFile(String filePath)
  {
    filePath = FixSlashes(TrimQuotes(filePath));
    if (filePath == null) {
      return true;
    }
    
    File f = new File(filePath);
    if (!f.exists())
    {
      return false;
    }
    try
    {
      f.delete();
    }
    catch (Exception ex)
    {
      return false;
    }
    return true;
  }
  

















  public static String ReadFile(String fileName, boolean convertCRLFtoLF)
  {
    byte[] data = ReadFileBinary(fileName);
    
    if (data == null)
      return null;
    String strData;
    try {
      strData = new String(data, "UTF-8");
    }
    catch (UnsupportedEncodingException ex) {
      String strData;
      strData = new String(data);
    }
    return convertCRLFtoLF ? strData.replaceAll("\r", "") : strData;
  }
  










  public static byte[] ReadFileBinary(String fileName)
  {
    fileName = FixSlashes(TrimQuotes(fileName));
    if (fileName == null) {
      return null;
    }
    



    try
    {
      File f = new File(fileName);
      if (!f.exists())
      {
        throw new RuntimeException(String.format("File '%s' does not exist", new Object[] { fileName }));
      }
      

      if (f.length() > 2147483647L)
      {
        throw new RuntimeException(String.format("File '%s' too large", new Object[] { fileName }));
      }
      
      FileInputStream in = new FileInputStream(f);
      byte[] data = new byte[(int)(f.length() & 0x7FFFFFFF)];
      in.read(data);
      in.close();
      
      return data;

    }
    catch (IOException ex)
    {
      throw new RuntimeException(String.format("File '%s' read error: %s", new Object[] { fileName, ex.toString() }));
    }
  }
  
  public static String ReadJarResource(String jarFile, String resourceName)
  {
    List<String> ret = new ArrayList();
    byte[] rdData = new byte['က'];
    
    try
    {
      if (jarFile != null) {
        Main.LoadRuntimeJar(new File(jarFile).getCanonicalPath());
      }
      InputStream stream = Multifunction.class.getResourceAsStream(resourceName);
      while (stream.available() > 0)
      {
        int bytesRead = stream.read(rdData);
        if (bytesRead <= 0)
          break;
        ret.add(new String(rdData, 0, bytesRead));
      }
    }
    catch (IOException ex)
    {
      Logger.getLogger(PICXMLLoader.class.getName()).log(Level.SEVERE, null, ex);
      return null;
    }
    
    return CatStringList(ret);
  }
  







  public static int UpdateFileIfDataDifferent(String fileName, byte[] newContents)
  {
    fileName = FixSlashes(TrimQuotes(fileName));
    

    File f = new File(fileName);
    if (f.exists())
    {

      if ((int)f.length() == newContents.length)
      {

        if (Arrays.equals(ReadFileBinary(fileName), newContents))
        {

          return 0;
        }
        
      }
    }
    else {
      try
      {
        f.createNewFile();
      }
      catch (IOException ex)
      {
        throw new RuntimeException(String.format("Unable to create file '%s': %s", new Object[] { fileName, ex.toString() }));
      }
    }
    

    try
    {
      FileOutputStream out = new FileOutputStream(f);
      out.write(newContents);
      out.close();
      return 1;
    }
    catch (IOException ex)
    {
      throw new RuntimeException(String.format("Unable to write to '%s': %s", new Object[] { fileName, ex.toString() }));
    }
  }
  



















  public static int WriteFile(String fileName, byte[] data, boolean append)
  {
    if (fileName == null)
    {
      fileName = "";
    }
    fileName = FixSlashes(TrimQuotes(fileName));
    
    try
    {
      if (fileName.isEmpty())
      {
        String printableData = new String(data);
        System.out.print(printableData);
        return 1;
      }
      


      File f = new File(fileName);
      if ((f.exists()) && (!append))
      {
        f.delete();
      }
      else if (append)
      {
        f.createNewFile();
      }
      

      FileOutputStream out = new FileOutputStream(f, append);
      if (data != null)
        out.write(data);
      out.close();
      
      return 1;
    }
    catch (IOException ex)
    {
      return -1;
    }
    catch (NullPointerException ex) {}
    
    return -2;
  }
  

  static boolean CopyFile(String sourceFilePath, String destFilePath)
  {
    try
    {
      Files.copy(new File(sourceFilePath).toPath(), new File(destFilePath).toPath(), new CopyOption[0]);
    }
    catch (IOException ex)
    {
      return false;
    }
    return true;
  }
  
























  public static int UpdateFileIfDataDifferent(String fileName, String newContents, boolean convertLFtoCRLF)
  {
    if (convertLFtoCRLF)
    {
      newContents = newContents.replaceAll("\n", Matcher.quoteReplacement("\r\n"));
    }
    try
    {
      return UpdateFileIfDataDifferent(fileName, newContents.getBytes("UTF-8"));
    }
    catch (UnsupportedEncodingException ex) {}
    
    return UpdateFileIfDataDifferent(fileName, newContents.getBytes());
  }
  




  public static boolean SaveObjToFile(Object o, String outputFilename)
  {
    try
    {
      FileOutputStream fileOut = new FileOutputStream(outputFilename, false);
      ObjectOutputStream objectOut = new ObjectOutputStream(fileOut);
      objectOut.writeObject(o);
      objectOut.close();
      fileOut.close();
    }
    catch (Exception ex)
    {
      System.err.printf("Error: unable to save object to \"%1$s\"\n       %2$s", new Object[] { outputFilename, ex.getMessage() });
      
      return false; }
    FileOutputStream fileOut;
    ObjectOutputStream objectOut; return true;
  }
  



  public static Object ReadObjFromFile(String objFilename)
  {
    try
    {
      FileInputStream in = new FileInputStream(objFilename);
      ObjectInputStream objectIn = new ObjectInputStream(in);
      Object ret = objectIn.readObject();
      objectIn.close();
      in.close();
      return ret;
    }
    catch (InvalidClassException ex)
    {
      System.err.printf("Error: unable to read object from \"%1$s\"\n       %2$s", new Object[] { objFilename, ex.getMessage() });

    }
    catch (Exception ex)
    {
      System.err.printf("Error: unable to read object from \"%1$s\"\n       %2$s", new Object[] { objFilename, ex.getMessage() });
    }
    
    return null;
  }
  















  public static String TrimQuotes(String str)
  {
    if (str == null)
    {
      return null;
    }
    
    str = str.trim();
    while ((str.startsWith("\"")) && (str.endsWith("\"")))
    {
      str = str.substring(1, str.length() - 1).trim();
    }
    return str;
  }
  









  public static String FixSlashes(String path)
  {
    if (path == null)
    {
      return null;
    }
    
    return path.replaceAll("[\\\\/]+", Matcher.quoteReplacement(File.separator));
  }
  
  public static List<String> ParseCommandLineArguments(String cmdLine)
  {
    if (cmdLine == null) {
      return null;
    }
    List<String> ret = new ArrayList();
    cmdLine = cmdLine.trim();
    Pattern p = Pattern.compile("([\"][^\"]{1,}[\"])|([^ ]{1,})");
    Matcher m = p.matcher(cmdLine);
    while (m.find())
    {
      ret.add(m.group().trim());
    }
    
    return ret;
  }
  










  public static String GetCanonicalPath(String path)
  {
    if (path == null)
    {
      return null;
    }
    File f = new File(path);
    try
    {
      return f.getCanonicalPath();
    }
    catch (IOException ex) {}
    
    return path;
  }
  









  public static String GetCanonicalPath(File file)
  {
    try
    {
      return file.getCanonicalPath();
    }
    catch (IOException ex) {}
    
    return file.getPath();
  }
  









  public static File GetCanonicalFile(File file)
  {
    try
    {
      return file.getCanonicalFile();
    }
    catch (IOException ex) {}
    
    return file;
  }
  



















  public static class SimpleFileFilter
    implements FileFilter
  {
    String filterString = "";
    

    public SimpleFileFilter() {}
    
    public SimpleFileFilter(String filterString)
    {
      this.filterString = filterString;
    }
    

    public boolean accept(File pathname)
    {
      if (pathname.isDirectory())
        return true;
      return pathname.getName().equalsIgnoreCase(filterString);
    }
  }
  
  public static List<File> FindFiles(String directory, String searchFilename, boolean recursive) {
    List<File> ret = new ArrayList();
    File[] fileList = null;
    try
    {
      fileList = new File(directory).listFiles(new SimpleFileFilter(searchFilename));
    }
    catch (Exception ex)
    {
      return ret;
    }
    
    if (fileList != null)
    {
      for (File f : fileList)
      {
        if (f.isFile())
        {
          ret.add(GetCanonicalFile(f));
        }
        else if ((f.isDirectory()) && (recursive))
        {

          List<File> foundSubFiles = FindFiles(f.getPath(), searchFilename, recursive);
          if (foundSubFiles != null)
          {
            for (File subFile : foundSubFiles)
            {
              ret.add(GetCanonicalFile(subFile));
            }
          }
        }
      }
    }
    return ret;
  }
  
















  public static List<File> FindFilesRegEx(String directory, String regexFile, boolean recursive)
  {
    Pattern p = Pattern.compile(regexFile);
    return FindFilesRegEx(directory, p, recursive);
  }
  
  public static List<File> FindFilesRegEx(String directory, Pattern fileSearchPattern, boolean recursive)
  {
    List<File> ret = new ArrayList();
    File[] fileList = null;
    
    try
    {
      fileList = new File(directory).listFiles();
    }
    catch (Exception ex)
    {
      return ret;
    }
    
    if (fileList == null) {
      return ret;
    }
    for (File f : fileList)
    {
      if ((f.isFile()) && (fileSearchPattern.matcher(f.getName()).matches()))
      {
        ret.add(f);
      }
      else if ((f.isDirectory()) && (recursive))
      {

        List<File> foundSubFiles = FindFilesRegEx(f.getPath(), fileSearchPattern, recursive);
        if (foundSubFiles != null)
          ret.addAll(foundSubFiles);
      }
    }
    return ret;
  }
  
  private static final char[] hexChars = "0123456789ABCDEF".toCharArray();
  










































  public static String bytesToHex(byte[] dataBytes, int bytesPerGroup, boolean groupsLittleEndian, int groupsPerLine)
  {
    int outIndex = 0;
    

    if ((dataBytes == null) || (dataBytes.length == 0))
    {
      return groupsPerLine >= 0 ? "\n" : "";
    }
    
    StringBuilder sb = new StringBuilder(dataBytes.length * 3);
    
    for (int inIndex = 0; inIndex < dataBytes.length;)
    {
      if (groupsLittleEndian)
      {
        inIndex += bytesPerGroup - 1;
        for (int i = 0; i < bytesPerGroup; inIndex--)
        {
          if (inIndex >= dataBytes.length)
          {
            sb.insert(outIndex++, ' ');
            sb.insert(outIndex++, ' ');
          }
          else
          {
            sb.insert(outIndex++, hexChars[(dataBytes[inIndex] >> 4 & 0xF)]);
            sb.insert(outIndex++, hexChars[(dataBytes[inIndex] & 0xF)]);
          }
          i++;
        }
        










        inIndex += bytesPerGroup + 1;
      }
      else
      {
        for (int i = 0; (i < bytesPerGroup) && (inIndex < dataBytes.length); inIndex++)
        {
          sb.insert(outIndex++, hexChars[(dataBytes[inIndex] >> 4 & 0xF)]);
          sb.insert(outIndex++, hexChars[(dataBytes[inIndex] & 0xF)]);i++;
        }
      }
      
      if ((groupsPerLine > 0) && ((outIndex + 1) % (groupsPerLine * (bytesPerGroup * 2 + 1)) == 0))
      {
        sb.insert(outIndex++, '\n');
      }
      else if (inIndex < dataBytes.length)
      {
        sb.insert(outIndex++, ' ');
      }
    }
    if ((groupsPerLine > 0) && (sb.charAt(outIndex - 1) != '\n'))
    {
      sb.insert(outIndex, '\n');
    }
    
    return sb.toString();
  }
  

























  public static String bytesToHex(byte[] dataBytes, int bytesPerLine)
  {
    return bytesToHex(dataBytes, 1, false, bytesPerLine);
  }
  












  public static String bytesToHex(byte[] dataBytes)
  {
    return bytesToHex(dataBytes, 1, false, -1);
  }
  
  static String FormatHelpText(int lineWidth, int indent, String helpText)
  {
    return FormatHelpText(helpText, lineWidth, indent, 0, true);
  }
  
  static String FormatHelpText(String helpText, int lineWidth, int indent, int hangingIndent, boolean trimConsecutiveWhitespace)
  {
    String textOut = "";
    int readIndex = 0;
    boolean firstLine = true;
    

    if (trimConsecutiveWhitespace)
    {
      helpText = helpText.replaceAll("\\s+", " ").trim();
    }
    
    readIndex = 0;
    while (readIndex < helpText.length())
    {
      for (int i = 0; i < indent; i++)
      {
        textOut = textOut + " ";
      }
      if (!firstLine)
      {
        for (int i = 0; i < hangingIndent; i++)
        {
          textOut = textOut + " ";
        }
      }
      


      int endReadIndex = readIndex + lineWidth - indent;
      if (helpText.length() < endReadIndex)
      {
        endReadIndex = helpText.length();
      }
      

      int newLinePosition = helpText.substring(readIndex, endReadIndex).indexOf('\n');
      if (newLinePosition >= 0)
      {
        endReadIndex = readIndex + newLinePosition + 1;

      }
      else
      {
        while (readIndex < endReadIndex)
        {
          if (endReadIndex == helpText.length()) {
            break;
          }
          
          if (helpText.charAt(endReadIndex) == ' ') {
            break;
          }
          

          endReadIndex--;
        }
      }
      



      if (endReadIndex == readIndex)
      {
        endReadIndex = readIndex + lineWidth - indent;
        if (endReadIndex > helpText.length())
        {
          endReadIndex = helpText.length();
        }
      }
      
      textOut = textOut + helpText.substring(readIndex, endReadIndex);
      if (newLinePosition < 0)
      {
        textOut = textOut + "\r\n";
      }
      readIndex = endReadIndex;
      



      if (newLinePosition < 0)
      {
        while (readIndex < helpText.length())
        {
          if (helpText.charAt(readIndex) == ' ')
          {


            readIndex++;
            if (readIndex == helpText.length())
            {
              return textOut;
            }
          }
        }
      }
    }
    
    return textOut;
  }
  
  public static List<MemoryRegion> makeList(MemoryRegion... regions)
  {
    if (regions == null)
      return null;
    List<MemoryRegion> ret = new ArrayList(regions.length);
    for (int i = 0; i < regions.length; i++)
    {
      ret.add(regions[i]);
    }
    return ret;
  }
  





  public static String CatStringList(List<String> stringList)
  {
    return CatStringList(stringList, null);
  }
  





  public static String CatStringList(Collection<Integer> decimalList, String delimiter)
  {
    List<String> strList = new ArrayList();
    for (Integer i : decimalList)
    {
      strList.add(i.toString());
    }
    return CatStringList(strList, delimiter);
  }
  






  public static String CatStringList(Collection<String> stringList)
  {
    return CatStringList(new ArrayList(stringList), null);
  }
  









  public static String CatStringList(List<String> stringList, String delimiter)
  {
    int outSize = 0;
    
    if (stringList == null)
    {
      return null;
    }
    
    for (String s : stringList)
    {
      outSize += s.length();
    }
    

    if ((delimiter != null) && (stringList.size() > 1))
    {
      outSize += (stringList.size() - 1) * delimiter.length();
      StringBuilder builder = new StringBuilder(outSize);
      int i = 0;
      for (int j = 0; j < stringList.size() - 1; j++)
      {
        String s = (String)stringList.get(j);
        builder.insert(i, s);
        i += s.length();
        builder.insert(i, delimiter);
        i += delimiter.length();
      }
      builder.insert(i, (String)stringList.get(stringList.size() - 1));
      return builder.toString();
    }
    

    StringBuilder builder = new StringBuilder(outSize);
    int i = 0;
    for (String s : stringList)
    {
      builder.insert(i, s);
      i += s.length();
    }
    return builder.toString();
  }
  

  static class StringList
    implements Serializable
  {
    static final long serialVersionUID = 1L;
    String topString;
    List<String> list = new ArrayList();
    
    StringList() {}
    
    public void sortPartNumbers() { Collections.sort(list, new PICNameComparator()); }
    

    public void sortSFRNames()
    {
      Collections.sort(list, new SFRNameComparator());
    }
    
    public void sort()
    {
      Collections.sort(list);
    }
    
    public void reverse()
    {
      Collections.reverse(list);
    }
    
    public String CatToString()
    {
      return Multifunction.CatStringList(list);
    }
    
    public String CatToString(String delimiter)
    {
      return Multifunction.CatStringList(list, delimiter);
    }
    
    public void Substitute1IfAllPresent(List<String> findList, String singleReplacement)
    {
      Multifunction.Substitute1IfAllPresent(list, findList, singleReplacement);
    }
  }
  

  public static void Substitute1IfAllPresent(List<String> findInList, List<String> findList, String singleReplacement)
  {
    if ((findList == null) || (findList.isEmpty()) || (findInList == null) || (findInList.isEmpty()))
    {
      return;
    }
    if (findInList.containsAll(findList))
    {
      findInList.removeAll(findList);
      findInList.add(singleReplacement);
    }
  }
  
  public static String[] SimpleSplit(String text, String splitToken)
  {
    if (text == null)
      return new String[0];
    if ((splitToken == null) || (splitToken.isEmpty()))
    {
      String[] specialRet = new String[1];
      specialRet[0] = text;
      return specialRet;
    }
    
    List<String> ret = new ArrayList(10 + text.length() / (splitToken.length() * 32));
    int lastPos = 0;
    int findPos = text.indexOf(splitToken, 0);
    while (findPos >= 0)
    {
      ret.add(text.substring(lastPos, findPos));
      lastPos = findPos + splitToken.length();
      findPos = text.indexOf(splitToken, lastPos);
    }
    ret.add(text.substring(lastPos));
    return (String[])ret.toArray(new String[0]);
  }
  





  public static long normalizePIC32Addr(long addr)
  {
    if ((addr & 0xFFC00000) == 2684354560L)
      addr ^= 0x20000000;
    if ((addr & 0x7FC00000) == 486539264L)
      addr |= 0x80000000;
    if ((addr & 0x7FC00000) == 532676608L) {
      addr |= 0xA0000000;
    }
    return addr;
  }
}
