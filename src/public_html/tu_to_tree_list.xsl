<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl= "http://www.w3.org/1999/XSL/Transform"
		xmlns:foaf="http://xmlns.com/foaf/0.1/" 
    		xmlns:dc="http://purl.org/dc/elements/1.1/"
   		xmlns:owl2="http://www.w3.org/2006/12/owl2#"
    		xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
    		xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
    		xmlns:owl="http://www.w3.org/2002/07/owl#"
		xmlns:cdao="http://www.evolutionaryontology.org/cdao/1.0/cdao.owl"
                xmlns="">
   <xsl:output method="xml" encoding="utf-8" omit-xml-declaration="yes"/>
   
   <xsl:template match="/">
     <rdf:RDF
                xmlns:foaf="http://xmlns.com/foaf/0.1/" 
    		xmlns:dc="http://purl.org/dc/elements/1.1/"
   		xmlns:owl2="http://www.w3.org/2006/12/owl2#"
    		xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
    		xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
    		xmlns:owl="http://www.w3.org/2002/07/owl#"
		xmlns:cdao="http://www.evolutionaryontology.org/cdao/1.0/cdao.owl"
                xmlns="">
      <xsl:for-each select="./*/tu[ @id = $target ]/tree">
         <cdao:Tree>
            <xsl:attribute name="rdf:about">
              <xsl:value-of select="@name"/>
            </xsl:attribute>
         </cdao:Tree>
     </xsl:for-each>
     </rdf:RDF>
   </xsl:template>
</xsl:stylesheet>
