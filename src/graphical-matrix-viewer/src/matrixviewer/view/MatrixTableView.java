/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.view;

import javax.swing.JTable;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import javax.swing.JPanel;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionListener;
import matrixviewer.model.Matrix;
import matrixviewer.model.MatrixDatum;
import matrixviewer.model.Range;
import matrixviewer.model.RangeSet;
import matrixviewer.view.MatrixDatumMetaDataView;
/**
 *
 * @author bchisham
 */
public class MatrixTableView extends JTable {
    Matrix model;
    private MatrixDatumMetaDataView metaView;
    public MatrixTableView(){
        this.model = null;
        this.metaView = new MatrixDatumMetaDataView();
    }
    public MatrixTableView( Matrix model ){
        this.model = model;
        this.metaView = new MatrixDatumMetaDataView();
    }

    public void setMatrix( Matrix model ){
        this.model = model;
        this.setModel( this.model );
    }
    
}
