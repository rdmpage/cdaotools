<?php require_once 'header.php' ?>
<div id="content">

<h1>Frequently Asked Questions</h1>
<ul>
  <li><a href="#what_is_cdao">What is CDAO</a></li>
  <li><a href="#what_is_phylows">What is PhlyoWS?</a></li>
  <li>Can I inject SPARQL queries?</li>
  <li>How do I import a new data file?</li>
  <li>How do I navigate the graphical browser?</li>
  <li>Where do the tree names come from?</li>
  <li>How do I get raw RDF/XML files of the tree/matrix?</li>
  <li>Can I export to a non-CDAO filetype?</li>
  <li>It appears the browser is not displaying a tree correctly</li>
</ul>

<br/>
<hr/>
<br/>

<h3 id="what_is_cdao">What is CDAO</h3>
<p>
  <a href="http://www.evolutionaryontology.org">CDAO</a> is a standard for describing comparative data
  in a semantically rigorous way.
</p>

<h3 id="what_is_phylows">What is PhyloWS</h3>
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
<hr/>
<p>
Other questions? Email:<a href="mailto:cdaotools@gmail.com">us</a>
</p>
</div><div id="footer"><hr/>   
WARNING: This is still in alpha-testing.  There are known bugs that will crash the application.  If you are using this and a tree crashes or does not display properly, please email <a href="mailto:cdaotools@gmail.com">us</a>
    
     <p about=""
     resource="http://www.w3.org/TR/rdfa-syntax"
          rel="dct:conformsTo" xmlns:dc="http://purl.org/dc/terms/">
	      <a href="http://validator.w3.org/check?uri=referer" ><img
	              src="http://www.w3.org/Icons/valid-xhtml-rdfa-blue"
		              alt="Valid XHTML + RDFa" style="border:0px;"   /></a>
			        </p>

  </div>
  </div>
 </body>
</html>
