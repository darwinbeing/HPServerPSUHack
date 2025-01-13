package com.microchip.apps.ezbl;

import com.sun.jna.Native;
import com.sun.jna.win32.StdCallLibrary;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;


























public class OSSpecific
{
  public OSSpecific() {}
  
  private static abstract interface Kernel32
    extends StdCallLibrary
  {
    public static final Kernel32 INSTANCE = (Kernel32)Native.loadLibrary("kernel32", Kernel32.class);
    public static final Kernel32 SYNC_INSTANCE = (Kernel32)Native.synchronizedLibrary(INSTANCE);
    














    public abstract int GetCurrentProcessId();
    














    public abstract int CreateFileW(char[] paramArrayOfChar, int paramInt1, int paramInt2, int paramInt3, int paramInt4, int paramInt5, int paramInt6);
    














    public abstract int GetCommConfig(int paramInt1, int paramInt2, int paramInt3);
    














    public abstract int QueryDosDeviceW(char[] paramArrayOfChar1, char[] paramArrayOfChar2, int paramInt);
  }
  













  public static List<String> EnumDosDevices()
  {
    return EnumDosDevices(null);
  }
  












  public static List<String> EnumDosDevices(String lpDeviceName)
  {
    try
    {
      Kernel32 lib = Kernel32.INSTANCE;
      char[] buffer = new char[32768];
      
      List<String> devices = new ArrayList();
      int ret;
      do
      {
        int ret;
        if (lpDeviceName == null)
        {
          ret = lib.QueryDosDeviceW(null, buffer, buffer.length);
        }
        else
        {
          while (lpDeviceName.endsWith("\\"))
          {
            lpDeviceName = lpDeviceName.substring(0, lpDeviceName.length() - 1);
          }
          ret = lib.QueryDosDeviceW((lpDeviceName + "\000").toCharArray(), buffer, buffer.length);
        }
        

        if ((ret == 0) && (Native.getLastError() == 122))
        {
          buffer = new char[buffer.length * 2];
        }
      } while ((ret == 0) && (Native.getLastError() == 122));
      

      if (ret == 0)
      {
        System.err.print("\nQueryDosDevice() error; GetLastError(): " + Native.getLastError() + "\n");
        return null;
      }
      

      int startIndex = 0;
      for (int i = 0; i < ret - 1; i++)
      {
        if (buffer[i] == 0)
        {
          devices.add(String.valueOf(buffer, startIndex, i - startIndex));
          startIndex = i + 1;
        }
      }
      
      return devices;
    }
    catch (RuntimeException e) {}
    
    return null;
  }
}
