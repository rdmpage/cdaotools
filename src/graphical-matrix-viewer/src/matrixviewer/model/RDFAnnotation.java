/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author bchisham
 */
public class RDFAnnotation implements Annotation{
    URI subject;
    URI property;
    URI object;

    public RDFAnnotation(){

    }

    public RDFAnnotation(URI subject, URI property, URI object) {
        this.subject = subject;
        this.property = property;
        this.object = object;
    }

    

    public URI getSubject() {
       
            return subject;
        
    }

    public URI getProperty() {
        
            return property;
        
    }

    public String getValue() {
        return object.toString();
    }

    public URI getValueUrl() {
        
            return object;
        
    }

    public void setProperty(URI property) {

    }

    public void setValue(URI value) {
        this.object = value;
    }

    @Override
    public Annotation clone() {
        return new RDFAnnotation(this.subject, this.property, this.object);
    }

}
