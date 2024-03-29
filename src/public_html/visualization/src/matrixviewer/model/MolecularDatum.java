/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

/**
 *
 * @author bchisham
 */
public class MolecularDatum implements MatrixDatum {
    private int rowno;
    private int colno;
    private String colname;
    private String rowname;
    private String value;
    private List< Annotation > annotations;

    public MolecularDatum(int rowno, int colno, String value) {
        this.rowno = rowno;
        this.colno = colno;
        this.value = value;
        this.annotations = new ArrayList();
    }

    public MolecularDatum(int rowno, int colno, String value, List<Annotation> annotations) {
        this.rowno = rowno;
        this.colno = colno;
        this.value = value;
        this.annotations = annotations;
    }
    
    public int getrow() {
        return rowno;
    }

    public int getcolumn() {
        return colno;
    }

    public String getvalue() {
        return value;
    }

    public void addAnnation(Annotation annotation) {
        annotations.add(annotation);
    }

    public Annotation getAnnotation(int i) {
        return annotations.get(i);
    }

    public int size() {
        return annotations.size();
    }

    public boolean isEmpty() {
        return annotations.isEmpty();
    }

    public boolean contains(Object o) {
        return annotations.contains(o.toString());
    }

    public Iterator iterator() {
        return annotations.iterator();
    }

    public Object[] toArray() {
        return annotations.toArray();
    }

    public Object[] toArray(Object[] a) {
        return annotations.toArray(a);
    }

    public boolean add(Object o) {
        //return annotations.add( o );
        return false;
    }

    public boolean remove(Object o) {
        //return annotations.remove(o);
        return false;
    }

    public boolean containsAll(Collection c) {
        return annotations.containsAll(c);
    }

    public boolean addAll(Collection c) {
        return annotations.addAll(c);
    }

    public boolean addAll(int index, Collection c) {
        return annotations.addAll(index, c);
    }

    public boolean removeAll(Collection c) {
        return annotations.removeAll(c);
    }

    public boolean retainAll(Collection c) {
        return annotations.retainAll(c);
    }

    public void clear() {
        annotations.clear();
    }

    public Object get(int index) {
        return annotations.get(index);
    }

    public Object set(int index, Object element) {
        //return annotations.set(index, element);
        return element;
    }

    public void add(int index, Object element) {
        //annotations.add(index, element.toString());
    }

    public Object remove(int index) {
        return annotations.remove( index );
    }

    public int indexOf(Object o) {
        return annotations.indexOf(o);
    }

    public int lastIndexOf(Object o) {
        return annotations.lastIndexOf(o);
    }

    public ListIterator listIterator() {
        return annotations.listIterator();
    }

    public ListIterator listIterator(int index) {
        return annotations.listIterator(index);
    }

    public List subList(int fromIndex, int toIndex) {
        return annotations.subList(fromIndex, toIndex);
    }

    @Override
    public String toString(){

        return this.getvalue();

    }

    public String getRowName() {
        return this.rowname;
    }

    public String getColumnName() {
        return this.colname;
    }

    public List<Annotation> getAnnotations() {
        return this.annotations;
    }

    public void setAnnotations(List<Annotation> annotations) {
        this.annotations = annotations;
    }

    public void setRowName(String name) {
        this.rowname = name;
    }

    public void setColumnName(String name) {
        this.colname = name;
    }

    public MatrixDatum clone() {
        String ret_value = this.value;
        List<Annotation> ret_annotations = new ArrayList< Annotation >();
        Iterator< Annotation > ait = this.annotations.iterator();
        while (ait.hasNext()){
            ret_annotations.add( ait.next().clone() );
        }

        MolecularDatum ret = new MolecularDatum(this.rowno, this.colno, this.value, ret_annotations);

        return ret;
    }

    public void setvalue(String value) {
        this.value = value;
    }
}
