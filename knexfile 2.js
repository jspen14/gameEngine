// Update with your config settings.

module.exports = {

  development: {
    client: 'mariasql', // do this matter?
    connection: {
      host: '127.0.0.1',
      user: 'root',
      password: '',
      db: 'gameengine',
      charset: 'utf8',
    }
  },

  staging: {
    client: 'postgresql',
    connection: {
      database: 'my_db',
      user:     'username',
      password: 'password'
    },
    pool: {
      min: 2,
      max: 10
    },
    migrations: {
      tableName: 'knex_migrations'
    }
  },

  production: {
    client: 'postgresql',
    connection: {
      database: 'my_db',
      user:     'username',
      password: 'password'
    },
    pool: {
      min: 2,
      max: 10
    },
    migrations: {
      tableName: 'knex_migrations'
    }
  }

};
