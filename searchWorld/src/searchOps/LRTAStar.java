package searchOps;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import searchOps.AStar.Pair;
import setup.Grid;
import setup.Node;


public class LRTAStar
{

	
	private Pair[][] searchArea;
    private static final int RIGHT = 1;
    private static final int LEFT = -1;
    private static final int UP = -1;
    private static final int DOWN = 1;
    private Grid mygrid;
    private int landCost;
    private int grassCost;

    public LRTAStar(Grid mygrid, int lCost, int gCost) {
    	landCost = lCost;
    	grassCost = gCost;
        this.mygrid = mygrid;
        this.searchArea = new Pair[mygrid.getNumOfRows()][mygrid.getNumOfColumns()];
        
    }

    private void setPairs(Pair target) {
    	

        for (int i = 0; i < searchArea.length; i++) 
        {
            for (int j = 0; j < searchArea[0].length; j++) 
            {

                Pair Pair = new Pair(i,j);
                Pair.calculateHeuristic(target);


                if(mygrid.getCell(i,j).isWall()) Pair.setWall(true);
                else if(mygrid.getCell(i,j).isGrass()) Pair.setGrass(true);
                else if(mygrid.getCell(i,j).isLand()) Pair.setLand(true);

                this.searchArea[i][j] = Pair;
            }
        }
    }

    private boolean isInBoundsX(int idx)
    { 
    	return idx >= 0 && idx < mygrid.getNumOfRows(); 
    }

    private boolean isInBoundsY(int idx)
    { 
    	return idx >= 0 && idx < mygrid.getNumOfColumns(); 
    }

    public ArrayList<Node> solve(){

    	
    	//src and target Pairs.
    	Pair src = new Pair(mygrid.getStart()[0], mygrid.getStart()[1]);
    	Pair target = new Pair(mygrid.getTerminal()[0], mygrid.getTerminal()[1]);
        
    	// init a distancing map.
    	setPairs(target);
    	
        
    	//Initial Pair.
        Pair curr = src;
        Pair node = null;
        //A conversion Array, GridGenerator works with Nodes.
        ArrayList<Node> path = new ArrayList<Node>();

        while(curr != null){
        	
        	//converting real - time.
        	path.add(new Node(curr.getX(),curr.getY()));
        	
        	//mark as visited.
        	curr.visited = true;
        
        	//solved
            if (mygrid.getCell(curr.getX(), curr.getY()).isTerminal())
            	return path;
               
          

            // fix mathematical initialization.
            if (curr.getH() == 0) 
            	curr.setH(curr.calculateHeuristic(target));

            if (!mygrid.getCell(curr.getX(), curr.getY()).isStart())
            {
                int minFValue = Integer.MAX_VALUE;
                List<Pair> adjacentPairs = getValidAdjacents(node);
                for (Pair n : adjacentPairs) {
                	
                    int fValue = getLRTACost(n,target);
                    if (fValue < minFValue){
                        minFValue = fValue;
                       
                    }
                }
                node.setH(minFValue);
            }

            
            int minFValue = Integer.MAX_VALUE;
            Pair nextPair = null;
            List<Pair> adjacentPairs = getValidAdjacents(curr);

            for (Pair n : adjacentPairs){
                int fValue = getLRTACost(n,target);
                n.setVisited(true);
            	
                if (fValue < minFValue)
                {
                	
                    minFValue = fValue;
                    nextPair = n;
                    
                }
            }

            if (nextPair.isGrass())
                nextPair.setPathCost(curr.getPathCost() + grassCost);
            else
                nextPair.setPathCost(curr.getPathCost() + landCost);

            
            node  = curr;
            curr = nextPair;
            curr.setParent(node);
           
           
        }
        return null;
    }

    private List<Pair> getValidAdjacents(Pair Pair) {

        List<Pair> neighbours = new ArrayList<Pair>();

        if(isInBoundsY(Pair.getY() + UP )){
            if(!mygrid.getCell(Pair.getX(),Pair.getY() + UP).isWall()){
                neighbours.add(this.searchArea[Pair.getX()][Pair.getY() + UP]);
            }
        }

        if(isInBoundsX(Pair.getX() + LEFT )){
            if(!mygrid.getCell(Pair.getX() + LEFT,Pair.getY()).isWall()){
                neighbours.add(this.searchArea[Pair.getX() + LEFT][Pair.getY()]);
            }
        }

        if(isInBoundsY(Pair.getY() + DOWN )){
            if(!mygrid.getCell(Pair.getX(),Pair.getY() + DOWN).isWall()){
                neighbours.add(this.searchArea[Pair.getX()][Pair.getY() + DOWN]);
            }
        }

        if(isInBoundsX(Pair.getX() + RIGHT )){
            if(!mygrid.getCell(Pair.getX() + RIGHT,Pair.getY()).isWall()){
                neighbours.add(this.searchArea[Pair.getX() + RIGHT][Pair.getY()]);
            }
        }
        return neighbours;
    }

    private int getLRTACost(Pair candNext, Pair target)
    {
        int fValue;
        int kValue;

  
        int hValue = candNext.getH();
        if(candNext.isGrass())
            kValue = 2;
        else
            kValue = 1;
        fValue = hValue + kValue;
    
        return fValue;
    }

   
 
    
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

	
	// Inner Class. Represents an extended Node
		class Pair
		{

			    private int x;
			    private int y;
			    private boolean visited;
			    private int pathCost;
			    private Pair parent;
			    private int f = 0;
			    private int g;
			    private int h;
			    private boolean isWall;
			    private boolean isGrass;
			    private boolean isLand;
			    private boolean isStarting;
			    private boolean isTerminal;

			    public Pair(int posx,int posy){

			        this.x = posx;
			        this.y = posy;
			        this.isGrass = false;
			        this.isLand = false;
			        this.isWall = false;
			        this.isStarting = false;
			        this.isTerminal = false;
			    }

			    public Pair(int posx,int posy,boolean visited,int pathCost){

			        this.x = posx;
			        this.y = posy;
			        this.visited = visited;
			        this.pathCost = pathCost;
			    }

			    public boolean equals(Object o) 
				{
					Pair p = (Pair ) o;
					return this.x == p.getX() && this.y == p.getY();
				}
			    public String toString(){
			        return "Position: (" + this.getX() + "," + this.getY() + ") - " + "Visited: " + this.isVisited() + "\nCost till now: " + getPathCost();
			    }

			    public int getX() {
			        return x;
			    }

			    public int getY() {
			        return y;
			    }

			    public boolean isVisited() {
			        return visited;
			    }

			    public void setVisited(boolean visited) {
			        this.visited = visited;
			    }

			    public int getPathCost() {
			        return pathCost;
			    }

			    public void setPathCost(int pc) {
			        this.pathCost = pc;
			    }

			    public int calculateHeuristic(Pair finalPair) {  this.h = Math.abs(finalPair.getX() - getX()) + Math.abs(finalPair.getY() - getY());
			        return this.h;
			    }

			    public void setPairData(Pair currentPair, int cost) {
			        int gCost = currentPair.getG() + cost;
			        setParent(currentPair);
			        setG(gCost);
			        calculateFinalCost();
			    }


			    private void calculateFinalCost() {
			        int finalCost = getG() + getH();
			        setF(finalCost);
			    }

			    public Pair getParent() {
			        return parent;
			    }

			    public void setParent(Pair currentPair) {
			        this.parent = currentPair;
			    }

			    public int getH() {
			        return h;
			    }

			    public void setH(int h) {
			        this.h = h;
			    }

			    public int getG() {
			        return g;
			    }

			    public void setG(int g) {
			        this.g = g;
			    }

			    public int getF() {
			        return f;
			    }

			    public void setF(int f) {
			        this.f = f;
			    }

			    public boolean isWall() {
			        return isWall;
			    }

			    public void setWall(boolean wall) {
			        isWall = wall;
			    }

			    public boolean isGrass() {
			        return isGrass;
			    }

			    public void setGrass(boolean grass) {
			        isGrass = grass;
			    }

			    public boolean isLand() {
			        return isLand;
			    }

			    public void setLand(boolean land) {
			        isLand = land;
			    }

			    public boolean isStarting() {
			        return isStarting;
			    }

			    public void setStarting(boolean starting) {
			        isStarting = starting;
			    }

			    public boolean isTerminal() {
			        return isTerminal;
			    }

			    public void setTerminal(boolean terminal) {
			        isTerminal = terminal;
			    }
			    public void print() 
			    {
			    	System.out.println(this.toString());
			    }
		}
		
	
}
