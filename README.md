# Key-Rank-Estimation-Algorithms
This repository contains the implementations in c language, of Key Rank Estimation Algorithms presented in the paper *"Tighter, faster, simpler side-channel security evaluations beyond computing power"* by *Bernstein et al.*. 
The algorithms aim to estimate the rank of a secret key used during a block cipher, given different scores for all possible subkeys. Our implementations require that these scores be stored in a file with the following format :
- First line : number_of_dimensions_or_subkeys number_of_possibilities_per_dimension
- Next lines : One dimension (or subkey) per line, each line containing probability for each possible subkey, separated by a whitespace 

# Scores_ASCAD
This folder contains probabilities computed from a Correlation Power Analysis Attack on the ASCAD Database from *"Study of Deep Learning Techniques for Side-Channel Analysis and Intro"* by *Benadjila et al.*, with different numbers of traces, resulting in different ranks for the secret key in each file. The secret key is :
``` 
{0x4d, 0xfb, 0xe0, 0xf2, 0x72, 0x21, 0xfe, 0x10, 0xa7, 0x8d, 0x4a, 0xdc, 0x8e, 0x49, 0x04, 0x69}
```
The filenames are of the format "scores_cpa_i" where i is the number of traces used for this instance of the attack

# PRO (Polynomial Rank Outlining Algorithm)
The source code and a Makefile for this algorithm are in the folder **PRO**. To run the algorithm, use the command 
```
./Test name_of_scores_file
```
**Test.c** contains the main program. To modify the accuracy parameters and the secret key, you can modify them directly from the file. The program prints at the end the interval containing the secret key rank (in log_2 format).

# REA & EREA (Rank Estimation Algorithm, Extended Rank Estimation Algorithm)
The source code and a Makefile for this algorithm are in the folder **EREA**. To run the algorithm, use the command 
```
./Test name_of_scores_file running_time_in_seconds_for_REA
```
**Test.c** contains the main program. To modify the accuracy parameters, the sampling number and the secret key, you can modify them directly from the file. The program prints at the end the interval containing the secret key rank (in log_2 format) from the **REA** output, and prints intervals with different confidence values for the key rank using **EREA** statistical approach. Please refer to  *Bernstein et al.*'s mains article for more details.
