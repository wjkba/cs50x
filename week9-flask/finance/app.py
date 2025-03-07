import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    userId = session["user_id"]
    userCash = db.execute("SELECT cash FROM users WHERE id = ?",userId)[0]["cash"]
    portfolio = db.execute("SELECT symbol, shares FROM user_holdings WHERE user_id = ? GROUP BY symbol", userId)

    grandTotal = userCash

    for stock in portfolio:
        quoteData = lookup(stock["symbol"])
        stock["price"] = quoteData["price"]
        grandTotal += stock["shares"] * stock["price"]
        stock["total"] = usd(stock["shares"] * stock["price"])
        stock["price"] = usd(quoteData["price"])



    return render_template("index.html", cash=usd(userCash), portfolio=portfolio, grandTotal=usd(grandTotal))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Invalid input")

        if not symbol:
            return apology("Please provide symbol")

        quoteData = lookup(symbol)
        if not quoteData:
            return apology("Invalid symbol")

        if shares < 1:
            return apology("Please provide positive integer")

        cart = quoteData["price"]*shares
        userId = session["user_id"]
        userCash = db.execute("SELECT cash FROM users WHERE id = ?",userId)[0]["cash"]
        if cart > userCash:
            return apology("Cannot afford")

        newBalance = userCash - cart
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newBalance, userId)

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, type) VALUES (?, ?, ?, ?, ?)", userId, symbol, shares, quoteData["price"], "BUY")

        hasStocks = db.execute("SELECT shares FROM user_holdings WHERE user_id = ? AND symbol = ?", userId, symbol)

        if hasStocks:
            currentShares = hasStocks[0]["shares"]
            updatedShares = currentShares + shares
            db.execute("UPDATE user_holdings SET shares = ? WHERE user_id = ? AND symbol = ?", updatedShares, userId, symbol)
        else:
            db.execute("INSERT INTO user_holdings (user_id, symbol, shares) VALUES (?, ?, ?)", userId, symbol, shares)


        return redirect("/")
    else:
        return render_template("/buy.html")


@app.route("/history")
@login_required
def history():
    userId = session["user_id"]
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY created_at DESC", userId)
    print(transactions)
    for transaction in transactions:
        transaction["price"] = usd(transaction["price"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("Please provide symbol")

        quoteData = lookup(symbol)
        if not quoteData:
            return apology("Invalid symbol")

        quoteData["price"] = usd(quoteData["price"])

        return render_template("/quoteInfo.html", quoteData=quoteData)

    else:
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")


        if not username:
            return apology("Please provide username")
        if not password or not confirmation:
            return apology("Please provide password")
        if password != confirmation:
            return apology("Passwords dont match")

        hashedPassword = generate_password_hash(password)

        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashedPassword)
        except ValueError:
            return apology("Username already exists")


        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    userId = session["user_id"]
    if request.method == "POST":
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Invalid input")

        if not symbol:
            return apology("Please provide stock")
        if not shares or shares < 1:
            return apology("Invalid shares input")

        hasStocks = db.execute("SELECT shares FROM user_holdings WHERE user_id = ? AND symbol = ?", userId, symbol)

        if not hasStocks:
            return apology("No stock")

        currentShares = hasStocks[0]["shares"]

        if shares > currentShares:
            return apology("Not enough shares")

        quoteData = lookup(symbol)
        if not quoteData:
            return apology("Invalid symbol")

        saleValue = quoteData["price"] * shares

        userCash = db.execute("SELECT cash FROM users WHERE id = ?", userId)[0]["cash"]
        newBalance = userCash + saleValue
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newBalance, userId)

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, type) VALUES (?, ?, ?, ?, ?)",
                  userId, symbol, -shares, quoteData["price"], "SELL")

        updatedShares = currentShares - shares

        if updatedShares > 0:
            db.execute("UPDATE user_holdings SET shares = ? WHERE user_id = ? AND symbol = ?",
                      updatedShares, userId, symbol)
        else:
            db.execute("DELETE FROM user_holdings WHERE user_id = ? AND symbol = ?",
                      userId, symbol)

        return redirect("/")
    else:
        stocks = db.execute("SELECT symbol FROM user_holdings WHERE user_id = ?", userId)
        return render_template("sell.html", stocks=stocks)


@app.route("/settings")
@login_required
def settings():
    return render_template("settings.html")

@app.route("/change-password", methods=["POST"])
@login_required
def changePassoword():
    userId = session["user_id"]
    oldPassword = request.form.get("oldPassword")
    newPassword = request.form.get("newPassword")
    hashedPassword = db.execute("SELECT hash FROM users WHERE id = ?", userId)[0]["hash"]

    if not oldPassword or not newPassword:
        return apology("Please provide password")

    oldPasswordMatches = check_password_hash(hashedPassword, oldPassword)

    if not oldPasswordMatches:
        return apology("Old password is incorrect")

    newHash = generate_password_hash(password=newPassword)
    db.execute("UPDATE users SET hash = ? WHERE id = ?", newHash, userId)

    return redirect("/logout")

@app.route("/add-cash", methods=["GET", "POST"])
@login_required
def addCash():
    if request.method == "POST":
        try:
            amount = int(request.form.get("amount"))
        except ValueError:
            return apology("Invalid amount")

        paymentMethod = request.form.get("payment-method")
        cardNumber = request.form.get("card-number")
        expirationDate = request.form.get("expiration-date")
        cvv = request.form.get("cvv")

        if amount <= 0:
            return apology("Invalid amount")

        if not paymentMethod or paymentMethod not in ["credit-card", "debit-card", "paypal"]:
            return apology("Invalid payment method")

        if not cardNumber or len(cardNumber.replace(" ", "")) < 13 or len(cardNumber.replace(" ", "")) > 19:
            return apology("Card number must be between 13 and 19 digits")

        if not expirationDate:
            return apology("Please provide expiration date")

        if not cvv or len(cvv) != 3:
            return apology("Invalid CVV")

        userId = session["user_id"]
        currentCash = db.execute("SELECT cash FROM users WHERE id = ?", userId)[0]["cash"]
        newBalance = currentCash + amount

        db.execute("UPDATE users SET cash = ? WHERE id = ?", newBalance, userId)
        return redirect("/")
    else:
        return render_template("add-cash.html")
