/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package phylipalrequin.model;

import java.net.URI;

/**
 * Abstract features of a data model.
 * @author bchisham
 */
public interface DataModel {
    /**
     * Get the name of the data source
     * @return
     */
    URI getName();
    /**
     * Get the type of data model.
     * @return
     */
    URI getDataModelType();
    /**
     * Get the type of data.
     * DNA/RNA/Protein/Continuous/Standard
     * @return
     */
    URI getDataType();

}
