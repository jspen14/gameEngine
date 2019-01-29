# gameEngine
- You need to have a mysql program installed. MariaSQL was used in production. (Follow instructions here: https://github.com/zappala/cs260-examples/tree/master/node-sql)

- Open MYSQL on terminal then use commands:
  - "CREATE DATABASE gameEngine;"
  - "quit;"

- Once database is created, in main folder (that contains c++ folder),run migrations with command:
  - "npx knex migrate:latest"

- After migrations have been added, in main folder (that contains c++ folder), add matrices and AI users with commands:
  - node data/addMatrices.js data/i1.json (then ctrl c)
  - node data/addUsers.js (then ctrl c)

- Navigate to C++ folder (cd C++) and compile C++ client with commands:
  - g++ client.cpp -o client

- Navigate back to main folder (that contains c++ folder), and install dependencies for node with command:
  - npm install
    - If it says that it didn't install all dependencies, follow the commands provided by terminal


****************************************************************************************
Running the program
****************************************************************************************
-Turn on mysql
  - brew services start mariadb
  - mysql -u root
- In the main folder, start the server with commands:
  - source .env
  - node server.js [game matrix] (served on port 3000)

- In the main folder, start the front end with command:
  - npm run dev (runs on port 8080)

- In the C++ folder, add a C++ client (if desired) with commands:
  - cd C++ (to navigate into C++ folder)
  - ./igPlayer Trump bully  
    - username can be Trump, Thumper, or Carnegie
    - Host is set to 127.0.0.1 but can be changed. If I need to change it, let me know.

- Start game
    - http://localhost:8080/#/admin
 

Josh

(If you want to connect multiple users on your machine you MUST use a different browser because we used JSW tokens)
(Preferred browser: Google Chrome)

## Build Setup

``` bash
# install dependencies
npm install


# add matrices from json into database table matrices
node data/addMatrices.js data/i1.json
```


## Extract Data from SQL DB

```bash
# Use Python Version 2.7 NOT 3.+
# install dependencies
pip install PyMySQL
# run
python getSQLData.py
```
