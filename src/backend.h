#ifndef _BACKEND_H_
#define _BACKEND_H_


#include <sqlite3.h>


/* Cuida de criar a tabela */
int create_table(sqlite3 *db);


/* Cuida de inserir dados na tabela */
int insert_task(sqlite3 *db, const char *task);


/* Cuida de atualizar uma tarefa */
int update_task(sqlite3*db, const char *task, int id);


/* Cuida de deletar uma tarefa */
int delete_task(sqlite3 *db, int delete_id);


#endif
