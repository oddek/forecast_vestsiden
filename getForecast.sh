#!/bin/bash

CURDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
srcPath=${CURDIR}/src
now=$(date)


# Run crawler
result=$(${srcPath}/forecastCrawler)

echo "${now}, ${result}, CODE: $?" >> log
echo "${now}, ${result}, CODE: $?"
