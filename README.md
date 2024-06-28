<h1 align="center"> TodoList em C com Gtk+ e Sqlite3 </h1>

<p align="center">
  <img loading="lazy" src="Imagens/todolistimg.png"/>
</p>

<p align="center">
  <img loading="lazy" src="https://img.shields.io/badge/License-MIT-yellow.svg"/>
</p>

## Descrição do Projeto

<p align="justify">
  Um projeto em C, de um aplicativo simples de gerenciador de tarefas (todolist), usando as bibliotecas Gtk+-3.0 e Sqlite3.
  Projeto usou a ferramenta glade para desenhar a interface gráfica.
  Projeto feito apenas com o propósito de estudo e aprendizado sobre o uso do Gtk e do Sqlite3 na linguagem C.
</p>

##
<p align="center">
  <img loading="lazy" src="/Imagens/img01.png"/>
</p>

<p align="center">
  <img loading="lazy" src="/Imagens/img02.png"/>
</p>

<p align="center">
  <img loading="lazy" src="/Imagens/img03.png"/>
</p>

<p align="center">
  <img loading="lazy" src="/Imagens/img04.png"/>
</p>

<p align="center">
  <img loading="lazy" src="/Imagens/img05.png"/>
</p>

<p align="center">
  <img loading="lazy" src="/Imagens/img06.png"/>
</p>

<p align="center">
  <img loading="lazy" src="/Imagens/img07.png"/>
</p>

<p align="center">
  <img loading="lazy" src="/Imagens/img08.png"/>
</p>

## Dependências do Projeto

* libgtk-3:
```
$ sudo apt install libgtk-3-dev
```

* libsqlite3:
```
$ sudo apt install libsqlite3-dev
```

## Fonte usada para os Labels

* `Roboto`

## Para usar o projeto (opcionalmente instalar)

Navegar para a pasta do projeto:
```
$ cd $HOME/c-gtk-todolist
$ make
```

Depois para executar:
```
$ ./build/tasks
```

Para instalar no sistema:
> [!NOTE]
> Aplicativo foi feito para propósito de aprendizado, não para ser realmente instalado no sistema.
>  Por essa razão, aplicativo não conta com icone nem com arquivo .desktop.
>  Se mesmo assim quiser instalar:

Primeiro precisa de descomentar as seguintes linhas no Makefile:

```
#install: $(addprefix $(BUILD_DIR)/, $(TARGETS))
#	$(INSTALL) -D $^ $(BINDIR)
#	$(INSTALL) -D $(HEADERS) $(INCDIR)
```
e depois:
```
$ make
$ sudo make install
$ make clean
```
Após isso executável estará em `/usr/local/bin/tasks`, certifique-se que diretório está no PATH.
