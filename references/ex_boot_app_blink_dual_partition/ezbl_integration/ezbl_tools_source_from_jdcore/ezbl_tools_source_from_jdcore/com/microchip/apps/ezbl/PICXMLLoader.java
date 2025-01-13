package com.microchip.apps.ezbl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;





























public class PICXMLLoader
{
  public PICXMLLoader() {}
  
  public static Document LoadPICXML(String baseSearchFolder, String fullPartNumber)
  {
    Document doc = null;
    InputStream picXML = null;
    List<String> searchFilenames = new ArrayList();
    
    searchFilenames.add(fullPartNumber + ".PIC");
    searchFilenames.add("crownking.edc.jar");
    searchFilenames.add("*.jar");
    
    for (String searchFile : searchFilenames)
    {
      List<File> foundFiles = Multifunction.FindFiles(baseSearchFolder, searchFile, true);
      if (!foundFiles.isEmpty())
      {
        Collections.sort(foundFiles);
        
        for (File f : foundFiles)
        {
          if (f.getName().equalsIgnoreCase(fullPartNumber + ".PIC"))
          {
            doc = LoadPICXML(f);
            if (doc != null) {
              return doc;
            }
          } else if ((f.getName().toLowerCase().endsWith(".jar")) || (f.getName().toLowerCase().endsWith(".zip")))
          {
            try
            {
              Main.LoadRuntimeJar(f.getCanonicalPath());
            }
            catch (IOException ex)
            {
              Logger.getLogger(PICXMLLoader.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            picXML = ClassLoader.getSystemClassLoader().getResourceAsStream("content/edc/dsPIC30/" + fullPartNumber.toUpperCase() + ".PIC");
            if (picXML != null)
              break;
            picXML = ClassLoader.getSystemClassLoader().getResourceAsStream("content/edc/dsPIC33/" + fullPartNumber.toUpperCase() + ".PIC");
            if (picXML != null)
              break;
            picXML = ClassLoader.getSystemClassLoader().getResourceAsStream("content/edc/32xxxx/" + fullPartNumber.toUpperCase() + ".PIC");
            if (picXML != null)
              break;
            picXML = ClassLoader.getSystemClassLoader().getResourceAsStream("content/edc/masksets/" + fullPartNumber.toUpperCase() + ".PIC");
            if (picXML != null)
              break;
          }
        }
        if (picXML != null) break;
      }
    }
    if (picXML == null) {
      return null;
    }
    try
    {
      DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
      DocumentBuilder db = dbf.newDocumentBuilder();
      doc = db.parse(picXML);
      doc.getDocumentElement().normalize();
    }
    catch (ParserConfigurationException|SAXException|IOException ex)
    {
      Logger.getLogger(PICXMLLoader.class.getName()).log(Level.SEVERE, null, ex);
    }
    
    return doc;
  }
  
  public static Document LoadPICXML(File picFile)
  {
    Document doc = null;
    InputStream picXML = null;
    

    try
    {
      FileInputStream reader = new FileInputStream(picFile);
      picXML = reader;
    }
    catch (FileNotFoundException ex)
    {
      Logger.getLogger(PICXMLLoader.class.getName()).log(Level.SEVERE, null, ex);
    }
    
    if (picXML == null) {
      return null;
    }
    try
    {
      DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
      DocumentBuilder db = dbf.newDocumentBuilder();
      doc = db.parse(picXML);
      doc.getDocumentElement().normalize();
    }
    catch (ParserConfigurationException|SAXException|IOException ex)
    {
      Logger.getLogger(PICXMLLoader.class.getName()).log(Level.SEVERE, null, ex);
    }
    
    return doc;
  }
}
