// Nathan Dow
// Period 1
// ShortestPathGraphRunner.java




/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.shortestpathgraph;

import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import static java.lang.System.*;
/**
 *
 * @author Nathan Dow
 */
public class ShortestPathGraphRunner {

    public static void main( String[] args ) throws IOException
    {
        Scanner file = new Scanner(new File("graph2.dat"));
        int howManyTimes = file.nextInt();
	file.nextLine();
        
        for(int x=0; x<howManyTimes; x++){
            
            ShortestPathGraph graph = new ShortestPathGraph(file.nextLine());
            String line = file.nextLine();
            boolean connected = graph.contains(line);
            if(connected) System.out.println(line.substring(0,1) + " connects to " + line.substring(1) + " == " + "yes " + "in " + graph.getShortest() + " steps");
            else System.out.println(line.substring(0,1) + " connects to " + line.substring(1) + " == " + "no");
            
            
	}
        
    }
    
}
