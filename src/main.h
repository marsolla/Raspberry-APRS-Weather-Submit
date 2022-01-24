/*
 raspi-aprs-weather-submit version 1.5
 Modified by Rafael Marsolla <rafamarsolla@gmail.com>
 <https://github.com/marsolla/Raspberry-APRS-Weather-Submit>

 Official Dev of aprs-weather-submit version 1.5:
 aprs-weather-submit version 1.5
 Copyright (c) 2019-2022 Colin Cogle <colin@colincogle.name>
 <https://github.com/rhymeswithmogul/aprs-weather-submit>
 
 This file, main.h, is part of raspi-aprs-weather-submit.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License along
with this program.  If not, see <https://www.gnu.org/licenses/agpl-3.0.html>.
*/

#ifndef main_h
#define main_h

/* Program constants that should be set by ./configure. */
#ifndef PACKAGE
#define PACKAGE "raspi-aprs_weather_submit"
#endif

#ifndef VERSION
#define VERSION "1.5"
#endif

/**
 * usage() -- show some help.
 *
 * @author Colin Cogle Modified by Rafael Marsolla
 * @since  1.5
 */
extern void
usage (void);

/**
 * help() -- show even more help.
 *
 * @author Colin Cogle Modified by Rafael Marsolla
 * @since  1.5
 */
extern void
help (void);

/**
 * version() -- show the version.
 *
 * @author Colin Cogle Modified by Rafael Marsolla
 * @since  1.5
 */
extern void
version (void);

/* A decent buffer size (plus one). */
#ifndef	BUFSIZE
#define BUFSIZE 1025
#endif

#endif /* main_h */
