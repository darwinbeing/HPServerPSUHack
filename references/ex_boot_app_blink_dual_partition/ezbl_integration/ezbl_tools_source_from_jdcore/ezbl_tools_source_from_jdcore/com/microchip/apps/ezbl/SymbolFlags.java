package com.microchip.apps.ezbl;

import java.io.Serializable;
import java.util.Arrays;









































public class SymbolFlags
  implements Serializable, Cloneable
{
  public boolean local = false;
  public boolean global = false;
  public boolean uniqueGlobal = false;
  public boolean neitherGlobalNorLocal = false;
  public boolean globalAndLocal = false;
  
  public boolean weak = false;
  

  public boolean constructor = false;
  

  public boolean warning = false;
  

  public boolean indirectReference = false;
  public boolean relocatableEvaluated = false;
  

  public boolean debugging = false;
  public boolean dynamic = false;
  

  public boolean function = false;
  public boolean file = false;
  public boolean object = false;
  


  public SymbolFlags() {}
  

  public SymbolFlags(String flagString)
  {
    if (flagString == null)
    {
      return;
    }
    
    if (flagString.length() != 7)
    {
      return;
    }
    

    char c = flagString.charAt(0);
    local = (c == 'l');
    global = (c == 'g');
    uniqueGlobal = (c == 'u');
    neitherGlobalNorLocal = (c == ' ');
    globalAndLocal = (c == '!');
    
    c = flagString.charAt(1);
    weak = (c == 'w');
    
    c = flagString.charAt(2);
    constructor = (c == 'C');
    
    c = flagString.charAt(3);
    warning = (c == 'W');
    
    c = flagString.charAt(4);
    indirectReference = (c == 'I');
    relocatableEvaluated = (c == 'i');
    
    c = flagString.charAt(5);
    debugging = (c == 'd');
    dynamic = (c == 'D');
    
    c = flagString.charAt(6);
    function = (c == 'F');
    file = (c == 'f');
    object = (c == 'O');
  }
  
  public SymbolFlags Clone()
  {
    SymbolFlags ret = new SymbolFlags();
    
    constructor = constructor;
    debugging = debugging;
    dynamic = dynamic;
    file = file;
    function = function;
    global = global;
    globalAndLocal = globalAndLocal;
    indirectReference = indirectReference;
    local = local;
    neitherGlobalNorLocal = neitherGlobalNorLocal;
    object = object;
    relocatableEvaluated = relocatableEvaluated;
    uniqueGlobal = uniqueGlobal;
    warning = warning;
    weak = weak;
    
    return ret;
  }
  
  public boolean equals(SymbolFlags symFlags)
  {
    return (constructor == constructor) && (debugging == debugging) && (dynamic == dynamic) && (file == file) && (function == function) && (global == global) && (globalAndLocal == globalAndLocal) && (indirectReference == indirectReference) && (local == local) && (neitherGlobalNorLocal == neitherGlobalNorLocal) && (object == object) && (relocatableEvaluated == relocatableEvaluated) && (uniqueGlobal == uniqueGlobal) && (warning == warning) && (weak == weak);
  }
  















  public String toString()
  {
    char[] flags = new char[7];
    for (int i = 0; i < flags.length; i++)
    {
      flags[i] = ' ';
    }
    
    if (local)
    {
      flags[0] = 'l';
    }
    if (global)
    {
      flags[0] = 'g';
    }
    if (uniqueGlobal)
    {
      flags[0] = 'u';
    }
    if (neitherGlobalNorLocal)
    {
      flags[0] = ' ';
    }
    if (globalAndLocal)
    {
      flags[0] = '!';
    }
    
    if (weak)
    {
      flags[1] = 'w';
    }
    
    if (constructor)
    {
      flags[2] = 'C';
    }
    
    if (warning)
    {
      flags[3] = 'W';
    }
    
    if (indirectReference)
    {
      flags[4] = 'I';
    }
    if (relocatableEvaluated)
    {
      flags[4] = 'i';
    }
    
    if (debugging)
    {
      flags[5] = 'd';
    }
    if (dynamic)
    {
      flags[5] = 'D';
    }
    
    if (function)
    {
      flags[6] = 'F';
    }
    if (file)
    {
      flags[6] = 'f';
    }
    if (object)
    {
      flags[6] = 'O';
    }
    
    return Arrays.toString(flags);
  }
  
  public boolean isLocal()
  {
    return local;
  }
  
  public void setLocal(boolean local)
  {
    this.local = local;
  }
  
  public boolean isGlobal()
  {
    return global;
  }
  
  public void setGlobal(boolean global)
  {
    this.global = global;
  }
  
  public boolean isUniqueGlobal()
  {
    return uniqueGlobal;
  }
  
  public void setUniqueGlobal(boolean uniqueGlobal)
  {
    this.uniqueGlobal = uniqueGlobal;
  }
  
  public boolean isNeitherGlobalNorLocal()
  {
    return neitherGlobalNorLocal;
  }
  
  public void setNeitherGlobalNorLocal(boolean neitherGlobalNorLocal)
  {
    this.neitherGlobalNorLocal = neitherGlobalNorLocal;
  }
  
  public boolean isGlobalAndLocal()
  {
    return globalAndLocal;
  }
  
  public void setGlobalAndLocal(boolean globalAndLocal)
  {
    this.globalAndLocal = globalAndLocal;
  }
  
  public boolean isWeak()
  {
    return weak;
  }
  
  public void setWeak(boolean weak)
  {
    this.weak = weak;
  }
  
  public boolean isConstructor()
  {
    return constructor;
  }
  
  public void setConstructor(boolean constructor)
  {
    this.constructor = constructor;
  }
  
  public boolean isWarning()
  {
    return warning;
  }
  
  public void setWarning(boolean warning)
  {
    this.warning = warning;
  }
  
  public boolean isIndirectReference()
  {
    return indirectReference;
  }
  
  public void setIndirectReference(boolean indirectReference)
  {
    this.indirectReference = indirectReference;
  }
  
  public boolean isRelocatableEvaluated()
  {
    return relocatableEvaluated;
  }
  
  public void setRelocatableEvaluated(boolean relocatableEvaluated)
  {
    this.relocatableEvaluated = relocatableEvaluated;
  }
  
  public boolean isDebugging()
  {
    return debugging;
  }
  
  public void setDebugging(boolean debugging)
  {
    this.debugging = debugging;
  }
  
  public boolean isDynamic()
  {
    return dynamic;
  }
  
  public void setDynamic(boolean dynamic)
  {
    this.dynamic = dynamic;
  }
  
  public boolean isFunction()
  {
    return function;
  }
  
  public void setFunction(boolean function)
  {
    this.function = function;
  }
  
  public boolean isFile()
  {
    return file;
  }
  
  public void setFile(boolean file)
  {
    this.file = file;
  }
  
  public boolean isObject()
  {
    return object;
  }
  
  public void setObject(boolean object)
  {
    this.object = object;
  }
}
