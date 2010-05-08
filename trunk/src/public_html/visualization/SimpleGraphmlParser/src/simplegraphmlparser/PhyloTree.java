/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package simplegraphmlparser;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

/**
 *
 * @author bchisham
 */
public class PhyloTree extends DefaultHandler{
    HashMap<String, ArrayList<String>> edges;
    HashMap<String, String> redges;
    Set< String > processedNodes;
    //int level;
    public PhyloTree(){
        this.edges = new HashMap();
        this.redges = new HashMap();
        this.processedNodes = new TreeSet();
        //this.level = 0;
    }

    public void parse(InputStream is){
        SAXParserFactory spf = SAXParserFactory.newInstance();
        try {
            SAXParser sp = spf.newSAXParser();
            sp.parse(is, this);
        } catch (IOException ex) {
            Logger.getLogger(PhyloTree.class.getName()).log(Level.SEVERE, null, ex);
        } catch (ParserConfigurationException ex) {
            Logger.getLogger(PhyloTree.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SAXException ex) {
            Logger.getLogger(PhyloTree.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void parse( File file) throws IOException{
        this.parse( file.toURL() );
    }
    public void parse( URL url ) throws IOException{
        this.parse( url.openStream() );
    }

    private void processSubtree(PrintStream ps, String current, int level){
        for ( int i = 0; i < level; ++i ){ ps.print("  "); }
        ps.println( current );
        ArrayList<String> nextNodes = null;
        if ( (nextNodes = this.edges.get( current )) != null){
            Iterator< String > childit = nextNodes.iterator();
            while (childit.hasNext()){
                processSubtree(ps, childit.next(), level + 1);
            }   
        }


    }

    public void printTree( PrintStream ps ){
        //PrintWriter ps = new PrintWriter(os);

        Set<String> snodes = this.edges.keySet();
        
        Iterator<String> current = snodes.iterator();

        //System.err.println("Printing, " + snodes.size() + ", edges");
        String root="";
        while ( current.hasNext() ){
            String cnode = current.next();
            if ( this.redges.get(cnode) == null ){
                root = cnode;
                break;
            }
        }
        processSubtree( ps, root, 0 );


    }

    @Override
    public void startElement(String uri, String localName, String qName,
		Attributes attributes) throws SAXException {
        //System.err.println( "Processing: uri: " + uri + " localName: " + localName + " qName: " + qName );
        if ( qName.equals("edge") ){
            String src = attributes.getValue("source") ;
            String dest = attributes.getValue( "target" );
            //System.err.println("source=" + src + " target=" + dest);
            ArrayList<String> children = edges.get( src );
            if ( children == null){ children = new ArrayList(); }
            children.add(dest);
            edges.put(src, children);
            redges.put(dest, src);
        } else if ( qName.equals("node") ){
            //this.nodes.add( attributes.getValue( "id" ) );
        }

    }

    @Override
    public void characters(char[] ch, int start, int length) throws SAXException {
		//tempVal = new String(ch,start,length);
    }

    @Override
    public void endElement(String uri, String localName,
		String qName) throws SAXException {

    }


}
