#ifndef EXPERIMENTS_TWM_EXPERIMENT_RESULT_H
#define EXPERIMENTS_TWM_EXPERIMENT_RESULT_H

#include <twm/Board.h>
#include <twm/Problem.h>

class ExperimentResult {
   private:
    twm::Problem problem;
    twm::Board start_board;

   public:
    ExperimentResult();
    virtual ~ExperimentResult();
};

#endif /* EXPERIMENTS_TWM_EXPERIMENT_RESULT_H */
