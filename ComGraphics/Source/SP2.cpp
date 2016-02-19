#include "SP2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"

#include <sstream>

#include "Application.h"
#include "SharedData.h"

SP2::SP2()
{
    chonFloat = false;
    chonFloaty = vibrateX = vibrateY = 0;
    // Set background color to dark blue
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //Enable depth buffer and depth testing
    glEnable(GL_DEPTH_TEST);

    //Enable back face culling
    glEnable(GL_CULL_FACE);

    //Default to fill mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Generate a default VAO for now
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    //Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Initialize camera settings

    //Vector3 playerPos = (SharedData::GetInstance()->player->position_);

    //minimappy.Init(Vector3(playerPos.x, (playerPos.y + 500), playerPos.z), Vector3(0, -1, 0), Vector3(1 , 0 , 0));


    Mtx44 projection;
    projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
    projectionStack.LoadMatrix(projection);

    //Load vertex and fragment shaders
    m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
    // Get a handle for our "MVP" uniform
    m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
    m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
    m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
    m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
    m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
    m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
    m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
    //light 0
    m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
    m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
    m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
    m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
    m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
    m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
    m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
    m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
    m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
    m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
    m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

    m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
    m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
    //Get a handle for our "colorTexture" uniform
    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
    //Get a handle for our "textColor" uniform
    m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
    m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
    // Use our shader
    glUseProgram(m_programID);

    //User Interface/ Heads up displays
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image/Text/small fonts.tga");

    meshList[GEO_DIALOGUEBOX] = MeshBuilder::GenerateQuad("dialogue_box", Color(0, 0, 0), 80, 20);
    meshList[GEO_DIALOGUEBOX]->textureID = LoadTGA("Image/Text/dialogue box.tga");

    //
    meshList[GEO_HUNGER_BAR] = MeshBuilder::GenerateOBJ("hunger_bar", "OBJ/bar_bar.obj");
    meshList[GEO_HUNGER_BAR]->textureID = LoadTGA("Image/hunger_bar.tga");

    meshList[GEO_HUNGER_UI] = MeshBuilder::GenerateQuad("hunger_ui", Color(0, 0, 0), 100, 5);
    //meshList[GEO_HUNGER_UI]->textureID = LoadTGA("Image/Text/hunger_uibar.tga");

    meshList[GEO_DAYS_UI] = MeshBuilder::GenerateCircle("days_ui", Color(0, 0, 1), 10, 10);
    //meshList[GEO_DAYS_UI]->textureID = LoadTGA("Image/Text/day_ui.tga");

    meshList[GEO_SHOPUI] = MeshBuilder::GenerateQuad("shop_ui", Color(1, 0.8, 0.8),1,1);
    meshList[GEO_SHOPUI]->textureID = LoadTGA("Image/layout/canteen_walls.tga");


    //skybox
    meshList[GEO_SKYBOX_LEFT] = MeshBuilder::GenerateQuad("skybox_left", Color(1, 1, 1), 1000, 1000);
    meshList[GEO_SKYBOX_LEFT]->textureID = LoadTGA("Image/Skybox/redplanet_left.tga");

    meshList[GEO_SKYBOX_RIGHT] = MeshBuilder::GenerateQuad("skybox_right", Color(1, 1, 1), 1000, 1000);
    meshList[GEO_SKYBOX_RIGHT]->textureID = LoadTGA("Image/Skybox/redplanet_right.tga");

    meshList[GEO_SKYBOX_TOP] = MeshBuilder::GenerateQuad("skybox_top", Color(1, 1, 1), 1000, 1000);
    meshList[GEO_SKYBOX_TOP]->textureID = LoadTGA("Image/Skybox/redplanet_top.tga");

    meshList[GEO_SKYBOX_BOTTOM] = MeshBuilder::GenerateQuad("skybox_bottom", Color(1, 1, 1), 1000, 1000);
    meshList[GEO_SKYBOX_BOTTOM]->textureID = LoadTGA("Image/Skybox/redplanet_bottom.tga");

    meshList[GEO_SKYBOX_BACK] = MeshBuilder::GenerateQuad("skybox_back", Color(1, 1, 1), 1000, 1000);
    meshList[GEO_SKYBOX_BACK]->textureID = LoadTGA("Image/Skybox/redplanet_back.tga");

    meshList[GEO_SKYBOX_FRONT] = MeshBuilder::GenerateQuad("skybox_front", Color(1, 1, 1), 1000, 1000);
    meshList[GEO_SKYBOX_FRONT]->textureID = LoadTGA("Image/Skybox/redplanet_front.tga");

    meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 2000, 2000);
    meshList[GEO_GROUND]->textureID = LoadTGA("Image/ground2.tga");

    meshList[GEO_MAN] = MeshBuilder::GenerateOBJ("man", "OBJ/man.obj");
    meshList[GEO_MAN]->textureID = LoadTGA("Image/man.tga");

	meshList[GEO_DEVTEXTURE] = MeshBuilder::GenerateQuad("devtexture", Color(1, 1, 1), 1, 1);
	meshList[GEO_DEVTEXTURE]->textureID = LoadTGA("Image/devtexture.tga");

    //Render NPCs
    meshList[GEO_ADOLPH] = MeshBuilder::GenerateOBJ("man", "OBJ/machoaf.obj");
    meshList[GEO_ADOLPH]->textureID = LoadTGA("Image/macho.tga");
    meshList[GEO_ADOLPH]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_ADOLPH]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_ADOLPH]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_ADOLPH]->material.kShininess = 1.f;

    meshList[GEO_STEMMIE] = MeshBuilder::GenerateOBJ("stemmie", "OBJ/hOi.obj");
    meshList[GEO_STEMMIE]->textureID = LoadTGA("Image/temmie.tga");

    meshList[GEO_STEMMIE_FACE] = MeshBuilder::GenerateQuad("stemmieface", Color(1, 1, 1), 1, 1);
    meshList[GEO_STEMMIE_FACE]->textureID = LoadTGA("Image/hOiWAT.tga");

    meshList[GEO_WENGSTANG] = MeshBuilder::GenerateOBJ("wengstang", "OBJ/toriel.obj");
    meshList[GEO_WENGSTANG]->textureID = LoadTGA("Image/toriel.tga");

    meshList[GEO_VEE] = MeshBuilder::GenerateOBJ("vee", "OBJ/asgore.obj");
    meshList[GEO_VEE]->textureID = LoadTGA("Image/asgore.tga");

    meshList[GEO_JASIM] = MeshBuilder::GenerateOBJ("jasim", "OBJ/sans.obj");
    meshList[GEO_JASIM]->textureID = LoadTGA("Image/sans.tga");

    meshList[GEO_CHON] = MeshBuilder::GenerateOBJ("chon", "OBJ/chon.obj");
    meshList[GEO_CHON]->textureID = LoadTGA("Image/chonUV.tga");

    //GEO_MAP
    meshList[GEO_MAP] = MeshBuilder::GenerateQuad("minimap", Color(1, 1, 1), 10, 10);
    
    meshList[GEO_LAYOUT] = MeshBuilder::GenerateOBJ("big map", "OBJ/bigarea.obj");
    meshList[GEO_LAYOUT]->textureID = LoadTGA("Image/layout/control_walls.tga");

    //Objects
    meshList[GEO_BENCH] = MeshBuilder::GenerateOBJ("bench", "OBJ/bench.obj");
    meshList[GEO_BENCH]->textureID = LoadTGA("Image/bench.tga");

    meshList[GEO_TOOLBOX] = MeshBuilder::GenerateOBJ("toolbox", "OBJ/toolbox.obj");
    meshList[GEO_TOOLBOX]->textureID = LoadTGA("Image/toolbox.tga");

    meshList[GEO_VENDINGMACHINE] = MeshBuilder::GenerateOBJ("vendingmachine", "OBJ/vendingmachine.obj");
    meshList[GEO_VENDINGMACHINE]->textureID = LoadTGA("Image/vendingmachine.tga");

    meshList[GEO_TABLE] = MeshBuilder::GenerateOBJ("table", "OBJ/table.obj");
    meshList[GEO_TABLE]->textureID = LoadTGA("Image/table.tga");

    meshList[GEO_HAMMER] = MeshBuilder::GenerateOBJ("hammer", "OBJ/hammer.obj");
    meshList[GEO_HAMMER]->textureID = LoadTGA("Image/hammer.tga");

    //New Objects
    meshList[GEO_BOXCRATE] = MeshBuilder::GenerateOBJ("boxcrate", "OBJ/boxcrate.obj");
    meshList[GEO_BOXCRATE   ]->textureID = LoadTGA("Image/boxcrate.tga");

    meshList[GEO_CHAIR] = MeshBuilder::GenerateOBJ("chair", "OBJ/chair.obj");
    meshList[GEO_CHAIR]->textureID = LoadTGA("Image/chair.tga");

    meshList[GEO_CONTROLROOM] = MeshBuilder::GenerateOBJ("controlroom", "OBJ/controlroom.obj");
    meshList[GEO_CONTROLROOM]->textureID = LoadTGA("Image/controlroom.tga");

    meshList[GEO_FIREEXTINGUISHER] = MeshBuilder::GenerateOBJ("fireextinguisher", "OBJ/fireextinguisher.obj");
    meshList[GEO_FIREEXTINGUISHER]->textureID = LoadTGA("Image/fireextinguisher.tga");

    meshList[GEO_FRIDGE] = MeshBuilder::GenerateOBJ("fridge", "OBJ/fridgedefault.obj");
    meshList[GEO_FRIDGE]->textureID = LoadTGA("Image/fridgedefault.tga");

    meshList[GEO_PLANT] = MeshBuilder::GenerateOBJ("plant", "OBJ/plant.obj");
    meshList[GEO_PLANT]->textureID = LoadTGA("Image/plant.tga");

    meshList[GEO_SHELF] = MeshBuilder::GenerateOBJ("shelf", "OBJ/shelf.obj");
    meshList[GEO_SHELF]->textureID = LoadTGA("Image/shelf.tga");

    meshList[GEO_TEMSHOP] = MeshBuilder::GenerateOBJ("temshop", "OBJ/temshop.obj");
    meshList[GEO_TEMSHOP]->textureID = LoadTGA("Image/temshop.tga");

    meshList[GEO_UMBRELLASTAND] = MeshBuilder::GenerateOBJ("umbrellastand", "OBJ/umbrellastand.obj");
    meshList[GEO_UMBRELLASTAND]->textureID = LoadTGA("Image/umbrellastand.tga");

    meshList[GEO_SHOPDISPLAY] = MeshBuilder::GenerateCube("display", Color(0, 0, 1), 5, 5, 5);
    meshList[GEO_SHOPDISPLAY]->textureID = LoadTGA("Image/layout/canteen_walls.tga");

    //Light
    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 10, 20);

    	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	meshList[GEO_HANGAR] = MeshBuilder::GenerateQuad("hangar", Color(1, 1, 1), 1, 1);
	meshList[GEO_HANGAR]->textureID = LoadTGA("Image/layout/control_room_floor.tga");

	meshList[GEO_CONNECTOR] = MeshBuilder::GenerateQuad("connector", Color(1, 1, 1), 1, 1);
	meshList[GEO_CONNECTOR]->textureID = LoadTGA("Image/layout/control_room_ceiling.tga");

	meshList[GEO_MAINWALLS] = MeshBuilder::GenerateQuad("mainwalls", Color(1, 1, 1), 1, 1);
	meshList[GEO_MAINWALLS]->textureID = LoadTGA("Image/layout/Main_walls.tga");

    meshList[GEO_INNERWALLS] = MeshBuilder::GenerateQuad("mainwalls", Color(1, 1, 1), 1, 1);
    meshList[GEO_INNERWALLS]->textureID = LoadTGA("Image/layout/Inner_walls.tga");

    meshList[GEO_LABWALL] = MeshBuilder::GenerateQuad("mainwalls", Color(1, 1, 1), 1, 1);
    meshList[GEO_LABWALL]->textureID = LoadTGA("Image/layout/lab_tile.tga");

    meshList[GEO_CONTROLWALLS] = MeshBuilder::GenerateQuad("mainwalls", Color(1, 1, 1), 1, 1);
    meshList[GEO_CONTROLWALLS]->textureID = LoadTGA("Image/layout/control_walls.tga");

    meshList[GEO_SHOP_WALLS] = MeshBuilder::GenerateQuad("mainwalls", Color(1, 1, 1), 1, 1);
    meshList[GEO_SHOP_WALLS]->textureID = LoadTGA("Image/layout/shop_walls.tga");

    meshList[GEO_CANTEENWALLS] = MeshBuilder::GenerateQuad("mainwalls", Color(1, 1, 1), 1, 1);
    meshList[GEO_CANTEENWALLS]->textureID = LoadTGA("Image/layout/canteen_walls.tga");

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	meshList[GEO_PIPETYPE1] = MeshBuilder::GenerateQuad("pipetype1", Color(1, 1, 1), 1, 1);
	meshList[GEO_PIPETYPE1]->textureID = LoadTGA("Image/pipetype1.tga");
				 
	meshList[GEO_PIPETYPE2] = MeshBuilder::GenerateQuad("pipetype2", Color(1, 1, 1), 1, 1);
	meshList[GEO_PIPETYPE2]->textureID = LoadTGA("Image/pipetype2.tga");
				 
	meshList[GEO_PIPETYPE3] = MeshBuilder::GenerateQuad("pipetype3", Color(1, 1, 1), 1, 1);
	meshList[GEO_PIPETYPE3]->textureID = LoadTGA("Image/pipetype3.tga");
				 
	meshList[GEO_PIPETYPE4] = MeshBuilder::GenerateQuad("pipetype4", Color(1, 1, 1), 1, 1);
	meshList[GEO_PIPETYPE4]->textureID = LoadTGA("Image/pipetype4.tga");

    meshList[GEO_CROSSHAIRS] = MeshBuilder::GenerateQuad("crosshairs", Color(1, 1, 1), 2, 2);
    meshList[GEO_CROSSHAIRS]->textureID = LoadTGA("Image/crosshairs.tga");

	meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("invbar", Color(1, 1, 1), 40, 5);
	meshList[GEO_INVENTORY]->textureID = LoadTGA("Image/inventorybar.tga");

	meshList[GEO_ITEMSELECT] = MeshBuilder::GenerateQuad("itemselect", Color(1, 1, 1), 4, 4);
	meshList[GEO_ITEMSELECT]->textureID = LoadTGA("Image/itemselect.tga");


    meshList[GEO_MAP] = MeshBuilder::GenerateMinimap("map", 10, 10);
    meshList[GEO_MAP]->textureID = LoadTGA("Image/Donna.tga");

    viewOptions = true;

    objx = objy = 0;
}

SP2::~SP2()
{
}

void SP2::Init()
{
    gamestate = GAME_STATE_FREE;

    //light 0
    light[0].type = Light::LIGHT_DIRECTIONAL;
    light[0].position.Set(0, 1000, 0);
    light[0].color.Set(1, 1, 1);
    light[0].power = 1;
    light[0].kC = 1.f;
    light[0].kL = 0.01f;
    light[0].kQ = 0.001f;

    light[0].cosCutoff = cos(Math::DegreeToRadian(45));
    light[0].cosInner = cos(Math::DegreeToRadian(30));
    light[0].exponent = 3.f;
    light[0].spotDirection.Set(0.f, 1.f, 0.f);

    //make sure uniform parameters are passed in after glUseProgram()
    glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
    glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
    glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
    glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
    glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
    glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
    glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
    glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

    glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	
	srand(time(0));
	controlpuzzle.setpuzzle();
	rotator = 0;
    ItemCollision walltest;
    walltest.minX = 5;     walltest.maxX = 235;     walltest.minZ = -120;  walltest.maxZ = -95;    SharedData::GetInstance()->collisionItems.push_back(walltest);

    Interaction* interactions;
    interactions = new PipePuzzleInteraction();
    interactions->middlePoint = Vector3(446, 15, 374.5f);   interactions->distX = 50;   interactions->distY = 15;   interactions->distZ = 5.f;     SharedData::GetInstance()->interactionItems.push_back(interactions);

    playerHung = SharedData::GetInstance()->player->getHunger();
    rotating = 0;

}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;
static float LSPEED = 10.f;

static double FramePerSecond;

void SP2::Update(double dt)
{
    FramePerSecond = 1 / dt;

    SharedData::GetInstance()->player->Walk(dt);

    SharedData::GetInstance()->camera->Update(dt);

    SharedData::GetInstance()->player->CheckInteraction();

	if (delayer > 0)
		delayer -= 1;
	rotator++;
    //options
    if (Application::IsKeyPressed('1')) //enable back face culling
        glEnable(GL_CULL_FACE);
    if (Application::IsKeyPressed('2')) //disable back face culling
        glDisable(GL_CULL_FACE);
    if (Application::IsKeyPressed('3'))
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
    if (Application::IsKeyPressed('4'))
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
    if (Application::IsKeyPressed('5'))
        viewOptions = true;     //switch on info
    if (Application::IsKeyPressed('6'))
        viewOptions = false;    //switch off info
    if (Application::IsKeyPressed('R')) {       //reset
        Reset();
    }

    //Temmie vibration
    if (vibrateX && vibrateY < 3)
    {
        vibrateX += 1;
        vibrateY += 1;
    }
    else
    {
        vibrateX -= 1.5;
        vibrateY -= 1.5;
    }

    if (chonFloat == false)
    {
        chonFloaty += 0.1;
        if (chonFloaty >= 10.0f)
        {
            chonFloat = true;
        }
    }
    if (chonFloat == true)
    {
        chonFloaty -= 0.1;
        if (chonFloaty <= 0)
        {
            chonFloat = false;
        }
    }
    ////minimapUpdate()
    //GEO_MAP
    //meshList[GEO_MAP] = MeshBuilder::GenerateMinimap("map", 10, 10);
    //meshList[GEO_MAP]->textureID = LoadTGA("Image/Donna.tga");
    
    if (Application::IsKeyPressed('O'))
    {
        playerHung -= 1;
        if (playerHung <= 0)
            playerHung = 0;
    }
    if (Application::IsKeyPressed('P'))
    {
        playerHung += 1;
        if (playerHung >= 100)
            playerHung = 100;

    }

    rotating += 30*dt;

    if (Application::IsKeyPressed('I'))
        objy += 30 * dt;
    if (Application::IsKeyPressed('K'))
        objy -= 30 * dt;
    if (Application::IsKeyPressed('J'))
        objx -= 30 * dt;
    if (Application::IsKeyPressed('L'))
        objx += 30 * dt;
}

void SP2::Render()
{
    // Render VBO here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mtx44 MVP;

    Vector3 playerPos = (SharedData::GetInstance()->player->position_);

    minimappy.Init(Vector3(playerPos.x, (playerPos.y + 500), playerPos.z), Vector3(playerPos.x, (playerPos.y + 300), playerPos.z), Vector3(1, 0, 0));

    //Set view matrix using camera settings
    viewStack.LoadIdentity();
    viewStack.
        //LookAt(minimappy.pos.x, minimappy.pos.y, minimappy.pos.z, minimappy.targ.x, minimappy.targ.y, minimappy.targ.z, minimappy.UP.x, minimappy.UP.y, minimappy.UP.z);
        LookAt(SharedData::GetInstance()->camera->position.x, SharedData::GetInstance()->camera->position.y, SharedData::GetInstance()->camera->position.z,
        SharedData::GetInstance()->camera->target.x, SharedData::GetInstance()->camera->target.y, SharedData::GetInstance()->camera->target.z,
        SharedData::GetInstance()->camera->up.x, SharedData::GetInstance()->camera->up.y, SharedData::GetInstance()->camera->up.z);
    //minimappy.UP.x;
    //minimappp.LoadIdentity();
    //minimappp.LookAt(minimappy.pos.x, minimappy.pos.y, minimappy.pos.z, minimappy.targ.x, minimappy.targ.y, minimappy.targ.z, minimappy.UP.x, minimappy.UP.y, minimappy.UP.z);

    modelStack.LoadIdentity();

    modelStack.PushMatrix();
    modelStack.Translate(0, 1, 0);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_MAP], false);
    modelStack.PopMatrix();

    //pass the position of light to shader based on light type
    //spotlights
    if (light[0].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
    }

    //modelStack.PushMatrix();
    //modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
    //RenderMesh(meshList[GEO_LIGHTBALL], false);
    //modelStack.PopMatrix();

    switch (gamestate)
    {
    case GAME_STATE_FREE: loadFree();
        break;
    case GAME_STATE_SHOP: loadShop();
        break;
    case GAME_STATE_WSGAME: loadWSGame();
        break;
    case GAME_STATE_CHONGAME:  loadChonGame();
        break;
    case GAME_STATE_VEEGAME: loadVeeGame();
        break;
    case GAME_STATE_RABBIT: loadRabbitGame();
        break;
    }

    //RenderMinimap();
}
void SP2::loadFree()
{  
    RenderSkybox();
    RenderGround();

	//RenderLayout();

    modelStack.PushMatrix();
    modelStack.Translate(420, 1, -23);
    modelStack.Scale(50, 50, 50);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_LAYOUT], true);
    modelStack.PopMatrix();

    RenderObjects();
    RenderPlayer();
    RenderNPC();
    RenderPuzzle();
    RenderUI();

	RenderInventory();

    RenderObjectOnScreen(meshList[GEO_CROSSHAIRS], 40, 30);
	RenderObjectOnScreen(meshList[GEO_INVENTORY], 40, 2.5);

    RenderObjectOnScreen(meshList[GEO_CROSSHAIRS], 40, 30,1,1);
    
    shoptemp();
    
    RenderUI();

    //RenderMinimap();
    
    //interaction
    if (SharedData::GetInstance()->canInteract) {
        RenderTextOnScreen(meshList[GEO_TEXT], "Press LMB", Color(1, 0, 0), 3, 2, 2);
    }
    
    if (Application::IsKeyPressed('H'))
    {
        gamestate = GAME_STATE_SHOP;
    }
}  
void SP2::loadShop()
{  
    if (!meshList[GEO_DIALOGUEBOX] || meshList[GEO_DIALOGUEBOX]->textureID <= 0)  //error check
        return;

    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10);    //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();   //no need camera for ortho mode

    modelStack.PushMatrix();
    modelStack.LoadIdentity();  //reset modelStack

    modelStack.PushMatrix();

    modelStack.Translate(25, 25, 0);
    modelStack.Rotate(90, 0, 1, 0);
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_STEMMIE], false);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);

    RenderPlayer();

    RenderDialogueOnScreen("temshop", Color(1, 0, 1), 15);
    RenderTextOnScreen(meshList[GEO_TEXT], "hOi, welcom, to da tem shop", Color(1, 1, 1), 2, 5, 5);

    if (Application::IsKeyPressed('L'))
    {
        gamestate = GAME_STATE_FREE;
    }
}  
void SP2::loadWSGame()
{  
   
}  
void SP2::loadChonGame()
{    
     
}    
void SP2::loadVeeGame()
{    
     
}    
void SP2::loadRabbitGame()
{

}

void SP2::Exit()
{
    // Cleanup VBO here
    glDeleteVertexArrays(1, &m_vertexArrayID);
    glDeleteProgram(m_programID);
}

void SP2::RenderMesh(Mesh* mesh, bool enableLight)
{
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    modelView = viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
    if (enableLight)
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 1);
        modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
        glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

        //load material
        glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
        glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
        glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
        glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
    }
    else
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    }

    if (mesh->textureID > 0)
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    }
    else
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
    }

    mesh->Render();

    if (mesh->textureID > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void SP2::RenderText(Mesh* mesh, std::string text, Color color)
{
    if (!mesh || mesh->textureID <= 0)  //error check
        return;

    glDisable(GL_DEPTH_TEST);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

    for (unsigned i = 0; i < text.length(); ++i) {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(i * 1.0f, 0, 0);  //1.f is the spacing of each character (can be changed)
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
        mesh->Render((unsigned)text[i] * 6, 6);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderObjectOnScreen(Mesh* mesh, float x, float y,float scalex, float scaley)
{
    if (!mesh)  //error check
        return;

    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10);    //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();   //no need camera for ortho mode
    modelStack.PushMatrix();
    modelStack.LoadIdentity();  //reset modelStack

    
    modelStack.PushMatrix();
    modelStack.Translate(x, y, 0);
    modelStack.Scale(scalex, scaley, 1);
    RenderMesh(mesh, false);
    modelStack.PopMatrix();

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
    if (!mesh || mesh->textureID <= 0)  //error check
        return;

    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10);    //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();   //no need camera for ortho mode

    modelStack.PushMatrix();
    modelStack.LoadIdentity();  //reset modelStack
    modelStack.Scale(size, size, size);
    modelStack.Translate(x, y, 0);

    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

    for (unsigned i = 0; i < text.length(); ++i) {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0);  //1.f is the spacing of each character (can be changed)
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
        mesh->Render((unsigned)text[i] * 6, 6);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();

    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderDialogueOnScreen(std::string text, Color color, float size)
{
    if (!meshList[GEO_DIALOGUEBOX] || meshList[GEO_DIALOGUEBOX]->textureID <= 0)  //error check
        return;

    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10);    //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();   //no need camera for ortho mode

    modelStack.PushMatrix();
    modelStack.LoadIdentity();  //reset modelStack

    modelStack.PushMatrix();
    modelStack.Translate(40, 10, 0);
    RenderMesh(meshList[GEO_DIALOGUEBOX], false);
    modelStack.PopMatrix();

    RenderTextOnScreen(meshList[GEO_TEXT], text, color, size, 1.5f, 4.5f);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderSkybox()
{
    modelStack.PushMatrix();

    //follow player

    modelStack.Translate(SharedData::GetInstance()->player->position_.x, -100, SharedData::GetInstance()->player->position_.z);

    //left
    modelStack.PushMatrix();
    modelStack.Translate(-495, 495, 0);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_SKYBOX_LEFT], false);
    modelStack.PopMatrix();

    //right
    modelStack.PushMatrix();
    modelStack.Translate(495, 495, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_SKYBOX_RIGHT], false);
    modelStack.PopMatrix();

    //back
    modelStack.PushMatrix();
    modelStack.Translate(0, 495, 495);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_SKYBOX_BACK], false);
    modelStack.PopMatrix();

    //front
    modelStack.PushMatrix();
    modelStack.Translate(0, 495, -495);
    RenderMesh(meshList[GEO_SKYBOX_FRONT], false);
    modelStack.PopMatrix();

    //top
    modelStack.PushMatrix();
    modelStack.Translate(0, 990, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_SKYBOX_TOP], false);
    modelStack.PopMatrix();

    //bottom
    modelStack.PushMatrix();
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_SKYBOX_BOTTOM], false);
    modelStack.PopMatrix();

    modelStack.PopMatrix();
}

void SP2::RenderGround()
{
    modelStack.PushMatrix();
	modelStack.Translate(100, 0, 0);
    modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_GROUND], false);
    modelStack.PopMatrix();
}

void SP2::RenderLayout()
{
	//hangar start
	for (int j = 0; j < 3; j++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(4, 15 + (j * 30), 60);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(4, 15 + (j * 30), -60);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(4, 15 + (j * 30), 90);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(4, 15 + (j * 30), -90);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(2, 15 + (j * 30), -45);
		modelStack.Scale(4, 30, 1);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(2, 15 + (j * 30), 45);
		modelStack.Scale(4, 30, 1);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 15 + (j * 30), 60);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 15 + (j * 30), -60);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 15 + (j * 30), 90);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 15 + (j * 30), -90);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		for (int i = 0; i < 8; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(15 + (i * 30), 15 + (j * 30), 105);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_HANGAR], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(15 + (i * 30), 15 + (j * 30), 101);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(180, 0, 1, 0);
			RenderMesh(meshList[GEO_HANGAR], true);
			modelStack.PopMatrix();

			if ((i == 6) && (j == 0))
			{
				continue;
			}

			modelStack.PushMatrix();
			modelStack.Translate(15 + (i * 30), 15 + (j * 30), -101);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_HANGAR], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(15 + (i * 30), 15 + (j * 30), -105);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(180, 0, 1, 0);
			RenderMesh(meshList[GEO_HANGAR], true);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(240, 15 + (j * 30), 30);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(240, 15 + (j * 30), -30);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(240, 15 + (j * 30), 60);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(240, 15 + (j * 30), -60);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(240, 15 + (j * 30), 90);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(240, 15 + (j * 30), -90);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(238, 15 + (j * 30), -15);
		modelStack.Scale(4, 30, 1);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(238, 15 + (j * 30), 15);
		modelStack.Scale(4, 30, 1);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(236, 15 + (j * 30), 30);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(236, 15 + (j * 30), -30);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(236, 15 + (j * 30), 60);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(236, 15 + (j * 30), -60);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(236, 15 + (j * 30), 90);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(236, 15 + (j * 30), -90);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_HANGAR], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(236, 45, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(236, 45, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(236, 75, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(236, 75, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(240, 45, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(240, 45, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(240, 75, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(240, 75, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(180, 15, -103);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, 15, -103);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(238, 30, 0);
	modelStack.Scale(4, 30, 30);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(195, 30, -103);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_HANGAR], true);
	modelStack.PopMatrix();
	// hangar end

	//hanger to main connector start
	modelStack.PushMatrix();
	modelStack.Translate(255, 15, -15);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(255, 15, 15);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(255, 15, -19);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(255, 15, 19);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(255, 30, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(255, 30, 0);
	modelStack.Scale(30, 30, 38);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();
	//hanger to main connector end

	//main start
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 7; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(266, 15 + (j * 30), 30 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_MAINWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(266, 15 + (j * 30), -30 - (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_MAINWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(270, 15 + (j * 30), 30 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_MAINWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(270, 15 + (j * 30), -30 - (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_MAINWALLS], true);
			modelStack.PopMatrix();
		}

		for (int i = 0; i < 10; i++)
		{
			if ((i != 2) && (i != 9))
			{
				modelStack.PushMatrix();
				modelStack.Translate(281 + (i * 30), 15 + (j * 30), -221);
				modelStack.Scale(30, 30, 30);
				RenderMesh(meshList[GEO_MAINWALLS], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(281 + (i * 30), 15 + (j * 30), -225);
				modelStack.Scale(30, 30, 30);
				modelStack.Rotate(180, 0, 1, 0);
				RenderMesh(meshList[GEO_MAINWALLS], true);
				modelStack.PopMatrix();
			}

			if ((i != 1) && (i != 5) && (i != 6))
			{
				modelStack.PushMatrix();
				modelStack.Translate(281 + (i * 30), 15 + (j * 30), 221);
				modelStack.Scale(30, 30, 30);
				modelStack.Rotate(180, 0, 1, 0);
				RenderMesh(meshList[GEO_MAINWALLS], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(281 + (i * 30), 15 + (j * 30), 225);
				modelStack.Scale(30, 30, 30);
				RenderMesh(meshList[GEO_MAINWALLS], true);
				modelStack.PopMatrix();
			}
		}

		for (int i = 0; i < 15; i++)
		{
			if ((i == 6) || (i == 7) || (i == 8))
			{
				continue;
			}
			modelStack.PushMatrix();
			modelStack.Translate(562, 15 + (j * 30), -210 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_MAINWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(566, 15 + (j * 30), -210 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_MAINWALLS], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PushMatrix();
	modelStack.Translate(266, 45, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(270, 45, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(311, 45, 221);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(311, 45, 225);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(326, 15, 223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(296, 15, 223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(311, 30, 223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(341, 45, -225);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(341, 45, -221);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(326, 15, -223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(356, 15, -223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(341, 30, -223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(431, 45, 221);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(431, 45, 225);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(416, 15, 223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(476, 15, 223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(446, 30, 223);
	modelStack.Scale(60, 30, 4);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(461, 45, 221);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(461, 45, 225);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(551, 45, -221);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(551, 45, -225);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MAINWALLS], true);
	modelStack.PopMatrix();
	//main end

	//main barracks start
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 11; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(315, 15 + (j * 30), -150 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(319, 15 + (j * 30), -150 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			if ((i != 1) && (i != 9))
			{
				modelStack.PushMatrix();
				modelStack.Translate(371, 15 + (j * 30), -150 + (i * 30));
				modelStack.Scale(30, 30, 30);
				modelStack.Rotate(-90, 0, 1, 0);
                RenderMesh(meshList[GEO_INNERWALLS], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(375, 15 + (j * 30), -150 + (i * 30));
				modelStack.Scale(30, 30, 30);
				modelStack.Rotate(90, 0, 1, 0);
                RenderMesh(meshList[GEO_INNERWALLS], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(435, 15 + (j * 30), -150 + (i * 30));
				modelStack.Scale(30, 30, 30);
				modelStack.Rotate(-90, 0, 1, 0);
                RenderMesh(meshList[GEO_INNERWALLS], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(439, 15 + (j * 30), -150 + (i * 30));
				modelStack.Scale(30, 30, 30);
				modelStack.Rotate(90, 0, 1, 0);
                RenderMesh(meshList[GEO_INNERWALLS], true);
				modelStack.PopMatrix();
			}
			else
			{
				modelStack.PushMatrix();
				modelStack.Translate(373, 15 + (j * 30), -135 + (i * 30));
				modelStack.Scale(4, 30, 30);
				modelStack.Rotate(180, 0, 1, 0);
                RenderMesh(meshList[GEO_INNERWALLS], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(373, 15 + (j * 30), -165 + (i * 30));
				modelStack.Scale(4, 30, 30);
                RenderMesh(meshList[GEO_INNERWALLS], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(437, 15 + (j * 30), -135 + (i * 30));
				modelStack.Scale(4, 30, 30);
				modelStack.Rotate(180, 0, 1, 0);
                RenderMesh(meshList[GEO_INNERWALLS], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(437, 15 + (j * 30), -165 + (i * 30));
				modelStack.Scale(4, 30, 30);
                RenderMesh(meshList[GEO_INNERWALLS], true);
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix();
			modelStack.Translate(491, 15 + (j * 30), -150 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(495, 15 + (j * 30), -150 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();
		}
		for (int i = 0; i < 6; i++)
		{
			if ((i == 2) || (i == 3))
			{
				continue;
			}
			modelStack.PushMatrix();
			modelStack.Translate(330 + (i * 30), 15 + (j * 30), -165);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(180, 0, 1, 0);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(330 + (i * 30), 15 + (j * 30), -161);
			modelStack.Scale(30, 30, 30);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(330 + (i * 30), 15 + (j * 30), 161);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(180, 0, 1, 0);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(330 + (i * 30), 15 + (j * 30), 165);
			modelStack.Scale(30, 30, 30);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(330 + (i * 30), 15 + (j * 30), -15);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(180, 0, 1, 0);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(330 + (i * 30), 15 + (j * 30), -11);
			modelStack.Scale(30, 30, 30);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(330 + (i * 30), 15 + (j * 30), 11);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(180, 0, 1, 0);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(330 + (i * 30), 15 + (j * 30), 15);
			modelStack.Scale(30, 30, 30);
            RenderMesh(meshList[GEO_INNERWALLS], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PushMatrix();
	modelStack.Translate(439, 45, -120);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(435, 45, -120);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(437, 30, -120);
	modelStack.Scale(4, 30, 30);
	modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(375, 45, -120);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(371, 45, -120);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(373, 30, -120);
	modelStack.Scale(4, 30, 30);
	modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(439, 45, 120);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(435, 45, 120);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(437, 30, 120);
	modelStack.Scale(4, 30, 30);
	modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(375, 45, 120);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(371, 45, 120);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(373, 30, 120);
	modelStack.Scale(4, 30, 30);
	modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_INNERWALLS], true);
	modelStack.PopMatrix();
	//main barracks end

	//lab entranceway start
	modelStack.PushMatrix();
	modelStack.Translate(326, 15, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(322, 15, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(360, 15, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(356, 15, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(341, 30, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(341, 30, -240);
	modelStack.Scale(38, 30, 30);
	modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();
	//lab entranceway end
   
	//lab start
	for (int j = 0; j < 2; j++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(371, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(371, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(311, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(311, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(281, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(281, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(251, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(251, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(221, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(221, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(191, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(191, 15 + (j * 30), -255);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(371, 15 + (j * 30), -371);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(371, 15 + (j * 30), -375);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(341, 15 + (j * 30), -371);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(341, 15 + (j * 30), -375);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(311, 15 + (j * 30), -371);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(311, 15 + (j * 30), -375);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(281, 15 + (j * 30), -371);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(281, 15 + (j * 30), -375);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(251, 15 + (j * 30), -371);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(251, 15 + (j * 30), -375);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(191, 15 + (j * 30), -371);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(191, 15 + (j * 30), -375);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_LABWALL], true);
		modelStack.PopMatrix();

		for (int i = 0; i < 2; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(386 + ((i - 1) * 210), 15 + (j * 30), -270);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_LABWALL], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(386 + ((i - 1) * 210), 15 + (j * 30), -300);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_LABWALL], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(386 + ((i - 1) * 210), 15 + (j * 30), -330);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_LABWALL], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(386 + ((i - 1) * 210), 15 + (j * 30), -360);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_LABWALL], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(386 + ((i - 1) * 210), 15 + (j * 30), -270);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_LABWALL], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(386 + ((i - 1) * 210), 15 + (j * 30), -300);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_LABWALL], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(386 + ((i - 1) * 210), 15 + (j * 30), -330);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_LABWALL], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(386 + ((i - 1) * 210), 15 + (j * 30), -360);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_LABWALL], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PushMatrix();
	modelStack.Translate(221, 45, -371);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_LABWALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(221, 45, -375);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_LABWALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(221, 30, -373);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_LABWALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(236, 15, -373);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_LABWALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(206, 15, -373);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_LABWALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(341, 45, -255);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_LABWALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(341, 45, -255);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_LABWALL], true);
	modelStack.PopMatrix();
	//lab end

	//connector main to control start
	modelStack.PushMatrix();
	modelStack.Translate(416, 15, 240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(476, 15, 240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(416, 15, 240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(476, 15, 240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(446, 30, 240);
	modelStack.Scale(60, 30, 30);
	modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(446, 30, 240);
	modelStack.Scale(60, 30, 30);
	modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();
	//connector main to control end

	//control room start
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(551 - (i * 30), 15 + (j * 30), 375);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_CONTROLWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(551 - (i * 30), 15 + (j * 30), 375);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(180, 0, 1, 0);
			RenderMesh(meshList[GEO_CONTROLWALLS], true);
			modelStack.PopMatrix();

			if ((i == 3) || (i == 4))
			{
				continue;
			}
			modelStack.PushMatrix();
			modelStack.Translate(551 - (i * 30), 15 + (j * 30), 255);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_CONTROLWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(551 - (i * 30), 15 + (j * 30), 255);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(180, 0, 1, 0);
			RenderMesh(meshList[GEO_CONTROLWALLS], true);
			modelStack.PopMatrix();
		}
		for (int i = 0; i < 4; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(566, 15 + (j * 30), 270 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_CONTROLWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(566, 15 + (j * 30), 270 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_CONTROLWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(326, 15 + (j * 30), 270 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_CONTROLWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(326, 15 + (j * 30), 270 + (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_CONTROLWALLS], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PushMatrix();
	modelStack.Translate(461, 45, 255);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_CONTROLWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(461, 45, 255);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_CONTROLWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(461, 75, 255);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_CONTROLWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(461, 75, 255);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_CONTROLWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(431, 45, 255);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_CONTROLWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(431, 45, 255);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_CONTROLWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(431, 75, 255);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_CONTROLWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(431, 75, 255);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_CONTROLWALLS], true);
	modelStack.PopMatrix();
	//control room end

	//tEMSHOP start
	for (int i = 0; i < 2; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(577, 15 + (i * 30), -45);
		modelStack.Scale(30, 30, 30);
        RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(577, 15 + (i * 30), -45);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(607, 15 + (i * 30), -45);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(607, 15 + (i * 30), -45);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(577, 15 + (i * 30), 45);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(607, 15 + (i * 30), 45);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(622, 15 + (i * 30), 30);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(622, 15 + (i * 30), 0);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(622, 15 + (i * 30), -30);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(622, 15 + (i * 30), 30);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(622, 15 + (i * 30), 0);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(622, 15 + (i * 30), -30);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_SHOP_WALLS], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(612, 50, -30);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(45, 0, 0, 1);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_SHOP_WALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(612, 50, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(45, 0, 0, 1);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_SHOP_WALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(612, 50, 30);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(45, 0, 0, 1);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_SHOP_WALLS], true);
	modelStack.PopMatrix();
	//tEMSHOP end

	//path to canteen start
	modelStack.PushMatrix();
	modelStack.Translate(536, 15, -223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(551, 30, -223);
	modelStack.Scale(30, 30, 4);
	modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(536, 15, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(536, 15, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(566, 15, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(562, 15, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(547, 30, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(547, 30, -240);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_CONNECTOR], true);
	modelStack.PopMatrix();
	//path to canteen end

	//canteen start
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 6; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(562, 15 + (j * 30), -270 - (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_CANTEENWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(566, 15 + (j * 30), -270 - (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_CANTEENWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(412, 15 + (j * 30), -270 - (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[GEO_CANTEENWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(416, 15 + (j * 30), -270 - (i * 30));
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_CANTEENWALLS], true);
			modelStack.PopMatrix();
		}
		for (int i = 0; i < 5; i++)
		{
			if (i != 4)
			{
				modelStack.PushMatrix();
				modelStack.Translate(431 + (i * 30), 15 + (j * 30), -255);
				modelStack.Scale(30, 30, 30);
				modelStack.Rotate(180, 0, 1, 0);
				RenderMesh(meshList[GEO_CANTEENWALLS], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(431 + (i * 30), 15 + (j * 30), -255);
				modelStack.Scale(30, 30, 30);
				RenderMesh(meshList[GEO_CANTEENWALLS], true);
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix();
			modelStack.Translate(427 + (i * 30), 15 + (j * 30), -435);
			modelStack.Scale(30, 30, 30);
			modelStack.Rotate(180, 0, 1, 0);
			RenderMesh(meshList[GEO_CANTEENWALLS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(429 + (i * 30), 15 + (j * 30), -435);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_CANTEENWALLS], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PushMatrix();
	modelStack.Translate(551, 45, -255);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_CANTEENWALLS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(551, 45, -255);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_CANTEENWALLS], true);
	modelStack.PopMatrix();
	//canteen end
}

void SP2::RenderPlayer()
{
    modelStack.PushMatrix();

    modelStack.Translate(0, 12, 0);
    modelStack.Translate(SharedData::GetInstance()->player->position_.x, SharedData::GetInstance()->player->position_.y, SharedData::GetInstance()->player->position_.z);

    modelStack.Rotate(SharedData::GetInstance()->player->direction_, 0, 1, 0);
    modelStack.Scale(3.f, 3.f, 3.f);
    RenderMesh(meshList[GEO_MAN], true);
    modelStack.PopMatrix();
}

void SP2::RenderNPC()
{
    modelStack.PushMatrix();
    modelStack.Translate(-139, 0, 68);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_ADOLPH], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(580, 0, 0 + 20);
    modelStack.Scale(5, 5, 5);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_STEMMIE], true);
    modelStack.PopMatrix();


    modelStack.PushMatrix();
    modelStack.Translate(576, 20 + vibrateY, 6 + 20 + vibrateX);
    modelStack.Scale(10, 10, 10);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_STEMMIE_FACE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(620, 0, -420);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_WENGSTANG], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(440, 0, 300);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_VEE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(440, 0, -344);
    modelStack.Scale(10, 10, 10);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_JASIM], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(200, 5 + chonFloaty, -295);
    modelStack.Scale(10, 10, 10);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_CHON], true);
    modelStack.PopMatrix();

    //modelStack.PushMatrix();
    //RenderMesh(meshList[GEO_AARON], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    //RenderMesh(meshList[GEO_MERCHANT], true);
    //modelStack.PopMatrix();
}

void SP2::RenderObjects()
{
    //modelStack.PushMatrix();
    //modelStack.Scale(10, 10, 10);
    //RenderMesh(meshList[GEO_BENCH], true);
    //modelStack.PopMatrix();

    //Canteen
    for (int i = 0; i < 130; i+=60)
    {
        modelStack.PushMatrix();
        modelStack.Translate(500, 0, -420 + i);
        modelStack.Scale(7, 7, 7);
        modelStack.Rotate(-90, 0, 1, 0);
        RenderMesh(meshList[GEO_BENCH], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(500, 0, -405 + i);
        modelStack.Scale(7, 7, 7);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_TABLE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(500, 0, -390 + i);
        modelStack.Scale(7, 7, 7);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_BENCH], true);
        modelStack.PopMatrix();
    }

    modelStack.PushMatrix();
    modelStack.Translate(435, 0, -275);
    modelStack.Scale(9, 9, 9);
    RenderMesh(meshList[GEO_VENDINGMACHINE], true);
    modelStack.PopMatrix();

    //Chon's Lab
    modelStack.PushMatrix();
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_TOOLBOX], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_HAMMER], true);
    modelStack.PopMatrix();
    //modelStack.PushMatrix();
    //modelStack.Scale(5, 5, 5);
    //RenderMesh(meshList[GEO_VENDINGMACHINE], true);
    //modelStack.PopMatrix();

    //Stemmie Shop
    modelStack.PushMatrix();
    modelStack.Translate(588, 0, -20);
    modelStack.Scale(10, 10, 10);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_VENDINGMACHINE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(580, 0, 25);
    modelStack.Scale(4, 4, 4);
    RenderMesh(meshList[GEO_TEMSHOP], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(10, 0, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_BOXCRATE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();

    modelStack.Translate(20, 0, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_CHAIR], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();

    modelStack.Translate(30, 0, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_CONTROLROOM], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();

    modelStack.Translate(40, 0, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_FIREEXTINGUISHER], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(50, 0, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_FRIDGE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(60, 0, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(70, 0, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_SHELF], true);
    modelStack.PopMatrix();   

    modelStack.PushMatrix();
    modelStack.Translate(90, 0, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_UMBRELLASTAND], true);
    modelStack.PopMatrix();
}

void SP2::RenderPuzzle()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(401 + (i * 10), 65 - (j * 10), 374);
            modelStack.Rotate(controlpuzzle.puzzlerotation[i][j], 0, 0, 1);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(10, 10, 10);
			//RenderMesh(meshList[GEO_PIPETYPE1], true);
			
			if (controlpuzzle.puzzlemap[i][j] == 0)
			{
				RenderMesh(meshList[GEO_PIPETYPE1], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 1)
			{
				modelStack.Rotate(90, 0, 0, 1);
				RenderMesh(meshList[GEO_PIPETYPE1], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 2)
			{
				RenderMesh(meshList[GEO_PIPETYPE2], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 3)
			{
				modelStack.Rotate(90, 0, 0, 1);
				RenderMesh(meshList[GEO_PIPETYPE2], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 4)
			{
				modelStack.Rotate(180, 0, 0, 1);
				RenderMesh(meshList[GEO_PIPETYPE2], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 5)
			{
				modelStack.Rotate(-90, 0, 0, 1);
				RenderMesh(meshList[GEO_PIPETYPE2], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 6)
			{
				RenderMesh(meshList[GEO_PIPETYPE3], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 7)
			{
				modelStack.Rotate(90, 0, 0, 1);
				RenderMesh(meshList[GEO_PIPETYPE3], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 8)
			{
				modelStack.Rotate(180, 0, 0, 1);
				RenderMesh(meshList[GEO_PIPETYPE3], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 9)
			{
				modelStack.Rotate(-90, 0, 0, 1);
				RenderMesh(meshList[GEO_PIPETYPE3], true);
			}
			else if (controlpuzzle.puzzlemap[i][j] == 10)
			{
				RenderMesh(meshList[GEO_PIPETYPE4], true);
			}
			modelStack.PopMatrix();
		}
	}
}

void SP2::RenderInventoryOnScreen(Mesh* mesh, float x, float y)
{
	if (!mesh)  //error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -1000, 1000);    //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();   //no need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();  //reset modelStack

	modelStack.PushMatrix();
	modelStack.Translate(x, y, 0);
	modelStack.Rotate(rotator, 0, 1, 0);
	modelStack.Rotate(-75, 1, 0, 1);
	modelStack.Scale(10, 10, 10);
	RenderMesh(mesh, false);
	modelStack.PopMatrix();

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SP2::RenderInventory()
{
	//SharedData::GetInstance()->player->inventory[0] = 1;
	//SharedData::GetInstance()->player->inventory[1] = 1;
	
	for (int i = 0; i < 8; i++)
	{
		if (SharedData::GetInstance()->player->inventory[i] == 0)
		{
			break;
		}
		if (SharedData::GetInstance()->player->inventory[i] == 1)
		{
			//gift1
			RenderInventoryOnScreen(meshList[GEO_HAMMER], 22.5 + (i * 5), 2.5);
			continue;
		}
		if (SharedData::GetInstance()->player->inventory[i] == 2)
		{
			//gift1
			RenderInventoryOnScreen(meshList[GEO_STEMMIE], 22.5 + (i * 5), 2.5);
			continue;
		}
	}
	if (delayer < 1)
	{
		if (Application::IsKeyPressed('V'))
			SharedData::GetInstance()->player->addItem(1);
		if (Application::IsKeyPressed('B'))
			SharedData::GetInstance()->player->addItem(2);
		if ((Application::IsKeyPressed('Z')) && (SharedData::GetInstance()->player->invselect > 0))
			SharedData::GetInstance()->player->invselect -= 1;
		if (Application::IsKeyPressed('X'))
			SharedData::GetInstance()->player->removeItem(SharedData::GetInstance()->player->invselect);
		if ((Application::IsKeyPressed('C')) && (SharedData::GetInstance()->player->invselect < 7))
			SharedData::GetInstance()->player->invselect += 1;
		delayer = 3;
	}

	RenderObjectOnScreen(meshList[GEO_ITEMSELECT], 22.5 + (SharedData::GetInstance()->player->invselect * 5), 2.5);
}

void SP2::RenderUI()
{
    
    
    if (viewOptions) {
        std::stringstream s;
        s << "FPS:" << FramePerSecond;
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 3, 0, 19);
        s.str("");
        s << "COORD:(" << (int)(SharedData::GetInstance()->player->position_.x) << "," << (int)(SharedData::GetInstance()->player->position_.y) << "," << (int)(SharedData::GetInstance()->player->position_.z) << ")";
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 3, 0, 18);

        s.str("");
        s << "objpos:(" << objx << " , " << objy << ")";
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 3, 0, 17);

        RenderObjectOnScreen(meshList[GEO_HUNGER_BAR], 10, 10, playerHung / 2, 1);
    }
}


void SP2::RenderMinimap()
{
    minimappp.LoadIdentity();
    minimappp.LookAt(minimappy.pos.x, minimappy.pos.y, minimappy.pos.z, minimappy.targ.x, minimappy.targ.y, minimappy.targ.z, minimappy.UP.x, minimappy.UP.y, minimappy.UP.z);

    if (!meshList[GEO_DIALOGUEBOX] || meshList[GEO_DIALOGUEBOX]->textureID <= 0)  //error check
        return;

    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10);    //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();   //no need camera for ortho mode
    minimappp.PushMatrix();     //NEW CODE
    minimappp.LoadIdentity();   //NEW CODE

    modelStack.PushMatrix();
    modelStack.LoadIdentity();  //reset modelStack

    modelStack.PushMatrix();

    //modelStack.PushMatrix();
    //modelStack.Translate(40, 10, 0);
    //modelStack.Scale(10, 10, 10);
    //RenderMesh(meshList[GEO_STEMMIE], false);
    //modelStack.PopMatrix();

    ////STUFF TO ADD
    //modelStack.Translate(75, 5, 0);
    //modelStack.Scale(3, 3, 3);
    //RenderMesh(meshList[GEO_MAP], false);

    //for (int i = 0; i < 500; i++)
    //{

    //    glPushMatrix(); //Start Phase
    //    //modelStack.PushMatrix();
    //    glEnable(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, GEO_GROUND);
    //    glBegin(GL_QUADS);
    //    glTexCoord2d(0, 0); glVertex2f(0, 0);
    //    glTexCoord2d(1, 0); glVertex2f(0 + 1000, 0);
    //    glTexCoord2d(1, 1); glVertex2f(0 + 1000, 0 + 1000);
    //    glTexCoord2d(0, 1); glVertex2f(0, 0 + 1000);
    
    modelStack.Translate(15, 15, 0);
    RenderMesh(meshList[GEO_MAP], false);
    //    glEnd();
    //    glPopMatrix();  //End Phase
    //}
    //modelStack.PopMatrix();
    
    //glDisable(GL_TEXTURE_2D);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    minimappp.PopMatrix();      //NEW CODE
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
    //std::vector<Vertex> vertex_buffer_data;
    //std::vector<GLuint> index_buffer_data;

    //Vertex v;
    //v.color = Color(0, 1, 0);

    //float width = 1.f / 200;
    //float height = 1.f / 200;
    //int offset = 0;

    //for (unsigned i = 0; i < 200; ++i) 
    //{
    //    for (unsigned j = 0; j < 200; ++j) 
    //    {
    //        //add 4 vertices into vertex_buffer_data
    //        v.pos.Set(-0.5f, -0.5f, 0);     v.normal.Set(0, 0, 1);  v.texCoord.Set(width * j, 1.f - height * (i + 1));     vertex_buffer_data.push_back(v);
    //        v.pos.Set(0.5f, -0.5f, 0);      v.normal.Set(0, 0, 1);  v.texCoord.Set(width * (j + 1), 1.f - height * (i + 1));   vertex_buffer_data.push_back(v);
    //        v.pos.Set(0.5f, 0.5f, 0);       v.normal.Set(0, 0, 1);  v.texCoord.Set(width * (j + 1), 1.f - height * i);   vertex_buffer_data.push_back(v);
    //        v.pos.Set(-0.5f, 0.5f, 0);      v.normal.Set(0, 0, 1);  v.texCoord.Set(width * j, 1.f - height * i);   vertex_buffer_data.push_back(v);

    //        //add 6 indices into index_buffer_data
    //        index_buffer_data.push_back(offset + 0);
    //        index_buffer_data.push_back(offset + 1);
    //        index_buffer_data.push_back(offset + 2);

    //        index_buffer_data.push_back(offset + 0);
    //        index_buffer_data.push_back(offset + 2);
    //        index_buffer_data.push_back(offset + 3);

    //        offset += 4;
    //    }
    //}

    //Mesh *mesh = new Mesh("minimap");

    //glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    //glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    //glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    //mesh->indexSize = index_buffer_data.size();
    //mesh->mode = Mesh::DRAW_TRIANGLES;

}


//void SP2::RenderMinimap()
//{
//    minimappp.LoadIdentity();
//    minimappp.LookAt(minimappy.pos.x, minimappy.pos.y, minimappy.pos.z, minimappy.targ.x, minimappy.targ.y, minimappy.targ.z, minimappy.UP.x, minimappy.UP.y, minimappy.UP.z);
//
//    if (!meshList[GEO_DIALOGUEBOX] || meshList[GEO_DIALOGUEBOX]->textureID <= 0)  //error check
//        return;
//
//    glDisable(GL_DEPTH_TEST);
//    Mtx44 ortho;
//    ortho.SetToOrtho(0, 80, 0, 60, -10, 10);    //size of screen UI
//    projectionStack.PushMatrix();
//    projectionStack.LoadMatrix(ortho);
//    viewStack.PushMatrix();
//    viewStack.LoadIdentity();   //no need camera for ortho mode
//    minimappp.PushMatrix();     //NEW CODE
//    minimappp.LoadIdentity();   //NEW CODE
//
//    modelStack.PushMatrix();
//    modelStack.LoadIdentity();  //reset modelStack
//
//    modelStack.PushMatrix();
//
//    //modelStack.PushMatrix();
//    //modelStack.Translate(40, 10, 0);
//    //modelStack.Scale(10, 10, 10);
//    //RenderMesh(meshList[GEO_STEMMIE], false);
//    //modelStack.PopMatrix();
//
//    ////STUFF TO ADD
//    modelStack.Translate(75, 5, 0);
//    modelStack.Scale(3, 3, 3);
//    RenderMesh(meshList[GEO_MAP], false);
//
//    projectionStack.PopMatrix();
//    viewStack.PopMatrix();
//    minimappp.PopMatrix();      //NEW CODE
//    modelStack.PopMatrix();
//    glEnable(GL_DEPTH_TEST);
//
//}

void SP2::shoptemp()
{
    if (Application::IsKeyPressed('H'))
    {
        RenderObjectOnScreen(meshList[GEO_SHOPUI], 40, 30, 50, 50);

        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 45, 0.2, 0.2);
        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 40, 0.2, 0.2);
        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 35, 0.2, 0.2);
        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 30, 0.2, 0.2);
        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 25, 0.2, 0.2);
        
        RenderDialogueOnScreen("Buy this item?", Color(1, 1, 1), 3);

        RotateDisplay();
    }
}

void SP2::RotateDisplay()
{
    modelStack.PushMatrix();
    modelStack.Rotate(20,1,0,0);

    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10);    //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();   //no need camera for ortho mode

	modelStack.PushMatrix();
    modelStack.LoadIdentity();  //reset modelStack


    modelStack.PushMatrix();
    modelStack.Translate(50, 30, 0);
    modelStack.Scale(2, 2, 2);
    modelStack.Rotate(20, 1, 0, 0);
    modelStack.Rotate(rotating, 0, 1, 0);
    RenderMesh(meshList[GEO_SHOPDISPLAY], true);
    modelStack.PopMatrix();

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);


    modelStack.PopMatrix();
}
    
void SP2::Reset()
{
    SharedData::GetInstance()->camera->Reset();
    Init();
}