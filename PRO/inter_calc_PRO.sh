#! /bin/bash

START=$SECONDS
for i in {10..62}; do
	./Test "/Vrac/3520903/Scores_ASCAD/scores_cpa_$i"
done
END=$SECONDS
T=$(($END - $START))

echo "DONE Time = $T"
