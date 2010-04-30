/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package cdaoexplorer.model.annotation;

import java.net.URL;
import java.util.HashMap;
import java.util.List;

/**
 *
 * @author bchisham
 */
public class MemoryStore implements AnnotationCollection {
    HashMap< URL, List< Triple > > store;

    public void setStoreConfig(StoreConfig tripleStore) {
         return;
    }

    public void addAnnotation(Triple annotation) {
        List< Triple > subjectList = store.get( annotation.getSubject() );
        subjectList.add(annotation);
    }

    public List<Triple> getAnnotations(URL subject) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void commit() {
        return;
    }

    public void commit(StoreConfig tripleStore) {
        return;
    }

}
