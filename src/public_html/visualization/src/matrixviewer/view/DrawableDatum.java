/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.view;

import java.awt.Color;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import matrixviewer.model.MatrixDatum;

/**
 * Static class to map data values to colors.
 * @author bchisham
 */
class DrawableDatum implements ColorCoder {
    private Map< String, Color > color_map;
    /**
     * Initialize the color map.
     */
    DrawableDatum(){
        color_map = new HashMap();
        //if (color_map == null ){
        //    color_map = new HashMap< String, Color >();
            color_map.put( "A", Color.blue);
            color_map.put( "T", Color.red);
            color_map.put( "U", Color.red);
            color_map.put( "C", Color.cyan);
            color_map.put( "G", Color.green);
        //}
    }
    /**
     * Initialize the color map with the specified set of unique values.
     * @param values
     */
    DrawableDatum(Set< String > values){
        Iterator< String > it = values.iterator();
        int delta_c;
        if (values.size() > 0){
            delta_c = 0xFFFFFF/ values.size();
        }
        else {
            delta_c = 255;
        }
         

        //System.err.println( "Delta c: " + delta_c );
        color_map = new HashMap< String, Color >();
        for (int ccc = 0; it.hasNext(); ccc += delta_c){
           color_map.put( it.next() ,  new Color( ccc ) );
        }
    }
    /**
     * Get the color associated with the specified datum.
     * @param datum
     * @return
     */
    public Color getColor(MatrixDatum datum) {
         if ( color_map.containsKey( datum.getvalue() ) ){
           return color_map.get( datum.getvalue()  );
         }
         return Color.white;
    }
}
