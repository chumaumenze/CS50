import sys

def main():
    if len(sys.argv) == 2 and sys.argv[1].isalpha():
        cipherkey = str(sys.argv[1])
        plaintext = str(input("plaintext: "))
        print("ciphertext: {}".format(encipher(plaintext, cipherkey)))
    else:
        print("Usage: python3 vigenere.py k")
        return


def encipher(text, key):
    firstchar = ord('A')
    lastchar = ord('Z') + 1
    
    cipher = ""
    j = 0
    for char in text:
        temp = ord(char.upper()) + (ord(key[j % len(key)].upper()) - firstchar)
        
        while temp > ord('Z'):
            temp = (temp - lastchar) + firstchar

        if char.isupper():
            temp = chr(temp).upper()
        elif char.islower():
            temp = chr(temp).lower()
        else:
            temp = char
            j -= 1

        cipher += temp
        j += 1

    return cipher


main()
