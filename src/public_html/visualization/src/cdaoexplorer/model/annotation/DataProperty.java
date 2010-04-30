
package cdaoexplorer.model.annotation;

import java.net.URL;

/**
 *
 * @author bchisham
 */
public class DataProperty implements Triple {
    private URL subject;
    private URL property;
    private Object object;
    /**
     *
     * @return
     */
    public Object getObject() {
        return object;
    }
    /**
     *
     * @param object
     */
    public void setObject(Object object) {
        this.object = object;
    }
    /**
     *
     * @return
     */
    public URL getProperty() {
        return property;
    }
    /**
     *
     * @param property
     */
    public void setProperty(URL property) {
        this.property = property;
    }
    /**
     *
     * @return
     */
    public URL getSubject() {
        return subject;
    }
    /**
     * 
     * @param subject
     */
    public void setSubject(URL subject) {
        this.subject = subject;
    }

}
