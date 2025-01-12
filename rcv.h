#pragma once

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>             // for variadic functions in testing

#define MAX_CANDIDATES 128
#define MAX_NAME       128

typedef struct vote_node {             // Vote data type: single voter preferences of candidates
  int id;                              // ID of the ballot for this vote
  int pos;                             // index of currently selected candidate
  int candidate_order[MAX_CANDIDATES]; // array of candidate preferences for this vote
  struct vote_node *next;              // pointer to the next vote in a list of votes or NULL
} vote_t;

typedef struct {                                  // Tally data type: votes associated with all candidates
  int candidate_count;                            // total candidates in the election, length of various arrays below
  char candidate_names[MAX_CANDIDATES][MAX_NAME]; // names of each candidate
  char candidate_status[MAX_CANDIDATES];          // flags for each candidate, on of UNKNOWN, LIVE, DROPPED
  int candidate_vote_counts[MAX_CANDIDATES];      // length of vot lists associated with each candidate
  vote_t *candidate_votes[MAX_CANDIDATES];        // pointers linked lists of votes for each candidate
  vote_t *invalid_votes;                          // list of votes that are invalid: no live candidate is ranked
  int invalid_vote_count;                         // length of invalid_vote list
} tally_t;

#define NO_CANDIDATE   -1       // used to indicate no preference of candidate in vote->candidate_order[]

// STATUS of candidates in an election
#define CAND_UNKNOWN  0          // likely uninitialized
#define CAND_ACTIVE   1          // still in the running
#define CAND_MINVOTES 2          // minimum votes detected, likely drop 
#define CAND_DROPPED  3          // candidate removed during a round of voting

// CONDITION of an election returned by the tally_condition() function
#define TALLY_ERROR    1         // something is wrong with the vote counts
#define TALLY_WINNER   2         // single active candidate who is the winner
#define TALLY_TIE      3         // an all-way tie which ends the election
#define TALLY_CONTINUE 4         // another round can be applied ot the tally

// Values for the LOG_LEVEL global variable to trigger verbose printing of log messages
#define LOG_DROP_MINVOTES  1
#define LOG_MINVOTE        2
#define LOG_SHOWVOTES      3
#define LOG_VOTE_TRANSFERS 4
#define LOG_FILEIO         5

// rcv_funcs.c
extern int LOG_LEVEL;
void vote_print(vote_t *vote);
int vote_next_candidate(vote_t *vote, char *candidate_status);
void tally_print_table(tally_t *tally);
void tally_set_minvote_candidates(tally_t *tally);
int tally_condition(tally_t *tally);
vote_t *vote_make_empty();
void tally_add_vote(tally_t *tally, vote_t *vote);
void tally_print_votes(tally_t *tally);
void tally_free(tally_t *tally);
void tally_transfer_first_vote(tally_t *tally, int candidate_index);
void tally_drop_minvote_candidates(tally_t *tally);
void tally_election(tally_t *tally);
tally_t *tally_from_file(char *fname);
