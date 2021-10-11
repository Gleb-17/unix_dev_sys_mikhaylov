# unix-dev-sys

# Репозиторий с файлами для курса UNIX-DEV-SYS

# Базовый курс по Linux

# Михайлов Г.В.

https://dl-ido.spbstu.ru/course/view.php?id=673
Для доступа необходимо 
1. Зарегистрироваться на портале https://dl-ido.spbstu.ru (если ранее не регистрировались).
2. Перейти по ссылке курса (выше)

# Начало работы с проектом

## Сборка и установка сервиса в систему

~~~
$ sudo apt-get install xinetd
~~~

> Установка xinetd (суперпользователь)

~~~
$ git pull origin lesson3
$ cd unix-dev-sys/myweb
$ make myweb
~~~

> Получение ветки репозитория lesson3 и сборка

~~~
$ sudo cp myweb /usr/local/bin/
$ sudo cp myweb-xinetd /etc/xinetd.d/
$ sudo systemctl restart xinetd
~~~

> Установка в систему, перезапуск службы

