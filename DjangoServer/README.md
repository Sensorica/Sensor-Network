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
4. On Ubuntu: Edit $VIRTUAL_ENV/bin/postactivate to contain the following lines:

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

4.3 To switch between development, staging, and production set LEAKNET_ENV =

'dev' for development
'staging' for staging
'production' for production

5. Deactivate and re-activate virtualenv:

```
deactivate
workon leaknet
```

4. Insert Keys into the Ignition!
=================================

The main settings file has some fields that need to be entered manually, or via a config file that is not shared publicly.

You will need to add your own SECRET_KEYS to the 


5. Database
===========
1. Create database table:

```
psql -Uyour_psql_user
CREATE DATABASE leaknet;
```

2. Migrations: ``./manage.py migrate``
3. Create admin: ``./manage.py createsuperuser``
4. Run the server ``./manage.py runserver``
