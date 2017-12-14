import requests
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, get_price, get_history, get_stocklist, get_userbal, get_userstock, login_required, lookup, trade, usd

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    current_userid = session["user_id"]
    userbalance = get_userbal(db, current_userid)
    userstocks = get_userstock(db, current_userid)
    stocklist = get_stocklist(db, stocksid=True, prices=True)
    if request.method == "GET" and current_userid:
        return render_template("index.html", userbalance=usd(userbalance), userstocks=userstocks, buystocks=stocklist)
    else:
        return apology("TODO")
    


@app.route("/buy", methods=["POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        response = trade(db, session['user_id'], request.form, 'BUY')
        if response:
            if response['type'] is 'error':
                flash(response['text'], 'error')
            else:
                flash(response['text'])
            return redirect("/")
    else:
        return apology("Page not found")


@app.route("/history", methods=["GET"])
@login_required
def history():
    """Show history of transactions"""
    current_userid = session["user_id"]
    userbalance = get_userbal(db, current_userid)
    userstocks = get_userstock(db, current_userid)
    stockhistory = get_history(db, current_userid)
    stocklist = get_stocklist(db, stocksid=True, prices=True)
    if request.method == "GET":
        return render_template("history.html", userbalance=usd(userbalance), userstocks=userstocks, buystocks=stocklist, stockhistory=stockhistory)
    else:
        return apology("TODO")



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        print("Dict: {}\nMultidict: {}".format(dict(request.form), request.form))

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    current_userid = session["user_id"]
    userbalance = get_userbal(db, current_userid)
    userstocks = get_userstock(db, current_userid)
    stocklist = get_stocklist(db, stocksid=True, prices=True)
    if request.method == "POST":
        response = lookup(request.form.get("custom_stock"))
        stocklist = get_stocklist(db)
        if response:
            if response['symbol'].upper() not in stocklist:
                # Include stock in known stocks
                db.execute("INSERT INTO stocks (name) VALUES (:stockname)", stockname=response['symbol'].upper())
                # Add a column for new stock in userstock
                db.execute("ALTER TABLE userstocks ADD COLUMN :stockname INTEGER NOT NULL DEFAULT 0", stockname=response['symbol'].upper())
            flash("{} costs ${}. Click 'Buy' to purchase it.".format(response['symbol'].upper(), response['price']))
        else:
            flash("{} is not a valid stock".format(request.form.get("custom_stock")), 'error')
        return redirect("/quote")
    else:
        return render_template("quote.html", userbalance=usd(userbalance), userstocks=userstocks, buystocks=stocklist)



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        password2 = request.form.get("password2")

        # ensure username and password was submitted
        if not username or not password:
            return apology("Please enter valid username and password")

        # ensure both password are the same
        elif password != password2:
            return apology("Passwords do not match")

        # check if username already exits in database
        user_exist = db.execute("SELECT username FROM users WHERE username = :username", username=username)
        print("user_exist: {}".format(user_exist))
        if user_exist:
            return apology("User already exists")
        else:
            db.execute("INSERT INTO 'users' ('username', 'hash') VALUES (:username, :hash)", username=username, hash=generate_password_hash(password))

            # Initialize user's stock data
            user = db.execute("SELECT * FROM users WHERE username = :username", username=username)[0]
            db.execute("INSERT INTO 'userstocks' ('user_id') VALUES (:userid)", userid=user["id"])

            return render_template("success.html")
    else:
        return render_template("register.html")



@app.route("/sell", methods=["POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        response = trade(db, session['user_id'], request.form, 'SELL')
        if response:
            if response['type'] is 'error':
                flash(response['text'], 'error')
            else:
                flash(response['text'])
            return redirect("/")
    else:
        return apology("Page not found")


@app.route("/qtrade", methods=["POST"])
@login_required
def quicktrade():
    if request.method == "POST":
        print(dict(request.form))
        if request.form.get('qbuy') == '1':
            response = trade(db, session['user_id'], request.form, 'BUY')
        else:
            response = trade(db, session['user_id'], request.form, 'SELL')
            
        if response:
            if response['type'] is 'error':
                flash(response['text'], 'error')
            else:
                flash(response['text'])
            return redirect("/")
    else:
        return apology("Page not found!", 404)

def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

# Run app if this is the main script
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=True)
