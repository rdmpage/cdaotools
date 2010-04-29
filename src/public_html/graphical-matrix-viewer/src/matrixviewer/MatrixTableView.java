/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer;

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
    
}
