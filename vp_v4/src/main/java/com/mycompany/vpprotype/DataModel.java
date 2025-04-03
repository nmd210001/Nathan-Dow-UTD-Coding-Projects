/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.vpprotype;

import java.awt.Color;
import java.io.Serializable;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;


public class DataModel implements Serializable{

    private Double[][]  runData;
    private Object[]    columnHeaders;   
    private JCheckBox[] inputs;
    private JComboBox[] colorSelections;    
    private String currentDirectory;

    public DataModel(Double[][] runData, Object[] columnHeaders, JCheckBox[] inputs, JComboBox[] colorSelections, String currentDirectory) {
        this.runData = runData;
        this.columnHeaders = columnHeaders;
        this.inputs = inputs;
        this.colorSelections = colorSelections;
        this.currentDirectory = currentDirectory;
    }

    public Double[][] getRunData() {
        return runData;
    }

    public Object[] getColumnHeaders() {
        return columnHeaders;
    }

    public JCheckBox[] getInputs() {
        return inputs;
    }

    public JComboBox[] getColorSelections() {
        return colorSelections;
    }

    public String getCurrentDirectory() {
        return currentDirectory;
    }
    
}
