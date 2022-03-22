

#FILE_TXT=generated_$1.txt

declare -A MODELS
MODELS=( ["DanBrown"]="lm_lstm_epoch16.25_0.6320.t7_cpu.t7"
	 ["TerryPratchett"]="lm_lstm_epoch11.21_0.6664.t7_cpu.t7"
       )

while true 
do
RND_STRING=`cat /dev/urandom | tr -dc \'a-zA-Z0-9\' | fold -w 32 | head -n 1`
RND_STRING=${RND_STRING//\'}
echo RND_STRING = $RND_STRING 

#th sample.lua /home/sgeorgiev/ml/char-rnn/cv/lm_lstm_epoch16.25_0.6320.t7 -length 10000 -primetext $RND_STRING > generated.txt
 

for NAME in "${!MODELS[@]}";
do
    FILE_TXT=generated_$NAME.txt
    echo l0 > $FILE_TXT
    echo l1 >> $FILE_TXT
    th sample.lua /home/sgeorgiev/ml/char-rnn/cv/${MODELS[$NAME]} -length 10000 -gpuid -1 -primetext $RND_STRING >> $FILE_TXT 
done

sleep 300

done
