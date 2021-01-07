#!/bin/bash 

#paths
CURDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
configPath=${CURDIR}/../config
sqlPath=${CURDIR}

echo 'Creating Database..'
mysql --defaults-extra-file=${configPath}/db.conf -s -N -e "
  source init.sql"
