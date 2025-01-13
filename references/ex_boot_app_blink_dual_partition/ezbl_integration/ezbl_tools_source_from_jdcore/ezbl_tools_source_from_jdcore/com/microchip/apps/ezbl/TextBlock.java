package com.microchip.apps.ezbl;

import java.io.Serializable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;





























public class TextBlock
  implements Serializable, Cloneable
{
  public String data = "";
  public int foundOuterStartIndex = 0;
  public int foundOuterEndIndex = 0;
  public int foundInnerStartIndex = 0;
  public int foundInnerEndIndex = 0;
  public int lastInnerBlockStartMatchLength = 0;
  public int lastInnerBlockEndMatchLength = 0;
  public String lastOuterBlockStartRegex = null;
  public String lastOuterBlockEndRegex = null;
  public String lastInnerBlockStartRegex = null;
  public String lastInnerBlockEndRegex = null;
  public Pattern lastOuterBlockStartPattern = null;
  public Pattern lastOuterBlockEndPattern = null;
  






  public TextBlock() {}
  






  public TextBlock(String blockText)
  {
    if (blockText != null)
    {
      data = blockText;
    }
  }
  



  public String getData()
  {
    return data;
  }
  



  public void setData(String data)
  {
    this.data = data;
  }
  



  public int getFoundOuterStartIndex()
  {
    return foundOuterStartIndex;
  }
  



  public void setFoundOuterStartIndex(int foundOuterStartIndex)
  {
    this.foundOuterStartIndex = foundOuterStartIndex;
  }
  



  public int getFoundOuterEndIndex()
  {
    return foundOuterEndIndex;
  }
  



  public void setFoundOuterEndIndex(int foundOuterEndIndex)
  {
    this.foundOuterEndIndex = foundOuterEndIndex;
  }
  



  public int getFoundInnerStartIndex()
  {
    return foundInnerStartIndex;
  }
  



  public void setFoundInnerStartIndex(int foundInnerStartIndex)
  {
    this.foundInnerStartIndex = foundInnerStartIndex;
  }
  



  public int getFoundInnerEndIndex()
  {
    return foundInnerEndIndex;
  }
  



  public void setFoundInnerEndIndex(int foundInnerEndIndex)
  {
    this.foundInnerEndIndex = foundInnerEndIndex;
  }
  



  public int getLastInnerBlockStartMatchLength()
  {
    return lastInnerBlockStartMatchLength;
  }
  




  public void setLastInnerBlockStartMatchLength(int lastInnerBlockStartMatchLength)
  {
    this.lastInnerBlockStartMatchLength = lastInnerBlockStartMatchLength;
  }
  



  public int getLastInnerBlockEndMatchLength()
  {
    return lastInnerBlockEndMatchLength;
  }
  




  public void setLastInnerBlockEndMatchLength(int lastInnerBlockEndMatchLength)
  {
    this.lastInnerBlockEndMatchLength = lastInnerBlockEndMatchLength;
  }
  



  public String getLastOuterBlockStartRegex()
  {
    return lastOuterBlockStartRegex;
  }
  



  public void setLastOuterBlockStartRegex(String lastOuterBlockStartRegex)
  {
    this.lastOuterBlockStartRegex = lastOuterBlockStartRegex;
  }
  



  public String getLastOuterBlockEndRegex()
  {
    return lastOuterBlockEndRegex;
  }
  



  public void setLastOuterBlockEndRegex(String lastOuterBlockEndRegex)
  {
    this.lastOuterBlockEndRegex = lastOuterBlockEndRegex;
  }
  



  public String getLastInnerBlockStartRegex()
  {
    return lastInnerBlockStartRegex;
  }
  



  public void setLastInnerBlockStartRegex(String lastInnerBlockStartRegex)
  {
    this.lastInnerBlockStartRegex = lastInnerBlockStartRegex;
  }
  



  public String getLastInnerBlockEndRegex()
  {
    return lastInnerBlockEndRegex;
  }
  



  public void setLastInnerBlockEndRegex(String lastInnerBlockEndRegex)
  {
    this.lastInnerBlockEndRegex = lastInnerBlockEndRegex;
  }
  



  public Pattern getLastOuterBlockStartPattern()
  {
    return lastOuterBlockStartPattern;
  }
  



  public void setLastOuterBlockStartPattern(Pattern lastOuterBlockStartPattern)
  {
    this.lastOuterBlockStartPattern = lastOuterBlockStartPattern;
  }
  



  public Pattern getLastOuterBlockEndPattern()
  {
    return lastOuterBlockEndPattern;
  }
  



  public void setLastOuterBlockEndPattern(Pattern lastOuterBlockEndPattern)
  {
    this.lastOuterBlockEndPattern = lastOuterBlockEndPattern;
  }
  















  boolean Find(String regex)
  {
    int searchStartOffset = foundOuterEndIndex;
    Pattern p = Pattern.compile("(" + regex + ")");
    
    Matcher m = p.matcher(data.substring(searchStartOffset));
    if (!m.find())
    {
      return false;
    }
    foundOuterStartIndex = (searchStartOffset + m.start());
    foundOuterEndIndex = (searchStartOffset + m.end());
    foundInnerStartIndex = foundOuterStartIndex;
    foundInnerEndIndex = foundOuterEndIndex;
    lastInnerBlockStartMatchLength = 0;
    lastInnerBlockEndMatchLength = 0;
    return true;
  }
  










  boolean Find(String regexInnerBlockStart, String regexInnerBlockEnd)
  {
    return Find(foundOuterEndIndex, null, regexInnerBlockStart, regexInnerBlockEnd, null);
  }
  
  boolean Find(String regexOuterBlockStart, String regexInnerBlockStart, String regexInnerBlockEnd)
  {
    return Find(foundOuterEndIndex, regexOuterBlockStart, regexInnerBlockStart, regexInnerBlockEnd, null);
  }
  
  boolean Find(String regexOuterBlockStart, String regexInnerBlockStart, String regexInnerBlockEnd, String regexOuterBlockEnd)
  {
    return Find(foundOuterEndIndex, regexOuterBlockStart, regexInnerBlockStart, regexInnerBlockEnd, regexOuterBlockEnd);
  }
  








































  boolean Find(int startOffset, String regexOuterBlockStart, String regexInnerBlockStart, String regexInnerBlockEnd, String regexOuterBlockEnd)
  {
    lastInnerBlockStartRegex = regexInnerBlockStart;
    lastInnerBlockEndRegex = regexInnerBlockEnd;
    
    if (foundOuterEndIndex < 0)
    {
      return false;
    }
    
    if (regexOuterBlockStart == null)
    {
      lastOuterBlockStartPattern = null;
    }
    else if (!regexOuterBlockStart.equals(lastOuterBlockStartRegex))
    {
      lastOuterBlockStartRegex = regexOuterBlockStart;
      lastOuterBlockStartPattern = Pattern.compile(regexOuterBlockStart);
    }
    if (regexOuterBlockEnd == null)
    {
      lastOuterBlockEndPattern = null;
    }
    else if (!regexOuterBlockEnd.equals(lastOuterBlockEndRegex))
    {
      lastOuterBlockEndRegex = regexOuterBlockEnd;
      lastOuterBlockEndPattern = Pattern.compile(regexOuterBlockEnd);
    }
    
    foundOuterStartIndex = startOffset;
    foundInnerStartIndex = foundOuterStartIndex;
    if (lastOuterBlockStartPattern != null)
    {
      Matcher m = lastOuterBlockStartPattern.matcher(data.substring(foundOuterStartIndex));
      if (!m.find())
      {
        return false;
      }
      
      foundInnerStartIndex = (foundOuterStartIndex + m.end());
      foundOuterStartIndex += m.start();
    }
    
    int foundDelimiters = 0;
    boolean openComplete = false;boolean closeComplete = false;
    int initialStartOffset = foundInnerStartIndex;
    
    Pattern p = Pattern.compile("(" + regexInnerBlockStart + ")|(" + regexInnerBlockEnd + ")");
    Matcher m = p.matcher(data.substring(initialStartOffset));
    while (m.find())
    {
      if (m.group(1) != null)
      {
        foundDelimiters++;
        if (!openComplete)
        {
          openComplete = true;
          foundInnerStartIndex = (initialStartOffset + m.end());
          lastInnerBlockStartMatchLength = m.group().length();
          if (lastOuterBlockStartPattern == null)
          {
            foundOuterStartIndex = (foundInnerStartIndex - lastInnerBlockStartMatchLength);
          }
          
        }
      }
      else
      {
        if (!openComplete) {
          continue;
        }
        

        foundDelimiters--;
        lastInnerBlockEndMatchLength = m.group().length();
      }
      
      if (foundDelimiters == 0)
      {
        closeComplete = true;
        foundInnerEndIndex = (initialStartOffset + m.start());
        foundOuterEndIndex = (initialStartOffset + m.end());
      }
    }
    
    if (!closeComplete)
    {
      foundOuterEndIndex = -1;
      return false;
    }
    
    if (regexOuterBlockEnd != null)
    {
      m = lastOuterBlockEndPattern.matcher(data.substring(foundOuterEndIndex));
      if (!m.find())
      {
        foundOuterEndIndex = -1;
        return false;
      }
      foundOuterEndIndex += m.end();
    }
    
    return true;
  }
  




  boolean FindNext()
  {
    return Find(foundOuterEndIndex, lastOuterBlockStartRegex, lastInnerBlockStartRegex, lastInnerBlockEndRegex, lastOuterBlockEndRegex);
  }
  




  void ResetFind()
  {
    foundOuterStartIndex = 0;
    foundInnerStartIndex = 0;
    foundInnerEndIndex = 0;
    foundOuterEndIndex = 0;
  }
  




  void ReplaceOuter(String newText)
  {
    data = (data.substring(0, foundOuterStartIndex) + newText + data.substring(foundOuterEndIndex));
    foundOuterEndIndex = (foundOuterStartIndex + newText.length());
    foundInnerStartIndex = foundOuterStartIndex;
    foundInnerEndIndex = foundOuterEndIndex;
  }
  


















  void InsertOuter(String insertText, boolean atBeginning, boolean justBefore)
  {
    if (atBeginning)
    {
      if (justBefore)
      {
        data = (insertText + data);
        foundOuterStartIndex += insertText.length();
        foundInnerStartIndex += insertText.length();
        foundInnerEndIndex += insertText.length();
        foundOuterEndIndex += insertText.length();
      }
      else
      {
        data = (data.substring(0, foundOuterStartIndex) + insertText + data.substring(foundOuterStartIndex));
        foundInnerStartIndex += insertText.length();
        foundInnerEndIndex += insertText.length();
        foundOuterEndIndex += insertText.length();
      }
      

    }
    else if (justBefore)
    {
      data = (data.substring(0, foundOuterEndIndex) + insertText + data.substring(foundOuterEndIndex));
      foundOuterEndIndex += insertText.length();
    }
    else
    {
      data += insertText;
    }
  }
  





  void ReplaceInner(String newText)
  {
    int sizeDiff = newText.length() - (foundInnerEndIndex - foundInnerStartIndex);
    
    data = (data.substring(0, foundInnerStartIndex) + newText + data.substring(foundInnerEndIndex));
    foundInnerEndIndex += sizeDiff;
    foundOuterEndIndex += sizeDiff;
  }
  

















  void InsertInner(String insertText, boolean atBeginning, boolean justBefore)
  {
    if (atBeginning)
    {
      if (justBefore)
      {
        data = (insertText + data);
        foundInnerStartIndex += insertText.length();
        foundInnerEndIndex += insertText.length();
      }
      else
      {
        data = (data.substring(0, foundInnerStartIndex) + insertText + data.substring(foundInnerStartIndex));
        foundInnerEndIndex += insertText.length();
      }
      

    }
    else if (justBefore)
    {
      data = (data.substring(0, foundInnerEndIndex) + insertText + data.substring(foundInnerEndIndex));
      foundInnerEndIndex += insertText.length();
    }
    else
    {
      data += insertText;
    }
    

    foundOuterEndIndex += insertText.length();
  }
  




  void DeleteOuter()
  {
    data = (data.substring(0, foundOuterStartIndex) + data.substring(foundOuterEndIndex));
    foundOuterEndIndex = foundOuterStartIndex;
    foundInnerStartIndex = foundOuterStartIndex;
  }
  




  void DeleteInner()
  {
    data = (data.substring(0, foundInnerStartIndex) + data.substring(foundInnerEndIndex));
    foundOuterEndIndex -= foundInnerEndIndex - foundInnerStartIndex;
    foundInnerEndIndex = (foundInnerStartIndex + 1);
  }
  





  void ReplaceFullBlock(String newBlockData)
  {
    data = newBlockData;
    ResetFind();
  }
  




  String GetFullBlock()
  {
    return data;
  }
  




  String GetOuter()
  {
    return data.substring(foundOuterStartIndex, foundOuterEndIndex);
  }
  




  String GetInner(boolean excludeDelimiters)
  {
    if (excludeDelimiters)
    {
      return data.substring(foundInnerStartIndex, foundInnerEndIndex);
    }
    return data.substring(foundInnerStartIndex - lastInnerBlockStartMatchLength, foundInnerEndIndex + lastInnerBlockEndMatchLength);
  }
  





  String GetOuterLeft(boolean excludeDelimiter)
  {
    if (excludeDelimiter)
    {
      return data.substring(foundOuterStartIndex, foundInnerStartIndex - lastInnerBlockStartMatchLength);
    }
    return data.substring(foundOuterStartIndex, foundInnerStartIndex);
  }
  





  String GetOuterRight(boolean excludeDelimiter)
  {
    if (excludeDelimiter)
    {
      return data.substring(foundInnerEndIndex + lastInnerBlockStartMatchLength, foundOuterEndIndex);
    }
    return data.substring(foundInnerEndIndex, foundOuterEndIndex);
  }
  




  int StartOuter()
  {
    return foundOuterStartIndex;
  }
  





  int StartInner(boolean rightOfDelimiter)
  {
    if (rightOfDelimiter)
    {
      return foundInnerStartIndex;
    }
    return foundInnerStartIndex - lastInnerBlockStartMatchLength;
  }
  





  int EndOuter()
  {
    return foundOuterEndIndex;
  }
  






  int EndInner(boolean leftOfDelimiter)
  {
    if (leftOfDelimiter)
    {
      return foundInnerEndIndex;
    }
    return foundInnerEndIndex + lastInnerBlockEndMatchLength;
  }
}
