<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
 xmlns:sparql="http://www.w3.org/2005/sparql-results#"
 xmlns:html="http://www.w3.org/1999/xhtml"
 xmlns="http://www.w3.org/1999/xhtml">
    <xsl:output method="xml" omit-xml-declaration="yes"/>

    
    <xsl:template match="/sparql:sparql/sparql:results">
        <div>
             <xsl:for-each select="sparql:result">
               <xsl:for-each select="sparql:binding">
                  <xsl:choose>
                    
                     <xsl:when test="@name = 'lname'">
                       <a>
                        <xsl:attribute name="href">http://www.cs.nmsu.edu/~bchisham/cgi-bin/author/html?last=<xsl:value-of select="sparql:literal"/>
                        </xsl:attribute>
                        <xsl:value-of select="sparql:literal"/>
                        </a><br/>
                     </xsl:when>
                     <!--
                     <xsl:when test="@name = 'history'">
                        <p><xsl:value-of select="sparql:uri"/></p>
                     </xsl:when>
                     <xsl:when test="@name = 'matrix'">
                        <p><xsl:value-of select="sparql:uri"/></p>
                     </xsl:when>
                     <xsl:when test="@name = 'tree'">
                        <p><xsl:value-of select="sparql:uri"/></p>
                     </xsl:when>
                     -->
                     <xsl:otherwise></xsl:otherwise>
                 </xsl:choose>
               </xsl:for-each>
           </xsl:for-each>
         </div>
    </xsl:template>
    <xsl:template select="*" priority="-1"/>
</xsl:stylesheet>
