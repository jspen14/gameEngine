exports.up = function(knex, Promise) {
    return knex.schema.table('chats', function(table) {
      table.increments('id').primary();
      table.integer('chatID').unsigned().notNullable().references('id').inTable("chatID");
      table.integer('userID').unsigned().notNullable().references('id').inTable("users");
      table.string('message');
      table.dateTime('created');
    })

};

exports.down = function(knex, Promise) {
  return knex.schema.table('chats', function(table) {
    table.dropColumn('id');
    table.dropColumn('chatID');
    table.dropColumn('userID');
    table.dropColumn('message');
    table.dropColumn('created');
  })
};
