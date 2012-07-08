/*
This is main.cpp of the game Everything, taken from the demo file for the Phenomenon engine.
One important part to notice is that this file uses the SDL_Event system, and passes events that it doesn't handle to the engine.
This is important, as the engine uses it to handle window resize and window loss of focus.
*/

#include <iostream>
#include "Phenomenon.h"                                         //The Phenomenon header file.

#define FOV 45.0f
#define Z_NEAR 0.1
#define Z_FAR 100


using namespace std;
using namespace phen;                                           //The Phenomenon engine namespace

int main(int argc, char* argv[])
{

    Window window;                                              //Create our window class, which handles all the windowing/SDL/GL init stuff

    int done = false;                                           //Main loop variable
    SDL_Event *event;                                           //Useed in collecting events
    event = new SDL_Event;

    window.create(1024, 768,32);                                  //Creates the window. Sets up SDL and OpenGL, sets some variables
    //window.create(640, 480,32);                                  //Creates the window. Sets up SDL and OpenGL, sets some variables
    window.setCaption("Everything");                            //The window should be created before you start setting up anything that could make SDL or OpenGL calls.
    window.enableBackfaceCulling();                             //Enable backface culling for a speed-up.

    cout<< "OpenGL version is reported as: " << glGetString(GL_VERSION) << endl;    //Show OpenGL version


    Scene scene;                                                //Scenes are just that, scenes. They are renderd with a camera. Each scene has its own root node.
    Node* rootNode = scene.getRootNode();                       //All objects are represented with nodes. Basic nodes can be used to group together other nodes. This node is the root node.

    Renderer renderer;                                          //A renderer object takes a camera and a scene and renders them.
    renderer.init(1024, 768, 2.2);
    Shader* fullscreen_shader = scene.newShader("fullscreen_quad_shader");
    fullscreen_shader->createShaderProgram("data/FullScreenQuad.vert", "./data/FullScreenQuad.frag");
    renderer.initQuad(fullscreen_shader);                       //Pass in your fullscreen quad shader. This is where post-processing effects are done


    Camera camera("camera");                                    //Cameras inherit Node too, and thus require a name. It also dosn't have to be attached to a root node, but can be attached to a node if you wish. Just <dynamic_cast> again.*/
    camera.setProjectionMatrix(1024, 768, FOV, Z_NEAR, Z_FAR);    //Aspect ratio, Field of View, near plane, far plane
    //camera.setProjectionMatrix(640, 480, FOV, Z_NEAR, Z_FAR);    //Aspect ratio, Field of View, near plane, far plane

    //SceneLoader scene_loader;                                   //A scene loader object takes a scene and a (currently VERY basic) scene file (.esc) and loads the objects specified in the file into the scene.
    //scene_loader.loadScene(&scene, "./data/scene_mirror.esc");
    //scene_loader.loadScene(&scene, "./data/scene.esc");

    Light* lightNode = scene.newLight("light");                 //Lights are handled specially, so you must create them using a scene object.

    scene.enableLighting();                                     //Enable lighting by default.

    lightNode->setDiffuse(1.0f, 1.0f, 1.0f);                    //Default is full white (1.0f, 1.0f, 1.0f)
    lightNode->setAmbient(0.8f, 0.8f, 0.8f);                    //Default is medium gray (0.5f, 0.5f, 0.5f)

    rootNode->setLocalPosition(0.0f, 0.0f, 0.0f);
    lightNode->setLocalPosition(0.0f, 5.0f, 1.0f);

    Shader* basic_shader = scene.newShader("basic_shader");
    basic_shader->createShaderProgram("./data/geometry_pass.vert", "./data/geometry_pass.frag");

    ModelLoader* loader = new ModelLoader(&scene, basic_shader, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);               //Give the ModelLoader our scene for use with .mtl loading. You can also set the texture filtering (min_filter then mag_filter) to be used for any model textues loaded. It defaults to GL_NEAREST, GL_LINEAR

    //Node* LevelGroup = loader->loadOBJ("./crytek-sponza-diff/sponza-tri.obj", "first_level_group");    //Object loader, when loading a .obj with multiple objects, will load all the objects and attach them to an empty parent node, which is then returned.
    Node* LevelGroup = loader->loadOBJ("./data/FirstLevelFull.obj", "first_level_group");    //Object loader, when loading a .obj with multiple objects, will load all the objects and attach them to an empty parent node, which is then returned.
    rootNode->addChild(LevelGroup);
    LevelGroup->setLocalPosition(0,-1,0);
    delete loader;                                                                                  //Delete the loader when done. Should create a new ModelLoader for each file.
    loader = NULL;
    std::cout << "loaded object\n";

    loader = new ModelLoader(&scene, basic_shader, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Node* MonkeyHead = loader->loadOBJ("./data/Monkey.obj", "monkey head");
    rootNode->addChild(MonkeyHead);
    MonkeyHead->setLocalPosition(0,0.5,4);
    MonkeyHead->setLocalRotation(0,180,0);

    int test[2] = {2};
    std::cout << "This is first: " << test[0] << " and second: " << test[1] << "\n";

/*
    PhysicsManager* physics_manager = scene.getPhysicsManager();

    Node* ground_plane = createSquare("ground");
    Material* ground_material = scene.newMaterial("ground_material");
    ground_plane->setMaterial(ground_material);
    rootNode->addChild(ground_plane);
    ground_plane->setLocalRotation(90,0,0);

    PhysicsObject* physics_ground_plane = new PhysicsObject("ground");
    PhenomenonMotionState* ground_motion_state = new PhenomenonMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0)), ground_plane);
    physics_ground_plane->setMotionState(ground_motion_state);

    btCollisionShape* ground_plane_shape = new btStaticPlaneShape(btVector3(0,1,0), 1);
    physics_ground_plane->setShape(ground_plane_shape);
    physics_manager->addCollisionShape(ground_plane_shape);

    physics_ground_plane->makeRigidBody(0);
    physics_manager->addPhysicsObject(physics_ground_plane);


    Node* falling_triangle = createTriangle("falling_triangle");
    Material* triangle_material = scene.newMaterial("triangle_material");
    falling_triangle->setMaterial(triangle_material);
    rootNode->addChild(falling_triangle);
    falling_triangle->setLocalPosition(0,0,0);

    PhysicsObject* physics_triangle = new PhysicsObject("falling_triangle");
    PhenomenonMotionState* triangle_motion_state = new PhenomenonMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,5,0)), falling_triangle);
    physics_triangle->setMotionState(triangle_motion_state);

    btCollisionShape* triangle_shape = new btSphereShape(1);
    physics_triangle->setShape(triangle_shape);
    physics_manager->addCollisionShape(triangle_shape);

    physics_triangle->makeRigidBody(1);
    physics_manager->addPhysicsObject(physics_triangle);

*/
    camera.setLocalPosition(0.0f, 0.0f, 0.0f);                  //Setting the camera 6 units toward the screen is the same as setting everything else 6 units away.
    camera.setLocalRotation(0.0f, 0.0f, 0.0f);
    camera.setLocalScale(1.0f, 1.0f, 1.0f);

    GLfloat camera_rotate_x = 0, camera_rotate_y = 0, camera_move_fb = 0, camera_move_lr = 0;       //Set up our movement variables
    GLfloat monkey_rotate_x = 0, monkey_rotate_y = 0;                                     //Set up our movement variables
    Vector monkey_current_rotation;


    while(!done)                                                //Main loop
    {
                                                                //Handle events in queue
        while (SDL_PollEvent(event))
        {
            if (event->type == SDL_KEYDOWN)                     //If a key is pressed, we handle it
            {
                switch (event->key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        window.quit();                          //Don't use window after you quit it, it will probally cause a seg fault.
                        done = true;
                        break;

                    case SDLK_F1:
                        window.toggleFullScreen();              //Still needs work. Fails on some systems, and dosn't actually change the OpenGL size on every platform.
                        break;

                    case SDLK_w:
                        camera_rotate_x = 1;
                        break;

                    case SDLK_a:
                        camera_rotate_y = 1;
                        break;

                    case SDLK_s:
                        camera_rotate_x = -1;
                        break;

                    case SDLK_d:
                        camera_rotate_y = -1;
                        break;

                    case SDLK_UP:
                        camera_move_fb = 1;
                        break;

                    case SDLK_DOWN:
                        camera_move_fb = -1;
                        break;

                    case SDLK_LEFT:
                        camera_move_lr = 1;
                        break;

                    case SDLK_RIGHT:
                        camera_move_lr = -1;
                        break;

                    case SDLK_i:
                        monkey_rotate_x = 1;
                        break;

                    case SDLK_j:
                        monkey_rotate_y = 1;
                        break;

                    case SDLK_k:
                        monkey_rotate_x = -1;
                        break;

                    case SDLK_l:
                        monkey_rotate_y = -1;
                        break;

                    case SDLK_o:
                        scene.enableLighting();
                        break;

                    case SDLK_p:
                        scene.disableLighting();
                        break;

                    case SDLK_u:
                        window.initGL();
                        scene.enableLighting();
                        cout << "u key pressed, ran workarounds\n";
                        break;

                    case SDLK_t:
                        window.enableBackfaceCulling();
                        cout << "t key pressed, enabeling backface culling\n";
                        break;

                    case SDLK_y:
                        window.disableBackfaceCulling();
                        cout << "y key pressed, disableing backface culling\n";
                        break;

                    default:
                        break;
                }
            }
        if (event->type == SDL_KEYUP)
            {
                switch (event->key.keysym.sym)
                {
                    case SDLK_w:
                        camera_rotate_x = 0;
                        break;

                    case SDLK_a:
                        camera_rotate_y = 0;
                        break;

                    case SDLK_s:
                        camera_rotate_x = 0;
                        break;

                    case SDLK_d:
                        camera_rotate_y = 0;
                        break;

                    case SDLK_UP:
                        camera_move_fb = 0;
                        break;

                    case SDLK_DOWN:
                        camera_move_fb = 0;
                        break;

                    case SDLK_LEFT:
                        camera_move_lr = 0;
                        break;

                    case SDLK_RIGHT:
                        camera_move_lr = 0;
                        break;

                    case SDLK_i:
                        monkey_rotate_x = 0;
                        break;

                    case SDLK_j:
                        monkey_rotate_y = 0;
                        break;

                    case SDLK_k:
                        monkey_rotate_x = 0;
                        break;

                    case SDLK_l:
                        monkey_rotate_y = 0;
                        break;

                    default:
                        break;
                }

            } else
            {
                if (event->type == SDL_QUIT)            //If the system wants us to quit, then do so.
                {
                    window.quit();                      //Quit window stuff
                    done = true;
                } else
                {
                    if (event->type == SDL_VIDEORESIZE)
                    {
                        window.simpleResize(event->resize.w, event->resize.h);
                        camera.setProjectionMatrix(event->resize.w, event->resize.h, FOV, Z_NEAR, Z_FAR);
                        renderer.init(event->resize.w, event->resize.h);
                        renderer.initQuad(fullscreen_shader);
                    } else {
                        window.handleEvent(event);          //Let the engine handle the event.
                    }
                }
            }
        }                                               //end while

        camera.rotate(camera_rotate_x, camera_rotate_y, 0);        //Rotate
        camera.goForward(camera_move_fb/10);                       //Moving foreward a negitive ammount is moving backward
        camera.goLeft(camera_move_lr/10);                          //Moving left a negitive ammount is moving right

        monkey_current_rotation = MonkeyHead->getLocalRotation();
        MonkeyHead->setLocalRotation(monkey_rotate_x + monkey_current_rotation.x, monkey_rotate_y + monkey_current_rotation.y, 0);        //Rotate

        if (window.isActive)                            //window.isActive is false if we've already quit, or if we're minimised.
        {
            //physics_manager->step(1);
            window.clearScreen();
            renderer.render(&camera, &scene);           //All nodes draw() function, including basic nodes like rootNode, calls the draw() functions of their children.
            window.swapBuffers();                       //This is done in the renderer which also gets the necessary camera data.
            //cout<<"drew frame\n";
        } else {/*sleep(1);*/}                          //and pass it the scene. It will apply the appropriate translation, rotation, and scale for its own properties,
                                                        //Scene will set up lights and rendering, then call draw() on the root node.
    }
                                                        /////////////////////////
                                                        //NOTE
                                                        /////////////////////////
                                                        //Nodes get destroyed when their parents falls out of scope.
                                                        //If you delete them manually (without using the deleteChild(child's name) from the parent node)
                                                        //the parent will segfault when it trys to delete them.
                                                        //Here scene owns rootNode, which is the parent of every other dynamically allocated node, so it deletes them all when it falls out of scope.
                                                        //Materials and textures are handeld by scene as well, and are deleted when it falls out of scope.
    return 0;
}
