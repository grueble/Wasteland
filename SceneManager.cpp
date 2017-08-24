#include "SceneManager.hpp"

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
   GLuint shader_program_id = loadShaders("shaders/bg.vert", "shaders/bg.frag");
   GLuint mvp_id = glGetUniformLocation(shader_program_id, "MVP");
   GLuint texture_id = glGetUniformLocation(shader_program_id, "ourTexture");

   std::vector<float> vertices = {
      // position    // color           // uv coords
      0.0f, 0.0f,    0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // bottom-left
      0.0f, 10.0f,   0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // top-left
      20.0f, 10.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // top-right
      20.0f, 0.0f,   0.0f, 0.0f, 0.0f,  1.0f, 0.0f   // bottom-right
   };

   RenderQuad* mesh = new RenderQuad(shader_program_id, mvp_id, texture_id, vertices);

   AABB_t* hitbox = new AABB_t((vec2_t){ 10.0f, 5.0f },  // p
                               (vec2_t){ 10.0f, 0.0f },  // xw
                               (vec2_t){ 0.0f,  5.0f }); // yw

   // problem here bc passing hitbox as PhysObj when using this constructor 
   SceneNode* root = new SceneNode((RenderObj*)mesh, (PhysObj*)hitbox, glm::mat4(1.0f), glm::mat4(1.0f));

   // SceneNode* platform0_bb = new SceneNode(
   //    NULL, 
   //    new AABB((vec2_t){ 9.5f, 3.0f },  // p
   //             (vec2_t){ 5.5f, 0.0f },  // xw
   //             (vec2_t){ 0.0f, 2.5f }), // yw
   //    glm::mat4(1.0f),
   //    glm::mat4(1.0f)
   // );

   // shader_program_id = loadShaders("shaders/platform.vert", "shaders/platform.frag");

   // GeomNode* platform0_0 = new GeomNode(
   //    new RenderQuad(shader_program_id,
   //                   glGetUniformLocation(shader_program_id, "MVP"), 
   //                   glGetUniformLocation(shader_program_id, "ourTexture")),
   //    new AABB((vec2_t){ 5.5f, 4.5f },  // p
   //             (vec2_t){ 1.5f, 0.0f },  // xw
   //             (vec2_t){ 0.0f, 1.0f }), // yw
   //    glm::mat4(1.0f),
   //    glm::mat4(1.0f)
   // );

   // GeomNode* platform0_1 = new GeomNode(
   //    new RenderTri(shader_program_id,
   //                  glGetUniformLocation(shader_program_id, "MVP"), 
   //                  glGetUniformLocation(shader_program_id, "ourTexture")),
   //    new RenderTri((vec2_t){ 9.0f, 3.5f }, // p
   //                  (vec2_t){ 2.0f, 0.0f }, // xw
   //                  (vec2_t){ 0.0f, 2.0f }, // yw
   //                  normalize((vec2_t){ 1.0f, 1.0f })), // 3rd axis @ pi/4 rads
   //    glm::mat4(1.0f),
   //    glm::mat4(1.0f)
   // );

   // GeomNode* platform0_2 = new GeomNode(
   //    new RenderQuad(shader_program_id,
   //                   glGetUniformLocation(shader_program_id, "MVP"), 
   //                   glGetUniformLocation(shader_program_id, "ourTexture")),
   //    new AABB((vec2_t){ 12.0f, 1.0f },  // p
   //             (vec2_t){  3.0f, 0.0f },  // xw
   //             (vec2_t){  0.0f, 0.5f }), // yw
   //    glm::mat4(1.0f),
   //    glm::mat4(1.0f)
   // );

   // platform0_bb->addChild(platform0_0);
   // platform0_bb->addChild(platform0_1);
   // platform0_bb->addChild(platform0_2);
   // root->addChild(platform0_bb);

   return root;
}

// ActorNode* loadPlayer()
// {
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
// }