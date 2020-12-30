package searchOps;

import java.util.ArrayList;
import java.util.Collections;

import setup.Grid;
import setup.Node;

/**
 * 
 * @author apfel-das
 * @brief Describes the general form of a search class. All non-abstract methods are to be used from extending classes.  
 * By extending this you have to implement a special search technique on given grid by implementing the abstract methods.
 * 
 * 
 * 
 */

public abstract class Search {
	
	
	/*
	 * Abstract methods to be implemented from subclasses.
	 * 
	 */
	
	
	

	/**
	 *
	 * @brief Solves the maze with implemented technique.
	 * @author     apfel-das
	 * @param      mygrid  The grid
	 * @param      N       Grid's rows.
	 * @param      M       Grid's collumns.
	 * @warning    This must be implemented in child classes.
	 * @return     The head Node of the solution path. 
	 */
	abstract public Node solve(Grid mygrid, int N, int M);
	


	/**
	 * @brief      Generates child Nodes for each possible (and valid) movement direction on the grid [up, down, left, right] based on the current Node.
	 * @note 	   By the term "children" we mean the adjacent/neighbor Nodes of <Node curr>.
	 * @note       This also marks each adjacent cell as visited and sets his parent field as <Node curr>.
	 * @note       There is a chance that different cells have common neighbors, if so a neighbor marked as "visited" will not get recreated or reprocessed.
	 * @author     apfel-das
	 * @param      curr    The current position-Node in the grid.
	 * @param      mygrid  The grid
	 * @param 	   visited[][] An array which marks already visited Nodes.
	 * @param      N       Grid's rows.
	 * @param      M       Grid's collumns.
	 * 
	 * @return    An arraylist containig children/adjacent/ neighbor Nodes.
	 */

	
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

		/**
		 * Initializes the path's marker array. Creates and returns an array with cells representing (accessible or not) parts of a path marked based on their abillity to be "visited".
		 * @brief This is symmetrical with marking each cell of the Grid as visited or not (yet).
		 * @note Non accessible parts are marked as Walls in the grid.
		 * @note Either Land or Grass Cells can be considered as accessible.
		 * @param      mygrid  The mygrid
		 * @param      N       Grid's rows.
		 * @param      M       Grid's collumns
		 * @warning    Each grid's Cell marked as "visited" will not be accesed at any stage.
		 * @return     The initialized "visited" array. 
		 */
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
		 * @brief Recreates the path from root to solution. Following function given a "solution" Node (usually provided from solve()) returns the full path [root->...->solution] as an arraylist of <Node>(s).
		 * @brief To do so it links the parent Node to it's ancestor and the 1st ancestor with it's pre-ancestor etc.	
		 * @note  Used also as an entry-point for the path's cost calculation (by calling printMetrics() ).
		 * @note  Linkage of Node with its  parents is crucial for backtracking the path (see generateChildren() to understand how this happens).	 
		 * @author     apfel-das
		 * @param      target  The final - target Node.
		 * @param      gCost   The grass cost
		 * @param      mygrid  The mygrid
		 *
		 * @return     An arraylist of Nodes depicting the path in the form [root->....->target].
		 */
		public ArrayList<Node> backtrack(Node target, int gCost, Grid mygrid)
		{
			
			// Get a terminal Node, construct a solution.
			ArrayList<Node> path = new ArrayList<Node>();
			
			
			
			//iterate parent-wise. 
			while(target != null) 
			{
				
					
					
				//add to path, then move to it's predecessor.
				path.add(target);
				target = target.getParent();
					
							
				
			}
			
			//reverse the path in a Root->..-> Terminal order. THIS MIGHT BE non-compatible with versions < Java8.
			Collections.reverse(path);
			
			// Inform about the cost of each given path.
			printMetrics(path, gCost, mygrid);
			
			return path;
			
		}
		/**
		 * Calculates the path's cost and prompts user.
		 * @author     apfel-das
		 * @param      path    The path as an arraylist of Nodes.
		 * @param      gCost   The grass cost which can be varying.
		 * @param      mygrid  The grid.
		 */
		public void printMetrics(ArrayList<Node> path, int gCost, Grid mygrid) 
		{
			int fCost = 0;
			int grass = 0;
			int land  = 0;
			
			for(Node curr : path) 
			{
				//add things up.
				fCost += mygrid.getCell(curr.getRow(), curr.getCol()).isLand() ? 1 : gCost;
				
				if(mygrid.getCell(curr.getRow(), curr.getCol()).isLand())
					land++;
				else
					grass++;
				
			}
			System.out.println("----------------------------------INFO:-----------------------------------------------");
			System.out.println("\nCurrent path has: "+grass+" Grass nodes "+land+" Land Nodes. Traverse Penalty: "+fCost+" [Grass Cost = "+gCost+"]");
			
			
		}
}
