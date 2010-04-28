/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer.model;

import java.util.HashMap;
import java.util.Map;

/**
 *
 * @author bchisham
 */
public class MatrixFactory {
    private static Map< String, Matrix > matrix_type_map = null;

    public static Matrix getMatrix(String type){
        if (matrix_type_map == null){
             matrix_type_map = new HashMap<String, Matrix>();
             matrix_type_map.put("csv", new CSVMatrix());
             matrix_type_map.put( "CSV", new CSVMatrix());
        }
        return matrix_type_map.get( type ).copy();
    }
}
