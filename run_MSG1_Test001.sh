#!/bin/bash

# TEST_LOG=eNB_MSG1_Test_001.log
# HOME_PATH=`pwd`
# PHY_PATH=../exec
# AS_PATH=../exec
# CTRL_PATH=../exec
# CTRL_SCRIPT_PATH=../exec
# MY_PASSWORD=123456
#MAC_COMMAND="--rachrepeat 0 --rachperiod 3 --rachstart 1"      #{1 2 4 8 16 32 64 128]
# echo "Shell Script:"
# echo $1 $2 $3 $4 $5 $6 $7
# exit 0;
MAC_COMMAND="--rachperiod ${1} --rachstart ${6} --rachrepeat ${2} --rachscofst 0 --rachnumsc 2 --rmax ${3} --gvalue ${4} --candidate ${7} --dlcqi 7 --rartbsindex 3 --rarsfindex 1 --dlrepeat 0 --s2tbs 2" #case1
#MAC_COMMAND="--rachperiod 6 --rachstart 0 --rachrepeat 4 --rachscofst 0 --rachnumsc 0 --rmax 5 --gvalue 2 --candidate 8 --dlcqi 4 --rartbsindex 3 --rarsfindex 1 --dlrepeat 0 --s2tbs 2" #case2
#MAC_COMMAND="--rachperiod 3 --rachstart 1 --rachrepeat 0 --rachscofst 1 --rachnumsc 0 --rmax 2 --gvalue 4 --candidate 2 --dlcqi 8 --rartbsindex 3 --rarsfindex 1 --dlrepeat 0 --s2tbs 2" #case3
#MAC_COMMAND="--rachperiod 0 --rachstart 1 --rachrepeat 1 --rachscofst 2 --rachnumsc 1 --rmax 2 --gvalue 3 --candidate 2 --dlcqi 12 --rartbsindex 3 --rarsfindex 1 --dlrepeat 0 --s2tbs 2" #case4
#MAC_COMMAND="--rachperiod 5 --rachstart 6 --rachrepeat 2 --rachscofst 6 --rachnumsc 0 --rmax 4 --gvalue 3 --candidate 4 --dlcqi 5 --rartbsindex 3 --rarsfindex 1 --dlrepeat 0 --s2tbs 2" #case5
#MAC_COMMAND="--rachperiod 7 --rachstart 7 --rachrepeat 3 --rachscofst 0 --rachnumsc 3 --rmax 5 --gvalue 4 --candidate 8 --dlcqi 4 --rartbsindex 3 --rarsfindex 1 --dlrepeat 0 --s2tbs 2" #case6
#MAC_COMMAND="--rachperiod 6 --rachstart 0 --rachrepeat 4 --rachscofst 0 --rachnumsc 0 --rmax 3 --gvalue 4 --candidate 8 --dlcqi 4 --rartbsindex 3 --rarsfindex 1 --dlrepeat 0 --s2tbs 2" #case7
#Check if MAC_COMMAND is input correctly done.
echo "MAC_COMMAND=${MAC_COMMAND}"
mkdir -p Result
BASEDIR=$(dirname $0)
# echo $BASEDIR
cp ShellCopy.sh Result
cd Result
mv ShellCopy.sh run_MSG1_Test${8}.sh
cat run_MSG1_Test${8}.sh |  sed "14c MAC_COMMAND=\"$MAC_COMMAND\"" > log.sh
# rm run_MSG1_Test${8}.sh
mv log.sh run_MSG1_Test${8}.sh
exit 0;
#Check if MAC_COMMAND is input correctly done.

# <<============= PHY Norml =====================
echo "startup III eNB PHY..."
gnome-terminal -t PHY -x bash -c "cd $PHY_PATH; echo $MY_PASSWORD | sudo -S ./init_iiirf.sh; sudo ./lte-softmodem $PHY_COMMAND; read -p \"Press any key to close terminal\""
# =============================================>>
sleep 1.5
#read -p "Press any key to start eNB AS..."
echo "startup III eNB AS..."
gnome-terminal -t eNB_AS -x bash -c "cd $AS_PATH; echo $MY_PASSWORD | sudo -S stdbuf -o 0 ./enb_as $MAC_COMMAND | tee $HOME_PATH/Test_Log/$TEST_LOG; read -p \"Press any key to close terminal\""
sleep 1
#read -p "Press any key to start eNB Ctrl..."
echo "startup III eNB Ctrl..."
gnome-terminal -t eNB_CTRL -x bash -c "cd $CTRL_PATH; echo $MY_PASSWORD | sudo -S ./enbctrl; read -p \"Press any key to close terminal\""
sleep 1
#echo "running III eNB Ctrl script..."
gnome-terminal -t eNB_CTRL_script -x bash -c "cd $CTRL_SCRIPT_PATH; ./enbctrl_script"
cd $HOME_PATH
