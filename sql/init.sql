	drop database weather;
	CREATE DATABASE IF NOT EXISTS weather DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;

	use weather;
	CREATE TABLE IF NOT EXISTS readings 
	(
		id int UNSIGNED NOT NULL AUTO_INCREMENT,
		time datetime NOT NULL UNIQUE,
		air_pressure_at_sea_level float,
		air_temperature float,
		cloud_area_fraction float,
		cloud_area_fraction_high float,
		cloud_area_fraction_low float,
		cloud_area_fraction_medium float,
		dew_point_temperature float,
		fog_area_fraction float,
		relative_humidity float,
		ultraviolet_index_clear_sky float,
		wind_from_direction float,
		wind_speed float,
		wind_speed_of_gust float,
		PRIMARY KEY (id)
	); 



