package com.microchip.apps.ezbl;

import java.io.File;
import java.io.PrintStream;
import java.security.DigestException;
import java.security.GeneralSecurityException;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;
import java.util.zip.CRC32;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.SecretKeySpec;





















public class Bl2b
{
  boolean architecture16Bit;
  List<DataRecord> records = null;
  int[] bootIDHash = new int[4];
  int appIDVerBuild = 0;
  int appIDVerMinor = 0;
  int appIDVerMajor = 0;
  String encryptPassword = null;
  

  public Bl2b()
  {
    records = new ArrayList();
    architecture16Bit = false;
    encryptPassword = null;
  }
  
  public Bl2b(boolean architecture16Bit)
  {
    records = new ArrayList();
    this.architecture16Bit = architecture16Bit;
  }
  
  public Bl2b(Blob v1Blob, int[] bootIDHash, int appIDVerMajor, int appIDVerMinor, int appIDVerBuild)
  {
    records = records;
    architecture16Bit = architecture16Bit;
    if ((bootIDHash != null) && (bootIDHash.length >= 5))
    {
      int copyCount = this.bootIDHash.length > bootIDHash.length ? bootIDHash.length : this.bootIDHash.length;
      System.arraycopy(bootIDHash, 0, this.bootIDHash, 0, copyCount);
    }
    this.appIDVerBuild = appIDVerBuild;
    this.appIDVerMinor = appIDVerMinor;
    this.appIDVerMajor = appIDVerMajor;
  }
  












  public Bl2b(byte[] bl2Bytes, String encryptionPassword)
  {
    boolean invalidFile = false;
    
    if (bl2Bytes.length < 100)
    {
      invalidFile = true;
      throw new RuntimeException("Invalid bl2; length too small");
    }
    String firstBytes = new String(bl2Bytes, 0, 64);
    int BL2BIndex = firstBytes.indexOf("BL2B");
    invalidFile = BL2BIndex < 0;
    
    encryptPassword = encryptionPassword;
    architecture16Bit = false;
    records = new ArrayList();
    int index = 64;
    if (BL2BIndex >= 0)
    {
      index = BL2BIndex + 48;
    }
    while (index < bl2Bytes.length - 64 - 4 - 32)
    {
      DataRecord record = new DataRecord(architecture16Bit);
      

      int recordByteLen = bl2Bytes[(index++)] & 0xFF;
      recordByteLen = (int)(recordByteLen | (bl2Bytes[(index++)] & 0xFF) << 8);
      recordByteLen = (int)(recordByteLen | (bl2Bytes[(index++)] & 0xFF) << 16);
      recordByteLen = (int)(recordByteLen | (bl2Bytes[(index++)] & 0xFF) << 24);
      address = (bl2Bytes[(index++)] & 0xFF);
      address |= (bl2Bytes[(index++)] & 0xFF) << 8;
      address |= (bl2Bytes[(index++)] & 0xFF) << 16;
      address |= (bl2Bytes[(index++)] & 0xFF) << 24;
      if ((invalidFile) && ((recordByteLen > 32768) || (recordByteLen < 0)))
      {
        recordByteLen = 32768;
      }
      data = Arrays.copyOfRange(bl2Bytes, index, index + recordByteLen);
      records.add(record);
      index += recordByteLen;
    }
    for (int i = 0; i < records.size(); i++)
    {
      DataRecord dr = (DataRecord)records.get(i);
      if ((data.length % 3 == 0) && ((address & 1L) == 0L) && (data.length != 0) && (address < 16777216L))
      {
        architecture16Bit = true;
      }
      if (i < records.size() - 1)
      {
        if ((address + data.length / 3 * 2 == records.get(i + 1)).address) && (architecture16Bit == true))
        {
          architecture16Bit = true;
          break;
        }
      }
    }
    if (!architecture16Bit)
    {
      boolean allRecords16Bit = true;
      for (DataRecord dr : records)
      {
        if (!architecture16Bit)
        {
          allRecords16Bit = false;
          break;
        }
      }
      architecture16Bit = allRecords16Bit;
    }
    for (DataRecord dr : records)
    {
      architecture16Bit = architecture16Bit;
    }
  }
  

  public static Bl2b GetBl2bFromFile(String artifactFilename)
  {
    byte[] fileData = Multifunction.ReadFileBinary(artifactFilename);
    if (fileData == null)
    {
      return null;
    }
    return new Bl2b(fileData, null);
  }
  
  public Bl2b(String hexFileContents)
  {
    architecture16Bit = false;
    records = Blob.ParseHex(hexFileContents, false);
    

    DataRecord.CoalesceRecords(records);
    

    if (Blob.TestRecordsLookLike16Bit(records))
    {
      architecture16Bit = true;
      for (DataRecord dr : records)
      {
        architecture16Bit = true;
      }
      

      RemoveMCU16PhantomBytesAndAddresses();
    }
  }
  

  private static enum HexDecoderState
  {
    COLON, 
    LENGTH0, 
    LENGTH1, 
    ADDRESS0, 
    ADDRESS1, 
    ADDRESS2, 
    ADDRESS3, 
    TYPE0, 
    TYPE1, 
    DATA0, 
    DATA1, 
    CHECK0, 
    CHECK1;
    
    private HexDecoderState() {}
  }
  
  static String HexToReadable(String hexFilePath) {
    HexDecoderState state = HexDecoderState.COLON;
    long decoderAddress = 0L;long hexHEX386Address = 0L;
    int hexRecordType = 0;
    int hexRecordAddress = 0;
    int hexRecordLen = 0;
    int hexRecordDataIndex = 0;
    int hexRecordChecksum = 0;int computedRecordChecksum = 0;
    int lineNumber = 0;
    int lineChecksumListIndex = 0;
    byte[] hexRecordData = new byte['Ā'];
    

    List<String> humanText = new ArrayList();
    
    byte[] hexFileContents = Multifunction.ReadFileBinary(hexFilePath);
    if (hexFileContents == null)
    {
      return "    Error: could not read input file \"" + hexFilePath + "\\n";
    }
    
    humanText.add("Line Num  Line Bytes  Rec Address  Record Type     Checksum  Computed Addr  Record Data\n-----------------------------------------------------------------------------------------------------------------");
    


    for (int inIndex = 0; inIndex < hexFileContents.length; tmp834_830++)
    {

      byte b = hexFileContents[inIndex];
      

      if ((b == 32) || (b == 13) || (b == 10) || (b == 9))
      {
        if (b == 10)
        {
          lineNumber++;
        }
        if (state != HexDecoderState.COLON)
        {
          humanText.add("\n    Error: whitespace detected within a record\n");

        }
        

      }
      else if (state == HexDecoderState.COLON)
      {
        if (b != 58)
        {
          humanText.add("\n    Error: unknown character encountered\n");
        }
        else {
          humanText.add("\n");
          state = HexDecoderState.LENGTH0;
        }
      }
      else
      {
        if ((b >= 48) && (b <= 57))
        {
          b = (byte)(b - 48);
        }
        else if ((b >= 97) && (b <= 102))
        {
          b = (byte)(b - 97 + 10);
        }
        else if ((b >= 65) && (b <= 70))
        {
          b = (byte)(b - 65 + 10);
        }
        else
        {
          humanText.add("\n    Error: unknown character encountered\n");
          continue;
        }
        

        switch (1.$SwitchMap$com$microchip$apps$ezbl$Bl2b$HexDecoderState[state.ordinal()])
        {
        case 1: 
          hexRecordLen = b & 0xF;
          state = HexDecoderState.LENGTH1;
          break;
        case 2: 
          hexRecordLen <<= 4;
          hexRecordLen |= b & 0xF;
          computedRecordChecksum = computedRecordChecksum + hexRecordLen & 0xFF;
          humanText.add(String.format("%1$8d      %2$6d  ", new Object[] { Integer.valueOf(lineNumber), Integer.valueOf(hexRecordLen) }));
          state = HexDecoderState.ADDRESS0;
          break;
        
        case 3: 
          hexRecordAddress = b & 0xF;
          state = HexDecoderState.ADDRESS1;
          break;
        case 4: 
          hexRecordAddress <<= 4;
          hexRecordAddress |= b & 0xF;
          computedRecordChecksum = computedRecordChecksum + hexRecordAddress & 0xFF;
          state = HexDecoderState.ADDRESS2;
          break;
        case 5: 
          hexRecordAddress <<= 4;
          hexRecordAddress |= b & 0xF;
          state = HexDecoderState.ADDRESS3;
          break;
        case 6: 
          hexRecordAddress <<= 4;
          hexRecordAddress |= b & 0xF;
          computedRecordChecksum = computedRecordChecksum + hexRecordAddress & 0xFF;
          humanText.add(String.format("0x%1$04X       ", new Object[] { Integer.valueOf(hexRecordAddress) }));
          state = HexDecoderState.TYPE0;
          break;
        
        case 7: 
          hexRecordType = b & 0xF;
          state = HexDecoderState.TYPE1;
          break;
        case 8: 
          hexRecordType <<= 4;
          hexRecordType |= b & 0xF;
          humanText.add(String.format("%1$X=%2$s  ", new Object[] { Integer.valueOf(hexRecordType), hexRecordType == 5 ? "St Lin Addr " : hexRecordType == 4 ? "Ext Lin Addr" : hexRecordType == 3 ? "St Seg Addr " : hexRecordType == 2 ? "Ext Seg Addr" : hexRecordType == 1 ? "EOF         " : hexRecordType == 0 ? "Data        " : "Illegal     " }));
          





          lineChecksumListIndex = humanText.size();
          
          hexRecordDataIndex = 0;
          computedRecordChecksum = computedRecordChecksum + hexRecordType & 0xFF;
          if (hexRecordType == 0)
          {
            humanText.add(String.format("0x%1$08X     ", new Object[] { Long.valueOf((decoderAddress & 0xFFFF0000 | hexRecordAddress) >> 1) }));
          }
          else if (hexRecordType != 4)
          {



            humanText.add("               ");
          }
          
          state = HexDecoderState.DATA0;
          if (hexRecordLen == 0)
          {
            state = HexDecoderState.CHECK0;
          }
          
          break;
        case 9: 
          hexRecordData[hexRecordDataIndex] = b;
          state = HexDecoderState.DATA1;
          break;
        case 10: 
          int tmp834_832 = hexRecordDataIndex; byte[] tmp834_830 = hexRecordData;tmp834_830[tmp834_832] = ((byte)(tmp834_830[tmp834_832] << 4)); int 
            tmp844_842 = hexRecordDataIndex; byte[] tmp844_840 = hexRecordData;tmp844_840[tmp844_842] = ((byte)(tmp844_840[tmp844_842] | b));
          tmp834_832.add(String.format("%1$02X", new Object[] { Byte.valueOf(hexRecordData[hexRecordDataIndex]) }));
          computedRecordChecksum = computedRecordChecksum + (hexRecordData[hexRecordDataIndex] & 0xFF) & 0xFF;
          hexRecordDataIndex++;
          if (hexRecordDataIndex % 4 == 0)
          {
            tmp834_832.add(" ");
          }
          state = HexDecoderState.DATA0;
          if (hexRecordDataIndex == hexRecordLen)
          {
            state = HexDecoderState.CHECK0;
          }
          
          break;
        case 11: 
          hexRecordChecksum = b & 0xF;
          state = HexDecoderState.CHECK1;
          break;
        case 12: 
          hexRecordChecksum <<= 4;
          hexRecordChecksum |= b & 0xF;
          computedRecordChecksum = -computedRecordChecksum & 0xFF;
          tmp834_832.add(lineChecksumListIndex, String.format("0x%1$02X %2$5s", new Object[] { Integer.valueOf(hexRecordChecksum), hexRecordChecksum == computedRecordChecksum ? "" : "(BAD)" }));
          computedRecordChecksum = 0;
          state = HexDecoderState.COLON;
          

          switch (hexRecordType)
          {

          case 0: 
            decoderAddress = decoderAddress & 0xFFFF0000 | decoderAddress + hexRecordLen & 0xFFFF;
            break;
          
          case 1: 
            if (tmp834_830 + 12 < hexFileContents.length)
            {
              tmp834_832.add("\n    Error: found EOF record with more data after it\n");
            }
            if (hexRecordLen != 0)
            {
              tmp834_832.add("\n    Error: found EOF record with data in it\n");
            }
            
            break;
          case 2: 
            hexHEX386Address = (hexRecordData[0] & 0xFF) << 12 | (hexRecordData[1] & 0xFF) << 4;
            if (hexRecordLen != 2)
            {
              tmp834_832.add("\n    Error: found extra data in an Extended Segment Address Record\n");
            }
            
            break;
          case 4: 
            decoderAddress = decoderAddress & 0xFFFF | (hexRecordData[0] & 0xFF) << 24 | (hexRecordData[1] & 0xFF) << 16;
            tmp834_832.add(lineChecksumListIndex + 1, String.format("0x%1$04X----     ", new Object[] { Long.valueOf((decoderAddress & 0xFFFF0000) >> 17) }));
            
            if (hexRecordLen != 2)
            {
              tmp834_832.add("\n    Error: found extra data in an Extended Linear Address Record\n");
            }
            
            break;
          case 5: 
            tmp834_832.add("\n    Error: Start Linear Address Records not supported\n");
            break;
          case 3: 
          default: 
            tmp834_832.add("\n    Error: unrecognized record type; skipping record\n");
          }
          
          hexRecordDataIndex = 0;
        }
        
      }
    }
    if (state != HexDecoderState.COLON)
    {
      tmp834_832.add("\n    Error: ran out of data in the middle of a record\n");
    }
    if (hexRecordType != 1)
    {
      tmp834_832.add("\n    Error: missing EOF record\n");
    }
    
    return Multifunction.CatStringList(tmp834_832);
  }
  












  public static byte[] EncryptBl2Bytes(EZBLState state, byte[] bl2Bytes)
  {
    byte[] counterBytes = new byte[16];
    


    byte[] encryptedBytes = new byte[bl2Bytes.length];
    


    System.arraycopy(bl2Bytes, 0, encryptedBytes, 0, encryptedBytes.length);
    

    System.arraycopy(encryptedBytes, 8, counterBytes, 4, 12);
    

    try
    {
      Cipher aes = Cipher.getInstance("AES/ECB/NoPadding");
      SecretKeySpec key = new SecretKeySpec(encryptionKey, "AES");
      aes.init(1, key);
    }
    catch (NoSuchAlgorithmException|NoSuchPaddingException|InvalidKeyException ex)
    {
      return null; }
    SecretKeySpec key;
    Cipher aes;
    long blobIndex = 28L;
    int blockIndex = (int)(blobIndex / 16L);
    int blockOffset = (int)(blobIndex % 16L);
    while (blockIndex * 16 < encryptedBytes.length)
    {


      counterBytes[0] = ((byte)blockIndex);
      counterBytes[1] = ((byte)(blockIndex >> 8));
      counterBytes[2] = ((byte)(blockIndex >> 16));
      counterBytes[3] = ((byte)(blockIndex >> 24));
      try
      {
        keySteamBytes = aes.doFinal(counterBytes);
      }
      catch (IllegalBlockSizeException|BadPaddingException ex) {
        byte[] keySteamBytes;
        return null;
      }
      
      byte[] keySteamBytes;
      
      int chunkSize = keySteamBytes.length;
      if (blockIndex * 16 + blockOffset + chunkSize > encryptedBytes.length)
      {
        chunkSize = encryptedBytes.length - blockIndex * 16 - blockOffset;
      }
      
      for (int i = blockOffset; i < chunkSize; i++)
      {
        int tmp211_210 = (blockIndex * 16 + i); byte[] tmp211_201 = encryptedBytes;tmp211_201[tmp211_210] = ((byte)(tmp211_201[tmp211_210] ^ keySteamBytes[i]));
      }
      blockIndex++;
      blockOffset = 0;
    }
    






    CRC32 myCRC = new CRC32();
    myCRC.update(encryptedBytes, 0, encryptedBytes.length - 4);
    long crcValue = myCRC.getValue();
    encryptedBytes[(encryptedBytes.length - 4)] = ((byte)(int)(crcValue & 0xFF));
    encryptedBytes[(encryptedBytes.length - 3)] = ((byte)(int)(crcValue >> 8 & 0xFF));
    encryptedBytes[(encryptedBytes.length - 2)] = ((byte)(int)(crcValue >> 16 & 0xFF));
    encryptedBytes[(encryptedBytes.length - 1)] = ((byte)(int)(crcValue >> 24 & 0xFF));
    
    return encryptedBytes;
  }
  




  public byte[] GetHash()
  {
    byte[] data = GetBytes();
    return Arrays.copyOfRange(data, data.length - 4 - 32, data.length - 4);
  }
  







  public void AddData(long address, byte[] data)
  {
    DataRecord newData = new DataRecord(address, data, architecture16Bit);
    AddressRange range = new AddressRange(address, newData.getEndAddress());
    
    RemoveIgnoreRange(range);
    records.add(newData);
    DataRecord.CoalesceRecords(records);
  }
  









  public Map<Integer, Integer> GetData()
  {
    Map<Integer, Integer> ret = new HashMap(4096);
    
    for (DataRecord dr : records)
    {




      int address = (int)address;
      if (!dr.isArchitecture16Bit())
      {
        for (int i = 0; i < data.length - 4; address += 4)
        {
          int data = data[(i++)] & 0xFF | (data[(i++)] & 0xFF) << 8 | (data[(i++)] & 0xFF) << 16 | (data[(i++)] & 0xFF) << 24;
          


          ret.put(Integer.valueOf(address), Integer.valueOf(data));
        }
        if (data.length - i == 3)
        {
          int data = data[(i++)] & 0xFF | (data[(i++)] & 0xFF) << 8 | (data[(i++)] & 0xFF) << 16;
          

          ret.put(Integer.valueOf(address), Integer.valueOf(data));
        }
        else if (data.length - i == 2)
        {
          int data = data[(i++)] & 0xFF | (data[(i++)] & 0xFF) << 8;
          
          ret.put(Integer.valueOf(address), Integer.valueOf(data));
        }
        else if (data.length - i == 1)
        {
          int data = data[(i++)] & 0xFF;
          ret.put(Integer.valueOf(address), Integer.valueOf(data));
        }
      }
      else
      {
        for (int i = 0; i < data.length - 3;)
        {
          int data = data[(i++)] & 0xFF | (data[(i++)] & 0xFF) << 8 | (data[(i++)] & 0xFF) << 16;
          

          ret.put(Integer.valueOf(address), Integer.valueOf(data));
          address += 2;
        }
        if (data.length - i == 2)
        {
          int data = data[(i++)] & 0xFF | (data[(i++)] & 0xFF) << 8;
          
          ret.put(Integer.valueOf(address), Integer.valueOf(data));

        }
        else if (data.length - i == 1)
        {
          int data = data[(i++)] & 0xFF;
          ret.put(Integer.valueOf(address), Integer.valueOf(data));
        }
      }
    }
    
    return ret;
  }
  







  public void FillData(MemoryRegion range, byte fillChar)
  {
    long startAddr = architecture16Bit ? startAddr : startAddr & 0x1FFFFFFF;
    long endAddr = architecture16Bit ? endAddr : endAddr & 0x1FFFFFFF;
    

    DataRecord newData = new DataRecord(architecture16Bit);
    address = startAddr;
    data = new byte[newData.getDataIndexOfAddress(endAddr)];
    Arrays.fill(data, fillChar);
    Bl2b tempBlob = new Bl2b(architecture16Bit);
    records.add(newData);
    


    List<AddressRange> conflicts = new ArrayList();
    for (int i = 0; i < records.size(); i++)
    {
      DataRecord rec = (DataRecord)records.get(i);
      if ((address < endAddr) && (rec.getEndAddress() > startAddr))
      {


        conflicts.add(rec.getAddressRange());
      }
    }
    
    tempBlob.RemoveIgnoreRanges(conflicts);
    

    records.addAll(records);
    

    DataRecord.CoalesceRecords(records);
  }
  





















  public byte[] GetBytes(String headerSYNCstring)
  {
    if (headerSYNCstring == null)
    {
      headerSYNCstring = "UUUUUUUUMCUPHCME";
    }
    

    int index = 0;
    ListIterator<DataRecord> iterator = records.listIterator();
    while (iterator.hasNext())
    {
      DataRecord record = (DataRecord)iterator.next();
      index += data.length + 8;
    }
    

    byte[] retData = new byte[headerSYNCstring.length() + 48 + index + 32 + 4];
    

    System.arraycopy(headerSYNCstring.getBytes(), 0, retData, 0, headerSYNCstring.length());
    index = headerSYNCstring.length();
    
    retData[(index++)] = 66;
    retData[(index++)] = 76;
    retData[(index++)] = 50;
    retData[(index++)] = 66;
    retData[(index++)] = ((byte)(retData.length - 16 - 8 & 0xFF));
    retData[(index++)] = ((byte)(retData.length - 16 - 8 >> 8 & 0xFF));
    retData[(index++)] = ((byte)(retData.length - 16 - 8 >> 16 & 0xFF));
    retData[(index++)] = ((byte)(retData.length - 16 - 8 >> 24 & 0xFF));
    

    for (int i = 0; i < 4; i++)
    {
      retData[(index++)] = ((byte)(bootIDHash[i] & 0xFF));
      retData[(index++)] = ((byte)(bootIDHash[i] >> 8 & 0xFF));
      retData[(index++)] = ((byte)(bootIDHash[i] >> 16 & 0xFF));
      retData[(index++)] = ((byte)(bootIDHash[i] >> 24 & 0xFF));
    }
    

    retData[(index++)] = ((byte)appIDVerBuild);
    retData[(index++)] = ((byte)(appIDVerBuild >> 8));
    retData[(index++)] = ((byte)(appIDVerBuild >> 16));
    retData[(index++)] = ((byte)(appIDVerBuild >> 24));
    retData[(index++)] = ((byte)appIDVerMinor);
    retData[(index++)] = ((byte)(appIDVerMinor >> 8));
    retData[(index++)] = ((byte)appIDVerMajor);
    retData[(index++)] = ((byte)(appIDVerMajor >> 8));
    

    Arrays.fill(retData, index, index + 16, (byte)0);
    index += 16;
    

    iterator = records.listIterator();
    while (iterator.hasNext())
    {

      DataRecord record = (DataRecord)iterator.next();
      

      retData[(index++)] = ((byte)(int)(data.length & 0xFF));
      retData[(index++)] = ((byte)(int)((data.length & 0xFF00) >> 8));
      retData[(index++)] = ((byte)(int)((data.length & 0xFF0000) >> 16));
      retData[(index++)] = ((byte)(int)((data.length & 0xFF000000) >> 24));
      retData[(index++)] = ((byte)(int)(address & 0xFF));
      retData[(index++)] = ((byte)(int)((address & 0xFF00) >> 8));
      retData[(index++)] = ((byte)(int)((address & 0xFF0000) >> 16));
      retData[(index++)] = ((byte)(int)((address & 0xFF000000) >> 24));
      

      System.arraycopy(data, 0, retData, index, data.length);
      index += data.length;
    }
    

    try
    {
      MessageDigest hashComputer = MessageDigest.getInstance("SHA-256");
      hashComputer.update(retData, 16, retData.length - 16 - 4 - 32);
      hashComputer.digest(retData, retData.length - 4 - 32, 32);
    }
    catch (NoSuchAlgorithmException ex)
    {
      System.err.println("ezbl_tools: Cannot find 'SHA-256' hash algorithm. Make sure your JRE includes SHA-256 support. Output hash will be set to all zeros.");
    }
    catch (DigestException ex)
    {
      System.err.printf("ezbl_tools: SHA-256 exception: %1$s\n", new Object[] { ex.getMessage() });
    }
    index += 32;
    







    CRC32 myCRC = new CRC32();
    myCRC.update(retData, 16, retData.length - 16 - 4);
    long crcValue = myCRC.getValue();
    retData[(retData.length - 4)] = ((byte)(int)(crcValue & 0xFF));
    retData[(retData.length - 3)] = ((byte)(int)(crcValue >> 8 & 0xFF));
    retData[(retData.length - 2)] = ((byte)(int)(crcValue >> 16 & 0xFF));
    retData[(retData.length - 1)] = ((byte)(int)(crcValue >> 24 & 0xFF));
    
    return retData;
  }
  








  public byte[] GetBytes()
  {
    return GetBytes(null);
  }
  









  public byte[] GetBinBytes()
  {
    int byteLen = 0;
    for (DataRecord r : records)
    {
      if (data != null)
      {


        byteLen += data.length;
      }
    }
    
    byte[] retData = new byte[byteLen];
    int index = 0;
    

    for (DataRecord r : records)
    {
      if (data != null)
      {


        System.arraycopy(data, 0, retData, index, data.length);
        index += data.length;
      }
    }
    return retData;
  }
  









  public String GetObjDumpView()
  {
    return GetObjDumpView(GetBytes());
  }
  












  public static String GetObjDumpView(String hexFileContents)
  {
    return GetObjDumpView(new Bl2b(hexFileContents).GetBytes());
  }
  












  public static String GetObjDumpView(byte[] fileBytes)
  {
    int recordCount = 0;
    int[] bootID = new int[4];
    


    List<String> output = new ArrayList();
    



    if (fileBytes == null)
    {
      return null;
    }
    
    if (fileBytes.length < 100)
    {
      output.add(String.format("  ERROR: BL2 file length too short to be a valid .bl2 file. Length = %1$d bytes, minimum is %2$d bytes.\n", new Object[] { Integer.valueOf(fileBytes.length), Integer.valueOf(100) }));
    }
    int index = 0;
    if (fileBytes.length >= 16)
    {
      index = 0;
      byte[] field = Arrays.copyOfRange(fileBytes, index, index + 16);
      String fieldStr = new String(field);
      String expect = "UUUUUUUUMCUPHCME";
      output.add(String.format("SYNC - Auto-baud and synchronization/bootloader wake-up bytes:\n  " + fieldStr + " (%1$02X %2$02X %3$02X %4$02X %5$02X %6$02X %7$02X %8$02X %9$02X %10$02X %11$02X %12$02X %13$02X %14$02X %15$02X %16$02X)\n", new Object[] { Byte.valueOf(field[0]), Byte.valueOf(field[1]), Byte.valueOf(field[2]), Byte.valueOf(field[3]), Byte.valueOf(field[4]), Byte.valueOf(field[5]), Byte.valueOf(field[6]), Byte.valueOf(field[7]), Byte.valueOf(field[8]), Byte.valueOf(field[9]), Byte.valueOf(field[10]), Byte.valueOf(field[11]), Byte.valueOf(field[12]), Byte.valueOf(field[13]), Byte.valueOf(field[14]), Byte.valueOf(field[15]) }));
      if (!fieldStr.equals(expect))
      {
        output.add("  ERROR: expected: " + expect + "\n");
      }
    }
    if (fileBytes.length >= 20)
    {
      index = 16;
      byte[] field = Arrays.copyOfRange(fileBytes, index, index + 4);
      String fieldStr = new String(field);
      String expect = "BL2B";
      output.add(String.format("\nFILE_ID - File type:\n  0x%1$02X%2$02X%3$02X%4$02X (\"" + fieldStr + "\") %5$s\n", new Object[] { Byte.valueOf(field[3]), Byte.valueOf(field[2]), Byte.valueOf(field[1]), Byte.valueOf(field[0]), fieldStr.equals("BL2B") ? "(Blob version 2)" : "" }));
      if (!fieldStr.equals(expect))
      {
        output.add("  ERROR: expected: " + expect + "\n");
      }
    }
    if (fileBytes.length >= 24)
    {
      index = 20;
      long size = fileBytes[(index++)] & 0xFF;
      size |= fileBytes[(index++)] << 8 & 0xFF00;
      size |= fileBytes[(index++)] << 16 & 0xFF0000;
      size |= fileBytes[(index++)] << 24 & 0xFF000000;
      output.add(String.format("\nFILE_LEN - File size from BOOTID_HASH to EOF:\n  %1$d bytes (%2$d byte file total)\n", new Object[] { Long.valueOf(size), Long.valueOf(size + 16L + 8L) }));
      if ((size != fileBytes.length - 16 - 8) || (size + 16L + 8L != fileBytes.length))
      {
        output.add(String.format("  ERROR: expected: %1$d bytes (%2$d bytes total)\n", new Object[] { Integer.valueOf(fileBytes.length - 16 - 8), Integer.valueOf(fileBytes.length) }));
      }
    }
    if (fileBytes.length >= 40)
    {
      index = 24;
      for (int i = 0; i < bootID.length; i++)
      {
        bootID[i] = (fileBytes[(index++)] & 0xFF | (fileBytes[(index++)] & 0xFF) << 8 | (fileBytes[(index++)] & 0xFF) << 16 | (fileBytes[(index++)] & 0xFF) << 24);
      }
      output.add(String.format("\nBOOTID_HASH - Right-most 16 bytes of SHA-256 hash of BOOTID_* strings:\n  %1$08X%2$08X%3$08X%4$08X %5$s\n", new Object[] { Integer.valueOf(bootID[3]), Integer.valueOf(bootID[2]), Integer.valueOf(bootID[1]), Integer.valueOf(bootID[0]), (bootID[3] | bootID[2] | bootID[1] | bootID[0]) == 0 ? "(image not generated from .elf?)" : "" }));
    }
    if (fileBytes.length >= 48)
    {
      index = 40;
      int appIDVerBuild = fileBytes[(index++)] & 0xFF | (fileBytes[(index++)] & 0xFF) << 8 | (fileBytes[(index++)] & 0xFF) << 16 | (fileBytes[(index++)] & 0xFF) << 24;
      int appIDVerMinor = fileBytes[(index++)] & 0xFF | (fileBytes[(index++)] & 0xFF) << 8;
      int appIDVerMajor = fileBytes[(index++)] & 0xFF | (fileBytes[(index++)] & 0xFF) << 8;
      output.add(String.format("\nAPPID_VER - Application version identifier:\n  %1$d.%2$02d.%3$04d\n", new Object[] { Integer.valueOf(appIDVerMajor), Integer.valueOf(appIDVerMinor), Integer.valueOf(appIDVerBuild) }));
    }
    if (fileBytes.length >= 64)
    {
      index = 48;
      byte[] field = Arrays.copyOfRange(fileBytes, index, index + 16);
      byte sumOR = 0;
      output.add(String.format("\nHMAC_SHA_256 - (future use):\n  ", new Object[0]));
      for (int i = 0; i < 16; i += 4)
      {
        output.add(String.format("%1$02X%2$02X%3$02X%4$02X", new Object[] { Byte.valueOf(field[i]), Byte.valueOf(field[(i + 1)]), Byte.valueOf(field[(i + 2)]), Byte.valueOf(field[(i + 3)]) }));
        sumOR = (byte)(sumOR | field[i] | field[(i + 1)] | field[(i + 2)] | field[(i + 3)]);
      }
      if (sumOR == 0)
      {
        output.add(sumOR == 0 ? " (file not encrypted)\n" : "\n");
      }
    }
    int recordCountInsertIndex = output.size();
    

    boolean architecture16Bit = true;
    int searchByteIndex = 64;
    while (searchByteIndex < fileBytes.length - 32 - 4)
    {



      if (searchByteIndex + 8 > fileBytes.length - 32 - 4) {
        break;
      }
      
      long recordLength = fileBytes[(searchByteIndex++)] & 0xFF;
      recordLength |= (fileBytes[(searchByteIndex++)] & 0xFF) << 8;
      recordLength |= (fileBytes[(searchByteIndex++)] & 0xFF) << 16;
      recordLength |= (fileBytes[(searchByteIndex++)] & 0xFF) << 24;
      long recordAddress = fileBytes[(searchByteIndex++)] & 0xFF;
      recordAddress |= (fileBytes[(searchByteIndex++)] & 0xFF) << 8;
      recordAddress |= (fileBytes[(searchByteIndex++)] & 0xFF) << 16;
      recordAddress |= (fileBytes[(searchByteIndex++)] & 0xFF) << 24;
      
      if (((recordAddress & 0xFFFFFFFFFF000001) != 0L) || (recordLength % 3L != 0L))
      {
        architecture16Bit = false;
        break;
      }
      searchByteIndex = (int)(searchByteIndex + recordLength);
    }
    
    output.add("Starting record decoding in " + (architecture16Bit ? "16-bit" : "8-bit/32-bit") + " architecture mode\n\n");
    
    index = 64;
    while (index < fileBytes.length - 32 - 4)
    {




      if (index + 8 > fileBytes.length - 32 - 4)
      {
        output.add(String.format("  ERROR: found %1$d bytes at the end of the file. This is too small to be a record header.\n", new Object[] { Integer.valueOf(fileBytes.length - 32 - 4 - index) }));
        output.add("    Bytes are: ");
        for (int i = 0; i < fileBytes.length - 32 - 4 - index; i++)
        {
          output.add(String.format("%1$02X", new Object[] { Byte.valueOf(fileBytes[(index + i)]) }));
        }
        output.add("\n");
        break;
      }
      
      long recordLength = fileBytes[(index++)] & 0xFF;
      recordLength |= (fileBytes[(index++)] & 0xFF) << 8;
      recordLength |= (fileBytes[(index++)] & 0xFF) << 16;
      recordLength |= (fileBytes[(index++)] & 0xFF) << 24;
      
      long recordAddress = fileBytes[(index++)] & 0xFF;
      recordAddress |= (fileBytes[(index++)] & 0xFF) << 8;
      recordAddress |= (fileBytes[(index++)] & 0xFF) << 16;
      recordAddress |= (fileBytes[(index++)] & 0xFF) << 24;
      
      recordCount++;
      
      output.add(String.format("DATA_REC_HDR.REC_LEN  - bytes in record     : 0x%1$08X (%1$d data bytes)\n", new Object[] { Long.valueOf(recordLength) }));
      output.add(String.format("DATA_REC_HDR.REC_ADDR - record start address: 0x%1$08X (to 0x%2$08X)\n", new Object[] { Long.valueOf(recordAddress), Long.valueOf(recordAddress + (architecture16Bit ? (recordLength + 2L) / 3L * 2L : recordLength)) }));
      if ((int)recordLength + index > fileBytes.length - 32 - 4)
      {
        output.add("  ERROR: record length indicator extends into hash/CRC area\n");
        recordLength = fileBytes.length - 32 - 4 - index;
      }
      else if (recordLength < 0L)
      {
        output.add("  ERROR: record length is invalid (computed record end address will be wrong)\n");
        recordLength = fileBytes.length - 32 - 4 - index;
      }
      
      if (architecture16Bit)
      {
        int lineByteIndex = 0;
        
        while (recordLength > 0L)
        {
          if (lineByteIndex == 0)
          {
            long printAddr = recordAddress;
            if (((recordAddress & 0x7) != 0L) && (recordLength >= 12L))
            {
              printAddr = recordAddress & 0xFFFFFFFFFFFFFFF8;
            }
            output.add(String.format("%1$06X  ", new Object[] { Long.valueOf(printAddr) }));
            
            if (((recordAddress & 0x7) != 0L) && (recordLength >= 12L))
            {
              for (printAddr = recordAddress & 0xFFFFFFFFFFFFFFF9; printAddr != recordAddress; printAddr += 2L)
              {
                output.add("       ");
                lineByteIndex += 3;
              }
              if ((recordAddress & 1L) != 0L)
              {
                recordAddress += 1L;
                if (recordLength >= 2L)
                {
                  output.add(String.format("%2$02X%1$02X   ", new Object[] { Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]) }));
                  lineByteIndex += 3;
                  recordLength -= 2L;
                }
                else
                {
                  output.add(String.format("  %1$02X   ", new Object[] { Byte.valueOf(fileBytes[(index++)]) }));
                  lineByteIndex += 3;
                  recordLength -= 1L;
                }
              }
            }
          }
          if (recordLength >= 3L)
          {
            output.add(String.format("%3$02X%2$02X%1$02X ", new Object[] { Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]) }));
            lineByteIndex += 3;
            recordAddress += 2L;
            recordLength -= 3L;
          }
          else if (recordLength == 2L)
          {
            output.add(String.format("  %2$02X%1$02X ", new Object[] { Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]) }));
            lineByteIndex += 3;
            recordAddress += 2L;
            recordLength -= 2L;
          }
          else if (recordLength == 1L)
          {
            output.add(String.format("    %1$02X ", new Object[] { Byte.valueOf(fileBytes[(index++)]) }));
            lineByteIndex += 3;
            recordAddress += 2L;
            recordLength -= 1L;
          }
          if ((lineByteIndex >= 12) || (recordLength <= 0L))
          {
            output.add("\n");
            lineByteIndex = 0;
          }
        }
      }
      else
      {
        for (int i = 0; i + 16 <= (int)recordLength; i += 16)
        {
          output.add(String.format("%1$08X  ", new Object[] { Long.valueOf(recordAddress) }));
          output.add(String.format("%4$02X%3$02X%2$02X%1$02X %8$02X%7$02X%6$02X%5$02X %12$02X%11$02X%10$02X%9$02X %16$02X%15$02X%14$02X%13$02X\n", new Object[] { Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]) }));
          recordAddress += 16L;
        }
        if (i < recordLength)
        {
          output.add(String.format("%1$08X  ", new Object[] { Long.valueOf(recordAddress) }));
          for (; i + 4 <= (int)recordLength; i += 4)
          {
            output.add(String.format("%4$02X%3$02X%2$02X%1$02X ", new Object[] { Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]) }));
          }
          if ((int)recordLength - i == 3)
          {
            output.add(String.format("  %3$02X%2$02X%1$02X", new Object[] { Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]) }));
          }
          else if ((int)recordLength - i == 2)
          {
            output.add(String.format("    %2$02X%1$02X", new Object[] { Byte.valueOf(fileBytes[(index++)]), Byte.valueOf(fileBytes[(index++)]) }));
          }
          else if ((int)recordLength - i == 1)
          {
            output.add(String.format("      %1$02X", new Object[] { Byte.valueOf(fileBytes[(index++)]) }));
          }
          
          output.add("\n");
        }
      }
      output.add("\n");
    }
    
    output.add(recordCountInsertIndex, String.format("\n\nScanning found DATA_RECORDs: %1$d\n", new Object[] { Integer.valueOf(recordCount) }));
    
    if (fileBytes.length >= 96)
    {
      index = fileBytes.length - 4 - 32;
      byte[] field = Arrays.copyOfRange(fileBytes, index, index + 32);
      byte[] computedHash = null;
      


      try
      {
        MessageDigest hashComputer = MessageDigest.getInstance("SHA-256");
        hashComputer.update(fileBytes, 16, fileBytes.length - 16 - 32 - 4);
        computedHash = hashComputer.digest();
      }
      catch (NoSuchAlgorithmException ex)
      {
        output.add("ezbl_tools: Cannot find 'SHA-256' hash algorithm. Make sure your JRE includes SHA-256 support. Cannot test hash.");
      }
      
      output.add(String.format("\n\nFILE_HASH - SHA-256 hash from FILE_ID field to last DATA_RECORD byte:\n  ", new Object[0]));
      for (int i = 0; i < 32; i += 4)
      {
        output.add(String.format("%1$02X%2$02X%3$02X%4$02X", new Object[] { Byte.valueOf(field[i]), Byte.valueOf(field[(i + 1)]), Byte.valueOf(field[(i + 2)]), Byte.valueOf(field[(i + 3)]) }));
      }
      boolean match = Arrays.equals(field, computedHash);
      output.add(String.format(" %1$s (computed over file offset 16 to %2$d)\n", new Object[] { match ? "(correct)" : "(INVALID)", Integer.valueOf(fileBytes.length - 16 - 32 - 4) }));
      if (!match)
      {
        output.add("\n  ERROR: expected: ");
        for (int i = 0; i < 32; i += 4)
        {
          output.add(String.format("%1$02X%2$02X%3$02X%4$02X", new Object[] { Byte.valueOf(computedHash[i]), Byte.valueOf(computedHash[(i + 1)]), Byte.valueOf(computedHash[(i + 2)]), Byte.valueOf(computedHash[(i + 3)]) }));
        }
      }
    }
    if (fileBytes.length >= 100)
    {

      index = fileBytes.length - 4;
      byte[] field = Arrays.copyOfRange(fileBytes, index, index + 4);
      long fieldCRC = field[0] & 0xFF | (field[1] & 0xFF) << 8 | (field[2] & 0xFF) << 16 | (field[3] & 0xFF) << 24;
      CRC32 myCRC = new CRC32();
      myCRC.update(fileBytes, 16, fileBytes.length - 16 - 4);
      long crcValue = myCRC.getValue();
      boolean match = crcValue == fieldCRC;
      output.add(String.format("\nCRC32 - CRC32 from FILE_ID to last FILE_HASH byte:\n  0x%1$02X%2$02X%3$02X%4$02X %5$s (computed over file offset 16 to %6$d)\n", new Object[] { Byte.valueOf(field[3]), Byte.valueOf(field[2]), Byte.valueOf(field[1]), Byte.valueOf(field[0]), match ? "(correct)" : "INVALID", Integer.valueOf(fileBytes.length - 16 - 4) }));
      if (!match)
      {
        output.add(String.format("  ERROR: expected: 0x%1$08X\n", new Object[] { Long.valueOf(crcValue) }));
      }
    }
    output.add("\nEnd of file/decoding");
    
    String ret = Multifunction.CatStringList(output).replace(" \n", "\n");
    if (File.separatorChar == '\\')
    {
      ret = ret.replace("\n", "\r\n");
    }
    return ret;
  }
  








  static long BytesToMCU16Addresses(long lengthInBytes)
  {
    long ret = (lengthInBytes * 2L + 2L) / 3L;
    if ((ret & 1L) != 0L)
    {
      ret += 1L;
    }
    return ret;
  }
  





  private void RemoveMCU16PhantomBytesAndAddresses()
  {
    ListIterator<DataRecord> iterator = records.listIterator();
    
    while (iterator.hasNext())
    {
      DataRecord record = (DataRecord)iterator.next();
      

      byte[] newData = new byte[(data.length * 3 + 3) / 4];
      int destIndex = 0;
      for (int i = 0; i < data.length; i++)
      {
        if ((address + i & 0x3) != 3L)
        {


          newData[(destIndex++)] = data[i];
        }
      }
      
      address >>= 1;
      data = newData;
      iterator.set(record);
    }
  }
  







  public void RemoveIgnoreRange(AddressRange ignoreRange)
  {
    List<AddressRange> ignoreRanges = new ArrayList();
    ignoreRanges.add(ignoreRange);
    RemoveIgnoreRanges(ignoreRanges);
  }
  
  public void RemoveIgnoreRangeByDR(DataRecord ignoreRecordRange)
  {
    List<AddressRange> ignoreRanges = new ArrayList();
    ignoreRanges.add(new AddressRange(address, ignoreRecordRange.getEndAddress()));
    RemoveIgnoreRanges(ignoreRanges);
  }
  
  public void RemoveIgnoreRangesByDR(List<DataRecord> ignoreRecordRanges)
  {
    List<AddressRange> ignoreRanges = new ArrayList();
    for (DataRecord dr : ignoreRecordRanges)
    {
      ignoreRanges.add(new AddressRange(address, dr.getEndAddress()));
    }
    RemoveIgnoreRanges(ignoreRanges);
  }
  












  public void RemoveIgnoreRanges(List<? extends AddressRange> ignoreRanges)
  {
    if (ignoreRanges == null)
    {
      return;
    }
    if (ignoreRanges.isEmpty())
    {
      return;
    }
    

    Collections.sort(records);
    Collections.sort(ignoreRanges);
    DataRecord rec;
    for (int i = 0; i < records.size(); i++)
    {
      rec = (DataRecord)records.get(i);
      
      for (AddressRange ignoreRange : ignoreRanges)
      {

        if ((address >= startAddr) && (rec.getEndAddress() <= endAddr))
        {
          records.remove(i);
          i--;
          break;
        }
        

        if ((rec.getEndAddress() >= startAddr) && (address < endAddr))
        {




          if ((address <= startAddr) && (rec.getEndAddress() >= endAddr))
          {
            byte[] newData = new byte[data.length - rec.getDataIndexOfAddress(endAddr)];
            System.arraycopy(data, rec.getDataIndexOfAddress(endAddr), newData, 0, newData.length);
            DataRecord newRecordRight = new DataRecord(endAddr, newData, architecture16Bit);
            records.add(i + 1, newRecordRight);
            
            newData = new byte[rec.getDataIndexOfAddress(startAddr)];
            System.arraycopy(data, 0, newData, 0, newData.length);
            data = newData;

          }
          else if (address >= startAddr)
          {
            byte[] newData = new byte[data.length - rec.getDataIndexOfAddress(endAddr)];
            System.arraycopy(data, rec.getDataIndexOfAddress(endAddr), newData, 0, newData.length);
            data = newData;
            address = endAddr;

          }
          else
          {
            byte[] newData = new byte[rec.getDataIndexOfAddress(startAddr)];
            System.arraycopy(data, 0, newData, 0, newData.length);
            data = newData;
          }
        }
      }
    }
    


    DataRecord.CoalesceRecords(records);
  }
  









  public void CoalesceAdjacentDataRecords()
  {
    DataRecord.CoalesceRecords(records);
  }
  










  public void ReorderForSequentialAccess()
  {
    List<DataRecord> orderFirstElements = new ArrayList();
    

    if (!architecture16Bit)
    {
      return;
    }
    

    for (int i = 0; i < records.size(); i++)
    {
      DataRecord dr = (DataRecord)records.get(i);
      

      if ((architecture16Bit) && ((address & 0x800000) == 8388608L))
      {

        if ((address & 0xFFFF00) != 16252928L)
        {




          orderFirstElements.add(dr);
          records.remove(i);
          i--;
        }
      }
    }
    

    records.addAll(0, orderFirstElements);
  }
  
  public void Coalesce(boolean presort, int leftAlignAddrSize, int rightAlignAddrSize)
  {
    DataRecord.CoalesceRecords(records, presort, leftAlignAddrSize, rightAlignAddrSize);
  }
}
