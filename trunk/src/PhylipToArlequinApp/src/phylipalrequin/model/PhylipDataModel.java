/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package phylipalrequin.model;

import java.net.URI;

/**
 *
 * @author bchisham
 */
public class PhylipDataModel implements MatrixDataModel {
    private URI name;
    private URI dataModelType;
    private URI dataType;
    
    private int nTax;
    private int nChar;

    private String[] taxLabels;
    private char[][] characters;


    /**
     * Create a new Phylip model
     * @param name
     * @param dataModelType
     * @param dataType
     */
    public PhylipDataModel(URI name, URI dataModelType, URI dataType) {
        this.name = name;
        this.dataModelType = dataModelType;
        this.dataType = dataType;
    }

    /**
     * 
     * @param name
     * @param dataModelType
     * @param dataType
     * @param nTax
     * @param nChar
     * @param characters
     */
    public PhylipDataModel(URI name, URI dataModelType, URI dataType, int nTax, int nChar, char[][] characters) {
        this.name = name;
        this.dataModelType = dataModelType;
        this.dataType = dataType;
        this.nTax = nTax;
        this.nChar = nChar;
        this.characters = characters;
    }




    public URI getName() {
        return name;
    }

    public URI getDataModelType() {
        return dataModelType;
    }

    public URI getDataType() {
        return dataType;
    }

    public int getNTax() {
        return nTax;
    }

    public int getNChar() {
        return nChar;
    }

    public char getCharacterState(int taxonNumber, int charNumber) {
        return this.characters[ taxonNumber][charNumber];
    }

    public String getTaxonName(int taxonNumber) {
        return this.taxLabels[ taxonNumber ];
    }
    
}
