/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.net.URI;

/**
 * Represents an annotation.
 * @author bchisham
 */
public interface Annotation extends Cloneable {
    URI getSubject();
    /**
     * The the property of the annotation.
     * @return
     */
    URI getProperty();
    /**
     * Get the value of the annotation
     * @return
     */
    String getValue();
    URI getValueUrl();
    /**
     * Set the annotation's property
     * @param property
     */
    void setProperty( URI property );
    /**
     * Set the annotation's value.
     * @param value
     */
    void setValue( URI value );

    Annotation clone();
}
