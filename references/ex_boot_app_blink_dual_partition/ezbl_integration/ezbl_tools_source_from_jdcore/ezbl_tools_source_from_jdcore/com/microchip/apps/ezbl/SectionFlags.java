package com.microchip.apps.ezbl;

import java.io.Serializable;






























public class SectionFlags
  implements Serializable, Cloneable
{
  static final long serialVersionUID = 1L;
  public String wholeString = "";
  public boolean CONTENTS = false;
  public boolean ALLOC = false;
  public boolean LOAD = false;
  public boolean CODE = false;
  public boolean DEBUGGING = false;
  public boolean DATA = false;
  public boolean NEVER_LOAD = false;
  public boolean PERSIST = false;
  public boolean PSV = false;
  public boolean PAGE = false;
  public boolean READONLY = false;
  public boolean EDS = false;
  public boolean RELOC = false;
  public boolean NEAR = false;
  public boolean REVERSE = false;
  public boolean SECURE = false;
  public boolean XMEMORY = false;
  public boolean YMEMORY = false;
  public boolean MEMORY = false;
  public boolean PACKEDFLASH = false;
  public boolean ABSOLUTE = false;
  public boolean PRESERVED = false;
  public boolean UPDATE = false;
  public boolean LINK_ONCE_SAME_SIZE = false;
  public String unknown = "";
  

  public SectionFlags() {}
  

  public SectionFlags Clone()
  {
    SectionFlags ret = new SectionFlags();
    
    ABSOLUTE = ABSOLUTE;
    ALLOC = ALLOC;
    CODE = CODE;
    CONTENTS = CONTENTS;
    DATA = DATA;
    DEBUGGING = DEBUGGING;
    EDS = EDS;
    LOAD = LOAD;
    MEMORY = MEMORY;
    NEAR = NEAR;
    NEVER_LOAD = NEVER_LOAD;
    PACKEDFLASH = PACKEDFLASH;
    PAGE = PAGE;
    PERSIST = PERSIST;
    PSV = PSV;
    READONLY = READONLY;
    RELOC = RELOC;
    REVERSE = REVERSE;
    SECURE = SECURE;
    XMEMORY = XMEMORY;
    YMEMORY = YMEMORY;
    PRESERVED = PRESERVED;
    UPDATE = UPDATE;
    LINK_ONCE_SAME_SIZE = LINK_ONCE_SAME_SIZE;
    unknown = unknown;
    wholeString = wholeString;
    
    return ret;
  }
  



  public String getWholeString()
  {
    return wholeString;
  }
  



  public void setWholeString(String wholeString)
  {
    this.wholeString = wholeString;
  }
  



  public boolean isCONTENTS()
  {
    return CONTENTS;
  }
  



  public void setCONTENTS(boolean CONTENTS)
  {
    this.CONTENTS = CONTENTS;
  }
  



  public boolean isALLOC()
  {
    return ALLOC;
  }
  



  public void setALLOC(boolean ALLOC)
  {
    this.ALLOC = ALLOC;
  }
  



  public boolean isLOAD()
  {
    return LOAD;
  }
  



  public void setLOAD(boolean LOAD)
  {
    this.LOAD = LOAD;
  }
  



  public boolean isCODE()
  {
    return CODE;
  }
  



  public void setCODE(boolean CODE)
  {
    this.CODE = CODE;
  }
  



  public boolean isDEBUGGING()
  {
    return DEBUGGING;
  }
  



  public void setDEBUGGING(boolean DEBUGGING)
  {
    this.DEBUGGING = DEBUGGING;
  }
  



  public boolean isDATA()
  {
    return DATA;
  }
  



  public void setDATA(boolean DATA)
  {
    this.DATA = DATA;
  }
  



  public boolean isNEVER_LOAD()
  {
    return NEVER_LOAD;
  }
  



  public void setNEVER_LOAD(boolean NEVER_LOAD)
  {
    this.NEVER_LOAD = NEVER_LOAD;
  }
  



  public boolean isPERSIST()
  {
    return PERSIST;
  }
  



  public void setPERSIST(boolean PERSIST)
  {
    this.PERSIST = PERSIST;
  }
  



  public boolean isPSV()
  {
    return PSV;
  }
  



  public void setPSV(boolean PSV)
  {
    this.PSV = PSV;
  }
  



  public boolean isPAGE()
  {
    return PAGE;
  }
  



  public void setPAGE(boolean PAGE)
  {
    this.PAGE = PAGE;
  }
  



  public boolean isREADONLY()
  {
    return READONLY;
  }
  



  public void setREADONLY(boolean READONLY)
  {
    this.READONLY = READONLY;
  }
  



  public boolean isEDS()
  {
    return EDS;
  }
  



  public void setEDS(boolean EDS)
  {
    this.EDS = EDS;
  }
  



  public boolean isRELOC()
  {
    return RELOC;
  }
  



  public void setRELOC(boolean RELOC)
  {
    this.RELOC = RELOC;
  }
  



  public boolean isNEAR()
  {
    return NEAR;
  }
  



  public void setNEAR(boolean NEAR)
  {
    this.NEAR = NEAR;
  }
  



  public boolean isREVERSE()
  {
    return REVERSE;
  }
  



  public void setREVERSE(boolean REVERSE)
  {
    this.REVERSE = REVERSE;
  }
  



  public boolean isSECURE()
  {
    return SECURE;
  }
  



  public void setSECURE(boolean SECURE)
  {
    this.SECURE = SECURE;
  }
  



  public boolean isXMEMORY()
  {
    return XMEMORY;
  }
  



  public void setXMEMORY(boolean XMEMORY)
  {
    this.XMEMORY = XMEMORY;
  }
  



  public boolean isYMEMORY()
  {
    return YMEMORY;
  }
  



  public void setYMEMORY(boolean YMEMORY)
  {
    this.YMEMORY = YMEMORY;
  }
  



  public boolean isMEMORY()
  {
    return MEMORY;
  }
  



  public void setMEMORY(boolean MEMORY)
  {
    this.MEMORY = MEMORY;
  }
  



  public boolean isPACKEDFLASH()
  {
    return PACKEDFLASH;
  }
  



  public void setPACKEDFLASH(boolean PACKEDFLASH)
  {
    this.PACKEDFLASH = PACKEDFLASH;
  }
  



  public boolean isABSOLUTE()
  {
    return ABSOLUTE;
  }
  



  public void setABSOLUTE(boolean ABSOLUTE)
  {
    this.ABSOLUTE = ABSOLUTE;
  }
  



  public String getUnknown()
  {
    return unknown;
  }
  



  public void setUnknown(String unknown)
  {
    this.unknown = unknown;
  }
}
