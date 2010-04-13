/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package prefuse.demos;

import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.event.ListDataListener;

/**
 *
 * @author bchisham
 */
public class TreeNameListModel implements javax.swing.ListModel {
    private List<String> data;
    private List<ListDataListener> selected;
    private List listeners;
    private static final String TREE_SERVICE_URI_BASE = "http://www.cs.nmsu.edu/~bchisham/cgi-bin/phylows/listing";
    private int start;
    private int limit;
    TreeNameListModel(){
        data = new ArrayList();
        listeners = new ArrayList();
        selected = new ArrayList();
        start = 0;
        limit = 50;
        this.updateModel();


    }

    TreeNameListModel( int limit ){
        assert( limit > 0 );
        this.data = new ArrayList();
        this.listeners = new ArrayList();
        this.selected = new ArrayList();
        this.start = 0;
        this.limit = limit;
        this.updateModel();
    }

    public void nextPage(){
        this.start += this.limit;
        this.data.clear();
        this.updateModel();
    }

    public void prevPage(){
        this.start -= this.limit;
        if (this.start < 0){ this.start = 0; }
        this.data.clear();
        this.updateModel();
    }

    public void setLimit(int limit){
        this.limit = limit;
    }

    public int getLimit(){ return limit; }

    public void updateModel(){
        try {
            URL dataFeed = new URL(TREE_SERVICE_URI_BASE + "/" + start + "/" + limit);
            Scanner reader = new Scanner( dataFeed.openStream());
            while (reader.hasNext()){
                this.data.add( reader.next() );
            }
        } catch (IOException ex) {
            Logger.getLogger(TreeNameListModel.class.getName()).log(Level.SEVERE, null, ex);
        }


    }

    public int getSize() {
        return data.size();
    }

    public Object getElementAt(int index) {
        return data.get(index);
    }

    public void addListDataListener(ListDataListener l) {
        listeners.add(l);
    }

    public void removeListDataListener(ListDataListener l) {
        listeners.remove(l);
    }

}
