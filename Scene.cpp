#include "GameObjects.hpp"

point2_t eye_position = {0.0f, 0.0f};
point2_t look_at_point = {-1.0f, -1.0f};
vector2_t up_dir = {0.0f, 1.0f};

// void getPhysObjs(std::vector<PhysObj>& phys_objs)
// {
//    phys_objs.push_back(new Rectangle)
// }

// want to create a slight illusion of 3d, place textures on layers with cam at distance, allow for zoom
