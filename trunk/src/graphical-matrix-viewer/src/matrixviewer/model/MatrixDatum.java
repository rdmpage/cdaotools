/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.util.List;

/**
 *
 * @author bchisham
 */
public interface MatrixDatum extends Annotable {
    int getrow();
    void setRowName(String name);
    String getRowName();
    int getcolumn();
    void setColumnName(String name);
    String getColumnName();
    String getvalue();
    List<String> getAnnotations();
    void setAnnotations(List< String> annotations);
}
