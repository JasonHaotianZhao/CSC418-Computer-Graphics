#include "point_AABBTree_squared_distance.h"
#include <queue> // std::priority_queue
#include "point_box_squared_distance.h"

bool point_AABBTree_squared_distance(
    const Eigen::RowVector3d & query,
    const std::shared_ptr<AABBTree> & root,
    const double min_sqrd,
    const double max_sqrd,
    double & sqrd,
    Eigen::RowVector3d & projection,
    std::shared_ptr<Object> & descendant)
{
    const auto comparator = [](const std::pair<double, std::shared_ptr<AABBTree>> node1, const std::pair<double, std::shared_ptr<AABBTree>> node2) -> bool {
        return node1.first > node2.first;
    };
    std::priority_queue<std::pair<double, std::shared_ptr<AABBTree>>, std::vector<std::pair<double, std::shared_ptr<AABBTree>>>, decltype(comparator)> q(comparator);
    
    sqrd = std::numeric_limits<double>::infinity();
    q.emplace(point_box_squared_distance(query, root->box), root);
    while (!q.empty()) {
        const double d_s = q.top().first;
        const std::shared_ptr<AABBTree> node = q.top().second;
        q.pop();
        if (d_s < sqrd) {
            // the current node is a leaf
            if (node->num_leaves <= 2) {
                double sqrd_;
                Eigen::RowVector3d projection_;
                std::shared_ptr<Object> descendant_;
                // calculate the distance between the query point and the left object, and compare it to the current global minimum found
                if (node->left && node->left->point_squared_distance(query, min_sqrd, max_sqrd, sqrd_, projection_, descendant_) && sqrd_ < sqrd) {
                    sqrd = sqrd_;
                    projection = projection_;
                    descendant = node->left;
                }
                // calculate the distance between the query point and the right object, and compare it to the current global minimum found
                if (node->right && node->right->point_squared_distance(query, min_sqrd, max_sqrd, sqrd_, projection_, descendant_) && sqrd_ < sqrd) {
                    sqrd = sqrd_;
                    projection = projection_;
                    descendant = node->right;
                }
            // push the left and right children into the queue
            } else {
                q.emplace(point_box_squared_distance(query, node->left->box), std::static_pointer_cast<AABBTree>(node->left));
                q.emplace(point_box_squared_distance(query, node->right->box), std::static_pointer_cast<AABBTree>(node->right));
            }
        }
    }
    return descendant != NULL;
}
