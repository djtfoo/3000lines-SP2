#include "Scene.h"
#include "Mesh.h"
#include "Camera3.h"
#include "MatrixStack.h"
#include "Light.h"

#include "Character.h"
#include "Collision.h"

#include <vector>

#ifndef ASSIGNMENT_3_H
#define ASSIGNMENT_3_H

class Assignment3 : public Scene
{
    enum WAKEUP_STATE
    {
        WAKEUP_SITUP,
        WAKEUP_PAUSE,
        WAKEUP_TURN,
        WAKEUP_GETDOWN,
        WAKEUP_TOTAL
    };

    enum QUICKTIME_STATE
    {
        QUICKTIME_CLOSEDOOR,
        QUICKTIME_DIALOGUE,
        QUICKTIME_ROTATEARM,
        QUICKTIME_PAUSE1,
        QUICKTIME_TRANSFORM,
        QUICKTIME_PAUSE2,
        QUICKTIME_CARDDISPLAY1,
        QUICKTIME_CARDROTATE,
        QUICKTIME_CARDDISPLAY2,
        QUICKTIME_TRANSFORM2,
        QUICKTIME_TOTAL
    };

    enum GEOMETRY_TYPE
    {
        //battle
        GEO_TRANSFORMLIGHT,
        GEO_HEALTHBAR,
        GEO_CROSSHAIRS,
        GEO_BULLETPLAYER,
        GEO_BULLETENEMY,
        GEO_EXPLOSION,

        GEO_BLACKSCREEN,
        //starting room: cell
        GEO_CELL_LEFT,
        GEO_CELL_RIGHT,
        GEO_CELL_TOP,
        GEO_CELL_BOTTOM,
        GEO_CELL_BACK,
        GEO_CELL_BED,
        GEO_CELL_BARS,
        GEO_CELL_DOOR,
        GEO_CELL_CCTV,
        //corridor
        GEO_CORRIDOR_WALL,
        GEO_CORRIDOR_CEILING,
        GEO_CORRIDOR_FLOOR,
        GEO_STAIRS_HORIZONTAL,
        GEO_STAIRS_VERTICAL,
        GEO_CORRIDOR_CREEPY,
        //corridor 2
        GEO_CORRIDOR2_FLOOR,
        GEO_CORRIDOR2_WALL,
        GEO_CORRIDOR2_DOORGATE,
        GEO_CORRIDOR2_DOOR,
        //control room
        GEO_ROOM_FRONTSIDE,
        GEO_ROOM_FRONTTOP,
        GEO_ROOM_BACK,
        GEO_ROOM_LEFT,
        GEO_ROOM_RIGHT,
        GEO_ROOM_TOP,
        GEO_ROOM_BOTTOM,
        GEO_ROOM_LEFT_OVERLAY,
        //control room objects
        GEO_HOLOGRAM_EARTH,
        GEO_HOLOGRAM_BASE,
        GEO_CONTROLPANEL1,
        GEO_CONTROLPANEL2,
        GEO_CHAIR,
        GEO_NUMLOCK,
        //space
        GEO_SPACE_FRONT,
        GEO_SPACE_TOP,
        GEO_SPACE_BOTTOM,
        GEO_SPACE_LEFT,
        GEO_SPACE_RIGHT,
        //lab
        GEO_LAB_LEFT,
        GEO_LAB_RIGHT,
        GEO_LAB_FRONT,
        GEO_LAB_BACK,
        GEO_LAB_TOP,
        GEO_LAB_BOTTOM,
        GEO_LAB_SCREEN,
		//characters/items
        GEO_MAN,
        GEO_MAN_ARM,
        GEO_MOTHERSHIP,
		GEO_DRAGON,
        //  GEO_STAGBEETLE,
        GEO_CARD_MOTHERSHIP,
        GEO_CARD_DRAGON,
        GEO_CARD_BACK,
        //text
        GEO_TEXT,
        GEO_DIALOGUEBOX,
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
        //light 1
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
        //light 2
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
        //light 3
        U_LIGHT3_POSITION,
        U_LIGHT3_COLOR,
        U_LIGHT3_POWER,
        U_LIGHT3_KC,
        U_LIGHT3_KL,
        U_LIGHT3_KQ,
        U_LIGHT3_TYPE,
        U_LIGHT3_SPOTDIRECTION,
        U_LIGHT3_COSCUTOFF,
        U_LIGHT3_COSINNER,
        U_LIGHT3_EXPONENT,
        //light 4
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
        //light 5
        U_LIGHT5_POSITION,
        U_LIGHT5_COLOR,
        U_LIGHT5_POWER,
        U_LIGHT5_KC,
        U_LIGHT5_KL,
        U_LIGHT5_KQ,
        U_LIGHT5_TYPE,
        //light 6
        U_LIGHT6_POSITION,
        U_LIGHT6_COLOR,
        U_LIGHT6_POWER,
        U_LIGHT6_KC,
        U_LIGHT6_KL,
        U_LIGHT6_KQ,
        U_LIGHT6_TYPE,

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
    Assignment3();
    ~Assignment3();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    unsigned m_vertexArrayID;
    Mesh *meshList[NUM_GEOMETRY];
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    Camera3 camera;
    MS modelStack, viewStack, projectionStack;

    Light light[7];

    //bool lightOn;

    bool lockedDown;

    bool battleMode;
    bool flightMode;
    bool viewOptions;
    QUICKTIME_STATE qt_state;

    //characters
    Player Player;
    Enemy Enemy;

    bool wakeUp;
    WAKEUP_STATE wake_state;
    float rotation_situp, rotation_turn;

    //text
    //float fontWidth[256];

    //for interactions
    double elapsedTime, maxElapsedTime;
    int interactNum;
    int dialogueNum;

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

    //door
    Door PrisonDoor;
    Door ControlRoomDoor;
    ItemCollision itemDoorClose;
    ItemCollision itemControlRoomDoor;
    ItemCollision itemDoorOpen;

    //battle
    std::vector<FlightCollision> flightCollision;
    std::vector<WallCollision> flightWall;
    std::vector<Bullet> playerBullets;
    std::vector<Bullet> enemyBullets;

    Hitbox playerHitbox;
    Hitbox enemyHitbox;
    double enemyROFelapsedTime;

    StairsCollision stairs;

    //interactions
    bool haveInteraction;
    bool interactDialogue;
    bool haveQuicktime;
    bool interactPrisonDoor;

    //animation
    float rotate_hologram;
    float rotate_hologram_speed;

    //quicktime animations
    float qt_rotate_arm;
    float qt_explosion_size;
    float qt_rotate_card;

    //battle outcome
    bool gameLose;
    bool winAnimation;
    bool blackScreen;
    bool splashScreen;

    void SetNonGlossMaterial(Mesh* mesh);

    //render
    void RenderMesh(Mesh* mesh, bool enableLight);
    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderDialogueOnScreen(std::string text, Color color, float size);
    void RenderCardOnScreen(GEOMETRY_TYPE TYPE);
    void RenderUI();
    void RenderBlackScreen();

    void RenderHealth();
    void RenderCrosshairs();

    void RenderCell();
	void RenderCorridor();
    void RenderStairs();
    void RenderCorridor2();
    void RenderDoor();
    void RenderEscapeDoor();
	void RenderControlRoom();
    void RenderLab();

    void RenderSpace();
    void RenderEnemy();
    void RenderHologram();
    void RenderControlPanels();

    void RenderPlayer();
    void RenderBullets();

    //interactions/quicktime events
    void WakeUp(double dt);

    void CheckInteraction(const std::vector<ItemInteraction>& itemInteract);
    void Interact();
    void Dialogue();

    void CheckDoor(Door& door, double dt);
    void MoveDoor(Door& door, double dt);

    void CheckQuicktimeEvent();
    void QuickTime(double dt);

    void WinAnimation(double dt);

    //battle
    void PlayerShoot(double dt);
    void EnemyShoot(double dt);
    void MoveBullets(double dt);
    void CheckPlayerBullets(Bullet& bullet);
    void CheckEnemyBullets(Bullet& bullet);
    void EditHitboxes();

    //reset
    void Reset();
};

#endif