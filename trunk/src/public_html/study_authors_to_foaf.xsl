<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
 xmlns:sparql="http://www.w3.org/2005/sparql-results#"
 xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
 xmlns:foaf="http://xmlns.com/foaf/0.1/"
 xmlns="http://xmlns.com/foaf/0.1/">
    <xsl:output method="xml" omit-xml-declaration="yes"/>

    
    <xsl:template match="/sparql:sparql/sparql:results">
             <xsl:for-each select="sparql:result">
               <xsl:for-each select="sparql:binding">
                  <xsl:choose>
                    
                     <xsl:when test="@name = 'lname'">
                       <foaf:maker>
                        <xsl:attribute name="rdf:resource">http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/author/last/<xsl:value-of select="sparql:literal"/>
                        </xsl:attribute>
                       </foaf:maker>
                     </xsl:when>
                     <xsl:otherwise></xsl:otherwise>
                 </xsl:choose>
               </xsl:for-each>
           </xsl:for-each>
    </xsl:template>
    <xsl:template select="*" priority="-1"/>
</xsl:stylesheet>
