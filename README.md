# aprs-weather-submit for Raspberry Pi - ARM Platform.
*LEGAL NOTICE*

*To use this app, you must be either:
a licensed amateur radio operator, or a member of the Citizen Weather Observer Program in good standing.*

This is a repository with minor adaptations to run on Raspberry Pi - ARM Platform, among the changes are:
* Changing the argument menu in getopt_long function main.c (return 255 instead of -1)
* Safe use of strncat function calculating buffer value without overflow (aprs-wx.c)
* Forced use mode APRS_IS, no longer optional - #define HAVE_APRSIS_SUPPORT 1 forever

To compile:
sudo apt install autoconf
autoreconf -i
./configure
make
make install (if you want to install)

For more details, see the official developer repository: https://github.com/rhymeswithmogul/aprs-weather-submit

QTH. 73, PU2XNA

The text below was copied from official repository:

# aprs-weather-submit

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a5e5337dd57b486089391aabd2f5429b)](https://app.codacy.com/gh/rhymeswithmogul/aprs-weather-submit?utm_source=github.com\&utm_medium=referral\&utm_content=rhymeswithmogul/aprs-weather-submit\&utm_campaign=Badge_Grade_Settings)

Not everyone has a fancy weather station with APRS connectivity built in.  Maybe you're like me, and have an old-school thermometer and CoCoRaHS-approved rain gauge.  This command-line app, written in C99, can compile on most Linux toolchains (Windows support is in the works) and will manually submit APRS 1.2.1-compliant weather information to the APRS-IS network.

## Help

Anyone can use this app to create [an APRS packet](http://www.aprs.org/doc/APRS101.PDF).  However, to send it to the APRS-IS network, you must have an account on an APRS-IS IGate server, as well as an amateur radio license or CWOP identifier (more on that below).

## Examples

At the bare minimum, you can submit your weather station's position with a command line like this:

```console
$ ./aprs-weather-submit --callsign W1AW-13 --latitude 41.714692 --longitude -72.728514 --altitude 240 --server example-igate-server.foo --port 12345 --username hiram --password percymaxim
```

If you'd like to report a temperature of 68°F, you can use a command like this:

```console
$ ./aprs-weather-submit -k W1AW-13 -n 41.714692 -e -72.728514 -I example-igate-server.foo -o 12345 -u hiram -d percymaxim -t 68
```

Or, if you just want the raw packet for your own use, don't specify server information:

```console
>>>>>>> 03eee40774a16ec1398e6c52ce82557d30d17f78
$ ./aprs-weather-submit -k W1AW-13 -n 41.714692 -e -72.728514 -t 68
W1AW-13>APRS,TCPIP*:@090247z4142.88N/07243.71W_.../...t068Xaprs-weather-submit/1.5
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

## Legal Notices

To use this app, you *must* be either:

1.  a licensed amateur radio operator, or
2.  a member of the [Citizen Weather Observer Program](http://wxqa.com/) in good standing.

[Getting your ham radio license is easy](https://hamstudy.org/), and [joining CWOP is even easier](http://wxqa.com/SIGN-UP.html).

Like it says in the license:  this app is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the [GNU Affero General Public License 3.0](https://www.gnu.org/licenses/agpl-3.0.html) for more details.  As such, you and you alone are solely responsible for using this app to submit complete and correct weather and/or location data.  Please do not use this app for evil.  Don't make me regret writing this app.

QTH. 73, KC1HBK

