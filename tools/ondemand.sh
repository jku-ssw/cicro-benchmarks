#! /bin/bash
#
# ondemand sets cpu govermor
#
# chkconfig: 2345 10 90
#
# description: Set the CPU Frequency Scaling governor to "performance"
#
### BEGIN INIT INFO
# Provides: $ondemand
### END INIT INFO

PATH=/sbin:/usr/sbin:/bin:/usr/bin

case "$1" in
    start)
        for CPUFREQ in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
        do
                [ -f $CPUFREQ ] || continue
                echo -n performance > $CPUFREQ
        done
        ;;
    restart|reload|force-reload)
        echo "Error: argument '$1' not supported" >&2
        exit 3
        ;;
    stop)
        for CPUFREQ in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
        do
                [ -f $CPUFREQ ] || continue
                echo -n powersave > $CPUFREQ
        done
        ;;
    *)
        echo "Usage: $0 start|stop" >&2
        exit 3
        ;;
esac
