from numpy import random

def selection ( DNAs ):

	DNA = DNAs[:]
	probability_bar = 0
	
	for dna in DNA:
		probability_bar += dna[-1]  
		#the last element is the fitness
		



	x = probability_bar*random.random()
	winner1 = []
	lower_bound = 0
	j = 0								   

	#check in which interval x falls
	for dna in DNA:

		if( x <= dna[-1] + lower_bound ):	
			winner1 = dna
			probability_bar -= dna[-1]  #update the segment
			del DNA[j]											
			break

		lower_bound += dna[-1]  #update the lower bound
		j += 1

	#same routine for the second one
	y = probability_bar*random.random()
	winner2 = []
	lower_bound = 0
	j = 0

	for dna in DNA:

		if( y <= dna[-1] + lower_bound ):
			winner2 = dna
			break

		lower_bound += dna[-1]				
		j += 1


	winners = [ winner1, winner2 ]

	return winners
		





