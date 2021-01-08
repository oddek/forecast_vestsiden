



# Forecast Vestsiden


<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
* [Libraries](#libraries)
* [Prerequisites](#prerequisites)
* [Usage](#usage)
* [License](#license)
* [Contact](#contact)



<!-- ABOUT THE PROJECT -->
## About The Project

This project, gathers weather forecasts from the area around Vestsiden lower secondary school in Kongsberg, and stores it in a database. The software runs once a day, fetching data for the coming 24 hours. 

The solution consists of a tiny shell script running a program written in C++

**Database**:
```
+-----------------------------+--------------+------+-----+---------+----------------+
| Field                       | Type         | Null | Key | Default | Extra          |
+-----------------------------+--------------+------+-----+---------+----------------+
| id                          | int unsigned | NO   | PRI | NULL    | auto_increment |
| time                        | datetime     | NO   | UNI | NULL    |                |
| air_pressure_at_sea_level   | float        | YES  |     | NULL    |                |
| air_temperature             | float        | YES  |     | NULL    |                |
| cloud_area_fraction         | float        | YES  |     | NULL    |                |
| cloud_area_fraction_high    | float        | YES  |     | NULL    |                |
| cloud_area_fraction_low     | float        | YES  |     | NULL    |                |
| cloud_area_fraction_medium  | float        | YES  |     | NULL    |                |
| dew_point_temperature       | float        | YES  |     | NULL    |                |
| fog_area_fraction           | float        | YES  |     | NULL    |                |
| relative_humidity           | float        | YES  |     | NULL    |                |
| ultraviolet_index_clear_sky | float        | YES  |     | NULL    |                |
| wind_from_direction         | float        | YES  |     | NULL    |                |
| wind_speed                  | float        | YES  |     | NULL    |                |
| wind_speed_of_gust          | float        | YES  |     | NULL    |                |
+-----------------------------+--------------+------+-----+---------+----------------+
```

## Libraries
* [Boost](https://www.boost.org)
* [MySQL Connector/C++ 1.1](https://dev.mysql.com/doc/connector-cpp/1.1/en/)
* [libcurl](https://curl.se/libcurl/)
* [nlohmann/json](https://github.com/nlohmann/json)


## Prerequisites

To get a local copy up and running, these are the prerequisites:

```sh
apt install tasksel  
tasksel install lamp-server 
mysql_secure_installation
apt install g++
apt install libcurl4-gnutls-dev
apt install build-essentials
apt install libboost-all-dev
apt install libmysqlcppconn-dev
```

<!-- USAGE -->
## Usage

First you need fill the contents of the files in config/, with credentials for the database. Detailed instructions can be found in the corresponding [readme](config/README.md). 

To initialize the database, run the [initDb.sh](sql/initDb.sh) script. 

Set up a cronjob, to execute the script [getForecast.sh](getForecast.sh) at a regular interval. To do this, open up the cronjob config file with this command:

```
crontab -e
```
and insert this line at the bottom of the file:
```
0 6 * * * /PATH/TO/PROJECT/ROOT/getForecast.sh
```
This will make the script run once every 24 hours, at 06:00.

<!-- LICENSE -->
## License

Distributed under the MIT License. See [`LICENSE`](LICENSE) for more information.

<!-- CONTACT -->
## Contact

Kent Odde - kentodde89@gmail.com

Project Link: [https://github.com/oddek/forecast_vestsiden](https://github.com/oddek/forecast_vestsiden)



