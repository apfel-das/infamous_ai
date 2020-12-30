package searchOps;

import java.util.ArrayList;
import java.util.Stack;

import setup.Grid;
import setup.Node;

public class searchDFS extends Search{

	/**
	 * @brief 	   Solves the maze by implementing a Depth-First Search (DFS) on Nodes.
	 * @brief 	   
	 * @param      mygrid  The grid
	 * @param      N       Grid's rows.
	 * @param      M       Grid's collumns.
	 * 
	 * @return     The head Node of the solution path. 
	 */
	@Override
	public Node solve(Grid mygrid, int N, int M) {
		
		//a structure to remember visited nodes.
		boolean [][] visited = initPath(mygrid,N,M);
		
		
		//a stack object.
		Stack<Node> stack = new Stack<>();
		//the root Node.
		Node src = new Node(mygrid.getStart()[0], mygrid.getStart()[1]);
		
		//pushing in the src.
		stack.push(src);
		
		//traverse
		while(!stack.isEmpty()) 
		{
			Node curr = stack.pop();
			
			
			//solved.
			if(mygrid.getCell(curr.getRow(),curr.getCol()).isTerminal())
			{
				return curr;
			
			}
			
			// fundamental difference of DFS (vs BFS).
			if(curr.getRow() < 0 || curr.getCol() < 0 || curr.getCol() >= M || curr.getRow() >= N)
				continue;
			
			// Constructing children, see BFS for more.
			
			ArrayList<Node> neighbors = generateChildren(curr, mygrid, visited, M, N);
			
			//mark children as to be visited.
			for(Node n : neighbors) 
			{
				stack.push(n);
			}
			
		}
		
		
		return null;
		
	}
	
	

}
