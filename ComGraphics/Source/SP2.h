/******************************************************************************/
/*!
\file	SP2.h
\author 3000 Lines Studio
\brief
SP2 header file
*/
/******************************************************************************/

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

#include "Bullet.h"
#include "Interaction.h"

#include "Shop.h"
#include "Pause.h"

#include <cstdlib>

#include <vector>
#include <map>

#ifndef SP2_H
#define SP2_H

/******************************************************************************/
/*!
Class SP2:
\brief	Stores all information needed for SP2 Scene
*/
/******************************************************************************/
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
        GEO_RABBIT_SKYBOX,

        GEO_GROUND,

        GEO_SPACESHIP,

        GEO_LOADTOP,
        GEO_LOADBTM,

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
        GEO_DETERMINATION,
        GEO_MEWMEWKISSYCUTIE,
        GEO_DOGFOOD,
        GEO_TRASH,
        GEO_BOOK,
        GEO_LMAO,   //a sh*tty joke
        GEO_JAGCUPBOARDS,
        GEO_TECHCHAIR,
        //POSTERS
        GEO_POSTER,
        GEO_POSTSAO,
        GEO_POSTPUNCH,
        GEO_POSTKYOJ,
        GEO_POSTFATE,
        GEO_POSTDGRAY,
        GEO_POSTRETT,
        GEO_POSTELECTION,
        GEO_SECRETGUIDE,
        GEO_KEYBOARD,
        //SIGNBOARDS
        GEO_SIGNBOARD,
        GEO_SIGNLAB,
        GEO_SIGNCTRL,
        GEO_SIGNCANTEEN,
        GEO_SIGNROOM1,
        GEO_SIGNROOM2,
        GEO_SIGNROOM3,
        GEO_SIGNROOM4,
        GEO_SIGNLEFT,       //sign left and right are directions
        GEO_SIGNRIGHT,      //   signs after the hangar entrance
        GEO_SIGNSHOP,
        GEO_SIGNTOILET,
        GEO_SIGNFARM,
        GEO_SIGNCOVER,
        GEO_SIGNCOMP1,
        GEO_SIGNCOMP2,
        GEO_SIGNCOMP3,
        GEO_SIGNEASTER,

        //shop displays
        GEO_COOKBOOK,
        GEO_ANIPOSTER,
        GEO_CHONKEY,
        GEO_SHOVEL,
        GEO_BOARDGAME,
        GEO_BUTTERPIE,
        GEO_NOVEL,
        GEO_JOKEBOOK,


        //^^^

        //Gifts
        GEO_ITEMSELECT,
        GEO_ITEM1,
        GEO_ITEM2,
        GEO_SHOPDISPLAY,

        //Shop
        GEO_SHOP_ARROW,

		//dev texture
		//GEO_DEVTEXTURE,
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
		GEO_SPAGHETTOROTTEN,
		GEO_FENCE,
        GEO_FARM,
        GEO_SHED,
        GEO_WEED,
		GEO_WEEDICON,
		GEO_FARMPLANT,

        //UI stuff
        GEO_CROSSHAIRS,
		GEO_INVENTORY,

        GEO_CURSOR,
        GEO_DIALOGUEOPTION,

        GEO_HEART,
        GEO_LOVEMETER_BAR,
        GEO_BOSSHEALTH_BAR,
        GEO_PLAYERHEALTH_BAR,

        //Switches
        GEO_SWITCH_1,
        GEO_SWITCH_2,
        GEO_SWITCH_3,
        GEO_SWITCH_4,
        GEO_CHECK_1,
        GEO_CHECK_2,
        GEO_CHECK_3,
        GEO_CHECK_4,
        GEO_CHECKER_BUTTON,

        //rabbit bullet
        GEO_BULLET,
        GEO_E_BULLET,
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
        //light 0 (DIRECTIONAL)
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
        //light 1 (Hangar spot 1)
        U_LIGHT1_POSITION,
        U_LIGHT1_COLOR,
        U_LIGHT1_POWER,
        U_LIGHT1_KC,
        U_LIGHT1_KL,
        U_LIGHT1_KQ,
        U_LIGHT1_TYPE,
        U_LIGHT1_SPOTDIRECTION,
        U_LIGHT1_COSCUTOFF,
        U_LIGHT1_COSINNER,
        U_LIGHT1_EXPONENT,
        //light 2 (Hangar spot 2)
        U_LIGHT2_POSITION,
        U_LIGHT2_COLOR,
        U_LIGHT2_POWER,
        U_LIGHT2_KC,
        U_LIGHT2_KL,
        U_LIGHT2_KQ,
        U_LIGHT2_TYPE,
        U_LIGHT2_SPOTDIRECTION,
        U_LIGHT2_COSCUTOFF,
        U_LIGHT2_COSINNER,
        U_LIGHT2_EXPONENT,
        //light 3 (Lab point green)
        U_LIGHT3_POSITION,
        U_LIGHT3_COLOR,
        U_LIGHT3_POWER,
        U_LIGHT3_KC,
        U_LIGHT3_KL,
        U_LIGHT3_KQ,
        U_LIGHT3_TYPE,
        //light 4 (Toilet spot)
        U_LIGHT4_POSITION,
        U_LIGHT4_COLOR,
        U_LIGHT4_POWER,
        U_LIGHT4_KC,
        U_LIGHT4_KL,
        U_LIGHT4_KQ,
        U_LIGHT4_TYPE,
        U_LIGHT4_SPOTDIRECTION,
        U_LIGHT4_COSCUTOFF,
        U_LIGHT4_COSINNER,
        U_LIGHT4_EXPONENT,
        //light 5 (control puzzle spot)
        U_LIGHT5_POSITION,
        U_LIGHT5_COLOR,
        U_LIGHT5_POWER,
        U_LIGHT5_KC,
        U_LIGHT5_KL,
        U_LIGHT5_KQ,
        U_LIGHT5_TYPE,
        U_LIGHT5_SPOTDIRECTION,
        U_LIGHT5_COSCUTOFF,
        U_LIGHT5_COSINNER,
        U_LIGHT5_EXPONENT,
        //light 6 (Shop spot 1)
        U_LIGHT6_POSITION,
        U_LIGHT6_COLOR,
        U_LIGHT6_POWER,
        U_LIGHT6_KC,
        U_LIGHT6_KL,
        U_LIGHT6_KQ,
        U_LIGHT6_TYPE,
        U_LIGHT6_SPOTDIRECTION,
        U_LIGHT6_COSCUTOFF,
        U_LIGHT6_COSINNER,
        U_LIGHT6_EXPONENT,
        //light 7 (farm sign)
        U_LIGHT7_POSITION,
        U_LIGHT7_COLOR,
        U_LIGHT7_POWER,
        U_LIGHT7_KC,
        U_LIGHT7_KL,
        U_LIGHT7_KQ,
        U_LIGHT7_TYPE,
        


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
    std::vector<PlayerBullet> playerbullet;
    std::vector<EnemyBullet> enemybullet;

    //PlayerBullet bullet[100];
    float pitch, yaw;
    double elapsedTime, e_elapsedTime;

    Enemy enemy;
    Player player;

    float delayer;
    float delayBuffer;
	float soundtimer;
    float rotator;
    float floodlevel;
    float lightpower;
    float lightpos;
    float loadDown;
    float loadUp;
    //Testing Minimap
    MS minimappp;

    Minimapcamera minimappy;
    LightPuzzle lightpuzz;
    //float one, two, three, four;

    Light light[2];

    //float fontWidth[256];

    //reset
    void Reset();

    std::string locations[LOCATION_TOTAL];
    void CheckCharacterLocation();

    void RenderMesh(Mesh* mesh, bool enableLight);
    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderObjectOnScreen(Mesh* mesh, float x, float y, float scalex = 1, float scaley = 1, float rotate = 0);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderDialogueOnScreen(std::string text, Color color, float size);
	void SP2::RenderInventoryOnScreenStatic(Mesh* mesh, float x, float y);
	void SP2::RenderInventoryOnScreenRotating(Mesh* mesh, float x, float y);
	void SP2::RenderTimeOnScreen(Mesh* mesh, float x, float y);
    
    void RenderSkybox();
    void RenderGround();
	void RenderPuzzle();
    void stemmieShop();
    void compactBallrender();
	void RenderInventory();
	void RenderTime();
    bool viewOptions;
    void RenderUI();
    void RenderLoveMeter();
    void renderPuzzle();
    void RenderSignboards();

    void RenderGates();

	void loadSpaghetti();
	void loadInv();
    void chonSecret();
    void pauseGame();
    void rabbitFightTransition();
    void rabbitFightTransitionCheckCursor(double dt);
    void loadRabbitGame();
	void loadWeedGame();
    void RenderFightSkybox();
    void puzzleLogic();
    void UpdateInventory(double dt);

    //RENDER STUFF.
    void RenderLayout();
    void RenderHangar();
    void RenderRoom1();
    void RenderRoom2();
    void RenderRoom3();
    void RenderRoom4();
    void RenderShop();
    void RenderCanteen();
    void RenderPlantation();
    void RenderControlRoom();
    void RenderLab();
    void RenderToilet();

    void RenderChonGame();
    void RenderInShop();

    //rabbit fight
    void playerShoot(double dt);
    void enemyShoot(double dt);
    void bulletMove(double dt);
    void RenderBullets();
    void bulletUpadtes(double dt);
    void checkP_BulletCollide(PlayerBullet& bullet);
    void checkE_BulletCollide(EnemyBullet& bullet);

    void RenderPlayerHealth();
    void RenderBossHealth();

    float BULLETSPEED = 200.f;

    bool spreadfire;
    bool unlimitedbulletworks = false;
    bool rapidfireon = false;
    int circlecount = 0;
    int ubwcount = 0;
    int rapidfirecount = 0;
    int powerspike = 1;
    float invulnerable = 5;

    //cursor for dialogue, pause, shop
    void RenderCursor();

    //initialise collisions
    void loadCollisions();

    void shoptemp();
    void RotateDisplay();

    float gateobjs[10];
    /*0,1 = hang 1,2            10, 11, 12, 13 = room 1, 2, 3, 4
    2, 3 = lab
    4, 5, = canteen
    6, 7, 8, 9 = control*/

    void gateOpenInteractions();
    void gateUpdate(double dt);
    bool gateopenBool[4];
    GateInteraction gatebounds[4];

    //for shop OBJs
    float rotating;

    //sleep
    double sleepTime;
    void Sleep(double dt);
    
    //Chon game variables
    bool ball[5];   
    /*0 = yellow,
    1 = black, 
    2 = white, 
    3 = blue, 
    4 = red*/
    bool pickupcheck;
    int pickupCounter;
    bool posreach1;
    bool posreach2;
    bool posreach3;
    int yellowcount;
    int bluecount;
    int redcount;
    float ballyellX, ballyellZ, ballbluX, ballbluZ, ballredX, ballredZ;
    float ballwhitey, ballblacky;
    void ballboundfunct();
    void ballmoveCheck();
    int postercounter;
    void compactMovement(bool first, bool second, bool third, int i);
    chonBallInteraction ballbounds[5];

    void ChonGameUpdate();

    Shop shop;
    Pause pause;

    void FinishGameUpdate(double dt);
    void FinishGame();

	std::map<int, Gift>invmap;
	std::map<int, GEOMETRY_TYPE>modelmap;
};

#endif