
package matrixviewer.model;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.Set;

/**
 * Implements a set of ranges.
 * @author bchisham
 */
public class RangeSet implements Set {
    ArrayList< Range > ranges;
    /**
     * Create an empty range set.
     */
    public RangeSet(){
        ranges = new ArrayList();
    }

    public RangeSet(int y, int y0) {
        ranges = new ArrayList();
    }
    /**
     * Get the size of the set.
     * @return
     */
    public int size() {
        return ranges.size();
    }
    /**
     * See if the set is empty.
     * @return
     */
    public boolean isEmpty() {
        return this.size() == 0;
    }
    
    public boolean contains( int pele ){
        Iterator<Range> it = this.ranges.iterator();
        boolean ret = false;
        while (it.hasNext() && (ret = it.next().contains(pele) )){}
        return ret;
    }

    /**
     * See if the range is in the set.
     * @param o
     * @return
     */
    public boolean contains(Object o) {
        if (o.getClass() == Range.class){
          return this.ranges.contains((Range)o);
        }
        return false;
    }
    /**
     * Get an iterator to the set.
     * @return
     */
    public Iterator iterator() {
        return this.ranges.iterator();
    }
    /**
     * Convert the the set to an array.
     * @return
     */
    public Object[] toArray() {
        return this.ranges.toArray();
    }
    /**
     * Convert the set to an array.
     * @param a
     * @return
     */
    public Object[] toArray(Object[] a) {
        return this.ranges.toArray(a);
    }
    /**
     * Add a new range to the set.
     * @param o
     * @return
     */
    public boolean add(Object o) {
        if ( o.getClass() == Range.class ){
          return this.ranges.add( (Range)o );
        }
        return false;
    }
    /**
     * Remove a range from the set.
     * @param o
     * @return
     */
    public boolean remove(Object o) {
        return this.ranges.remove( (Range)o );
    }
    /**
     * Determine if the collection is contained in this set.
     * @param c
     * @return
     */
    public boolean containsAll(Collection c) {
        return this.ranges.containsAll(c);
    }
    /***
     * Add the specified collection to the set.
     * @param c
     * @return
     */
    public boolean addAll(Collection c) {
        return this.ranges.addAll(c);
    }
    /**
     * Retain the specified collection discarding the remainder of the set
     * @param c
     * @return
     */
    public boolean retainAll(Collection c) {
        return this.ranges.retainAll(c);
    }
    /**
     * Remove the specified elements from the set
     * @param c
     * @return
     */
    public boolean removeAll(Collection c) {
        return this.ranges.removeAll(c);
    }
    /**
     * Remove all elements from the set.
     */
    public void clear() {
        this.ranges.clear();
    }

}
