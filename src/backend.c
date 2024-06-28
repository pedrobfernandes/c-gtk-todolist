#include "backend.h"


int create_table(sqlite3 *db)
{
    char *sql = "create table if not exists Tasks ("
        "id integer primary key autoincrement,"
        "tarefa text not null);";

    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK)
    {
        return(-1);
    }

    return(0);
}


int insert_task(sqlite3 *db, const char *task)
{
    /* Aqui, primeiro pegamos o registro com o maior id.
     * Depois de pegarmos e atribuirmos a select_id,
     * incrementamos o id em um. Assim garantimos que os
     * ids são continuos (sem lacunas). Por exemplo, se temos
     * 3 registros e deletamos o registro com id 3, quando
     * inserimos aqui, ficaria com id 4. Assim fica com o id 3 */
    sqlite3_stmt *stmt_select;
    sqlite3_stmt *stmt_insert;

    char *sql_select = "select * from Tasks "
                        "where id = (select max(id) from Tasks);";
    char *sql_insert = "insert into Tasks(id, tarefa) values (?, ?);";


    if (sqlite3_prepare_v2(db, sql_select, -1,
        &stmt_select, 0) != SQLITE_OK)
    {
        return(-1);
    }

    int select_id = 0;
    if (sqlite3_step(stmt_select) == SQLITE_ROW)
    {
        select_id = sqlite3_column_int(stmt_select, 0);
    }

    sqlite3_finalize(stmt_select);
    select_id += 1;


    /* Insere */
    if (sqlite3_prepare_v2(db, sql_insert, -1,
        &stmt_insert, 0) != SQLITE_OK)
    {
        return(-1);
    }


    sqlite3_bind_int(stmt_insert, 1, select_id);
    sqlite3_bind_text(stmt_insert, 2, task,
        -1, SQLITE_STATIC);


    if (sqlite3_step(stmt_insert) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt_insert);
        return(-1);
    }

    sqlite3_finalize(stmt_insert);
    return(0);
}


int update_task(sqlite3 *db, const char *task, int id)
{
    sqlite3_stmt *stmt;
    char *sql = "update Tasks set tarefa = ? where id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1,
        &stmt, 0) != SQLITE_OK)
    {
        return(-1);
    }

    sqlite3_bind_text(stmt, 1, task,
        -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return(result);
}


int delete_task(sqlite3 *db, int delete_id)
{
    /* Aqui, atualizamos os ids (incrementamos em 1), os ids
     * que são maiores do que o id do registro que deletamos.
     * Assim garantimos que não tem lacunas nos ids */
    char *sql_delete = "delete from tasks where id = ?;";
    char *sql_update = "update tasks set id = id - 1 where id > ?;";

    sqlite3_stmt *stmt_delete;
    sqlite3_stmt *stmt_update;


    /* Primeiro trata da exclusão */
    if (sqlite3_prepare_v2(db, sql_delete, -1,
        &stmt_delete, 0) != SQLITE_OK)
    {
        return(-1);
    }

    sqlite3_bind_int(stmt_delete, 1, delete_id);

    int result_delete = sqlite3_step(stmt_delete);
    sqlite3_finalize(stmt_delete);

    /* Agora verificamos se excluiu Ok */
    if (result_delete != SQLITE_DONE)
    {
        return(-1);
    }


    /* Agora atualizamos os ids */
    if (sqlite3_prepare_v2(db, sql_update, -1,
        &stmt_update, 0) != SQLITE_OK)
    {
        return(-1);
    }

    sqlite3_bind_int(stmt_update, 1, delete_id);

    int result_update = sqlite3_step(stmt_update);
    sqlite3_finalize(stmt_update);

    if (result_update != SQLITE_DONE)
    {
        return(-1);
    }

    return(result_delete);
}
