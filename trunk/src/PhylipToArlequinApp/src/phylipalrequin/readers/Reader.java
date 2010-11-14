/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package phylipalrequin.readers;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URI;
import phylipalrequin.model.DataModel;

/**
 *
 * @author bchisham
 */
public interface Reader {

    DataModel readModel( URI modelLocation, URI modelType ) throws MalformedURLException, IOException;

}
