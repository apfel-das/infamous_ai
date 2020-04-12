package setup;

public class Node {

	private int row;
	private int col;
	private int dist;
	private Node parent;
	
	public Node(int i, int j) 
	{
		row = i;
		col = j;
		dist = 0;
	}
	
	
	public Node getParent() 
	{
		return parent;
	}
	
	public void setParent(Node p) 
	{
		parent = p;
	}
	public int getRow() 
	{
		return row;
	}
	
	public int getCol() 
	{
		return col;
	}
	
	public void setDist(int d) 
	{
		dist = d;
	}
	public int getDist() 
	{
		return dist;
	}
	
	public String toString() 
	{
		return "("+Integer.toString(row)+","+Integer.toString(col)+")";
	}
	
	public void print() 
	{
		System.out.println(this.toString());
	}
}
