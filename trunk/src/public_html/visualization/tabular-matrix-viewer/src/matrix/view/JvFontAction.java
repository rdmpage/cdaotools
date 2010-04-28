package matrix.view;

import javax.swing.*;
import java.awt.event.*;
import javax.swing.event.*;
import javax.swing.undo.*;

import java.awt.*;

class JvFontAction extends AbstractAction
{
    protected JComponent component;
    protected UndoManager undoManager;
    protected String fontSpec;


    public JvFontAction(JComponent component, UndoManager undoManager, String fontSpec)
    {
        super("Font " + fontSpec);
        this.component = component;
        this.undoManager = undoManager;
        this.fontSpec = fontSpec.replace(" ", "-");
    }


    public void actionPerformed(ActionEvent event)
    {
        Font oldFont = component.getFont();
        Font font = Font.decode(fontSpec);

        JvFontEdit edit = new JvFontEdit(component, oldFont, font);
        edit.doit();
        UndoableEditEvent editEvent = new UndoableEditEvent(component, edit);
        undoManager.undoableEditHappened(editEvent);
    }
}


