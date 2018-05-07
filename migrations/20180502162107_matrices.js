
exports.up = function(knex, Promise) {
  return knex.schema.createTable('matrices', function(table){
  	table.increments('id').primary();
  	table.string('type');
  	table.string('matrix');
  })
};

exports.down = function(knex, Promise) {
  return knex.schema.dropTable('users', function(table) {
    table.dropColumn('id');
    table.dropColumn('type');
    table.dropColumn('matrix');
})
};
