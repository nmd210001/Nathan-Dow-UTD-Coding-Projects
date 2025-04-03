
package com.mycompany.snakesweep;


import javax.swing.Icon;
import javax.swing.JLabel;
import javax.swing.ImageIcon;


public class Square extends JLabel {
    
    private int numBombs;
    private int gridX;          // the x location of the square in the grid
    private int gridY;          // the y location of the square in the grid
    private boolean flag;
    private boolean covered;
    
    
    // Number of bombs will determine which icons to show when clicked
    // 0     Display a blank square
    // 1 - 8 Displays the number of adjacent bombs
    // 9     Displays a bomb
    
    // Data class will change the Icon Instance Variable based on numbers
    // 0 - blank image
    // 1 - "1" in Blue
    // 2 - "2" in Green
    // 3 - "3" in Red
    // 4 - "4" in Purple
    // 5 - "5" in Dark Red
    // 6 - "6" in Teal
    // 7 - "7" in Black
    // 8 - "8" in Gray
    // 9 - Snake Bomb
    // Right click - Flag on Button image
    
    // All image Icons are 32 x 32 pixels

    public Square(int gridX, int gridY) {
        this.numBombs = 0;
        this.gridX = gridX;
        this.gridY = gridY;
        this.flag = false; //use this in flag
        this.covered = true;
        
        //Set the intial image icon to the Tile image
        this.setIcon(new ImageIcon("images\\tile.gif"));
    }

    public boolean isFlag() {
        return flag;
    }

    public void setFlag(boolean flag) {
        this.flag = flag;
    }

    public boolean isCovered() {
        return covered;
    }

    public void setCovered(boolean covered) {
        this.covered = covered;
    }
    
    public int getGridX() {
        return gridX;
    }

    public int getGridY() {
        return gridY;
    }

    public int getNumBombs() {
        return numBombs;
    }

    public void setNumBombs(int numBombs) {
        this.numBombs = numBombs;
    }
    
    
}
