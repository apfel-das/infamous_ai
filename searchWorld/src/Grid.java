/**
			INTELLIGENCE LAB
	course		: 	COMP 417 - Artificial Intelligence
	authors		:	A. Georgara, A. Vogiatzis
	excercise	:	1st Programming
	term 		: 	Spring 2018-2019
	date 		:   March 2019
*/
import java.util.Random;
import java.io.*;
import java.util.Arrays;

class Grid {
	private int N=13,M=9;
	private Cell [][] mygrid = new Cell[N][M];

	private int [] walls;
	private int [] grass;

	private int start_idx = 96;
	private int terminal_idx = 42;
	private int grass_cost = 2;

	Grid(){
		this.init();
		this.storeWorld();
	}
	Grid(int N, int M){
		this.N = N;
		this.M = M;
		this.mygrid = new Cell[N][M];
		this.init();
		this.storeWorld();
	}

	Grid(int grass_cost){
		this.grass_cost = grass_cost;
		this.init();
		this.storeWorld();
	}

	Grid(String filename){
		this.loadWold(filename);

		for (int i = 0; i<this.N; i++){
			for(int j = 0; j<this.M; j++){
				this.mygrid[i][j] = new Cell('L',((i*this.M+j)==this.start_idx), ((i*this.M+j)==this.terminal_idx),1);
			}
		}
		for (int w = 0; w < this.walls.length; w++){
			int i = this.walls[w]/this.M;
			int j = this.walls[w]%this.M;
			this.mygrid[i][j].changeCellType('W', Integer.MAX_VALUE);
		}
		for (int g = 0; g < this.grass.length; g++){
			int i = this.grass[g]/this.M;
			int j = this.grass[g]%this.M;
			this.mygrid[i][j].changeCellType('G', this.grass_cost);
		}
	}

	public Cell getCell(int i, int j){
		return this.mygrid[i][j];
	}
	public int[] getStart(){
		int [] idx = new int[2];
		idx[0] = this.start_idx/M;
		idx[1] = this.start_idx%M;
		return idx;
	}

	public int[] getTerminal(){
		int [] idx = new int[2];
		idx[0] = this.terminal_idx/M;
		idx[1] = this.terminal_idx%M;
		return idx;
	}
	public int getTerminalidx(){return this.terminal_idx;}
	public int getStartidx(){return this.start_idx;}
	public int getNumOfRows(){return this.N;}
	public int getNumOfColumns(){return this.M;}
	public int[] getWalls(){return this.walls;}
	public int[] getGrass(){return this.grass;}

	private void storeWorld(){
		try{
			File f = new File("newRandomLevel.world");
			FileOutputStream stream = new FileOutputStream(f);
			BufferedWriter bw = new BufferedWriter( new OutputStreamWriter(stream) );

			bw.write("dimensions:"+this.N+"x"+this.M);
			bw.newLine();
			
			String st2w = "";
			st2w += "walls:"+this.walls.length+":";
			for(int w=0; w<this.walls.length; w++){
				st2w += Integer.toString(this.walls[w])+",";
			}
			st2w = st2w.substring(0, st2w.length() - 1);
			bw.write(st2w);
			bw.newLine();

			st2w = "grass:"+this.grass.length+":";
			for(int g=0; g<this.grass.length; g++){
				st2w += Integer.toString(this.grass[g])+",";
			}
			st2w = st2w.substring(0, st2w.length() - 1);
			bw.write(st2w);
			bw.newLine();

			bw.write("start_idx:"+this.start_idx);
			bw.newLine();
			bw.write("terminal_idx:"+this.terminal_idx);
			bw.newLine();
			bw.write("grass_cost:"+this.grass_cost);
			bw.close();

		}catch(IOException e){System.out.println("error on writing");}
	}
	private void loadWold(String filename){
		try{
			File f = new File(filename);
			BufferedReader br = new BufferedReader(new FileReader(f));
			String param;

			param=br.readLine();
			while( param!=null){
				if(param.split(":")[0].equalsIgnoreCase("dimensions")){
					// read board dimensions
					param = param.split(":")[1];
					this.N = Integer.parseInt(param.split("x")[0]);
					this.M = Integer.parseInt(param.split("x")[1]);
				}else if(param.split(":")[0].equalsIgnoreCase("walls")){
					// read wall-cells
					this.walls = new int[Integer.parseInt(param.split(":")[1])];
					param = param.split(":")[2];
					for(int w=0; w<this.walls.length; w++)
						this.walls[w] = Integer.parseInt(param.split(",")[w]);

				}else if(param.split(":")[0].equalsIgnoreCase("grass")){
					// read grass-cells
					this.grass = new int[Integer.parseInt(param.split(":")[1])];
					param = param.split(":")[2];
					for(int g=0; g<this.grass.length; g++)
						this.grass[g] = Integer.parseInt(param.split(",")[g]);
				}else if(param.split(":")[0].equalsIgnoreCase("start_idx")){
					// read start point
					this.start_idx = Integer.parseInt(param.split(":")[1]);
				}else if(param.split(":")[0].equalsIgnoreCase("terminal_idx")){
					// read terminal point
					this.terminal_idx = Integer.parseInt(param.split(":")[1]);
				}else if(param.split(":")[0].equalsIgnoreCase("grass_cost")){
					// read cost for grass
					this.grass_cost = Integer.parseInt(param.split(":")[1]);
				}
				param = br.readLine();
			}

		this.mygrid = new Cell[this.N][this.M];		
		}catch(IOException e){}
	}
	private void init(){
		Random random = new Random();

		this.start_idx = random.nextInt(this.N*this.M-1);
		int tmp = random.nextInt(this.N*this.M-1);
		while(Math.abs(this.start_idx-tmp)<45)
			tmp = random.nextInt(this.N*this.M-1);

		this.terminal_idx =tmp;

		this.walls = new int[random.nextInt(4)+14];
		int start_row = this.start_idx/this.M;
		int terminal_row = this.terminal_idx/this.M;
		int middle_row = (int) (Math.abs(start_row - terminal_row)+1)/2 + Math.min(start_row,terminal_row);
		int middle_column = random.nextInt(5);
		for(int w=0; w<4; w++)
			walls[w] = middle_row*M+middle_column+w;

		for(int w = 4; w < this.walls.length; w++ ){
			tmp= random.nextInt(this.N*this.M-1);
			while(tmp==this.start_idx || tmp==this.terminal_idx)
				tmp = random.nextInt(this.N*this.M-1);
			this.walls[w] = tmp;
		}
		this.grass = new int[random.nextInt(5)+ 37];

		for(int g = 0; g < this.grass.length; g++ ){
			tmp = random.nextInt(this.N*this.M-1);
			while(tmp==this.start_idx || tmp==this.terminal_idx)
				tmp = random.nextInt(this.N*this.M-1);
			this.grass[g] = tmp;
		}

		for (int i = 0; i<this.N; i++){
			for(int j = 0; j<this.M; j++){
				this.mygrid[i][j] = new Cell('L',((i*this.M+j)==this.start_idx), ((i*this.M+j)==this.terminal_idx),1);
			}
		}

		for (int w = 0; w < this.walls.length; w++){
			int i = this.walls[w]/M;
			int j = this.walls[w]%M;
			this.mygrid[i][j].changeCellType('W', Integer.MAX_VALUE);	
		}

		int count_g = 0;
		int [] tmp_g = new int[this.grass.length];

		for (int g = 0; g < this.grass.length; g++){
			int i = this.grass[g]/M;
			int j = this.grass[g]%M;
			if(! this.mygrid[i][j].isWall()){
				this.mygrid[i][j].changeCellType('G', this.grass_cost);
				tmp_g[count_g] = this.grass[g];
				count_g += 1;
			} 
		}
		if(count_g<this.grass.length){
			this.grass = new int[count_g-1];
			for (int g = 0; g < this.grass.length; g++)
				this.grass[g] = tmp_g[g];
		}
	}
}
