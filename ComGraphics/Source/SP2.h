#include "Scene.h"
#include "Mesh.h"
#include "Camera3.h"
#include "MiniMapCamera.h"
#include "MatrixStack.h"
#include "Light.h"

#include "Collision.h"

#include "PipePuzzle.h"
#include "GameState.h"
#include "Item.h"

#include <cstdlib>

#include <vector>
#include <map>

#ifndef SP2_H
#define SP2_H

class SP2 : public Scene
{
    enum LOCATION
    {
        HANGAR,
        LABORATORY,
        CONTROLROOM,
        CORRIDOR,
        //can split corridor into different segments
        CANTEEN,
        //TOILET,
        OUTSIDE,

        LOCATION_TOTAL
    };

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
        //Minimap
        GEO_MAP,

        GEO_LAYOUT,

        //player character
        GEO_PLAYER_HEAD,
        GEO_PLAYER_BODY,
        GEO_PLAYER_UPPERARM,
        GEO_PLAYER_LOWERARM,
        GEO_PLAYER_LEG,

        //Our NPCs
        GEO_ADOLPH,
        GEO_STEMMIE,
        GEO_STEMMIE_FACE,
        GEO_WENGSTANG,
        GEO_VEE,
        GEO_JASIM,
        GEO_CHON,
        GEO_AARON,
        GEO_MERCHANT,
        
        //text
        GEO_TEXT,
        GEO_DIALOGUEBOX,
        GEO_HUNGER_BAR,
        GEO_HUNGER_UI,
        GEO_DAYS_UI,
        GEO_SHOPUI,

        //Objects
        GEO_BENCH,
        GEO_TOOLBOX,
        GEO_VENDINGMACHINE,
        GEO_TABLE,
        GEO_HAMMER,
        //newnew today vvv
        GEO_BOXCRATE,
        GEO_CHAIR,
        GEO_CONTROLROOM,
        GEO_FIREEXTINGUISHER,
        GEO_FRIDGE,
        GEO_PLANT,
        GEO_SHELF,
        GEO_TEMSHOP,
        GEO_UMBRELLASTAND,
        GEO_TOILBOWL,
        GEO_LMAO,   //a sh*tty joke
        //^^^

		//Gifts
		GEO_ITEMSELECT,
		GEO_ITEM1,
		GEO_ITEM2,

        GEO_SHOPDISPLAY,

        //Light Source
        GEO_LIGHTBALL,

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


		//day night
		GEO_DAYNIGHTICON,

        GEO_COUNTER,
        //GEO_SHELF,
        //GEO_FRIDGE,

		GEO_PIPETYPE1,
		GEO_PIPETYPE2,
		GEO_PIPETYPE3,
		GEO_PIPETYPE4,

        GEO_CROSSHAIRS,
		GEO_INVENTORY,
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

    float vibrateY, vibrateX;
    float chonFloaty;
    float lightpower;
    bool chonFloat;

    MS modelStack, viewStack, projectionStack;
	
	PipePuzzle controlpuzzle;
	int delayer;
	int rotator;
	float daynighttime;

    //Testing Minimap
    MS minimappp;

    Minimapcamera minimappy;

    Light light[1];

    //float fontWidth[256];

    //reset
    void Reset();

    void RenderMesh(Mesh* mesh, bool enableLight);
    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderObjectOnScreen(Mesh* mesh, float x, float y, float scalex = 1, float scaley = 1);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderDialogueOnScreen(std::string text, Color color, float size);
	void SP2::RenderInventoryOnScreenStatic(Mesh* mesh, float x, float y);
	void SP2::RenderInventoryOnScreenRotating(Mesh* mesh, float x, float y);
	void SP2::RenderTimeOnScreen(Mesh* mesh, float x, float y);
    
    void RenderSkybox();
    void RenderGround();
    void RenderPlayer();
	void RenderPuzzle();
    void stemmieShop();
    void chonLab();
    void veeControlroom();
    void jasimCanteen();
    void loadHangar();
	void RenderNPC();
	void RenderInventory();
	void RenderTime();
    bool viewOptions;
    void RenderUI();

    void RenderMinimap();

	void loadInv();
    void loadFree();
    void loadShop();
    void loadWSGame();
    void loadChonGame();
    void loadVeeGame();
    void pauseGame();
    void loadRabbitGame();

    void shoptemp();
    void RotateDisplay();
    void pauseAnimation(double dt);


    int playerHung;
    float objx, objy;

    float rotating;
    float ptxt1;    //pause text 1
    float ptxt2;
    float ptxt3;
    float ptxtexit;

	std::map<int, Gift>invmap;
	std::map<int, GEOMETRY_TYPE>modelmap;
};

#endif