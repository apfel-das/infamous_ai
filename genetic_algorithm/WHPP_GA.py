import numpy as np
import random

def create_pop(population_size,days,employees):
	
	"""
	Step 2:
	Creates random population.
	randint returns integers from LOW to HIGH (0,1,2,3)
	>>> np.random.randint(0,5,size=(2,4))
	array([[4,0,2,1],
		   [3,2,2,0]])
	"""
	pop = np.random.randint(0,4,(population_size,days,employees))
	return pop


def feasibility(pop,population_size,days,employees):
	"""
	Step 3:
	This function checks for the HARD CONSTRAINTS
	of the problem. As you can see in pg 15:
	
		----------------------------------
		-|A/A|MON|TUE|WED|THU|FRI|SAT|SUN|-
		-|MOR|10 |10 | 5 | 5 | 5 | 5 | 5 |-
		-|AFT|10 |10 |10 | 5 |10 | 5 | 5 |- 
		-|NGH| 5 | 5 | 5 | 5 | 5 | 5 | 5 |-
		----------------------------------- 
	"""
	hcons = np.zeros(3,14) #3 shifts, 2 weeks(14days)
	for i in range(3):
		for j in range(14):
			if (i==0 and j==0) or (i==1 and j==0) or (i==0 and j==1) or
				(i==1 and j==1) or (i==1 and j==2) or (i==1 and j==4):
				hcons[i][j] = 10
			else:
				h[i][j] = 5



def check_fitness(pop,population_size,days,employees):
	"""

	"""
