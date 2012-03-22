#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Phenomenon.h"

class SceneLoader
{
    public:
        SceneLoader();
        virtual ~SceneLoader();
        int loadScene(Scene* scene, string file_path);

    protected:
    private:
};

#endif // SCENELOADER_H
