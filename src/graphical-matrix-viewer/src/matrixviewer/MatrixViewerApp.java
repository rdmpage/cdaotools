/*
 * MatrixViewerApp.java
 */

package matrixviewer;

import java.net.URL;
import org.jdesktop.application.Application;
import org.jdesktop.application.SingleFrameApplication;

/**
 * The main class of the application.
 */
public class MatrixViewerApp extends SingleFrameApplication {
    private static MatrixViewerView view;
    /**
     * At startup create and show the main frame of the application.
     */
    @Override protected void startup() {
        show(view = new MatrixViewerView(this));
    }

    /**
     * This method is to initialize the specified window by injecting resources.
     * Windows shown in our application come fully initialized from the GUI
     * builder, so this additional configuration is not needed.
     */
    @Override protected void configureWindow(java.awt.Window root) {
    }

    public void showURL(URL url){
        (new MatrixViewerView(this)).openFile(url);
    }

    /**
     * A convenient static getter for the application instance.
     * @return the instance of MatrixViewerApp
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
