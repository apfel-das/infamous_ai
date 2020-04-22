import numpy as np
import random,math

"""
Project_name: WHHP genetic algorithm
Subject:	  Artificial Intelligence
Engineer:	  Christos Trimas
"""


def create_pop(population_size,days,employees):
	
	"""
	Step 2:
	Creates random population.
	randint returns integers from LOW to HIGH (0,1,2,3)
	>>> np.random.randint(0,5,size=(2,4))
	array([[4,0,2,1],
		   [3,2,2,0]])
	"""
	pop = np.random.randint(0,4,size=(population_size,employees,days))
	return pop


def feasibility(pop,population_size,days,employees):
	"""
	Step 3:
	This function checks for the HARD CONSTRAINTS
	of the problem. As you can see in pg 15:
	
		----------------------------------
		-|A/A|MON|TUE|WED|THU|FRI|SAT|SUN|-
		-| 1 |10 |10 | 5 | 5 | 5 | 5 | 5 |-
		-| 2 |10 |10 |10 | 5 |10 | 5 | 5 |- 
		-| 3 | 5 | 5 | 5 | 5 | 5 | 5 | 5 |-
		----------------------------------- 

		where 1 is Morning Shift
			  2 is Afternoon Shift
			  3 is Night Shift
	We take the above matrix 2 times since we have 2 weeks
	"""
	hcons = np.zeros((3,14)) #3 shifts, 2 weeks(14days)
	for i in range(3):
		for j in range(14):
			if (i==0 and j==0) or (i==1 and j==0) or (i==0 and j==1) or \
				(i==1 and j==1) or (i==1 and j==2) or (i==1 and j==4) \
				or (i==0 and j==7) or (i==1 and j==7) or (i==0 and j==8) or \
				(i==1 and j==8) or (i==1 and j==9) or (i==0 and j==11):
				hcons[i][j] = 10
			else:
				hcons[i][j] = 5

	#init
	feas_check = np.zeros(population_size)
	mshift = 0
	ashift = 0
	nshift = 0

	#Num of employee shifts per day
	for i in range(population_size):
		for j in range(days):
			for k in range(employees):
				if pop[i,k,j]==1:
					mshift += 1
				elif pop[i,k,j]==2:
					ashift += 1
				elif pop[i,k,j]==3:
					nshift += 1
			if hcons[0,j]==mshift and hcons[1,j]==ashift and hcons[2,j]==nshift:
				feas_check[i] = 1
			else:
				mshift = 0
				ashift = 0
				nshift = 0
				break
	return feas_check


def check_fitness(pop,population_size,days,employees):
	"""
	Soft Constraints:
	1) Max 70 Hours(1000)
	2) Max 7 straight days with no day off(1000)
	3) Max 4 straight night shifts(1000)
	4) No morning shift after night shift(1000)
	5) No morning shift after afternoon shift(800)
	6) No afternoon shift after night shift(800)
	7) At least 2 days day off after 4 straight night shifts(100)
	8) At least 2 days off after 7 days shift(100)
	9) No shift-day off-shift schedule(1)
	10) No day off-shift-day off schedule(1)
	11) At least one shift on weekends of those two weeks(1)
	"""
	penalty = np.zeros(len(population_size))
	hours = 0
	n_days_off = 0
	d_days_off = 0

	for i in range(len(population_size)):
		penalty[i] = 0
		for j in range(employees):
			if hours >= 70: 			#1
				penalty[i] += 1000
			if n_days_off < 2: 			#7
				penalty[i] += 100
			if d_days_off < 2:			#8
				penalty[i] += 100
			hours = 0
			cons_d = 0
			cons_n = 0
			nights = 0
			n_days_off = 0
			d_days_off = 0
			workdays = 0
			d_flag = 0
			a_flag = 0
			n_flag = 0
			day_off_flag = 0
			for k in range(days):
				#morning shift restrictions
				if pop[i,j,k] == 1:
					hours += 8
					cons_d += 1
					cons_n = 0
					if n_flag == 1:
						penalty[i] += 1000 #4
						n_flag = 0
					if a_flag ==1:
						penalty[i] += 800	#5
						a_flag = 0
					workdays += 1
					if (cons_d ==1 and day_off_flag == 1):
						penalty[i] += 1		#9
						day_off_flag = 0
				#afternoon shift restrictions
				elif pop[i,j,k] == 2:
					hours += 8
					cons_d += 1
					cons_n = 0
					if n_flag == 1:
						penalty[i] += 800	#6
						n_flag = 0
					a_flag = 1
					workdays += 1
					if cons_d ==1 and day_off_flag == 1:
						penalty[i] += 1 #9
						day_off_flag = 0
				#night shift restrictions
				elif pop[i,j,k] == 3:
					hours += 10
					cons_d += 1
					cons_n += 1
					night_flag = 1
					nights += 1
					workdays += 1
					if cons_d == 1 and day_off_flag == 1:
						penalty[i] += 1		#10
						day_off_flag = 0
				#day off
				else:
					if n_flag == 1:
						n_flag = 0
					if a_flag == 1:
						a_flag = 0
					if nights >= 4:
						n_days_off += 1
					if workdays >= 7:
						d_days_off += 1
					if cons_d == 1:
						if day_off_flag == 0:
							penalty[i] += 1 	#9
						cons_d = 0
						day_off_flag = 0
					else:
						cons_d = 0
				if cons_d > 7:
					penalty[i] += 1000			#2
					cons_d = 0
					cons_n = 0
				if cons_n > 4:
					penalty[i] += 1000			#3
					cons_n = 0
				if k == 13:
					if pop[i,j,k] != 0 and pop[i,j,k-1] != 0:
						if pop[i,j,k-7] != 0 and pop[i,j,k-8] != 0:
							penalty[i] += 1		#11
	return penalty

def selection(passsed_chromosomes):
	J = list()
	tmp = 0
	for i in range(1,len(passsed_chromosomes)+1):
			t = np.random.randint(1,len(passsed_chromosomes)+1)
			# print('t =',t)

			if t >= 2:
				if passsed_chromosomes[0] > passsed_chromosomes[1]:
					tmp = passsed_chromosomes[0]
				else:
					tmp = passsed_chromosomes[1]

				for j in range(t):
					if (j+1)<t and passsed_chromosomes[j] > passsed_chromosomes[j+1]:
						# print('Tournament: 1)',passsed_chromosomes[j],j)
						# print('Tournament: 2)',passsed_chromosomes[j+1],(j+1))
						tmp = passsed_chromosomes[j]

					elif (j+1)<t and passsed_chromosomes[j] < passsed_chromosomes[j+1]:
						# print('Tournament: 1)',passsed_chromosomes[j],j)
						# print('Tournament: 2)',passsed_chromosomes[j+1],(j+1))
						tmp = passsed_chromosomes[j+1]

				J.append(tmp)

			if t==1:
				# print('Tournament: 1)',passsed_chromosomes[i-1],i-1)
				J.append(passsed_chromosomes[i-1])
				
	return max(J)


def crossover_1(parent_1,parent_2,par1,par2,days):
	if parent_1 is not None and parent_2 is not None:
		random_cross_point = np.random.randint(1,days-1)
		child = np.hstack((par1[:, 0:random_cross_point], par2[:, random_cross_point:]))
		return child
	else:
		return None

# def crossover_2(parent_1,parent2,par1,par2,days):
# 	if parent_1 is not None and parent_2 is not None:
# 		random_cross_point = list()
# 		points = np.random.randint(2,10) #num of crossover points
# 		for i in range(points):
# 			random_cross_point.append(np.random.randint(1,days-1))
# 			random_cross_point.sort()
# 		first = random_cross_point[0]
# 		for i in range(points):
# 			if i+1 < points:
# 				child = np.hstack((par1[:,i:first],par2[:,first:random_cross_point[i+1]]))
# 				first = random_cross_point[i+1]
# 		return child
# 	else:
# 		None


def crossover_2(parent_1,parent2,par1,par2,days):
	if parent_1 is not None and parent_2 is not None:
		random_cross_point = list()
		for i in range(5):
			random_cross_point.append(np.random.randint(1,days-1))
			random_cross_point.sort()
			
		child = np.hstack((par1[:,0:random_cross_point[0]],par2[:,random_cross_point[0]:random_cross_point[1]],
							par1[:,random_cross_point[1]:random_cross_point[2]],par2[:,random_cross_point[2]:random_cross_point[3]],
							par1[:,random_cross_point[3]:random_cross_point[4]],par2[:,random_cross_point[4]:]))

		
		return child
	else:
		None



days = 14
employees = 30
population_size = 3000
iteration = 10 #wiil see about the criteria of ending

pop = create_pop(population_size,days,employees)
check = feasibility(pop,population_size,days,employees)

# Find the postion of the passing chromosomes
# and put them on a list

passsed_chromosomes = list()
check_matrix = list()
for i in range(population_size):
	if check[i]==1:
		print(check[i],i)
		check_matrix.append(check[i])
		passsed_chromosomes.append(i)
print("Starting Generation")
print('\nNumber of passed chromosomes: ', len(passsed_chromosomes))
print('\nChromosomes: ',passsed_chromosomes)
# print('\nCheck matrix: ',check_matrix)

penalty_matrix = check_fitness(pop,check_matrix,days,employees)
# print(penalty_matrix)


highest = np.max(penalty_matrix)
average = np.average(penalty_matrix)

highest_matrix = list()
highest_matrix.append(int(highest))
average_matrix = list()
average_matrix.append(int(average))
# print(average_matrix)
# print(highest_matrix)


"""
We implement now the basic idea of the GA
Check page 14 of the  exercise for more information 
or the report.
"""

Pr_selection = 0.05
Pr_crossover = 0.15
Pr_mutation = 0.15
for i in range(iteration):
	print(f'\nGeneration: {i+1}')
	new_pop = list()

	for j in range(int(population_size/2)):
		Pselection = np.random.random()
		Pcrossover = np.random.random()
		Pmutation = np.random.random()

		if Pselection > Pr_selection:
			parent_1 = selection(passsed_chromosomes)
			parent_2 = selection(passsed_chromosomes)
			# print('parent1',pop[parent_1])
			# print('parent2',pop[parent_2])
			par1 = pop[parent_1]
			par2 = pop[parent_2]
			while parent_1==parent_2 and len(passsed_chromosomes)>2:
				parent_1 = selection(passsed_chromosomes)
			if Pcrossover > Pr_crossover and Pmutation < Pr_mutation:
				child = crossover_2(parent_1,parent_2,par1,par2,days)
				print(child,len(child))
				new_pop.append(child)


# print(new_pop)


