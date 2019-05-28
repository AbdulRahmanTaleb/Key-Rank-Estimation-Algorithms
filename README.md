# Key-Rank-Estimation-Algorithms
This repository contains the implementations in c language, of Key Rank Estimation Algorithms presented in the paper *"Tighter, faster, simpler side-channel security evaluations beyond computing power"* by *Bernstein et al.*. 
The algorithms aim to estimate the rank of a secret key used during a block cipher, given different scores for all possible subkeys. Our implementations require that these scores be stored in a file with the following format :
- First line : number_of_dimensions_or_subkeys number_of_possibilities_per_dimension
- Next lines : One dimension (or subkey) per line, each line containing probabilities for each possible subkey 

# Scores_ASCAD
This folder contains probabilities computed from a Correlation Power Analysis Attack on the ASCAD Database, with different numbers of traces, resulting in different ranks for the secret key in each file. The secret key is :
``` 
{0x4d, 0xfb, 0xe0, 0xf2, 0x72, 0x21, 0xfe, 0x10, 0xa7, 0x8d, 0x4a, 0xdc, 0x8e, 0x49, 0x04, 0x69}
```
The filenames are of the format "scores_cpa_i" where i is the number of traces used for this instance of the attack
