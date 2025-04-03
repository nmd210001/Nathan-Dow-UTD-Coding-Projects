/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.shortestpathmaze;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

/**
 *
 * @author Nathan Dow
 */
public class ShortestPathMazeRunner {

    public static void main(String[] args) throws FileNotFoundException {
        Scanner scan = new Scanner(new File("maze2.dat"));
        
       
        
        while(scan.hasNext()){
            int num = scan.nextInt();
            int[][] input = new int[num][num];
            for(int r = 0; r < num; r ++){
                
                for(int c = 0; c < input[r].length; c++){
                   
                    input[r][c] = scan.nextInt();
                    
                }
                
            }
            
            for(int[] row : input){
                
                for(int index : row){
                    System.out.print(index + " ");
                }
                
              System.out.println();
            }
            
            ShortestPathMaze maze = new ShortestPathMaze(input);
            maze.checkForExitPath(0, 0, 0);
            
            System.out.println(maze);
            System.out.println();
        
        }
    }
}
