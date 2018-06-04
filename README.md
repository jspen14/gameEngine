# gameEngine

> 1st make of interface to do user study

## Build Setup

``` bash
# install dependencies
npm install

# serve with hot reload at localhost:8080
npm run dev

# build for production with minification
npm run build

# build for production and view the bundle analyzer report
npm run build --report

# add matrices from json into database table matrices
node data/addMatrices.js data/i1.json
```

For a detailed explanation on how things work, check out the [guide](http://vuejs-templates.github.io/webpack/) and [docs for vue-loader](http://vuejs.github.io/vue-loader).

## Extract Data from SQL DB

```bash
# Use Python Version 2.7 NOT 3.+
# install dependencies
pip install PyMySQL
# run
python getSQLData.py
```
