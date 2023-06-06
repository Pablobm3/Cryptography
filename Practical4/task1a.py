#!/usr/bin/env python

m = 2000 # number of balls
n = 0 # number of draw
probability = 1

while(probability>0.5):
	n+=1
	probability*=(m-n)/m
	
print("minimal n : "+str(n)+" ; with a probability of : "+str(1-probability))
print("approximation of minimal n (sqrt(m)) : "+str(m**(1/2)))