import csv
import urllib.request
from functools import wraps

from cs50 import SQL
from flask import redirect, render_template, request, session


def apology(message, code=400):
    """Renders message as an apology to user."""

    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s

    return render_template("apology.html", top=code,
                           bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)

    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # reject symbol if it starts with caret
    if symbol.startswith("^"):
        return None

    # reject symbol if it contains comma
    if "," in symbol:
        return None

    # query Yahoo for quote
    # http://stackoverflow.com/a/21351911
    try:

        # GET CSV
        url = f"http://download.finance.yahoo.com/d/quotes.csv?f=snl1&s={symbol}"
        webpage = urllib.request.urlopen(url)

        # read CSV
        datareader = csv.reader(webpage.read().decode("utf-8").splitlines())

        # parse first row
        row = next(datareader)

        # ensure stock exists
        try:
            price = float(row[2])
        except:
            return None

        # return stock's name (as a str), price (as a float), and (uppercased) symbol (as a str)
        return {
            "name": row[1],
            "price": price,
            "symbol": row[0].upper()
        }

    except:
        pass

    # Query Alpha Vantage for quote instead
    # https://www.alphavantage.co/documentation/
    try:

        url = f"https://www.alphavantage.co/query?apikey=NAJXWIA8D6VN6A3K&datatype=csv&function=TIME_SERIES_INTRADAY&interval=1min&symbol={symbol}"

        webpage = urllib.request.urlopen(url)

        # parse CSV
        datareader = csv.reader(webpage.read().decode("utf-8").splitlines())

        # ignore first row
        next(datareader)

        # parse second row
        row = next(datareader)

        # ensure stock exists
        try:
            price = float(row[4])
        except:
            return None

        # return stock's name (as a str), price (as a float), and (uppercased) symbol (as a str)
        return {
            "name": symbol.upper(),  # for backward compatibility with Yahoo
            "price": price,
            "symbol": symbol.upper()
        }

    except:
        return None


def usd(value):
    """Formats value as USD."""
    return f"${value:,.2f}"


def get_userbal(database, userid, table="users"):
    """
    Gets the user cash balance
    Returns a floating point value
    """
    if not isinstance(database, SQL):
        print("DEBUG:cs50:Pass in a valid database instance")
        return None

    if not isinstance(id, int):
        print("DEBUG:cs50:ID is not an integer")

    result = database.execute("SELECT cash FROM :table WHERE id = :userid",
                              table=table, userid=userid)

    return result[0]['cash'] if result else None


def get_price(*symbols):
    """
    Gets the current stock prices for stock symbols passed in

    Returns a dictionary if multiple arguments are passed in, else a string; None on error
    """
    if len(symbols) is 1:
        result = lookup(symbols[0])
        # print(f"get_price(): ${symbols} price is ${result.get('price')}")
        return result.get('price') if result else None
    prices = {}
    for symbol in symbols:
        # Store lookup values for each symbols
        result[symbol] = lookup(symbol)
        try:
            # Extract prices for each symbol
            prices[symbol] = result[symbol]['price']
        except:
            pass

    print(f"price is ${prices}")
    return prices


def get_stocklist(database, stocksid=False, prices=False):
    """
    Gets a list of all known stocks
    
    Returns a list of stock name if prices is false else returns a list of dictionary with name and price
    """
    if not isinstance(database, SQL):
        print("DEBUG:cs50:Pass in a valid database instance")
        return None

    result = database.execute("SELECT * FROM stocks")
    stocklist = []
    for i in range(len(result)):
        values = list(result[i].values())
        # Dunno why but doesn't return values in a persistent order. So using ternary ops to check values
        stockid = values[0] if isinstance(values[0], int) else values[1]
        name = values[1] if isinstance(values[1], str) else values[0]
        # print("id: {}, name: {}".format(stockid, name))
        price = get_price(name)
        if price:
            price = usd(price)
        if prices or stocksid:
            stocklist.append({'id': stockid, 'name': name, 'price': price})
        else:
            stocklist.append(name)

    return stocklist


def get_stockid(database, symbol):
    """
    Returns a stocks ID
    """
    if not isinstance(database, SQL):
        print("DEBUG:cs50:Pass in a valid database instance")
        return None

    result = database.execute("SELECT id FROM stocks WHERE name = :symbol",
                              symbol=symbol)

    return result[0]['id'] if result else None


def get_stockname(database, stockid):
    """
    Returns a stocks ID
    """
    if not isinstance(database, SQL):
        print("DEBUG:cs50:Pass in a valid database instance")
        return None

    result = database.execute("SELECT name FROM stocks WHERE id = :stockid",
                              stockid=stockid)

    return result[0]['name'] if result else None


def get_userstock(database, userid, table="userstocks"):
    """
    Gets the user's stocks

    Returns a list of dictionary with user's stock amount and price
    ex. [{'name': symbol, 'quantity': quantity, 'price': price}]
    """
    if not isinstance(database, SQL):
        print("DEBUG:cs50:Pass in a valid database instance")
        return None

    if not isinstance(id, int):
        print("DEBUG:cs50:ID is not an integer")

    result = database.execute("SELECT * FROM :table WHERE user_id = :userid",
                              table=table, userid=userid)

    if result:
        # Extract from list
        result = result[0]
        # Remove user_id (not needed in stocks)
        del result['user_id']

        stocks = []
        # Add current stock prices
        for stock in result:
            # Extract symbol which is key to stock amount
            symbol = stock
            quantity = int(result[symbol])
            price = get_price(symbol)
            if price:
                price = usd(price)
            stockid = get_stockid(database, symbol)
            if quantity:
                stocks.append(
                    {'name': symbol, 'quantity': quantity, 'price': price,
                     'id': stockid})
        return stocks
    return None


def get_history(database, userid):
    """
    Gets a user's transaction history

    """
    if not isinstance(database, SQL):
        print("DEBUG:cs50:Pass in a valid database instance")
        return None

    result = database.execute(
        "SELECT trans.id, trans.stock_id, trans.action, trans.price, trans.shares, trans.time, stocks.name FROM transactions AS trans JOIN stocks ON trans.stock_id = stocks.id WHERE user_id = :userid ORDER BY trans.id DESC",
        userid=userid)

    return result if result else None


def trade(database, userid, requestform, action):
    """
    Executes a trade action (i.e. BUY or SELL)
    Returns a dict object
    database: The database object to query
    userid: Current user's ID
    requestform: A MultiDict with the parsed form data from POST request ( eg. flask.request.form)
    action: A string which is either BUY or SELL
    """
    if not isinstance(database, SQL):
        print("DEBUG:cs50:Pass in a valid database instance")
        return None

    # Get current balance
    balance = float("{:.2f}".format(get_userbal(database, userid)))

    if action.upper() == 'FUND':
        balance = balance + 100
        sitebuilder = request.form.get('buildername').upper()
        siteauthor = ['CHUMA UMENZE', 'CHUMAUMENZE', 'CHIMA', 'CHUMA',
                      'UMENZE', 'CHIMA UMENZE']

        # Check answer
        if sitebuilder in siteauthor:
            # Update balance
            database.execute(
                "UPDATE users SET cash = :balance WHERE id = :userid",
                balance=balance, userid=userid)

            # Log transaction
            database.execute(
                "INSERT INTO 'transactions' ('user_id','action','price') VALUES (:userid,:action,:price)",
                userid=userid, action="FUND", price=100)

            return dict(type='success',
                        text='You just funded your account with $100.00')
        else:
            return None

    requestform = dict(requestform)

    # Quick trade option
    if 'qbuy' in requestform:
        del requestform['qbuy']
    elif 'qsell' in requestform:
        del requestform['qsell']

    # stocks = get_userstock(database, userid)
    stocks = {}
    costs = []

    for stockid in requestform:  # key value in requestform are stockids
        stockname = get_stockname(database, stockid)
        if not stockname:
            return dict(type='error', text='Invalid request')
        stockquantity = requestform[stockid][0]
        # Dictionary with stock name = quantity
        stocks[stockname] = stockquantity

        # Add stock prices to list to match total with userbalance
        if stockquantity:
            if int(stockquantity) > 0:
                costs.append(get_price(stockname) * float(stockquantity))
            elif int(stockquantity) < 0:
                return dict(type='error',
                            text='Shares cannot be negative value')
    # Total amount of order
    cost = sum(costs)

    # Debugging
    print("Balance: {}, Cost: {}".format(balance, cost))
    print("DEBUG:cs50:trade(): {}".format(stocks))

    if action.upper() == 'BUY':

        if not cost:
            return dict(type='error',
                        text='Please add the amount of Shares to purchase')
        elif balance >= cost:
            # Update balance
            balance = float(balance - cost)
            database.execute(
                "UPDATE users SET cash = :balance WHERE id = :userid",
                balance=balance, userid=userid)

            for stock in stocks:
                if stocks[stock]:
                    # Update user's stocks
                    database.execute(
                        "UPDATE userstocks SET {0} = {0} + {1} WHERE user_id = :userid".format(
                            stock, stocks[stock]), userid=userid)

                    # Log transaction
                    database.execute(
                        "INSERT INTO 'transactions' ('user_id','stock_id','action','price','shares') VALUES (:userid,:stockid,:action,:price,:shares)",
                        userid=userid, stockid=get_stockid(database, stock),
                        action="BUY", price=get_price(stock),
                        shares=stocks[stock])
            return dict(type='success', text='Stocks Purchased',
                        balance=balance)
        else:
            return dict(type='error',
                        text='You balance is insuffient. Reduce the amount of stocks to purchase')

    elif action.upper() == 'SELL':
        if not cost:
            return dict(type='error',
                        text='Please add the amount of Shares to sell')
        else:
            # Security check - We don't want users selling stocks they don't own
            # Using stocks of users from database
            userstocks = get_userstock(database, userid)
            if not userstocks:
                return dict(type='error', text='You do not own any stocks yet')
            else:
                # Check the amount of share owned
                for i in range(len(userstocks)):
                    if i == len(stocks):
                        break
                    print(
                        f"line:379:sell():userstocks: {userstocks}, share: {stocks.get(userstocks[0]['name'])}")
                    share = stocks.get(userstocks[i]['name'])
                    if share:
                        if int(share) > userstocks[i]['quantity']:
                            return dict(type='error',
                                        text='Quantity of shares to sell exceeds the amount owned')

                # Update balance
                balance = float(balance + cost)
                database.execute(
                    "UPDATE users SET cash = :balance WHERE id = :userid",
                    balance=balance, userid=userid)
                # Sell shares
                for stock in stocks:
                    if stocks[stock]:
                        # Update user's stocks
                        database.execute(
                            "UPDATE userstocks SET {0} = {0} - {1} WHERE user_id = :userid".format(
                                stock, stocks[stock]), userid=userid)

                        # Log transaction
                        database.execute(
                            "INSERT INTO 'transactions' ('user_id','stock_id','action','price','shares') VALUES (:userid,:stockid,:action,:price,:shares)",
                            userid=userid,
                            stockid=get_stockid(database, stock),
                            action="SELL", price=get_price(stock),
                            shares=stocks[stock])
                return dict(type='success', text='Stocks Sold',
                            balance=balance)
    else:
        return None
