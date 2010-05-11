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
public interface AnnotationCollection {
    /**
     * Specify how to connect to the triplestore
     * associated with this collection.
     * @param tripleStore
     */
    public void setStoreConfig( StoreConfig tripleStore );
    /**
     * Add an annotation to the collection.
     * @param annotation
     */
    public void addAnnotation( Triple annotation );
    /**
     * Get the annotations for the specified subject.
     * @param subject
     * @return
     */
    public List< Triple > getAnnotations(URL subject );

    public Set< URL > getSubjects();
    /**
     * Save the current annotation collection to the store.
     * Do nothing if a default store has not been set.
     */
    public void commit(  );
    /**
     * Commit to the specified store.
     * @param tripleStore
     */
    public void commit( StoreConfig tripleStore );
}
