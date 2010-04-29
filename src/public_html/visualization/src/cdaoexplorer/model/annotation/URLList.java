

package cdaoexplorer.model.annotation;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import javax.swing.event.ListDataListener;

/**
 *
 * @author bchisham
 */
public class URLList implements javax.swing.ComboBoxModel {
    private List< URL > list;
    private List listeners;
    private int selectedItem;
    /**
     * Create an empty list of urls.
     */
    public URLList(){
        this.list = new ArrayList( );
        this.listeners = new ArrayList();
        this.selectedItem = -1;
    }
    /**
     * Specify the initial set of urls.
     * @param model
     */
    public URLList( List< URL > model ){
        this.list = model;
        this.listeners = new ArrayList();
        this.selectedItem = 0;
    }
    /**
     * Add a new url to the list.
     * @param url
     */
    public void addItem(URL url){
        this.list.add(url);
    }
    /**
     * Set the selected item
     * @param anItem
     */
    public void setSelectedItem(Object anItem) {
        this.selectedItem = this.list.indexOf(anItem);
    }
    /**
     * Get the selected item.
     * @return
     */
    public Object getSelectedItem() {
        return list.get(selectedItem);
    }
    /**
     * Get the number of items in the list
     * @return
     */
    public int getSize() {
        return this.list.size();
    }
    /**
     * Get the element at the specified position.
     * @param index
     * @return
     */
    public Object getElementAt(int index) {
        return this.list.get(index);
    }
    /**
     * Add a list data listener
     * @param l
     */
    public void addListDataListener(ListDataListener l) {
        this.listeners.add(l);
    }
    /**
     * Remove a list data listener.
     * @param l
     */
    public void removeListDataListener(ListDataListener l) {
        this.listeners.remove(l);
    }

}
