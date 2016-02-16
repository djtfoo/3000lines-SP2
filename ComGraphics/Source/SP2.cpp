#include "SP2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"

#include <sstream>

#include "Application.h"

SP2::SP2()
{
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
    camera.Init(Vector3(0, 10, 100), Vector3(0, 10, 110), Vector3(0, 1, 0));

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

    //meshBuilder
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image/Text/small fonts.tga");

    meshList[GEO_DIALOGUEBOX] = MeshBuilder::GenerateQuad("dialogue_box", Color(0, 0, 0), 80, 20);
    meshList[GEO_DIALOGUEBOX]->textureID = LoadTGA("Image/Text/dialogue box.tga");

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

    meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 1000, 1000);
    meshList[GEO_GROUND]->textureID = LoadTGA("Image/ground2.tga");

    meshList[GEO_MAN] = MeshBuilder::GenerateOBJ("man", "OBJ/man.obj");
    meshList[GEO_MAN]->textureID = LoadTGA("Image/man.tga");

    viewOptions = true;
}

SP2::~SP2()
{
}

void SP2::Init()
{
    interactNum = 0;
    dialogueNum = 0;

    //booleans
    haveInteraction = false;
    interactDialogue = false;
    haveQuicktime = false;
    interactPrisonDoor = false;

    lockedDown = false;

    battleMode = false;
    flightMode = false;

    //interaction timings
    elapsedTime = 0.f;
    maxElapsedTime = 3.f;

    //quicktime
    qt_state = QUICKTIME_CLOSEDOOR;
    qt_rotate_arm = 0.f;
    qt_explosion_size = 0.f;
    qt_rotate_card = 0.f;

    //game states
    gameLose = false;
    winAnimation = false;
    blackScreen = false;
    splashScreen = false;

    //wake up
    wakeUp = true;
    wake_state = WAKEUP_SITUP;

    rotate_hologram = 0.f;
    rotate_hologram_speed = 10.f;

    //characters
    Player.pos.Set(0, 0, 0);
    Player.health = 10;
    Player.pitch = 0.f;
    Player.yaw = 0.f;
    Player.takeDamage = false;

    Enemy.pos.Set(6490, 980, 7490);
    Enemy.health = 50;
    Enemy.yaw = 180.f;
    Enemy.takeDamage = false;

    enemyROFelapsedTime = 0.f;

    //light 0
    light[0].type = Light::LIGHT_SPOT;
    light[0].position.Set(6490, 1500, -1000);
    light[0].color.Set(1, 1, 1);
    light[0].power = 500;
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

    /*WallCollision wall;
    //cell
    wall.dir = WALL_LEFT;   wall.coordinate = -485;     collisionWallCell.push_back(wall);
    wall.dir = WALL_RIGHT;  wall.coordinate = 485;   collisionWallCell.push_back(wall);
    wall.dir = WALL_BACK;   wall.coordinate = 485;     collisionWallCell.push_back(wall);

    //corridor
    wall.dir = WALL_FRONT;   wall.coordinate = -1485;    collisionWallCorridor.push_back(wall);
    wall.dir = WALL_LEFT;   wall.coordinate = -985;    collisionWallCorridor.push_back(wall);

    //corridor 2
    wall.dir = WALL_FRONT;  wall.coordinate = -1485;    collisionWallCorridor2.push_back(wall);
    wall.dir = WALL_RIGHT;  wall.coordinate = 6985;     collisionWallCorridor2.push_back(wall);

    //control room
    wall.dir = WALL_RIGHT;  wall.coordinate = 8400;    collisionWallControlRoom.push_back(wall);
    wall.dir = WALL_LEFT;  wall.coordinate = 4700;    collisionWallControlRoom.push_back(wall);
    wall.dir = WALL_BACK;  wall.coordinate = 8300;    collisionWallControlRoom.push_back(wall);

    ItemCollision item;
    //bed
    item.minX = -175;   item.maxX = 500;   item.minZ = 200;   item.maxZ = 500;   collisionItemCell.push_back(item);

    //bars
    item.minX = -600;   item.maxX = 100;   item.minZ = -525;   item.maxZ = -450;   collisionItemCell.push_back(item);
    item.minX = 200;    item.maxX = 600;   item.minZ = -525;   item.maxZ = -450;   collisionItemCell.push_back(item);
    
    //prison door
    itemDoorClose.minX = 50;     itemDoorClose.maxX = 300;   itemDoorClose.minZ = -525;   itemDoorClose.maxZ = -450;
    collisionItemCell.push_back(itemDoorClose);

    itemDoorOpen.minX = 250;    itemDoorOpen.maxX = 400;    itemDoorOpen.minZ = -715;   itemDoorOpen.maxZ = -450;
    
    //corridor
    item.minX = -1000;   item.maxX = 100;   item.minZ = -515;   item.maxZ = -400;   collisionItemCorridor.push_back(item);
    item.minX =  200;   item.maxX = 6000;   item.minZ = -515;   item.maxZ = -400;   collisionItemCorridor.push_back(item);

    //prison door
    collisionItemCorridor.push_back(itemDoorClose);

    item.minX = 5000;   item.maxX = 6015;   item.minZ = -501;   item.maxZ = 4500;   collisionItemCorridor2.push_back(item);
    item.minX = 5950;   item.maxX = 6250;   item.minZ = 4450;   item.maxZ = 4550;   collisionItemCorridor2.push_back(item);
    item.minX = 6700;   item.maxX = 7000;   item.minZ = 4450;   item.maxZ = 4550;   collisionItemCorridor2.push_back(item);

	//globe
    item.minX = 5350;   item.maxX = 6050;   item.minZ = 6150;   item.maxZ = 6850;   collisionItemControlRoom.push_back(item);

    //control panels
    item.minX = 6700;   item.maxX = 8300;   item.minZ = 5550;   item.maxZ = 6200;   collisionItemControlRoom.push_back(item);
    item.minX = 6700;   item.maxX = 8300;   item.minZ = 7050;   item.maxZ = 7700;   collisionItemControlRoom.push_back(item);

    //chairs
    item.minX = 7200;   item.maxX = 7800;   item.minZ = 5250;   item.maxZ = 6200;   collisionItemControlRoom.push_back(item);
    item.minX = 7200;   item.maxX = 7800;   item.minZ = 6750;   item.maxZ = 7700;   collisionItemControlRoom.push_back(item);
    
	//walls
	item.minX = 4700;   item.maxX = 6250;   item.minZ = 4450;   item.maxZ = 4550;   collisionItemControlRoom.push_back(item);
    item.minX = 6700;   item.maxX = 8400;   item.minZ = 4450;   item.maxZ = 4550;   collisionItemControlRoom.push_back(item);

    //control room door
    itemControlRoomDoor.minX = 6250;    itemControlRoomDoor.maxX = 6700;    itemControlRoomDoor.minZ = 4450;    itemControlRoomDoor.maxZ = 4550;

    //stairs
    stairs.dir = STAIR_X;   stairs.minVal = 4000;   stairs.maxVal = 6000;   stairs.minY = 120;    stairs.maxY = 1120;  stairs.steps = 20;  stairs.incrementVal = 100.f; stairs.incrementY = 50.f;

    //item interaction
    ItemInteraction interact;
    //bed
    interact.minX = -175;   interact.maxX = 500;    interact.minY = -500;   interact.maxY = -100;   interact.minZ = 200;    interact.maxZ = 500;    itemCell.push_back(interact);

    //creepy door
    interact.minX = 1550;   interact.maxX = 1900;   interact.minY = -500;   interact.maxY = 300;    interact.minZ = -1600;  interact.maxZ = -1400;  itemCorridor.push_back(interact);

    //prison door
    interact.minX = 50;    interact.maxX = 300;    interact.minY = -500;   interact.maxY = 500;    interact.minZ = -600;   interact.maxZ = -450;
    itemCell.push_back(interact);
    itemCorridor.push_back(interact);

    //globe
    interact.minX = 5350;   interact.maxX = 6050;   interact.minY = 600;   interact.maxY = 1200;   interact.minZ = 6150;   interact.maxZ = 6850;   itemControlRoom.push_back(interact);

    //numlock
    interact.minX = 8350;   interact.maxX = 8600;   interact.minY = 1000;   interact.maxY = 1150;   interact.minZ = 5600;   interact.maxZ = 5800;   itemControlRoom.push_back(interact);

    FlightCollision flight;
    //globe
    flight.minX = 5350; flight.maxX = 6050; flight.minY = 500;  flight.maxY = 1200; flight.minZ = 6150; flight.maxZ = 6850; flightCollision.push_back(flight);

    //control panels
    flight.minX = 6700; flight.maxX = 8300; flight.minY = 500;  flight.maxY = 1050; flight.minZ = 5550; flight.maxZ = 6200; flightCollision.push_back(flight);
    flight.minX = 6700; flight.maxX = 8300; flight.minY = 500;  flight.maxY = 1050; flight.minZ = 7050; flight.maxZ = 7700; flightCollision.push_back(flight);

    //chairs
    flight.minX = 7200; flight.maxX = 7800; flight.minY = 500;  flight.maxY = 1450; flight.minZ = 5250; flight.maxZ = 5550; flightCollision.push_back(flight);
    flight.minX = 7200; flight.maxX = 7800; flight.minY = 500;  flight.maxY = 1450; flight.minZ = 6750; flight.maxZ = 7050; flightCollision.push_back(flight);

    //walls
    wall.dir = WALL_RIGHT;      wall.coordinate = 8400;     flightWall.push_back(wall);
    wall.dir = WALL_LEFT;       wall.coordinate = 4700;     flightWall.push_back(wall);
    wall.dir = WALL_BACK;       wall.coordinate = 8300;     flightWall.push_back(wall);
    wall.dir = WALL_FRONT;      wall.coordinate = 4600;     flightWall.push_back(wall);
    wall.dir = WALL_TOP;        wall.coordinate = 2400;     flightWall.push_back(wall);
    wall.dir = WALL_BOTTOM;     wall.coordinate = 600;      flightWall.push_back(wall);

    //doors
    PrisonDoor.index = 0;
    PrisonDoor.state = DOOR_CLOSE;
    PrisonDoor.change = 0;

    ControlRoomDoor.index = 1;
    ControlRoomDoor.state = DOOR_CLOSE;
    ControlRoomDoor.change = 0;*/
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;
static float LSPEED = 10.f;

static double FramePerSecond;

void SP2::Update(double dt)
{
    FramePerSecond = 1 / dt;

    if (Application::IsKeyPressed('W')) {   //player moves forward
        Player.pos.x -= (float)(5 * sin(Math::DegreeToRadian(Player.yaw)) * dt);
        Player.pos.z -= (float)(5 * cos(Math::DegreeToRadian(Player.yaw)) * dt);
    }
    if (Application::IsKeyPressed('S')) {   //player moves backward
        Player.pos.x += (float)(5 * sin(Math::DegreeToRadian(Player.yaw)) * dt);
        Player.pos.z += (float)(5 * cos(Math::DegreeToRadian(Player.yaw)) * dt);
    }
    if (Application::IsKeyPressed('A')) {   //turn left
        Player.yaw += (float)(50 * dt);
    }
    if (Application::IsKeyPressed('D')) {   //turn right
        Player.yaw -= (float)(50 * dt);
    }

    camera.Update(dt, Player, flightWall, collisionItemCell, stairs);
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

    if (Application::IsKeyPressed('E') && haveInteraction) {
        Interact();
    }
}

void SP2::Render()
{
    // Render VBO here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mtx44 MVP;

    //Set view matrix using camera settings
    viewStack.LoadIdentity();
    viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
        camera.target.x, camera.target.y, camera.target.z,
        camera.up.x, camera.up.y, camera.up.z);

    modelStack.LoadIdentity();

    //pass the position of light to shader based on light type
    //spotlights
    if (light[0].type == Light::LIGHT_SPOT)
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
        Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
        glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    }

    RenderSkybox();
    RenderGround();

    RenderPlayer();

    //checking for item interaction
    //if (haveInteraction) {
    //    RenderTextOnScreen(meshList[GEO_TEXT], "PRESS E", Color(1, 0, 0), 3, 4, 9);
    //}

    //if (interactDialogue) {
    //    Dialogue();
    //}

    ////====NEUTRAL RENDERS/ALL SCENES====
    //if (blackScreen) {
    //    RenderBlackScreen();
    //}

    //if (splashScreen) {
    //    RenderBlackScreen();
    //    if (gameLose) {
    //        RenderTextOnScreen(meshList[GEO_TEXT], "YOU WERE CAPTURED...", Color(1, 1, 1), 3, 3, 10);
    //    }
    //    else {
    //        RenderTextOnScreen(meshList[GEO_TEXT], "YOU MANAGED TO ESCAPE...", Color(1, 1, 1), 3, 2, 10);
    //    }
    //    RenderTextOnScreen(meshList[GEO_TEXT], "OR IS IT?", Color(1, 1, 1), 3, 10, 6);
    //}
    
    //render UI
    RenderUI();
}

void SP2::Exit()
{
    // Cleanup VBO here
    glDeleteVertexArrays(1, &m_vertexArrayID);
    glDeleteProgram(m_programID);
}

void SP2::SetNonGlossMaterial(Mesh* mesh)
{
    mesh->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
    mesh->material.kSpecular.Set(0.05f, 0.05f, 0.05f);
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
    modelStack.Translate(0, -100, Player.pos.z);

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
    modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_GROUND], false);
    modelStack.PopMatrix();
}

void SP2::RenderCardOnScreen(GEOMETRY_TYPE TYPE)
{
    if (!meshList[GEO_CARD_BACK] || !meshList[TYPE])  //error check
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
    modelStack.Translate(40, 30, 0);
    modelStack.Rotate(qt_rotate_card, 0, 1, 0);
    modelStack.Scale(0.5f, 0.55f, 1);
    RenderMesh(meshList[GEO_CARD_BACK], false);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[TYPE], false);
    modelStack.PopMatrix();

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderUI()
{
    if (viewOptions) {
        std::stringstream s;
        s << "FPS:" << FramePerSecond;
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 3, 0, 19);
        s.str("");
        s << "COORD:(" << (int)(Player.pos.x) << "," << (int)(Player.pos.y) << "," << (int)(Player.pos.z) << ")";
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 3, 0, 18);
    }

    if (battleMode) {
        RenderHealth();
    }
    if (flightMode) {
        RenderCrosshairs();
    }
}

void SP2::RenderBlackScreen()
{
    if (!meshList[GEO_BLACKSCREEN])  //error check
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
    modelStack.Translate(40, 30, 0);
    RenderMesh(meshList[GEO_BLACKSCREEN], false);
    modelStack.PopMatrix();

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderHealth()
{
    if (!meshList[GEO_HEALTHBAR])  //error check
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

    //player health
    modelStack.PushMatrix();
    modelStack.Translate(3, 30, 0);
    for (unsigned int i = 0; i < Player.health; ++i) {
        modelStack.Translate(0, 2, 0);
        RenderMesh(meshList[GEO_HEALTHBAR], false);
    }
    modelStack.PopMatrix();

    //enemy health
    int enemyHPBar = (Enemy.health + 4) / 5;
    modelStack.PushMatrix();
    modelStack.Translate(77, 30, 0);
    for (int i = 0; i < enemyHPBar; ++i) {
        modelStack.Translate(0, 2, 0);
        RenderMesh(meshList[GEO_HEALTHBAR], false);
    }
    modelStack.PopMatrix();

    RenderTextOnScreen(meshList[GEO_TEXT], "Player", Color(0.8f, 0.8f, 0.8f), 2.f, 0.5f, 14.f);
    RenderTextOnScreen(meshList[GEO_TEXT], "Enemy", Color(0.8f, 0.8f, 0.8f), 2.f, 35.f, 14.f);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderCrosshairs()
{
    if (!meshList[GEO_CROSSHAIRS] || meshList[GEO_CROSSHAIRS]->textureID <= 0)  //error check
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
    modelStack.Translate(40, 30, 0);
    RenderMesh(meshList[GEO_CROSSHAIRS], false);
    modelStack.PopMatrix();

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderStairs()
{
    //steps
    for (int i = 0; i < stairs.steps; ++i) {
        modelStack.PushMatrix();
        modelStack.Translate(i * stairs.incrementVal - 50, -474 + i * stairs.incrementY, 0);
        modelStack.Rotate(-90, 0, 1, 0);
        RenderMesh(meshList[GEO_STAIRS_VERTICAL], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(i * stairs.incrementVal, -499 + (i + 1) * stairs.incrementY, 0);
        modelStack.Rotate(-90, 0, 0, 1);
        modelStack.Rotate(-90, 0, 1, 0);
        RenderMesh(meshList[GEO_STAIRS_HORIZONTAL], true);
        modelStack.PopMatrix();
    }

    for (int i = 0; i < 4; ++i) {
        //"left" wall
        modelStack.PushMatrix();
        modelStack.Translate(200.f + 500.f * i, 0, -500);
        RenderMesh(meshList[GEO_CORRIDOR_WALL], true);
        modelStack.PopMatrix();
        //"left" wall
        modelStack.PushMatrix();
        modelStack.Translate(200.f + 500.f * i, 1000, -500);
        modelStack.Rotate(180, 0, 0, 1);
        RenderMesh(meshList[GEO_CORRIDOR_WALL], true);
        modelStack.PopMatrix();
        //"right" wall
        modelStack.PushMatrix();
        modelStack.Translate(200.f + 500.f * i, 0, 500);
        modelStack.Rotate(180, 0, 1, 0);
        RenderMesh(meshList[GEO_CORRIDOR_WALL], true);
        modelStack.PopMatrix();
        //"right" wall
        modelStack.PushMatrix();
        modelStack.Translate(200.f + 500.f * i, 1000, 500);
        modelStack.Rotate(180, 0, 0, 1);
        modelStack.Rotate(180, 0, 1, 0);
        RenderMesh(meshList[GEO_CORRIDOR_WALL], true);
        modelStack.PopMatrix();
    }

    for (int i = 0; i < 5; ++i) {
        //ceiling
        modelStack.PushMatrix();
        modelStack.Translate(200.f + i * 447.2f, 499.f + i * 223.6f, -250);
        modelStack.Translate(-250, 0, 0);
        modelStack.Rotate(26.5f, 0, 0, 1);
        modelStack.Translate(250, 0, 0);
        modelStack.Rotate(90, 1, 0, 0);
        RenderMesh(meshList[GEO_CORRIDOR_CEILING], true);
        modelStack.PopMatrix();
        //ceiling
        modelStack.PushMatrix();
        modelStack.Translate(200.f + i * 447.2f, 499.f + i * 223.6f, 250);
        modelStack.Translate(-250, 0, 0);
        modelStack.Rotate(26.5f, 0, 0, 1);
        modelStack.Translate(250, 0, 0);
        modelStack.Rotate(90, 1, 0, 0);
        RenderMesh(meshList[GEO_CORRIDOR_CEILING], true);
        modelStack.PopMatrix();
    }
}

void SP2::RenderCorridor2()
{
    //dead end
    modelStack.PushMatrix();
    modelStack.Translate(250.f, 500.f, -250.f);
    RenderMesh(meshList[GEO_CORRIDOR2_WALL], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(-250.f, 500.f, -250.f);
    RenderMesh(meshList[GEO_CORRIDOR2_WALL], true);
    modelStack.PopMatrix();

    for (int i = 0; i < 12; ++i) {
        //floor
        modelStack.PushMatrix();
        modelStack.Translate(0, 0, i * 499.f);
        modelStack.Rotate(-90, 1, 0, 0);
        RenderMesh(meshList[GEO_CORRIDOR2_FLOOR], true);
        modelStack.PopMatrix();
        //ceiling
        modelStack.PushMatrix();
        modelStack.Translate(250.f, 999.f, 499.f * i);
        modelStack.Rotate(-90, 0, 1, 0);
        modelStack.Rotate(90, 0, 1, 0);
        modelStack.Rotate(90, 1, 0, 0);
        RenderMesh(meshList[GEO_CORRIDOR_CEILING], true);
        modelStack.PopMatrix();
        //ceiling
        modelStack.PushMatrix();
        modelStack.Translate(-250.f, 999.f, 499.f * i);
        modelStack.Rotate(-90, 0, 1, 0);
        modelStack.Rotate(90, 0, 1, 0);
        modelStack.Rotate(90, 1, 0, 0);
        RenderMesh(meshList[GEO_CORRIDOR_CEILING], true);
        modelStack.PopMatrix();
        //"left" wall
        modelStack.PushMatrix();
        modelStack.Translate(500.f, 500.f, 499.f * i);
        modelStack.Rotate(-90, 0, 1, 0);
        RenderMesh(meshList[GEO_CORRIDOR2_WALL], true);
        modelStack.PopMatrix();
        //"right" wall
        if (i >= 2) {
            modelStack.PushMatrix();
            modelStack.Translate(-500.f, 500.f, 499.f * i);
            modelStack.Rotate(90, 0, 1, 0);
            RenderMesh(meshList[GEO_CORRIDOR2_WALL], true);
            modelStack.PopMatrix();
        }
    }
}

void SP2::RenderDoor()
{
    //door
    modelStack.PushMatrix();
    modelStack.Translate(0, 500.f + ControlRoomDoor.change, 0);
    modelStack.Scale(200, 200, 50);
    RenderMesh(meshList[GEO_CORRIDOR2_DOOR], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 500.f, 0);
    modelStack.Scale(200, 200, 50);
    RenderMesh(meshList[GEO_CORRIDOR2_DOORGATE], true);
    modelStack.PopMatrix();
}

void SP2::RenderEscapeDoor()
{
    //door
    modelStack.PushMatrix();
    modelStack.Scale(200, 200, 50);
    RenderMesh(meshList[GEO_CORRIDOR2_DOOR], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Scale(200, 200, 50);
    RenderMesh(meshList[GEO_CORRIDOR2_DOORGATE], true);
    modelStack.PopMatrix();

    //numlock
    modelStack.PushMatrix();
    modelStack.Translate(-600, 0, 0);
    modelStack.Scale(200, 200, 50);
    RenderMesh(meshList[GEO_NUMLOCK], true);
    modelStack.PopMatrix();
}

void SP2::RenderControlRoom()
{
    //bottom - floor
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            modelStack.PushMatrix();
            modelStack.Translate(-1750.f + i * 500.f, 0, -1750.f + j * 500.f);
            modelStack.Rotate(-90, 1, 0, 0);
            RenderMesh(meshList[GEO_ROOM_BOTTOM], true);
            modelStack.PopMatrix();
        }
    }

    //top - ceiling
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            modelStack.PushMatrix();
            modelStack.Translate(-1500.f + i * 1000.f, 2000, -1500.f + j * 1000.f);
            modelStack.Rotate(180, 1, 0, 0);
            modelStack.Rotate(-90, 1, 0, 0);
            RenderMesh(meshList[GEO_ROOM_TOP], true);
            modelStack.PopMatrix();
        }
    }

    //left
    modelStack.PushMatrix();
    modelStack.Translate(-1999, 1000, 0);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_ROOM_LEFT], true);
    modelStack.PopMatrix();

    //right
    modelStack.PushMatrix();
    modelStack.Translate(1999, 1000, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_ROOM_RIGHT], true);
    modelStack.PopMatrix();

    //back - window
    modelStack.PushMatrix();
    modelStack.Translate(0, 1000, 1999);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_ROOM_BACK], true);
    modelStack.PopMatrix();

    //side walls
    modelStack.PushMatrix();
    modelStack.Translate(1250, 1000, -1970);
    RenderMesh(meshList[GEO_ROOM_FRONTSIDE], true);
    modelStack.Translate(-2500, 0, 0);
    RenderMesh(meshList[GEO_ROOM_FRONTSIDE], true);
    modelStack.PopMatrix();
    //top wall
    modelStack.PushMatrix();
    modelStack.Translate(0, 1500, -1970);
    RenderMesh(meshList[GEO_ROOM_FRONTTOP], true);
    modelStack.PopMatrix();

    //left overlay
    modelStack.PushMatrix();
    modelStack.Translate(-1975, 1000, 0);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_ROOM_LEFT_OVERLAY], false);
    modelStack.PopMatrix();
}

void SP2::RenderLab()
{
    //left
    modelStack.PushMatrix();
    modelStack.Translate(-499, 0, 0);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_LAB_LEFT], true);
    modelStack.PopMatrix();

    //right
    modelStack.PushMatrix();
    modelStack.Translate(499, 0, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_LAB_RIGHT], true);
    modelStack.PopMatrix();

    //back
    modelStack.PushMatrix();
    modelStack.Translate(0, 0, 499);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_LAB_BACK], true);
    modelStack.PopMatrix();

    //front
    modelStack.PushMatrix();
    modelStack.Translate(750, 0, -499);
    RenderMesh(meshList[GEO_LAB_FRONT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(-750, 0, -499);
    RenderMesh(meshList[GEO_LAB_FRONT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 0, -499);
    RenderMesh(meshList[GEO_CORRIDOR_CREEPY], true);
    modelStack.PopMatrix();

    //top
    modelStack.PushMatrix();
    modelStack.Translate(0, 499, 0);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_LAB_TOP], true);
    modelStack.PopMatrix();

    //bottom
    modelStack.PushMatrix();
    modelStack.Translate(0, -499, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_LAB_BOTTOM], true);
    modelStack.PopMatrix();

    //screen
    modelStack.PushMatrix();
    modelStack.Translate(-499, 120, -100);
    modelStack.Rotate(90, 0, 1, 0);
    modelStack.Scale(150, 150, 50);
    RenderMesh(meshList[GEO_LAB_SCREEN], false);
    modelStack.PopMatrix();
}

void SP2::RenderEnemy()
{
    if (!battleMode) {      //man
        modelStack.PushMatrix();
        modelStack.Scale(120, 120, 120);
        RenderMesh(meshList[GEO_MAN], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(0, 110.52f, 0);
        modelStack.Rotate(qt_rotate_arm, 1, 0, 0);
        modelStack.Translate(0, -110.52f, 0);
        modelStack.Scale(120, 120, 120);
        RenderMesh(meshList[GEO_MAN_ARM], true);
        modelStack.PopMatrix();

        if (qt_state == QUICKTIME_ROTATEARM || qt_state == QUICKTIME_PAUSE1 || qt_state == QUICKTIME_TRANSFORM) {
            modelStack.PushMatrix();
            modelStack.Translate(141.24f, 0, 0);
            modelStack.Translate(0, 110.52f, 0);
            modelStack.Rotate(qt_rotate_arm, 1, 0, 0);
            modelStack.Translate(0, -110.52f, 0);
            modelStack.Translate(0, -188.24f, -50);
            modelStack.Rotate(-90, 1, 0, 0);

            modelStack.PushMatrix();
            modelStack.Rotate(180, 0, 1, 0);
            RenderMesh(meshList[GEO_CARD_MOTHERSHIP], true);
            modelStack.PopMatrix();

            RenderMesh(meshList[GEO_CARD_BACK], true);

            modelStack.PopMatrix();
        }
    }
    else {      //mothership
        modelStack.PushMatrix();
        modelStack.Scale(120, 120, 120);
        modelStack.Rotate(Enemy.yaw, 0, 1, 0);
        RenderMesh(meshList[GEO_MOTHERSHIP], true);
        modelStack.PopMatrix();
    }
}

void SP2::RenderHologram()
{
    modelStack.PushMatrix();
    modelStack.Translate(-800, 450, 0);

    modelStack.PushMatrix();
    modelStack.Translate(0, -350, 0);
    modelStack.Scale(200, 200, 200);
    RenderMesh(meshList[GEO_HOLOGRAM_BASE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Rotate(rotate_hologram, 0, 1, 0);
    modelStack.Rotate(30, 1, 0, 0);
    modelStack.Scale(300, 300, 300);
    glBlendFunc(1.5, 1);
    RenderMesh(meshList[GEO_HOLOGRAM_EARTH], false);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    modelStack.PopMatrix();

    modelStack.PopMatrix();
}

void SP2::RenderControlPanels()
{
    //panel 1
    modelStack.PushMatrix();
    modelStack.Translate(1000, 250, -500);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(300, 500, 300);
    RenderMesh(meshList[GEO_CONTROLPANEL1], true);
    modelStack.PopMatrix();

    //chair
    modelStack.PushMatrix();
    modelStack.Translate(1000, 250, -500);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(300, 500, 300);
    RenderMesh(meshList[GEO_CHAIR], true);
    modelStack.PopMatrix();

    //panel 2
    modelStack.PushMatrix();
    modelStack.Translate(1000, 250, 1000);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(300, 500, 300);
    RenderMesh(meshList[GEO_CONTROLPANEL2], true);
    modelStack.PopMatrix();

    //chair
    modelStack.PushMatrix();
    modelStack.Translate(1000, 250, 1000);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(300, 500, 300);
    RenderMesh(meshList[GEO_CHAIR], true);
    modelStack.PopMatrix();
}

void SP2::RenderPlayer()
{
    modelStack.PushMatrix();

    modelStack.Translate(0, 1, 0);
    modelStack.Translate(Player.pos.x, Player.pos.y, Player.pos.z);

    modelStack.Rotate(Player.yaw, 0, 1, 0);
    modelStack.Scale(0.2f, 0.2f, 0.2f);
    RenderMesh(meshList[GEO_MAN], true);
    modelStack.PopMatrix();
}

void SP2::RenderBullets()
{
    for (unsigned i = 0; i < playerBullets.size(); ++i) {
        modelStack.PushMatrix();
        modelStack.Translate(playerBullets[i].pos.x, playerBullets[i].pos.y, playerBullets[i].pos.z);
        modelStack.Rotate(playerBullets[i].pitch, 1, 0, 0);
        modelStack.Rotate(playerBullets[i].yaw, 0, 1, 0);
        modelStack.Scale(5, 5, 5);
        RenderMesh(meshList[GEO_BULLETPLAYER], false);
        modelStack.PopMatrix();
    }

    for (unsigned i = 0; i < enemyBullets.size(); ++i) {
        modelStack.PushMatrix();
        modelStack.Translate(enemyBullets[i].pos.x, enemyBullets[i].pos.y, enemyBullets[i].pos.z);
        modelStack.Rotate(enemyBullets[i].yaw, 0, 1, 0);
        modelStack.Scale(5, 5, 5);
        RenderMesh(meshList[GEO_BULLETENEMY], false);
        modelStack.PopMatrix();
    }
}

void SP2::CheckInteraction(const std::vector<ItemInteraction>& itemInteract)
{
    Vector3 view = (camera.target - camera.position).Normalized();
    view = 20 * view;
    Vector3 interaction = camera.target;
    haveInteraction = false;

    for (unsigned i = 0; i < itemInteract.size(); ++i) {
        for (int j = 0; j < 20; ++j, interaction += view) {
            if (interaction.x > itemInteract[i].minX && interaction.x < itemInteract[i].maxX && interaction.y > itemInteract[i].minY && interaction.y < itemInteract[i].maxY && interaction.z > itemInteract[i].minZ && interaction.z < itemInteract[i].maxZ) {
                haveInteraction = true;
                interactNum = i;
                break;
            }
        }
        if (haveInteraction) {
            break;
        }
        interaction = camera.target;
    }
}

void SP2::Interact()
{
    if (camera.position.y < 1120) {     //basement level
        if (camera.target.z > -500) {  //cell
            switch (interactNum)
            {
            case 0: interactDialogue = true;    //bed
                dialogueNum = 0;
                break;
            case 1:     //prison door
                if (!interactPrisonDoor) {
                    interactDialogue = true;
                    dialogueNum = 1;
                    interactPrisonDoor = true;
                }
                break;
            }
        }
        else {      //corridor
            switch (interactNum)
            {
            case 0: interactDialogue = true;    //creepy door
                dialogueNum = 2;
                break;
            case 1:     //prison door
                break;
            }
        }
    }
    else {      //control room level
        if (camera.position.z < 4500) {     //corridor
            
        }
        else {      //control room
            switch (interactNum)
            {
            case 0:     //globe
                if (rotate_hologram_speed != 50.f) {
                    rotate_hologram_speed = 50.f;
                }
                break;
            case 1:     //numlock
                splashScreen = true;
                break;
            }
        }
    }
}

void SP2::Dialogue()
{
    switch (dialogueNum)
    {
    case 0: RenderDialogueOnScreen("Just woke up.", Color(1, 1, 1), 3);
        break;
    case 1: RenderDialogueOnScreen("It's not locked.", Color(1, 1, 1), 3);
        break;
    case 2: RenderDialogueOnScreen("Feels creepy...", Color(1, 0, 0), 3);
        break;
    case 3: RenderDialogueOnScreen("How dare you escape...", Color(1, 1, 1), 3);
        break;
    }

}

void SP2::CheckDoor(Door& door, double dt)
{
    switch (door.index)
    {
    case 0:
        if (Application::IsKeyPressed('E') && haveInteraction && interactNum == 1) {      //prison door
            if (door.state == DOOR_CLOSE) {
                door.state = DOOR_OPENING;
                door.direction = 1;
            }
            if (door.state == DOOR_OPEN) {
                door.state = DOOR_CLOSING;
                door.direction = -1;
            }
        }
        break;

    case 1:
        if (camera.target.z > 2250) {   //door should open
            if (door.state != DOOR_OPEN && door.state != DOOR_OPENING) {
                door.state = DOOR_OPENING;
                door.direction = 1;
            }
        }
        else if (camera.target.z < 2000) {  //door should close
            if (door.state != DOOR_CLOSE && door.state != DOOR_CLOSING) {
                door.state = DOOR_CLOSING;
                door.direction = -1;
            }
        }
        break;
    }

    if (door.state == DOOR_OPENING || door.state == DOOR_CLOSING) {
        MoveDoor(door, dt);
    }
}

void SP2::MoveDoor(Door& door, double dt)
{
    switch (door.index)
    {
    case 0:
        door.change += (float)(door.direction * 100 * dt);
        if (door.change <= 0 || door.change >= 90) {
            if (door.direction == 1) {
                door.state = DOOR_OPEN;
                door.change = 90;
                collisionItemCell.pop_back();
                collisionItemCorridor.pop_back();
                collisionItemCell.push_back(itemDoorOpen);
                collisionItemCorridor.push_back(itemDoorOpen);
            }
            else {
                door.state = DOOR_CLOSE;
                door.change = 0;
                collisionItemCell.pop_back();
                collisionItemCorridor.pop_back();
                collisionItemCell.push_back(itemDoorClose);
                collisionItemCorridor.push_back(itemDoorClose);
            }
        }
        break;
    case 1:
        door.change += (float)(door.direction * 500 * dt);
        if (door.change <= 0 || door.change >= 850) {
            if (door.direction == 1) {
                door.state = DOOR_OPEN;
                door.change = 850;
            }
            else {
                door.state = DOOR_CLOSE;
                door.change = 0;
            }
        }
        break;
    }
}

void SP2::CheckQuicktimeEvent()
{
    if (camera.position.z >= 5100 && ControlRoomDoor.state == DOOR_OPEN) {
        haveQuicktime = true;
    }
}

void SP2::QuickTime(double dt)
{
    switch (qt_state)
    {
    case QUICKTIME_CLOSEDOOR:
        if (ControlRoomDoor.state != DOOR_CLOSING) {
            ControlRoomDoor.state = DOOR_CLOSING;
            ControlRoomDoor.direction = -1;
        }
        MoveDoor(ControlRoomDoor, dt);

        if (ControlRoomDoor.state == DOOR_CLOSE) {
            lockedDown = true;
            light[0].power = 0;
            light[1].power = 0;
            light[2].power = 0;
            glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
            glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
            glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
            collisionItemControlRoom.push_back(itemControlRoomDoor);
            qt_state = QUICKTIME_DIALOGUE;
        }
        break;
    case QUICKTIME_DIALOGUE: interactDialogue = true;
        dialogueNum = 3;
        if (Application::IsKeyPressed(VK_SPACE)) {
            interactDialogue = false;
            qt_state = QUICKTIME_ROTATEARM;
        }
        break;
    case QUICKTIME_ROTATEARM:
        qt_rotate_arm += (float)(50 * dt);
        if (qt_rotate_arm >= 90.f) {
            qt_rotate_arm = 90.f;
            qt_state = QUICKTIME_PAUSE1;
        }
        break;
    case QUICKTIME_PAUSE1:
        elapsedTime += dt;
        if (elapsedTime >= 1.0) {
            elapsedTime = 0;
            qt_state = QUICKTIME_TRANSFORM;
        }
        break;
    case QUICKTIME_TRANSFORM:
        qt_explosion_size += (float)(750 * dt);
        light[5].power += (float)(800 * dt);
        glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
        if (light[5].power >= 2500) {
            battleMode = true;
            Enemy.pos.y += 200.f;
            light[5].position.y += 200.f;
            m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
            qt_state = QUICKTIME_PAUSE2;
            qt_explosion_size = 0.f;
        }
        break;
    case QUICKTIME_PAUSE2:
        elapsedTime += dt;
        if (elapsedTime >= 2.0) {
            elapsedTime = 0;
            qt_state = QUICKTIME_CARDDISPLAY1;
        }
        break;
    case QUICKTIME_CARDDISPLAY1:
        elapsedTime += dt;
        if (elapsedTime >= 1.0) {
            elapsedTime = 0;
            qt_state = QUICKTIME_CARDROTATE;
        }
        break;
    case QUICKTIME_CARDROTATE:
        qt_rotate_card += (float)(200 * dt);
        if (qt_rotate_card >= 180.f) {
            qt_rotate_card = 180.f;
            qt_state = QUICKTIME_CARDDISPLAY2;
        }
        break;
    case QUICKTIME_CARDDISPLAY2:
        elapsedTime += dt;
        if (elapsedTime >= 1.0) {
            elapsedTime = 0;
            qt_state = QUICKTIME_TRANSFORM2;
        }
        break;
    case QUICKTIME_TRANSFORM2:
        qt_explosion_size += (float)(10 * dt);
        if (qt_explosion_size >= 10.f) {
            flightMode = true;
            haveQuicktime = false;
            elapsedTime = 1.f;
        }
        break;
    }
}

void SP2::WinAnimation(double dt)
{
    qt_explosion_size += (float)(400 * dt);
    if (qt_explosion_size >= 1500) {
        qt_explosion_size = 0.f;
        winAnimation = false;
        battleMode = false;
        flightMode = false;
        light[5].power = 0;
        glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
    }
}

void SP2::WakeUp(double dt)
{
    Vector3 view = (camera.target - camera.position).Normalized();
    Mtx44 rotation, rotation2;

    switch (wake_state)
    {
    case WAKEUP_SITUP:
        rotation.SetToIdentity();
        rotation.SetToRotation(-1, 0, 0, 1);
        camera.position = rotation * camera.position;
        view = rotation * view;
        camera.target = camera.position + view;

        if (camera.position.y >= 150.f) {
            camera.position.y = 150.f;
            wake_state = WAKEUP_PAUSE;
        }
        break;
    case WAKEUP_PAUSE:
        elapsedTime += dt;
        if (elapsedTime >= 0.5) {
            elapsedTime = 0;
            wake_state = WAKEUP_TURN;
        }
        break;
    case WAKEUP_TURN:
        rotation.SetToIdentity();
        rotation2.SetToIdentity();
        rotation.SetToRotation(25.f * dt, 0, 1, 0);
        rotation2.SetToRotation(-10.f * dt, 0, 0, 1);
        view = rotation2 * rotation * view;
        camera.target = camera.position + view;

        if (camera.target.y <= 149.65f) {
            wake_state = WAKEUP_GETDOWN;
        }
        break;
    case WAKEUP_GETDOWN:
        camera.position.z -= (float)(200 * dt);
        camera.target.z -= (float)(200 * dt);
        camera.position.y -= (float)(25 * dt);
        camera.target.y -= (float)(25 * dt);

        if (camera.target.y <= 120) {
            wakeUp = false;
            Application::cursor_xpos = Application::cursor_newxpos;
            Application::cursor_ypos = Application::cursor_newypos;
        }
        break;
    }
}

static float BULLETSPEED = 2000.f;

void SP2::PlayerShoot(double dt)
{
    elapsedTime += dt;
    if (Application::IsKeyPressed(VK_LBUTTON) && elapsedTime >= 0.5) {    //rate of fire
        Bullet bullet;
        bullet.pos = Player.pos;
        bullet.dir = BULLETSPEED * (float)(dt) * (camera.target - camera.position).Normalized();
        bullet.collided = false;

        bullet.pitch = Player.pitch;
        bullet.yaw = Player.yaw;

        playerBullets.push_back(bullet);
        elapsedTime = 0.f;  //reset
    }
}

void SP2::EnemyShoot(double dt)
{
    enemyROFelapsedTime += dt;
    if (enemyROFelapsedTime >= 0.3) {    //rate of fire
        Bullet bullet;
        bullet.pos = Enemy.pos;
        bullet.dir = 2.f * BULLETSPEED * (float)(dt) * (camera.target - Enemy.pos).Normalized();
        bullet.collided = false;

        bullet.pitch = 0.f;
        bullet.yaw = Enemy.yaw;

        enemyBullets.push_back(bullet);
        enemyROFelapsedTime = 0.f;  //reset
    }
}

void SP2::MoveBullets(double dt)
{
    //move player bullets
    for (unsigned i = 0; i < playerBullets.size(); ++i) {
        playerBullets[i].pos += playerBullets[i].dir;
    }

    //move enemy bullets
    for (unsigned i = 0; i < enemyBullets.size(); ++i) {
        enemyBullets[i].pos += enemyBullets[i].dir;
    }
}

void SP2::CheckPlayerBullets(Bullet& bullet)
{
    //check collision with wall
    for (unsigned i = 0; i < flightWall.size(); ++i) {
        if (flightWall[i].dir == WALL_LEFT) {
            if (bullet.pos.x < flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_RIGHT) {
            if (bullet.pos.x > flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_FRONT) {
            if (bullet.pos.z < flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_BACK) {
            if (bullet.pos.z > flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_TOP) {
            if (bullet.pos.y > flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_BOTTOM) {
            if (bullet.pos.y < flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }

        //check collision with objects
        for (unsigned i = 0; i < flightCollision.size(); ++i) {
            if (bullet.pos.x > flightCollision[i].minX && bullet.pos.x < flightCollision[i].maxX &&
                bullet.pos.y > flightCollision[i].minY && bullet.pos.y < flightCollision[i].maxY &&
                bullet.pos.z > flightCollision[i].minZ && bullet.pos.z < flightCollision[i].maxZ)
            {
                bullet.collided = true;
                return;
            }
        }

        //check collision with enemy
        if (!Enemy.takeDamage) {
            if (bullet.pos.x > enemyHitbox.minX && bullet.pos.x < enemyHitbox.maxX &&
                bullet.pos.y > enemyHitbox.minY && bullet.pos.y < enemyHitbox.maxY &&
                bullet.pos.z > enemyHitbox.minZ && bullet.pos.z < enemyHitbox.maxZ)
            {
                Enemy.takeDamage = true;
                bullet.collided = true;
                return;
            }
        }
    }

}

void SP2::CheckEnemyBullets(Bullet& bullet)
{
    //check collision with wall
    for (unsigned i = 0; i < flightWall.size(); ++i) {
        if (flightWall[i].dir == WALL_LEFT) {
            if (bullet.pos.x < flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_RIGHT) {
            if (bullet.pos.x > flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_FRONT) {
            if (bullet.pos.z < flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_BACK) {
            if (bullet.pos.z > flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_TOP) {
            if (bullet.pos.y > flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }
        else if (flightWall[i].dir == WALL_BOTTOM) {
            if (bullet.pos.y < flightWall[i].coordinate) {
                bullet.collided = true;
                return;
            }
        }

        //check collision with objects
        for (unsigned i = 0; i < flightCollision.size(); ++i) {
            if (bullet.pos.x > flightCollision[i].minX && bullet.pos.x < flightCollision[i].maxX &&
                bullet.pos.y > flightCollision[i].minY && bullet.pos.y < flightCollision[i].maxY &&
                bullet.pos.z > flightCollision[i].minZ && bullet.pos.z < flightCollision[i].maxZ)
            {
                bullet.collided = true;
                return;
            }
        }

        //check collision with player
        if (!Player.takeDamage) {
            if (bullet.pos.x > playerHitbox.minX && bullet.pos.x < playerHitbox.maxX &&
                bullet.pos.y > playerHitbox.minY && bullet.pos.y < playerHitbox.maxY &&
                bullet.pos.z > playerHitbox.minZ && bullet.pos.z < playerHitbox.maxZ)
            {
                Player.takeDamage = true;
                bullet.collided = true;
                return;
            }
        }
    }

}

void SP2::EditHitboxes()
{
    playerHitbox.minX = Player.pos.x - 50.f;
    playerHitbox.maxX = Player.pos.x + 50.f;
    playerHitbox.minY = Player.pos.y - 50.f;
    playerHitbox.maxY = Player.pos.y + 50.f;
    playerHitbox.minZ = Player.pos.z - 60.f;
    playerHitbox.maxZ = Player.pos.z + 60.f;

    enemyHitbox.minX = Enemy.pos.x - 450.f;
    enemyHitbox.maxX = Enemy.pos.x + 450.f;
    enemyHitbox.minY = Enemy.pos.y - 350.f;
    enemyHitbox.maxY = Enemy.pos.y + 400.f;
    enemyHitbox.minZ = Enemy.pos.z - 600.f;
    enemyHitbox.maxZ = Enemy.pos.z + 450.f;
}

void SP2::Reset()
{
    camera.Reset();

    //clear vectors
    collisionWallCell.clear();
    collisionWallCorridor.clear();
    collisionWallCorridor2.clear();
    collisionWallControlRoom.clear();

    collisionItemCell.clear();
    collisionItemCorridor.clear();
    collisionItemCorridor2.clear();
    collisionItemControlRoom.clear();

    itemCell.clear();
    itemCorridor.clear();
    itemControlRoom.clear();

    flightCollision.clear();
    flightWall.clear();
    playerBullets.clear();
    enemyBullets.clear();

    Init();
    /*camera.position.Set(-150, -100, 350);
    camera.target.Set(-150, 100, 350);

    wakeUp = true;
    wake_state = WAKEUP_SITUP;

    //reset interactions
    haveInteraction = false;
    interactDialogue = false;
    interactPrisonDoor = false;

    //reset doors
    PrisonDoor.state = DOOR_CLOSE;
    PrisonDoor.change = 0;

    ControlRoomDoor.state = DOOR_CLOSE;
    ControlRoomDoor.change = 0;*/
}