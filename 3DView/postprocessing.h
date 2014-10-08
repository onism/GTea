#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

class PostProcessing
{
public:
    PostProcessing();
    int post_solutions(char * file);
    void SolutionMinMaxVector_2D(int zone_no,int sol_no,int SolLocation1, int SolLocation2, double *MinValueSol, double *MaxValueSol);
};

#endif // POSTPROCESSING_H
