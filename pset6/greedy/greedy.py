def get_float(s):
    try:
        f = float(input(s))
    except:
        f = get_float("Try again: ")
    finally:
        return f

def mult(amt, denom):
    temp = amt - (amt % denom)
    return temp // denom

change = round(get_float("O hai! How much change is owed?\n"), 2)
while change < 0:
    change = round(get_float("Try again: "), 2)

coins = 0
cents = int(change * 100)
denoms = {"quarter":25, "dime":10, "nickel":5, "penny":1}

while not cents == 0:
    if cents >= denoms["quarter"]:
        coins += mult(cents, denoms["quarter"])
        cents = cents % denoms["quarter"]
    elif cents >= denoms["dime"]:
        coins += mult(cents, denoms["dime"])
        cents = cents % denoms["dime"]
    elif cents >= denoms["nickel"]:
        coins += mult(cents, denoms["nickel"])
        cents = cents % denoms["nickel"]
    elif cents >= denoms["penny"]:
        coins += mult(cents, denoms["penny"])
        cents = cents % denoms["penny"]

print(coins)
