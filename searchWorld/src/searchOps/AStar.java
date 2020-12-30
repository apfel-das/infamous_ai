package searchOps;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;

import setup.Grid;
import setup.Node;


public class AStar extends Search {

	protected final int MOVE_COST;
	
	/**
	 
	 Class constructor

	 */
	public AStar(int mCost) 
	{
		//cost of horizontal, vertical movement through a bush.
		MOVE_COST = mCost;
		
	}



	/**
	 * @brief 	   Solves the maze by implementing a A-Star (A*) search on Nodes.
	 * @brief 	   
	 * @param      mygrid  The grid
	 * @param      N       Grid's rows.
	 * @param      M       Grid's collumns.
	 * @warning    Uses a Stack
	 * @return     The head Node of the solution path. 
	 */

	@Override
	public Node solve(Grid mygrid, int N, int M) 
	{
		
		//Data structures to hold various information.
		boolean [][] visited = initPath(mygrid,N,M);
	    HashMap<Node, Double> distances = new HashMap<>();
	    PriorityQueue<Pair> queue = new PriorityQueue<>();
		
	    
	    //initialize the distances map.
	    for(int i =0; i<mygrid.getNumOfRows(); i++) 
	    {
	    	for(int j = 0; j < mygrid.getNumOfColumns(); j++) 
	    	{
	    		Node n = new Node(i,j);
	    		distances.put(n, Double.MAX_VALUE);
	    		
	    		
	    	}
	    }
	    
	    
		
		// root and target Nodes.
		Node src = new Node(mygrid.getStart()[0], mygrid.getStart()[1]);
		Node target = new Node(mygrid.getTerminal()[0], mygrid.getTerminal()[1]);
	
		// initial distance of src Node.
		distances.put(src, 0.0);
		queue.add(new Pair(src, 0,0));
		
		//traverse
		while(!queue.isEmpty()) 
		{
			
			// the current Node.
			Node curr = queue.poll().getNode();
			
			if(!visited[curr.getRow()][curr.getCol()])
				visited[curr.getRow()][curr.getCol()] = true;
			
				
			// solved.
			if(mygrid.getCell(curr.getRow(), curr.getCol()).isTerminal())
				return curr;
			
			
			
			ArrayList<Node> neighbors = generateChildren(curr, mygrid, visited, M, N);
			
			for(Node n : neighbors) 
			{
				
				double predictedDist = calcDistance(n,target);
				double neighDist = mygrid.getCell(n.getRow(), n.getCol()).isGrass() ? calcDistance(curr,n)*MOVE_COST : calcDistance(curr,n);
				double totDist = calcDistance(curr,src) + predictedDist + neighDist;
				double d = distances.get(n);
			
				if(totDist < d) 
				{
					Pair p = new Pair(n,totDist,predictedDist);
					
					n.setParent(curr);
					distances.put(n, totDist);
					queue.add(p);
				}
				
			}
			
		}
		
		
		return null;
	}

	/**
	 * This class describes an extension of a Node in order to bind heuristics to the Node.
	 * @author apfel-das 
	 */
	class Pair implements Comparable<Object> 
	{

	    Node node;
	    double totalDistance; //f = g + h.
	    double predictedDistance; //heuristics - h

	    public Pair(Node n, double totalDistance, double predictedDistance) {
	        this.node = n;
	        this.totalDistance = totalDistance;
	        this.predictedDistance = predictedDistance;
	    }

	    @Override
	    public int compareTo(Object o) {
	        if (this.totalDistance > ((Pair) o).totalDistance) {
	            return 1;
	        } else if (this.totalDistance < ((Pair) o).totalDistance) {
	            return -1;
	        } else {
	            return 0;
	        }
	    }

	    public Node getNode() {
	        return node;
	    }
	}
	
	 protected double calcDistance(Node a, Node b) {
	        return Math.sqrt(Math.pow(a.getRow() - b.getRow(), 2) + Math.pow(a.getCol() - b.getCol(), 2));
	    }
	
	 
	 
	
}



