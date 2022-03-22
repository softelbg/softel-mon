



declare -a LOGS=("vmkernel" "hostd" "hostd-probe" "vobd");

for LOG in "${LOGS[@]}";
do

  echo "PROCESS $LOG";

  for i in `seq 0 999`;
  do
    FILE=$LOG.$i.gz;
    if [ ! -f $FILE ]; then
      #echo breaking on file = $FILE;
      break
    fi
    echo file = $FILE;
    gzip -d $FILE;
  done

  LOG_ALL=$LOG-all.log

  rm -f $LOG_ALL

  for i in `seq -s " " 0 999|rev`;
  do
    FILE=$LOG.$i;
    if [ ! -f $FILE ]; then
      continue
    fi
    echo file = $FILE;
    cat $FILE >> $LOG_ALL;
  done
  FILE=$LOG;
  if [ -f $FILE ]; then
      cat $FILE >> $LOG_ALL;
  fi

done
