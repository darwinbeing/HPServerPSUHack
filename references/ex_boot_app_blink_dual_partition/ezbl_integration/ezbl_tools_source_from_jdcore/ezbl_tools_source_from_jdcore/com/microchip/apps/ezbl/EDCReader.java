package com.microchip.apps.ezbl;

import java.io.PrintStream;
import java.io.Serializable;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Field;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;















































public class EDCReader
{
  public static void GroupPICPartNumbers(List<String> partNumberList, DeviceProductLines productClasses, TreeMap<String, Multifunction.StringList> productFamilies)
  {
    for (Field f : productClasses.getClass().getDeclaredFields())
    {
      List<String> classList = null;
      try
      {
        classList = (List)f.get(productClasses);
      }
      catch (IllegalArgumentException|IllegalAccessException ex)
      {
        Logger.getLogger(EDCReader.class.getName()).log(Level.SEVERE, null, ex);
      }
      
      if (classList != null)
      {
        Multifunction.Substitute1IfAllPresent(partNumberList, classList, f.getName() + "*");
      }
    }
    for (Multifunction.StringList partFamily : productFamilies.values())
    {
      Multifunction.Substitute1IfAllPresent(partNumberList, list, topString);
    }
    
    Collections.sort(partNumberList, new PICNameComparator());
  }
  
  public static class BitField
    implements Serializable
  {
    static final long serialVersionUID = 1L;
    String name;
    String desc;
    EDCReader.EZBL_SFR parentSFR;
    List<String> aliases = new ArrayList();
    int position;
    int width;
    int sfrBitmask;
    boolean isHidden;
    
    public BitField() {}
  }
  
  public static class EZBL_SFR implements Comparable<EZBL_SFR>, Serializable {
    static final long serialVersionUID = 1L;
    public String name = null;
    public List<String> aliases = new ArrayList();
    public String desc = null;
    public long addr = 0L;
    public long endAddr = 0L;
    public String srcModule = null;
    public int moduleAddrWidth = 0;
    public int width = 0;
    public int bitsDefined = 0;
    public boolean isHidden = false;
    public boolean isDCR = false;
    public List<EDCReader.BitField> bitfields = new ArrayList();
    public TreeMap<String, EDCReader.BitField> bitfieldByName = new TreeMap();
    public TreeMap<Integer, EDCReader.BitField> bitfieldByPos = new TreeMap();
    public Node xNode = null;
    

    public int compareTo(EZBL_SFR s)
    {
      return addr > addr ? 1 : Objects.equals(Long.valueOf(addr), Long.valueOf(addr)) ? 0 : -1;
    }
    





    public EZBL_SFR() {}
    





    public EZBL_SFR(String[] sfrCSVFields, String[] bitfieldCSVRows)
    {
      int i = 2;
      addr = Long.decode(sfrCSVFields[(i++)]).longValue();
      name = (sfrCSVFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(sfrCSVFields[(i - 1)]));
      desc = (sfrCSVFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(sfrCSVFields[(i - 1)]));
      srcModule = (sfrCSVFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(sfrCSVFields[(i - 1)]));
      








      for (String bfRow : bitfieldCSVRows)
      {
        i = 1;
        String[] bfFields = Multifunction.SimpleSplit(bfRow, ",");
        if (bfFields[(i++)].equals(sfrCSVFields[0]))
        {
          EDCReader.BitField bf = new EDCReader.BitField();
          position = Integer.decode(bfFields[(i++)]).intValue();
          width = Integer.decode(bfFields[(i++)]).intValue();
          name = (bfFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(bfFields[(i - 1)]));
          boolean matchFound = false;
          Iterator i$ = bitfields.iterator(); if (i$.hasNext()) { EDCReader.BitField searchBF = (EDCReader.BitField)i$.next();
            
            if (position == position)
            {
              aliases.add(name);
              matchFound = true;
            }
          }
          
          if (!matchFound)
          {
            parentSFR = this;
            desc = (bfFields[(i++)].isEmpty() ? null : Multifunction.TrimQuotes(bfFields[(i - 1)]));
            isHidden = (!bfFields[(i++)].equals("0"));
            bitfields.add(bf);
            bitfieldByName.put(name, bf);
            bitfieldByPos.put(Integer.valueOf(position), bf);
            bitsDefined |= (int)Math.pow(2.0D, width) - 1 << position;
          }
        }
      }
    }
    
    public EDCReader.BitField findBitfield(String bitFieldName, String parentRegName, String parentModuleName, String desc, Integer position, Integer width, Boolean isHidden, Boolean isDCR) {
      for (EDCReader.BitField bf : bitfields)
      {
        if (((bitFieldName == null) || 
          (name.equals(bitFieldName))) && 
          
          ((parentRegName == null) || 
          (parentSFR.name.equals(parentRegName))) && 
          
          ((parentModuleName == null) || 
          (parentSFR.srcModule.equals(parentModuleName))) && 
          
          ((desc == null) || 
          (desc.toLowerCase().contains(desc.toLowerCase()))) && 
          
          ((position == null) || 
          (position == position.intValue())) && 
          
          ((width == null) || 
          (width == width.intValue())) && 
          
          ((isHidden == null) || 
          (isHidden == isHidden.booleanValue())) && (
          
          (isDCR == null) || 
          (parentSFR.isDCR == isDCR.booleanValue())))
        {

          return bf;
        }
      }
      return null;
    }
    
    public String getSFRStructureHashAsString()
    {
      return Multifunction.bytesToHex(getSFRStructureHash());
    }
    
    public byte[] getSFRStructureHash()
    {
      byte[] computedHash = null;
      


      try
      {
        hashComputer = MessageDigest.getInstance("SHA-256");
        
        hashComputer.update(name.getBytes("UTF-8"));
        hashComputer.update((byte)(isDCR ? 0 : 1));
        hashComputer.update((byte)(isHidden ? 0 : 1));
        hashComputer.update(Integer.toHexString(width).getBytes("UTF-8"));
        hashComputer.update(srcModule.getBytes("UTF-8"));
        
        for (EDCReader.BitField bf : bitfields)
        {
          hashComputer.update(name.getBytes("UTF-8"));
          hashComputer.update(Integer.toHexString(position).getBytes("UTF-8"));
          hashComputer.update(Integer.toHexString(width).getBytes("UTF-8"));
          hashComputer.update((byte)(isHidden ? 0 : 1));
          for (String alias : aliases)
          {
            hashComputer.update(alias.getBytes("UTF-8"));
          }
        }
      }
      catch (NoSuchAlgorithmException ex) {
        MessageDigest hashComputer;
        throw new RuntimeException("ezbl_tools: Cannot find 'SHA-256' hash algorithm. Make sure your JRE includes SHA-256 support.");
      }
      catch (UnsupportedEncodingException ex)
      {
        throw new RuntimeException("ezbl_tools: Cannot decode string as UTF-8.");
      }
      MessageDigest hashComputer;
      computedHash = hashComputer.digest();
      return computedHash;
    }
  }
  
  static class PICDatabase
    implements Serializable
  {
    static final long serialVersionUID = 1L;
    TreeMap<String, EDCReader.EZBL_SFR> regsByName = new TreeMap();
    TreeMap<String, EDCReader.PICDevice> partsByName = new TreeMap();
    TreeMap<String, EDCReader.BitField> bitFields = new TreeMap();
    
    PICDatabase() {}
  }
  
  static class DeviceProductLines implements Serializable
  {
    static final long serialVersionUID = 1L;
    List<String> dsPIC30F = new ArrayList();
    List<String> dsPIC33F = new ArrayList();
    List<String> dsPIC33E = new ArrayList();
    List<String> dsPIC33C = new ArrayList();
    List<String> PIC24FK = new ArrayList();
    List<String> PIC24FJ = new ArrayList();
    List<String> PIC24H = new ArrayList();
    List<String> PIC24E = new ArrayList();
    List<String> generic_16bit = new ArrayList();
    List<String> generic_16bit_da = new ArrayList();
    List<String> generic_16bit_ep = new ArrayList();
    List<String> generic_16dsp_ch = new ArrayList();
    List<String> generic_unknown = new ArrayList();
    
    DeviceProductLines() {}
  }
  
  static class FieldsOnGenericDevice implements Serializable {
    static final long serialVersionUID = 1L;
    List<EDCReader.BitFieldParams> generic_16bit = new ArrayList();
    List<EDCReader.BitFieldParams> generic_16bit_da = new ArrayList();
    List<EDCReader.BitFieldParams> generic_16bit_ep = new ArrayList();
    List<EDCReader.BitFieldParams> generic_16dsp_ch = new ArrayList();
    
    FieldsOnGenericDevice() {}
    
    public void sortByFieldName() { Collections.sort(generic_16bit, new EDCReader.BitFieldParamsNameComparator());
      Collections.sort(generic_16bit_da, new EDCReader.BitFieldParamsNameComparator());
      Collections.sort(generic_16bit_ep, new EDCReader.BitFieldParamsNameComparator());
      Collections.sort(generic_16dsp_ch, new EDCReader.BitFieldParamsNameComparator());
    }
    
    public void sortByAddress()
    {
      Collections.sort(generic_16bit, new EDCReader.BitFieldParamsAddrComparator());
      Collections.sort(generic_16bit_da, new EDCReader.BitFieldParamsAddrComparator());
      Collections.sort(generic_16bit_ep, new EDCReader.BitFieldParamsAddrComparator());
      Collections.sort(generic_16dsp_ch, new EDCReader.BitFieldParamsAddrComparator());
    }
  }
  

  static class SFRList
    implements Serializable
  {
    static final long serialVersionUID = 1L;
    List<EDCReader.EZBL_SFR> list = new ArrayList();
    
    SFRList() {}
  }
  
  static class BitFieldList implements Serializable {
    static final long serialVersionUID = 1L;
    List<EDCReader.BitField> list = new ArrayList();
    
    BitFieldList() {}
  }
  
  public static class DeviceRegs implements Serializable {
    static final long serialVersionUID = 1L;
    TreeMap<Long, EDCReader.EZBL_SFR> regsByAddr = new TreeMap();
    TreeMap<String, EDCReader.EZBL_SFR> regsByName = new TreeMap();
    TreeMap<String, EDCReader.SFRList> regsWithBitfield = new TreeMap();
    TreeMap<String, EDCReader.BitField> allBitFieldsInAllRegs = new TreeMap();
    
    public DeviceRegs() {}
    
    public DeviceRegs addAll(DeviceRegs moreRegs) { if (moreRegs == null) {
        return this;
      }
      allBitFieldsInAllRegs.putAll(allBitFieldsInAllRegs);
      regsByAddr.putAll(regsByAddr);
      regsByName.putAll(regsByName);
      regsWithBitfield.putAll(regsWithBitfield);
      
      return this;
    }
    



    public TreeMap<String, EDCReader.BitField> findBitfield(String bitFieldName, String parentRegName, String parentModuleName, String desc, Integer position, Integer width, Boolean isHidden, Boolean isDCR)
    {
      TreeMap<String, EDCReader.BitField> ret = new TreeMap();
      for (EDCReader.EZBL_SFR sfr : regsByAddr.values())
      {
        EDCReader.BitField possibleMatch = sfr.findBitfield(bitFieldName, parentRegName, parentModuleName, desc, position, width, isHidden, isDCR);
        if (possibleMatch != null)
          ret.put(name, possibleMatch);
      }
      return ret;
    }
    
    public boolean isEmpty()
    {
      return regsByAddr.isEmpty();
    }
  }
  
  public static class PICDevice
    extends EDCReader.DeviceRegs
  {
    static final long serialVersionUID = 1L;
    protected String partNumber = null;
    String supersetPartNumber = null;
    String partClassName = null;
    protected String genericClassName = null;
    long devID = -1L;
    String datasheetID = null;
    String dosID = null;
    EDCReader.Programming2 prog = null;
    
    public PICDevice(String partNumber, EDCReader.DeviceRegs regs)
    {
      setPartNumber(partNumber);
      allBitFieldsInAllRegs = allBitFieldsInAllRegs;
      regsByAddr = regsByAddr;
      regsByName = regsByName;
      regsWithBitfield = regsWithBitfield;
    }
    
    public PICDevice(String partNumber)
    {
      setPartNumber(partNumber);
      genericClassName = (partNumber.startsWith("PIC32") ? partNumber.substring(0, "PIC32MM".length()) : partNumber.startsWith("PIC24") ? partNumber.substring(0, "PIC24F".length()) : partNumber.startsWith("dsPIC") ? partNumber.substring(0, "dsPIC30F".length()) : partNumber);
    }
    


    public PICDevice() {}
    


    public String getPartNumber()
    {
      return partNumber;
    }
    
    public void setPartNumber(String partNumber) {
      this.partNumber = partNumber;
      genericClassName = (partNumber.startsWith("PIC32") ? partNumber.substring(0, "PIC32MM".length()) : partNumber.startsWith("PIC24") ? partNumber.substring(0, "PIC24F".length()) : partNumber.startsWith("dsPIC") ? partNumber.substring(0, "dsPIC30F".length()) : "");
    }
  }
  

  public static class BitFieldParamsNameComparator
    implements Comparator<EDCReader.BitFieldParams>
  {
    public BitFieldParamsNameComparator() {}
    
    public int compare(EDCReader.BitFieldParams x, EDCReader.BitFieldParams y)
    {
      SFRNameComparator c = new SFRNameComparator();
      int ret = c.compare(name, name) * 2;
      if (addr < addr)
      {
        ret /= 2;
      }
      
      return ret;
    }
  }
  
  public static class BitFieldParamsAddrComparator
    implements Comparator<EDCReader.BitFieldParams>
  {
    public BitFieldParamsAddrComparator() {}
    
    public int compare(EDCReader.BitFieldParams x, EDCReader.BitFieldParams y)
    {
      if (addr < addr)
      {
        return -1;
      }
      if (addr == addr)
      {
        return 0;
      }
      return 1;
    }
  }
  
  static class BitFieldParams
    implements Comparable<BitFieldParams>, Serializable
  {
    static final long serialVersionUID = 1L;
    boolean isSFR = false;
    boolean isDCR = false;
    boolean isBitfield = false;
    boolean isDevice = false;
    boolean isGenericDevice = false;
    boolean isEverything = false;
    String name;
    String parentName;
    long addr;
    int position;
    int length;
    int mask;
    List<String> devsWithMatchingAddr = new ArrayList();
    List<String> devsWithMatchingParentAndPosition = new ArrayList();
    List<String> devsWithMatchingAddrAndParentAndPosition = new ArrayList();
    List<String> devsWithMatchingPosition = new ArrayList();
    List<String> containedByGenericDevs = new ArrayList();
    
    BitFieldParams() {}
    
    public void GroupMatchingDevNames(EDCReader.DeviceProductLines productClasses, TreeMap<String, Multifunction.StringList> productFamilies) { EDCReader.GroupPICPartNumbers(devsWithMatchingAddr, productClasses, productFamilies);
      EDCReader.GroupPICPartNumbers(devsWithMatchingParentAndPosition, productClasses, productFamilies);
      EDCReader.GroupPICPartNumbers(devsWithMatchingAddrAndParentAndPosition, productClasses, productFamilies);
      EDCReader.GroupPICPartNumbers(devsWithMatchingPosition, productClasses, productFamilies);
      EDCReader.GroupPICPartNumbers(containedByGenericDevs, productClasses, productFamilies);
    }
    

    public int compareTo(BitFieldParams s)
    {
      if (equals(s))
      {
        return 0;
      }
      if ((isSFR) && (isSFR))
      {
        if (addr < addr)
        {
          return -1;
        }
        if (addr > addr)
        {
          return 1;
        }
        return name.compareTo(name);
      }
      if ((isBitfield) && (isBitfield))
      {
        if ((name.equals(name)) && (parentName.equals(parentName)) && (position == position) && (length == length))
        {
          return 0;
        }
        if (!name.equals(name))
          return name.compareTo(name);
        return 1;
      }
      if (isBitfield)
      {
        return 1;
      }
      if (isSFR)
      {
        return -1;
      }
      
      if (addr < addr)
      {
        return -1;
      }
      if (addr > addr)
      {
        return 1;
      }
      if (position < position)
      {
        return -1;
      }
      if (position > position)
      {
        return 1;
      }
      if (!name.equals(name)) {
        return name.compareTo(name);
      }
      return 1;
    }
  }
  

  public static void GenLEDPatternBoing(String outputFilename)
  {
    byte[] data = new byte['က'];
    int statesPerLED = 4;
    double velocity = 0.0D;
    double position = 8 * statesPerLED;
    double acceleration = -39.24D;
    double fps = 250.0D;
    double deltaTime = fps / data.length;
    double reboundEfficiency = 0.98D;
    
    for (int i = 0; i < data.length; i += 4)
    {
      velocity += acceleration * deltaTime;
      position += velocity * deltaTime;
      if (position <= 0.0D)
      {
        position = 0.0D;
        velocity = -velocity * reboundEfficiency;
      }
      int mainPos = (int)(position / statesPerLED);
      data[i] = ((byte)(1 << (int)(position / statesPerLED)));
      data[(i + 2)] = ((int)((position - 1.0D) / statesPerLED) == mainPos ? data[i] : 0);
      data[(i + 1)] = ((int)((position - 2.0D) / statesPerLED) == mainPos ? data[i] : 0);
      data[(i + 3)] = ((int)((position - 3.0D) / statesPerLED) == mainPos ? data[i] : 0);
    }
    
    Multifunction.UpdateFileIfDataDifferent(outputFilename, data);
  }
  


  public static Node FindAncestorByNodeType(Node startingNode, String ancestorNodeType)
  {
    Node ancestor = startingNode.getParentNode();
    do
    {
      if (ancestor.getNodeName().equals(ancestorNodeType))
      {
        return ancestor;
      }
      
      ancestor = ancestor.getParentNode();
    } while (ancestor != null);
    
    return null;
  }
  










  public EDCReader() {}
  










  public static List<Node> FindChildrenByNodeNames(NodeList startingNodeList, String... nodeNames)
  {
    if (startingNodeList == null) {
      return null;
    }
    List<Node> matchingChildren = new ArrayList();
    for (int i = 0; i < startingNodeList.getLength(); i++)
    {
      Node item = startingNodeList.item(i);
      for (String name : nodeNames)
      {
        matchingChildren.addAll(FindChildrenByNodeNames(item, new String[] { name }));
      }
    }
    return matchingChildren;
  }
  
















  public static List<Node> FindChildrenByNodeNames(Node startingNode, String... nodeNames)
  {
    if (startingNode == null) {
      return null;
    }
    

    List<Node> matchingChildren = new ArrayList();
    
    if (!startingNode.hasChildNodes())
    {
      return matchingChildren;
    }
    
    NodeList children = startingNode.getChildNodes();
    int numChildren = children.getLength();
    for (int i = 0; i < numChildren; i++)
    {
      Node testNode = children.item(i);
      if (testNode.getNodeType() == 1)
      {

        String nodeName = testNode.getNodeName();
        for (String searchName : nodeNames)
        {
          if (nodeName.equals(searchName))
          {
            matchingChildren.add(testNode);
          }
        }
        matchingChildren.addAll(FindChildrenByNodeNames(testNode, nodeNames));
      }
    }
    return matchingChildren;
  }
  


















  public static DeviceRegs IndexSFRDCRNodes(NodeList SFRDefNodes, String... modes)
  {
    boolean allModes = false;
    DeviceRegs dev = new DeviceRegs();
    
    if (SFRDefNodes == null) {
      return null;
    }
    if ((modes == null) || (modes.length == 0))
    {
      modes = new String[1];
      modes[0] = "DS.0";
    }
    else if ((modes.length >= 1) && (modes[0].equals("*"))) {
      allModes = true;
    }
    for (int i = 0; i < SFRDefNodes.getLength(); i++)
    {
      Node item = SFRDefNodes.item(i);
      if (item.getNodeType() == 1)
      {

        if (item.hasAttributes())
        {

          if ((item.getNodeName().equals("edc:SFRDef")) || (item.getNodeName().equals("edc:DCRDef")))
          {

            EZBL_SFR sfr = new EZBL_SFR();
            name = GetAttr(item, "edc:cname", "");
            xNode = item;
            addr = Long.decode(GetAttr(item, "edc:_addr", "-1")).longValue();
            isDCR = (!item.getNodeName().equals("edc:SFRDef"));
            isHidden = Boolean.parseBoolean(GetAttr(item, "edc:ishidden", "false"));
            desc = GetAttr(item, "edc:desc", "");
            srcModule = GetAttr(item, "edc:_modsrc", "");
            width = Integer.decode(GetAttr(item, "edc:nzwidth", "16")).intValue();
            moduleAddrWidth = (width / 8);
            endAddr = (addr + width / 8);
            if ((addr < 8388608L) && (width > 16))
            {
              endAddr = (addr + width / 8 / 3 * 2);
              moduleAddrWidth = (width / 8 / 3 * 2);
            }
            Node temp = item.getAttributes().getNamedItem("edc:_begin");
            Node temp2 = item.getAttributes().getNamedItem("edc:_end");
            if ((temp != null) && (temp2 != null)) {
              moduleAddrWidth = (Integer.decode(temp2.getNodeValue()).intValue() - Integer.decode(temp.getNodeValue()).intValue());
            }
            List<Node> aliases = FindChildrenByNodeNames(item, new String[] { "edc:AliasList" });
            for (int j = 0; j < aliases.size(); j++)
            {
              if (((Node)aliases.get(j)).hasAttributes())
              {
                aliases.add(((Node)aliases.get(j)).getAttributes().getNamedItem("edc:cname").getNodeValue()); }
            }
            List<Node> bitfieldNodes;
            List<Node> bitfieldNodes;
            if (isDCR)
            {
              bitfieldNodes = FindChildrenByNodeNames(item, new String[] { "edc:DCRMode", "edc:DCRFieldDef", "edc:AdjustPoint" });
            }
            else
            {
              bitfieldNodes = FindChildrenByNodeNames(item, new String[] { "edc:SFRMode", "edc:SFRFieldDef", "edc:AdjustPoint" });
              if (!allModes)
              {
                boolean killNodes = false;
                for (int j = 0; j < bitfieldNodes.size(); j++)
                {
                  Node n = (Node)bitfieldNodes.get(j);
                  if (n.getNodeName().equals("edc:SFRMode"))
                  {
                    killNodes = true;
                    for (String m : modes)
                    {
                      String id = GetAttr(n, "edc:id");
                      if ((id == null) || (id.equals(m)))
                      {
                        killNodes = false;
                        break;
                      }
                    }
                  }
                  else if (killNodes) {
                    bitfieldNodes.remove(j--);
                  }
                }
              }
            }
            
            int bitPos = -1;
            for (int j = 0; j < bitfieldNodes.size(); j++)
            {
              Node bfNode = (Node)bitfieldNodes.get(j);
              if (bfNode.getNodeType() == 1)
              {
                if (bfNode.hasAttributes())
                {
                  if ((bfNode.getNodeName().equals("edc:SFRMode")) || (bfNode.getNodeName().equals("edc:DCRMode")))
                  {
                    bitPos = 0;

                  }
                  else if (bfNode.getNodeName().equals("edc:AdjustPoint"))
                  {
                    bitPos += Integer.decode(GetAttr(bfNode, "edc:offset", "1")).intValue();

                  }
                  else if (bfNode.getNodeName().endsWith("FieldDef"))
                  {
                    String localCName = GetAttr(bfNode, "edc:cname");
                    if (localCName != null)
                    {
                      BitField bf = new BitField();
                      name = localCName;
                      parentSFR = sfr;
                      position = bitPos;
                      width = Integer.decode(GetAttr(bfNode, "edc:nzwidth", "16")).intValue();
                      sfrBitmask = ((int)Math.pow(2.0D, width) - 1 << position);
                      desc = GetAttr(bfNode, "edc:desc", "");
                      isHidden = Boolean.parseBoolean(GetAttr(bfNode, "edc:ishidden", "false"));
                      bitfieldByName.put(name, bf);
                      bitfields.add(bf);
                      allBitFieldsInAllRegs.put(name, bf);
                      SFRList sfrsWithThisBitfield = (SFRList)regsWithBitfield.get(name);
                      if (sfrsWithThisBitfield == null)
                        sfrsWithThisBitfield = new SFRList();
                      list.add(sfr);
                      regsWithBitfield.put(name, sfrsWithThisBitfield);
                      bitPos += width;
                    }
                  } } }
            }
            regsByName.put(name, sfr);
            regsByAddr.put(Long.valueOf(addr), sfr);
          } } }
    }
    return dev;
  }
  
  public static DecodedNodeList IndexNodes(NodeList XMLNodes, int childRecurseDepth)
  {
    DecodedNodeList decodedNodes = new DecodedNodeList();
    

    for (int i = 0; i < XMLNodes.getLength(); i++)
    {
      Node xmlNode = XMLNodes.item(i);
      if (xmlNode.getNodeType() == 1)
      {
        list.add(new DecodedNode(xmlNode));
        if ((childRecurseDepth > 0) && (xmlNode.hasChildNodes()))
        {
          decodedNodes.putAll(IndexNodes(xmlNode.getChildNodes(), childRecurseDepth - 1));
        }
      }
    }
    return decodedNodes;
  }
  
  public static LinkedHashMap<String, DecodedNode> IndexNodeChildren(Node xmlNode, boolean recursive)
  {
    if (xmlNode == null) {
      return null;
    }
    LinkedHashMap<String, DecodedNode> nodeListMap = new LinkedHashMap();
    if (!xmlNode.hasChildNodes()) {
      return nodeListMap;
    }
    NodeList children = xmlNode.getChildNodes();
    
    for (int i = 0; i < children.getLength(); i++)
    {
      Node child = children.item(i);
      if (child.getNodeType() == 1)
      {
        DecodedNode dn = new DecodedNode(child);
        if (recursive)
        {
          if (child.hasChildNodes())
          {
            children = IndexNodeChildren(child, recursive);
          }
        }
        nodeListMap.put(child.getNodeName(), dn);
      }
    }
    return nodeListMap;
  }
  
  public static Map<String, EZBL_SFR> IndexSFRNodes(NodeList SFRDefNodes)
  {
    return IndexSFRDCRNodes0regsByName;
  }
  
  public static Node GetAttrNode(Node XMLNode, String attrName)
  {
    return GetAttrNode(XMLNode, attrName, null);
  }
  
  public static Node GetAttrNode(Node XMLNode, String attrName, Node defaultValue)
  {
    if (XMLNode == null) {
      return defaultValue;
    }
    if (attrName == null) {
      attrName = "edc:cname";
    }
    NamedNodeMap nm = XMLNode.getAttributes();
    if (nm == null) {
      return defaultValue;
    }
    Node attrNode = nm.getNamedItem(attrName);
    if (attrNode == null) {
      return defaultValue;
    }
    return attrNode;
  }
  
  public static String GetAttr(Node XMLNode, String attrName)
  {
    return GetAttr(XMLNode, attrName, null);
  }
  
  public static String GetAttr(Node XMLNode, String attrName, String defaultValue)
  {
    if (XMLNode == null) {
      return defaultValue;
    }
    if (attrName == null) {
      attrName = "edc:cname";
    }
    NamedNodeMap nm = XMLNode.getAttributes();
    if (nm == null) {
      return defaultValue;
    }
    Node item = nm.getNamedItem(attrName);
    if (item == null) {
      return defaultValue;
    }
    String nodeVal = item.getNodeValue();
    if (nodeVal == null)
    {
      return defaultValue;
    }
    
    return nodeVal;
  }
  
  public static String FindAttrib(Document doc, String searchString, String attributeName, String defaultValue)
  {
    if (doc == null) {
      return defaultValue;
    }
    Node n = FindNode(doc, searchString);
    if ((n == null) || (!n.hasAttributes()))
      return defaultValue;
    n = n.getAttributes().getNamedItem(attributeName);
    if (n == null) {
      return defaultValue;
    }
    return n.getNodeValue();
  }
  
  public static Long FindAttribAsLong(Document doc, String searchString, String attributeName, Long defaultValue)
  {
    if (doc == null) {
      return defaultValue;
    }
    Node n = FindNode(doc, searchString);
    if ((n == null) || (!n.hasAttributes()))
      return defaultValue;
    n = n.getAttributes().getNamedItem(attributeName);
    if (n == null) {
      return defaultValue;
    }
    try
    {
      return Long.decode(n.getNodeValue());
    }
    catch (NumberFormatException ex) {}
    
    return defaultValue;
  }
  

  public static Integer FindAttribAsInt(Document doc, String searchString, String attributeName, Integer defaultValue)
  {
    if (doc == null) {
      return defaultValue;
    }
    Node n = FindNode(doc, searchString);
    if ((n == null) || (!n.hasAttributes()))
      return defaultValue;
    n = n.getAttributes().getNamedItem(attributeName);
    if (n == null) {
      return defaultValue;
    }
    try
    {
      return Integer.decode(n.getNodeValue());
    }
    catch (NumberFormatException ex) {}
    
    return defaultValue;
  }
  

  public static Boolean FindAttribAsBoolean(Document doc, String searchString, String attributeName, Boolean defaultValue)
  {
    if (doc == null) {
      return defaultValue;
    }
    Node n = FindNode(doc, searchString);
    if ((n == null) || (!n.hasAttributes()))
      return defaultValue;
    n = n.getAttributes().getNamedItem(attributeName);
    if (n == null) {
      return defaultValue;
    }
    return Boolean.valueOf(Boolean.parseBoolean(n.getNodeValue()));
  }
  


  public static Node FindNode(Document doc, String searchString)
  {
    if (doc == null) {
      return null;
    }
    if ((searchString == null) || (searchString.isEmpty())) {
      return doc;
    }
    int nextTokenPos = searchString.indexOf("->");
    String search = nextTokenPos >= 0 ? searchString.substring(0, nextTokenPos) : searchString;
    int attribsStart = search.indexOf('(');
    if (attribsStart >= 0) {
      search = search.substring(0, attribsStart);
    }
    return FindNode(doc.getElementsByTagName(search), searchString);
  }
  


  public static Node FindNode(NodeList startingNode, String searchString)
  {
    if (startingNode == null) {
      return null;
    }
    if ((searchString == null) || (searchString.isEmpty())) {
      return startingNode.item(0);
    }
    int nextTokenPos = searchString.indexOf("->");
    String search = nextTokenPos >= 0 ? searchString.substring(0, nextTokenPos) : searchString;
    
    int attribsStart = search.indexOf('(');
    HashMap<String, String> attribEquals = null;
    if (attribsStart >= 0)
    {
      attribEquals = new HashMap();
      String attribSearchString = search.substring(attribsStart + 1, search.length() - 1);
      search = search.substring(0, attribsStart);
      
      String[] attribSearches = attribSearchString.split("\\,");
      for (int j = 0; j < attribSearches.length; j++)
      {
        int equalIndex = attribSearches[j].indexOf("==");
        if (equalIndex >= 0)
        {
          attribEquals.put(attribSearches[j].substring(0, equalIndex), attribSearches[j].substring(equalIndex + 2, attribSearches[j].length()));
        }
        else {
          attribEquals.put(attribSearches[j], null);
        }
      }
    }
    for (int j = 0; j < startingNode.getLength(); j++)
    {
      Node n = startingNode.item(j);
      if (n.getNodeType() == 1)
      {
        if ((search.equals("*")) || (n.getNodeName().equals(search)))
        {
          if (attribEquals != null)
          {
            if (n.hasAttributes())
            {
              boolean matched = true;
              for (String attr : attribEquals.keySet())
              {
                Node attribNode = n.getAttributes().getNamedItem(attr);
                if (attribNode == null)
                {
                  matched = false;
                  break;
                }
                if (attribEquals.get(attr) != null)
                {
                  if (!attribNode.getNodeValue().equals(attribEquals.get(attr)))
                  {
                    matched = false;
                    break;
                  } }
              }
              if (!matched) {}
            }
          }
          else
          {
            if (nextTokenPos < 0) {
              return n;
            }
            if (n.hasChildNodes())
            {
              Node ret = FindNode(n.getChildNodes(), searchString.substring(nextTokenPos + 2));
              if (ret != null)
                return ret;
            }
          } } } }
    return null;
  }
  
  static class SFRAlternates
    implements Comparable<SFRAlternates>
  {
    Long lowestAddr;
    String name;
    boolean isDCR = false;
    TreeMap<Long, String> addrDevListPairs = new TreeMap();
    
    SFRAlternates() {}
    
    public int compareTo(SFRAlternates o) {
      if ((!isDCR) && (isDCR))
      {
        return 1;
      }
      if ((isDCR) && (!isDCR))
      {
        return -1;
      }
      if (lowestAddr.longValue() < lowestAddr.longValue())
      {
        return -1;
      }
      if ((lowestAddr.longValue() == lowestAddr.longValue()) && (name.equals(name)))
      {
        if (addrDevListPairs.size() < addrDevListPairs.size())
        {
          return -1;
        }
        if (addrDevListPairs.size() == addrDevListPairs.size())
        {
          return 0;
        }
      }
      return 1; } }
  
  public static class Programming2 implements Comparable<Programming2> { static final String tag = "Programming2";
    static final String description = "Parameters needed for flash, config word and other NVM erase/programming";
    String partname;
    String superset;
    long devidaddr;
    long devidmask;
    long devid;
    long nvmconaddr;
    long nvmkeyaddr;
    
    public Programming2() { devidaddr = 16711680L;
      devidmask = 65535L;
      


      nvmkey1 = 85L;
      nvmkey2 = 170L;
      vddmin = 3000;
      vddmax = 3600;
      flashconfig = true;
      mclrlvl = "vdd";
      memories = new ProgrammingMemories();
      nvmops = new TreeMap();
    }
    


    static class MemoryRange
      implements Comparable<MemoryRange>
    {
      static final String tag = "Range";
      

      String cname;
      

      long beginaddr;
      

      long endaddr;
      

      String supports;
      
      boolean hasecc;
      
      boolean otp;
      

      MemoryRange()
      {
        beginaddr = 0L;
        endaddr = 0L;
        
        hasecc = false;
        otp = false;
      }
      
      public String toString() {
        String options = String.format(" edc:hasecc=\"%s\" edc:otp=\"%s\"", new Object[] { Boolean.toString(hasecc), Boolean.toString(otp) });
        return String.format("\n<edc:%s edc:cname=%-18s edc:beginaddr=\"0x%06X\" edc:endaddr=\"0x%06X\" edc:supports=\"%s\"%s/>", new Object[] { "Range", "\"" + cname + "\"", Long.valueOf(beginaddr), Long.valueOf(endaddr), supports, options });
      }
      


      public int compareTo(MemoryRange x)
      {
        if (equals(x))
          return 0;
        if (endaddr == endaddr)
          return (int)(beginaddr - beginaddr);
        if (beginaddr < beginaddr)
          return -1;
        if (beginaddr > beginaddr)
          return 1;
        if (endaddr < endaddr)
          return -1;
        if (endaddr > endaddr)
          return 1;
        int ret = cname.compareTo(cname);
        if (ret != 0)
          return ret;
        ret = supports.compareTo(supports);
        if (ret != 0)
          return ret;
        if ((hasecc == hasecc) && (otp == otp))
          return 0;
        if ((hasecc) && (!hasecc))
          return 1;
        if ((!hasecc) && (hasecc))
          return -1;
        if ((otp) && (!otp))
          return 1;
        if ((!otp) && (otp))
          return -1;
        return -2;
      }
      

      static class MemoryRangeIgnoreAddrComparator
        implements Comparator<EDCReader.Programming2.MemoryRange>
      {
        MemoryRangeIgnoreAddrComparator() {}
        
        public int compare(EDCReader.Programming2.MemoryRange x, EDCReader.Programming2.MemoryRange y)
        {
          if (x.equals(y))
            return 0;
          int ret = cname.compareTo(cname);
          if (ret != 0)
            return ret;
          ret = supports.compareTo(supports);
          if (ret != 0)
            return ret;
          if ((hasecc == hasecc) && (otp == otp))
            return 0;
          if ((hasecc) && (!hasecc))
            return 1;
          if ((!hasecc) && (hasecc))
            return -1;
          if ((otp) && (!otp))
            return 1;
          if ((!otp) && (otp)) {
            return -1;
          }
          return -2;
        }
      }
    }
    
    static class SpecialRange
      extends EDCReader.Programming2.MemoryRange
    {
      static final String tag = "Special";
      String somethingSpecial = "";
      

      public SpecialRange() {}
      

      public SpecialRange(EDCReader.Programming2.MemoryRange mem)
      {
        beginaddr = beginaddr;
        endaddr = endaddr;
        cname = cname;
        hasecc = hasecc;
        otp = otp;
        supports = supports;
      }
      
      public SpecialRange(EDCReader.Programming2.MemoryRange mem, String specialty)
      {
        somethingSpecial = specialty;
        beginaddr = beginaddr;
        endaddr = endaddr;
        cname = cname;
        hasecc = hasecc;
        otp = otp;
        supports = supports;
      }
      

      public String toString()
      {
        String options = String.format(" edc:hasecc=\"%s\" edc:otp=\"%s\" %s", new Object[] { Boolean.toString(hasecc), Boolean.toString(otp), somethingSpecial });
        return String.format("\n<edc:%s edc:cname=%-18s edc:beginaddr=\"0x%06X\" edc:endaddr=\"0x%06X\" edc:supports=\"%s\"%s/>", new Object[] { "Special", "\"" + cname + "\"", Long.valueOf(beginaddr), Long.valueOf(endaddr), supports, options });
      }
    }
    

    public static class ProgrammingMemories
    {
      static final String tag = "Memories";
      List<EDCReader.Programming2.MemoryRange> ranges = new ArrayList();
      List<EDCReader.Programming2.SpecialRange> specialRanges = new ArrayList();
      
      public ProgrammingMemories() {}
      
      public String toString() {
        List<String> ret = new ArrayList();
        
        if (ranges != null)
        {
          for (EDCReader.Programming2.MemoryRange r : ranges)
          {
            ret.add(r.toString());
          }
        }
        if (specialRanges != null)
        {
          for (EDCReader.Programming2.SpecialRange r : specialRanges)
          {
            ret.add(r.toString());
          }
        }
        return "\n<edc:Memories>" + Multifunction.CatStringList(ret).replace("\n", "\n    ") + "\n</edc:" + "Memories" + ">";
      }
    }
    
    public static class NVMOp
      implements Cloneable
    {
      String tag;
      String cname;
      String description;
      int nvmop;
      int nvmopmask;
      int destaddrinc = -1;
      int destbytes = -1;
      int srctype = -1;
      long srcaddr = -1L;
      long durationtyp;
      long durationmax;
      long durationuncalmax;
      
      public NVMOp() {}
      
      public NVMOp clone()
      {
        try {
          return (NVMOp)super.clone();
        }
        catch (CloneNotSupportedException ex) {}
        
        return null;
      }
      




      public String toString()
      {
        String ret = String.format("\n<edc:%s\n    edc:cname=\"%s\"\n    edc:description=\"%s\"\n    edc:nvmop=\"0x%04X\"\n    edc:nvmopmask=\"0x%04X\"", new Object[] { tag, cname, description, Integer.valueOf(nvmop), Integer.valueOf(nvmopmask) });
        




        if (srctype >= 0)
          ret = ret + String.format("\n    edc:srctype=\"%d\"", new Object[] { Integer.valueOf(srctype) });
        if (srcaddr >= 0L)
          ret = ret + String.format("\n    edc:srcaddr=\"0x%X\"", new Object[] { Long.valueOf(srcaddr) });
        if (destaddrinc >= 0)
          ret = ret + String.format("\n    edc:destaddrinc=\"0x%X\"", new Object[] { Integer.valueOf(destaddrinc) });
        if (destbytes >= 0)
          ret = ret + String.format("\n    edc:destbytes=\"%d\"", new Object[] { Integer.valueOf(destbytes) });
        ret = ret + String.format("\n    edc:durationtyp=\"%d\"\n    edc:durationmax=\"%d\"\n    edc:durationuncalmax=\"%d\"/>", new Object[] { Long.valueOf(durationtyp), Long.valueOf(durationmax), Long.valueOf(durationuncalmax) });
        


        return ret;
      }
    }
    

    public String toString()
    {
      return toString(true);
    }
    
    public String toString(boolean includeHelpComment)
    {
      List<String> ret = new ArrayList();
      
      ret.add(String.format("\n<edc:Programming2 edc:partname=\"%s\" edc:superset=\"%s\" edc:devidaddr=\"0x%06X\" edc:devidmask=\"0x%06X\" edc:devid=\"0x%04X\" edc:nvmconaddr=\"0x%04X\" edc:nvmkeyaddr=\"0x%04X\" edc:nvmkey1=\"0x%02X\" edc:nvmkey2=\"0x%02X\" edc:vddmin=\"%d\" edc:vddmax=\"%d\" edc:mclrlvl=\"%s\" edc:description=\"%s\">\n", new Object[] { partname, superset, Long.valueOf(devidaddr), Long.valueOf(devidmask), Long.valueOf(devid), Long.valueOf(nvmconaddr), Long.valueOf(nvmkeyaddr), Long.valueOf(nvmkey1), Long.valueOf(nvmkey2), Integer.valueOf(vddmin), Integer.valueOf(vddmax), mclrlvl, "Parameters needed for flash, config word and other NVM erase/programming" }));
      













      if (includeHelpComment)
        ret.add("    <!--" + "\nNon-overlapping Address ranges where some kind of NVMOP is allowed or will serve some type of purpose (attempted use of a command at an unsupported address outside of this list with matching supports token will yield undefined results; software should assume a default outcome of correct erase/programming, but must test for device reset or other incorrect state afterwards if supporting attempted programming of unspecified address ranges).\n@cname          Canonical name for this program space region. Do not define new names unless the hardware genuinely does do something special in the range and no existing programming algorithms can properly support the address range without a new code implenmentation.\n@beginaddr      First program space address that this memory range is defining, inclusive.\n@endaddr        First legal program space address after this memory range. I.e. this is an exclusive end address type.\n@supports       List of subsequent NVMCON commands for erasing and programming the address range. ICSP tools and bootloaders will choose how to access this address range from the list of supported commands.\n@hasecc         [Optional, default=false] Boolean indicating if the ApplyRange contains ECC protection and therefore can trigger erroneous ECC SEC and DED errors if programmed more than once with non all '1's values.\n@otp            [Optional, default=false] Boolean indicating if this memory range can only be programmed once (such as user OTP).\n\nNVMCON<NVMOP> opcode encodings and properties\n@destaddrinc    Destination address space address increment affected by this NVMOP. Ex: 0x2 for flash word/Config byte on 16-bit, 0x4 for flash double word on 16-bit, 0x8 for flash double word on 32-bit PIC32MM, 0x10 for flash quad word on 32-bit PIC32MX/MZ. This is typically used to increment a software pointer in order to iterate over a memory range and hit every location exacty once. This parameter is required for all NVMOPs that can be steered to a particular destination address and is not mass intrinsically defined parallel erase operation. It should be included for opcodes that do have intrinsic target address effects, but which are small, such as FBOOT.\n@destbytes      Destination address space bytes affected (or number of bytes of non-phantom data from input stream that would be consumed to define the whole destination block.\n@nvpop          Value to write to NVMCON to trigger this NVMOP, not including the WR bit itself.\n@nvmopmask      Bit mask defining which bits of edc:nvmop are important to trigger the NVMOP. This value is used by software to avoid destroying user state bits in NVMCON, such as SFTSWP, power down in idle, etc.\n@srctype        0 = transparent latches at the dest address, 1 = mapped program space latches or NVMDATAx registers (ex: 0xFA0000, 0x7FF800 for slave loading via PSV latches, or 0xBF8023B0 for NVMDATA0 on PIC32M), 2 = RAM (bus mastered read from system RAM with clocking and phantom bytes needed; set srcaddr to base address of the NVMSRCADR SFRs), 3 = Packed RAM (bus mastered read from system RAM with clocking, no phantom bytes and reordered bytes for 3 RAM input words per destination double word; set srcaddr to base address of the NVMSRCADR SFRs).\n@srcaddr        Base address where the NVM controller gets the source data from. Ex: 16-bit devices: \"0\" (transparent latches), \"0xFA0000\", \"0x7FF800\", \"0x0762\" (RAM bus mastered) and on PIC32M: \"0xBF8023B0\" (NVMDATA0) or \"0xBF8023D0\" (NVMSRCADDR for RAM bus mastered).\n@durationtyp    Data sheet typical execution duration at 25C in nanoseconds. This value is required and is used by software to display non-critical user feedback, such as estimated time remaining for programming completion. If unknown, compute this value assuming no FRC error. i.e: 1/(FRC target frequency) * (number of FRC clocks required by the NVM state machine).\n@durationmax    Data sheet maximum limit for highest temperature/slowest FRC execution time in nanoseconds, unless the data sheet is assuming something unfavorably abnormal, like having OSCTUN set to the slowest possible user settable frequency. If the abnormal case exists or this value is unknown, compute this value assuming a -7% slower than specified FRC PERIOD (not frequency). i.e: 1/(FRC frequency) * 0.93 * (number of FRC clocks required by the NVM state machine). This is the duration an ICSP programming tool or software should use when doing open loop algorithm continuation (not polling the NVMCON<WR> bit for operation completion) with ordinary production/calibrated devices. It may also be used for testing for timeout in the event something unexpected happens, like an I/O connection to VDD/VSS/MCLR/PGECx/PGEDx lost during command execution.\n@durationuncalmax Theoretical absolute max duration for execution on an uncalibrated device at highest temperature/slowest FRC in nanoseconds. If unknown, compute this value assuming a -40% slower than typical FRC PERIOD (not frequency). i.e: 1/(FRC frequency) * 0.60 * (number of FRC clocks required by the NVM state machine). This value may be used when programming blind-build/uncalibrated engineering samples during New Product Development cycles. It may also be used as the worst case timeout if the programming tool is explicitly supportive of engineering development or a user explicitly selects a \"safe mode\" to enable use of this timing parameter. NOTE: this should NEVER be used or supported on production programmers outside of a safe mode as deviation this far away from the expected timing case would be highly indicative of a defective device that shouldn't be in customer hands (i.e. high risk of incorrect application operation, which could pose a human safety hazard if allowed to still be programmed and used in an OEM's production line.".replace("\n", "\n        ") + "\n    -->");
      ret.add(memories.toString().replace("\n", "\n    ") + "\n");
      for (NVMOp op : nvmops.values())
      {
        ret.add(op.toString().replace("\n", "\n    "));
      }
      return Multifunction.CatStringList(ret) + "\n</edc:" + "Programming2" + ">";
    }
    

    public static NVMOp cloneOp(NVMOp destOp, NVMOp sourceOp)
    {
      cname = cname;
      destaddrinc = destaddrinc;
      destbytes = destbytes;
      durationmax = durationmax;
      durationtyp = durationtyp;
      durationuncalmax = durationuncalmax;
      nvmop = nvmop;
      nvmopmask = nvmopmask;
      srcaddr = srcaddr;
      srctype = srctype;
      return destOp;
    }
    



    public int compareTo(Programming2 x) { return toString().compareTo(x.toString()); }
    
    long nvmkey1;
    long nvmkey2;
    int vddmin;
    int vddmax;
    boolean flashconfig;
    String mclrlvl;
    ProgrammingMemories memories;
    TreeMap<String, NVMOp> nvmops;
    static final String help = "\nNon-overlapping Address ranges where some kind of NVMOP is allowed or will serve some type of purpose (attempted use of a command at an unsupported address outside of this list with matching supports token will yield undefined results; software should assume a default outcome of correct erase/programming, but must test for device reset or other incorrect state afterwards if supporting attempted programming of unspecified address ranges).\n@cname          Canonical name for this program space region. Do not define new names unless the hardware genuinely does do something special in the range and no existing programming algorithms can properly support the address range without a new code implenmentation.\n@beginaddr      First program space address that this memory range is defining, inclusive.\n@endaddr        First legal program space address after this memory range. I.e. this is an exclusive end address type.\n@supports       List of subsequent NVMCON commands for erasing and programming the address range. ICSP tools and bootloaders will choose how to access this address range from the list of supported commands.\n@hasecc         [Optional, default=false] Boolean indicating if the ApplyRange contains ECC protection and therefore can trigger erroneous ECC SEC and DED errors if programmed more than once with non all '1's values.\n@otp            [Optional, default=false] Boolean indicating if this memory range can only be programmed once (such as user OTP).\n\nNVMCON<NVMOP> opcode encodings and properties\n@destaddrinc    Destination address space address increment affected by this NVMOP. Ex: 0x2 for flash word/Config byte on 16-bit, 0x4 for flash double word on 16-bit, 0x8 for flash double word on 32-bit PIC32MM, 0x10 for flash quad word on 32-bit PIC32MX/MZ. This is typically used to increment a software pointer in order to iterate over a memory range and hit every location exacty once. This parameter is required for all NVMOPs that can be steered to a particular destination address and is not mass intrinsically defined parallel erase operation. It should be included for opcodes that do have intrinsic target address effects, but which are small, such as FBOOT.\n@destbytes      Destination address space bytes affected (or number of bytes of non-phantom data from input stream that would be consumed to define the whole destination block.\n@nvpop          Value to write to NVMCON to trigger this NVMOP, not including the WR bit itself.\n@nvmopmask      Bit mask defining which bits of edc:nvmop are important to trigger the NVMOP. This value is used by software to avoid destroying user state bits in NVMCON, such as SFTSWP, power down in idle, etc.\n@srctype        0 = transparent latches at the dest address, 1 = mapped program space latches or NVMDATAx registers (ex: 0xFA0000, 0x7FF800 for slave loading via PSV latches, or 0xBF8023B0 for NVMDATA0 on PIC32M), 2 = RAM (bus mastered read from system RAM with clocking and phantom bytes needed; set srcaddr to base address of the NVMSRCADR SFRs), 3 = Packed RAM (bus mastered read from system RAM with clocking, no phantom bytes and reordered bytes for 3 RAM input words per destination double word; set srcaddr to base address of the NVMSRCADR SFRs).\n@srcaddr        Base address where the NVM controller gets the source data from. Ex: 16-bit devices: \"0\" (transparent latches), \"0xFA0000\", \"0x7FF800\", \"0x0762\" (RAM bus mastered) and on PIC32M: \"0xBF8023B0\" (NVMDATA0) or \"0xBF8023D0\" (NVMSRCADDR for RAM bus mastered).\n@durationtyp    Data sheet typical execution duration at 25C in nanoseconds. This value is required and is used by software to display non-critical user feedback, such as estimated time remaining for programming completion. If unknown, compute this value assuming no FRC error. i.e: 1/(FRC target frequency) * (number of FRC clocks required by the NVM state machine).\n@durationmax    Data sheet maximum limit for highest temperature/slowest FRC execution time in nanoseconds, unless the data sheet is assuming something unfavorably abnormal, like having OSCTUN set to the slowest possible user settable frequency. If the abnormal case exists or this value is unknown, compute this value assuming a -7% slower than specified FRC PERIOD (not frequency). i.e: 1/(FRC frequency) * 0.93 * (number of FRC clocks required by the NVM state machine). This is the duration an ICSP programming tool or software should use when doing open loop algorithm continuation (not polling the NVMCON<WR> bit for operation completion) with ordinary production/calibrated devices. It may also be used for testing for timeout in the event something unexpected happens, like an I/O connection to VDD/VSS/MCLR/PGECx/PGEDx lost during command execution.\n@durationuncalmax Theoretical absolute max duration for execution on an uncalibrated device at highest temperature/slowest FRC in nanoseconds. If unknown, compute this value assuming a -40% slower than typical FRC PERIOD (not frequency). i.e: 1/(FRC frequency) * 0.60 * (number of FRC clocks required by the NVM state machine). This value may be used when programming blind-build/uncalibrated engineering samples during New Product Development cycles. It may also be used as the worst case timeout if the programming tool is explicitly supportive of engineering development or a user explicitly selects a \"safe mode\" to enable use of this timing parameter. NOTE: this should NEVER be used or supported on production programmers outside of a safe mode as deviation this far away from the expected timing case would be highly indicative of a defective device that shouldn't be in customer hands (i.e. high risk of incorrect application operation, which could pose a human safety hazard if allowed to still be programmed and used in an OEM's production line.";
  }
  
  public static void GenerateDifferentialSFRList(String inputDir, String outputFilename) {
    PICDatabase picDB = new PICDatabase();
    Map<String, Long> devIDMap = new HashMap();
    TreeMap<Long, String> partNumIDMap = new TreeMap();
    TreeMap<String, Multifunction.StringList> partsByDSNum = new TreeMap();
    DeviceProductLines allProducts = new DeviceProductLines();
    

    SQLitePartDatabase sqlDB = new SQLitePartDatabase();
    sqlDB.openDatabase("ezbl_sqlite3_parts.db");
    TreeMap<String, EDCProperties> parts = sqlDB.getParts();
    
    Multifunction.WriteFile(outputFilename, null, false);
    
    for (String fullPartNumber : parts.keySet())
    {
      EDCProperties dbDev = (EDCProperties)parts.get(fullPartNumber);
      PICDevice dev = new PICDevice();
      partNumber = fullPartNumber;
      System.out.println("Processing " + partNumber);
      
      int architectureBits = 16;
      if ((partNumber.startsWith("PIC32")) || (partNumber.startsWith("M")))
      {
        architectureBits = 32;
        partClassName = "PIC32";
      }
      
      if (partNumber.startsWith("dsPIC30"))
      {
        dsPIC30F.add(partNumber);
        partClassName = "dsPIC30";
      }
      else if (partNumber.startsWith("dsPIC33F"))
      {
        dsPIC33F.add(partNumber);
        partClassName = "dsPIC33F";
      }
      else if ((partNumber.startsWith("dsPIC33E")) || (partNumber.startsWith("dsPIC33D")))
      {
        dsPIC33E.add(partNumber);
        partClassName = "dsPIC33E";
      }
      else if (partNumber.startsWith("dsPIC33C"))
      {
        if (partNumber.contains("RA")) {
          continue;
        }
        
        partClassName = "dsPIC33C";
        dsPIC33C.add(partNumber);
      }
      else if (partNumber.startsWith("PIC24E"))
      {
        PIC24E.add(partNumber);
        partClassName = "PIC24E";
      }
      else if (partNumber.startsWith("PIC24H"))
      {
        PIC24H.add(partNumber);
        partClassName = "PIC24H";
      }
      else if (partNumber.startsWith("PIC24F"))
      {
        if (partNumber.contains("K"))
        {
          PIC24FK.add(partNumber);
          partClassName = "PIC24FK";
        }
        else
        {
          PIC24FJ.add(partNumber);
          partClassName = "PIC24FJ";
        }
      } else {
        if (partNumber.startsWith("AC"))
          continue;
      }
      partsByName.put(partNumber, dev);
      
      regsByName = sqlDB.getSFRs(partNumber);
      regsByAddr = sqlDB.getSFRsByAddr(partNumber);
      for (Iterator i$ = regsByAddr.values().iterator(); i$.hasNext();) { sfr = (EZBL_SFR)i$.next();
        
        for (BitField bf : bitfields)
        {
          allBitFieldsInAllRegs.put(name, bf);
          SFRList regsWithBF = (SFRList)regsWithBitfield.get(name);
          if (regsWithBF == null)
          {
            regsWithBF = new SFRList();
          }
          list.add(sfr);
          regsWithBitfield.put(name, regsWithBF);
        } }
      EZBL_SFR sfr;
      datasheetID = String.valueOf(dsNum);
      dosID = String.valueOf(dosNum);
      Multifunction.StringList partsInDS = (Multifunction.StringList)partsByDSNum.get(datasheetID);
      if (partsInDS == null)
      {
        partsInDS = new Multifunction.StringList();
      }
      list.add(partNumber);
      partsByDSNum.put(datasheetID, partsInDS);
      devID = devID;
      devIDMap.put(partNumber, Long.valueOf(devID));
      

      if ((partNumIDMap.get(Long.valueOf(devID)) == null) || (partNumber.length() <= ((String)partNumIDMap.get(Long.valueOf(devID))).length()))
      {
        partNumIDMap.put(Long.valueOf(devID), partNumber);
      }
      























      if (regsByName.containsKey("PSVPAG"))
      {
        genericClassName = "generic-16bit";
        generic_16bit.add(partNumber);
      }
      else if (partNumber.startsWith("PIC24F"))
      {
        genericClassName = "generic-16bit-da";
        generic_16bit_da.add(partNumber);
      }
      else if (partNumber.startsWith("dsPIC33C"))
      {
        genericClassName = "generic-16dsp-ch";
        generic_16dsp_ch.add(partNumber);
      }
      else if ((partNumber.startsWith("dsPIC33E")) || (partNumber.startsWith("PIC24E")))
      {
        genericClassName = "generic-16bit-ep";
        generic_16bit_ep.add(partNumber);
      }
      else
      {
        genericClassName = "generic-unknown";
        generic_unknown.add(partNumber);
      }
      



























































































































      prog = new Programming2();
      prog.devid = devID;
      prog.devidmask = devIDMask;
      prog.devidaddr = devIDAddr;
      prog.mclrlvl = "vdd";
      prog.nvmconaddr = regsByName.get("NVMCON")).addr;
      prog.nvmkeyaddr = regsByName.get("NVMKEY")).addr;
      prog.nvmkey1 = (architectureBits == 16 ? 85L : 2862179925L);
      prog.nvmkey2 = (architectureBits == 16 ? 170L : 1432787370L);
      prog.partname = partNumber;
      prog.vddmax = 3600;
      prog.vddmin = 3000;
      
      EDCReader.Programming2.NVMOp op = new EDCReader.Programming2.NVMOp();
      tag = "EraseChip";
      cname = "Chip Erase";
      description = "Intrinsically targetted erase command affecting all edc:Memories on the device necessary to be considered a blank device (excludes Executive Flash if possible). Ex: User Flash + Auxiliary Flash + FGS Config byte + FAS Config byte, or FBOOT + Active Partition Flash + Inactive Partition Flash.";
      nvmop = 16398;
      nvmopmask = 16399;
      durationtyp = 22900000L;
      durationmax = 24100000L;
      durationuncalmax = 32700000L;
      prog.nvmops.put(tag, op);
      
      op = new EDCReader.Programming2.NVMOp();
      tag = "EraseCourse";
      cname = "Page Erase";
      description = "Largest addressable sized flash erase command supported on the device, generally a Page Erase with all the same values as the edc:EraseFine tag, but alternatively used for faster erase times on Microchip internal flash cells that are row erasable. Durations are in integer nanoseconds.";
      nvmop = (coreType == EZBLState.CPUClass.mm ? 16388 : 16387);
      nvmopmask = 16399;
      destaddrinc = eraseBlockSize;
      destbytes = (architectureBits == 16 ? destaddrinc / 2 * 3 : destaddrinc);
      durationtyp = 22900000L;
      durationmax = 24100000L;
      durationuncalmax = 32700000L;
      prog.nvmops.put(tag, op);
      
      op = ((EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseCourse")).clone();
      tag = "EraseFine";
      description = "Smallest sized flash erase command supported on the device, generally a Page Erase or Row Erase.  Durations are in integer nanoseconds.";
      prog.nvmops.put(tag, op);
      
      op = new EDCReader.Programming2.NVMOp();
      tag = "ProgramCourse";
      cname = "Row Program (64x)";
      description = "Largest sized flash programming command supported on the device for use when best programming speed is desired over addressing granularity. Normally a &quot;row&quot; program NVMCON command. If word/double word/quad word programming is the only supported programming method for the device, this edc:ProgramCourseBlock tag must duplicate the contents from the edc:ProgramFineBlock tag so software can choose one or the other on any device without needing an absolute definition of Fine vs Course.  Durations are in integer nanoseconds.";
      nvmop = 16386;
      nvmopmask = 16399;
      destaddrinc = 128;
      destbytes = (architectureBits == 16 ? destaddrinc / 2 * 3 : destaddrinc);
      srctype = 1;
      srcaddr = 16384000L;
      durationtyp = 1500000L;
      durationmax = 1790000L;
      durationuncalmax = 2150000L;
      if (coreType == EZBLState.CPUClass.mm)
      {
        nvmop = 16387;
        destaddrinc = 256;
        destbytes = destaddrinc;
        cname = "Row Program (64x bus mastered)";
        srctype = 3;
        srcaddr = regsByName.get("NVMSRCADDR")).addr;
      }
      else if ((regsByName.containsKey("NVMSRCADR")) || (regsByName.containsKey("NVMSRCADRL")))
      {
        cname = "Row Program (64x bus mastered)";
        srctype = 2;
        if (regsByName.containsKey("NVMSRCADR")) {
          srcaddr = regsByName.get("NVMSRCADR")).addr;
        } else
          srcaddr = regsByName.get("NVMSRCADRL")).addr;
      }
      prog.nvmops.put(tag, op);
      
      op = new EDCReader.Programming2.NVMOp();
      tag = "ProgramFine";
      cname = "Double-word Program";
      description = "Smallest sized flash programming command supported on the device for use when best addressing granularity is desired over programming speed. Normally a flash word, double word or quad word NVMCON programming command. Durations are in integer nanoseconds.";
      nvmop = 16385;
      nvmopmask = 16399;
      destaddrinc = 4;
      destbytes = (architectureBits == 16 ? destaddrinc / 2 * 3 : destaddrinc);
      srctype = 1;
      srcaddr = 16384000L;
      durationtyp = 48200L;
      durationmax = 57600L;
      durationuncalmax = 68800L;
      if (coreType == EZBLState.CPUClass.mm)
      {
        nvmop = 16386;
        destaddrinc = 8;
        destbytes = (architectureBits == 16 ? destaddrinc / 2 * 3 : destaddrinc);
        srctype = 1;
        srcaddr = regsByName.get("NVMDATA0")).addr;
      }
      prog.nvmops.put(tag, op);
      
      if ((coreType != EZBLState.CPUClass.mm) && (regsByName.containsKey("FICD")))
      {
        if (regsByName.get("FICD")).addr >= 8388608L)
        {
          op = new EDCReader.Programming2.NVMOp();
          tag = "WriteConfig";
          cname = "Config Byte Write";
          description = "Special command specific to writing one Configuration Byte on applicable devices. These Config Bytes behave as a self-erased EEPROM cell and do not require/support an equivalent erase command. Not all bits within the byte may be implemented and their default factory &quot;erased&quot; state generally isn't all '1's";
          nvmop = 16384;
          nvmopmask = 16399;
          destaddrinc = 2;
          destbytes = 1;
          srctype = 1;
          srcaddr = 16384000L;
          durationtyp = 24400000L;
          durationmax = 25700000L;
          durationuncalmax = 34800000L;
          prog.nvmops.put(tag, op);
        }
      }
      
      if (partNumber.startsWith("dsPIC30"))
      {
        prog.flashconfig = false;
        prog.vddmax = 5500;
        prog.vddmin = 4500;
        prog.mclrlvl = "vpp";
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseChip");
        nvmop = 16494;
        nvmopmask = 16511;
        description = "Erase Boot, Secure and General Segments, then erase FBS, FSS and FGS configuration registers. EEPROM and other Config bytes are retained.";
        durationtyp = 4000000L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseFine");
        cname = "Row Erase (32x)";
        nvmop = 16497;
        nvmopmask = 16511;
        destaddrinc = 64;
        destbytes = (destaddrinc / 2 * 3);
        srcaddr = 0L;
        srctype = 0;
        durationtyp = 2000000L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        op = op.clone();
        tag = "EraseEECourse";
        cname = "EEPROM Row Erase (16x)";
        description = "Erase 16 EEPROM words (32 bytes)";
        nvmop = 16501;
        destaddrinc = 32;
        destbytes = destaddrinc;
        prog.nvmops.put(tag, op);
        
        op = op.clone();
        tag = "EraseEEFine";
        cname = "EEPROM Word Erase (1x)";
        description = "Erase 1 EEPROM word (2 bytes)";
        nvmop = 16500;
        destaddrinc = 2;
        destbytes = destaddrinc;
        prog.nvmops.put(tag, op);
        
        op = op.clone();
        tag = "ProgramEECourse";
        cname = "EEPROM Row Program (16x)";
        description = "Program 16 EEPROM words (32 bytes)";
        nvmop = 16389;
        destaddrinc = 32;
        destbytes = destaddrinc;
        prog.nvmops.put(tag, op);
        
        op = op.clone();
        tag = "ProgramEEFine";
        cname = "EEPROM Word Program (1x)";
        description = "Program 1 EEPROM word (2 bytes)";
        nvmop = 16388;
        destaddrinc = 2;
        destbytes = destaddrinc;
        prog.nvmops.put(tag, op);
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramFine");
        cname = "Row Program (32x)";
        description = "Program 32 instruction words at a row boundary (96 bytess)";
        nvmop = 16385;
        srcaddr = 0L;
        srctype = 0;
        destaddrinc = 64;
        destbytes = (destaddrinc / 2 * 3);
        
        Programming2.cloneOp((EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramCourse"), op);
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("WriteConfig");
        nvmopmask = 16392;
        srcaddr = 0L;
        srctype = 0;
        destaddrinc = 2;
        destbytes = 2;
        durationtyp = 4000000L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));

      }
      else if ((partNumber.startsWith("dsPIC33F")) || (partNumber.startsWith("PIC24H")))
      {
        prog.flashconfig = false;
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseChip");
        cname = "Chip Erase";
        nvmop = 16463;
        nvmopmask = 16463;
        durationtyp = 22913432L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseFine");
        cname = "Page Erase";
        nvmop = 16450;
        nvmopmask = 16463;
        destaddrinc = 1024;
        destbytes = (destaddrinc / 2 * 3);
        srcaddr = 0L;
        srctype = 0;
        durationtyp = 22865264L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramFine");
        cname = "Word Program (1x)";
        nvmop = 16387;
        nvmopmask = 16463;
        destaddrinc = 2;
        destbytes = (destaddrinc / 2 * 3);
        srcaddr = 0L;
        srctype = 0;
        durationtyp = 48168L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramCourse");
        cname = "Row Program (64x)";
        nvmop = 16385;
        destaddrinc = 128;
        destbytes = (destaddrinc / 2 * 3);
        durationtyp = 1501221L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        if ((regsByName.get("FICD") == null) || (regsByName.get("FICD")).addr < 8388608L))
        {
          Programming2.cloneOp((EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramCourse"), (EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramFine"));
        }
        else
        {
          op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("WriteConfig");
          nvmopmask = 16463;
          durationtyp = 24366485L;
          durationmax = ((durationtyp * 1.07D));
          durationuncalmax = ((durationtyp * 1.4D));
          srctype = 0;
          srcaddr = 0L;
        }
      }
      else if ((partNumber.startsWith("PIC24FJ")) && (regsByName.get("NVMCON")).bitfieldByName.containsKey("ERASE")))
      {
        prog.vddmin = 2750;
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseChip");
        cname = "Chip Erase";
        nvmop = 16463;
        nvmopmask = 16463;
        durationtyp = 3010L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseFine");
        cname = "Page Erase";
        nvmop = 16450;
        nvmopmask = 16463;
        destaddrinc = 1024;
        destbytes = (destaddrinc / 2 * 3);
        srcaddr = 0L;
        srctype = 0;
        durationtyp = 3000L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        Programming2.cloneOp((EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseCourse"), op);
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramFine");
        cname = "Word Program (1x)";
        nvmop = 16387;
        nvmopmask = 16463;
        destaddrinc = 2;
        destbytes = (destaddrinc / 2 * 3);
        srcaddr = 0L;
        srctype = 0;
        durationtyp = 44375L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        Programming2.cloneOp((EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramCourse"), op);
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramCourse");
        cname = "Row Program (64x)";
        nvmop = 16385;
        destaddrinc = 128;
        destbytes = (destaddrinc / 2 * 3);
        durationtyp = 1383000L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
      }
      else if ((partNumber.startsWith("PIC24F")) && (!partNumber.startsWith("PIC24FJ")))
      {
        prog.flashconfig = false;
        prog.vddmax = 5000;
        prog.vddmin = (partNumber.startsWith("PIC24FV") ? 2700 : 2000);
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseChip");
        nvmop = 16484;
        nvmopmask = 20607;
        description = "Erase Flash and Configuration registers (Programming Executive flash retained)";
        durationtyp = 4000000L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseFine");
        cname = "Row Erase (32x)";
        nvmop = 16472;
        nvmopmask = 20607;
        destaddrinc = 64;
        destbytes = (destaddrinc / 2 * 3);
        srcaddr = 0L;
        srctype = 0;
        durationtyp = 2000000L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        op = Programming2.cloneOp((EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseCourse"), op);
        cname = "Row Erase (128x)";
        nvmop = 16474;
        nvmopmask = 20607;
        srcaddr = 0L;
        srctype = 0;
        destaddrinc *= 4;
        destbytes *= 4;
        durationtyp *= 4L;
        durationmax *= 4L;
        durationuncalmax *= 4L;
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramFine");
        cname = "Row Program (32x)";
        nvmop = 16388;
        nvmopmask = 20607;
        destaddrinc = 64;
        destbytes = (destaddrinc / 2 * 3);
        srcaddr = 0L;
        srctype = 0;
        durationtyp = 2000000L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
        
        Programming2.cloneOp((EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramCourse"), op);
        
        op = ((EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseCourse")).clone();
        tag = "EraseEECourse";
        cname = "EEPROM Row Erase (8x)";
        description = "Erase 8 EEPROM words (16 bytes)";
        nvmop = 16474;
        srcaddr = 0L;
        srctype = 0;
        destaddrinc = 16;
        destbytes = destaddrinc;
        prog.nvmops.put(tag, op);
        
        op = op.clone();
        tag = "EraseEEFine";
        cname = "EEPROM Word Erase (1x)";
        description = "Erase 1 EEPROM word (2 bytes)";
        nvmop = 16472;
        srcaddr = 0L;
        srctype = 0;
        destaddrinc = 2;
        destbytes = destaddrinc;
        prog.nvmops.put(tag, op);
        
        op = op.clone();
        tag = "ProgramEEFine";
        cname = "EEPROM Word Program (1x)";
        description = "Program 1 EEPROM word (2 bytes)";
        nvmop = 16388;
        srcaddr = 0L;
        srctype = 0;
        destaddrinc = 2;
        destbytes = destaddrinc;
        prog.nvmops.put(tag, op);
        
        op = (EDCReader.Programming2.NVMOp)prog.nvmops.get("WriteConfig");
        nvmop = 16388;
        nvmopmask = 20607;
        srcaddr = 0L;
        srctype = 0;
        destbytes = 2;
        destaddrinc = 2;
        durationtyp = 4000000L;
        durationmax = ((durationtyp * 1.07D));
        durationuncalmax = ((durationtyp * 1.4D));
      }
      else if (coreType == EZBLState.CPUClass.e)
      {
        prog.vddmin = 3200;
        if (partNumber.contains("33EV"))
        {
          prog.vddmin = 4500;
          prog.vddmax = 5500;
        }
      }
      
      if (regsByName.containsKey("FBOOT"))
      {

        op = ((EDCReader.Programming2.NVMOp)prog.nvmops.get("ProgramFine")).clone();
        tag = "ProgramFBOOT";
        cname = "Program FBOOT Double-word";
        description = "Special command required to program FBOOT global Flash Configuration Word. Target address is intrinsic.";
        nvmop = 16392;
        prog.nvmops.put(tag, op);
        

        op = ((EDCReader.Programming2.NVMOp)prog.nvmops.get("EraseFine")).clone();
        tag = "EraseInactive";
        cname = "Erase Inactive Partition";
        description = "Special command to erase all of the currently Inactive Parition pages. Target address is intrinsic at 0x4000000+. The absolute identity of the target partition can be evaluated by checking NVMCON<P2ACTIV>.";
        nvmop = 16388;
        prog.nvmops.put(tag, op);
      }
      
      List<MemoryRegion> dbProgSpaces = sqlDB.getMemories(SQLitePartDatabase.MemoryTable.BootloadableRanges, partNumber, null, null);
      for (MemoryRegion mr : dbProgSpaces)
      {
        EDCReader.Programming2.MemoryRange e = new EDCReader.Programming2.MemoryRange();
        cname = name;
        
        if ((coreType == EZBLState.CPUClass.mm) || (coreType == EZBLState.CPUClass.c) || (coreType == EZBLState.CPUClass.b) || (partNumber.startsWith("dsPIC33EV")) || (regsByName.containsKey("ECCCON")) || (regsByName.containsKey("ECCCONL")) || (regsByName.containsKey("ECCCON1")) || (regsByName.containsKey("ECCCON1L")) || (allBitFieldsInAllRegs.containsKey("ECCDBE")) || (allBitFieldsInAllRegs.containsKey("ECCIF")))
        {

          hasecc = true;
        }
        if (name.equalsIgnoreCase("bootcfg"))
        {
          supports = "EraseChip,ProgramFBOOT";
          cname = "FBOOT Config Flash";
          mr.alignToProgSize();
        }
        else if (type == MemoryRegion.MemType.OTP)
        {
          cname = "OTP";
          otp = true;
          supports = "ProgramCourse,ProgramFine";
          mr.alignToProgSize();
        }
        else if (((type == MemoryRegion.MemType.ROM) || (type == MemoryRegion.MemType.FLASHFUSE)) && (partition == MemoryRegion.Partition.single))
        {
          cname = "Primary Flash";
          supports = "EraseChip,EraseCourse,EraseFine,ProgramCourse,ProgramFine";
          mr.alignToEraseSize();
        }
        else if (((type == MemoryRegion.MemType.ROM) || (type == MemoryRegion.MemType.FLASHFUSE)) && (partition == MemoryRegion.Partition.partition1))
        {
          cname = "Active Partition Flash";
          supports = "EraseChip,EraseInactive,EraseCourse,EraseFine,ProgramCourse,ProgramFine";
          mr.alignToEraseSize();
        }
        else if (((type == MemoryRegion.MemType.ROM) || (type == MemoryRegion.MemType.FLASHFUSE)) && (partition == MemoryRegion.Partition.partition2))
        {
          cname = "Inactive Partition Flash";
          supports = "EraseChip,EraseInactive,EraseCourse,EraseFine,ProgramCourse,ProgramFine";
          mr.alignToEraseSize();
        }
        else if (type == MemoryRegion.MemType.EEPROM)
        {
          cname = "EEPROM";
          supports = "EraseEECourse,EraseEEFine,ProgramEEFine";
          mr.alignToProgSize();
        }
        else if (name.equals("auxflash"))
        {
          cname = "Auxiliary Flash";
          supports = "EraseChip,EraseAux,EraseFine,EraseCourse,ProgramCourse,ProgramFine";
          mr.alignToEraseSize();
        }
        else if (type == MemoryRegion.MemType.TEST)
        {
          if (partNumber.startsWith("PIC24F04"))
            continue;
          cname = "Executive Flash";
          supports = "EraseCourse,EraseFine,ProgramCourse,ProgramFine";
          mr.alignToProgSize();
        }
        else if (type == MemoryRegion.MemType.BYTEFUSE)
        {
          cname = "Config Bytes";
          supports = "WriteConfig";
          mr.alignToProgSize();
        }
        else if (type == MemoryRegion.MemType.FLASHFUSE)
        {
          supports = "EraseChip,EraseCourse,EraseFine,ProgramCourse,ProgramFine";
        }
        beginaddr = startAddr;
        endaddr = endAddr;
        
        prog.memories.ranges.add(e);
      }
      






































































































































      Collections.sort(prog.memories.ranges);
      for (int i = 0; i < prog.memories.ranges.size(); i++)
      {
        EDCReader.Programming2.MemoryRange mem = (EDCReader.Programming2.MemoryRange)prog.memories.ranges.get(i);
        for (int j = i + 1; j < prog.memories.ranges.size(); j++)
        {
          EDCReader.Programming2.MemoryRange mem2 = (EDCReader.Programming2.MemoryRange)prog.memories.ranges.get(j);
          if (new EDCReader.Programming2.MemoryRange.MemoryRangeIgnoreAddrComparator().compare(mem, mem2) != 0)
            break;
          if ((beginaddr <= endaddr) && (endaddr >= beginaddr))
          {
            beginaddr = (beginaddr > beginaddr ? beginaddr : beginaddr);
            endaddr = (endaddr < endaddr ? endaddr : endaddr);
            prog.memories.ranges.remove(j--);
          }
        }
      }
      

      String outData = "\n\n\n<!-- http://www.microchip.com/" + partNumber + " DS number: " + datasheetID + " -->" + prog.toString(partNumber.equals(parts.firstKey()));
      Multifunction.WriteFile(outputFilename, outData.getBytes(), true);
      regsByAddr = null;
      regsByName = null;
      regsWithBitfield = null;
    }
    

    System.out.printf("Devices Families:\n", new Object[0]);
    for (Multifunction.StringList dsPartList : partsByDSNum.values())
    {
      dsPartList.sortPartNumbers();
      topString = ((String)list.get(0) + " family");
      System.out.printf("  %s\n", new Object[] { list.get(0) });
    }
  }
  







































































































































































































  public static int GeneratePICAbstractionHeader(String partNumber, String outputFilename)
  {
    List<String> hStrings = new ArrayList();
    List<String> asmStrings = new ArrayList();
    List<PairWithText> RPOutEncodings = new ArrayList();
    List<PairWithText> pinNumberFunctionMappings = new ArrayList();
    int ret = 0;
    Document doc = null;
    
    doc = PICXMLLoader.LoadPICXML("C:/Program Files (x86)/Microchip/MPLABX/vDefault/", partNumber);
    
    if (doc == null)
    {
      System.err.printf("Could not load crownking EDC file for %1$s\n", new Object[] { partNumber });
      System.exit(-1);
    }
    
    NodeList sfrNodes = doc.getElementsByTagName("edc:SFRDef");
    NodeList dcrNodes = doc.getElementsByTagName("edc:DCRDef");
    Map<String, EZBL_SFR> SFRMap = IndexSFRNodes(sfrNodes);
    Map<String, EZBL_SFR> DCRMap = IndexSFRNodes(dcrNodes);
    List<EZBL_SFR> sortedSFRs = new ArrayList();
    sortedSFRs.addAll(SFRMap.values());
    Collections.sort(sortedSFRs);
    List<EZBL_SFR> sortedDCRs = new ArrayList();
    sortedSFRs.addAll(DCRMap.values());
    Collections.sort(sortedDCRs);
    
    for (Iterator i$ = sortedSFRs.iterator(); i$.hasNext();) { sfr = (EZBL_SFR)i$.next();
      
      hStrings.add(String.format("\n#define ADDR_%1$-15s 0x%2$04X // to 0x%3$04X; Module: %4$s", new Object[] { name, Long.valueOf(addr), Long.valueOf(endAddr), srcModule.replace(".Module", "") }));
      asmStrings.add(String.format("\n_%1$-15s = 0x%2$04X", new Object[] { name, Long.valueOf(addr) }));
      asmStrings.add(String.format("\n\t.global   _%1$s", new Object[] { name }));
      for (BitField bf : bitfieldByName.values())
      {
        String macroName = name + "_" + name + "_POS";
        hStrings.add(String.format("\n#define %1$-20s %2$-2d     // to %3$2d", new Object[] { macroName, Integer.valueOf(position), Integer.valueOf(position + width) }));
      } }
    EZBL_SFR sfr;
    for (Iterator i$ = sortedDCRs.iterator(); i$.hasNext();) { sfr = (EZBL_SFR)i$.next();
      
      hStrings.add(String.format("\n#define ADDR_%1$-15s 0x%2$06X   // to 0x%3$06X; Module: %4$s", new Object[] { name, Long.valueOf(addr), Long.valueOf(endAddr), srcModule.replace(".Module", "") }));
      for (BitField bf : bitfieldByName.values())
      {
        String macroName = name + "_" + name + "_POS";
        hStrings.add(String.format("\n#define %1$-20s %2$-2d     // to %3$2d; %4$s", new Object[] { macroName, Integer.valueOf(position), Integer.valueOf(position + width), desc }));
      }
    }
    
    EZBL_SFR sfr;
    NodeList pinLists = doc.getElementsByTagName("edc:PinList");
    if (pinLists.getLength() != 0)
    {
      NodeList pinList = pinLists.item(0).getChildNodes();
      
      int pinNumber = 0;
      for (int i = 0; i < pinList.getLength(); i++)
      {
        Node pin = pinList.item(i);
        if (pin.getNodeName().equals("edc:Pin"))
        {


          pinNumber++;
          int functionNumber = 0;
          NodeList virtualPins = pin.getChildNodes();
          if (virtualPins.getLength() != 0)
          {


            for (int j = 0; j < virtualPins.getLength(); j++)
            {
              Node virtualPin = virtualPins.item(j);
              

              if (virtualPin.getNodeName().equals("edc:VirtualPin"))
              {


                functionNumber++;
                String funcMneumonic = virtualPin.getAttributes().getNamedItem("edc:name").getNodeValue();
                pinNumberFunctionMappings.add(new PairWithText(pinNumber, functionNumber, funcMneumonic));
              }
            } }
        }
      }
    }
    NodeList virtualPins = doc.getElementsByTagName("edc:VirtualPin");
    for (int i = 0; i < virtualPins.getLength(); i++)
    {
      Node n = virtualPins.item(i);
      if (n.hasAttributes())
      {
        NamedNodeMap attr = n.getAttributes();
        Node name = attr.getNamedItem("edc:name");
        Node RPOutVal = attr.getNamedItem("edc:ppsval");
        if ((name != null) && (RPOutVal != null))
        {
          RPOutEncodings.add(new PairWithText(Long.decode(RPOutVal.getNodeValue()).longValue(), name.getNodeValue()));
        }
      }
    }
    
    String fileOutContents = "// THIS IS AN AUTO-GENERATED FILE!\n// Created for: " + partNumber + "\n" + "\n#ifndef PIC_ABSTRACTION_H" + "\n#define PIC_ABSTRACTION_H" + "\n\n" + Multifunction.CatStringList(hStrings) + "\n" + "\n" + "\n // I/O pins and their named function mappings" + "\n //     Pin Function     Pin Number             All pin functions";
    








    for (int i = 0; i < pinNumberFunctionMappings.size(); i++)
    {
      PairWithText mapping = (PairWithText)pinNumberFunctionMappings.get(i);
      String allFunctionComment = "";
      for (PairWithText mapping2 : pinNumberFunctionMappings)
      {
        if (first == first)
        {


          if ((!mapping.equals(mapping2)) && (text.equals(text)))
          {
            text += String.format("_AT_%1$d", new Object[] { Long.valueOf(first) });
          }
          allFunctionComment = allFunctionComment + text + "/";
        } }
      allFunctionComment = allFunctionComment.substring(0, allFunctionComment.length() - 1);
      text = text.replace("+", "p");
      text = text.replace("-", "n");
      fileOutContents = fileOutContents + String.format("\n#define %1$-16s %2$3d\t// Pin %2$3d: %3$s", new Object[] { text, Long.valueOf(first), allFunctionComment });
    }
    
    fileOutContents = fileOutContents + "\n\n\n // Remappable Peripheral Output encodings";
    

    for (PairWithText mapping : RPOutEncodings)
    {
      fileOutContents = fileOutContents + String.format("\n#define RPOUT_%1$-16s 0x%2$02Xu", new Object[] { text, Long.valueOf(first) });
    }
    
    fileOutContents = fileOutContents + "\n\n#endif  // #ifndef PIC_ABSTRACTION_H\n";
    


    Multifunction.UpdateFileIfDataDifferent(outputFilename, fileOutContents, true);
    
    return ret;
  }
}
