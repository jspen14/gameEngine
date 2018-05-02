exports.up = function(knex, Promise) {
    return knex.schema.table('chatID', function(table) {
      table.increments('id').primary();
      table.integer('gameID').unsigned().notNullable().references('id').inTable("games");
      table.integer('user1').unsigned().notNullable().references('id').inTable("users"); // user can be either a coach or a player
      table.integer('user2').unsigned().notNullable().references('id').inTable("users");
      table.string('chatType'); //pp, pc
    })

};

exports.down = function(knex, Promise) {
  return knex.schema.table('chatID', function(table) {
    table.dropColumn('id');
    table.dropColumn('gameID');
    table.dropColumn('user1');
    table.dropColumn('user2');
    table.dropColumn('chatType');
  })

};
