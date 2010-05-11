/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package cdaoexplorer.model.annotation;

import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;

/**
 *
 * @author bchisham
 */
public class MemoryStore implements AnnotationCollection {
    HashMap< URL, List< Triple > > store;

    public MemoryStore(){
        this.store = new HashMap();
    }

    public void setStoreConfig(StoreConfig tripleStore) {
         return;
    }

    public void addAnnotation(Triple annotation) {
        List< Triple > subjectList = store.get( annotation.getSubject() );
        if ( subjectList == null ){
            subjectList = new ArrayList< Triple >();
        }
        subjectList.add(annotation);
        this.store.put( annotation.getSubject()  , subjectList);
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

    public Set<URL> getSubjects() {
        return this.store.keySet();
    }

}
