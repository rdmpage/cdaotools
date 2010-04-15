package matrix.view;

import javax.swing.*;
import java.awt.*;
import javax.swing.table.*;


public class JvTable extends JTable
{
    private Font originalFont;
    private int originalRowHeight;
    private float zoomFactor = 1.0f;


    public JvTable(TableModel dataModel)
    {
        super(dataModel);
    }


    @Override
    public void setFont(Font font)
    {
        originalFont = font;
        // When setFont() is first called, zoomFactor is 0.
        if (zoomFactor != 0.0 && zoomFactor != 1.0)
        {
            float scaledSize = originalFont.getSize2D() * zoomFactor;
            font = originalFont.deriveFont(scaledSize);
        }

        super.setFont(font);
    }


    @Override
    public void setRowHeight(int rowHeight)
    {
        originalRowHeight = rowHeight;
        // When setRowHeight() is first called, zoomFactor is 0.
        if (zoomFactor != 0.0 && zoomFactor != 1.0)
            rowHeight = (int) Math.ceil(originalRowHeight * zoomFactor);

        super.setRowHeight(rowHeight);

    }

    public float getZoom()
    {
        return zoomFactor;
    }


    public void setZoom(float zoomFactor)
    {
        if (this.zoomFactor == zoomFactor)
            return;

        if (originalFont == null)
            originalFont = getFont();
        if (originalRowHeight == 0)
            originalRowHeight = getRowHeight();

        float oldZoomFactor = this.zoomFactor;
        this.zoomFactor = zoomFactor;
        Font font = originalFont;
        if (zoomFactor != 1.0)
        {
            float scaledSize = originalFont.getSize2D() * zoomFactor;
            font = originalFont.deriveFont(scaledSize);
        }

        super.setFont(font);
        super.setRowHeight((int) Math.ceil(originalRowHeight * zoomFactor));
        getTableHeader().setFont(font);
        calcColumnWidths();
        firePropertyChange("zoom", oldZoomFactor, zoomFactor);
    }


    @Override
    public Component prepareRenderer(TableCellRenderer renderer, int row, int column)
    {
        Component comp = super.prepareRenderer(renderer, row, column);
        comp.setFont(this.getFont());
        return comp;
    }


    @Override
    public Component prepareEditor(TableCellEditor editor, int row, int column)
    {
        Component comp = super.prepareEditor(editor, row, column);
        comp.setFont(this.getFont());
        return comp;
    }
    /*
     * Adjust the column width to the biggest cell/header
     */
	public void calcColumnWidths()
	{
		JTableHeader header = this.getTableHeader();
		TableCellRenderer defaultHeaderRenderer = null;
		if (header != null)
		defaultHeaderRenderer = header.getDefaultRenderer();
	
		TableColumnModel columns = this.getColumnModel();
		TableModel data = this.getModel();
		int margin = columns.getColumnMargin(); 
		int rowCount = data.getRowCount();
		int totalWidth = 0;
	
		for (int i = columns.getColumnCount() - 1; i >= 0; --i)
		{
			TableColumn column = columns.getColumn(i);
			int columnIndex = column.getModelIndex();
			int width = -1;
	
			TableCellRenderer h = column.getHeaderRenderer();
	
			if (h == null)
				h = defaultHeaderRenderer;
	
			if (h != null)
			{
				Component c = h.getTableCellRendererComponent(this, column.getHeaderValue(), false, false, -1, i);
				width = c.getPreferredSize().width;
			}
	
			for (int row = rowCount - 1; row >= 0; --row)
			{
				TableCellRenderer r = this.getCellRenderer(row, i);
				Component c = r.getTableCellRendererComponent(this, data.getValueAt(row, columnIndex), false, false,
				row, i);
				width = Math.max(width, c.getPreferredSize().width);
			}
		
			if (width >= 0)
				column.setPreferredWidth(width + margin); 
			else
				totalWidth += column.getPreferredWidth();
		}
	}
}



