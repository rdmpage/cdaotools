/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package phylipalrequin.writers;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.URL;
import phylipalrequin.model.DataModel;
import phylipalrequin.model.MatrixDataModel;

/**
 *
 * @author bchisham
 */
public class PhylipMatrixWriter implements Writer {

    public void write(File writeTo, DataModel dm) throws FileNotFoundException {

        MatrixDataModel mdm;

        if ( dm instanceof MatrixDataModel ){
            mdm = (MatrixDataModel) dm;
            OutputStream os = new FileOutputStream( writeTo );
            PrintWriter pw = new PrintWriter( os );
            pw.println( mdm.getNTax() + " " + mdm.getNChar() );
            for (int i = 0; i < mdm.getNTax(); ++i){
                pw.print( mdm.getTaxonName(i).substring(0, 10)  );
                for ( int j = 0; j < mdm.getNChar(); ++j ){
                    pw.print( mdm.getCharacterState(i, j) );
                }
                pw.println();
            }
            pw.close();
        }
    }

}
