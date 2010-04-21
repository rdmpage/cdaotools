/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.util.List;

/**
 * Represents a single cell in a matrix.
 * @author bchisham
 */
public interface MatrixDatum extends Annotable, Cloneable {
    /**
     * Get the row number this cell belongs to.
     * @return
     */
    int getrow();
    /**
     * Set the row label for this cell.
     * @param name
     */
    void setRowName(String name);
    /**
     * Get the row lable for this cell.
     * @return
     */
    String getRowName();
    /**
     * Get the column number of this cell.
     * @return
     */
    int getcolumn();
    /**
     * Set the name of this column
     * @param name
     */
    void setColumnName(String name);
    /**
     * Get the name of the column this cell belongs to.
     * @return
     */
    String getColumnName();
    /**
     * Get the value of this cell.
     * @return
     */
    String getvalue();

    void setvalue(String value);

    /**
     * Get the annotations appllied to this cell.
     * @return
     */
    List< Annotation > getAnnotations();
    /**
     * Set the annoatations applied to this cell
     * @param annotations
     */
    void setAnnotations(List< Annotation > annotations);



    MatrixDatum clone();
}
