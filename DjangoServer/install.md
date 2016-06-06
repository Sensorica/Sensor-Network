Welcome to the Leaknet Django Administration Site code created by Sensorica. This document explains
the basic steps to get started. 

1. Install pre-requisites
=========================

Virtualenv
----------
Standard installation with virtualevnwrapper.

PostgreSQL
----------
Standard installation.


2. Create virtual environment
=============================

1. Clone repository: ``git clone https://github.com/Sensorica/Sensor-Network.git``
2. Create virtual environment: ``mkvirtualenv leaknet``
3. Install requirements ``pip install -r requirements.txt``
4. Edit $VIRTUAL_ENV/bin/postactivate to contain the following lines:

```
export LEAKNET_ENV=dev
export DB_USER=your_psql_user
export DB_PASSWORD=your_psql_user_pass
export DEV_ADMIN_EMAIL=your_email
export DJANGO_SETTINGS_MODULE=leaknet.settings
```
4.1 The Windows equivalent is %VIRTUAL_ENV%/Scripts/activate.bat
```
set LEAKNET_ENV=dev
set DB_USER=your_psql_user
set DB_PASSWORD=your_psql_user_pass
set DEV_ADMIN_EMAIL=your_email
set DJANGO_SETTINGS_MODULE=leaknet.settings
```


5. Deactivate and re-activate virtualenv:

```
deactivate
workon leaknet
```

4. Database
=============
1. Create database table:

```
psql -Uyour_psql_user
CREATE DATABASE leaknet;
```

2. Migrations: ``./manage.py migrate``
3. Create admin: ``./manage.py createsuperuser``
4. Run the server ``./manage.py runserver``
