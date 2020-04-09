/**
			INTELLIGENCE LAB
	course		: 	COMP 417 - Artificial Intelligence
	authors		:	A. Georgara, A. Vogiatzis
	excercise	:	1st Programming
	term 		: 	Spring 2018-2019
	date 		:   March 2019
*/
import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.JFrame;
import java.awt.Color;
import java.awt.Rectangle;
import java.awt.TexturePaint;
import java.awt.image.BufferedImage;
import java.io.*;
import javax.imageio.ImageIO;

public class Drawing extends Canvas{
	public static int N = 13;
	public static int M = 9;
	public static int [] walls;
	public static int [] grass;
	public static int [] step_sequence=null;
	public static int start_idx,terminal_idx;
	public static int s =30;

	Drawing(int N, int M, int[] walls, int[] grass,int start_idx,int terminal_idx){
		this.N = N;
		this.M = M;
		this.walls = walls;
		this.grass = grass;
		this.start_idx = start_idx;
		this.terminal_idx = terminal_idx;
	}
	Drawing(int N, int M, int[] walls, int[] grass, int [] step_sequence,int start_idx,int terminal_idx){
		this.N = N;
		this.M = M;
		this.walls = walls;
		this.grass = grass;
		this.step_sequence = step_sequence;
		this.start_idx = start_idx;
		this.terminal_idx = terminal_idx;
	}

	public void paint(Graphics g){
		Graphics2D g2 = (Graphics2D) g;
		BufferedImage img = null;

		for(int i = 0; i<M; i++){
			for (int j=0; j<N; j++){
				g2.setColor(new Color(210,180,140) );
				try{
					img = ImageIO.read(new File("img/land2.jpg")); 
					Rectangle r = new Rectangle(0,0,s,s);
					g2.setPaint(new TexturePaint(img, r));
					Rectangle rect = new Rectangle(i*s,j*s,s,s);
    				g2.fill(rect);
				}catch(IOException e){System.out.println("Error");}

				g2.setColor(Color.BLACK);
				g2.drawLine(i*s, j*s,(i+1)*s, j*s);
				g2.drawLine(i*s, j*s,i*s, (j+1)*s);

			}
		}

		int k = 0;
		while(k<walls.length){
			g2.setColor(Color.DARK_GRAY);
			int i = walls[k]%M;
			int j = walls[k]/M;

			try{
				img = ImageIO.read(new File("img/wall.png")); 
				Rectangle r = new Rectangle(0,0,s,s);
				g2.setPaint(new TexturePaint(img, r));
				Rectangle rect = new Rectangle(i*s,j*s,s,s);
				g2.fill(rect);
			}catch(IOException e){System.out.println("Error");}

			g2.setColor(Color.BLACK);
			g2.drawLine(i*s, j*s,(i+1)*s, j*s);
			g2.drawLine(i*s, j*s,i*s, (j+1)*s);
			k++;
		}

		k = 0;
		while(k<grass.length){
			g2.setColor(new  Color(34,139,34));
			int i = grass[k]%M;
			int j = grass[k]/M;

			try{
				img = ImageIO.read(new File("img/grass.png")); 
				Rectangle r = new Rectangle(0,0,s,s);
				g2.setPaint(new TexturePaint(img, r));
				Rectangle rect = new Rectangle(i*s,j*s,s,s);
				g2.fill(rect);
			}catch(IOException e){System.out.println("Error");}

			g2.setColor(Color.BLACK);
			g2.drawLine(i*s, j*s,(i+1)*s, j*s);
			g2.drawLine(i*s, j*s,i*s, (j+1)*s);
			k++;
		}

		if(step_sequence!=null){
			k = 0;
			while(k<step_sequence.length){
				g2.setColor(new  Color(34,139,34));
				int i = step_sequence[k]%M;
				int j = step_sequence[k]/M;

				try{
					img = ImageIO.read(new File("img/walk.jpg")); 
					Rectangle r = new Rectangle(0,0,s,s);
					g2.setPaint(new TexturePaint(img, r));
					Rectangle rect = new Rectangle(i*s,j*s,s,s);
					g2.fill(rect);
				}catch(IOException e){System.out.println("Error");}

				g2.setColor(Color.BLACK);
				g2.drawLine(i*s, j*s,(i+1)*s, j*s);
				g2.drawLine(i*s, j*s,i*s, (j+1)*s);
				k++;
			}
		}

		int i=terminal_idx%M;
		int j=terminal_idx/M;
		try{
			img = ImageIO.read(new File("img/terminal3.png")); 
			Rectangle r = new Rectangle(0,0,s,s);
			g2.setPaint(new TexturePaint(img, r));
			Rectangle rect = new Rectangle(i*s,j*s,s,s);
			g2.fill(rect);
		}catch(IOException e){System.out.println("Error");}
		g2.setColor(Color.BLACK);
		g2.drawLine(i*s, j*s,(i+1)*s, j*s);
		g2.drawLine(i*s, j*s,i*s, (j+1)*s);

		i=start_idx%M;
		j=start_idx/M;
		try{
			img = ImageIO.read(new File("img/start.png")); 
			Rectangle r = new Rectangle(0,0,s,s);
			g2.setPaint(new TexturePaint(img, r));
			Rectangle rect = new Rectangle(i*s,j*s,s,s);
			g2.fill(rect);
		}catch(IOException e){System.out.println("Error");}

		g2.setColor(Color.BLACK);
		g2.drawLine(i*s, j*s,(i+1)*s, j*s);
		g2.drawLine(i*s, j*s,i*s, (j+1)*s);
	}	
}