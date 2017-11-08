def checkcc(nums):
    
    def cctype(cc):
        cc = str(cc)
        cc_range = {"visa": [13, 16], "mc": [16], "amex":[15]}
        digits = len(str(nums))
        start = int(cc[:2])
        if int(cc[:1]) == 4 and digits in cc_range["visa"]:
            return "VISA"
        elif start in [34, 37] and digits in cc_range["amex"]:
            return "AMEX"
        elif start in list(range(51, 56)) and digits in cc_range["mc"]:
            return "MASTERCARD"
        else:
            return None
    
    def luhn(cc):
        cc = str(cc)[::-1] # Reverse string
        seclast_digits = list(map(lambda x: int(x) * 2, cc[1::2])) # Extract all second digits to a list multiplied by 2
        rem_digits = sum(list(map(lambda x: int(x), cc[::2]))) # Sum up remaining digits
        
        for i in range(len(seclast_digits)):
            if seclast_digits[i] > 9:
                seclast_digits[i] -= 9
        seclast_digits = sum(seclast_digits)
        if (seclast_digits + rem_digits) % 10 == 0:
            return True
        else:
            return False
    
    cardtype = cctype(nums)
    
    if cardtype is not None and luhn(nums) == True:
        return cardtype
    else:
        return "INVALID"

def get_int(s, pos):
    try:
        i = int(input(s))
    except:
        i = get_int("Try again: ", pos)
    finally:
        if pos == True and i < 0:
            i = get_int("Try again: ", pos)
        return i

def main():
    ccdigits = get_int("Number: ", True)
    print(checkcc(ccdigits))


main()
