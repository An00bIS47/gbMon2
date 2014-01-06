#!/bin/sh

#  makeGraphs.sh
#  gbMon2
#
#  Created by michael on 06.01.14.
#  Copyright (c) 2014 michael. All rights reserved.
echo 'Creating Graphs'

echo '  Last Hour'
rrdtool graph /home/pi/.gbmon/persist/graphs/temperature1h.png            \
--start end-1h                                             \
--title 'Temperatur'                                       \
--vertical-label '°C'                                      \
--width 700 --height 275                                   \
--alt-y-grid                                               \
--interlaced                                               \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                           \
-c FONT#8E8E93                                             \
-c BACK#ffffff                                             \
'DEF:ds1=temperature.rrd:temperature_1:AVERAGE'            \
'DEF:ds2=temperature.rrd:temperature_2:AVERAGE'            \
'DEF:ds3=temperature.rrd:temperature_3:AVERAGE'            \
'LINE2:ds1#5AC8FA:Temperatur 1\t\t\t\t'                    \
'LINE2:ds2#FF2D55:Temperatur 2\t\t\t\t'                    \
'LINE2:ds3#8E8E93:Temperatur 3\n'                          \
'GPRINT:ds1:MAX:   Maximum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds2:MAX:   Maximum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds3:MAX:   Maximum\:\t%3.1lf °C\n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds2:MIN:   Minimum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds3:MIN:   Minimum\:\t%3.1lf °C\n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %3.1lf °C'          \
'GPRINT:ds2:AVERAGE:   Durchschnitt\:  %3.1lf °C'          \
'GPRINT:ds3:AVERAGE:   Durchschnitt\:  %3.1lf °C'

rrdtool graph /home/pi/.gbmon/persist/graphs/humidity1h.png               \
--start end-1h                                             \
--title 'Luftfeuchtigkeit'                                 \
--vertical-label '%'                                       \
--width 700 --height 275                                   \
--alt-y-grid                                               \
--interlaced                                               \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                           \
-c FONT#8E8E93                                             \
-c BACK#ffffff                                             \
'DEF:ds1=humidity.rrd:humidity_1:AVERAGE'                  \
'LINE2:ds1#5AC8FA:Luftfeuchtigkeit\n'                      \
'GPRINT:ds1:MAX:   Maximum\:\t%3.1lf %%\n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%3.1lf %%\n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %3.1lf %%'

rrdtool graph /home/pi/.gbmon/persist/graphs/ecLevel1h.png                      \
--start end-1h                                                    \
--title 'eC-Wert'                                     \
--vertical-label '%'                                           \
--width 700 --height 275       \
--alt-y-grid                                                   \
--interlaced                                                   \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                               \
-c FONT#8E8E93                                                 \
-c BACK#ffffff                                                 \
'DEF:ds1=ecLevel.rrd:ecLevel_1:AVERAGE'                     \
'DEF:ds2=ecLevel.rrd:ecLevel_2:AVERAGE'                     \
'DEF:ds3=ecLevel.rrd:ecLevel_3:AVERAGE'                     \
'LINE2:ds1#5AC8FA:eC-Wert 1\t\t\t \t'                                \
'LINE2:ds2#FF2D55:eC-Wert 2\t\t\t \t'                                \
'LINE2:ds3#8E8E93:eC-Wert 3\n'                          \
'GPRINT:ds1:MAX:   Maximum\:\t%4.1lf \t \t'               \
'GPRINT:ds2:MAX:   Maximum\:\t%4.1lf \t \t'               \
'GPRINT:ds3:MAX:   Maximum\:\t%4.1lf \n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%4.1lf \t \t'               \
'GPRINT:ds2:MIN:   Minimum\:\t%4.1lf \t \t'               \
'GPRINT:ds3:MIN:   Minimum\:\t%4.1lf \n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %4.1lf\t'          \
'GPRINT:ds2:AVERAGE:   Durchschnitt\:  %4.1lf\t'          \
'GPRINT:ds3:AVERAGE:   Durchschnitt\:  %4.1lf'

rrdtool graph /home/pi/.gbmon/persist/graphs/lightFan1h.png                     \
--start end-1h                                                   \
--title 'Licht und Ventilator'                                 \
--vertical-label '%'                                           \
--width 700 --height 275                                      \
--alt-y-grid                                                   \
--interlaced                                                   \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                               \
-c FONT#8E8E93                                                 \
-c BACK#ffffff                                                 \
'DEF:line_1=lightFan.rrd:light_1:AVERAGE'                      \
'DEF:line_2=lightFan.rrd:fan_1:AVERAGE'                        \
'AREA:line_1#FF2D55:Licht'                                    \
'AREA:line_2#5AC8FA:Ventilator'

echo '  Last Day'
rrdtool graph /home/pi/.gbmon/persist/graphs/temperature1d.png            \
--start end-1d                                             \
--title 'Temperatur'                                       \
--vertical-label '°C'                                      \
--width 700 --height 275                                   \
--alt-y-grid                                               \
--interlaced                                               \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                           \
-c FONT#8E8E93                                             \
-c BACK#ffffff                                             \
'DEF:ds1=temperature.rrd:temperature_1:AVERAGE'            \
'DEF:ds2=temperature.rrd:temperature_2:AVERAGE'            \
'DEF:ds3=temperature.rrd:temperature_3:AVERAGE'            \
'LINE2:ds1#5AC8FA:Temperatur 1\t\t\t\t'                    \
'LINE2:ds2#FF2D55:Temperatur 2\t\t\t\t'                    \
'LINE2:ds3#8E8E93:Temperatur 3\n'                          \
'GPRINT:ds1:MAX:   Maximum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds2:MAX:   Maximum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds3:MAX:   Maximum\:\t%3.1lf °C\n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds2:MIN:   Minimum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds3:MIN:   Minimum\:\t%3.1lf °C\n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %3.1lf °C'          \
'GPRINT:ds2:AVERAGE:   Durchschnitt\:  %3.1lf °C'          \
'GPRINT:ds3:AVERAGE:   Durchschnitt\:  %3.1lf °C'


rrdtool graph /home/pi/.gbmon/persist/graphs/humidity1d.png               \
--start end-1d                                             \
--title 'Luftfeuchtigkeit'                                 \
--vertical-label '%'                                       \
--width 700 --height 275                                   \
--alt-y-grid                                               \
--interlaced                                               \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                           \
-c FONT#8E8E93                                             \
-c BACK#ffffff                                             \
'DEF:ds1=humidity.rrd:humidity_1:AVERAGE'                  \
'LINE2:ds1#5AC8FA:Luftfeuchtigkeit\n'                      \
'GPRINT:ds1:MAX:   Maximum\:\t%3.1lf %%\n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%3.1lf %%\n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %3.1lf %%'

rrdtool graph /home/pi/.gbmon/persist/graphs/ecLevel1d.png                      \
--start end-1d                                                    \
--title 'eC-Wert'                                     \
--vertical-label '%'                                           \
--width 700 --height 275       \
--alt-y-grid                                                   \
--interlaced                                                   \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                               \
-c FONT#8E8E93                                                 \
-c BACK#ffffff                                                 \
'DEF:ds1=ecLevel.rrd:ecLevel_1:AVERAGE'                     \
'DEF:ds2=ecLevel.rrd:ecLevel_2:AVERAGE'                     \
'DEF:ds3=ecLevel.rrd:ecLevel_3:AVERAGE'                     \
'LINE2:ds1#5AC8FA:eC-Wert 1\t\t\t \t'                                \
'LINE2:ds2#FF2D55:eC-Wert 2\t\t\t \t'                                \
'LINE2:ds3#8E8E93:eC-Wert 3\n'                          \
'GPRINT:ds1:MAX:   Maximum\:\t%4.1lf \t \t'               \
'GPRINT:ds2:MAX:   Maximum\:\t%4.1lf \t \t'               \
'GPRINT:ds3:MAX:   Maximum\:\t%4.1lf \n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%4.1lf \t \t'               \
'GPRINT:ds2:MIN:   Minimum\:\t%4.1lf \t \t'               \
'GPRINT:ds3:MIN:   Minimum\:\t%4.1lf \n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %4.1lf\t'          \
'GPRINT:ds2:AVERAGE:   Durchschnitt\:  %4.1lf\t'          \
'GPRINT:ds3:AVERAGE:   Durchschnitt\:  %4.1lf'

rrdtool graph /home/pi/.gbmon/persist/graphs/lightFan1d.png                     \
--start end-1d                                                   \
--title 'Licht und Ventilator'                                 \
--vertical-label '%'                                           \
--width 700 --height 275                                     \
--alt-y-grid                                                   \
--interlaced                                                   \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                               \
-c FONT#8E8E93                                                 \
-c BACK#ffffff                                                 \
'DEF:line_1=lightFan.rrd:light_1:AVERAGE'                      \
'DEF:line_2=lightFan.rrd:fan_1:AVERAGE'                        \
'AREA:line_1#FF2D55:Licht'                                    \
'AREA:line_2#5AC8FA:Ventilator'

echo '  Last Week'
rrdtool graph /home/pi/.gbmon/persist/graphs/temperature1w.png            \
--start end-1w                                             \
--title 'Temperatur'                                       \
--vertical-label '°C'                                      \
--width 700 --height 275                                   \
--alt-y-grid                                               \
--interlaced                                               \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                           \
-c FONT#8E8E93                                             \
-c BACK#ffffff                                             \
'DEF:ds1=temperature.rrd:temperature_1:AVERAGE'            \
'DEF:ds2=temperature.rrd:temperature_2:AVERAGE'            \
'DEF:ds3=temperature.rrd:temperature_3:AVERAGE'            \
'LINE2:ds1#5AC8FA:Temperatur 1\t\t\t\t'                    \
'LINE2:ds2#FF2D55:Temperatur 2\t\t\t\t'                    \
'LINE2:ds3#8E8E93:Temperatur 3\n'                          \
'GPRINT:ds1:MAX:   Maximum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds2:MAX:   Maximum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds3:MAX:   Maximum\:\t%3.1lf °C\n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds2:MIN:   Minimum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds3:MIN:   Minimum\:\t%3.1lf °C\n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %3.1lf °C'          \
'GPRINT:ds2:AVERAGE:   Durchschnitt\:  %3.1lf °C'          \
'GPRINT:ds3:AVERAGE:   Durchschnitt\:  %3.1lf °C'


rrdtool graph /home/pi/.gbmon/persist/graphs/humidity1w.png               \
--start end-1w                                             \
--title 'Luftfeuchtigkeit'                                 \
--vertical-label '%'                                       \
--width 700 --height 275                                   \
--alt-y-grid                                               \
--interlaced                                               \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                           \
-c FONT#8E8E93                                             \
-c BACK#ffffff                                             \
'DEF:ds1=humidity.rrd:humidity_1:AVERAGE'                  \
'LINE2:ds1#5AC8FA:Luftfeuchtigkeit\n'                      \
'GPRINT:ds1:MAX:   Maximum\:\t%3.1lf %%\n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%3.1lf %%\n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %3.1lf %%'

rrdtool graph /home/pi/.gbmon/persist/graphs/ecLevel1w.png                      \
--start end-1w                                                    \
--title 'eC-Wert'                                     \
--vertical-label '%'                                           \
--width 700 --height 275       \
--alt-y-grid                                                   \
--interlaced                                                   \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                               \
-c FONT#8E8E93                                                 \
-c BACK#ffffff                                                 \
'DEF:ds1=ecLevel.rrd:ecLevel_1:AVERAGE'                     \
'DEF:ds2=ecLevel.rrd:ecLevel_2:AVERAGE'                     \
'DEF:ds3=ecLevel.rrd:ecLevel_3:AVERAGE'                     \
'LINE2:ds1#5AC8FA:eC-Wert 1\t\t\t \t'                                \
'LINE2:ds2#FF2D55:eC-Wert 2\t\t\t \t'                                \
'LINE2:ds3#8E8E93:eC-Wert 3\n'                          \
'GPRINT:ds1:MAX:   Maximum\:\t%4.1lf \t \t'               \
'GPRINT:ds2:MAX:   Maximum\:\t%4.1lf \t \t'               \
'GPRINT:ds3:MAX:   Maximum\:\t%4.1lf \n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%4.1lf \t \t'               \
'GPRINT:ds2:MIN:   Minimum\:\t%4.1lf \t \t'               \
'GPRINT:ds3:MIN:   Minimum\:\t%4.1lf \n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %4.1lf\t'          \
'GPRINT:ds2:AVERAGE:   Durchschnitt\:  %4.1lf\t'          \
'GPRINT:ds3:AVERAGE:   Durchschnitt\:  %4.1lf'

rrdtool graph /home/pi/.gbmon/persist/graphs/lightFan1w.png                     \
--start end-1w                                                   \
--title 'Licht und Ventilator'                                 \
--vertical-label '%'                                           \
--width 700 --height 275       \
--alt-y-grid                                                   \
--interlaced                                                   \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                               \
-c FONT#8E8E93                                                 \
-c BACK#ffffff                                                 \
'DEF:line_1=lightFan.rrd:light_1:AVERAGE'                      \
'DEF:line_2=lightFan.rrd:fan_1:AVERAGE'                        \
'AREA:line_1#FF2D55:Licht'                                    \
'AREA:line_2#5AC8FA:Ventilator'

echo '  Last Month'
rrdtool graph /home/pi/.gbmon/persist/graphs/temperature1m.png            \
--start end-4w                                             \
--title 'Temperatur'                                       \
--vertical-label '°C'                                      \
--width 700 --height 275                                   \
--alt-y-grid                                               \
--interlaced                                               \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                           \
-c FONT#8E8E93                                             \
-c BACK#ffffff                                             \
'DEF:ds1=temperature.rrd:temperature_1:AVERAGE'            \
'DEF:ds2=temperature.rrd:temperature_2:AVERAGE'            \
'DEF:ds3=temperature.rrd:temperature_3:AVERAGE'            \
'LINE2:ds1#5AC8FA:Temperatur 1\t\t\t\t'                    \
'LINE2:ds2#FF2D55:Temperatur 2\t\t\t\t'                    \
'LINE2:ds3#8E8E93:Temperatur 3\n'                          \
'GPRINT:ds1:MAX:   Maximum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds2:MAX:   Maximum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds3:MAX:   Maximum\:\t%3.1lf °C\n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds2:MIN:   Minimum\:\t%3.1lf °C\t\t'               \
'GPRINT:ds3:MIN:   Minimum\:\t%3.1lf °C\n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %3.1lf °C'          \
'GPRINT:ds2:AVERAGE:   Durchschnitt\:  %3.1lf °C'          \
'GPRINT:ds3:AVERAGE:   Durchschnitt\:  %3.1lf °C'


rrdtool graph /home/pi/.gbmon/persist/graphs/humidity1m.png               \
--start end-4w                                             \
--title 'Luftfeuchtigkeit'                                 \
--vertical-label '%'                                       \
--width 700 --height 275                                   \
--alt-y-grid                                               \
--interlaced                                               \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                           \
-c FONT#8E8E93                                             \
-c BACK#ffffff                                             \
'DEF:ds1=humidity.rrd:humidity_1:AVERAGE'                  \
'LINE2:ds1#5AC8FA:Luftfeuchtigkeit\n'                      \
'GPRINT:ds1:MAX:   Maximum\:\t%3.1lf %%\n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%3.1lf %%\n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %3.1lf %%'

rrdtool graph /home/pi/.gbmon/persist/graphs/ecLevel1m.png                      \
--start end-4w                                                    \
--title 'eC-Wert'                                     \
--vertical-label '%'                                           \
--width 700 --height 275       \
--alt-y-grid                                                   \
--interlaced                                                   \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                               \
-c FONT#8E8E93                                                 \
-c BACK#ffffff                                                 \
'DEF:ds1=ecLevel.rrd:ecLevel_1:AVERAGE'                     \
'DEF:ds2=ecLevel.rrd:ecLevel_2:AVERAGE'                     \
'DEF:ds3=ecLevel.rrd:ecLevel_3:AVERAGE'                     \
'LINE2:ds1#5AC8FA:eC-Wert 1\t\t\t \t'                                \
'LINE2:ds2#FF2D55:eC-Wert 2\t\t\t \t'                                \
'LINE2:ds3#8E8E93:eC-Wert 3\n'                          \
'GPRINT:ds1:MAX:   Maximum\:\t%4.1lf \t \t'               \
'GPRINT:ds2:MAX:   Maximum\:\t%4.1lf \t \t'               \
'GPRINT:ds3:MAX:   Maximum\:\t%4.1lf \n'                 \
'GPRINT:ds1:MIN:   Minimum\:\t%4.1lf \t \t'               \
'GPRINT:ds2:MIN:   Minimum\:\t%4.1lf \t \t'               \
'GPRINT:ds3:MIN:   Minimum\:\t%4.1lf \n'                 \
'GPRINT:ds1:AVERAGE:   Durchschnitt\:  %4.1lf\t'          \
'GPRINT:ds2:AVERAGE:   Durchschnitt\:  %4.1lf\t'          \
'GPRINT:ds3:AVERAGE:   Durchschnitt\:  %4.1lf'

rrdtool graph /home/pi/.gbmon/persist/graphs/lightFan1m.png                     \
--start end-4w                                                   \
--title 'Licht und Ventilator'                                 \
--vertical-label '%'                                           \
--width 700 --height 275       \
--alt-y-grid                                                   \
--interlaced                                                   \
--font 'DEFAULT:13:/usr/share/fonts/macfonts/helvetica_neue_ultralight.ttf' \
-c CANVAS#ffffff                                               \
-c FONT#8E8E93                                                 \
-c BACK#ffffff                                                 \
'DEF:line_1=lightFan.rrd:light_1:AVERAGE'                      \
'DEF:line_2=lightFan.rrd:fan_1:AVERAGE'                        \
'AREA:line_1#FF2D55:Licht'                                    \
'AREA:line_2#5AC8FA:Ventilator'

echo '\n'
echo 'Transfering Graphs to Server ...'
sshpass -p 'alpine' scp /home/pi/.gbmon/persist/graphs/*.png service@192.168.178.10:./Sites/MacServer/gbmon/graphs/
echo 'Transfering Images to Server ...'
sshpass -p 'alpine' scp /home/pi/.gbmon/pics/*.jpg service@192.168.178.10:./Sites/MacServer/gbmon/pics/
