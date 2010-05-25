/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package matrixviewer;

import org.jdesktop.application.Application;
import org.jdesktop.application.SingleFrameApplication;

/**
 *
 * @author bchisham
 */
public class MatrixViewerApp extends SingleFrameApplication {
    private String[] args;

    @Override protected void initialize(String[] args){
        System.err.println("Initialized with # args =" + args.length);
        this.args = args;
    }

    /**
     * At startup create and show the main frame of the application.
     */
    @Override protected void startup() {
        if (this.args == null || this.args.length == 0){
            show(new MatrixViewerView(this));
        }
        else if (this.args.length > 0){
            show(new MatrixViewerView(this, args[0]) );
        }
    }
    
    /**
     * This method is to initialize the specified window by injecting resources.
     * Windows shown in our application come fully initialized from the GUI
     * builder, so this additional configuration is not needed.
     */
    @Override protected void configureWindow(java.awt.Window root) {
    }

    /**
     * A convenient static getter for the application instance.
     * @return the instance of TestApp
     */
    public static MatrixViewerApp getApplication() {
        return Application.getInstance(MatrixViewerApp.class);
    }

    /**
     * Main method launching the application.
     */
    public static void main(String[] args) {
        launch(MatrixViewerApp.class, args);
    }

    

}
