/*
This is main.cpp of the game Everything, taken from the demo file for the Phenomenon engine.
One important part to notice is that this file uses the SDL_Event system, and passes events that it doesn't handle to the engine.
This is important, as the engine uses it to handle window resize and window loss of focus.
*/

#include <iostream>
#include "Phenomenon.h"                                         //The Phenomenon header file.
#include "SceneLoader.h"                                        //Object that loads scene files (.esc)

using namespace std;

int main(int argc, char* argv[])
{

    Window window;                                              //Create our window class, which handles all the windowing/SDL/GL init stuff

    int done = false;                                           //Main loop variable
    SDL_Event *event;                                           //Useed in collecting events
    event = new SDL_Event;

    window.create(640,480,32);                                  //Creates the window. Sets up SDL and OpenGL, sets some variables
    window.setCaption("Everything");                            //The window should be created before you start setting up anything that could make SDL or OpenGL calls.

    cout<< "OpenGL version is reported as: " << glGetString(GL_VERSION) << endl;    //Show OpenGL version



    Scene scene;                                                //Scenes are just that, scenes. They are renderd with a camera. Each scene has its own root node. Rendering will also take place inside of scenes.
    Node* rootNode = scene.getRootNode();                       //All objects are represented with nodes. Basic nodes can be used to group together other nodes. This node is the root node.

    SceneLoader scene_loader;                                   //A scene loader object takes a scene and a (currently VERY basic) scene file (.esc) and loads the objects specified in the file into the scene.
    scene_loader.loadScene(&scene, "./data/scene.esc");

    Light* lightNode = scene.newLight("light");                 //Lights are handled specially, so you must create them using a scene object.

    scene.enableLighting();                                     //Enable lighting by default.

    Camera camera("camera");                                    //Cameras inherit Node too, and thus require a name. It also dosn't have to be attached to a root node, but can be attached to a node if you wish. Just <dynamic_cast> again.*/
    lightNode->setDiffuse(1.0f, 1.0f, 1.0f);                    //Default is full white (1.0f, 1.0f, 1.0f)
    lightNode->setAmbient(0.8f, 0.8f, 0.8f);                    //Default is medium gray (0.5f, 0.5f, 0.5f)

    rootNode->setLocalPosition(0.0f, 0.0f, 0.0f);
    lightNode->setLocalPosition(0.0f, 5.0f, 1.0f);

    Node* shaded_node = createTriangle("shaded_triangle");
    Material* shaded_material = scene.newMaterial("shaded_material");

    Shader* basic_shader = new Shader("basic_shader");
    //basic_shader.createShaderProgram("./data/sample_shader.vert", "./data/sample_shader.frag");
    basic_shader->createShaderProgram("./data/sample_shader.frag");

    shaded_material->setShader(basic_shader);
    shaded_node->setMaterial(shaded_material);

    rootNode->addChild(shaded_node);

    camera.setLocalPosition(0.0f, 0.0f, 6.0f);                  //Setting the camera 6 units toward the screen is the same as setting everything else 6 units away.
    camera.setLocalRotation(0.0f, 0.0f, 0.0f);
    camera.setLocalScale(1.0f, 1.0f, 1.0f);

    GLfloat camera_rotate_x = 0, camera_rotate_y = 0, camera_move_fb = 0, camera_move_lr = 0;       //Set up our movement variables


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

                    case SDLK_l:
                        scene.enableLighting();
                        break;

                    case SDLK_k:
                        scene.disableLighting();
                        break;

                    case SDLK_j:
                        window.initGL();
                        scene.enableLighting();
                        cout << "j key pressed, ran workarounds\n";

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
                    window.handleEvent(event);          //If not a keypress or SDL_QUIT, let the engine handle the event. Possibilities include window resize, losing window focus.
                }
            }
        }                                               //end while

        camera.rotate(camera_rotate_x, camera_rotate_y, 0);        //Rotate
        camera.goForward(camera_move_fb/10);                       //Moving foreward a negitive ammount is moving backward
        camera.goLeft(camera_move_lr/10);                          //Moving left a negitive ammount is moving right


        if (window.isActive)                            //window.isActive is false if we've already quit, or if we're minimised.
        {
            window.clearScreen();
            camera.drawScene(&scene);                   //All nodes draw() function, including basic nodes like rootNode, calls the draw() functions of their children.
            window.swapBuffers();                       //Thus, calling rootNode.draw() will draw the entire scene. However, to use a camera, you must call drawScene on it
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
