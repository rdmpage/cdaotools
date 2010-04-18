/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.util.List;
import java.util.ListIterator;
import java.util.Set;
//import javax.media.jai.util.Range;

/**
 *
 * @author bchisham
 */
public interface Matrix extends Annotable {
    List< MatrixDatum > getRow( int i );
    List< MatrixDatum > getRow( String name );
    List< MatrixDatum > getColumn( int i );
    List< MatrixDatum > getColumn( String name );
    Set< String > getUniqueValues();
    MatrixDatum getDatum(int row, int column);
    ListIterator getRowIterator();
    ListIterator getColumnIterator();

    ListIterator getRowIterator(int row);
    ListIterator getColumnIterator(int col);

    int getrowcount();
    int getcolumncount();

    String getRowLabel( int row );
    String getColumnLabel( int col);

    void read( java.io.InputStream input );
    void write( java.io.OutputStream output );

    Matrix copy();
    Matrix extractRange( Range rows, Range cols);

    
}
