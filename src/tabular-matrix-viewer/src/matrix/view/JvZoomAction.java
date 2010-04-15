package matrix.view;

import javax.swing.*;
import javax.swing.undo.*;
import java.awt.event.*;
import javax.swing.event.*;

class JvZoomAction extends AbstractAction
{
    protected JvTable table;
    protected UndoManager undoManager;
    protected float scaleFactor;


    public JvZoomAction(JvTable table, UndoManager undoManager, int percent)
    {
        super("Zoom " + percent + "%");
        this.table = table;
        this.undoManager = undoManager;
        scaleFactor = percent / 100.0f;
    }


    public void actionPerformed(ActionEvent event)
    {
        float oldScaleFactor = table.getZoom();
        if (oldScaleFactor == scaleFactor)
            return;

        JvTableZoomEdit edit = new JvTableZoomEdit(table, oldScaleFactor, scaleFactor);
        edit.doit();
        UndoableEditEvent editEvent = new UndoableEditEvent(table, edit);
        undoManager.undoableEditHappened(editEvent);
    }
}



