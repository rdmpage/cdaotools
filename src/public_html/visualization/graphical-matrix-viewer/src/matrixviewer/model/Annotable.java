/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.util.List;

/**
 *
 * @author bchisham
 */
public interface Annotable extends List {
    void addAnnation(Annotation annotation);
    Annotation getAnnotation( int i );
}
