/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package cdaoexplorer.model.annotation;

import java.net.URI;
import java.util.HashMap;
import java.util.Set;

/**
 *
 * @author bchisham
 */
public class AnnotationSourceCollection {
    private HashMap< String, Set< URI > > model;

    public AnnotationSourceCollection(  ){
        this.model = new HashMap();
    }

    public void setModel(HashMap<String, Set<URI>> model){
        this.model = model;
    }

    public void addSource(String key, Set<URI> properties){
        this.model.put(key, properties);
    }

    public Set< URI > getTerms( String key){
        return this.model.get(key);
    }

    public Set< String > getSource(){
        return this.model.keySet();
    }
    
}
