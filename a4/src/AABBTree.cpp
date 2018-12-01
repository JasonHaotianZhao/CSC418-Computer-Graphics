#include "AABBTree.h"
#include "insert_box_into_box.h"
#include <iostream>

AABBTree::AABBTree(
  const std::vector<std::shared_ptr<Object> > & objects,
  int a_depth): 
  depth(std::move(a_depth)), 
  num_leaves(objects.size())
{
    int n = objects.size();
    if (n == 0) {
        this->left = NULL;
        this->right = NULL;
    // leaf node with 1 object
    } else if (n == 1) {
        this->left = objects[0];
        this->right = NULL;
        insert_box_into_box(this->left->box, this->box);
    // leaf node with 2 objects
    } else if (n == 2) {
        this->left = objects[0];
        this->right = objects[1];
        insert_box_into_box(this->left->box, this->box);
        insert_box_into_box(this->right->box, this->box);
    } else {
        // create overall bounding box
        for (const auto& object : objects) {
            insert_box_into_box(object->box, this->box);
        }

        // find the longest axis
        int axis;
        (this->box.max_corner-this->box.min_corner).maxCoeff(&axis);
        const double split = 0.5*(box.max_corner(axis)+box.min_corner(axis));
        
        // split objects into two partitions based on the mid point of the longest axis
        std::vector<std::shared_ptr<Object>> leftPartition;
        std::vector<std::shared_ptr<Object>> rightPartition;
        for (const auto& object : objects) {
            if (object->box.center()(axis) < split) {
                leftPartition.emplace_back(object);
            } else {
                rightPartition.emplace_back(object);
            }
        }
        
        // handle corner case where we can not split the objects into two partitions
        if (leftPartition.size() == 0 && rightPartition.size() != 0) {
            leftPartition.emplace_back(rightPartition.back());
            rightPartition.pop_back();
        } else if (leftPartition.size() != 0 && rightPartition.size() == 0) {
            rightPartition.emplace_back(leftPartition.back());
            leftPartition.pop_back();
        }
        
        // create left and right bounding boxes
        this->left = std::make_shared<AABBTree>(leftPartition, a_depth+1);
        this->right = std::make_shared<AABBTree>(rightPartition, a_depth+1);
    }
}

bool AABBTree::ray_intersect(
  const Ray& ray,
  const double min_t,
  const double max_t,
  double & t,
  std::shared_ptr<Object> & descendant) const
{
    // first check whether the ray hits the bounding box or not
    if (ray_intersect_box(ray, this->box, min_t, max_t, t)) {
        double lRec, rRec;
        std::shared_ptr<Object> lDescendant, rDescendant;
        // get results from two children
        bool lHit = (this->left != NULL) && this->left->ray_intersect(ray, min_t, max_t, lRec, lDescendant);
        bool rHit = (this->right != NULL) && this->right->ray_intersect(ray, min_t, max_t, rRec, rDescendant);
        
        // left/right child is an actually object in the scene
        if (lHit && !lDescendant) {
            lDescendant = left;
        }
        
        if (rHit && !rDescendant) {
            rDescendant = right;
        }
        
        // choose the closest hit
        if (lHit && rHit) {
            if (lRec < rRec) {
                t = lRec;
                descendant = lDescendant;
            } else {
                t = rRec;
                descendant = rDescendant;
            }
            return true;
        } else if (lHit) {
            t = lRec;
            descendant = lDescendant;
            return true;
        } else if (rHit) {
            t = rRec;
            descendant = rDescendant;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
