from cs50 import SQL

# Connect to database
db = SQL("sqlite:////var/www/html/phpliteadmin/finance.db")

# Create required tables
try:
    # Create Users table (Already created by default in finance.db)
    db.execute(
        "CREATE TABLE 'users' ('id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'username' TEXT NOT NULL, 'hash' TEXT NOT NULL, 'cash' NUMERIC NOT NULL DEFAULT 10000.00 )")
except:
    pass

try:
    # Create Users' Stock table
    db.execute(
        "CREATE TABLE 'userstocks' ('user_id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'GOOG' INTEGER NOT NULL DEFAULT 0, 'JPM' INTEGER NOT NULL DEFAULT 0, 'MSFT' INTEGER NOT NULL DEFAULT 0, 'NFLX' INTEGER NOT NULL DEFAULT 0)")
except:
    pass

try:
    # Create Transactions table
    db.execute(
        "CREATE TABLE 'transactions' ('id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'user_id' INTEGER NOT NULL, 'stock_id' INTEGER NOT NULL, 'action' TEXT NOT NULL, 'price' NUMERIC NOT NULL, 'shares' INTEGER NOT NULL, 'time' DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP)")
except:
    pass

try:
    # Create Stock ID table
    db.execute(
        "CREATE TABLE 'stocks' ('id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'name' TEXT NOT NULL)")
    # Add already known stocks
    db.execute(
        "INSERT INTO 'stocks' ('name') VALUES ('GOOG'), ('JPM'), ('MSFT'), ('NFLX')")
    db.execute("CREATE UNIQUE INDEX 'stocks_index' ON 'stocks'(name)")
except:
    pass

''' 
try:
    # Create Trade Action table
    db.execute("CREATE TABLE 'actions' ('id' INTEGER PRIMARY KEY NOT NULL, 'name' TEXT NOT NULL)")
    # Add Trade options
    db.execute("INSERT INTO 'actions' ('name') VALUES ('BUY'), ('SELL')")
except:
    pass
 '''
