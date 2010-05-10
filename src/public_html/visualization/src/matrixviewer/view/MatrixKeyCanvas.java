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
 *
 * @author bchisham
 */
public class MatrixKeyCanvas extends Canvas {
    private Matrix model;
    private DrawableDatum rederer;
    private static final int ROW_LABEL_WIDTH = 50;
    private static final int key_entry_height = 10;
    public MatrixKeyCanvas(){
        model = null;
        this.rederer = new DrawableDatum();
    }

    @Override
    public void paint( Graphics g ){
        if (model != null){
             // int col_label_offset = cellheight + key_entry_block_size ;
            Iterator< String > mval = this.model.getUniqueValues().iterator();
            //Draw the key.
////
            for (int key = 0; key < this.model.getUniqueValues().size() && mval.hasNext(); ++key){
                g.setColor(Color.black);
                String current = mval.next();
                g.drawString( current , 0  , key_entry_height * (key+1) + key_entry_height);
                g.setColor( this.rederer.getColor( new MolecularDatum(0,0, current) ) );
                g.fillRect( ROW_LABEL_WIDTH , key_entry_height *( key +1 ), key_entry_height, key_entry_height);
            }
        }
    }

    public void setModel( Matrix model ){
        this.model = model;
        this.repaint();
    }
}
