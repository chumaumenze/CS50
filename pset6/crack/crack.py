import crypt
import itertools
import sys


#characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"
characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" # CS50 charset


def main():
    if len(sys.argv) == 2:
        argv = sys.argv[1]
    else:
        print("Usage: python3 crack.py hash")
        return
    salt = argv[:2]
    wordlist = create_wordlist(characters, 4)
    
    for word in wordlist:
        if crypt.crypt(word, salt) == argv:
            print(word)
            return word


def create_wordlist(charset, wordlen):
    if str(wordlen).isdigit() != True:
        return None
    
    words = []
    for i in range(1, wordlen + 1):
        words += list(map("".join, itertools.product(str(charset), repeat=i)))
    return words


main()
