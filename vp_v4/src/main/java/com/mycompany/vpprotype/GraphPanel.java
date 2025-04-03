/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.vpprotype;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.util.ArrayList;
import javax.swing.JPanel;

/**
 *
 * @author Green I
 */
public class GraphPanel extends JPanel{

    private int scaleFactor;
    private ArrayList<Line> lines;
    
    public GraphPanel(){
        
        this.scaleFactor = 1;
        this.lines = new ArrayList<>();
        setPreferredSize(new Dimension(10650, 400));
        
    }
    
    public void setScaleFactor(int scaleFactor) {        
        this.scaleFactor = scaleFactor;
        repaint();
      
    }
   
    public void setLines(ArrayList<Line> lines) {
        
        this.lines = lines;
        repaint();     
        
    }
    
    @Override
    public void paintComponent(Graphics g) {
        
        super.paintComponent(g);
        
        // Clears the display area of the entire panel
        g.setColor(Color.WHITE);
        g.fillRect(0, 0, getWidth(), getHeight());
        
        for (Line line : lines) {
            graphLine(line, g);
        }
        
    }
    
    private void graphLine(Line line, Graphics g) {
        /* Method will take and array dataset and graph a line based on the
            array values.  The line will be the color specified in the 
            color parameter.  All values are absolute.
        
            Method finds the min and max values for the line and uses them to
            scale the line to the display area.  All min values will be at the
            baseline, and the max value will be at the topline.  Values that do
            not start at 0 will be adjusted so that the start value is 0 - minValue.
        */
        
        int displayWidth = 600 + (scaleFactor*100);

        g.setColor(line.getColor());
        
        double[] data = line.getData();
        
        int horizontalMargin = 20;
        int verticalMargin =  100;
        
        // Find the range by finding the min and max values of the dataset
        double min = data[0];
        double max = data[0];
        
        for (Double value : data) {
            if (value < min)    min = value;
            if (value > max)    max = value;
        }
        
        double range = max - min;
        
        // Calculates the vertical and horizontal units based on the display area.
        // These unites are multiplied by the data value for the y value, and spacing
        // for the x values.
        
        double verticalUnit = (getHeight()- 2*verticalMargin)/range;
        int horizontalUnit = ((displayWidth- 2*horizontalMargin))/(data.length-1); 
        
        int baseLine = getHeight() - verticalMargin;

        int x = horizontalMargin;
        
        int prevX = x;
        int prevY = baseLine;

        // Loops through the data array, and calculates the scaled y value based on
        // the range of data.
        for (int i=0; i<data.length; i++){
            
            int y = (int) (baseLine-(data[i]-min)*verticalUnit);
            
            g.fillOval(x, y, 5, 5);
            g.drawLine(prevX, prevY, x, y);
            
            prevX = x;
            prevY = y;
            
            x+=horizontalUnit;
        }

    }


}
