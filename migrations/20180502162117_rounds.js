exports.up = function(knex, Promise) {
    return knex.schema.createTable('rounds', function(table) {
      table.increments('id').primary();
      table.integer('gameID').unsigned().notNullable().references('id').inTable("games");
      table.integer('matrixID').unsigned().notNullable().references('id').inTable("matrices");
      table.string('result');
    })

};

exports.down = function(knex, Promise) {
  return knex.schema.dropTable('rounds', function(table) {
    table.dropColumn('id');
    table.dropColumn('gameID');
    table.dropColumn('matrixID');
    table.dropColumn('result');
  })
};
