#include "SceneManager.hpp"
#include <iostream>

// void loadScene(SceneNode& root)
// {
//    GLuint tri_shader_prog_id = loadShaders("shaders/triangle.vert", 
//                                            "shaders/triangle.frag");

//    GLuint tri_mvp_id = glGetUniformLocation(tri_shader_prog_id, "MVP");

//    /* NOTE: model @ origin
//     * NOTE: **screen** coords are always of the form: 
//     * -> ( [-1.0f, 1.0f], [-1.0f, 1.0f] ) */
//    root.push_back(new Triangle(
//          tri_shader_prog_id, tri_mvp_id, // ID's associated w/ shader program
//          glm::mat4(1.0f),                // obj. @ origin in world space
//          // (point2_t){-1.0f, -1.0f}, (point2_t){1.0f, -1.0f}, (point2_t){0.0f, 1.0f}));
//          (tri_data_t){ (point2_t){-1.0f, -1.0f}, 
//                        (point2_t){ 1.0f, -1.0f}, 
//                        (point2_t){ 0.0f,  1.0f} }));

//    GLuint quad_shader_prog_id = loadShaders("shaders/quad.vert", 
//                                             "shaders/quad.frag");

//    GLuint quad_mvp_id = glGetUniformLocation(quad_shader_prog_id, "MVP");

//    glm::mat4 translation = glm::translate(glm::vec3(2, 0, 0));

//    root.push_back(new Quad(
//       quad_shader_prog_id, quad_mvp_id,   // ID's associated w/ shader program
//       glm::translate(glm::vec3(2, 0, 0)), // obj. @ (2, 0, 0) in world space
//       (quad_data_t){ (point2_t){-0.5f,  0.5f},     // top-left
//                      (point2_t){ 0.5f,  0.5f},     // top-right
//                      (point2_t){-0.5f, -0.5f},     // bottom-left
//                      (point2_t){ 0.5f, -0.5f} })); // bottom-right

//    m_projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
//    // m_projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);

//    // this is camera frame code, will need to be input controlled
//    glm::vec3 eye_position = glm::vec3(4.0f, 3.0f, 3.0f);
//    glm::vec3 look_at_point = glm::vec3(0.0f, 0.0f, 0.0f);
//    glm::vec3 up_dir = glm::vec3(0.0f, 1.0f, 0.0f);
//    m_view = glm::lookAt(eye_position, look_at_point, up_dir);
// }

SceneNode* loadScene()
{
   // local and world xforms allow us to only enter relative coordinates - calling update at onset populates

   // pointer invalidation is probably happening somewhere in here...
   // -> i went thru all of the tex id's etc. and nothing seemed out of place - vertex attrs, tex image unit, tex ID, etc/

   SceneNode* root = new SceneNode(NULL, NULL); // i think this may need to be instantiated NOT as a pointer...
   root->setXform(glm::mat4(1.0));

   // could do this stuff in RenderObj() constructor...
   GLuint shader_program_id = loadShaders("shaders/texture.vert", "shaders/texture.frag");
   GLuint mvp_id = glGetUniformLocation(shader_program_id, "MVP");
   GLuint texture_id = glGetUniformLocation(shader_program_id, "ourTexture");

   // NOTE: position of hitbox is starting/default node position
   AABB_t* hitbox0 = new AABB_t((vec2_t){ 10.0f, 5.0f },  // p
                                (vec2_t){ 10.0f, 0.0f },  // xw
                                (vec2_t){ 0.0f,  5.0f }); // yw

   // might want a default color array to use
   std::vector<float> vertices = {
      // position                                                  // color           // uv coords
      hitbox0->p.x - hitbox0->xw.x, hitbox0->p.y + hitbox0->yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
      hitbox0->p.x + hitbox0->xw.x, hitbox0->p.y + hitbox0->yw.y,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
      hitbox0->p.x + hitbox0->xw.x, hitbox0->p.y - hitbox0->yw.y,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
      hitbox0->p.x - hitbox0->xw.x, hitbox0->p.y - hitbox0->yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
   };

   RenderQuad* mesh0 = new RenderQuad(shader_program_id, mvp_id, texture_id, vertices);
   mesh0->loadTexture("./assets/dune_glitch.png");

   // problem here bc passing hitbox as PhysObj when using this constructor 
   GeomNode* platform0 = new GeomNode((RenderObj*)mesh0, (PhysObj*)hitbox0);
   // SceneNode* platform0 = new GeomNode ... (this is valid, but needs to make sure to call the proper ~)
   platform0->setXform(glm::mat4(1.0));
   root->addChild((SceneNode*)platform0);

   Triangle* hitbox1 = new Triangle((vec2_t){  0.0f,  15.0f },  // p
                                    (vec2_t){ -15.0f, 0.0f },   // xw
                                    (vec2_t){  0.0f, -15.0f }); // yw

   // NOTE: uv coordinate origin @ top-left
   vertices = {
      // position                                  // color           // uv coords
      hitbox1->p.x, hitbox1->p.y + hitbox1->yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
      hitbox1->p.x, hitbox1->p.y,                  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // bottom-left
      hitbox1->p.x + hitbox1->xw.x, hitbox1->p.y,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f   // bottom-right
   }; // would it be possible to generate uv coords from hitbox??? should be

   RenderTri* mesh1 = new RenderTri(shader_program_id, mvp_id, texture_id, vertices);
   mesh1->loadTexture("./assets/aliendesert.jpg");

   GeomNode* platform1 = new GeomNode((RenderObj*)mesh1, (PhysObj*)hitbox1);
   platform1->setXform(glm::mat4(1.0));
   root->addChild((SceneNode*)platform1);

   Triangle* hitbox2 = new Triangle((vec2_t){ 0.0f, 10.0f }, // p
                                    (vec2_t){ 5.0f, 0.0f },  // xw
                                    (vec2_t){ 0.0f, 5.0f }); // yw

   vertices = {
      // position                                  // color           // uv coords
      hitbox2->p.x, hitbox2->p.y + hitbox2->yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
      hitbox2->p.x, hitbox2->p.y,                  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // bottom-left
      hitbox2->p.x + hitbox2->xw.x, hitbox2->p.y,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f   // bottom-right
   }; 

   RenderTri* mesh2 = new RenderTri(shader_program_id, mvp_id, texture_id, vertices);
   // mesh2->loadTexture("./assets/dunepool.jpg");

   GeomNode* platform2 = new GeomNode((RenderObj*)mesh2, (PhysObj*)hitbox2);
   platform2->setXform(glm::mat4(1.0));
   root->addChild((SceneNode*)platform2);

   AABB_t* hitbox3 = new AABB_t((vec2_t){ 23.5f, 2.5f },  // p
                                (vec2_t){  3.5f, 0.0f },  // xw
                                (vec2_t){  0.0f, 2.5f }); // yw

   vertices = {
      // position                                                  // color           // uv coords
      hitbox3->p.x - hitbox3->xw.x, hitbox3->p.y + hitbox3->yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
      hitbox3->p.x + hitbox3->xw.x, hitbox3->p.y + hitbox3->yw.y,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
      hitbox3->p.x + hitbox3->xw.x, hitbox3->p.y - hitbox3->yw.y,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
      hitbox3->p.x - hitbox3->xw.x, hitbox3->p.y - hitbox3->yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
   };

   RenderQuad* mesh3 = new RenderQuad(shader_program_id, mvp_id, texture_id, vertices);
   // mesh3->loadTexture("./assets/brklynware.jpg");

   // problem here bc passing hitbox as PhysObj when using this constructor 
   GeomNode* platform3 = new GeomNode((RenderObj*)mesh3, (PhysObj*)hitbox3);
   platform0->setXform(glm::mat4(1.0));
   root->addChild((SceneNode*)platform3);

   root->update();

   return root;
}

ActorNode* loadPlayer()
{
   // ActorNode* player = new ActorNode(
   //    NULL,
   //    new Circle((vec2){ 7.0f, 4.0f }, 1.0f),
   //    glm::mat4(1.0f),
   //    glm::mat4(1.0f)
   // );

   // GLuint shader_program_id = loadShaders("shaders/wheel.vert", "shaders/wheel.frag");

   // SceneNode* front_wheel = new SceneNode(
   //    new RenderQuad(shader_program_id,
   //                   glGetUniformLocation(shader_program_id, "MVP"), 
   //                   glGetUniformLocation(shader_program_id, "ourTexture")),
   //    new Circle((vec2){ 7.5f, 4.0f }, 0.5f),
   //    glm::mat4(1.0f),
   //    glm::mat4(1.0f)
   // );

   // SceneNode* back_wheel = new SceneNode(
   //    new RenderQuad(shader_program_id,
   //                   glGetUniformLocation(shader_program_id, "MVP"), 
   //                   glGetUniformLocation(shader_program_id, "ourTexture")),
   //    new Circle((vec2_t{ 6.5f, 4.0f }, 0.5f)),
   //    glm::mat4(1.0f),
   //    glm::mat4(1.0f)
   // );

   // player->addChild(front_wheel);
   // player->addChild(back_wheel);

   // return player;
}