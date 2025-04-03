// Nathan Dow
// Period 1
// ShortestPathGraph.java



/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.shortestpathgraph;

import java.io.File;
import java.io.IOException;
import java.util.Map;
import java.util.TreeMap;
import java.util.Scanner;
import static java.lang.System.*;
import java.util.HashSet;
import java.util.Set;

public class ShortestPathGraph
{
	private TreeMap<String, String> map;
	private boolean found;
        private String been;
	private int shortest;
        

	public ShortestPathGraph(String line)
	{
            been = "";
            map = new TreeMap<String, String>();
            found = false;
            // find the values of each item and its connections
                  
            Scanner scanner = new Scanner(line);
            while(scanner.hasNext()){
                String pair = scanner.next();
                String first = pair.substring(0,1);
                String second = pair.substring(1);
            
                //check if the value is already in the map
                if (map.containsKey(first) == false){
                    map.put(first, second);
                }
                else {
                    String values = map.get(first);
                    map.put(first,values + second);    
                }
            
                if (map.containsKey(second) == false){
                    map.put(second, first);
                }
                else {
                    String values = map.get(second);
                    map.put(second,values + first);    
                }
            }
	}

	public boolean contains(String letter)
	{
            been = "";
            shortest = 100;
            check(letter.substring(0,1), letter.substring(1),0);
            return found;
	}

	public void check(String first, String second, int steps)
        {
            steps++;
            
            been += first;
            
            if (map.get(first).contains(second)){
                found = true;
                shortest = Math.min(shortest,steps);
            }
            else{
                String checkValues = map.get(first);
                    
                for (int i = 0; i < checkValues.length(); i ++){
                    first = checkValues.substring(i, i + 1);
                        
                    if(!been.contains(first)){
                        check(first, second, steps);
                    }
                }  
            }
            
	}
        public int getShortest(){
            return shortest;
        }
            

	public String toString()
	{
            String output = "";
            
            for (String key: map.keySet()){
                output += key + " : " + map.get(key) +"\n";
                
            }
            
            return output;
	}
}