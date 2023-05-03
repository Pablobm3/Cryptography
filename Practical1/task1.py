#!/usr/bin/python

import sys
from math import ceil

e = [2,4,1,5,3]

def encrypt(plaintext):
    length = len(plaintext)
    nb_iteration = ceil(length/5)
    total_nb_char = nb_iteration*5
    plaintext = plaintext.ljust(total_nb_char,'x')
    print("The plaintext is: "+str(plaintext))
    cyphertext = ""
    for i in range (0, nb_iteration):
        lower_bound = i*5
        upper_bound = (i+1)*5
        p = plaintext[lower_bound:upper_bound]
        print(p)
        c = ['x']*5
        for j in range (1, 6):
            c[e.index(j)] = p[j-1]
        print(c)
        cyphertext += "".join([str(k) for k in c])
    print("The encrypted plaintext (cyphertext) is: "+str(cyphertext))

def calculate_decryption_key():
    d = ['x']*5
    
    for i in range (1, 6):
        d[i-1] = e.index(i)+1
    print("The decryption key is: "+str(d))
    return d

def decrypt(cyphertext):
    d = calculate_decryption_key()

    length = len(cyphertext)
    nb_iteration = ceil(length/5)
    plaintext = ""
    print("The cyphertext is: "+str(cyphertext))
    for i in range (0, nb_iteration):
        lower_bound = i*5
        upper_bound = (i+1)*5
        c = cyphertext[lower_bound:upper_bound]
        print(c)
        p = ['x']*5
        for j in range (1, 6):
            p[d.index(j)] = c[j-1]
        print(p)
        plaintext += "".join([str(k) for k in p])
    print("The decrypted cyphertext (plaintext) is: "+str(plaintext))

if __name__ == '__main__':
    function = sys.argv[1]
    match function:
        case "encrypt":
            encrypt(sys.argv[2])
        case "decrypt":
            decrypt(sys.argv[2])
        case "ekey":
            print("The choosen encryption key is: "+str(e))
        case "dkey":
            calculate_decryption_key()
        case _:
            print("Error, please use the right argmuent")
