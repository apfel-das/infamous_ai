package setup;

public class Node {

	private int row;
	private int col;
	private Node parent;
	
	
	public Node(int i, int j) 
	{
		row = i;
		col = j;
	
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
	
	public boolean equals(Object o) 
	{
		Node n = (Node ) o;
		return this.row == n.getRow() && this.col == n.getCol();
	}
	
	public int hashCode() {
        int hash = 5;
        hash = 13 * hash + this.row;
        hash = 13 * hash + this.col;
       
        return hash;
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
