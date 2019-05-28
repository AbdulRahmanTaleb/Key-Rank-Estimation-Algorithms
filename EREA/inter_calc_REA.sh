#! /bin/bash

for i in {39..62}; do
	./Test "/Vrac/3520903/Scores_ASCAD/scores_cpa_$i" 100
done

echo "DONE"
