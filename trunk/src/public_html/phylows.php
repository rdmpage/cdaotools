<?php require_once 'header.php' ?>
<div id="content">

		<h3> Import Datafiles to the TripleStore </h3>


		 <form action="cgi-bin/import" method="get">
                   <p><span>URL of RDF file:</span><input type="text" class="text" id="ontology" name="ontology"/>
                   <input type="submit" class="button" id="import-submit" value="Import" /></p>
		 </form>
		<h3>Translate a file into CDAO format</h3>
		<form action="cgi-bin/inputfiletranslator" enctype="multipart/form-data" method="post">
		<p>File format
		<select name="format">
			<option value="MEGA">MEGA</option>
			<option value="PHYLIP">PHYLIP</option>
			<option value="NEXUS">NEXUS</option>
		</select>
		File name
		<input type="file" name="inputfile">
		<input type="submit" name="choice" value="Translate" />
                  </p>
                </form>
                

		   <hr/><br/>
                 

		<h3> Find specific trees in the TripleStore: </h3>
		<h4>by means of</h4><br/>
		<ul>
			<li>
                 		<form action="cgi-bin/author/html" method="get">
                   		<div about="study:Study"><p rel="study:has_Author"><span>Author:</span><span property="study:has_last_name"><input type="text" class="text" id="last" name="last"/></span>
                   		<input type="submit" class="button" id="search" value="search" /></p></div>
				</form>
			</li>
			<li>
				Treebase Accesion Number
                              <form action="cgi-bin/tree/query" method="get">
                                  <p><!--NeXML: <input type="radio" name="format" value="nexml"/>-->
                                     <!--View: <input type="radio" name="format" value="graphml"/>-->
                                     <!--Prolog: <input type="radio" name="format" value="prolog"/>-->
                                    <input type="hidden" name="format" value="html" />
                                   <!--<form action="cdao-explorer/launch.php" method="get">-->
                                      <span>Tree:</span><input about="cdao:Tree" type="text" class="text" id="tree" name="tree" value="Tree3099"/>
                                      <input type="submit" value="View"/>  
                                   <!--</form>
 			           <input type="submit" value="Query"/>-->
					
                                  </p>
                              </form>
			</li>
			<li>
				By method of construction
                                <form action="cgi-bin/algorithm/html" method="get">
                                    <p about="study:Analysis">
                                    <span>Algorithm:</span>
                                    <span property="study:has_Algorithm">
                                    <select name="algorithm">
                                                <option>2 runs, 10 m generations</option>
                                                <option>6-parameter parsimony</option>
                                                <option>bayesian inference and relaxed molecular clock</option>
                                                <option>bayesian inference</option>
                                                <option>bayesian</option>
                                                <option>bayesian mcmc</option>
                                                <option>bio neighbor joining</option>
                                                <option>bionj</option>
                                                <option>bootstrap analysis</option>
                                                <option>bootstrap consensus</option>
                                                <option>bootstrap</option>
                                                <option>by hand</option>
                                                <option>character optimization</option>
                                                <option>cluster analysis</option>
                                                <option>confidence interval sieving</option>
                                                <option>consensus of  maximum likelihood, parsimony, and distance</option>
                                                <option>distance</option>
                                                <option>equal weight</option>
                                                <option>fast parsimony bootstrapping</option>
                                                <option>fitch margoliash</option>
                                                <option>fitch-margoliash</option>
                                                <option>generic</option>
                                                <option>genetic algorithm</option>
                                                <option>genetic distance</option>
                                                <option>goloboff fit parsimony (k=2)</option>
                                                <option>greedy assembly using repetition indices</option>
                                                <option>gtr g</option>
                                                <option>gtr i gamma</option>
                                                <option>gtr r</option>
                                                <option>heuristic</option>
                                                <option>heuristic search</option>
                                                <option>hky85-gamma</option>
                                                <option>implied weight</option>
                                                <option>implied weights</option>
                                                <option>inaase</option>
                                                <option>iterative strongest evidence analysis </option>
                                                <option>jackknife</option>
                                                <option>jacknife</option>
                                                <option>lake's invariants</option>
                                                <option>likelihood ratchet</option>
                                                <option>manual compatability</option>
                                                <option>manual</option>
                                                <option>maximum likelihood and neighbor joining</option>
                                                <option>maximum likelihood</option>
                                                <option>maximum parsimony</option>
                                                <option>minimize deep coalescences</option>
                                                <option>minimum evolution and maximum likelihood</option>
                                                <option>minimum evolution-k2p</option>
                                                <option>minimum evolution</option>
                                                <option>ml mp nj</option>
                                                <option>ml, nj, mp</option>
                                                <option>mp and ml</option>
                                                <option>mp and nj</option>
                                                <option>mp </option>
                                                <option>mp, nj, ml</option>
                                                <option>mprs</option>
                                                <option>n/a</option>
                                                <option>na</option>
                                                <option>neighbor joining</option>
                                                <option>neighbor joining, parsimony and maximum likelihood</option>
                                                <option>neighbour joining, parsimony and bayesian inference</option>
                                                <option>parsimony, bayesian</option>
                                                <option>parsimony dollo</option>
                                                <option>parsimony jackknifing</option>
                                                <option selected="selected">parsimony</option>
                                                <option>parsimony, max. likelihood, and neighbor joining</option>
                                                <option>parsimony ratchet</option>
                                                <option>parsimony wagner</option>
                                                <option>preference</option>
                                                <option>profdistance nj</option>
                                                <option>profile distance nj</option>
                                                <option>quartet puzzle</option>
                                                <option>ratchet</option>
                                                <option>rci reweighted</option>
                                                <option>sci reweighted</option>
                                                <option>shallowest divergences</option>
                                                <option>stratocladistics</option>
                                                <option>successive weighting</option>
                                                <option>supertree composite</option>
                                                <option>tamura-nei</option>
                                                <option>unspecified</option>
                                                <option>upgma</option>
                                                <option>veto supertree</option>
                                                <option>weighted parsimony</option>
                                           </select>
                                        </span>
                                     <input type="submit" class="button" id="algorithm-tree-submit" value="Search"/>
                                     </p>
                                  </form>
			</li>
			<li>
				By Software used
                                <form action="cgi-bin/software/html" method="get">
                                    <p about="study:Analysis">
                                      <span>Sofware:</span>
                                      <span property="study:has_Software">
                                      <select name="software">
                                        <option>bayesphylogenies</option>
                                        <option>beast 1.4.5</option>
                                        <option>beast</option>
                                        <option>best</option>
                                        <option>biosys-1</option>
                                        <option>by hand</option>
                                        <option>clados?</option>
                                        <option>clustal w</option>
                                        <option>clustalw</option>
                                        <option>clustal x</option>
                                        <option>clustalx</option>
                                        <option>dnaml</option>
                                        <option>estbranches and multidivtime</option>
                                        <option>fastdnaml</option>
                                        <option>formulae 9 &amp; 10</option>
                                        <option>garli</option>
                                        <option>genetree</option>
                                        <option>gtr g</option>
                                        <option>hennig86 and paup</option>
                                        <option>hennig86</option>
                                        <option>hennig86, paup</option>
                                        <option>hennig86, phylip</option>
                                        <option>inaase</option>
                                        <option>iqpnni</option>
                                        <option>jackknife</option>
                                        <option>jac</option>
                                        <option></option>
                                        <option>macclade</option>
                                        <option>manual</option>
                                        <option>mega 2.0</option>
                                        <option>mega2</option>
                                        <option>mega 3.1</option>
                                        <option>mega3.1</option>
                                        <option>mega4</option>
                                        <option>megalign</option>
                                        <option>mega</option>
                                        <option>mega, paup, phylip</option>
                                        <option>mesquite</option>
                                        <option>modified ga</option>
                                        <option>molphy-protml</option>
                                        <option>mr bayes</option>
                                        <option>mrbayes</option>
                                        <option>n/a</option>
                                        <option>na</option>
                                        <option>ni and li distance</option>
                                        <option>nona and paup</option>
                                        <option>nona</option>
                                        <option>none</option>
                                        <option>not specified</option>
                                        <option>nt-sys</option>
                                        <option>ntsys</option>
                                        <option>oy</option>
                                        <option>paml</option>
                                        <option>parsimony jacknifer</option>
                                        <option>paup* 4.04a for the macintosh</option>
                                        <option>paup and contree</option>
                                        <option>paup and hennig86</option>
                                        <option>paup and mega</option>
                                        <option>paup and mrbayes</option>
                                        <option>paup and njboot2</option>
                                        <option>paup and phylip</option>
                                        <option>paup + hennig86</option>
                                        <option>paup, hennig86</option>
                                        <option>paup, hennig86, nona, and pee-wee</option>
                                        <option>paup, hennig86, nona, pee-wee</option>
                                        <option>paup, hennig86, phylip</option>
                                        <option selected="selected">paup</option>
                                        <option>paup*</option>
                                        <option>paup, phylip</option>
                                        <option>pee-wee</option>
                                        <option>penny</option>
                                        <option>personal software</option>
                                        <option>phase</option>
                                        <option>phylip 3.57c dnaml</option>
                                        <option>phylip and wagner78</option>
                                        <option>phylip dnaml</option>
                                        <option>phylip</option>
                                        <option>phylip paup</option>
                                        <option>phylobayes</option>
                                        <option>phylomatic</option>
                                        <option>phyml</option>
                                        <option>physic</option>
                                        <option>physis</option>
                                        <option>physys &amp; phylip</option>
                                        <option>physys</option>
                                        <option>populations</option>
                                        <option>poy</option>
                                        <option>profdist</option>
                                        <option>profile distance</option>
                                        <option>puzzle</option>
                                        <option>python script</option>
                                        <option>raxml</option>
                                        <option>raxml v.7.0</option>
                                        <option>saitou &amp; nei</option>
                                        <option>sepal</option>
                                        <option>sepal (salisbury b.a.(1999)</option>
                                        <option>snap workbench</option>
                                        <option>systat</option>
                                        <option>tnt</option>
                                        <option>treecon</option>
                                        <option>treefinder</option>
                                        <option>tree-puzzle and phylip</option>
                                        <option>tree-puzzle</option>
                                        <option>unknown</option>
                                        <option>wagner 78</option>
                                        <option>wagner78</option>
                                        <option>winclada and nona</option>
                                        <option>winclada</option>
                                        <option>wisconsin package</option>
                                        <option>xac</option>
                                     </select>
                                   </span>
                                   <input type="submit" class="button" id="software-tree-submit" value="Search"/>
                                   </p>
                               </form>
			</li>
			<li>
				Trees containing a specific Taxa
                       		<form action="cgi-bin/tu/html" method="get">
                          		<p about="cdao:TU"><span>TU:</span><input type="text" class="text" id="html-tu" name="tu" value="Ilex_anomala" />
                          		<input type="submit" class="button" id="html-tu-submit" value="Search"/></p>
                       		</form>
			</li>
         </ul><br/><br/>
	<h2><a href="cdao-explorer/launch.jnlp">Go to CDAO Explorer Application</a></h2>
	
	</div>
<?php require_once 'footer.php'?>
