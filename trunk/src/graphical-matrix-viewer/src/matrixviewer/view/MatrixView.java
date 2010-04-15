/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.view;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.util.Iterator;
import matrixviewer.model.Matrix;
import matrixviewer.model.MolecularDatum;

/**
 * Visual representation of a character state data matrix.
 * @author bchisham
 */
public class MatrixView extends Canvas {
    private Matrix model;
    private DrawableDatum rederer = new DrawableDatum();
    int cellwidth;
    int cellheight;
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
        }
    }
    /**
     * (re)set the matrix being displayed
     * @param model
     */
    public void setMatrix( Matrix model ){
        this.model = model;
        this.rederer = new DrawableDatum( this.model.getUniqueValues() );
        this.repaint();
    }
    /**
     * Get the model associated with this matrix view
     * @return
     */
    public Matrix getMatrix(){
        return this.model;
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
            this.cellwidth = this.getWidth()/this.model.getcolumncount()/2;
            this.cellheight = this.getHeight()/this.model.getrowcount()/2;
            
            //put the column labels.
            //int datum_x_offset = -5;
            //int datum_y_offset = -5;
            int key_entry_height = 25;
            int key_entry_block_size = key_entry_height * this.model.getUniqueValues().size();
            int row_label_offset = cellwidth ;
            int col_label_offset = cellheight + key_entry_block_size ;
            Iterator< String > mval = this.model.getUniqueValues().iterator();
            //Draw the key.
            for (int key = 0; key < this.model.getUniqueValues().size() && mval.hasNext(); ++key){
                g.setColor(Color.black);
                String current = mval.next();
                g.drawString( current , 0  , key_entry_height * (key+1) + key_entry_height/3);
                g.setColor( this.rederer.getColor( new MolecularDatum(0,0, current) ) );
                g.fillRect( row_label_offset/2 , key_entry_height *( key +1 ), key_entry_height, key_entry_height);
            }


            g.setColor(Color.black);
            //Draw the column labels.
            for ( int col = 0; col < this.model.getcolumncount(); ++col ){
                g.drawString( this.model.getColumnLabel(col) , row_label_offset + col * cellwidth + cellwidth/2 , key_entry_block_size + col_label_offset/5);

            }
            //Draw the matrix
            g.setColor(Color.black);
            for (int row = 0; row < model.getrowcount(); ++row){
                //Draw the row label
                g.setColor(Color.black);
                g.drawString( this.model.getRowLabel(row) , 0, col_label_offset+(row)*cellheight + cellheight/2);
                for (int col = 0; col < model.getcolumncount(); ++col ){
                    g.setColor( this.rederer.getColor( this.model.getDatum(row, col) ) );
                    //Draw the cell.
                    g.fillRect(row_label_offset + (row)* cellwidth,
                            col_label_offset + (col)*cellheight,
                            cellwidth,
                            cellheight);
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
}
