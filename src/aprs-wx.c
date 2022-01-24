/*
 raspi-aprs-weather-submit version 1.5
 Modified by Rafael Marsolla <rafamarsolla@gmail.com>

 Official Dev of aprs-weather-submit version 1.5:

 aprs-weather-submit version 1.5
 Copyright (c) 2019-2022 Colin Cogle <colin@colincogle.name>

 This file, aprs-wx.c, is part of aprs-weather-submit.
 <https://github.com/rhymeswithmogul/aprs-weather-submit>

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

#include <stdio.h>   /* fprintf(), printf(), snprintf(), fputs() */
#include <string.h>  /* strcpy(), strncpy(), strncat(), strlen() */
#include <math.h>    /* floor(), round(), pow(), fabs() */
#include <time.h>    /* struct tm, time_t, time(), gmtime() */
#include <assert.h>  /* assert() */

#include "main.h"    /* PACKAGE, VERSION, BUFSIZE */
#include "aprs-wx.h"
#define HAVE_APRSIS_SUPPORT 1

/**
 * packetConstructor() -- put some default values into an APRSPacket
 *
 * @param p	The struct to instantiate.
 * @since 0.1
 */
void
packetConstructor (APRSPacket* const p)
{
	strcpy(p->callsign, "");
	strcpy(p->latitude, "");
	strcpy(p->longitude, "");
	strcpy(p->altitude, "");
	strcpy(p->windDirection, " ");
	strcpy(p->windSpeed, " ");
	strcpy(p->gust, "..");
	strcpy(p->temperature, "...");
	strcpy(p->rainfallLastHour, "...");
	strcpy(p->rainfallLast24Hours, "...");
	strcpy(p->rainfallSinceMidnight, "...");
	strcpy(p->humidity, "..");
	strcpy(p->pressure, ".....");
	strcpy(p->luminosity, "....");
	strcpy(p->radiation, "...");
	strcpy(p->waterLevel, "....");
	strcpy(p->voltage, "...");
	strcpy(p->snowfallLast24Hours, "...");
	return;
}

/**
 * compressedWindSpeed() -- return an APRS-compressed wind speed.
 *
 * This value will fill the APRS speed value, calculated as below.
 * Let R be the return value, and S be speed.
 *
 * Given that:  1.08^R - 1 = S
 * Then:            1.08^R = S + 1
 *            log (1.08^R) = log (S + 1)
 *              R log 1.08 = log (S + 1)
 *                       R = log (S + 1) / log 1.08
 *
 * Then, as per the APRS spec, add 33 to convert it to ASCII.
 *
 * @author       Colin Cogle
 * @param speed  The wind speed, in miles per hour.
 * @since        0.2
 */
inline char
compressedWindSpeed (const unsigned short speed)
{
    return (char)(round(log1p(speed) / logf(1.08)) + 33);
}

/**
 * compressedWindDirection() -- return an APRS-compressed wind direction.
 *
 * This value will fill the APRS direction/heading value.
 *
 * @author          Colin Cogle
 * @param direction The direction in which the wind is blowing, in degrees from
 *                  true north.
 * @since           0.2
 */
inline char
compressedWindDirection (const unsigned short direction)
{
	return (char)(round(direction / 4) + 33);
}

/**
 * compressedPosition() -- return an APRS-compressed latitude or longitude value.
 *
 * @author             Colin Cogle
 * @param pResult      A constant pointer to the return value.
 * @param decimal      The latitude or longitude value, in decimal-formatted
 *                     degrees.
 * @param isLongitude  The constant IS_LATITUDE or IS_LONGITUDE.
 * @since              0.2
 */
void
compressedPosition (char* const pResult, const double decimal,
                    const char isLongitude)
{
	char         pos = 0;       /* position; an iterator */
	unsigned int x   = 190463;  /* magic number for longitude
	                             * (see APRS 1.0 spec, p.38) */

	if (isLongitude == IS_LONGITUDE)
	{
		x = (unsigned int)(x * (180 + decimal));
	}
	else
	{
		/* The magic number for latitude is exactly twice that of longitude,
		 * so that's why we're doubling it here (also on p.38 of the APRS spec).
		 */
		x = (unsigned int)(x * 2 * (90 - decimal));
	}

	for (; pos < 3; pos++)
	{
		unsigned int divisor = (unsigned int)pow(91, 3 - pos);
		unsigned int result  = (unsigned int)floor(x / divisor);
		pResult[(unsigned int)pos] = (char)(result + 33);
		x %= divisor;
	}
	pResult[3] = (char)((x % 91) + 33);
	pResult[4] = '\0';

	return;
}


/**
 * uncompressedPosition() -- return an APRS-uncompressed latitude or longitude
 * value.
 *
 * @author            Colin Cogle
 * @param pResult     A constant pointer to the return value.
 * @param decimal     The latitude or longitude value, in decimal-formatted
 *                    degrees.
 * @param isLongitude The constant IS_LATITUDE or IS_LONGITUDE.
 * @since             0.2
 */
void
uncompressedPosition (char* const pResult, const double decimal,
                      const char isLongitude)
{
	short degrees;
	float minutes = 0;

	if (!isLongitude && (decimal > 90 || decimal < -90))
	{
		degrees = 90;
	}
	else if (isLongitude && (decimal < -180 || decimal > 180))
	{
		degrees = 180;
	}
	else
	{
		float x = fabs(decimal);
		degrees = (short)floor(x);
		minutes = (x - degrees) * 60;
	}

	if (isLongitude == IS_LATITUDE)
	{
		int ret = snprintf(pResult,  9, "%02hi%.2f%c",
		                   degrees, minutes, (decimal < 0 ? 'S' : 'N'));
		assert(ret >= 0);
	}
	else
	{
		int ret = snprintf(pResult, 10, "%03hi%.2f%c",
		         degrees, minutes, (decimal < 0 ? 'W' : 'E'));
		assert(ret >= 0);
	}

	return;
}

/**
 * rain() -- format a rainfall measurement
 *
 * @author        Colin Cogle
 * @param pResult A constant pointer to the return value.
 * @param precip  A constant representing how much precipitation precipitated.
 * @since         0.2
 */
inline void
rain (char* const pResult, const double precip)
{
	int ret = snprintf(pResult, 4, "%03d", (unsigned short)precip);
	assert(ret >= 0);
	return;
}

/**
 * notNull() -- return !0 if the user specified a meaningful value.
 *
 * Return !0 if there is a numerical value for this parameter.  If the user
 * hasn't specified a value, then packetConstructor() would have filled this
 * with dots, so return 0.
 *
 * @author    Colin Cogle
 * @param val A constant pointer to the raw value, which will remain constant.
 * @return    0 if this value is unspecified/not meaningful; !0 otherwise.
 * @since     0.2
 */
inline int
notNull (const char* const val)
{
	return val[0] != '.';
}

/**
 * printAPRSPacket() -- create a textual representation of an APRS weather packet.
 *
 * @author                  Colin Cogle
 * @param p                 A constant pointer to an APRS packet of type
 *                          (struct APRSPacket).
 * @param ret               A constant pointer to a string that will hold the
 *                          return value.
 * @param compressedPacket  The constant COMPRESSED_PACKET or UNCOMPRESSED_PACKET.
 * @param suppressUserAgent If !=0, don't put the Linux flag ('X') nor the app
 *                          name and version in the comment field.
 * @since 0.1
 */
void
printAPRSPacket (APRSPacket* restrict const p, char* restrict const ret,
                 char compressPacket, char suppressUserAgent)
{
	char       result[BUFSIZE] = "\0";
	time_t     t               = time(NULL);
	struct tm* now             = gmtime(&t); /* APRS uses GMT */

	if (compressPacket == COMPRESSED_PACKET)
	{
		/* Compression type byte ("T"):
		 * (GPS fix: current) | (NMEA source: other) | (Origin: software) = 34
		 * Add 33 as per the spec, and you get 67, the ASCII code for 'C'.
		 *                                                           ?
		 *                              header_________ timestamp____ pos_wc_s_Tt__*/
		//int ret = snprintf(result, 48, "%s>APRS,TCPIP*:@%.2d%.2d%.2dz/%s%s_%c%cg%cCt%s",
		int ret = snprintf(result, 53, "%s>APRS,TCPIP*:@%.2d%.2d%.2dz/%s%s_%c%cg%cCt%s",
			p->callsign, now->tm_mday, now->tm_hour, now->tm_min,
			p->latitude, p->longitude, p->windDirection[0], p->windSpeed[0],p->gust[0],
			p->temperature);
		assert(ret >= 0);
	}
	else {
		/*                              header_________ timestamp____pos__wc_ s_t__*/
		//int ret = snprintf(result, 61, "%s>APRS,TCPIP*:@%.2d%.2d%.2dz%s/%s_%s/%sg%st%s",
		int ret = snprintf(result, 66, "%s>APRS,TCPIP*:@%.2d%.2d%.2dz%s/%s_%s/%sg%st%s",
			p->callsign, now->tm_mday, now->tm_hour, now->tm_min,
			p->latitude, p->longitude, p->windDirection, p->windSpeed,p->gust,
			p->temperature);
		assert(ret >= 0);
	}

/*	if (notNull(p->gust))
	{
		strncat(result, "g=", 2);
		strncat(result, p->gust, 4);
	}
*/
	if (notNull(p->rainfallLastHour))
	{
		strncat(result, "r", (sizeof(result) -1 ) - strlen(result));
		strncat(result, p->rainfallLastHour, (sizeof(result) -1 ) - strlen(result));
	}

	if (notNull(p->rainfallLast24Hours))
	{
		strncat(result, "p", (sizeof(result) -1 ) - strlen(result));
		strncat(result, p->rainfallLast24Hours, (sizeof(result) -1 ) - strlen(result));
	}

	if (notNull(p->rainfallSinceMidnight))
	{
		strncat(result, "P", (sizeof(result) -1 ) - strlen(result));
		strncat(result, p->rainfallSinceMidnight, (sizeof(result) -1 ) - strlen(result));
	}

	if (notNull(p->humidity))
	{
		strncat(result, "h",(sizeof(result) -1 ) - strlen(result));
		strncat(result, p->humidity, (sizeof(result) -1 ) - strlen(result));
	}

	if (notNull(p->pressure))
	{
		strncat(result, "b",(sizeof(result) -1 ) - strlen(result));
		strncat(result, p->pressure, (sizeof(result) -1 ) - strlen(result));
	}

	if (notNull(p->luminosity))
	{
		/* Remember, the letter is included below. */
		strncat(result, p->luminosity, (sizeof(result) -1 ) - strlen(result));
	}

	if (notNull(p->radiation))
	{
		strncat(result, "X", (sizeof(result) -1 ) - strlen(result));
		strncat(result, p->radiation, (sizeof(result) -1 ) - strlen(result));
	}

	/* F is required by APRS 1.2 if voltage is present  */
	if (notNull(p->waterLevel) || notNull(p->voltage))
	{
		strncat(result, "F", (sizeof(result) -1 ) - strlen(result));
		strncat(result, p->waterLevel, (sizeof(result) -1 ) - strlen(result));
	}

	if (notNull(p->voltage))
	{
		strncat(result, "V",(sizeof(result) -1 ) - strlen(result));
		strncat(result, p->voltage,(sizeof(result) -1 ) - strlen(result));
	}

	if (notNull(p->snowfallLast24Hours))
	{
		/* Add snowfall last, as some sites (aprs.fi) accidentally parse a
		 * period as the start of a comment, and will ignore the weather data
		 * after it.
		 */
		strncat(result, "s",(sizeof(result) -1 ) - strlen(result));
		strncat(result, p->snowfallLast24Hours, (sizeof(result) -1 ) - strlen(result));
	}

	if (notNull(p->altitude))
	{
		strncat(result, "/A=", (sizeof(result) -1 ) - strlen(result));
		strncat(result, p->altitude, (sizeof(result) -1 ) - strlen(result));
		strncat(result, "m", (sizeof(result) -1 ) - strlen(result));
	}
	
	if (suppressUserAgent != 0)
	{
		strncat(result, " X ",(sizeof(result) -1 ) - strlen(result));
		//strncat(result, "FATWeatherStation", BUFSIZE - strlen(result) - 17);
		strncat(result, PACKAGE, (sizeof(result) -1 ) - strlen(result));
		//strncat(result, "/", 1);
		strncat(result, " V", (sizeof(result) -1 ) - strlen(result));
		strncat(result, VERSION, (sizeof(result) -1 ) - strlen(result));
	}

	strncat(result, "\n\0",(sizeof(result) -1 ) - strlen(result));
	strcpy(ret, result);
	return;
}
