export DOMOS_ROOT="${PWD/\/domos\/*//domos}";

if [[ "$DOMOS_ROOT" == *"domos"* ]];
then
    alias domos="python $DOMOS_ROOT/dx/cli";
else
    unalias domos;
    unset DOMOS_ROOT;
    echo "Please only run this within the domos folder.";
fi
