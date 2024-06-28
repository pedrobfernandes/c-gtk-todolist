#include "todolist.h"


TodoList *init_todolist()
{
    /* Alocamos TodoList, builder, interface e database.
     * Tambem conecta os widgets com o builder */
    TodoList *todolist;
    if ((todolist = (TodoList *)malloc(
        sizeof(TodoList))) == NULL)
    {
        show_error("Erro alocando TodoList\n");
        return(NULL);
    }

    if ((todolist->builder = init_builder()) == NULL)
    {
        show_error("Erro inicializando o builder");
        destruct_todolist(todolist);
        return(NULL);
    }

    if((todolist->interface = init_interface()) == NULL)
    {
        show_error("Erro iniciando interface\n");
        destruct_todolist(todolist);
        return(NULL);
    }

    if (get_interface_objects(todolist) == FALSE)
    {
        show_error("Erro conectando Widgets com Builder");
        destruct_todolist(todolist);
        return(NULL);
    }

    if((todolist->database = init_database()) == NULL)
    {
        /* No caso das mensagens de erro com o sqlite3
         * não basta a mensagem. Colocamos o erro do sqlite3
         * em sqlite3_errmsg. Por isso usamos snprintf() */
        g_snprintf(todolist->errmsg,
            sizeof(todolist->errmsg),
            "Erro iniciando database, Sqlite3: %s",
            sqlite3_errmsg(todolist->database));

        show_error(todolist->errmsg);

        destruct_todolist(todolist);
        return(NULL);
    }

    todolist->selected_task_id = -1;

    return(todolist);
}


GtkBuilder *init_builder()
{
    GtkBuilder *builder = gtk_builder_new_from_file(
        "glade/TodoList.glade");
    if (builder == NULL)
    {
        return(NULL);
    }

    return(builder);
}


GuiWidgets *init_interface()
{
    GuiWidgets *gui = (GuiWidgets *)malloc(
        sizeof(GuiWidgets));
    if (gui == NULL)
    {
        return(NULL);
    }

    return(gui);
}


gboolean get_interface_objects(TodoList *todolist)
{
    /* Aqui, não queria fazer assim, mas usar algum loop for,
     * por exemplo, criar um array de struct com os widgets
     * e os ids, e fazer um loop, mas, para isso tinha que
     * colocar em todos GTK_WIDGET(), e ao compilar, recebi
     * um monte de avisos e erros do gtk, pois alguns são
     * GTKCELLRENDERER TREE_VIEW_COLUMN etc, então...
     * temos isto */

    if ((todolist->interface->Window = GTK_WIDGET(
        gtk_builder_get_object(
        todolist->builder, "Window"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->UpdateTaskButton = GTK_WIDGET(
        gtk_builder_get_object(
        todolist->builder,
        "UpdateTaskButton"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->DeleteTaskButton = GTK_WIDGET(
        gtk_builder_get_object(
        todolist->builder,
        "DeleteTaskButton"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->EditConfirmButton = GTK_WIDGET(
        gtk_builder_get_object(
        todolist->builder,
        "EditConfirmButton"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->DeleteConfirmButton = GTK_WIDGET(
        gtk_builder_get_object(
        todolist->builder,
        "DeleteConfirmButton"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->InsertTaskButton = GTK_WIDGET(
        gtk_builder_get_object(
        todolist->builder,
        "InsertTaskButton"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->InsertConfirmButton = GTK_WIDGET(
        gtk_builder_get_object
        (todolist->builder,
        "InsertConfirmButton"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->InsertUpdateTaskEntry = GTK_WIDGET(
        gtk_builder_get_object(
        todolist->builder,
        "InsertUpdateTaskEntry"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->TaskListTreeView = GTK_WIDGET(
        gtk_builder_get_object(
        todolist->builder,
        "TaskListTreeView"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->ListStore = GTK_LIST_STORE(
        gtk_builder_get_object(
        todolist->builder,
        "ListStore"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->Scrolled1 = GTK_SCROLLED_WINDOW(
        gtk_builder_get_object(
        todolist->builder,
        "Scrolled1"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->Selection1 = GTK_TREE_SELECTION(
        gtk_builder_get_object(
        todolist->builder,
        "Selection1"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->ID = GTK_TREE_VIEW_COLUMN(
        gtk_builder_get_object(
        todolist->builder, "ID"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->Task = GTK_TREE_VIEW_COLUMN(
        gtk_builder_get_object(
        todolist->builder, "Task"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->IDRenderer = GTK_CELL_RENDERER(
        gtk_builder_get_object(
        todolist->builder,
        "IDRenderer"))) == NULL)
    {
        return(FALSE);
    }

    if ((todolist->interface->TaskRenderer = GTK_CELL_RENDERER(
        gtk_builder_get_object(
        todolist->builder,
        "TaskRenderer"))) == NULL)
    {
        return(FALSE);
    }


    gtk_tree_view_column_add_attribute(
        todolist->interface->ID,
        todolist->interface->IDRenderer,
        "text", 0);

    gtk_tree_view_column_add_attribute(
        todolist->interface->Task,
        todolist->interface->TaskRenderer,
        "text", 1);

    return(TRUE);
}


sqlite3 *init_database()
{
    sqlite3 *database;
    if (sqlite3_open("Tasks.db", &database)
        != SQLITE_OK)
    {
        return(NULL);
    }

    /* Criamos a tabela com "if not exists", então
     * podemos chamar sempre que caso exista não faz nada */
    if (create_table(database) == -1)
    {
        return(NULL);
    }

    return(database);
}


void destruct_todolist(TodoList *todolist)
{
    /* Assim, podemos liberar tudo mesmo quando
     * nada foi alocado. Não faz sentido, mas assim
     * elimina os erros de chamar a função na hora errada */
    if (todolist != NULL)
    {
        if (todolist->interface != NULL)
        {
            free(todolist->interface);
        }

        if (todolist->builder != NULL)
        {
            g_object_unref(todolist->builder);
        }

        if (todolist->database != NULL)
        {
            sqlite3_close(todolist->database);
        }

        free(todolist);
    }
}


void on_InsertTaskButton_clicked(GtkButton
    *InsertTaskButton, TodoList *todolist)
{
    (void)InsertTaskButton;
    gtk_widget_grab_focus(GTK_WIDGET(
        todolist->interface->InsertUpdateTaskEntry));
}


void on_InsertConfirmButton_clicked(GtkButton
    *InsertConfirmButton,
    TodoList *todolist)
{
    (void)InsertConfirmButton;
    if (todolist->interface->InsertUpdateTaskEntry == NULL)
    {
        return;
    }

    const gchar *text = gtk_entry_get_text(GTK_ENTRY(
        todolist->interface->InsertUpdateTaskEntry));

    /* Aqui, garantimos que não inserimos uma tarefa vazia: "",
     * caso o cursor esteja no GTK_ENTRY, mas o usuário tenha
     * apertado o botão de confirmar sem escrever nada */
    if (strlen(text) > 0)
    {
        if (insert_task(todolist->database, text) == -1)
        {
            g_snprintf(todolist->errmsg,
                sizeof(todolist->errmsg),
                "Erro inserindo tarefa, Sqlite3: %s",
                sqlite3_errmsg(todolist->database));

            show_error(todolist->errmsg);

            destruct_todolist(todolist);
            exit(EXIT_FAILURE);
        }

        gtk_entry_set_text(GTK_ENTRY(
            todolist->interface->InsertUpdateTaskEntry), "");
    }

    view_tasks(todolist);
}


 void on_UpdateTaskButton_clicked(GtkButton
    *UpdateTaskButton,
    TodoList *todolist)
{
    (void)UpdateTaskButton;

    /* Para atualizar/deletar uma tarefa, primeiro a tarefa
     * tem que estar selecionada no TreeView. Depois
     * colocamos ela no GTK_ENTRY para o usuario a atualizar.
     * Então, vemos se o TreeView tem tarefas e se o
     * usuario selecionou uma tarefa. */
    GtkTreeModel *model = gtk_tree_view_get_model(
        GTK_TREE_VIEW(todolist->interface->TaskListTreeView));

    if (gtk_tree_model_iter_n_children(model, NULL) == 0)
    {
        return;
    }

    todolist->interface->Selection1 = gtk_tree_view_get_selection(
        GTK_TREE_VIEW(todolist->interface->TaskListTreeView));
    GtkTreeIter iter;

    if (!gtk_tree_selection_get_selected(
            todolist->interface->Selection1,
            &model, &iter))
    {
        return;
    }

    /* Aqui, guarda o id em selected_task_id, para na prómima
     * função, estar disponivel */
    gtk_tree_model_get(model, &iter, 0,
        &todolist->selected_task_id, -1);

    gchar *task;
    gtk_tree_model_get(model, &iter, 1, &task, -1);
    gtk_entry_set_text(GTK_ENTRY(
        todolist->interface->InsertUpdateTaskEntry), task);
    g_free(task);

    /* Aqui, a intenção era colocar o cursor no final da tarefa,
     * e não selecionar a tarefa toda, mas todos os tutoriais
     * que vi supostamente fazer isso, falharam no meu caso.
     * Enão, usei o grab_focus()*/
    gtk_widget_grab_focus(GTK_WIDGET(
        todolist->interface->InsertUpdateTaskEntry));
}


void on_EditConfirmButton_clicked(GtkButton
    *EditConfirmButton,
    TodoList *todolist)
{
    (void)EditConfirmButton;

    if (todolist->interface->InsertUpdateTaskEntry == NULL)
    {
        return;
    }

    if (todolist->selected_task_id == -1)
    {
        return;
    }

    const gchar *text = gtk_entry_get_text(GTK_ENTRY(
        todolist->interface->InsertUpdateTaskEntry));

    if (strlen(text) > 0)
    {
        if (update_task(todolist->database, text,
            todolist->selected_task_id) == -1)
        {
            g_snprintf(todolist->errmsg,
                sizeof(todolist->errmsg),
                "Erro atualizando tarefa, Sqlite3: %s",
                sqlite3_errmsg(todolist->database));

            show_error(todolist->errmsg);

            destruct_todolist(todolist);
            exit(EXIT_FAILURE);
        }

        gtk_entry_set_text(GTK_ENTRY(
            todolist->interface->InsertUpdateTaskEntry), "");
    }

    view_tasks(todolist);

    /* Depois de atualizar/Deletar,
     * reseta select_task_id */
    todolist->selected_task_id = -1;
}


void on_DeleteTaskButton_clicked(GtkButton
    *DeleteTaskButton,
    TodoList *todolist)
{
    (void)DeleteTaskButton;

    GtkTreeModel *model = gtk_tree_view_get_model(
        GTK_TREE_VIEW(todolist->interface->TaskListTreeView));
    if (gtk_tree_model_iter_n_children(model, NULL) == 0)
    {
        return;
    }

    todolist->interface->Selection1 = gtk_tree_view_get_selection(
        GTK_TREE_VIEW(todolist->interface->TaskListTreeView));
    GtkTreeIter iter;

    if (!gtk_tree_selection_get_selected(
        todolist->interface->Selection1,
        &model, &iter))
    {
        return;
    }

    gtk_tree_model_get(model, &iter, 0,
        &todolist->selected_task_id, -1);

    gchar *task;
    gtk_tree_model_get(model, &iter,
        1, &task, -1);

    gtk_entry_set_text(GTK_ENTRY(
        todolist->interface->InsertUpdateTaskEntry), task);

    g_free(task);

    gtk_widget_grab_focus(GTK_WIDGET(
        todolist->interface->InsertUpdateTaskEntry));

}


void on_DeleteConfirmButton_clicked (GtkButton
    *DeleteConfirmButton,
    TodoList *todolist)
{
    (void)DeleteConfirmButton;

    if (todolist->interface->InsertUpdateTaskEntry == NULL)
    {
        return;
    }

    if (todolist->selected_task_id == -1)
    {
        return;
    }
    else
    {
        if (delete_task(todolist->database,
            todolist->selected_task_id) == -1)
        {
            g_snprintf(todolist->errmsg,
                sizeof(todolist->errmsg),
                "Erro deletando tarefa, Sqlite3: %s",
                sqlite3_errmsg(todolist->database));

            show_error(todolist->errmsg);

            destruct_todolist(todolist);
            exit(EXIT_FAILURE);
        }
    }

    gtk_entry_set_text(GTK_ENTRY(
        todolist->interface->InsertUpdateTaskEntry), "");

    view_tasks(todolist);
    todolist->selected_task_id = -1;
}


void view_tasks(TodoList *todolist)
{
    gtk_list_store_clear(todolist->interface->ListStore);

    if (todolist->database == NULL)
    {
        show_error("Base de Dados não existe ou algum"
            " erro nos impede de acessar-la");
        return;
    }


    char *sql = "select * from Tasks;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(todolist->database, sql,
        -1, &stmt, NULL) != SQLITE_OK)
    {
        g_snprintf(todolist->errmsg,
            sizeof(todolist->errmsg),
            "Erro compilando consulta SQL, Sqlite3: %s",
            sqlite3_errmsg(todolist->database));

        show_error(todolist->errmsg);

        sqlite3_finalize(stmt);
        return;
    }


    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *task = sqlite3_column_text(stmt, 1);


        GtkTreeIter iter;
        gtk_list_store_append(
            todolist->interface->ListStore, &iter);

        gtk_list_store_set(todolist->interface->ListStore,
            &iter, 0, id, 1, task, -1);
    }

    sqlite3_finalize(stmt);
}


void show_error(const gchar *message)
{
    GtkWidget *dialog;
    GtkWidget *window;

    /* Criamos assim, pois desta forma, podemos mostrar
     * a janela mesmo que ainda não tenhamos iniciado
     * a janela principal */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Erro");
    gtk_window_set_transient_for(GTK_WINDOW(window), NULL);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
        GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK, "%s", message);

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);
    gtk_widget_destroy(window);
}
