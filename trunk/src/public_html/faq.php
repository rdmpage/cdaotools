<?php require_once 'header.php' ?>
<div id="content">

<h1 id="top">Frequently Asked Questions</h1>
<ul>
  <li><a href="#what_is_cdao">What is CDAO?</a></li>
  <li><a href="#what_is_phylows">What is PhlyoWS?</a></li>
  <li><a href="#sparql">Can I inject SPARQL queries?</a></li>
  <li><a href="#import">How do I import a new data file?</a></li>
  <li><a href="#explore">How do I navigate CDAO explorer?</a></li>
  <li><a href="#names">Where do the tree/matrix names come from?</a></li>
  <li><a href="#output">How do I get raw RDF/XML files of the tree/matrix?</a></li>
  <li><a href="#export">Can I export to a non-CDAO filetype?</a></li>
  <li><a href="#error">It appears the browser is not displaying a tree correctly</a></li>
</ul>

<br/>
<hr/>
<br/>

<h3 id="what_is_cdao">What is CDAO?</h3>
<p>
  <a href="http://www.evolutionaryontology.org">CDAO</a> is a standard for describing comparative data
  in a semantically rigorous way.
</p>
<br/><a href="#top">Back to Top</a><br/>
<h3 id="what_is_phylows">What is PhyloWS?</h3>
<p>
  <a href="https://www.nescent.org/wg/evoinfo/index.php?title=PhyloWS">PhyloWS</a> is a standard for exposing phylogeny data using a REST-ful API.
</p>
<p>
  CDAO-store uses PhyloWS to allow clients to access information in the store about trees, taxonomic identifiers, minimum-spanning clade,
  and nearest common-ancestor of a set of nodes.
  For example to access the TreeBASE tree Tree3099:
  <code>
     curl http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/tree/Tree3099
  </code><br/>
  Matrices are similarly accessed:
  <code>
     curl http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/matrix/M4307
  </code><br/>
  The nearest common ancestor of a set of nodes can be found using:
  <code>
      curl http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/nca/TreeBASE_Tree_ID/TU1/TU2/.../TU_N
  </code><br/>
  The minimum spanning clade of a set of nodes can also be found using:
  <code>
      curl http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/msc/TreeBASE_Tree_ID/TU1/TU2/.../TU_N
  </code><br/>

  We also support searching for available tree and matrix id&quot;s as follows.
  <code>
    curl http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/listing/trees[?key=SUB_STRING]
  </code><br/>
  or 
  <code>
    curl http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/listing/matrices[?key=SUB_STRING]
  </code><br/>
  The key parameter is optional and allows one to filter the id&quot;s based on some those containing some string.
</p>
<br/><a href="#top">Back to Top</a><br/>
<h3 id="sparql">SPARQL queries</h3> 
  <p> 
  </p>
<br/><a href="#top">Back to Top</a><br/>
<h3 id="import">Importing files to CDAO store</h3> 
  <p>
      CDAO store currently supports importing RDF files into the triple store.  In addition, we can currently translate NEXUS, MEGA, and PHYLIP formats to our format. 
  </p>
<br/><a href="#top">Back to Top</a><br/>
<h3 id="explore">CDAO explorer</h3> 
  <p> 
  </p>
<br/><a href="#top">Back to Top</a><br/>
<h3 id="names">Tree/Matrix Naming Convention</h3> 
<p> 
  Names are currently based on the TreeBASE accession names they were given.
</p>
<br/><a href="#top">Back to Top</a><br/>
<h3 id="output">Getting RDF/XML file of my tree/matrix</h3> 
  <p> 
    See:<a href="#what_is_phylows">PhyloWS</a>
  </p>
<br/><a href="#top">Back to Top</a><br/>
<h3 id="export">Exporting to non-CDAO file format</h3> 
  <p> 
    CDAO store does not currently support exporting files out of RDF format.  This might change in a future implementation.
  </p>
<br/><a href="#top">Back to Top</a><br/>
<h3 id="error">There appears to be an error...</h3> 
  <p> 
    CDAO Explorer has its own error reporting system, however if something is wrong with the PhyloWS services, please <a href="mailto:cdaotools@gmail.com">Email Us</a>
  </p>
<br/><a href="#top">Back to Top</a><br/>
<hr/>
<p>
Other questions? Email:<a href="mailto:cdaotools@gmail.com">us</a>
</p>
</div>
<?php require_once 'footer.php'?>
