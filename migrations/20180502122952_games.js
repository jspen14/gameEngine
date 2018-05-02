exports.up = function(knex, Promise) {
    return knex.schema.table('games', function(table) {
      table.increments('id').primary();
      table.integer('player1ID').unsigned().notNullable().references('id').inTable("users");
      table.integer('coach1ID').unsigned().references('id').inTable("users");
      table.integer('player2ID').unsigned().notNullable().references('id').inTable("users");
      table.integer('coach2ID').unsigned().references('id').inTable("users");
    })

};

exports.down = function(knex, Promise) {
  return knex.schema.table('games', function(table) {
    table.dropColumn('id');
    table.dropColumn('player1ID');
    table.dropColumn('coach1ID');
    table.dropColumn('player2ID');
    table.dropColumn('coach2ID');
  })

};
