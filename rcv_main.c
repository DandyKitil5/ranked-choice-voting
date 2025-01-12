#include "rcv.h"
int main(int argc, char *argv[]){
    if(argc == 2) {
        tally_t *tally = tally_from_file(argv[1]);
        if(tally != NULL) {
            tally_election(tally);
            tally_free(tally);
        }
        else {
            printf("Could not load votes file. Exiting with error code 1\n");
            return 1;
        }
    }
    else if(argc == 4) {
        LOG_LEVEL = atoi(argv[2]);
        tally_t *tally = tally_from_file(argv[3]);
        if(tally != NULL) {
            tally_election(tally);
            tally_free(tally);
        }
        else{
            printf("Could not load votes file. Exiting with error code 1\n");
            return 1;
        }

    }
}