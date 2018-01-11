import os
import re
from flask import Flask, jsonify, render_template, request

from cs50 import SQL
from helpers import lookup

# add API key
if not os.environ.get("API_KEY"):
    os.environ["API_KEY"] = input("Enter Google Maps API: ")

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
def index():
    """Render map"""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))


@app.route("/articles")
def articles():
    """Look up articles for geo"""

    if request.args.get("geo"):
        place = lookup(request.args.get("geo"))
    return jsonify(place)


@app.route("/search")
def search():
    """Search for places that match query"""

    user_query = request.args.get("q")
    
    if user_query.isdigit():
    # search by postal code
        result = db.execute("SELECT * FROM places WHERE places MATCH :query", query=(f"postal_code: {user_query}*"))
    else:
        # seperate city and state
        tokens = user_query.replace(", ", " ")
        result = db.execute("SELECT * FROM places WHERE places MATCH :query", query=(user_query + "*"))
        
    return jsonify(result if result else None)


@app.route("/update")
def update():
    """Find up to 10 places within view"""

    # Ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # Ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # Explode southwest corner into two variables
    sw_lat, sw_lng = map(float, request.args.get("sw").split(","))

    # Explode northeast corner into two variables
    ne_lat, ne_lng = map(float, request.args.get("ne").split(","))

    # Find 10 cities within view, pseudorandomly chosen if more within view
    if sw_lng <= ne_lng:

        # Doesn't cross the antimeridian
        # rows = db.execute("""SELECT * FROM places
        #                   WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
        #                   GROUP BY country_code, place_name, admin_code1
        #                   ORDER BY RANDOM()
        #                   LIMIT 10""",
        #                   sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat)

    else:

        # Crosses the antimeridian
        # rows = db.execute("""SELECT * FROM places
        #                   WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
        #                   GROUP BY country_code, place_name, admin_code1
        #                   ORDER BY RANDOM()
        #                   LIMIT 10""",
        #                   sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat)

    # Output places as JSON
    return jsonify(rows)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=True)
