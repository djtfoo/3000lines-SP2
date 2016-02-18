#include "Scene.h"
#include "Mesh.h"
#include "Camera3.h"
#include "MatrixStack.h"
#include "Light.h"

#include "Collision.h"

#include "PipePuzzle.h"

#include <cstdlib>

#include <vector>

#ifndef SP2_H
#define SP2_H

class SP2 : public Scene
{
    enum GEOMETRY_TYPE
    {
        //skybox
        GEO_SKYBOX_LEFT,
        GEO_SKYBOX_RIGHT,
        GEO_SKYBOX_TOP,
        GEO_SKYBOX_BOTTOM,
        GEO_SKYBOX_BACK,
        GEO_SKYBOX_FRONT,

        GEO_GROUND,
        
        GEO_MAN,
        //text
        GEO_TEXT,
        GEO_DIALOGUEBOX,
		//dev texture
		GEO_DEVTEXTURE,
		GEO_HANGAR,
		GEO_CONNECTOR,
		GEO_MAINWALLS,
        GEO_INNERWALLS,
        GEO_LABWALL,
        GEO_CONTROLWALLS,
        GEO_SHOP_WALLS,
        GEO_CANTEENWALLS,


		GEO_PIPETYPE1,
		GEO_PIPETYPE2,
		GEO_PIPETYPE3,
		GEO_PIPETYPE4,
        NUM_GEOMETRY,
    };

    enum UNIFORM_TYPE
    {
        U_MVP = 0,
        U_MODELVIEW,
        U_MODELVIEW_INVERSE_TRANSPOSE,
        U_MATERIAL_AMBIENT,
        U_MATERIAL_DIFFUSE,
        U_MATERIAL_SPECULAR,
        U_MATERIAL_SHININESS,
        //light 0
        U_LIGHT0_POSITION,
        U_LIGHT0_COLOR,
        U_LIGHT0_POWER,
        U_LIGHT0_KC,
        U_LIGHT0_KL,
        U_LIGHT0_KQ,
        U_LIGHT0_TYPE,
        U_LIGHT0_SPOTDIRECTION,
        U_LIGHT0_COSCUTOFF,
        U_LIGHT0_COSINNER,
        U_LIGHT0_EXPONENT,

        U_LIGHTENABLED,
        U_NUMLIGHTS,
        //textures
        U_COLOR_TEXTURE_ENABLED,
        U_COLOR_TEXTURE,
        //text
        U_TEXT_ENABLED,
        U_TEXT_COLOR,
        U_TOTAL,
    };

public:
    SP2();
    ~SP2();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    unsigned m_vertexArrayID;
    Mesh *meshList[NUM_GEOMETRY];
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    MS modelStack, viewStack, projectionStack;
	
	PipePuzzle controlpuzzle;

    Light light[1];

    //text
    //float fontWidth[256];

    std::vector<WallCollision> collisionWallCell;
    std::vector<WallCollision> collisionWallCorridor;
    std::vector<WallCollision> collisionWallCorridor2;
    std::vector<WallCollision> collisionWallControlRoom;

    std::vector<ItemCollision> collisionItemCell;
    std::vector<ItemCollision> collisionItemCorridor;
    std::vector<ItemCollision> collisionItemCorridor2;
    std::vector<ItemCollision> collisionItemControlRoom;

    std::vector<ItemInteraction> itemCell;
    std::vector<ItemInteraction> itemCorridor;
    std::vector<ItemInteraction> itemControlRoom;

    //reset
    void Reset();

    void RenderMesh(Mesh* mesh, bool enableLight);
    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderDialogueOnScreen(std::string text, Color color, float size);
    
    void RenderSkybox();
    void RenderGround();
    void RenderLayout();
    void RenderPlayer();
	
	void RenderPuzzle();

    bool viewOptions;
    void RenderUI();
};

#endif