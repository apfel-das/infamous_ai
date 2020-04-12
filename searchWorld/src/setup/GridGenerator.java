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


import java.util.Random;
import java.util.Scanner;
import java.util.Stack;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.*;


import java.awt.Graphics;
import java.awt.Color;
import java.util.ArrayList;
import java.util.LinkedList;
import java.awt.Canvas; 

class GridGenerator{

	public static void main(String[] args) {
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
					int [] path = BFS(mygrid,N,M);
					
					VisualizeGrid("BFS Solution",N,M,mygrid.getWalls(),mygrid.getGrass(),path,mygrid.getStartidx(),mygrid.getTerminalidx());
					break;
				case "d":
					int [] path_d = DFS(mygrid,N,M);
					
					VisualizeGrid("DFS Solution",N,M,mygrid.getWalls(),mygrid.getGrass(),path_d,mygrid.getStartidx(),mygrid.getTerminalidx());
					break;	
				default:
				
			}
			
		
			
			
		
			
		}while(ch.compareTo("q") != 0 || !isValidCmd(ch));
		
		
		
		
		
		
		
		
	}
	
	//print the available commands in a menu - style printing.
	private static void menuPrint() 
	{
		
		
		System.out.println("\nType a command or q (Exit):");
		System.out.println("v - Visualize the given maze.");
		System.out.println("b - Solve the given maze with BFS.");
		System.out.println("d - Solve the given maze with DFS.");
		System.out.println("a - Solve the given maze with A*.");
		
	}
	
	// estimate input's validity via a regex.
	private static boolean isValidCmd(String cmd) 
	{
		String valid = "[abdvq]";
		
		Matcher m = Pattern.compile(valid).matcher(cmd);
		
		if(!m.matches()) 
		{
			System.out.println("Bad Command");
			return false;
		}
		
		return true;
		
		
	}
	
	private static int[] BFS(Grid mygrid, int N, int M) 
	{
		//initialize a grid Array.
				
			
				//will be filled up with the solution - path as Nodes.
				ArrayList<Integer> solution = new ArrayList<Integer>();				
				
			
				boolean [][] visited = new boolean [N][M];
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
					Node c;
					
					//case target is found.
					
					if(mygrid.getCell(curr.getRow(),curr.getCol()).isTerminal()) 
					{
						
						//fill the solution ArrayList with the proper Node - path.
						while(curr != null) 
						{
							/* Remove block comment if debugging

							   curr.print();
							*/
							
							solution.add(curr.getRow()*M + curr.getCol());
							curr = curr.getParent();
						}
						
						
						//nothing else to be done.
						break;
					}
					
					//Constructing children. Four possible dirs, 4 possible children per visited Node.
					
					//up
					if(curr.getRow()-1 >= 0 && visited[curr.getRow()-1][curr.getCol()] == false) 
					{
						visited[curr.getRow()-1][curr.getCol()] = true;
						c = new Node(curr.getRow()-1, curr.getCol());
						c.setParent(curr);
						queue.add(c);
					}
					//down
					if(curr.getRow()+1 >= 0 && curr.getRow()+1 < N && visited[curr.getRow()+1][curr.getCol()] == false) 
					{
						visited[curr.getRow()+1][curr.getCol()] = true;
						c = new Node(curr.getRow()+1, curr.getCol());
						c.setParent(curr);
						queue.add(c);
					}
					//left
					if(curr.getCol()-1 >= 0 && visited[curr.getRow()][curr.getCol()-1] == false) 
					{
						visited[curr.getRow()][curr.getCol()-1] = true;
						c = new Node(curr.getRow(), curr.getCol()-1);
						c.setParent(curr);
						queue.add(c);
					}
					//right
					
					if(curr.getCol()+1 >= 0 && curr.getCol() + 1 < M && visited[curr.getRow()][curr.getCol()+1] == false) 
					{
						visited[curr.getRow()][curr.getCol()+1] = true;
						c = new Node(curr.getRow(), curr.getCol()+1);
						c.setParent(curr);
						queue.add(c);
					}
				}
				
				//using Java > 8 will do the work here. Conversion of the ArrayList to int[] primitive, filtering out the null_values via lamda function.
				int[] arr = solution.stream().filter(i -> i != null).mapToInt(i -> i).toArray();
				
				return arr;
	}
	
	
	private static int[] DFS(Grid mygrid, int N, int M) 
	{
		//will be filled up with the solution - path as Nodes.
		ArrayList<Integer> solution = new ArrayList<Integer>();
		
		//a structure to remeber visited nodes.
		boolean [][] visited = new boolean [N][M];
		//mark the walls.
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
			Node c;
			
			//solved.
			if(mygrid.getCell(curr.getRow(),curr.getCol()).isTerminal())
			{
				
				while(curr.getParent() != null) 
				{
					solution.add(curr.getRow()*M + curr.getCol());
					curr = curr.getParent();
				}
			}
			
			if(curr.getRow() < 0 || curr.getCol() < 0 || curr.getCol() >= M || curr.getRow() >= N)
				continue;
			
			//moving
			
			//up
			if(curr.getRow()-1 >= 0 && visited[curr.getRow()-1][curr.getCol()] == false) 
			{
				visited[curr.getRow()-1][curr.getCol()] = true;
				c = new Node(curr.getRow()-1, curr.getCol());
				c.setParent(curr);
				stack.push(c);
			}
			//down
			if(curr.getRow()+1 >= 0 && curr.getRow()+1 < N && visited[curr.getRow()+1][curr.getCol()] == false) 
			{
				visited[curr.getRow()+1][curr.getCol()] = true;
				c = new Node(curr.getRow()+1, curr.getCol());
				c.setParent(curr);
				stack.push(c);
			}
			//left
			if(curr.getCol()-1 >= 0 && visited[curr.getRow()][curr.getCol()-1] == false) 
			{
				visited[curr.getRow()][curr.getCol()-1] = true;
				c = new Node(curr.getRow(), curr.getCol()-1);
				c.setParent(curr);
				stack.push(c);
			}
			//right
			
			if(curr.getCol()+1 >= 0 && curr.getCol() + 1 < M && visited[curr.getRow()][curr.getCol()+1] == false) 
			{
				visited[curr.getRow()][curr.getCol()+1] = true;
				c = new Node(curr.getRow(), curr.getCol()+1);
				c.setParent(curr);
				stack.push(c);
			}
			
		}
		
		
		//using Java > 8 will do the work here. Conversion of the ArrayList to int[] primitive, filtering out the null_values via lamda function.
		int[] arr = solution.stream().filter(i -> i != null).mapToInt(i -> i).toArray();
		
		return arr;
	}
	
	
	/**
	 * Function to visualize the initial grid.
	 * @author Athena Georgaraki, Antonis Vogiatzis (INTELLIGENCE LAB)
	 * */
	public static void VisualizeGrid(String frame_name, int N, int M, int [] walls, int [] grass, int start_idx, int terminal_idx ){
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
	 * 
	 * */
	
	public static void VisualizeGrid(String frame_name, int N, int M, int [] walls, int [] grass, int [] steps ,int start_idx, int terminal_idx ){
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
	

