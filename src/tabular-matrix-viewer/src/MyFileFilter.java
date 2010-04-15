

import javax.swing.filechooser.FileFilter;
import java.io.File;



class MyFileFilter extends FileFilter
{
   private String[] s;

   MyFileFilter(String[] sArg)
   {
      s = sArg;
   }

   // determine which files to display in the chooser
   public boolean accept(File fArg)
   {
      // if it's a directory, show it
      if (fArg.isDirectory())
      return true;

      // if the filename contains the extension, show it
      for (int i = 0; i < s.length; ++i)
         if (fArg.getName().toLowerCase().indexOf(s[i].toLowerCase()) > 0)
            return true;

      // filter out everything else
      return false;
   }

   public String getDescription()
   {
      String tmp = "";
      for (int i = 0; i < s.length; ++i)
      tmp += "*" + s[i] + " ";

      return tmp;
   }
}
