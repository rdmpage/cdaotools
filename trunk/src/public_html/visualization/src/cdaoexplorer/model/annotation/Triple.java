
package cdaoexplorer.model.annotation;

import java.net.URL;

/**
 *
 * @author bchisham
 */
public interface Triple {
    URL getSubject();
    void setSubject(URL subject  );
    URL getProperty();
    void setProperty(URL property  );
    Object getObject();
    void setObject( Object object );
}
