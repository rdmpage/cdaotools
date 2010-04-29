/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.view;

import java.awt.Canvas;
import java.awt.Graphics;
import matrixviewer.model.Matrix;

/**
 *
 * @author bchisham
 */
public class RowLabelCanvas extends Canvas {
    private Matrix model;
    private int cellheight = 15;
    public RowLabelCanvas(){
        this.model = null;
    }

    public RowLabelCanvas(Matrix model){
        this.model = model;
    }

    public void setModel(Matrix model){
        this.model = model;
        this.repaint();
    }

    @Override
    public void paint(Graphics g){
        if (this.model != null){
            for (int i = 0; i < this.model.getrowcount(); ++i){
                g.drawString( this.model.getRowLabel(i) , 0,(i+1) * this.cellheight );
            }
        }
        else {
            g.drawString( "No Matrix", 0, this.getHeight()/2);
        }
    }
}
