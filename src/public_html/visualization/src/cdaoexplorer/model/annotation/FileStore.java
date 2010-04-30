/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package cdaoexplorer.model.annotation;

import java.net.URL;
import java.util.List;

/**
 *
 * @author bchisham
 */
public class FileStore implements AnnotationCollection {
    StoreConfig location;
    MemoryStore tempStore;

    public void setStoreConfig(StoreConfig tripleStore) {
        this.location = tripleStore;
    }

    public void addAnnotation(Triple annotation) {
        tempStore.addAnnotation(annotation);
    }

    public List<Triple> getAnnotations(URL subject) {
        return tempStore.getAnnotations( subject );
    }

    public void commit() {
        return;
    }

    public void commit(StoreConfig tripleStore) {
        return;
    }

}
