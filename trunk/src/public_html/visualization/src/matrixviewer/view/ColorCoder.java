/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.view;

import java.awt.Color;
import matrixviewer.model.MatrixDatum;

/**
 *
 * @author bchisham
 */
public interface ColorCoder {
    Color getColor( MatrixDatum datum );
}
