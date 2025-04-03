// Nathan Dow
// Period 1
// ShortestPathMaze.java




/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.shortestpathmaze;

/**
 *
 * @author Nathan Dow
 */
public class ShortestPathMaze {
    private int[][] maze;
    private boolean exitFound;
    private int shortest;

    public ShortestPathMaze(int[][] m)
    {
        shortest = 100;
        maze = m;
        exitFound = false;
    }

    public void checkForExitPath(int r, int c , int steps)
    {
        steps++;
        
        
        if(r >= 0 && c >= 0 && r < maze.length && c < maze[r].length && maze[r][c] == 1)
        {
            if(c == maze[r].length -1){
                exitFound = true;
                shortest = Math.min(shortest,steps);
                return;
            }
            else {
                maze[r][c] = 0;
                checkForExitPath(r+1, c, steps);
                checkForExitPath(r-1, c, steps);
                checkForExitPath(r, c+1, steps);
                checkForExitPath(r, c-1, steps);
                maze[r][c] = 1;
            }
        }      
    }

    public String toString()
    {
        String output="";
        if (exitFound){
            output += "shortest path of " + shortest;
        }
        else{
            output += "no path";
        }
	return output;
    }
}
