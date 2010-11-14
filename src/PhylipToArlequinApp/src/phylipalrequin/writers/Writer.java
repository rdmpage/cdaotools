/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package phylipalrequin.writers;

import java.io.File;
import java.io.FileNotFoundException;
import java.net.URL;
import phylipalrequin.model.DataModel;

/**
 *
 * @author bchisham
 */
public interface Writer {

    void write( File writeTo, DataModel dm ) throws FileNotFoundException;

}
