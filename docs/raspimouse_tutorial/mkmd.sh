#!/usr/bash
readonly LOGFILE="/tmp/tillog/${0##*/}.log"
readonly PROCNAME=${0##*/}

function log() {
  local fname=${BASH_SOURCE[1]##*/}
  echo -e "$(date '+%Y-%m-%dT%H:%M:%S') ${PROCNAME} (${fname}:${BASH_LINENO[0]}:${FUNCNAME[1]}) $@" | tee -a ${LOGFILE}
}

function isHoliday(){
    todayis=`date "+%Y-%m-%d"`
    holiday_list=`curl -sS https://raw.githubusercontent.com/holiday-jp/holiday_jp-ruby/master/holidays.yml | awk '{print $1}'`
    echo ${holiday_list} | grep ${todayis} > /dev/null

    if [ ${?} -eq 0 ] ; then
        # 土日祝日
        #echo "holiday"
        echo "0"
    else
        # 平日
        #echo "normal-day"
        echo "1"
    fi
}
cd "/home/gisen/git/ros_study_airly/raspimouse_tutorial/"
today=`date '+%Y%m%d'`
filename=$today".md"
isH=`isHoliday`

if [[ ! -e $filename ]] && [ $isH = "1" ]; then
    echo "make mdfile"
    if [[ -e "201907XX.md" ]]; then
        cmd=`cp 201907XX.md $filename` 
    fi
fi