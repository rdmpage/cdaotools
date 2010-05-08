/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package simplegraphmlparser;

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
        try {
            PhyloTree pt = new PhyloTree();
            System.err.println("Preparing to read the tree");
            if (args.length > 0){
                pt.parse( new URL(args[0]) );
            } else {
                pt.parse(new URL("http://www.cs.nmsu.edu/~bchisham/cgi-bin/phylows/tree/Tree3099?format=graphml"));
            }
            System.err.println("Parsing complete -- Printing...");
            //pt.printTree( System.err );
            System.err.println("Dumping GraphML");
            pt.printGraphML( System.out );
        } catch (IOException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

}
