
package cdaoexplorer.model.annotation;

import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import javax.swing.DefaultListModel;

/**
 *
 * @author bchisham
 */
public class PropertyListModel {
    //HashMap< URL, Triple > model;
    AnnotationCollection model;
    DefaultListModel subs;
    DefaultListModel props;
    DefaultListModel vals;
    public PropertyListModel( ){
        //this.model = new HashMap();
        this.model = new MemoryStore();
        subs = new DefaultListModel();
        props = new DefaultListModel();
        vals = new DefaultListModel();

    }

    public DefaultListModel getSubjects(){
        return this.subs;
    }

    public DefaultListModel getProperties(){
        return this.props;
    }

    public DefaultListModel getValues(){
        return this.vals;
    }

    public void addAnnotation( Triple item ){
        this.model.addAnnotation( item );
        this.subs.addElement( item.getSubject() );
        this.props.addElement( item.getProperty() );
        this.vals.addElement( item.getObject() );

    }
    public AnnotationCollection getAnnotations(){
        return this.model;
    }
}
