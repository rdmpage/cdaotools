/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package cdaoexplorer.model.annotation;

import java.net.URL;

/**
 *
 * @author bchisham
 */
public class ObjectProperty implements Triple {
    private URL subject;
    private URL property;
    private URL object;
    /**
     *
     * @return
     */
    public URL getSubject() {
        return this.subject;
    }
    /**
     *
     * @param subject
     */
    public void setSubject(URL subject) {
        this.subject = subject;
    }
    /**
     *
     * @return
     */
    public URL getProperty() {
        return this.property;
    }
    /**
     *
     * @param property
     */
    public void setProperty(URL property) {
        this.property = property;
    }
    public Object getObject() {
        return object;
    }
    /**
     *
     * @param object
     */
    public void setObject(Object object) {
        assert( object.getClass() == URL.class );
        this.object = (URL)object;
    }

   



}
