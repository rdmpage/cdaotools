/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
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
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.swing.event.TableModelListener;
//import javax.media.jai.util.Range;

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
    private List< Annotation > annotations;

    private ArrayList< TableModelListener > tml;
    
    public CSVMatrix(){
        this.row_names = new ArrayList();
        this.col_names = new ArrayList();
        this.columns = new ArrayList();
        this.rows = new ArrayList();
        this.uniques = new TreeSet();
        this.annotations = new ArrayList();
        this.tml = new ArrayList();
        
    }

    public CSVMatrix(ArrayList<String> row_names,
                     ArrayList<String> col_names,
                     ArrayList<List<MatrixDatum>> rows,
                     List<Annotation> annotations) {
        this.row_names = row_names;
        this.col_names = col_names;
        this.rows = rows;
        this.annotations = annotations;
        this.uniques = new TreeSet();
        this.transpose();
    }


    private void transpose(){

        this.columns = new ArrayList( this.col_names.size());
        for (int i = 0; i < this.col_names.size(); ++i){
            this.columns = new ArrayList( this.row_names.size() );
        }
        Iterator< List< MatrixDatum > > rit = this.rows.iterator();
        while (rit.hasNext()){
            Iterator< MatrixDatum > colit = rit.next().iterator();
            while ( colit.hasNext() ){
                MatrixDatum md = colit.next();
                this.columns.get( md.getcolumn()).set( md.getrow() , md);
            }
        }
    }


    public List<MatrixDatum> getRow(int i) {
        return rows.get(i);
    }

    public List<MatrixDatum> getRow(String name) {
        return rows.get( row_names.indexOf(name) );
    }

    public List<MatrixDatum> getColumn(int i) {
        ArrayList< MatrixDatum> ret= new ArrayList();
        Iterator< List< MatrixDatum> > rit = rows.iterator();

        while(rit.hasNext()){
            ret.add( rit.next().get(i) );
        }

        return ret;
    }

    public List<MatrixDatum> getColumn(String name) {
        return this.getColumn( col_names.indexOf( name ) );
    }

    public MatrixDatum getDatum(int row, int column) {
        if (row < this.rows.size() && column < this.rows.get(row).size()){
             return rows.get(row).get(column);
        }
        return new MolecularDatum(row,column,"Not Defined!");
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

    public void addAnnation(Annotation annotation) {
        annotations.add(annotation);
    }

    public Annotation getAnnotation(int i) {
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

    protected void setModel( ArrayList<String> row_names,  ArrayList< List< MatrixDatum > > model ){
        this.rows = model;
        this.columns = new ArrayList< List< MatrixDatum > >();
        this.row_names = row_names;
        this.col_names = new ArrayList<String>();

        for (int i =0; i < this.rows.get(0).size(); ++i){ this.col_names.add("triat" + i); }

        this.uniques = new TreeSet< String >();
        for (int row = 0; row < this.rows.size(); ++row){
            for (int col = 0; col < this.rows.get(row).size(); ++col){
                if (!this.uniques.contains( this.rows.get(row).get(col).getvalue() )){
                    this.uniques.add( this.rows.get(row).get(col).getvalue() );
                }
            }
        }
    }

    public void read(InputStream input) {
        Scanner input_scanner = new Scanner( input);
        String current_line;
        Pattern line_tokenizer = Pattern.compile(",");
        col_names = new ArrayList<String>();
        row_names = new ArrayList<String>();
        //this.columns = new ArrayList< List< MatrixDatum > >();
        this.rows    = new ArrayList< List< MatrixDatum > >();
        this.uniques = new TreeSet< String >();
        //process the first line with the column names.
        Matcher mat = null;
        String[] split_line = null;

        int current_row = 0;
        int last_row_size = 0;
        while (input_scanner.hasNextLine()){
           current_line = input_scanner.nextLine();
           split_line = line_tokenizer.split(current_line);
           if ( col_names.size() == 0 ){
               //col_names.add("row_name");
              for ( int i = 0; i < split_line.length-1; ++i ){
                  col_names.add("trait"+i);
              }
           }

           if (current_row > 0){ assert( split_line.length == last_row_size  ); }

           row_names.add(split_line[0]);

           //System.err.println( "Extracted," + split_line.length +  ", fields" );

           //System.err.println("Row label: " + split_line[0]);
           ArrayList<MatrixDatum> row_data = new ArrayList();
           //rows.add(row_data);
           //System.err.println( current_line );
           for (int i = 1; i < split_line.length; ++i){
                  MolecularDatum md = new MolecularDatum( current_row, i-1,  split_line[i] );
                  md.setRowName(split_line[0]);
                  md.setColumnName( "trait"+ i );
                  row_data.add( md );
                  if (!uniques.contains( split_line[ i ] )){
                      uniques.add(split_line[i]);
                  }
                  //col_data[ current_row ].add( i , rows.get(current_row).get(i) );
                  //System.err.println( "Cell Data: " + split_line[i] );
           }
           last_row_size = row_data.size();
           rows.add( row_data );
           current_row++;
        }
        
    }

    public void read( File file ){
        try {
            read(new FileInputStream(file));
        } catch (FileNotFoundException ex) {
            Logger.getLogger(CSVMatrix.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void write(OutputStream output) {
        PrintWriter pw = new PrintWriter( output );
        //pw.print( col_names.get( 0 ) );
        //for (int i = 1; i < col_names.size();++i){
        //    pw.printf(",%s",  col_names.get(i) );
        //}
        //pw.println();
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

    public Matrix extractRange(Range rows, Range cols) {
        ArrayList< List< MatrixDatum > > ret_model = new ArrayList();
        ArrayList< String > ret_row_names = new ArrayList();
        CSVMatrix ret = new CSVMatrix();

        for (int row = rows.getMinValue(); row < rows.getMaxValue(); ++row){
            ret_model.add( new ArrayList() );
            ret_row_names.add( this.getRowLabel(row) );
            for (int col = cols.getMinValue(); col < cols.getMaxValue(); ++col){
                ret_model.get(row - rows.getMinValue() ).add( this.getDatum(row, col) );
            }
        }

        ret.setModel( ret_row_names, ret_model );
        return ret;
    }

    public int getRowCount() {
        return this.getrowcount();
    }

    public int getColumnCount() {
        return this.getcolumncount();
    }

    public String getColumnName(int columnIndex) {
        return this.getColumnLabel(columnIndex);
    }

    public Class<?> getColumnClass(int columnIndex) {
        return MatrixDatum.class;
    }

    public boolean isCellEditable(int rowIndex, int columnIndex) {
        return false;
    }

    public Object getValueAt(int rowIndex, int columnIndex) {
        return this.getDatum(rowIndex, columnIndex);
    }

    public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
        this.rows.get(rowIndex).set(columnIndex, (MatrixDatum)aValue );
    }

    public void addTableModelListener(TableModelListener l) {
        this.tml.add(l);
    }

    public void removeTableModelListener(TableModelListener l) {
        this.tml.remove(l);
    }

}
