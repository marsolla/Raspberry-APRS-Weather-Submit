# aprs-weather-submit
Not everyone has a fancy weather station with APRS connectivity built in.  Maybe you're like me, and have an old-school thermometer and CoCoRaHS-approved rain gauge.  This command-line app, written in classic K&R C, can compile on most Linux toolchains and will manually submit weather information to the APRS-IS network.

## Help
Anyone can use this app to create an APRS 1.2-compliant packet.  However, to send it to the APRS-IS network, you must have an account on an APRS-IS IGate server.

## Examples
At the bare minimum, you can submit your weather station's position with a command line like this:
```bash
./aprs-weather-submit --callsign W1AW-13 --latitude 41.714692 --longitude -72.728514 --server example-igate-server.foo --port 12345 --username hiram --password percymaxim
```

If you'd like to report a temperature of 68°F, you can use a command like this:
```bash
./aprs-weather-submit -k W1AW-13 -n 41.714692 -e -72.728514 -I example-igate-server.foo -o 12345 -u hiram -d percymaxim -t 68
```

Or, if you just want the raw packet for your own use, don't specify server information:
```bash
$ ./aprs-weather-submit -k W1AW-13 -n 41.714692 -e -72.728514 -t 68
W1AW-13>APRS,TCPIP*:@210548z/9F,=<+74_  Ct068Xaprs-weather-submit/1.0
```

This app supports all of the weather data parameters defined in APRS versions up to and including version 1.2:
* Barometric pressure (`-b`, `--pressure`)
* Luminosity (`-L`, `--luminosity`)
* Radiation (`-X`, `--radiation`)
* Rainfall in the past 24 hours (`-p`, `--rainfall-last-24-hours`)
* Rainfall since midnight (`-P`, `--rainfall-since-midnight`)
* Rainfall in the past hour (`-r`, `--rainfall-last-hour`)
* Relative humidity (`-h`,` --humidity`)
* Snowfall in the past 24 hours (`-s`, `--snowfall-last-24-hours`)
* Temperature (`-t`, `--temperature`)
* Water level above flood stage or mean tide (`-F`, `--water-level-above-stage`)
* Weather station battery voltage (`-V`, `--voltage`)
* Wind direction (`-c`, `--wind-direction`)
* Wind speed, peak in the last five minutes (`-g`, `--gust`)
* Wind speed, sustained over the last minute (`-S`, `--wind-speed`)

## Legal Notices
To use this app, you *must* be either:
1. a licensed amateur radio operator, or
2. a member of the [Citizen Weather Observer Program](http://wxqa.com/) in good standing.
[Getting your ham radio license is easy](https://hamstudy.org/), and [joining CWOP is even easier](http://wxqa.com/SIGN-UP.html).  

Like it says in the license:  this app is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the [GNU General Public License 3.0](https://www.gnu.org/licenses/) for more details.  As such, you and you alone are solely responsible for using this app to submit complete and correct weather and/or location data.  Please do not use this app for evil.  Don't make me regret writing this app.

73, KC1HBK
