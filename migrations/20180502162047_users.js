exports.up = function(knex, Promise) {
    return knex.schema.createTable('users', function(table) {
      table.increments('id').primary();
      table.string('role'); // either player or coach
      table.string('coachType'); // Human, AI, or Dummy
      //TODO when we do the login page, the information is going to go here.

    })

};

exports.down = function(knex, Promise) {
  return knex.schema.dropTable('users', function(table) {
    table.dropColumn('id');
    table.dropColumn('role');
    table.dropColumn('coachType');

  })
};

