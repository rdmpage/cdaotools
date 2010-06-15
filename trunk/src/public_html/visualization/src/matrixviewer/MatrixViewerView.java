/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer;

import matrixviewer.model.Matrix;
import org.jdesktop.application.FrameView;
import org.jdesktop.application.ResourceMap;
import org.jdesktop.application.SingleFrameApplication;

/**
 *
 * @author bchisham
 */
public class MatrixViewerView extends FrameView {
    private MatrixViewer viewer;
    ResourceMap map;
    String[] args;


    public MatrixViewerView(SingleFrameApplication app){
        super(app);
        this.viewer = new MatrixViewer();
        this.setFrame(viewer);
        //map = getResourceMap();
        
    }

    public MatrixViewerView(SingleFrameApplication app, String src){
        super(app);
         this.viewer = new MatrixViewer(src);
         System.err.println( "MatrixViewerView opening: " + src );
        this.setFrame(viewer);
       
    }

    public MatrixViewerView(SingleFrameApplication app, Matrix mat){
        super(app);
        this.viewer = new MatrixViewer(mat);
        this.setFrame(viewer);

        
    }
}
