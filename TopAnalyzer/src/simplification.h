#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

#include "topdata.h"
#include "topmesh.h"
#include "featureset.h"

extern "C"
{
#include <tourtre.h>
}

/**
 * Depth first search simplification. The algorithm follows the tree until it hits a leaf branch.
 * When a leaf branch is found, its importance is calculated via the "importance_measure" callback
 * and if the calculated importance is lesser than a given threshold, the branch is marked for
 * removal.
*/
//void simplify_tree_dfs(ctContext*, ctBranch*, ctBranch**, size_t, double (*importance_measure)(ctBranch*), double)

//DEPRECATED void simplify_from_branchmap(ctBranch**, size_t, double(*importance_measure)(ctBranch*), double);

/**
 * @brief topSimplifyTree
 * This function applies the simplification operation on the contour tree.
 * 
 * The simplification is follows a simple algorithm:
 * do {
 *   queue all unvisited leaves of the contour tree.
 *   sort the leaves by their importance.
 *   find the first leaf with an importance lower than the threshold.
 *   for every leaf after this point
 *     merge their features with their parents' features
 *     delete the branch from the tree.
 *   mark the other leaves as visited.
 * } while there are unvisited leaves.
 */
void topSimplifyTree(ctContext*, ctBranch*, top::Dataset&, double(*importance_cb)(ctBranch*), double);

DEPRECATED void topSimplifyTreeZhou(ctContext*, ctBranch*, ctBranch**, top::Dataset&, double(*importance_cb)(ctBranch*), double);

#endif // SIMPLIFICATION_H
