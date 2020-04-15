package searchOps;

import java.util.ArrayList;
import java.util.Collections;

import setup.Grid;
import setup.Node;

public abstract class Search {
	
	
	
	/*
	 * Abstract methods to be implemented from subclasses.
	 * */
	
	//Solve the maze.
	abstract public Node solve(Grid mygrid, int N, int M);
	
	
	// Generates children Nodes for the 4 possible directions on a maze-grid.
	protected ArrayList<Node> generateChildren(Node curr, Grid mygrid, boolean visited[][], int M, int N)
	{
		ArrayList<Node> children = new ArrayList<Node>();
	
		Node c;
		
		//up
		if(curr.getRow()-1 >= 0 && visited[curr.getRow()-1][curr.getCol()] == false) 
		{
			visited[curr.getRow()-1][curr.getCol()] = true;
			c = new Node(curr.getRow()-1, curr.getCol());
			c.setParent(curr);
			children.add(c);
		}
		//down
		if(curr.getRow()+1 >= 0 && curr.getRow()+1 < N && visited[curr.getRow()+1][curr.getCol()] == false) 
		{
			visited[curr.getRow()+1][curr.getCol()] = true;
			c = new Node(curr.getRow()+1, curr.getCol());
			c.setParent(curr);
			children.add(c);
		}
		//left
		if(curr.getCol()-1 >= 0 && visited[curr.getRow()][curr.getCol()-1] == false) 
		{
			visited[curr.getRow()][curr.getCol()-1] = true;
			c = new Node(curr.getRow(), curr.getCol()-1);
			c.setParent(curr);
			children.add(c);
		}
		//right
		
		if(curr.getCol()+1 >= 0 && curr.getCol() + 1 < M && visited[curr.getRow()][curr.getCol()+1] == false) 
		{
			visited[curr.getRow()][curr.getCol()+1] = true;
			c = new Node(curr.getRow(), curr.getCol()+1);
			c.setParent(curr);
			children.add(c);
		}
		
		return children;
		
	}

	// Creates and returns an array, with cells representing accessible or not parts of a path.
		protected boolean [][] initPath(Grid mygrid, int N, int M )
		{
			boolean [][] visited = new boolean[N][M];
			
			for(int i = 0; i<N; i++) 
			{
				for(int j=0; j<M; j++) 
				{
					//a wall will never be visited after all.
					if(mygrid.getCell(i, j).isWall())
						visited[i][j] = true;
					else
						visited[i][j] = false;
					
					
				}
			}
			return visited;
		}	

		/**
		 * Given the solution-Node, this returns the full path [root->..->solution]. To do so it links the parent Node to it's ancestor and the 1st ancestor with it's pre-ancestor etc.
		 * @param target
		 * @return path- ArrayList of Nodes.
		 */
		public ArrayList<Node> backtrack(Node target)
		{
			// Get a terminal Node, construct a solution.
			ArrayList<Node> path = new ArrayList<Node>();
			
			
			
			//iterate parent-wise. 
			while(target != null) 
			{
				//add to path, the move to it's predecessor.
				path.add(target);
				target = target.getParent();
					
							
				
			}
			
			//reverse the path in a Root->..-> Terminal order. THIS MIGHT BE non-compatible with versions < Java8.
			Collections.reverse(path);
			
		
			
			return path;
			
		}
}
