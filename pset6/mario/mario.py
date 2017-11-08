def get_int(s):
    try:
        i = int(input(s))
    except:
        i = get_int("Try again: ")
    finally:
        return i

height = get_int("Height: ")

while height < 0 or height > 23:
        print("Invalid input: Height must be a non-negative integer no greater than 23")
        height = get_int("Height: ")

temp = height + 1 # Real Height
for hash in range(1, temp):
    space = temp - (hash % temp)
    print("{0}{1}".format(" " * space, "#" * hash, " " * 2))
