#include "simplification.h"

bool marked_for_removal(ctBranch* b) {
    if(b == NULL) return true;
    FeatureSet* fs = (FeatureSet*) b->data;
    return fs->remove;
    return fs == NULL? false : fs->remove;
}

bool branch_is_leaf(ctBranch* b)
{
    if(b == NULL || b->children.head == NULL) return true;

    bool will_remove = true;
    for(ctBranch* c = b->children.head; c != NULL && will_remove == true; c = c->nextChild)
        will_remove = marked_for_removal(c);

    return will_remove;
}

//void remove_branch(ctBranch* branch_to_remove, ctBranch** b_map, Data* data, ctContext* ctx)
//{
//    if(branch_to_remove == NULL) return;

//    ctBranch* parent = branch_to_remove->parent;
//    if(parent == NULL) return;

//    //    for(unsigned int i = 0; i < data->totalSize; i++) {
//    //        if(b_map[i]->extremum == branch_to_remove->extremum && b_map[i]->saddle == branch_to_remove->saddle)
//    //            b_map[i] = parent;
//    //    }
//    free(branch_to_remove->data);
//    branch_to_remove->data = NULL;
//    ctBranchList_remove(&parent->children, branch_to_remove);
//    ctBranch_delete(branch_to_remove, ctx);
//}

void mark_for_removal(ctBranch* root_branch, ctBranch** branch_map, size_t data_size, double (*importance_measure)(ctBranch*), double threshold)
{
    if(root_branch == NULL) return;
    FeatureSet* branch_data = (FeatureSet*) root_branch->data;

    for(ctBranch* c = root_branch->children.head; c != NULL; c = c->nextChild)
        mark_for_removal(c, branch_map, data_size, importance_measure, threshold);

    if(branch_is_leaf(root_branch)) {
        importance_measure(root_branch) > threshold ? branch_data->remove = false : branch_data->remove = true;
        //TODO: Associate the vertices with the parent branch
        for(size_t i = 0; i < data_size; i++) {
            if(branch_map[i] == root_branch && root_branch->parent != NULL) {
                branch_map[i] = root_branch->parent;
            }
        }
    }
}

//UNUSED
//void remove_marked_branches(ctBranch* root_branch, ctBranch** branch_map, Data* data, ctContext* ctx)
//{
//    if(root_branch == NULL) return;
//    FeatureSet* branch_data = (FeatureSet*) root_branch->data;

//    if(branch_data == NULL) return;
//    for(ctBranch* c = root_branch->children.head; c != NULL; c = c->nextChild) {
//        remove_marked_branches(c, branch_map, data, ctx);
//        remove_branch(c, branch_map, data, ctx);
//    }

//    //if(branch_is_leaf(root_branch))

//}

void simplify_tree_dfs(ctBranch* root_branch, ctBranch** branch_map, size_t data_size, double (*importance_measure)(ctBranch*), double threshold)
{
    mark_for_removal(root_branch, branch_map, data_size, importance_measure, threshold);
    //    remove_marked_branches(root_branch, branch_map, data, ctx);
    //    if(root_branch == NULL) return;
    //    FeatureSet* branch_data = (FeatureSet*) root_branch->data;

    //    for(ctBranch* c = root_branch->children.head; c != NULL; c = c->nextChild)
    //        simplify_tree_dfs(c, branch_map, data, ctx, importance_measure, threshold);

    //    if(/*root_branch->children.head == NULL*/branch_is_leaf(root_branch)) {
    //        double arc_importance = importance_measure(root_branch);
    //        if(arc_importance < threshold) {
    //            branch_data->remove = true;
    //            //remove_branch(root_branch, branch_map, data, ctx);
    //        } else {
    //            std::cout << "Volume: " << branch_data->v << "\tHypervolume: " << branch_data->hv << "\tPersistance: " << branch_data->p << std::endl;
    //        }
    //        return;
    //    }
}

//void simplify_tree_bfs(ctBranch* root_branch, double (*importance_measure)(ctBranch*), double threshold)
//{
//    if(root_branch == NULL) return;

//    std::queue<ctBranch*> branch_queue;
//    branch_queue.push(root_branch);

//    do {
//        ctBranch* curr_branch = branch_queue.front();
//        branch_queue.pop();

//        double branch_importance = importance_measure(curr_branch);
//        if(branch_importance < threshold) {
//            FeatureSet* ptr = (FeatureSet*) curr_branch->data;
//            ptr->remove = true;
//            ctBranch* p = curr_branch->parent;

//            //Associate the sons with the parent. Delete the branch.
//            //What happens to the vertices of the deleted branch?
//        }

//        for(ctBranch* c = curr_branch->children.head; c != NULL; c = c->nextChild) {
//            branch_queue.push(c);
//        }
//    } while(!branch_queue.empty());
//}
