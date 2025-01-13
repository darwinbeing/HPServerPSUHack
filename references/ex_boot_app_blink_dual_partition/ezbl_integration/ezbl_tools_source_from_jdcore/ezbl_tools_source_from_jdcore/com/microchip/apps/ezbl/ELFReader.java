package com.microchip.apps.ezbl;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.regex.Matcher;























public class ELFReader
{
  public ELFReader() {}
  
  class ELFHeader
  {
    char[] e_ident = new char[16];
    int e_type;
    int e_machine;
    int e_version;
    int e_entry;
    int e_phoff;
    int e_shoff;
    int e_flags;
    int e_ehsize;
    int e_phentsize;
    int e_phnum;
    int e_shentsize;
    int e_shnum;
    int e_shstrndx;
    
    ELFHeader() {}
  }
  
  private final class e_type {
    int ET_NONE = 0;
    int ET_REL = 1;
    int ET_EXEC = 2;
    int ET_DYN = 3;
    int ET_CORE = 4;
    int ET_LOPROC = 65280;
    int ET_HIPROC = 65535;
    
    private e_type() {}
  }
  
  public static final class e_machine {
    static int EM_NONE = 0;
    

    static int EM_386 = 3;
    


    static int EM_MIPS = 8;
    static int EM_PIC30 = 118;
    static int EM_XC8 = 204;
    
    public e_machine() {}
  }
  
  class SectionHeader {
    int sh_name;
    int sh_type;
    int sh_flags;
    int sh_addr;
    int sh_offset;
    int sh_size;
    int sh_link;
    int sh_info;
    int sh_addralign;
    int sh_entsize;
    
    SectionHeader() {}
  }
  
  private static final class sh_type {
    static int SHT_NULL = 0;
    static int SHT_PROGBITS = 1;
    static int SHT_SYMTAB = 2;
    static int SHT_STRTAB = 3;
    static int SHT_RELA = 4;
    static int SHT_HASH = 5;
    static int SHT_DYNAMIC = 6;
    static int SHT_NOTE = 7;
    static int SHT_NOBITS = 8;
    static int SHT_REL = 9;
    static int SHT_SHLIB = 10;
    static int SHT_DYNSYM = 11;
    static int SHT_LOPROC = 1879048192;
    static int SHT_HIPROC = Integer.MAX_VALUE;
    static int SHT_LOUSER = Integer.MIN_VALUE;
    static int SHT_HIUSER = -1;
    
    private sh_type() {}
  }
  
  public static int Machine(String elfFilePath) { elfFilePath = elfFilePath.trim().replaceAll("[\\\\/]+", Matcher.quoteReplacement(File.separator));
    




    try
    {
      File f = new File(elfFilePath);
      if (!f.exists())
      {
        return e_machine.EM_NONE;
      }
      
      FileInputStream in = new FileInputStream(f);
      byte[] data = new byte[20];
      in.read(data);
      in.close();
      
      return data[18] & 0xFF | (data[19] & 0xFF) << 8;
    }
    catch (IOException ex)
    {
      throw new RuntimeException(ex);
    }
  }
  
  public static void main(String[] args)
  {
    if (args.length == 0)
      System.exit(-1);
    int machine = Machine(args[0]);
    System.exit(machine == e_machine.EM_386 ? 386 : machine == e_machine.EM_XC8 ? 8 : machine == e_machine.EM_PIC30 ? 16 : machine == e_machine.EM_MIPS ? 32 : machine);
  }
}
