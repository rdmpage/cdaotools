<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="2.0"
    xmlns:nex="http://www.nexml.org/1.0" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
    xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
    xmlns:cdao="http://www.evolutionaryontology.org/cdao.owl#"
    xmlns:owl="http://www.w3.org/2002/07/owl#" xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:phyloxml="http://www.phyloxml/org/1.10/phyloxml.xsd"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    <xsl:output method="xml"/>
    <!-- Make new root element -->
    <xsl:template match="/nex:nexml">
         <phyloxml:phyloxml
	 	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	 	xmlns="http://www.phyloxml.org"
		xsi:schemaLocation="http://www.phyloxml.org http://www.phyloxml.org/1.10/phyloxml.xsd">
		<!-- Process the rest of the document -->
		<xsl:apply-templates select="descendant::*"/>
 	 </phyloxml:phyloxml>
	 <xsl:text>&#10;</xsl:text>
    </xsl:template>

    <!-- Create Phylogenies from trees -->
    <xsl:template match="nex:tree">
         <phyloxml:name><xsl:value-of select="@id"/></phyloxml:name>
	 <xsl:if test="@label">
		<phyloxml:description><xsl:value-of select="@label"/></phyloxml:description>
	 </xsl:if>
	 <xsl:for-each select="nex:edge[ @root = 'true']">
	 	<xsl:call-template name="makeclades">
			<xsl:with-param name="cnodeid" select="@source"/>
	 	</xsl:call-template>
	</xsl:for-each>

    </xsl:template>

    <!-- Create Phylogenies from Networks -->
    <xsl:template match="nex:network">
	 <phyloxml:name><xsl:value-of select="@id"/></phyloxml:name>
	 <xsl:if test="@label">
		<phyloxml:description><xsl:value-of select="@label"/></phyloxml:description>
	 </xsl:if>
	 <xsl:for-each select="nex:edge">
	 	<xsl:call-template name="makeclades">
			<xsl:with-param name="cnodeid" select="@source"/>
	 	</xsl:call-template>
	</xsl:for-each>

    </xsl:template>
    
    <xsl:template name="makeclades">
	<xsl:param name="cnodeid"/>
	<xsl:variable name="treeid" select="../@id"/>
	<xsl:choose>
		<xsl:when test="../nex:node[ @id = $cnodeid ]">
			<xsl:variable name="edge" select="../nex:edge[ @source = $cnodeid]"/>
			<xsl:for-each select="$edge">
				<phyloxml:clade>
					<phyloxml:name><xsl:value-of select="$cnodeid"/></phyloxml:name>
					<xsl:if test="$edge/@length">
						<phyloxml:branch_length><xsl:value-of select="$edge/length"/></phyloxml:branch_length>
						<xsl:text>&#10;</xsl:text>
					</xsl:if>
		        	<xsl:call-template name="makeclades">
		       			<xsl:with-param name="cnodeid" select="$edge/@target"/>
		 		</xsl:call-template>
				</phyloxml:clade>
				<xsl:text>&#10;</xsl:text>
			</xsl:for-each>
		</xsl:when>
	</xsl:choose>
    </xsl:template>


    <!-- Make clades out of nodes -->
    <!-- 
    <xsl:template match="nex:node">
         <phyloxml:clade>
		<taxonomy>
 			<id><xsl:value-of select="@id"/></id>
			<xsl:if test="@label">
				<scientific_name><xsl:value-of select="@label"/></scientific_name>
			</xsl:if>
		</taxonomy>
	 </phyloxml:clade>
    </xsl:template>
    -->

    <!-- Make clades out of edges (these will be the internal nodes).-->
    <!--
    <xsl:template match="nex:edge">
	<phyloxml:clade>
		<phyloxml:name><xsl:value-of select="@id"/></phyloxml:name>
		<xsl:if test="@label">
			<phyloxml:description><xsl:value-of select="@label"/></phyloxml:description>
		</xsl:if>
		<xsl:if test="@length">
		     <phyloxml:branch_length><xsl:value-of select="@length"/></phyloxml:branch_length>
		</xsl:if>
	</phyloxml:clade>
    </xsl:template>-->
</xsl:stylesheet>
