<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
 xmlns:sparql="http://www.w3.org/2005/sparql-results#"
 xmlns:foaf="http://xmlns.com/foaf/0.1/"
 xmlns:dc="http://purl.org/dc/elements/1.1/"
 xmlns="http://xmlns.com/foaf/0.1/">
    <xsl:output method="xml" omit-xml-declaration="yes"/>

    <xsl:template name="procabstract">
       <xsl:param name="abs"/>
       <p><xsl:value-of select="$abs"/></p>
    </xsl:template>


    <xsl:template match="/sparql:sparql/sparql:results">
             <xsl:for-each select="sparql:result">
               <xsl:for-each select="sparql:binding">
                  <xsl:choose>
                     <xsl:when test="@name = 'citation'">
                        <dc:description><xsl:value-of select="sparql:literal"/></dc:description>
                     </xsl:when>
                     <xsl:when test="@name = 'abstract'">
                        <dc:abstract><xsl:value-of select="sparql:literal"/></dc:abstract>
                     </xsl:when>
                     <xsl:otherwise></xsl:otherwise>
                 </xsl:choose>
               </xsl:for-each>
           </xsl:for-each>
    </xsl:template>
    <xsl:template select="*" priority="-1"/>
</xsl:stylesheet>
