import selection as sl
from numpy import random

#mutation routine
def error_prbability( dna, prob_err ):	

	if ( random.randint(1,100) <= prob_err ):
		
		n_error = random.randint( 0,(len(dna)-1) )

		if ( dna[n_error] == '0' ):
			dna = dna[:n_error] + '1' + dna[n_error+1:]

		elif ( dna[n_error] == '1' ):
			dna = dna[:n_error] + '0' + dna[n_error+1:]

	return dna 



def genetic_algorithm( DNA, prob_err ):

	winners = sl.selection(DNA)
	dna1 = ''
	dna2 = ''


	for x in winners[0][:-1]:		  						
		#converts the elements in binary and put them in a string
		dna1 += bin(x)[2:].zfill(8)	  


	for y in winners[1][:-1]:		  

		dna2 += bin(y)[2:].zfill(8)


	N_max = len(dna1) - 1	 #maximum cut

	cut = random.randint(1,N_max)

	#define the output
	dna1_f = dna1[:cut] + dna2[cut:]		
	dna2_f = dna2[:cut] + dna1[cut:]
	
	#mutation process
	dna1_f = error_prbability( dna1_f, prob_err)
	dna2_f = error_prbability( dna2_f, prob_err)

	
	#number of integers in the list
	N = int(len(dna1_f)/8)			
	new_dna1 = []
	new_dna2 = []
	l_b = 0

	#converts the string in a list of integers
	for i in range(N):
		new_dna1.append( int(dna1_f[l_b:8+i*8],2 ) )
		new_dna2.append( int(dna2_f[l_b:8+i*8],2 ) )
		l_b = 8+i*8


	new_dnas = [ new_dna1, new_dna2 ]


	return new_dnas
	

	

