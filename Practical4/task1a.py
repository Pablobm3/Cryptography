#!/usr/bin/env python

m = 2000
n = 0
results = 1

while(results>0.5):
	n+=1;
	results*=(m-n)/m
	
print("minimal n : "+str(n)+" ; with a probability of : "+str(1-results))
print("approximation of minimal n (sqrt(m)) : "+str(m**(1/2)))
