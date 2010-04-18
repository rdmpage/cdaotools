/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.view;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import javax.swing.JPanel;
import matrixviewer.model.Matrix;
import matrixviewer.model.MatrixDatum;
import matrixviewer.model.Range;
import matrixviewer.model.RangeSet;
 
/**
 * Visual representation of a character state data matrix.
 * @author bchisham
 */
public class MatrixView extends JPanel implements MouseMotionListener {
    private Matrix model;
    private DrawableDatum rederer = new DrawableDatum();
    private int cellwidth;
    private int cellheight;
    private int key_entry_block_size;
    private int row_label_offset;
    private MatrixDatumMetaDataView metaView;
    private RangeSet highlight_rows;
    private RangeSet highlight_cols;

    public MatrixView(){
        this.model = null;
        this.cellheight = 0;
        this.cellwidth = 0;
        this.rederer = null;
        this.highlight_cols = new RangeSet();
        this.highlight_rows = new RangeSet();
        this.initComponents();
        

    }
    /**
     * Specify the model being drawn.
     * @param model
     */
    public MatrixView( Matrix model ){
        this.model = model;
        if (model != null){
          this.cellwidth = this.getWidth()/this.model.getcolumncount();
          this.cellheight = this.getHeight()/this.model.getrowcount();
          this.rederer = new DrawableDatum( this.model.getUniqueValues() );
          this.initComponents();
        }
    }

    private void initComponents(){
        this.addMouseMotionListener(this);
        this.addMouseListener(new MouseListener(){

            public void mouseClicked(MouseEvent e) {
                handleMouseClicked(e);
            }

            public void mousePressed(MouseEvent e) {
                
            }

            public void mouseReleased(MouseEvent e) {
                
            }

            public void mouseEntered(MouseEvent e) {
                
            }

            public void mouseExited(MouseEvent e) {
                
            }
        });
        this.metaView = new MatrixDatumMetaDataView();
    }

    /**
     * (re)set the matrix being displayed
     * @param model
     */
    public void setMatrix( Matrix model ){
        if (model != null && model.getrowcount() > 0 && model.getcolumncount() > 0){
            this.model = model;
            this.cellheight = 15;
            this.cellwidth = 15;
            this.row_label_offset = 0;//150;
            this.key_entry_block_size = 0;//this.cellheight * this.model.getUniqueValues().size();
            this.rederer = new DrawableDatum( this.model.getUniqueValues() );
            //this.setSize( this.model.getcolumncount() * this.cellwidth + this.row_label_offset, this.model.getrowcount() * this.cellheight + this.key_entry_block_size);
            this.setPreferredSize( new Dimension( this.model.getcolumncount() * this.cellwidth + this.row_label_offset, this.model.getrowcount() * this.cellheight + this.key_entry_block_size ) );
            //this.invalidate();
            //this.repaint();
        }
        else {
            this.model = null;
            this.rederer = null;
            this.setPreferredSize( new Dimension( 400,400 ));
            //this.invalidate();
            //this.repaint();
        }
        this.repaint();
    }
    /**
     * Get the model associated with this matrix view
     * @return
     */
    public Matrix getMatrix(){
        return this.model;
    }

    public void setHighlightedRange( Range row_range, Range col_range ){
        this.highlight_cols.add(col_range);
        this.highlight_rows.add(row_range);
    }

    public void clearHighlights(){
        this.highlight_cols.clear();
        this.highlight_rows.clear();
    }

    public void handleMouseClicked( java.awt.event.MouseEvent evt ){
        this.metaView.setMatrixDatum( this.getDatum(evt.getX(), evt.getY()) );
        this.metaView.setVisible(true);
        
    }

    public MatrixView(Matrix model, int cellwidth, int cellheight, int key_entry_block_size, int row_label_offset, RangeSet highlight_rows, RangeSet highlight_cols) {
        this.model = model;
        this.cellwidth = cellwidth;
        this.cellheight = cellheight;
        this.key_entry_block_size = key_entry_block_size;
        this.row_label_offset = row_label_offset;
        this.highlight_rows = highlight_rows;
        this.highlight_cols = highlight_cols;
    }

    private int coordinate_to_row_number(int y){
        int ret = (y / this.cellheight);
        return  ret > 0? ret : 0 ;
    }

    private int row_number_to_coordinate(int row){
        return (row)*this.cellheight;
    }

    private int coordinate_to_col_number(int x){
        int ret = (x/ this.cellwidth);
        return ret > 0 ? ret : 0 ;
    }

    private int col_number_to_coordinate(int col){
        return (col+1)*this.cellwidth + this.row_label_offset;
    }

    private MatrixDatum getDatum(int x, int y){
        return this.model.getDatum( coordinate_to_col_number(y), coordinate_to_row_number(x));
    }
    /**
     * Draw the datum.
     * @param g
     */
    @Override
    public void paint( Graphics g ){
        if (this.model != null && 
                this.model.getcolumncount() > 0 &&
                this.model.getrowcount() > 0 ){
        
            


            g.setColor(Color.black);
            //Draw the column labels.
            
            //Draw the matrix
            g.setColor(Color.black);
            for (int row = 0; row < model.getrowcount(); ++row){
                //Draw the row label
                g.setColor(Color.black);
               // System.err.println( "Row[" + row + "]: " +this.model.getRowLabel(row) );

                //g.drawString( this.model.getRowLabel(row) , 0, /*col_label_offset+*/(row+1)*cellheight + cellheight /2);

                for (int col = 0; col < model.getcolumncount(); ++col ){
                    g.setColor( this.rederer.getColor( this.model.getDatum(row, col) ) );
                    //Draw the cell.
                    if ( this.highlight_cols.contains( col ) && this.highlight_rows.contains(row)  ){

                        g.fill3DRect(row_label_offset + (col/*+1*/)* cellheight,
                                  /*this.key_entry_block_size +*/ (row/*+1*/)*cellwidth,
                                  cellwidth,
                                  cellheight, true);
                    }
                    else {
                        g.fillRect(row_label_offset + (col+1)* cellheight,
                                  /*this.key_entry_block_size +*/ (row+1)*cellwidth,
                                  cellwidth,
                                  cellheight);
                    }

                    //g.setColor(Color.black);
                    //g.drawString( this.model.getDatum( row, col).getvalue(), row_label_offset +(row)*cellwidth + (row*cellwidth)/4, col_label_offset+ col*cellheight  + (col*cellheight)/4 );
                }
            }
        }
        else {
            String noneLoaded = "No Matrix Loaded";
            g.drawString( noneLoaded, this.getWidth()/2 - noneLoaded.length()/2 , this.getHeight()/2);
        }
    }

    public void mouseDragged(MouseEvent e) {

    }

    public void mouseMoved(MouseEvent evt) {
           //if (!evt.isConsumed() ){
            if (this.model != null){
                String tipText = this.model.getRowLabel( this.coordinate_to_row_number( evt.getY() ) ) + ", trait" + this.coordinate_to_col_number( evt.getX());
                this.setToolTipText(  tipText   );
                //System.err.println( tipText );
            }
        //}
        return;
    }
}
