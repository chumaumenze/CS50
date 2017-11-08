import sys

def main():
    if len(sys.argv) == 2 and sys.argv[1].isdigit():
        cipherkey = int(sys.argv[1])
        plaintext = str(input("plaintext: "))
        print("ciphertext: {}".format(encipher(plaintext, cipherkey)))
    else:
        print("Usage: python3 caesar.py k")
        return


def encipher(text, key):
    firstchar = ord('A')
    lastchar = ord('Z') + 1
    
    cipher = ""
    for char in text:
        temp = ord(char.upper()) + int(key)
        while temp > ord('Z'):
            temp = (temp - lastchar) + firstchar

        if char.isupper():
            temp = chr(temp).upper()
        elif char.islower():
            temp = chr(temp).lower()
        else:
            temp = char

        cipher += temp
    return cipher


main()
