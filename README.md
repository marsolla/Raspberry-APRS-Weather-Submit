# raspi-aprs-weather-submit for Raspberry Pi - ARM Platform.

This code was inspired/copied from https://github.com/rhymeswithmogul/aprs-weather-submit and adapted to run on the Raspberry Pi ARM platform.

This is a repository with small adaptations to run on Raspberry Pi - ARM Platform, among the changes are:
* Changing the return code comparison of the arguments menu in the getopt_long main.c function (return 255 instead of -1);
* Safe use of the strncat function calculating the buffer value without overflow (aprs-wx.c);
* Removed temperature field from packet header and moved to weather data field;
* Adjusted Agent Suppression Option logic;
* APRS_IS, no longer optional - Now is default - fully integrated (#define HAVE_APRSIS_SUPPORT 1 removed);

## Legal Notices

To use this app, you *must* be either:
1.  a licensed amateur radio operator, or
2.  a member of the [Citizen Weather Observer Program](http://wxqa.com/) in good standing.

Anyone can use this app to create [an APRS packet](http://www.aprs.org/doc/APRS101.PDF).  However, to send it to the APRS-IS network, you must have an account on an APRS-IS IGate server, as well as an amateur radio license or CWOP identifier.

## How to compile:

```console
sudo apt install autoconf
autoreconf -i
./configure
make
make install (if you want to install)
```

## How to use:

```console
./raspi-aprs-weather-submit --callsign PPPXXX --latitude 12.34567 --longitude -12.34567 --server brazil.d2g.com --port 14579 --username PPPXXX --password 1234 --temperature-celsius 22.0 --altitude 600.5 --pressure 1013.0 --humidity 50.5 --wind-direction 0 --wind-speed 22.5 --gust 31.5  
```

Or, if you just want the raw packet for your own use, don't specify server information:

```console
./raspi-aprs-weather-submit --callsign PPPXXX --latitude 12.34567 --longitude -12.34567 --temperature-celsius 22.0 --altitude 600.5 --pressure 1013.0 --humidity 50.5 --wind-direction 0 --wind-speed 22.5 --gust 31.5  

PPPXXX>APRS,TCPIP*:@240411z1220.74N/01220.74W_000/023g032t072h50b10130/A=  600m X raspi-aprs-weather-submit/V1.5
```

This app supports all of the weather data parameters defined in APRS versions up to and including version 1.2.1:

*   Altitude (`-A`, `--altitude`)
*   Barometric pressure (`-b`, `--pressure`)
*   Luminosity (`-L`, `--luminosity`)
*   Radiation (`-X`, `--radiation`)
*   Rainfall in the past 24 hours (`-p`, `--rainfall-last-24-hours`)
*   Rainfall since midnight (`-P`, `--rainfall-since-midnight`)
*   Rainfall in the past hour (`-r`, `--rainfall-last-hour`)
*   Relative humidity (`-h`,`  --humidity `)
*   Snowfall in the past 24 hours (`-s`, `--snowfall-last-24-hours`)
*   Temperature (°F) (`-t`, `--temperature`)
*   Temperature (°C) (`-T`, `--temperature-celsius`)
*   Water level above flood stage or mean tide (`-F`, `--water-level-above-stage`)
*   Weather station battery voltage (`-V`, `--voltage`)
*   Wind direction (`-c`, `--wind-direction`)
*   Wind speed, peak in the last five minutes (`-g`, `--gust`)
*   Wind speed, sustained over the last minute (`-S`, `--wind-speed`)

For more details, see the official developer repository: https://github.com/rhymeswithmogul/aprs-weather-submit

73, PU2XNA
