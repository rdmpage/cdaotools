/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package cdaoexplorer.model.annotation;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author bchisham
 */
public class FileStoreConfig implements StoreConfig {
    private File storeLocation;
    private String authInfo;

    FileStoreConfig(){
        this.authInfo = "";
    }

    public URL getStoreLocation() {
        try {
            if (storeLocation != null) {
                return storeLocation.toURL();
            }
            return new URL("file:///no_file_set.xml");
        } catch (MalformedURLException ex) {
            Logger.getLogger(FileStoreConfig.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }

    public void setStoreLocation(URL storeLocation) {
        try {
            this.storeLocation = new File(storeLocation.toURI());
        } catch (URISyntaxException ex) {
            Logger.getLogger(FileStoreConfig.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public String getStoreAuthenticationInfo() {
        return this.authInfo;
    }

    public void setStoreAuthenticationInfo(String info) {
        this.authInfo = info;
    }

}
