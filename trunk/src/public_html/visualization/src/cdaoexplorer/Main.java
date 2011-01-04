/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package cdaoexplorer;

import cdaoexplorer.forms.Loader;
import cdaoexplorer.forms.Workbench;
import prefuse.demos.CDAOview;

/**
 *
 * @author bchisham
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
       //Workbench workbench = new Workbench();
       System.err.println("Called with:");
            for (int i = 0; i < args.length; ++i){
                System.err.println("argument[" + i + "]: " + args[i]);
            }
       System.err.println("--End of Argument list--");
            //Workbench.main(args);
            Loader.main(args);
        
        
    }

}
