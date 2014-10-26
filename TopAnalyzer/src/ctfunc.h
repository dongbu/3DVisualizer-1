#ifndef CTFUNC_H
#define CTFUNC_H

#include "topdata.h"
#include "topmesh.h"
#include "featureset.h"

extern "C"
{
#include <tourtre.h>
}

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum
{
    LINEAR,     // y = ax + b.
    HAT,        //0.45 to 0.55 is different than 0. Max at 0.5.
    TRIANGLE,   //Linear grow up to 0.5 linear decrease afterwards.
    TRAPEZOID,  //Growth until 0.45. Max value to the end.
    HTRAPEZOID  //Half trapezoid
} TFShape;

size_t count_branches(ctBranch*);
void calc_branch_features(ctBranch**, top::Dataset*);
double calc_avg_importance(ctBranch*, double (*importance_measure)(ctBranch*));
void calc_branch_depth(ctBranch*, size_t*, size_t);
void calc_branch_num_children(ctBranch*);
void calc_centroid(ctBranch*, top::Dataset*);
void normalize_features(ctBranch*);
int label_branches(ctBranch*);

/**
 * @brief zero_branches - Terrible workaround to guarantee that the data field of
 * a branch will always be zero before the importance calculations.
 * @param root_branch - The root of the branch decomposition.
 */
void zero_branches(ctBranch*);
size_t save_vertex_branch_volume(ctBranch**, std::string, size_t, size_t, size_t);
void calc_vertices_branch(ctBranch*, ctBranch**, size_t);
void rebuild_branch_map(ctBranch*, ctBranch**, size_t);

//Functions - @Netto
void calc_residue_flow(ctBranch* root_branch, double alpha_d, double rate_Q, top::Dataset* data);
void calc_saddle_min_max(ctBranch* root_branch, top::Dataset* data);
double calc_gsd(ctBranch* b, top::Dataset* data);
double half_std_avg_importance_normalized(ctBranch*);
double calc_alpha_sum(ctBranch* b);
double* calc_final_alpha(ctBranch* b, TFShape shape);

//Callback functions used on my implementation.
double std_avg_importance(ctBranch*);
double std_log_importance(ctBranch*);

//Callback functions provided to libtourtre.
double std_value(size_t, void*);
size_t std_neighbors(size_t, size_t*, void*);
void vertex_proc(size_t, ctArc*, void*);
void arc_merge_proc(ctArc*, ctArc*, void*);
double arc_priority_proc(ctNode*, void*);

#ifdef  __cplusplus
}
#endif

#endif // CTFUNC_H
