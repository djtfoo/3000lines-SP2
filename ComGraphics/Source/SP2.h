#include "Scene.h"
#include "Mesh.h"
#include "Camera3.h"
#include "MiniMapCamera.h"
#include "MatrixStack.h"
#include "Light.h"

#include "Collision.h"

#include "LightPuzzle.h"
#include "GameState.h"
#include "Item.h"
#include "DialogueProcessor.h"

#include "Interaction.h"

#include <cstdlib>

#include <vector>
#include <map>

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
        //Minimap
        GEO_MAP,

        GEO_LAYOUT,
        GEO_GATE1,
        GEO_GATE2,

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
        //GEO_AARON,
        //GEO_MERCHANT,
        
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
		GEO_BED,
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
		GEO_TOILETFLOOD,


		//day night
		GEO_DAYNIGHTICON,

        GEO_COUNTER,
        GEO_SHOPSHELF,
        GEO_LABCOUNTER1,
        GEO_LABCOUNTER_CORNER,
        GEO_SPHEREYELLOW,
        GEO_SPHEREBLUE,
        GEO_SPHERERED,
        GEO_SPHEREBLACK,
        GEO_SPHEREWHITE,


		GEO_SPAGHETTO,
		GEO_FENCE,
        GEO_FARM,
        GEO_SHED,
        GEO_WEED,

		GEO_PIPETYPE1,
		GEO_PIPETYPE2,
		GEO_PIPETYPE3,
		GEO_PIPETYPE4,

        GEO_CROSSHAIRS,
		GEO_INVENTORY,

        //Switches
        GEO_SWITCH_1,
        GEO_SWITCH_2,
        GEO_SWITCH_3,
        GEO_SWITCH_4,
        GEO_CHECK_1,
        GEO_CHECK_2,
        GEO_CHECK_3,
        GEO_CHECK_4,
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
    bool chonFloat;

    MS modelStack, viewStack, projectionStack;
	std::vector<Vector3> weedgame;

	int delayer;
	int rotator;
	float floodlevel;
    float lightpower;
    float lightpos;
    float delayBuffer;

    //Testing Minimap
    MS minimappp;

    Minimapcamera minimappy;
    LightPuzzle lightpuzz;
    //float one, two, three, four;

    Light light[1];

    //float fontWidth[256];

    //reset
    void Reset();

    std::string locations[LOCATION_TOTAL];
    void CheckCharacterLocation();

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
	void RenderRoom();
    void chonLab();
    void veeControlroom();
    void jasimCanteen();
    void loadHangar();
	void renderFarm();
	void RenderNPC();
	void RenderInventory();
	void RenderTime();
    bool viewOptions;
    void RenderUI();
    void renderPuzzle();

    void RenderMinimap();
    void RenderGates();

	void loadInv();
    void loadFree();
    void loadShop();
    void loadWSGame();
    void loadChonGame();
    void loadVeeGame();
    void pauseGame();
    void loadRabbitGame();
	void loadWeedGame();

    //initialise collisions
    void loadCollisions();

    void shoptemp();
    void RotateDisplay();
    void pauseAnimation(double dt);


    float objx, objy;

    float gate1pos, gate2pos, gatemove;
    void gateOpenInteractions();

    float rotating;
    float ptxt1;    //pause text 1
    float ptxt2;
    float ptxt3;
    float ptxtexit;
    
    //Chon game variables
    bool ball[5];   
    /*0 = yellow, 
    1 = black, 
    2 = white, 
    3 = blue, 
    4 = red*/
    bool pickupcheck;
    float ballfloat, ballfloat1, ballfloat2, ballfloat3, ballfloat4;
    void ballboundfunct();
    chonBallInteraction ballbounds[5];

   

	std::map<int, Gift>invmap;
	std::map<int, GEOMETRY_TYPE>modelmap;
};

#endif