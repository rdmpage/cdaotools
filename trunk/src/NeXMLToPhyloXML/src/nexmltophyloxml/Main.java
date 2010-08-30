/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package nexmltophyloxml;

import java.io.IOException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author bchisham
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.err.println("Starting Translation");
        try {
            NeXMLTree pt = new NeXMLTree();
            System.err.println("Preparing to read the tree");
            if (args.length > 0){
                pt.parse( new URL(args[0]) );
            } else {
                pt.parse( System.in );
            }
            System.err.println("Parsing complete -- Printing...");
            //pt.printTree( System.err );
            System.err.println("Dumping PhyloXML");
            pt.printPhyloXML( System.out );
        } catch (IOException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

}
