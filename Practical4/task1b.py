#!/usr/bin/env python

m = 2000 # number of balls
n = 1 # number of draw
p2_prime = 1 #complement probability

while(p2_prime>0.5):
	p2_prime*=(m-1)/m
	n+=1
	
print("minimal n : "+str(n)+" ; with a probability of : "+str(1-p2_prime))
print("approximation of minimal n (m/2) : "+str(m/2))
