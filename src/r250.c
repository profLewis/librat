#include <stdlib.h>

int seed_randomise(int i){
    srand(i);
    return(1);
}


double Random(void){
  return(1.*rand()/(RAND_MAX+1.0));
}
