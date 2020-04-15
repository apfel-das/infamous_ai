package searchOps;

import java.util.ArrayList;
import java.util.LinkedList;

import setup.Node;
import setup.Grid;

public class searchBFS extends Search {

	@Override
	public Node solve(Grid mygrid, int N, int M) {
		int cost = 0;
		//exclude any walls from getting visited, and initalize a boolean flag-path and a queue.
		boolean [][] visited = initPath(mygrid,N,M);
		LinkedList<Node> queue = new LinkedList<Node>();
		
		//the root - Node				
		Node src = new Node(mygrid.getStart()[0],mygrid.getStart()[1]);
		
		//mark as visited and insert to queue
		visited[src.getRow()][src.getCol()] = true;
		queue.add(src);
		
		while(!queue.isEmpty()) 
		{
			//pop from queue.Retrieve the head.
			Node curr = queue.poll();
			
			
			//case target is found.
			if(mygrid.getCell(curr.getRow(),curr.getCol()).isTerminal()) 
				return curr;
			
			
			//Constructing children. Four possible dirs, 4 possible children per visited Node.
			
			ArrayList<Node> neighbors = generateChildren(curr, mygrid, visited, M, N);
			
			//mark them as visited.
			for(Node n : neighbors) 
			{
				queue.add(n);
			}
		}
		
		
		
		return null;
	}

	
}
