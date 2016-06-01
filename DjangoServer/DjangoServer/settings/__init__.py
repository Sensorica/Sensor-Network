import os

ENV = os.environ.get('LEAKNET_ENV').upper()
DEV = ENV == 'DEV'
STAGING = ENV == 'STAGING'
PROD = ENV == 'PROD'

if DEV:
    from DjangoServer.settings.dev import *
elif STAGING:
    from DjangoServer.settings.staging import *
else:
    from DjangoServer.settings.prod import *