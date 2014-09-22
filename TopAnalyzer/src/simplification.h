#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

#include "data.h"
#include "mesh.h"

extern "C"
{
#include <tourtre/tourtre.h>
#include "featureset.h"
}

/**
   Depth first search simplification. The algorithm follows the tree until it hits a leaf branch.
   When a leaf branch is found, its importance is calculated via the "importance_measure" callback
   and if the calculated importance is lesser than a given threshold, the branch is marked for
   removal.
*/
void simplify_tree_dfs(ctBranch*, ctBranch**, size_t, double (*importance_measure)(ctBranch*), double);

//void simplify_tree_bfs(ctBranch*, double (*importance_measure)(ctBranch*), double);

#endif // SIMPLIFICATION_H
