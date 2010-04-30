/**
 * Copyright (c) 2004-2006 Regents of the University of California.
 * See "license-prefuse.txt" for licensing terms.
 */
package prefuse.data.io;

import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;

import prefuse.data.Edge;
import prefuse.data.Graph;
import prefuse.data.Node;
import prefuse.data.Schema;
import prefuse.util.io.XMLWriter;

/**
 * GraphWriter instance that writes a graph file formatted using the
 * GraphML file format. GraphML is an XML format supporting graph
 * structure and typed data schemas for both nodes and edges. For more
 * information about the format, please see the
 * <a href="http://graphml.graphdrawing.org/">GraphML home page</a>.
 * 
 * <p>The GraphML spec only supports the data types <code>int</code>,
 * <code>long</code>, <code>float</code>, <code>double</code>,
 * <code>boolean</code>, and <code>string</code>. An exception will
 * be thrown if a data type outside these allowed types is
 * encountered.</p>
 * 
 * @author <a href="http://jheer.org">jeffrey heer</a>
 */
public class NexmlWriter extends AbstractGraphWriter {

    /**
     * String tokens used in the GraphML format.
     */
    public interface Tokens extends GraphMLReader.Tokens  {
        public static final String NEXML = "nexml";
        
        public static final String NEXML_HEADER =
           "<nex:nexml xmlns:nex=\"http://www.nexml.org/1.0\"\n" +
           "xmlns=\"http://www.nexml.org/1.0\"\n" +
           "xmlns:owl=\"http://www.w3.org/2002/07/owl#\"\n" +
           "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n" +
           "xmlns:skos=\"http://www.w3.org/2004/02/skos/core#\"\n" +
           "xmlns:tb=\"http://treebase.org/terms#\"\n" +
           "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"\n" +
           "xmlns:xsi=\"http://www.w3.org/1999/XMLSchema-instance\"\n" +
           "generator=\"org.nexml.model.impl.DocumentImpl\"\n" +
           "version=\"0.8\" " + "xml:base=\"http://purl.org/phylo/treebase/phylows/\">\n\n";
    }
    
    /**
     * Map containing legal data types and their names in the GraphML spec
     */
    private static final HashMap TYPES = new HashMap();
    static {
        TYPES.put(int.class, Tokens.INT);
        TYPES.put(long.class, Tokens.LONG);
        TYPES.put(float.class, Tokens.FLOAT);
        TYPES.put(double.class, Tokens.DOUBLE);
        TYPES.put(boolean.class, Tokens.BOOLEAN);
        TYPES.put(String.class, Tokens.STRING);
    }
    
    /**
     * @see prefuse.data.io.GraphWriter#writeGraph(prefuse.data.Graph, java.io.OutputStream)
     */
    public void writeGraph(Graph graph, OutputStream os) throws DataIOException
    {
        // first, check the schemas to ensure GraphML compatibility
        Schema ns = graph.getNodeTable().getSchema();
        Schema es = graph.getEdgeTable().getSchema();
        checkNeXMLSchema(ns);
        checkNeXMLSchema(es);
        
        XMLWriter xml = new XMLWriter(new PrintWriter(os));
        xml.begin(Tokens.NEXML_HEADER, 2);
        
        xml.comment("prefuse NeXML Writer | "
                + new Date(System.currentTimeMillis()));
        
        // print the graph schema
        printSchema(xml, Tokens.NODE, ns, null);
        printSchema(xml, Tokens.EDGE, es, new String[] {
            graph.getEdgeSourceField(), graph.getEdgeTargetField()
        });
        xml.println();
        
        // print graph contents
        xml.start(Tokens.GRAPH, Tokens.EDGEDEF,
            graph.isDirected() ? Tokens.DIRECTED : Tokens.UNDIRECTED);
        
        // print the nodes
        xml.comment("nodes");
        Iterator nodes = graph.nodes();
        while ( nodes.hasNext() ) {
            Node n = (Node)nodes.next();
            
            if ( ns.getColumnCount() > 0 ) {
                xml.start(Tokens.NODE, Tokens.ID, String.valueOf(n.getRow()));
                for ( int i=0; i<ns.getColumnCount(); ++i ) {
                    String field = ns.getColumnName(i);
                    xml.contentTag(Tokens.DATA, Tokens.KEY, field,
                                   n.getString(field));
                }
                xml.end();
            } else {
                xml.tag(Tokens.NODE, Tokens.ID, String.valueOf(n.getRow()));
            }
        }
        
        // add a blank line
        xml.println();
        
        // print the edges
        String[] attr = new String[]{Tokens.ID, Tokens.SOURCE, Tokens.TARGET};
        String[] vals = new String[3];
        
        xml.comment("edges");
        Iterator edges = graph.edges();
        while ( edges.hasNext() ) {
            Edge e = (Edge)edges.next();
            vals[0] = String.valueOf(e.getRow());
            vals[1] = String.valueOf(e.getSourceNode().getRow());
            vals[2] = String.valueOf(e.getTargetNode().getRow());
            
            if ( es.getColumnCount() > 2 ) {
                xml.start(Tokens.EDGE, attr, vals, 3);
                for ( int i=0; i<es.getColumnCount(); ++i ) {
                    String field = es.getColumnName(i);
                    if ( field.equals(graph.getEdgeSourceField()) ||
                         field.equals(graph.getEdgeTargetField()) )
                        continue;
                    
                    xml.contentTag(Tokens.DATA, Tokens.KEY, field, 
                                   e.getString(field));
                }
                xml.end();
            } else {
                xml.tag(Tokens.EDGE, attr, vals, 3);
            }
        }
        xml.end();
        
        // finish writing file
        xml.finish("</"+Tokens.NEXML+">\n");
    }
    
    /**
     * Print a table schema to a GraphML file
     * @param xml the XMLWriter to write to
     * @param group the data group (node or edge) for the schema
     * @param s the schema
     */
    private void printSchema(XMLWriter xml, String group, Schema s,
                             String[] ignore)
    {
        String[] attr = new String[] {Tokens.ID, Tokens.FOR,
                Tokens.ATTRNAME, Tokens.ATTRTYPE };
        String[] vals = new String[4];

OUTER:
        for ( int i=0; i<s.getColumnCount(); ++i ) {
            vals[0] = s.getColumnName(i);
            
            for ( int j=0; ignore!=null && j<ignore.length; ++j ) {
                if ( vals[0].equals(ignore[j]) )
                    continue OUTER;
            }
            
            vals[1] = group;
            vals[2] = vals[0];
            vals[3] = (String)TYPES.get(s.getColumnType(i));
            Object dflt = s.getDefault(i);
            
            if ( dflt == null ) {
                xml.tag(Tokens.KEY, attr, vals, 4);
            } else {
                xml.start(Tokens.KEY, attr, vals, 4);
                xml.contentTag(Tokens.DEFAULT, dflt.toString());
                xml.end();
            }
        }
    }
    
    /**
     * Checks if all Schema types are compatible with the NeXML specification.
     * The NeXML spec only allows the types <code>int</code>,
     * <code>long</code>, <code>float</code>, <code>double</code>,
     * <code>boolean</code>, and <code>string</code>.
     * @param s the Schema to check
     */
    private void checkNeXMLSchema(Schema s) throws DataIOException {
        for ( int i=0; i<s.getColumnCount(); ++i ) {
            Class type = s.getColumnType(i);
            if ( TYPES.get(type) == null ) {
                throw new DataIOException("Data type unsupported by the "
                    + "NeXML format: " + type.getName());
            }
        }
    }
    
} // end of class GraphMLWriter