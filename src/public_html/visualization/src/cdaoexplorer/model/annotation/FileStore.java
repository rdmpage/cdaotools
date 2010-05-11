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
public class FileStore implements AnnotationCollection {
    StoreConfig location;
    MemoryStore tempStore;

    public FileStore(){
        this.tempStore = new MemoryStore();
    }

    public FileStore( MemoryStore ms ){
        this.tempStore = ms;
    }

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

    public Set<URL> getSubjects() {
        return this.tempStore.getSubjects();
    }

}
