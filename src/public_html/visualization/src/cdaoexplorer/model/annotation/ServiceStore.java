/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package cdaoexplorer.model.annotation;

import java.net.URL;
import java.util.List;
import java.util.Set;

/**
 *
 * @author bchisham
 */
public class ServiceStore implements AnnotationCollection {

    public void setStoreConfig(StoreConfig tripleStore) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void addAnnotation(Triple annotation) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public List<Triple> getAnnotations(URL subject) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void commit() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void commit(StoreConfig tripleStore) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public Set<URL> getSubjects() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

}
