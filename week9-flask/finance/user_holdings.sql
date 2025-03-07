CREATE TABLE IF NOT EXISTS user_holdings (
  user_id INTEGER NOT NULL,
  symbol TEXT NOT NULL,
  shares INTEGER NOT NULL CHECK (shares >= 0),
  PRIMARY KEY (user_id, symbol),
  FOREIGN KEY (user_id) REFERENCES users(id)
);