package setup;
/**
	INTELLIGENCE LAB
	course		: 	COMP 417 - Artificial Intelligence
	authors		:	A. Georgara, A. Vogiatzis
	excercise	:	1st Programming
	term 		: 	Spring 2018-2019
	date 		:   March 2019
	expansion	: 	dasApfel - Spring 2020.
*/


import java.util.Scanner;
import javax.swing.*;


import searchOps.AStar;
import searchOps.LRTAStar;
import searchOps.searchBFS;
import searchOps.searchDFS;

import java.util.ArrayList;
import java.util.Collections;
import java.awt.Canvas; 

class GridGenerator{

	public static void main(String[] args) throws InterruptedException {
		
		@SuppressWarnings("unused")
		String frame = "Random World";
		Grid mygrid;
		if (args.length<1)
			mygrid = new Grid();
		else if (args[0].equals("-i")){
			mygrid = new Grid(args[1]);
			frame = args[1].split("/")[1];
		}else if (args[0].equals("-d")){
			mygrid = new Grid(Integer.parseInt(args[1]),Integer.parseInt(args[2]));
		}else{
			mygrid = new Grid("world_examples/hard_c.world");
			frame = "default.world";
		}
		int N = mygrid.getNumOfRows();
		int M = mygrid.getNumOfColumns();
		
		
		/*
		 * Additions below
		 * */
		Scanner in = new Scanner(System.in);
		String ch;
		
		do
		{
			int gCost = 0;
			menuPrint();
			System.out.println();
			System.out.print("CMD> ");
			
			// use newline as delimiter
			in.useDelimiter("\\n"); 
			
			//get trimmed input
			ch = in.nextLine().trim();
			
			
			switch(ch) 
			{
				case "v":
					VisualizeGrid("Initial Maze",N,M,mygrid.getWalls(),mygrid.getGrass(),mygrid.getStartidx(),mygrid.getTerminalidx());
					
					break;
				case "b":
					
					
					
					// initalize grass cost.
					while(gCost != 2 && gCost != 10)
					{
						System.out.println("Choose your preferable Grass Cost [2/10]");
						gCost = Integer.parseInt(in.nextLine().trim());
						System.out.println(gCost);
						
					}
					
					searchBFS bfs =  new searchBFS();
					Node goal = bfs.solve(mygrid, N, M);
					ArrayList<Node> path = bfs.backtrack(goal, gCost,mygrid);
					
					if(goal != null) 
					{
						
						System.out.println("Path found [via BFS]. Print solution? [Y/n] ");
						char c = in.next().charAt(0);
					
						
						if(c == 'y')
						{
							
							System.out.println("Tuples or Visual? [T/v]");
							c = in.next().charAt(0);
							if(c == 't') 
								printPath(path);
							else if(c == 'v')
								VisualizeGrid("BFS Solution"+"["+"Grass Cost"+gCost+"]",N,M,mygrid.getWalls(),mygrid.getGrass(),makePrintable(path,M),mygrid.getStartidx(),mygrid.getTerminalidx());
							else
								break;
							
						}
				
						
						
					}
					else 
					{
						//No solution was found.
						System.out.println("No solution found.");
					}
					
					in.nextLine();
					break;
				case "d":
					
					
					// initalize grass cost.
					while(gCost != 2 && gCost != 10)
					{
						System.out.println("Choose your preferable Grass Cost [2/10]");
						gCost = Integer.parseInt(in.nextLine().trim());
						System.out.println(gCost);
						
					}
					
					searchDFS dfs =  new searchDFS();
					Node goalDFS = dfs.solve(mygrid, N, M);
					ArrayList<Node> pathDFS = dfs.backtrack(goalDFS, gCost, mygrid);
					
					if(goalDFS != null) 
					{
						
						System.out.println("Path found [via DFS]. Print solution? [Y/n] ");
						char c = in.next().charAt(0);
					
						if(c == 'y')
						{
							
							System.out.println("Tuples or Visual? [t/v]");
							c = in.next().charAt(0);
							if(c == 't') 
								printPath(pathDFS);
							else if(c == 'v')
								VisualizeGrid("DFS Solution"+"["+"Grass Cost"+gCost+"]"+gCost,N,M,mygrid.getWalls(),mygrid.getGrass(),makePrintable(pathDFS,M),mygrid.getStartidx(),mygrid.getTerminalidx());
							else
								break;
							
						}
				
						
						
					}
					else 
					{
						//No solution was found.
						System.out.println("No solution found.");
					}
					
					in.nextLine();
					
					
					break;
				case "a":
					
					
					// initalize grass cost.
					while(gCost != 2 && gCost != 10)
					{
						System.out.println("Choose your preferable Grass Cost [2/10]");
						gCost = Integer.parseInt(in.nextLine().trim());
						System.out.println(gCost);
						
					}
					
					
					AStar aStar =  new AStar(gCost);
					Node goalStar = aStar.solve(mygrid, N, M);
					ArrayList<Node> pathStar = aStar.backtrack(goalStar, gCost, mygrid);
					
					if(goalStar != null) 
					{
						
						System.out.println("Path found [via A*]. Print solution? [Y/n] ");
						char c = in.next().charAt(0);
					
						if(c == 'y')
						{
							
							System.out.println("Tuples or Visual? [t/v]");
							c = in.next().charAt(0);
							if(c == 't') 
								printPath(pathStar);
							else if(c == 'v')
								VisualizeGrid("A* Solution"+"["+"Grass Cost"+gCost+"]",N,M,mygrid.getWalls(),mygrid.getGrass(),makePrintable(pathStar,M),mygrid.getStartidx(),mygrid.getTerminalidx());
							else
								break;
							
						}
				
						
					}
					else 
					{
						//No solution was found.
						System.out.println("No solution found.");
					}
					
					in.nextLine();
					
					
					break;
				case "l":
					
					
					// initalize grass cost.
					while(gCost != 2 && gCost != 10)
					{
						System.out.println("Choose your preferable Grass Cost [2/10]");
						gCost = Integer.parseInt(in.nextLine().trim());
						System.out.println(gCost);
						
					}
					
					
					LRTAStar lrtaStar =  new LRTAStar(mygrid,1,gCost);
					ArrayList<Node> lrtaPath = lrtaStar.solve();
					lrtaStar.printMetrics(lrtaPath, gCost, mygrid);
					if(lrtaPath != null) 
					{
						System.out.println("Path found [via LRTA*]. Print solution? [Y/n] ");
						char c = in.next().charAt(0);
					
						if(c == 'y')
						{
							
							System.out.println("Tuples or Visual? [t/v]");
							c = in.next().charAt(0);
							if(c == 't') 
								printPath(lrtaPath);
							else if(c == 'v')
								VisualizeGrid("LRTA* Solution"+"["+"Grass Cost"+gCost+"]",N,M,mygrid.getWalls(),mygrid.getGrass(),makePrintable(lrtaPath,M),mygrid.getStartidx(),mygrid.getTerminalidx());
							else
								break;
							
						}
						
					}
					else 
					{
						//No solution was found.
						System.out.println("No solution found.");
					}
					
					in.nextLine();
					
					
					break;		
				default:
					System.out.println("Bad Command");
				
			}
	
			
		}while(ch.compareTo("q") != 0);
		
		
		in.close();
		
	}
	
	//print the available commands in a menu - style printing.
	private static void menuPrint() 
	{
		
		
		System.out.println("\nType a command or q (Exit):");
		System.out.println("\"v\" - Visualize the given maze.");
		System.out.println("\"b\" - Solve the given maze with BFS.");
		System.out.println("\"d\" - Solve the given maze with DFS.");
		System.out.println("\"a\" - Solve the given maze with A*.");
		System.out.println("\"l\" - Solve the given maze with LRTA*.");
		
	}
	
	/**
	 * Converts given path to an Array format by escaping null-elements.Uses a lambda-expression and some Java 13 magic.Converts (i,j) -> i*M+j format.
	 * @param path
	 * @return a printable representation of the path
	 */
	private static int[] makePrintable(ArrayList<Node> path, int M) 
	{
		//using Java > 8 will do the work here. Conversion of the ArrayList to int[] primitive, filtering out the null_values via lamda function.
		return path.stream().filter(i -> i != null).mapToInt(i -> (i.getRow()*M+i.getCol())).toArray();
	}
	
	/**
	 * Given the solution-Node, this returns the full path [root->..->solution]. To do so it links the parent Node to it's ancestor and the 1st ancestor with it's pre-ancestor etc.
	 * @param target
	 * @return path- ArrayList of Nodes.
	 */
	public static ArrayList<Node> backtrack(Node target)
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
	
	/*
	 * Printing module
	 */
	public static void printInfo(ArrayList<Node> path, boolean sel) 
	{
		//printing info is DISABLED.
		if(!sel)
			return;
		
		System.out.println("Path Length: "+path.size()+"Root: "+path.get(0).toString()+"End: "+path.get(path.size()-1).toString());
	}
	
	public static void printPath(ArrayList<Node> path) 
	{
		System.out.format("\t%40s\n","----------Solution [Root->Goal]----------------");
		for(Node n : path) 
		{
			System.out.format("\t\t\t(%d,%d)\n",n.getRow(),n.getCol());
		}
		
	}
	
	
	
	
	
	
	/*
	 *  Visualization functions
	 */
	
	/**
	 * Function to visualize the initial grid.
	 * @author Athena Georgaraki, Antonis Vogiatzis (INTELLIGENCE LAB)
	 * */
	 public static synchronized void VisualizeGrid(String frame_name, int N, int M, int [] walls, int [] grass, int start_idx, int terminal_idx ){
		JFrame frame = new JFrame(frame_name);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		Canvas canvas = new Drawing(N,M,walls,grass,start_idx,terminal_idx);
		canvas.setSize(M*30,N*30);
		System.out.println("Visualizing..");
		frame.add(canvas);
		frame.pack();
		frame.setVisible(true);
		
	}
	/**
	 * Function to visualize the initial grid and the solution given as Array.
	 * @author Athena Georgaraki, Antonis Vogiatzis (INTELLIGENCE LAB)
	 * @throws InterruptedException 
	 * 
	 * */
	
	 public static synchronized void VisualizeGrid(String frame_name, int N, int M, int [] walls, int [] grass, int [] steps ,int start_idx, int terminal_idx ) throws InterruptedException{
		JFrame frame = new JFrame(frame_name);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		Canvas canvas = new Drawing(N,M,walls,grass, steps, start_idx,terminal_idx);
		canvas.setSize(M*30,N*30);
		System.out.println("Visualizing..");
		frame.add(canvas);
		frame.pack();
		frame.setVisible(true);
		
		
	}

	
	}
	

