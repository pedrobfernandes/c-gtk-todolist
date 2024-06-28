#include "todolist.h"


int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    TodoList *todolist = NULL;

    if ((todolist = init_todolist()) == NULL)
    {
        return(EXIT_FAILURE);
    }

    g_signal_connect(todolist->interface->Window, "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(todolist->interface->InsertTaskButton, "clicked",
        G_CALLBACK(on_InsertTaskButton_clicked), todolist);

    g_signal_connect(todolist->interface->InsertConfirmButton, "clicked",
        G_CALLBACK(on_InsertConfirmButton_clicked), todolist);

    g_signal_connect(todolist->interface->UpdateTaskButton, "clicked",
        G_CALLBACK(on_UpdateTaskButton_clicked), todolist);

    g_signal_connect(todolist->interface->EditConfirmButton, "clicked",
        G_CALLBACK(on_EditConfirmButton_clicked), todolist);

    g_signal_connect(todolist->interface->DeleteTaskButton, "clicked",
        G_CALLBACK(on_DeleteTaskButton_clicked), todolist);

    g_signal_connect(todolist->interface->DeleteConfirmButton, "clicked",
        G_CALLBACK(on_DeleteConfirmButton_clicked), todolist);


    gtk_widget_show_all(todolist->interface->Window);
    view_tasks(todolist);


    gtk_main();
    destruct_todolist(todolist);
    return(EXIT_SUCCESS);
}
