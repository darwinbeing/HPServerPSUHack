package com.microchip.apps.ezbl;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.io.Serializable;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;





















public class ELFDumpParser
  implements Serializable
{
  public ELFDumpParser() {}
  
  private static enum PiecewiseParseStates
  {
    PROCESSING_SECTION_HEADERS, 
    PROCESSING_SYMBOL_TABLE, 
    PROCESSING_SECTION_CONTENTS, 
    PROCESSING_DISASSEMBLY, 
    PROCESSING_OTHER;
    
    private PiecewiseParseStates() {} }
  public List<Section> sections = new ArrayList();
  public List<Symbol> symbols = new ArrayList();
  public TreeMap<Long, String> symMap = new TreeMap();
  public TreeMap<String, Symbol> symbolsByName = new TreeMap();
  public TreeMap<String, Symbol> functionSymsByName = new TreeMap();
  public TreeMap<Long, Symbol> functionSymsByAddr = new TreeMap();
  public TreeMap<Long, Section> romSectionMapByAddr = new TreeMap();
  public Map<String, Section> romSectionMapByName = new HashMap();
  public Map<String, Section> sectionsByName = new HashMap();
  public TreeMap<Integer, String> disassemblyMap = new TreeMap();
  private String piecewiseSecName = null;
  private List<DataRecord> piecewiseSecData = null;
  private List<Process> parserProcesses = new ArrayList();
  private List<InputStream> parserOutput = new ArrayList();
  private String piecewiseResidual = "";
  private PiecewiseParseStates parser = PiecewiseParseStates.PROCESSING_OTHER;
  public String symbolTableAsString = null;
  public List<String> symbolTableAsStrings = null;
  





















  @Deprecated
  public ELFDumpParser(String dumpContents)
  {
    symbols = new ArrayList();
    sections = new ArrayList();
    

    dumpContents = dumpContents.replaceAll("\r", "");
    

    Pattern p = Pattern.compile("\nSections:[\\s]*?\n[^\\n]*?\n(([\\s[0-9]]*?[^\\n]*?\n[^\\n]*?\n)*?)(?=[^\\s0-9])", 32);
    Matcher m = p.matcher(dumpContents);
    if (!m.find())
    {
      System.err.println("EZBL: Could not locate Sections table. Ensure \"Symbols info: Strip all\" option was not selected when building.");
      return;
    }
    
    String[] parseLines = m.group(1).split("\n");
    for (int i = 0; i < parseLines.length; i += 2)
    {
      Section sec = new Section(parseLines[i] + "\n" + parseLines[(i + 1)]);
      sections.add(sec);
      sectionsByName.put(name, sec);
    }
    

    p = Pattern.compile("\nSYMBOL TABLE:[^\n]*?\n(.*)\n{2}", 32);
    m = p.matcher(dumpContents);
    if (!m.find())
    {
      System.err.println("EZBL: Could not locate SYMBOL TABLE. Ensure \"Symbols info: Strip all\" option was not selected when building.");
      return;
    }
    

    parseLines = m.group(1).split("\n");
    String lastFilenameFound = "";
    
    for (String symbolLine : parseLines)
    {
      Symbol symbol = new Symbol(symbolLine);
      probableFile = lastFilenameFound;
      if (flags.file)
      {
        lastFilenameFound = name;
      }
      else if (section.equals("__c30_signature"))
      {
        lastFilenameFound = "*";
      }
      
      symbols.add(symbol);
    }
  }
  



  public int startObjDump(String... commandAndOptions)
  {
    ProcessBuilder proc = new ProcessBuilder(commandAndOptions);
    return startObjDump(proc);
  }
  
  public int startObjDump(List<String> commandAndOptions)
  {
    ProcessBuilder proc = new ProcessBuilder(commandAndOptions);
    return startObjDump(proc);
  }
  
  public int startObjDump(ProcessBuilder proc)
  {
    try
    {
      proc.redirectErrorStream(true);
      Process ps = proc.start();
      parserProcesses.add(ps);
      parserOutput.add(ps.getInputStream());
      
      return parserProcesses.size() - 1;
    }
    catch (IOException e)
    {
      System.err.println("ezbl_tools: " + e.getMessage()); }
    return -1;
  }
  











  public String parseAllObjOutput(int processIndex)
  {
    if (processIndex >= parserProcesses.size())
    {
      return "";
    }
    
    List<String> objDumpText = new ArrayList();
    boolean lastIteration = false;
    Charset charset = (Charset)Charset.availableCharsets().get("ISO-8859-1");
    
    InputStream obj = (InputStream)parserOutput.get(processIndex);
    byte[] b = new byte[' '];
    
    try
    {
      for (;;)
      {
        int readBytes = obj.read(b);
        if (readBytes <= 0)
        {
          parserProcesses.remove(processIndex);
          parserOutput.remove(processIndex);
          parsePiecewise((String)null);
          return Multifunction.CatStringList(objDumpText);
        }
        
        String piece = new String(b, 0, readBytes, charset);
        objDumpText.add(piece);
        parsePiecewise(piece);
      }
    }
    catch (IOException e) {
      try {
        for (;;) {
          int retCode = ((Process)parserProcesses.get(processIndex)).waitFor();
          if ((retCode != 0) || (lastIteration))
            break;
          lastIteration = true;
        }
      }
      catch (InterruptedException localInterruptedException) {}
      



      b = null;
      parserProcesses.remove(processIndex);
      parserOutput.remove(processIndex);
      parsePiecewise((String)null); }
    return Multifunction.CatStringList(objDumpText);
  }
  


  private void endPiecewiseSectionDataParsing()
  {
    if ((symbolTableAsStrings != null) && (!symbolTableAsStrings.isEmpty()))
    {
      symbolTableAsString = Multifunction.CatStringList(symbolTableAsStrings);
      symbolTableAsStrings = null;
    }
    
    if (piecewiseSecName == null)
    {
      return;
    }
    

    boolean matchFound = false;
    DataRecord.CoalesceRecords(piecewiseSecData, true);
    
    Section existingSec = (Section)sectionsByName.get(piecewiseSecName);
    if (existingSec != null)
    {
      if (piecewiseSecData.size() > 0)
      {
        if (piecewiseSecData.get(0)).address != loadMemoryAddress)
        {
          if (piecewiseSecData.get(0)).address == virtualMemoryAddress)
            piecewiseSecData.get(0)).address = loadMemoryAddress;
        }
        data = ((DataRecord)piecewiseSecData.get(0));
        if (piecewiseSecData.size() > 1)
        {

          System.err.println("ezbl_tools: More section fragments than expected.");
        }
      }
      matchFound = true;
    }
    if (!matchFound)
    {
      for (Section s : sections)
      {
        if ((name.equals(piecewiseSecName)) && ((loadMemoryAddress == piecewiseSecData.get(0)).address) || (virtualMemoryAddress == piecewiseSecData.get(0)).address)))
        {
          if (piecewiseSecData.size() > 0)
          {
            data = ((DataRecord)piecewiseSecData.get(0));
            if (piecewiseSecData.size() > 1)
            {

              System.err.println("ezbl_tools: More section fragments than expected.");
            }
          }
          matchFound = true;
          break;
        }
      }
    }
    
    if (!matchFound)
    {
      Section s = new Section();
      name = piecewiseSecName;
      if (piecewiseSecData.size() > 0)
      {
        data = ((DataRecord)piecewiseSecData.get(0));
        if (piecewiseSecData.size() > 1)
        {

          System.err.println("ezbl_tools: More section fragments than expected.");
        }
        loadMemoryAddress = data.address;
        virtualMemoryAddress = data.address;
        size = (data.architecture16Bit ? (data.data.length + 2) / 3 * 2 : data.data.length);
        isDebug = true;
      }
      sections.add(s);
      sectionsByName.put(name, s);
      if (isROM)
      {
        romSectionMapByName.put(name, s);
        for (long addr = loadMemoryAddress; addr < loadMemoryAddress + size; addr += 2L)
        {
          romSectionMapByAddr.put(Long.valueOf(addr), s);
        }
      }
    }
    piecewiseSecName = null;
    piecewiseSecData = null;
  }
  

  public void parsePiecewise(String dumpText)
  {
    int newLineIndex = -1;
    boolean elementChange = false;
    
    if (dumpText == null)
    {
      endPiecewiseSectionDataParsing();
      return;
    }
    
    piecewiseResidual += dumpText;
    piecewiseResidual = piecewiseResidual.replace("\r", "");
    int startIndex;
    for (;;)
    {
      startIndex = newLineIndex + 1;
      

      newLineIndex = piecewiseResidual.indexOf('\n', startIndex);
      if (newLineIndex < 0) {
        break;
      }
      


      if (piecewiseResidual.startsWith("Sections:", startIndex))
      {
        newLineIndex = piecewiseResidual.indexOf('\n', newLineIndex + 1);
        if (newLineIndex < 0) {
          break;
        }
        
        parser = PiecewiseParseStates.PROCESSING_SECTION_HEADERS;
        elementChange = true;
      }
      else if (piecewiseResidual.startsWith("SYMBOL TABLE:", startIndex))
      {
        parser = PiecewiseParseStates.PROCESSING_SYMBOL_TABLE;
        symbolTableAsStrings = new ArrayList();
        symbolTableAsString = null;
        elementChange = true;
      }
      else if (piecewiseResidual.startsWith("Contents of section ", startIndex))
      {









        parser = PiecewiseParseStates.PROCESSING_SECTION_CONTENTS;
        elementChange = true;
      }
      else if (piecewiseResidual.startsWith("Disassembly of section ", startIndex))
      {


















        parser = PiecewiseParseStates.PROCESSING_DISASSEMBLY;
        elementChange = true;
      }
      else if ((piecewiseResidual.charAt(startIndex) == '\n') && (parser != PiecewiseParseStates.PROCESSING_DISASSEMBLY))
      {
        parser = PiecewiseParseStates.PROCESSING_OTHER;
        elementChange = true;
      }
      

      if (elementChange)
      {
        endPiecewiseSectionDataParsing();
        if (parser == PiecewiseParseStates.PROCESSING_SECTION_CONTENTS)
        {

          piecewiseSecName = piecewiseResidual.substring(startIndex + "Contents of section ".length(), newLineIndex - 1);
          piecewiseSecData = new ArrayList();
        }
        elementChange = false;
      }
      else
      {
        switch (1.$SwitchMap$com$microchip$apps$ezbl$ELFDumpParser$PiecewiseParseStates[parser.ordinal()])
        {

        case 1: 
          newLineIndex = piecewiseResidual.indexOf('\n', newLineIndex + 1);
          if (newLineIndex < 0)
          {
            piecewiseResidual = piecewiseResidual.substring(startIndex);
            return;
          }
          Section sec = new Section(piecewiseResidual.substring(startIndex, newLineIndex));
          
          if (name == null)
          {
            parser = PiecewiseParseStates.PROCESSING_OTHER;
            elementChange = true;
          }
          else
          {
            if (isROM)
            {
              romSectionMapByName.put(name, sec);
              for (long addr = loadMemoryAddress; addr < loadMemoryAddress + size; addr += 2L)
              {
                romSectionMapByAddr.put(Long.valueOf(addr), sec);
              }
            }
            sections.add(sec);
            sectionsByName.put(name, sec);
          }
          break;
        
        case 2: 
          String sub = piecewiseResidual.substring(startIndex, newLineIndex);
          Symbol s = new Symbol(sub);
          if (name == null)
          {
            parser = PiecewiseParseStates.PROCESSING_OTHER;
            symbolTableAsString = Multifunction.CatStringList(symbolTableAsStrings);
            symbolTableAsStrings = null;
            elementChange = true;
          }
          else
          {
            symbolTableAsStrings.add(piecewiseResidual.substring(startIndex, newLineIndex + 1));
            symbols.add(s);
            symMap.put(Long.valueOf(address), name);
            symbolsByName.put(name, s);
            if ((flags.function) && (!flags.local))
            {
              functionSymsByAddr.put(Long.valueOf(address), s);
              functionSymsByName.put(name, s);
            }
          }
          
          break;
        case 3: 
          DataRecord dr = new DataRecord(piecewiseResidual.substring(startIndex, newLineIndex + 1));
          if (address < 0L)
          {
            parser = PiecewiseParseStates.PROCESSING_OTHER;
            elementChange = true;
          }
          else
          {
            if (piecewiseSecData.size() > 0)
            {
              architecture16Bit = piecewiseSecData.get(0)).architecture16Bit;
            }
            piecewiseSecData.add(dr);
          }
          break;
        
        case 4: 
          String line = piecewiseResidual.substring(startIndex, newLineIndex);
          if (line.matches("^[ ]*[0-9a-zA-Z]+[\\:]\\t[^$]*"))
          {
            int end = line.indexOf(':');
            
            int instrStart = 0;
            
            for (int start = end - 1; start >= 0; start--)
            {
              if (line.charAt(start) == ' ')
              {
                start++;
                break;
              }
            }
            if ((start < end) && (end >= 1) && (start >= 0))
            {
              instrStart = line.indexOf('\t');
              if (instrStart > 0)
              {
                instrStart = line.indexOf('\t', instrStart + 1);
              }
              if (instrStart > 0)
              {
                Integer addr = Integer.decode("0x" + line.substring(start, end));
                String disassembledOp = line.substring(instrStart + 1).replace('\002', '?').replace('\001', '?').trim();
                disassemblyMap.put(addr, disassembledOp);
              }
            }
          }
          break;
        }
        
      }
    }
    piecewiseResidual = piecewiseResidual.substring(startIndex);
  }
  

  public void parsePiecewise(byte[] dumpBytes)
  {
    if (dumpBytes == null)
    {
      parsePiecewise((String)null);
    }
    else
    {
      parsePiecewise(new String(dumpBytes, (Charset)Charset.availableCharsets().get("ISO-8859-1")));
    }
  }
  
  public void parsePiecewise(byte[] dumpBytes, int offset, int length)
  {
    parsePiecewise(new String(dumpBytes, offset, length, (Charset)Charset.availableCharsets().get("ISO-8859-1")));
  }
  










  public static long NormalizePIC32Addr(long address)
  {
    if ((address & 0xFFC00000) == 2684354560L)
      address ^= 0x20000000;
    if ((address & 0x7FC00000) == 486539264L)
      address |= 0x80000000;
    if ((address & 0x7FC00000) == 532676608L)
      address |= 0xA0000000;
    return address;
  }
  





  public void normalizePIC32Addresses()
  {
    if (symbols != null)
    {
      for (Symbol s : symbols)
      {
        address = NormalizePIC32Addr(address);
      }
    }
    if (symMap != null)
    {
      for (Long addr : symMap.keySet())
      {
        addr = Long.valueOf(NormalizePIC32Addr(addr.longValue()));
      }
    }
    
    if (sections != null)
    {
      for (Section s : sections)
      {
        loadMemoryAddress = NormalizePIC32Addr(loadMemoryAddress);
        if (symbols != null)
        {
          for (Long addr : symbolsByAddr.keySet())
          {
            addr = Long.valueOf(NormalizePIC32Addr(addr.longValue()));
          }
        }
      }
    }
    
    if (romSectionMapByAddr != null)
    {
      for (Long addr : romSectionMapByAddr.keySet())
      {
        addr = Long.valueOf(NormalizePIC32Addr(addr.longValue()));
      }
    }
    
    if (functionSymsByAddr != null)
    {
      for (Long addr : functionSymsByAddr.keySet())
      {
        addr = Long.valueOf(NormalizePIC32Addr(addr.longValue()));
      }
    }
  }
  
  public List<Section> removeSections(String sectionNameRegex)
  {
    List<Section> removed = new ArrayList();
    
    for (int secIndex = 0; secIndex < sections.size(); secIndex++)
    {
      Section sec = (Section)sections.get(secIndex);
      



      if (name.matches(sectionNameRegex))
      {
        removed.add(sec);
        sections.remove(sec);
        sectionsByName.remove(name);
        romSectionMapByName.remove(name);
        for (long addr = loadMemoryAddress; addr < loadMemoryAddress + size; addr += 2L)
        {
          romSectionMapByAddr.remove(Long.valueOf(addr));
        }
        secIndex--;
      }
    }
    

    return removed;
  }
  
  public void addSection(Section sec)
  {
    if (!sections.contains(sec))
      sections.add(sec);
    sectionsByName.put(name, sec);
  }
  
  public void addSections(Collection<Section> sections)
  {
    for (Section sec : sections)
    {
      addSection(sec);
    }
  }
  
  public void addSymbol(Symbol sym)
  {
    if (!symbols.contains(sym))
      symbols.add(sym);
    symbolsByName.put(name, sym);
    symMap.put(Long.valueOf(address), name);
  }
  
  public void addSymbols(Collection<Symbol> symbols) {
    for (Symbol sym : symbols)
    {
      addSymbol(sym);
    }
  }
  
  public String generateHTMLReport()
  {
    String elfFile = "";
    String devName = "";
    int devID = 0;
    int devREV = 0;
    String dumpToolVer = "";
    String dumpToolFile = "";
    



    String htmlSymbolTable = "";
    



    String htmlHeader = "\n\n<html>\n<head>\n  <style>\n  table {\n    border: 2px solid black;\n    border-collapse: collapse;\n    table-layout: fixed;\n    vertical-align: middle;\n    text-align: right;\n  }\n  tr:hover {\n    background-color: #F5F5F5;\n  }\n  td {\n    border: 1px solid #D0D0D0;\n    min-width: 3em;\n    padding: 5px;\n    vertical-align: middle;\n    font-family: \"Courier New\", \"Lucida Console\";\n    text-align: right;\n  }\n  td.text {\n    text-align: left;\n  }\n  td.num {\n    text-align: right;\n  }\n  td.dimNum {\n    color: #E0E0E0;\n    text-align: right;\n  }\n  td.boldNum {\n    font-weight: bold;\n    text-align: right;\n  }\n  th.vert {\n    background: #D0D0D0;\n    border: 1px solid black;\n    min-height: 4em;\n  }\n  th.horiz {\n    min-width: 7em;\n  }\n  </style>\n</head>";
    














































    String htmlBody = "\n" + String.format("\n<body>", new Object[0]) + String.format("\n<h1>%1$s report</h1>", new Object[] { elfFile }) + String.format("\n<h2>%1$s</h2>", new Object[] { devName }) + String.format("\n<li>Dump tool: %1$s</li>", new Object[] { dumpToolFile }) + String.format("\n<li>Dump tool version: %1$s</li>", new Object[] { dumpToolVer }) + String.format("\n<li>DEVID: 0x%1$06X, DEVREV: 0x%2$06X</li>", new Object[] { Integer.valueOf(devID), Integer.valueOf(devREV) }) + "\n<br/>";
    








    long lastEndAddr = 4096L;
    String htmlSectionTable = "\n" + String.format("\n<p><strong>Number of sections:</strong> %1$d</p>", new Object[] { Integer.valueOf(sections.size()) }) + "\n<p><strong>RAM Sections</strong></p>" + "\n<table>" + "\n<tr>" + "\n  <th class='vert' id='SecLMA'>Load Addr</th>" + "\n  <th class='vert' id='SecByteLen'>Bytes</th>" + "\n  <th class='vert' id='SecName'>Name</th>" + "\n  <th class='vert' id='SecFlags'>Flags</th>" + "\n  <th class='vert' id='SecFlags'>Alignment</th>" + "\n  <th class='vert' id='SecVMA'>Virtual Addr</th>" + "\n  <th class='vert' id='SecID'>ID</th>" + "\n  <th class='vert' id='SecFlags'>Offset</th>" + "</tr>";
    












    for (Section sec : sections)
    {
      if (isRAM)
      {


        htmlSectionTable = htmlSectionTable + "\n<tr>" + String.format("\n  <td class='%1$s'>%2$04X:%3$04X</td>", new Object[] { loadMemoryAddress != lastEndAddr ? "boldNum" : "num", Long.valueOf(loadMemoryAddress), Long.valueOf(loadMemoryAddress + size) }) + String.format("\n  <td class='num'>%1$d</td>", new Object[] { Long.valueOf(size) }) + String.format("\n  <td class='text'>%1$s</td>", new Object[] { name }) + String.format("\n  <td class='text'>%1$s</td>", new Object[] { flags.wholeString }) + String.format("\n  <td class='num'>%1$d</td>", new Object[] { Integer.valueOf(alignment) }) + String.format("\n  <td class='%1$s'>%2$04X:%3$04X</td>", new Object[] { loadMemoryAddress == virtualMemoryAddress ? "dimNum" : "num", Long.valueOf(virtualMemoryAddress), Long.valueOf(virtualMemoryAddress + size) }) + String.format("\n  <td class='num'>%1$3d</td>", new Object[] { Integer.valueOf(id) }) + String.format("\n  <td class='num'>%1$d</td>", new Object[] { Long.valueOf(fileOffset) }) + "\n</tr>";
        








        lastEndAddr = loadMemoryAddress + size;
      } }
    htmlSectionTable = htmlSectionTable + "\n</table>";
    
    lastEndAddr = 0L;
    htmlSectionTable = htmlSectionTable + "\n\n<p><strong>ROM Sections</strong></p>\n<table>\n<tr>\n  <th class='vert' id='SecLMA'>Load Addr</th>\n  <th class='vert' id='SecByteLen'>Size (bytes)</th>\n  <th class='vert' id='SecName'>Name</th>\n  <th class='vert' id='SecFlags'>Flags</th>\n  <th class='vert' id='SecFlags'>Alignment</th>\n  <th class='vert' id='SecVMA'>Virtual Addr</th>\n  <th class='vert' id='SecID'>ID</th>\n  <th class='vert' id='SecFlags'>Offset</th></tr>";
    











    for (Section sec : sections)
    {
      if (isROM)
      {


        htmlSectionTable = htmlSectionTable + "\n<tr>" + String.format("\n  <td class='%1$s'>%2$06X:%3$06X</td>", new Object[] { loadMemoryAddress != lastEndAddr ? "boldNum" : "num", Long.valueOf(loadMemoryAddress), Long.valueOf(loadMemoryAddress + size) }) + String.format("\n  <td class='num'>%1$d</td>", new Object[] { Long.valueOf(size) }) + String.format("\n  <td class='text'>%1$s</td>", new Object[] { name }) + String.format("\n  <td class='text'>%1$s</td>", new Object[] { flags.wholeString }) + String.format("\n  <td class='num'>%1$d</td>", new Object[] { Integer.valueOf(alignment) }) + String.format("\n  <td class='%1$s'>%2$06X:%3$06X</td>", new Object[] { loadMemoryAddress == virtualMemoryAddress ? "dimNum" : "num", Long.valueOf(virtualMemoryAddress), Long.valueOf(virtualMemoryAddress + size) }) + String.format("\n  <td class='num'>%1$3d</td>", new Object[] { Integer.valueOf(id) }) + String.format("\n  <td class='num'>%1$d</td>", new Object[] { Long.valueOf(fileOffset) }) + "\n</tr>";
        








        lastEndAddr = loadMemoryAddress + size;
      } }
    htmlSectionTable = htmlSectionTable + "\n</table>";
    
    lastEndAddr = 0L;
    htmlSectionTable = htmlSectionTable + "\n\n<p><strong>Debugging and other sections</strong></p>\n<table>\n<tr>\n  <th class='vert' id='SecLMA'>Load Addr</th>\n  <th class='vert' id='SecByteLen'>Size (bytes)</th>\n  <th class='vert' id='SecName'>Name</th>\n  <th class='vert' id='SecFlags'>Flags</th>\n  <th class='vert' id='SecFlags'>Alignment</th>\n  <th class='vert' id='SecVMA'>Virtual Addr</th>\n  <th class='vert' id='SecID'>ID</th>\n  <th class='vert' id='SecFlags'>Offset</th></tr>";
    











    for (Section sec : sections)
    {
      if (isDebug)
      {


        htmlSectionTable = htmlSectionTable + "\n<tr>" + String.format("\n  <td class='%1$s'>%2$08X:%3$08X</td>", new Object[] { loadMemoryAddress != lastEndAddr ? "boldNum" : "num", Long.valueOf(loadMemoryAddress), Long.valueOf(loadMemoryAddress + size) }) + String.format("\n  <td class='num'>%1$d</td>", new Object[] { Long.valueOf(size) }) + String.format("\n  <td class='text'>%1$s</td>", new Object[] { name }) + String.format("\n  <td class='text'>%1$s</td>", new Object[] { flags.wholeString }) + String.format("\n  <td class='num'>%1$d</td>", new Object[] { Integer.valueOf(alignment) }) + String.format("\n  <td class='%1$s'>%2$08X:%3$08X</td>", new Object[] { loadMemoryAddress == virtualMemoryAddress ? "dimNum" : "num", Long.valueOf(virtualMemoryAddress), Long.valueOf(virtualMemoryAddress + size) }) + String.format("\n  <td class='num'>%1$3d</td>", new Object[] { Integer.valueOf(id) }) + String.format("\n  <td class='num'>%1$d</td>", new Object[] { Long.valueOf(fileOffset) }) + "\n</tr>";
        








        lastEndAddr = loadMemoryAddress + size;
      } }
    htmlSectionTable = htmlSectionTable + "\n</table>";
    






















    String htmlFooter = "\n</body>\n</html>";
    

    String rep = htmlHeader + htmlBody + htmlSectionTable + htmlSymbolTable + htmlFooter;
    
    return rep;
  }
  
  public List<Symbol> getSymbols()
  {
    return symbols;
  }
  
  public void setSymbols(List<Symbol> symbols)
  {
    this.symbols = symbols;
  }
  
  public List<Section> getSections()
  {
    return sections;
  }
  
  public void setSections(List<Section> sections)
  {
    this.sections = sections;
  }
}
