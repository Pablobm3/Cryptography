#!/usr/bin/env python

m = 2000 # number of balls
n = 0 # number of draw
probality = 1

while(probality>0.5):
	n+=1
	probality*=(m-1)/m
	
print("minimal n : "+str(n)+" ; with a probability of : "+str(1-probality))
print("approximation of minimal n (sqrt(m)) : "+str(m/2))
