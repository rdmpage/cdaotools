/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 *
 * @author bchisham
 */
public class CSVMatrix implements Matrix {

    
    private ArrayList< String > row_names;
    private ArrayList< String > col_names;

    private ArrayList < List < MatrixDatum > > columns;
    private ArrayList < List < MatrixDatum > > rows;
    private Set< String > uniques;
    private List< String > annotations;

    public List<MatrixDatum> getRow(int i) {
        return rows.get(i);
    }

    public List<MatrixDatum> getRow(String name) {
        return rows.get( row_names.indexOf(name) );
    }

    public List<MatrixDatum> getColumn(int i) {
        return columns.get(i);
    }

    public List<MatrixDatum> getColumn(String name) {
        return columns.get( col_names.indexOf( name ) );
    }

    public MatrixDatum getDatum(int row, int column) {
        return rows.get(row).get(column);
    }

    public ListIterator getRowIterator(){
        return rows.listIterator();
    }
    
    public ListIterator getColumnIterator(){
        return columns.listIterator();
    }

    public ListIterator getColumnIterator(int index){
        return rows.get(index).listIterator();
    }

    public ListIterator getRowIterator( int index){
        return columns.get(index).listIterator();
    }

    public void addAnnation(String annotation) {
        annotations.add(annotation);
    }

    public String getAnnotation(int i) {
        return annotations.get(i);
    }

    public int size() {
        return this.annotations.size();
    }

    public boolean isEmpty() {
        return rows.isEmpty();
    }

    public boolean contains(Object o) {
        return false;
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
        return false;
    }

    public boolean remove(Object o) {
        return false;
    }

    public boolean containsAll(Collection c) {
        return annotations.containsAll(c);    }

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
        rows.clear();
        columns.clear();
        annotations.clear();
    }

    public Object get(int index) {
        return annotations.get( index );
    }

    public Object set(int index, Object element) {
        return null;
    }

    public void add(int index, Object element) {
        return;
    }

    public Object remove(int index) {
        return null;
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

    public int getrowcount() {
        return row_names.size();
    }

    public int getcolumncount() {
        return col_names.size();
    }

    public String getRowLabel(int row) {
        return row_names.get( row );
    }

    public String getColumnLabel(int col) {
        return col_names.get(col);
    }

    public void read(InputStream input) {
        Scanner input_scanner = new Scanner( input);
        String current_line;
        Pattern line_tokenizer = Pattern.compile(",");
        col_names = new ArrayList<String>();
        row_names = new ArrayList<String>();
        this.columns = new ArrayList< List< MatrixDatum > >();
        this.rows    = new ArrayList< List< MatrixDatum > >();
        this.uniques = new TreeSet< String >();
        //process the first line with the column names.
        Matcher mat = null;
        String[] split_line = null;
        if ( input_scanner.hasNextLine() ){
            current_line = input_scanner.nextLine();
            split_line = line_tokenizer.split( current_line );
        }
        assert( split_line != null );
        for (int i = 0; i < split_line.length; ++i){
            col_names.add( split_line[ i ] );
        }
        ArrayList[] col_data = new ArrayList[ col_names.size() ];
           for (int i =0; i < col_names.size(); ++i){
               col_data[i] = new ArrayList();
        }
        int current_row = 0;
        while (input_scanner.hasNextLine()){
           current_line = input_scanner.nextLine();
           split_line = line_tokenizer.split(current_line);
           row_names.add(split_line[0]);
           ArrayList<MatrixDatum> row_data = new ArrayList();
           //rows.add(row_data);
           System.err.println( current_line );
           for (int i = 1; i < split_line.length; ++i){
                  row_data.add( new MolecularDatum( current_row, i-1,  split_line[i] ));
                  if (!uniques.contains( split_line[ i ] )){
                      uniques.add(split_line[i]);
                  }
                  //col_data[ current_row ].add( i , rows.get(current_row).get(i) );
                  //System.err.println( "Cell Data: " + split_line[i] );
           }
           rows.add( row_data );
        }
        current_row++;
    }

    public void write(OutputStream output) {
        PrintWriter pw = new PrintWriter( output );
        pw.print( col_names.get( 0 ) );
        for (int i = 1; i < col_names.size();++i){
            pw.printf(",%s",  col_names.get(i) );
        }
        pw.println();
        for (int row = 0; row < row_names.size(); ++row){
            pw.print( row_names.get(row) );
            for ( int col = 0; col < rows.get(row).size(); ++col ){
                pw.printf(",%s",  rows.get(row).get(col) );
            }
            pw.println();
        }
    }

    public Matrix copy(){
        Matrix ret = new CSVMatrix();
        return ret;
    }

    public Set<String> getUniqueValues() {
        return this.uniques;
    }


}
