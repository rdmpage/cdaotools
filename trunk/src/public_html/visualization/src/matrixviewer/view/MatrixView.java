
/*
 * MatrixView.java
 *
 * Created on May 24, 2010, 10:01:56 PM
 */

package matrixviewer.view;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.util.Iterator;
import java.util.logging.Level;
import java.util.logging.Logger;
import matrixviewer.model.Matrix;
import matrixviewer.model.MatrixDatum;
import matrixviewer.model.Range;
import matrixviewer.model.RangeSet;

/**
 *
 * @author bchisham
 */
public class MatrixView extends javax.swing.JPanel implements MouseMotionListener {
    /**
     * Handles deferred paint requests for the matrix panel.
     */
    private class AsyncPainter implements Runnable {

        private MatrixView parent;
        //private Graphics g;
        private boolean paint_requested;
        /**
         * Specify the matrix associated with this painter
         * @param parent
         */
        public AsyncPainter( MatrixView parent ){
            this.parent = parent;
            this.paint_requested = false;
            //this.g = g;
        }
        /**
         * Idle loop
         */
        public void run() {
            while ( true ){
//                try {
//                    if (!this.paint_requested){
//                        Thread.sleep(250);
//                    }
//                } catch (InterruptedException ex) {
//                    Logger.getLogger(MatrixView.class.getName()).log(Level.SEVERE, null, ex);
//                }
            }
        }
        /**
         * Handle painting.
         * @param g
         */
        public void requestPaint( Graphics g ){
            this.paint_requested = true;
            //System.err.println( "Doing background paint!" );
            this.parent.async_paint(g);
            this.paint_requested = false;
            //this.g = g;
        }
    }

    private Matrix model;
    private DrawableDatum renderer;
    private int cellwidth;
    private int cellheight;
    private int key_entry_block_size;
    private int row_label_offset;
    private int x_offset;
    private int y_offset;
    private MatrixDatumMetaDataView metaView;
    private RangeSet highlight_rows;
    private RangeSet highlight_cols;
    private Range selected_rows;
    private Range selected_cols;
    private boolean selectionActive;
    private final static int MIN_TEXT_SIZE = 10;
    private final static int DEFAULT_BLOCK_SIZE = 15;
    private final static int MIN_3D_BLOCK_SIZE = 5;
    private AsyncPainter painter;
    private Thread painter_context;

    private int visible_left;
    private int visible_width;
    private int visible_top;
    private int visible_height;

    /** Creates new form MatrixView */
    public MatrixView() {
        this.model = null;
        this.renderer = new DrawableDatum();
        this.cellheight = 0;
        this.cellwidth = 0;
        this.renderer = null;
        this.x_offset = 0;
        this.y_offset = 0;
        this.highlight_cols = new RangeSet();
        this.highlight_rows = new RangeSet();
        this.selectionActive = false;
        this.visible_left = 0;
        this.visible_top = 0;
        initComponents();
        this.postinit();
    }

    
    /**
     * Specify the model being drawn.
     * @param model
     */
    public MatrixView(Matrix model) {
        this.model = model;
        if (model != null) {
            this.renderer = new DrawableDatum( this.model.getUniqueValues() );
            this.cellwidth = this.getWidth() / this.model.getcolumncount();
            this.cellheight = this.getHeight() / this.model.getrowcount();
            this.renderer = new DrawableDatum(this.model.getUniqueValues());
            this.x_offset = model.getXOffset();
            this.y_offset = model.getYOffset();
            this.selectionActive = false;
            this.initComponents();
            this.postinit();
        }
    }
    
    private void postinit(){
        this.addMouseMotionListener(this);
        
        this.metaView = new MatrixDatumMetaDataView();
    }
    
     /**
     * Scale up the matrix by the specified factor.
     * @param scaleFactor
     */
    public void scaleUp( int scaleFactor ){
        this.cellheight *= scaleFactor;
        this.cellwidth  *= scaleFactor;

        if (this.cellheight * this.model.getrowcount() > this.getMaximumSize().getHeight()){
            this.cellheight = (int)this.getMaximumSize().getHeight()/this.model.getrowcount();
        }
        if (this.cellwidth * this.model.getcolumncount() > this.getMaximumSize().getWidth()){
            this.cellwidth = (int)this.getMaximumSize().getWidth()/this.model.getcolumncount();
        }
        this.setPreferredSize( new Dimension(this.cellwidth * this.model.getcolumncount(), this.cellheight * this.model.getrowcount()) );
        this.invalidate();
        this.repaint();

    }
    /**
     * Scale down the matrix by the specified factor.
     * @param scaleFactor
     */
    public void scaleDown( int scaleFactor ){
        this.cellheight /= scaleFactor;
        this.cellwidth  /= scaleFactor;

        if (this.cellheight < 1){ this.cellheight = 1; }
        if (this.cellwidth < 1){ this.cellwidth = 1; }
        this.setPreferredSize( new Dimension(this.cellwidth * this.model.getcolumncount(), this.cellheight * this.model.getrowcount()) );
        this.invalidate();
        this.repaint();
    }
    /**
     * Resets the scale of the matrix to the default size.
     */
    public void resetScale(){
        this.cellheight = MatrixView.DEFAULT_BLOCK_SIZE;
        this.cellwidth = MatrixView.DEFAULT_BLOCK_SIZE;
        this.setPreferredSize( new Dimension(this.cellwidth * this.model.getcolumncount(), this.cellheight * this.model.getrowcount()) );
        this.invalidate();
        this.repaint();
    }

    /**
     * (re)set the matrix being displayed
     * @param model
     */
    public void setMatrix(Matrix model) {

        System.err.println("MatrixView::setMatrix");

        if (model != null && model.getrowcount() > 0 && model.getcolumncount() > 0) {

            //System.err.println("Dimensions, rows: " + model.getrowcount() + " cols: " + model.getcolumncount());

            this.model = model;
            this.cellheight = MatrixView.DEFAULT_BLOCK_SIZE;
            this.cellwidth = MatrixView.DEFAULT_BLOCK_SIZE;
            //this.row_label_offset = 0;//150;
            //this.key_entry_block_size = 0;//this.cellheight * this.model.getUniqueValues().size();
            this.x_offset = model.getXOffset();
            this.y_offset = model.getYOffset();
            this.renderer = new DrawableDatum(this.model.getUniqueValues());
            //this.setSize( this.model.getcolumncount() * this.cellwidth + this.row_label_offset, this.model.getrowcount() * this.cellheight + this.key_entry_block_size);
            this.setPreferredSize(new Dimension(this.model.getcolumncount() * this.cellwidth + this.row_label_offset, this.model.getrowcount() * this.cellheight + this.key_entry_block_size));
            this.selected_cols = new Range(0, this.model.getcolumncount());
            this.selected_rows = new Range(0, this.model.getrowcount());
            //this.invalidate();
            //this.repaint();
        } else {
            this.model = null;
            this.renderer = null;
            this.setPreferredSize(new Dimension(400, 400));
            //this.invalidate();
            //this.repaint();
        }
        this.repaint();
    }

    /**
     * Get the model associated with this matrix view
     * @return
     */
    public Matrix getMatrix() {
        return this.model;
    }
    /**
     * Set the range to highlight.
     * @param row_range
     * @param col_range
     */
    public void setHighlightedRange(Range row_range, Range col_range) {
        this.highlight_cols.add(col_range);
        this.highlight_rows.add(row_range);
    }
    /**
     * Clear all highlighted ranges.
     */
    public void clearHighlights() {
        this.highlight_cols.clear();
        this.highlight_rows.clear();
    }
    /**
     * Handle mouse click.
     * @param evt
     */
    public void handleMouseClicked(MouseEvent evt) {
        if (evt.getButton() == MouseEvent.BUTTON1) {
            if (this.model != null && this.model.getcolumncount() > 0 && this.model.getrowcount() > 0 && evt.getClickCount() > 1) {
                this.metaView.setMatrixDatum(this.getDatum(evt.getX(), evt.getY()));
                this.metaView.setVisible(true);
            }

            this.selectionActive = !this.selectionActive;
            this.startSelection(evt.getX(), evt.getY());
        } else if (evt.getButton() == MouseEvent.BUTTON2) {
            /**TODO: Show annotation for row or column */

        }
        //this.selected_rows = new Range( this.getY(), this.getY() );
        //this.selected_cols = new Range( this.getX(), this.getX() );

    }
    /**
     * Initialize a matrix with the specified cell sized and highlighted cells.
     * @param model
     * @param cellwidth
     * @param cellheight
     * @param key_entry_block_size
     * @param row_label_offset
     * @param highlight_rows
     * @param highlight_cols
     */
    public MatrixView(Matrix model, int cellwidth, int cellheight, int key_entry_block_size, int row_label_offset, RangeSet highlight_rows, RangeSet highlight_cols) {
        this.model = model;
        this.cellwidth = cellwidth;
        this.cellheight = cellheight;
        this.key_entry_block_size = key_entry_block_size;
        this.row_label_offset = row_label_offset;
        this.highlight_rows = highlight_rows;
        this.highlight_cols = highlight_cols;
    }
    /**
     * Start selecting a region.
     * @param X
     * @param Y
     */
    private void startSelection(int X, int Y) {
        //this.selectionActive = true;
        int crow = this.coordinate_to_row_number(Y);
        int ccol = this.coordinate_to_col_number(X);
        this.selected_rows = new Range(crow, crow + 1);
        this.selected_cols = new Range(ccol, ccol + 1);
    }
    /**
     * Convert a 'y' coordinate into a row number for the matrix.
     * @param y
     * @return
     */
    private int coordinate_to_row_number(int y) {
        int ret = ((y / this.cellheight) - this.y_offset );
        ret = ret > this.model.getrowcount() ? this.model.getrowcount() : ret;
        return ret > 0 ? ret : 0;
    }
    /**
     * convert a row number to a coordinate.
     * @param row
     * @return
     */
    private int row_number_to_coordinate(int row) {
        return (row + this.y_offset) * this.cellheight;
    }
    /**
     * convert an 'x' coordinage to a column number
     * @param x
     * @return
     */
    private int coordinate_to_col_number(int x) {
        int ret = (x / this.cellwidth) - this.x_offset ;
        ret = ret > this.model.getcolumncount() ? this.model.getcolumncount() : ret;
        return ret > 0 ? ret : 0;
    }
    /**
     * Convert a column number to a coordinate.
     * @param col
     * @return
     */
    private int col_number_to_coordinate(int col) {
        return (col + this.x_offset) * this.cellwidth;
    }
    /**
     * Get the datum at the specified graphics coordinates.
     * @param x
     * @param y
     * @return
     */
    private MatrixDatum getDatum(int x, int y) {
        return this.model.getDatum(coordinate_to_col_number(y), coordinate_to_row_number(x));
    }
    /**
     * True if a selection is underway.
     * @return
     */
    public boolean isSelectionActive() {
        return this.selectionActive;
    }
    /**
     * Reset the selection to the whole matrix.
     */
    public void resetSelection() {
        this.selected_cols = new Range(0, this.model.getcolumncount());
        this.selected_rows = new Range(0, this.model.getrowcount());
    }
    /**
     * get the visible left coordinate.
     * @return
     */
    public int getVisible_left() {
        return visible_left;
    }
    /**
     * set the visible left coordinate
     * @param visible_left
     */
    public void setVisible_left(int visible_left) {
        this.visible_left = visible_left;
    }
    /**
     * Get the visible top coordinate.
     * @return
     */
    public int getVisible_top() {
        return visible_top;
    }

    public int getVisible_height() {
        return visible_height;
    }

    public void setVisible_height(int visible_height) {
        this.visible_height = visible_height;
    }

    public int getVisible_width() {
        return visible_width;
    }

    public void setVisible_width(int visible_width) {
        this.visible_width = visible_width;
    }


    /**
     * Set the visible top coordinate.
     * @param visible_top
     */
    public void setVisible_top(int visible_top) {
        this.visible_top = visible_top;
    }

    

    /**
     * Get the selected rows
     */
    public Range getSelectedRows() {
        return this.selected_rows;
    }

    /**
     * Get the selected columns
     * @return
     */
    public Range getSelectedCols() {
        return this.selected_cols;
    }
    /**
     * Handle deferred painting of the matrix.
     * @param g
     */
    protected void async_paint( Graphics g ){
        if (this.model != null
                && this.model.getcolumncount() > 0
                && this.model.getrowcount() > 0) {
            //iterate over visible rows.
            for (int row = 0 /*this.coordinate_to_row_number( this.visible_top )*/; row < model.getrowcount() /*&& row < this.coordinate_to_row_number( this.visible_height )*/ ; ++row) {
                //Draw the row label
                g.setColor(Color.black);
                // System.err.println( "Row[" + row + "]: " +this.model.getRowLabel(row) );

                //g.drawString( this.model.getRowLabel(row) , 0, /*col_label_offset+*/(row+1)*cellheight + cellheight /2);
                //Iterate over visible columns
                Iterator<MatrixDatum> crowit = this.model.getRow(row).iterator();
                for (int col = this.coordinate_to_col_number( this.visible_left ); col < model.getColumnCount() && crowit.hasNext() && col < this.coordinate_to_col_number( this.visible_width ); ++col){
                //while (crowit.hasNext()) {
                    MatrixDatum cdat = crowit.next();
                    // for (int col = 0; col < model.getcolumncount()-1; ++col ){
                    g.setColor(this.renderer.getColor(cdat));
                    //System.err.println( "row: " + cdat.getrow() + " col: " + cdat.ge tcolumn()+ " value: " + cdat.getvalue() );
                    //if ( this.selected_rows.contains( cdat.getrow() ) && this.selected_cols.contains( cdat.getcolumn()) ){
                    //    g.setColor(  g.getColor().darker() );
                    //}
                    //Draw the cell.
                    if (this.highlight_cols.contains(cdat.getcolumn()) && this.highlight_rows.contains(cdat.getrow()) && this.cellheight > MatrixView.MIN_3D_BLOCK_SIZE && this.cellwidth > MatrixView.MIN_3D_BLOCK_SIZE ) {
                        g.fill3DRect(/*row_label_offset + */col  /*+1*/ *  cellwidth,
                                /*this.key_entry_block_size +*/ row   * cellheight,
                                cellwidth,
                                cellheight, true);
                    } else {
                        g.fillRect( col   * cellwidth,
                                /*this.key_entry_block_size +*/ row   * cellheight,
                                cellwidth,
                                cellheight);
                    }
                    //don't try to show the text if the cells are too small.
                    if ( this.cellheight > MatrixView.MIN_TEXT_SIZE && this.cellwidth > MatrixView.MIN_TEXT_SIZE ){
                        g.setColor(new Color(Color.white.getRGB() ^ g.getColor().getRGB()));
                        g.drawString(cdat.getvalue(), col * this.cellwidth, (row + 1) * this.cellheight );
                    }
                    //g.setColor(Color.black);
                    //g.drawString( this.model.getDatum( row, col).getvalue(), row_label_offset +(row)*cellwidth + (row*cellwidth)/4, col_label_offset+ col*cellheight  + (col*cellheight)/4 );
                }
            }
        } else {
            String noneLoaded = "No Matrix Loaded";
            g.drawString(noneLoaded, this.getWidth() / 2 - noneLoaded.length() / 2, this.getHeight() / 2);
        }
    }

    /**
     * Draw the datum.
     * @param g
     */
    @Override
    public void paint(Graphics g) {
        if ( this.painter == null ){
            this.painter = new AsyncPainter( this );
            this.painter_context = new Thread( painter );
            this.painter_context.start();
        }
        //g.setColor(Color.WHITE);
        //g.fillRect(0, 0, this.getWidth(), this.getHeight());
        //g.setColor(Color.BLACK);
        this.painter.requestPaint(g);
        //this.async_paint(g);
    }



    /**
     * Handles mouse based selection.
     * @param e
     */
    public void mouseDragged(MouseEvent e) {
        //System.err.println( "MatrixView::mouseDragged" );
        if (this.model != null) {
            if (!selectionActive) {
                selectionActive = true;
                this.startSelection(e.getX(), e.getY());

            }

            if (e.getX() > this.selected_cols.getMinValue()) {
                this.selected_cols.setMaxValue(this.coordinate_to_col_number(e.getX()));
            } else {
                int newMax = this.selected_cols.getMinValue();
                this.selected_cols.setMinValue(this.coordinate_to_col_number(e.getX()));
                this.selected_cols.setMaxValue(newMax);
            }
            if (e.getY() > this.selected_rows.getMinValue()) {
                this.selected_rows.setMaxValue(this.coordinate_to_row_number(e.getY()));
            } else {
                int newMax = this.selected_rows.getMinValue();
                this.selected_rows.setMinValue(this.coordinate_to_row_number(e.getY()));
                this.selected_rows.setMaxValue(newMax);
            }

        }
    }
    /**
     * Handle tool tip display.
     * @param evt
     */
    public void mouseMoved(MouseEvent evt) {
        //if (!evt.isConsumed() ){
        if (this.model != null) {
            if (this.coordinate_to_row_number(evt.getY()) < this.model.getrowcount() && this.coordinate_to_col_number(evt.getX()) < this.model.getcolumncount()) {
                String tipText = this.model.getRowLabel(this.coordinate_to_row_number(evt.getY())) + ", trait" + this.coordinate_to_col_number(evt.getX());
                this.setToolTipText(tipText);
            }

            //System.err.println( tipText );
        }
        //}
        return;
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                formMouseClicked(evt);
            }
        });

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 400, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 300, Short.MAX_VALUE)
        );
    }// </editor-fold>//GEN-END:initComponents

    private void formMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_formMouseClicked
        this.handleMouseClicked(evt);
    }//GEN-LAST:event_formMouseClicked


    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables

}
