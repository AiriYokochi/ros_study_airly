#!/usr/bash

# ERROR
set -E

trap catch ERR
trap interpret SIGINT
trap stoped SIGTSTP

today=`date '+%Y%m%d'`
MY_BASENAME=$(basename $0)
MY_ABS_PATH=$(cd $(dirname $0); pwd)/$MY_BASENAME
DEFAULT_FILE_NAME=$today".md"

catch() {
  echo -n 'failed to coping:'
  echo CATCH
  echo "[ERROR] please retry"
  exit 1
}
interpret() {
  echo .
  echo '[WARN] sigint '${MY_BASENAME}' may not success'
  exit 1
}
stoped(){
  echo .
  echo '[WARN] sigstop '${MY_BAENAME}' may not success'
  exit 1
}

# LOG
LOGFILE="../../push_log/git_add_commit_push.log"
readonly PROCNAME=${0##*/}
function log() {
  local fname=${BASH_SOURCE[1]##*/}
  echo -e "$(date '+%Y-%m-%d %H:%M:%S')  (${fname}:${BASH_LINENO[0]}:${FUNCNAME[1]}) $@" | tee -a ${LOGFILE} # ${PROCNAME}
}

USER_NAME="AiriYokochi"
GITHUB_PASS=$(</home/gisen/git/pass.txt)

ask_yN(){
    while true; do
        read -p "(Y/N): " yN
        case "$yN" in 
            N)     return 1;;
            [yY]*) return 0;;
            *)     return 0;;
        esac
    done
}

push_func(){
    echo "push..."
    expect -c "
    spawn git push
    expect \"Username for 'https://github.com':\"
    send -- \"$USER_NAME\n\"
    expect \"Password for 'https://AiriYokochi@github.com':\"
    send -- \"$GITHUB_PASS\n\"
    "
    #echo "return :"$?
    echo ""
    echo "---------------------------------------------"
    log "[done] git push"
    echo "[warn] check git push is really done"
    echo "---------------------------------------------"
    return 0
}

add_func(){
    git add .
    echo "---------------------------------------------"
    log "[done] git add ."
    echo "---------------------------------------------"
    if [ $? = 0 ]; then
        return 0
    else
        return 1
    fi
}

# commit function
commit_func(){
    add_func
    echo "generate git commit message"
    echo -n "[1] new_file [2] add_exist_file [3] other :"
    while :
        do 
            read num
            case $num in
            1)  echo -n "put_filename "
                echo -n "(or default name :"$DEFAULT_FILE_NAME") :"
                read file_name
                if [ "$file_name" = ""  ]; then
                    file_name=$DEFAULT_FILE_NAME
                fi
                echo -n "put_describe if u hav:"
                read describe
                echo -n "message: "$file_name"の追加 "$describe
                ask_yN
                ret=$?
                case ${ret} in
                    0)  
                        git commit -m $file_name"の追加 ""$describe"
                        log "[done]commit with message: "$file_name"の追加 "$describe
                        echo "---------------------------------------------"
                        break
                        ;;
                    1) "please try again"
                       continue ;;
                esac
                ;;
            2)  echo -n "put_filename"
                echo -n "(or default name :"$DEFAULT_FILE_NAME") :"
                read file_name
                if [ "$file_name" = ""  ]; then
                    file_name=$DEFAULT_FILE_NAME
                fi
                echo -n "put_describe (e.g. ROS14 ROS15) :"
                read describe
                echo -n "message: "$file_name"の追記 "$describe
                ask_yN
                ret=$?
                case ${ret} in
                    0)  
                        git commit -m $file_name"の追記 ""$describe"
                        log "[done]commit with message: "$file_name"の追記 "$describe
                        echo "---------------------------------------------"
                        break
                        ;;
                    1) "please try again"
                       continue ;;
                esac
                ;;
            3)  echo -n "put what 2 u do :"
                read other 
                echo -n "message: "$other
                ask_yN
                ret=$?
                case ${ret} in
                    0)  
                        git commit -m "$other"
                        log "[done]commit with message: ""$other"
                        echo "---------------------------------------------"
                        break
                        ;;
                    1) "please try again"
                       continue ;;
                esac
            ;;
            q)   echo "quit..."
                    break;;
            * )   echo -n "put number or q to quit :"
                ;;
            esac
        done

    if [ "$1" = "1" ]; then
        push_func
    fi
}

# commitをなかったことにする
# git reset --soft HEAD^

extended_func(){
    # 色を黄色にする
    echo "---------------------------------------------"


    while :
    do
    echo -n "type [a]commit reset [h]help(none) [q]quit :"
    read cmd
        case "$cmd" in
        "a")    echo "git reset --soft HEAD^ "
                ask_yN
                ret=$?
                case ${ret} in
                    0)  
                        git reset --soft HEAD^
                        break
                        ;;
                    1)  echo "canseled commit reset"
                        break
                        ;;
                esac 
                ;;
        "h")    echo "will be..."
                continue;;
        "q")    echo "quit..."
                echo "---------------------------------------------"
                read_num_func
                break
                ;;
        *)      echo -n "type [a]commit reset [h]help(none)  :"
                continue
                ;;
        esac
    done

}

read_num_func(){
    echo "enter number                                    "
    echo -n "[1]add. [2]add_to_cm [3]add_to_push [4]extend :"

    while :
    do 
        read num
        case "$num" in
        "1" )   add_func
                break
                ;;
        "2" )   commit_func 0
                break
                ;;
        "3" )   commit_func 1
                break
                ;;
        "4" )   extended_func
                break
                ;;
        "q" )   echo "quit..."
                break
                ;;
        "*" )   echo -n "put number or q to quit :"
                ;;
        esac
    done

    return 0

}


echo "---------------------------------------------"
echo "  * git_airly ver2.0 * 2019.07.12            "
echo "---------------------------------------------"

if [ "$1" = "" ]; then
    read_num_func
else
    case "$1" in
    1)  add_func 
        ;;
    2)  commit_func 0
        ;;
    3)  commit_func 1
        ;;
    *)  echo "[ERROR] unknown param try again"
        ;;
    esac
fi

exit 0