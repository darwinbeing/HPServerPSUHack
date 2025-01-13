package com.microchip.apps.ezbl;

import java.io.PrintStream;
import java.security.DigestException;
import java.security.GeneralSecurityException;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.ListIterator;
import java.util.zip.CRC32;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.SecretKeySpec;





















public class Blob
{
  boolean architecture16Bit;
  List<DataRecord> records = null;
  long typeCode = 1112493122L;
  

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
  

  public Blob(boolean architecture16Bit)
  {
    records = new ArrayList();
    this.architecture16Bit = architecture16Bit;
  }
  









  public Blob(byte[] blobData, boolean architecture16Bit)
  {
    byte[] hash = new byte[20];
    long crcInFile = 0L;
    

    if (blobData.length < 32)
    {
      throw new RuntimeException("Invalid blob; length too small");
    }
    

    System.arraycopy(blobData, 8, hash, 0, 20);
    for (int i = 0; i < 4; i++)
    {
      crcInFile |= (blobData[(blobData.length - 4 + i)] & 0xFF) << i * 8;
      blobData[(blobData.length - 4 + i)] = 0;
    }
    Arrays.fill(blobData, 8, 28, (byte)0);
    



    try
    {
      hashComputer = MessageDigest.getInstance("SHA-1");
    }
    catch (NoSuchAlgorithmException ex) {
      MessageDigest hashComputer;
      throw new RuntimeException("Cannot parse blob; missing SHA-1 java hashing library"); }
    MessageDigest hashComputer;
    hashComputer.update(blobData);
    byte[] computedHash = hashComputer.digest();
    for (int i = 0; i < 20; i++)
    {
      if (computedHash[i] != hash[i])
      {
        throw new RuntimeException("Invalid blob; internal hash mismatches with computed hash");
      }
    }
    
    long length = blobData[0] & 0xFF;
    length |= (blobData[1] & 0xFF) << 8;
    length |= (blobData[2] & 0xFF) << 16;
    length |= (blobData[3] & 0xFF) << 24;
    
    if (length != blobData.length)
    {
      throw new RuntimeException("Invalid blob; data size does not match blob size");
    }
    
    records = new ArrayList();
    
    typeCode = (blobData[4] & 0xFF);
    typeCode |= (blobData[5] & 0xFF) << 8;
    typeCode |= (blobData[6] & 0xFF) << 16;
    typeCode |= (blobData[7] & 0xFF) << 24;
    

    System.arraycopy(computedHash, 0, blobData, 8, computedHash.length);
    

    CRC32 myCRC = new CRC32();
    myCRC.update(blobData, 0, blobData.length - 4);
    long crcValue = myCRC.getValue();
    if (crcValue != crcInFile)
    {
      throw new RuntimeException("Invalid blob; internal CRC mismatches computed CRC");
    }
    
    int curIndex = 28;
    while (curIndex < blobData.length - 4)
    {
      DataRecord record = new DataRecord(architecture16Bit);
      

      int recordByteLen = blobData[(curIndex++)] & 0xFF;
      recordByteLen = (int)(recordByteLen | (blobData[(curIndex++)] & 0xFF) << 8);
      recordByteLen = (int)(recordByteLen | (blobData[(curIndex++)] & 0xFF) << 16);
      recordByteLen = (int)(recordByteLen | (blobData[(curIndex++)] & 0xFF) << 24);
      address = (blobData[(curIndex++)] & 0xFF);
      address |= (blobData[(curIndex++)] & 0xFF) << 8;
      address |= (blobData[(curIndex++)] & 0xFF) << 16;
      address |= (blobData[(curIndex++)] & 0xFF) << 24;
      data = Arrays.copyOfRange(blobData, curIndex, curIndex + recordByteLen - 8);
      records.add(record);
      curIndex += recordByteLen - 8;
    }
    
    this.architecture16Bit = architecture16Bit;
  }
  
  public Blob(String hexFileContents, boolean architecture16Bit)
  {
    this.architecture16Bit = false;
    records = ParseHex(hexFileContents, false);
    

    DataRecord.CoalesceRecords(records);
    

    if ((architecture16Bit) || (TestRecordsLookLike16Bit(records)))
    {
      this.architecture16Bit = true;
      for (DataRecord dr : records)
      {
        architecture16Bit = true;
      }
      

      RemoveMCU16PhantomBytesAndAddresses();
    }
  }
  










  public static boolean TestRecordsLookLike16Bit(List<DataRecord> records)
  {
    int highAddresses = 0;
    int dataAtPhantomBytePos = 0;
    for (DataRecord dr : records)
    {
      if ((address & 0xFFFFFFFFFE000000) != 0L)
      {
        highAddresses++;
      }
      for (int i = 3 - (int)(address & 0x3); i < data.length; i += 4)
      {
        if (data[i] != 0)
        {
          dataAtPhantomBytePos++;
        }
      }
    }
    return (highAddresses == 0) && (dataAtPhantomBytePos == 0);
  }
  

  static List<DataRecord> ParseHex(byte[] hexFileContents, boolean architecture16Bit)
  {
    HexDecoderState state = HexDecoderState.COLON;
    long decoderAddress = 0L;long hexHEX386Address = 0L;
    int hexRecordType = 0;
    int hexRecordAddress = 0;
    int hexRecordLen = 0;
    int hexRecordDataIndex = 0;
    int hexRecordChecksum = 0;int computedRecordChecksum = 0;
    byte[] hexRecordData = new byte['Ā'];
    
    DataRecord record = null;
    List<DataRecord> records = new ArrayList();
    

    for (int inIndex = 0; inIndex < hexFileContents.length; tmp562_560++)
    {

      byte b = hexFileContents[inIndex];
      

      if ((b == 32) || (b == 13) || (b == 10) || (b == 9))
      {
        if (state != HexDecoderState.COLON)
        {
          System.err.println("    EZBL: Error reading .hex file: whitespace detected within a record; skipping");

        }
        

      }
      else if (state == HexDecoderState.COLON)
      {
        if (b != 58)
        {
          System.err.println("    EZBL: Error reading .hex file: unknown character encountered; skipping");
        }
        else {
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
          System.err.println("    EZBL: Error reading .hex file: unknown character encountered; skipping");
          continue;
        }
        

        switch (1.$SwitchMap$com$microchip$apps$ezbl$Blob$HexDecoderState[state.ordinal()])
        {
        case 1: 
          hexRecordLen = b & 0xF;
          state = HexDecoderState.LENGTH1;
          break;
        case 2: 
          hexRecordLen <<= 4;
          hexRecordLen |= b & 0xF;
          computedRecordChecksum = computedRecordChecksum + hexRecordLen & 0xFF;
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
          state = HexDecoderState.TYPE0;
          break;
        
        case 7: 
          hexRecordType = b & 0xF;
          state = HexDecoderState.TYPE1;
          break;
        case 8: 
          hexRecordType <<= 4;
          hexRecordType |= b & 0xF;
          hexRecordDataIndex = 0;
          computedRecordChecksum = computedRecordChecksum + hexRecordType & 0xFF;
          byte[] recordData = new byte[hexRecordLen];
          record = new DataRecord(decoderAddress & 0xFFFF0000 | hexRecordAddress, recordData, architecture16Bit);
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
          int tmp562_560 = hexRecordDataIndex; byte[] tmp562_558 = hexRecordData;tmp562_558[tmp562_560] = ((byte)(tmp562_558[tmp562_560] << 4)); int 
            tmp572_570 = hexRecordDataIndex; byte[] tmp572_568 = hexRecordData;tmp572_568[tmp572_570] = ((byte)(tmp572_568[tmp572_570] | b));
          data[hexRecordDataIndex] = hexRecordData[hexRecordDataIndex];
          computedRecordChecksum = computedRecordChecksum + (hexRecordData[hexRecordDataIndex] & 0xFF) & 0xFF;
          hexRecordDataIndex++;
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
          if (hexRecordChecksum != computedRecordChecksum)
          {
            System.err.println("    EZBL: Error reading .hex file: record checksum failure; using bad data anyway");
          }
          computedRecordChecksum = 0;
          state = HexDecoderState.COLON;
          

          switch (hexRecordType)
          {

          case 0: 
            records.add(record);
            

            decoderAddress = decoderAddress & 0xFFFF0000 | decoderAddress + hexRecordLen & 0xFFFF;
            break;
          
          case 1: 
            if (tmp562_560 + 12 < hexFileContents.length)
            {
              System.err.println("    EZBL: Error reading .hex file: found EOF record with more data after it; going to try and process extra data");
            }
            if (hexRecordLen != 0)
            {
              System.err.println("    EZBL: Error reading .hex file: found EOF record with data in it; throwing extra data away");
            }
            
            break;
          case 2: 
            hexHEX386Address = (hexRecordData[0] & 0xFF) << 12 | (hexRecordData[1] & 0xFF) << 4;
            if (hexRecordLen != 2)
            {
              System.err.println("    EZBL: Error reading .hex file: found extra data in an Extended Segment Address Record; throwing extra data away");
            }
            
            break;
          case 4: 
            decoderAddress = decoderAddress & 0xFFFF | (hexRecordData[0] & 0xFF) << 24 | (hexRecordData[1] & 0xFF) << 16;
            if (hexRecordLen != 2)
            {
              System.err.println("    EZBL: Error reading .hex file: found extra data in an Extended Linear Address Record; throwing extra data away");
            }
            
            break;
          case 5: 
            System.err.println("    EZBL: Error reading .hex file: Start Linear Address Records not supported; skipping record");
            break;
          case 3: 
          default: 
            System.err.println("    EZBL: Error reading .hex file: unrecognized record type; skipping record");
          }
          
          hexRecordDataIndex = 0;
        }
        
      }
    }
    if (state != HexDecoderState.COLON)
    {
      System.err.println("    EZBL: Error reading .hex file: ran out of data in the middle of a record; discarding record fragment");
    }
    if (hexRecordType != 1)
    {
      System.err.println("    EZBL: Error reading .hex file: missing EOF record; continuing anyway");
    }
    
    return records;
  }
  




  static List<DataRecord> ParseHex(String hexFileContents, boolean architecture16Bit)
  {
    return ParseHex(hexFileContents.getBytes(), architecture16Bit);
  }
  


  public static byte[] EncryptBlobBytes(EZBLState state, byte[] blobBytes)
  {
    byte[] counterBytes = new byte[16];
    


    byte[] encryptedBytes = new byte[blobBytes.length];
    


    System.arraycopy(blobBytes, 0, encryptedBytes, 0, encryptedBytes.length);
    

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
  








  public static String GetObjDumpView(byte[] blobBytes, boolean architecture16Bit)
  {
    String header = "";
    String expectedHash = "";
    


    int byteIndex = 0;
    int recordCount = 0;
    byte[] hash = new byte[20];
    long metaLen = 0L;long metaType = 0L;
    String metaTypeString = null;
    int[] bootID = new int[8];
    int appIDVerMajor = 0;
    int appIDVerMinor = 0;
    int appIDVerBuild = 0;
    
    List<String> output = new ArrayList();
    
    long size = blobBytes[(byteIndex++)] & 0xFF;
    size |= (blobBytes[(byteIndex++)] & 0xFF) << 8;
    size |= (blobBytes[(byteIndex++)] & 0xFF) << 16;
    size |= (blobBytes[(byteIndex++)] & 0xFF) << 24;
    
    long typeField = blobBytes[(byteIndex++)] & 0xFF;
    typeField |= (blobBytes[(byteIndex++)] & 0xFF) << 8;
    typeField |= (blobBytes[(byteIndex++)] & 0xFF) << 16;
    typeField |= (blobBytes[(byteIndex++)] & 0xFF) << 24;
    String typeFieldString = String.format("%1$c%2$c%3$c%4$c", new Object[] { Byte.valueOf(blobBytes[(byteIndex - 4)]), Byte.valueOf(blobBytes[(byteIndex - 3)]), Byte.valueOf(blobBytes[(byteIndex - 2)]), Byte.valueOf(blobBytes[(byteIndex - 1)]) });
    
    String hashString = "";
    for (int i = 0; i < 20; i += 4)
    {
      hashString = hashString + String.format("%1$02X%2$02X%3$02X%4$02X", new Object[] { Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]) });
    }
    
    if (typeFieldString.equals("BL2B"))
    {
      metaLen = blobBytes[(byteIndex++)] & 0xFF | (blobBytes[(byteIndex++)] & 0xFF) << 8 | (blobBytes[(byteIndex++)] & 0xFF) << 16 | (blobBytes[(byteIndex++)] & 0xFF) << 24;
      metaType = blobBytes[(byteIndex++)] & 0xFF | (blobBytes[(byteIndex++)] & 0xFF) << 8 | (blobBytes[(byteIndex++)] & 0xFF) << 16 | (blobBytes[(byteIndex++)] & 0xFF) << 24;
      metaTypeString = String.format("%1$c%2$c%3$c%4$c", new Object[] { Byte.valueOf(blobBytes[(byteIndex - 4)]), Byte.valueOf(blobBytes[(byteIndex - 3)]), Byte.valueOf(blobBytes[(byteIndex - 2)]), Byte.valueOf(blobBytes[(byteIndex - 1)]) });
      for (int i = 0; i < bootID.length; i++)
      {
        bootID[i] = (blobBytes[(byteIndex++)] & 0xFF | (blobBytes[(byteIndex++)] & 0xFF) << 8 | (blobBytes[(byteIndex++)] & 0xFF) << 16 | (blobBytes[(byteIndex++)] & 0xFF) << 24);
      }
      appIDVerBuild = blobBytes[(byteIndex++)] & 0xFF | (blobBytes[(byteIndex++)] & 0xFF) << 8 | (blobBytes[(byteIndex++)] & 0xFF) << 16 | (blobBytes[(byteIndex++)] & 0xFF) << 24;
      appIDVerMinor = blobBytes[(byteIndex++)] & 0xFF | (blobBytes[(byteIndex++)] & 0xFF) << 8;
      appIDVerMajor = blobBytes[(byteIndex++)] & 0xFF | (blobBytes[(byteIndex++)] & 0xFF) << 8;
    }
    
    long readCRC = blobBytes[(blobBytes.length - 4)] & 0xFF;
    readCRC |= (blobBytes[(blobBytes.length - 3)] & 0xFF) << 8;
    readCRC |= (blobBytes[(blobBytes.length - 2)] & 0xFF) << 16;
    readCRC |= (blobBytes[(blobBytes.length - 1)] & 0xFF) << 24;
    

    try
    {
      MessageDigest hashComputer = MessageDigest.getInstance("SHA-1");
      hashComputer.update(blobBytes, 0, 8);
      hashComputer.update(hash);
      hashComputer.update(blobBytes, 28, blobBytes.length - 28 - 4);
      hashComputer.update(hash, 0, 4);
      hash = hashComputer.digest();
      for (int i = 0; i < 20; i += 4)
      {
        expectedHash = expectedHash + String.format("%1$02X%2$02X%3$02X%4$02X", new Object[] { Byte.valueOf(hash[i]), Byte.valueOf(hash[(i + 1)]), Byte.valueOf(hash[(i + 2)]), Byte.valueOf(hash[(i + 3)]) });
      }
    }
    catch (NoSuchAlgorithmException ex)
    {
      System.err.println("    EZBL: ERROR! Can't find 'SHA-1' hash algorithm. Make sure your JRE includes SHA-1 support. Output hash will be set to all zeros.");
    }
    

    CRC32 myCRC = new CRC32();
    myCRC.update(blobBytes, 0, blobBytes.length - 4);
    long expectedCRC = myCRC.getValue();
    
    header = header + String.format("Blob file length:      0x%1$08X (%1$d)\n", new Object[] { Integer.valueOf(blobBytes.length) }) + String.format("Computed hash:         %1$s\n", new Object[] { expectedHash }) + String.format("Computed CRC:          0x%1$08X\n", new Object[] { Long.valueOf(expectedCRC) }) + "\n" + String.format("Blob header.length:    0x%1$08X (%1$d) (%2$s)\n", new Object[] { Long.valueOf(size), size == blobBytes.length ? "Correct" : "INVALID" }) + String.format("Blob header.flags:     0x%1$08X (\"%2$s\")\n", new Object[] { Long.valueOf(typeField), typeFieldString }) + String.format("Blob header.hash:      %1$s (%2$s)\n", new Object[] { hashString, hashString.equals(expectedHash) ? "Correct" : "INVALID" });
    





    if (metaTypeString != null)
    {
      header = header + "\n" + String.format("Blob meta.length:      0x%1$08X (%1$d) (%2$s)\n", new Object[] { Long.valueOf(metaLen), metaLen == 8 + bootID.length * 4 + 8 ? "Correct" : "INVALID" }) + String.format("Blob meta.type:        0x%1$08X (\"%2$s\") (%3$s)\n", new Object[] { Long.valueOf(metaType), metaTypeString, metaTypeString.equals("META") ? "Correct" : "INVALID" }) + String.format("Blob meta.BOOTID:      %1$08X%2$08X%3$08X%4$08X%5$08X%6$08X%7$08X%8$08X\n", new Object[] { Integer.valueOf(bootID[7]), Integer.valueOf(bootID[6]), Integer.valueOf(bootID[5]), Integer.valueOf(bootID[4]), Integer.valueOf(bootID[3]), Integer.valueOf(bootID[2]), Integer.valueOf(bootID[1]), Integer.valueOf(bootID[0]) }) + String.format("Blob meta.APPID_VER:   %1$d.%2$02d.%3$04d\n", new Object[] { Integer.valueOf(appIDVerMajor), Integer.valueOf(appIDVerMinor), Integer.valueOf(appIDVerBuild) });
    }
    



    header = header + "\n" + String.format("Blob footer CRC32:     0x%1$08X (%2$s)\n", new Object[] { Long.valueOf(readCRC), readCRC == expectedCRC ? "Correct" : "INVALID" }) + "\n";
    



    if (architecture16Bit)
    {
      int searchByteIndex = byteIndex;
      while (searchByteIndex < blobBytes.length - 4)
      {



        if (searchByteIndex + 8 > blobBytes.length - 4) {
          break;
        }
        
        long recordLength = blobBytes[(searchByteIndex++)] & 0xFF;
        recordLength |= (blobBytes[(searchByteIndex++)] & 0xFF) << 8;
        recordLength |= (blobBytes[(searchByteIndex++)] & 0xFF) << 16;
        recordLength |= (blobBytes[(searchByteIndex++)] & 0xFF) << 24;
        long recordAddress = blobBytes[(searchByteIndex++)] & 0xFF;
        recordAddress |= (blobBytes[(searchByteIndex++)] & 0xFF) << 8;
        recordAddress |= (blobBytes[(searchByteIndex++)] & 0xFF) << 16;
        recordAddress |= (blobBytes[(searchByteIndex++)] & 0xFF) << 24;
        
        if ((recordAddress & 0xFFFFFFFFFF000000) != 0L)
        {
          architecture16Bit = false;
          break;
        }
        searchByteIndex = (int)(searchByteIndex + (recordLength - 8L));
      }
    }
    
    header = header + "Starting record decoding in " + (architecture16Bit ? "16-bit" : "8-bit/32-bit") + " architecture mode\n" + "\n";
    

    while (byteIndex < blobBytes.length - 4)
    {




      if (byteIndex + 8 > blobBytes.length - 4)
      {
        output.add(String.format("    ERROR: found %1$d bytes at the end of the file. This is too small to be a record header.\n", new Object[] { Integer.valueOf(blobBytes.length - 4 - byteIndex) }));
        output.add("    Bytes are: ");
        for (int i = 0; i < blobBytes.length - 4 - byteIndex; i++)
        {
          output.add(String.format("%1$02X", new Object[] { Byte.valueOf(blobBytes[(byteIndex + i)]) }));
        }
        output.add("\n");
        break;
      }
      
      long recordLength = blobBytes[(byteIndex++)] & 0xFF;
      recordLength |= (blobBytes[(byteIndex++)] & 0xFF) << 8;
      recordLength |= (blobBytes[(byteIndex++)] & 0xFF) << 16;
      recordLength |= (blobBytes[(byteIndex++)] & 0xFF) << 24;
      
      long recordAddress = blobBytes[(byteIndex++)] & 0xFF;
      recordAddress |= (blobBytes[(byteIndex++)] & 0xFF) << 8;
      recordAddress |= (blobBytes[(byteIndex++)] & 0xFF) << 16;
      recordAddress |= (blobBytes[(byteIndex++)] & 0xFF) << 24;
      
      recordCount++;
      
      output.add(String.format("Record header.length:  0x%1$08X (8 header + %2$d data bytes)\n", new Object[] { Long.valueOf(recordLength), Long.valueOf(recordLength - 8L) }));
      output.add(String.format("Record header.address: 0x%1$08X (to 0x%2$08X)\n", new Object[] { Long.valueOf(recordAddress), Long.valueOf(recordAddress + (architecture16Bit ? (recordLength - 8L + 2L) / 3L * 2L : recordLength - 8L)) }));
      recordLength -= 8L;
      if ((int)recordLength + byteIndex > blobBytes.length - 4)
      {
        output.add("    ERROR: record length indicator extends into CRC area\n");
        recordLength = blobBytes.length - 4 - byteIndex;
      }
      else if (recordLength < 0L)
      {
        output.add("    ERROR: record length is invalid (computed record end address will be wrong)\n");
        recordLength = blobBytes.length - 4 - byteIndex;
      }
      
      if (architecture16Bit)
      {
        for (int i = 0; i + 12 <= (int)recordLength; i += 12)
        {
          output.add(String.format("%1$06X  %4$02X%3$02X%2$02X %7$02X%6$02X%5$02X %10$02X%9$02X%8$02X %13$02X%12$02X%11$02X\n", new Object[] { Long.valueOf(recordAddress), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]) }));
          recordAddress += 8L;
        }
        if (i < recordLength)
        {
          output.add(String.format("%1$06X  ", new Object[] { Long.valueOf(recordAddress) }));
          for (; i + 3 <= (int)recordLength; i += 3)
          {
            output.add(String.format("%3$02X%2$02X%1$02X ", new Object[] { Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]) }));
          }
          if ((int)recordLength - i == 2)
          {
            output.add(String.format("  %2$02X%1$02X", new Object[] { Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]) }));
          }
          else if ((int)recordLength - i == 1)
          {
            output.add(String.format("    %1$02X", new Object[] { Byte.valueOf(blobBytes[(byteIndex++)]) }));
          }
          
          output.add("\n");
        }
      }
      else
      {
        for (int i = 0; i + 16 <= (int)recordLength; i += 16)
        {
          output.add(String.format("%1$08X  ", new Object[] { Long.valueOf(recordAddress) }));
          output.add(String.format("%4$02X%3$02X%2$02X%1$02X %8$02X%7$02X%6$02X%5$02X %12$02X%11$02X%10$02X%9$02X %16$02X%15$02X%14$02X%13$02X\n", new Object[] { Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]) }));
          recordAddress += 16L;
        }
        if (i < recordLength)
        {
          output.add(String.format("%1$08X  ", new Object[] { Long.valueOf(recordAddress) }));
          for (; i + 4 <= (int)recordLength; i += 4)
          {
            output.add(String.format("%4$02X%3$02X%2$02X%1$02X ", new Object[] { Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]) }));
          }
          if ((int)recordLength - i == 3)
          {
            output.add(String.format("  %3$02X%2$02X%1$02X", new Object[] { Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]) }));
          }
          else if ((int)recordLength - i == 2)
          {
            output.add(String.format("    %2$02X%1$02X", new Object[] { Byte.valueOf(blobBytes[(byteIndex++)]), Byte.valueOf(blobBytes[(byteIndex++)]) }));
          }
          else if ((int)recordLength - i == 1)
          {
            output.add(String.format("      %1$02X", new Object[] { Byte.valueOf(blobBytes[(byteIndex++)]) }));
          }
          
          output.add("\n");
        }
      }
      output.add("\n");
    }
    
    header = header + String.format("Found Records: %1$d\n", new Object[] { Integer.valueOf(recordCount) }) + "\n";
    
    output.add(0, header);
    output.add(String.format("CRC32: 0x%1$08X\n", new Object[] { Long.valueOf(readCRC) }));
    
    return Multifunction.CatStringList(output);
  }
  




  public byte[] GetHash()
  {
    return Arrays.copyOfRange(GetBytes(), 8, 28);
  }
  







  public void AddData(long address, byte[] data)
  {
    DataRecord newData = new DataRecord(address, data, architecture16Bit);
    AddressRange range = new AddressRange(address, newData.getEndAddress());
    
    RemoveIgnoreRange(range);
    records.add(newData);
    DataRecord.CoalesceRecords(records);
  }
  








  public void FillData(AddressRange range, byte fillChar)
  {
    DataRecord newData = new DataRecord(architecture16Bit);
    address = startAddr;
    data = new byte[newData.getDataIndexOfAddress(endAddr)];
    Arrays.fill(data, fillChar);
    Blob tempBlob = new Blob(architecture16Bit);
    records.add(newData);
    


    List<AddressRange> conflicts = new ArrayList();
    for (int i = 0; i < records.size(); i++)
    {
      DataRecord rec = (DataRecord)records.get(i);
      if (rec.getEndAddress() > startAddr)
      {


        if (address < endAddr)
        {




          conflicts.add(rec.getAddressRange());
        }
      }
    }
    tempBlob.RemoveIgnoreRanges(conflicts);
    

    records.addAll(records);
    

    DataRecord.CoalesceRecords(records);
  }
  










  public byte[] GetBytes()
  {
    int index = 0;
    ListIterator<DataRecord> iterator = records.listIterator();
    while (iterator.hasNext())
    {
      DataRecord record = (DataRecord)iterator.next();
      index += data.length + 8;
    }
    

    byte[] retData = new byte[28 + index + 4];
    

    retData[0] = ((byte)(retData.length & 0xFF));
    retData[1] = ((byte)(retData.length >> 8 & 0xFF));
    retData[2] = ((byte)(retData.length >> 16 & 0xFF));
    retData[3] = ((byte)(retData.length >> 24 & 0xFF));
    retData[4] = ((byte)(int)(typeCode & 0xFF));
    retData[5] = ((byte)(int)(typeCode >> 8 & 0xFF));
    retData[6] = ((byte)(int)(typeCode >> 16 & 0xFF));
    retData[7] = ((byte)(int)(typeCode >> 24 & 0xFF));
    
    index = 28;
    

    iterator = records.listIterator();
    while (iterator.hasNext())
    {

      DataRecord record = (DataRecord)iterator.next();
      

      retData[(index++)] = ((byte)(int)(data.length + 8 & 0xFF));
      retData[(index++)] = ((byte)(int)((data.length + 8 & 0xFF00) >> 8));
      retData[(index++)] = ((byte)(int)((data.length + 8 & 0xFF0000) >> 16));
      retData[(index++)] = ((byte)(int)((data.length + 8 & 0xFF000000) >> 24));
      retData[(index++)] = ((byte)(int)(address & 0xFF));
      retData[(index++)] = ((byte)(int)((address & 0xFF00) >> 8));
      retData[(index++)] = ((byte)(int)((address & 0xFF0000) >> 16));
      retData[(index++)] = ((byte)(int)((address & 0xFF000000) >> 24));
      

      System.arraycopy(data, 0, retData, index, data.length);
      index += data.length;
    }
    

    try
    {
      MessageDigest hashComputer = MessageDigest.getInstance("SHA-1");
      hashComputer.update(retData);
      hashComputer.digest(retData, 8, 20);
    }
    catch (NoSuchAlgorithmException ex)
    {
      System.err.println("ezbl_tools: Cannot find 'SHA-1' hash algorithm. Make sure your JRE includes SHA-1 support. Output hash will be set to all zeros.");
    }
    catch (DigestException ex)
    {
      System.err.printf("ezbl_tools: SHA-1 exception: %1$s\n", new Object[] { ex.getMessage() });
    }
    






    CRC32 myCRC = new CRC32();
    myCRC.update(retData, 0, retData.length - 4);
    long crcValue = myCRC.getValue();
    retData[(retData.length - 4)] = ((byte)(int)(crcValue & 0xFF));
    retData[(retData.length - 3)] = ((byte)(int)(crcValue >> 8 & 0xFF));
    retData[(retData.length - 2)] = ((byte)(int)(crcValue >> 16 & 0xFF));
    retData[(retData.length - 1)] = ((byte)(int)(crcValue >> 24 & 0xFF));
    
    return retData;
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
  













  public void RemoveIgnoreRanges(List<AddressRange> ignoreRanges)
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
  

  public void AlignRecords()
  {
    if (!architecture16Bit)
    {
      return;
    }
    
    DataRecord.CoalesceRecords(records, false, architecture16Bit ? 4L : 16L, architecture16Bit ? 4L : 16L);
  }
  
  public void AlignRecords(int alignSize)
  {
    DataRecord.CoalesceRecords(records, false, alignSize, alignSize);
  }
}
