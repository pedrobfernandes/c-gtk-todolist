#ifndef _TODOLIST_H_
#define _TODOLIST_H_


#include <stdlib.h>
#include <gtk/gtk.h>
#include "backend.h"


/* Representa os widgets */
typedef struct
{
    GtkWidget *Window;
    GtkWidget *UpdateTaskButton;
    GtkWidget *DeleteTaskButton;
    GtkWidget *EditConfirmButton;
    GtkWidget *DeleteConfirmButton;
    GtkWidget *InsertTaskButton;
    GtkWidget *InsertConfirmButton;
    GtkWidget *InsertUpdateTaskEntry;
    GtkWidget *TaskListTreeView;

    GtkListStore *ListStore;
    GtkScrolledWindow *Scrolled1;
    GtkTreeSelection *Selection1;
    GtkTreeViewColumn *ID;
    GtkTreeViewColumn *Task;
    GtkCellRenderer *IDRenderer;
    GtkCellRenderer *TaskRenderer;
}
GuiWidgets;


/* O "objeto" TodoList */
typedef struct
{
    GuiWidgets *interface;
    GtkBuilder *builder;
    sqlite3 *database;
    char errmsg[100];

    /* Este é o id da tarefa
     * para deletar ou atualizar */
    int selected_task_id;
}
TodoList;


/* Cuida de iniciar TodoList */
TodoList *init_todolist();


/* Cuida de criar o builder */
GtkBuilder *init_builder();


/* Cuida de alocar a interface */
GuiWidgets *init_interface();


/* Cuida de conectar o builder e os widgets */
gboolean get_interface_objects(TodoList *todolist);


/* Cuida de criar o banco de dados e a tabela */
sqlite3 *init_database();


/* Cuida de liberar TodoList */
void destruct_todolist(TodoList *todolist);


/* Cuida de colocar o cursor do mouse no GTK_ENTRY,
 * quando apertar botão de inserir tarefa */
void on_InsertTaskButton_clicked(GtkButton
    *InsertTaskButton,
    TodoList *todolist);


/* Cuida de inserir tarefa na tabela ao
 * apertar botão de confirmar inserção */
void on_InsertConfirmButton_clicked(GtkButton
    *InsertConfirmButton,
    TodoList *todolist);


/* Cuida de colocar o cursor do mouse no GTK_ENTRY,
 * quando apertar botão de atualizar tarefa */
 void on_UpdateTaskButton_clicked(GtkButton
    *UpdateTaskButton,
    TodoList *todolist);


/* Cuida de colocar o cursor do mouse no GTK_ENTRY,
 * quando apertar botão de deletar tarefa */
void on_DeleteTaskButton_clicked(GtkButton
    *DeleteTaskButton,
    TodoList *todolist);


/* Cuida de atualizar tarefa na tabela ao
 * apertar botão de confirmar atualização */
void on_EditConfirmButton_clicked(GtkButton
    *EditConfirmButton,
    TodoList *todolist);


/* Cuida de deletar tarefa da tabela ao
 * apertar botão de confirmar atualização */
void on_DeleteConfirmButton_clicked (GtkButton
    *DeleteConfirmButton,
    TodoList *todolist);


/* Cuida de mostrar as tarefas no TREE_VIEW */
void view_tasks(TodoList *todolist);


/* Cuida de mostrar mensagem de erro */
void show_error(const gchar *message);


#endif
