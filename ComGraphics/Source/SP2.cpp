/******************************************************************************/
/*!
\file	SP2.cpp
\author 3000 Lines Studio
\brief
SP2 Scene where the game takes place
*/
/******************************************************************************/

#include "SP2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"

#include <fstream>
#include <sstream>

#include "Application.h"
#include "SharedData.h"

/******************************************************************************/
/*!
\brief	SP2 default constructor
*/
/******************************************************************************/
SP2::SP2()
{
    //for rabbit fight
    pitch = 0;
    yaw = 0;
    elapsedTime = 0;
    e_elapsedTime = 0;
    spreadfire = false;

    delayer = 0;
    delayBuffer = 0;
    lightpower = 0.3f;
    lightpos = 1000;
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
    ////light 1
    //m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
    //m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
    //m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
    //m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
    //m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
    //m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
    //m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
    //m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
    //m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
    //m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
    //m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
    ////light 2
    //m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
    //m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
    //m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
    //m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
    //m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
    //m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
    //m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
    //m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
    //m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
    //m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
    //m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");
    ////light 3
    //m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
    //m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
    //m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
    //m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
    //m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
    //m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
    //m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
    ////light 4
    //m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
    //m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
    //m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
    //m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
    //m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
    //m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
    //m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
    //m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
    //m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
    //m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
    //m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");
    ////light 5
    //m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
    //m_parameters[U_LIGHT5_COLOR] = glGetUniformLocation(m_programID, "lights[5].color");
    //m_parameters[U_LIGHT5_POWER] = glGetUniformLocation(m_programID, "lights[5].power");
    //m_parameters[U_LIGHT5_KC] = glGetUniformLocation(m_programID, "lights[5].kC");
    //m_parameters[U_LIGHT5_KL] = glGetUniformLocation(m_programID, "lights[5].kL");
    //m_parameters[U_LIGHT5_KQ] = glGetUniformLocation(m_programID, "lights[5].kQ");
    //m_parameters[U_LIGHT5_TYPE] = glGetUniformLocation(m_programID, "lights[5].type");
    //m_parameters[U_LIGHT5_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[5].spotDirection");
    //m_parameters[U_LIGHT5_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[5].cosCutoff");
    //m_parameters[U_LIGHT5_COSINNER] = glGetUniformLocation(m_programID, "lights[5].cosInner");
    //m_parameters[U_LIGHT5_EXPONENT] = glGetUniformLocation(m_programID, "lights[5].exponent");
    ////light 6
    //m_parameters[U_LIGHT6_POSITION] = glGetUniformLocation(m_programID, "lights[6].position_cameraspace");
    //m_parameters[U_LIGHT6_COLOR] = glGetUniformLocation(m_programID, "lights[6].color");
    //m_parameters[U_LIGHT6_POWER] = glGetUniformLocation(m_programID, "lights[6].power");
    //m_parameters[U_LIGHT6_KC] = glGetUniformLocation(m_programID, "lights[6].kC");
    //m_parameters[U_LIGHT6_KL] = glGetUniformLocation(m_programID, "lights[6].kL");
    //m_parameters[U_LIGHT6_KQ] = glGetUniformLocation(m_programID, "lights[6].kQ");
    //m_parameters[U_LIGHT6_TYPE] = glGetUniformLocation(m_programID, "lights[6].type");
    //m_parameters[U_LIGHT6_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[6].spotDirection");
    //m_parameters[U_LIGHT6_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[6].cosCutoff");
    //m_parameters[U_LIGHT6_COSINNER] = glGetUniformLocation(m_programID, "lights[6].cosInner");
    //m_parameters[U_LIGHT6_EXPONENT] = glGetUniformLocation(m_programID, "lights[6].exponent");
    ////light 7
    //m_parameters[U_LIGHT7_POSITION] = glGetUniformLocation(m_programID, "lights[7].position_cameraspace");
    //m_parameters[U_LIGHT7_COLOR] = glGetUniformLocation(m_programID, "lights[7].color");
    //m_parameters[U_LIGHT7_POWER] = glGetUniformLocation(m_programID, "lights[7].power");
    //m_parameters[U_LIGHT7_KC] = glGetUniformLocation(m_programID, "lights[7].kC");
    //m_parameters[U_LIGHT7_KL] = glGetUniformLocation(m_programID, "lights[7].kL");
    //m_parameters[U_LIGHT7_KQ] = glGetUniformLocation(m_programID, "lights[7].kQ");
    //m_parameters[U_LIGHT7_TYPE] = glGetUniformLocation(m_programID, "lights[7].type");


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

    meshList[GEO_LOADTOP] = MeshBuilder::GenerateQuad("load_top", Color(1, 1, 1), 80, 70);
    meshList[GEO_LOADTOP]->textureID = LoadTGA("Image/Text/loadTop.tga");
    meshList[GEO_LOADBTM] = MeshBuilder::GenerateQuad("load_btm", Color(1, 1, 1), 80, 70);
    meshList[GEO_LOADBTM]->textureID = LoadTGA("Image/Text/loadBtm.tga");

    //
    meshList[GEO_HUNGER_BAR] = MeshBuilder::GenerateOBJ("hunger_bar", "OBJ/bar_bar.obj");
    meshList[GEO_HUNGER_BAR]->textureID = LoadTGA("Image/hunger_bar.tga");

    meshList[GEO_HUNGER_UI] = MeshBuilder::GenerateQuad("hunger_ui", Color(0, 0, 0), 100, 5);
    //meshList[GEO_HUNGER_UI]->textureID = LoadTGA("Image/Text/hunger_uibar.tga");

    meshList[GEO_DAYS_UI] = MeshBuilder::GenerateCircle("days_ui", Color(0, 0, 1), 10, 10);
    //meshList[GEO_DAYS_UI]->textureID = LoadTGA("Image/Text/day_ui.tga");

    meshList[GEO_SHOPUI] = MeshBuilder::GenerateQuad("shop_ui", Color(1.f, 0.8f, 0.8f), 1, 1);
    meshList[GEO_SHOPUI]->textureID = LoadTGA("Image/layout/canteen_walls.tga");

    meshList[GEO_HEART] = MeshBuilder::GenerateQuad("love meter heart", Color(1, 0, 0), 3, 3);
    meshList[GEO_HEART]->textureID = LoadTGA("Image/heart.tga");

    meshList[GEO_LOVEMETER_BAR] = MeshBuilder::GenerateOBJ("hunger_bar", "OBJ/bar_bar.obj");
    meshList[GEO_LOVEMETER_BAR]->textureID = LoadTGA("Image/lovemeter.tga");

    meshList[GEO_BOSSHEALTH_BAR] = MeshBuilder::GenerateOBJ("bosshealth_bar", "OBJ/bar_bar.obj");
    meshList[GEO_BOSSHEALTH_BAR]->textureID = LoadTGA("Image/bosshpmeter.tga");

    meshList[GEO_PLAYERHEALTH_BAR] = MeshBuilder::GenerateOBJ("hunger_bar", "OBJ/bar_bar.obj");
    meshList[GEO_PLAYERHEALTH_BAR]->textureID = LoadTGA("Image/playerhpmeter.tga");

    //skybox
    meshList[GEO_SKYBOX_LEFT] = MeshBuilder::GenerateQuad("skybox_left", Color(1, 1, 1), 1500, 1500);
    meshList[GEO_SKYBOX_LEFT]->textureID = LoadTGA("Image/Skybox/redplanet_left.tga");

    meshList[GEO_SKYBOX_RIGHT] = MeshBuilder::GenerateQuad("skybox_right", Color(1, 1, 1), 1500, 1500);
    meshList[GEO_SKYBOX_RIGHT]->textureID = LoadTGA("Image/Skybox/redplanet_right.tga");

    meshList[GEO_SKYBOX_TOP] = MeshBuilder::GenerateQuad("skybox_top", Color(1, 1, 1), 1500, 1500);
    meshList[GEO_SKYBOX_TOP]->textureID = LoadTGA("Image/Skybox/redplanet_top.tga");

    meshList[GEO_SKYBOX_BOTTOM] = MeshBuilder::GenerateQuad("skybox_bottom", Color(1, 1, 1), 1500, 1500);
    meshList[GEO_SKYBOX_BOTTOM]->textureID = LoadTGA("Image/Skybox/redplanet_bottom.tga");

    meshList[GEO_SKYBOX_BACK] = MeshBuilder::GenerateQuad("skybox_back", Color(1, 1, 1), 1500, 1500);
    meshList[GEO_SKYBOX_BACK]->textureID = LoadTGA("Image/Skybox/redplanet_back.tga");

    meshList[GEO_SKYBOX_FRONT] = MeshBuilder::GenerateQuad("skybox_front", Color(1, 1, 1), 1500, 1500);
    meshList[GEO_SKYBOX_FRONT]->textureID = LoadTGA("Image/Skybox/redplanet_front.tga");

    meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 2000, 2000);
    meshList[GEO_GROUND]->textureID = LoadTGA("Image/ground2.tga");

	//meshList[GEO_DEVTEXTURE] = MeshBuilder::GenerateQuad("devtexture", Color(1, 1, 1), 1, 1);
	//meshList[GEO_DEVTEXTURE]->textureID = LoadTGA("Image/devtexture.tga");
    
    //player
    meshList[GEO_PLAYER_HEAD] = MeshBuilder::GenerateOBJ("player head", "OBJ/cube.obj");
    meshList[GEO_PLAYER_BODY] = MeshBuilder::GenerateOBJ("player body", "OBJ/cube.obj");
    meshList[GEO_PLAYER_UPPERARM] = MeshBuilder::GenerateOBJ("player upp arm", "OBJ/cube.obj");
    meshList[GEO_PLAYER_LOWERARM] = MeshBuilder::GenerateOBJ("player lower arm", "OBJ/lower arm.obj");
    meshList[GEO_PLAYER_LEG] = MeshBuilder::GenerateOBJ("player leg", "OBJ/cube.obj");

    //Render NPCs
    meshList[GEO_ADOLPH] = MeshBuilder::GenerateOBJ("man", "OBJ/machoaf.obj");
    meshList[GEO_ADOLPH]->textureID = LoadTGA("Image/macho.tga");
    meshList[GEO_ADOLPH]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_ADOLPH]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_ADOLPH]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_ADOLPH]->material.kShininess = 1.f;

    meshList[GEO_STEMMIE] = MeshBuilder::GenerateOBJ("stemmie", "OBJ/hOi.obj");
    meshList[GEO_STEMMIE]->textureID = LoadTGA("Image/temmie.tga");
    meshList[GEO_STEMMIE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_STEMMIE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_STEMMIE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_STEMMIE]->material.kShininess = 1.f;

    meshList[GEO_STEMMIE_FACE] = MeshBuilder::GenerateQuad("stemmieface", Color(1, 1, 1), 1, 1);
    meshList[GEO_STEMMIE_FACE]->textureID = LoadTGA("Image/hOiWAT.tga");

    meshList[GEO_WENGSTANG] = MeshBuilder::GenerateOBJ("wengstang", "OBJ/toriel.obj");
    meshList[GEO_WENGSTANG]->textureID = LoadTGA("Image/toriel.tga");
    meshList[GEO_WENGSTANG]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_WENGSTANG]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_WENGSTANG]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_WENGSTANG]->material.kShininess = 1.f;

    meshList[GEO_VEE] = MeshBuilder::GenerateOBJ("vee", "OBJ/asgore.obj");
    meshList[GEO_VEE]->textureID = LoadTGA("Image/asgore.tga");
    meshList[GEO_VEE]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_VEE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_VEE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_VEE]->material.kShininess = 1.f;

    meshList[GEO_JASIM] = MeshBuilder::GenerateOBJ("jasim", "OBJ/sans.obj");
    meshList[GEO_JASIM]->textureID = LoadTGA("Image/sans.tga");
    meshList[GEO_JASIM]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_JASIM]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
    meshList[GEO_JASIM]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_JASIM]->material.kShininess = 1.f;

    meshList[GEO_CHON] = MeshBuilder::GenerateOBJ("chon", "OBJ/chon.obj");
    meshList[GEO_CHON]->textureID = LoadTGA("Image/chonUV.tga");
    meshList[GEO_CHON]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_CHON]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_CHON]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_CHON]->material.kShininess = 1.f;

    meshList[GEO_LAYOUT] = MeshBuilder::GenerateOBJ("big map", "OBJ/bigareaFixed.obj");
    meshList[GEO_LAYOUT]->textureID = LoadTGA("Image/layout/bigareatxture.tga");
    meshList[GEO_LAYOUT]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_LAYOUT]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_LAYOUT]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
    meshList[GEO_LAYOUT]->material.kShininess = 1.f;

    meshList[GEO_GATE1] = MeshBuilder::GenerateOBJ("gate1", "OBJ/spgate.obj");
    meshList[GEO_GATE1]->textureID = LoadTGA("Image/layout/spGateTGA.tga");
    meshList[GEO_GATE1]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_GATE1]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_GATE1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
    meshList[GEO_GATE1]->material.kShininess = 1.f;

    meshList[GEO_GATE2] = MeshBuilder::GenerateOBJ("gate1", "OBJ/spgate.obj");
    meshList[GEO_GATE2]->textureID = LoadTGA("Image/layout/spGate2TGA.tga");
    meshList[GEO_GATE2]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_GATE2]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_GATE2]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
    meshList[GEO_GATE2]->material.kShininess = 1.f;


    //Objects
    meshList[GEO_BENCH] = MeshBuilder::GenerateOBJ("bench", "OBJ/bench.obj");
    meshList[GEO_BENCH]->textureID = LoadTGA("Image/bench.tga");
    meshList[GEO_BENCH]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_BENCH]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_BENCH]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
    meshList[GEO_BENCH]->material.kShininess = 1.f;

    meshList[GEO_TOOLBOX] = MeshBuilder::GenerateOBJ("toolbox", "OBJ/toolbox.obj");
    meshList[GEO_TOOLBOX]->textureID = LoadTGA("Image/toolbox.tga");
    meshList[GEO_TOOLBOX]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_TOOLBOX]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_TOOLBOX]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_TOOLBOX]->material.kShininess = 1.f;

    meshList[GEO_VENDINGMACHINE] = MeshBuilder::GenerateOBJ("vendingmachine", "OBJ/vendingmachine.obj");
    meshList[GEO_VENDINGMACHINE]->textureID = LoadTGA("Image/vendingmachine.tga");
    meshList[GEO_VENDINGMACHINE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_VENDINGMACHINE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_VENDINGMACHINE]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_VENDINGMACHINE]->material.kShininess = 1.f;

    meshList[GEO_TABLE] = MeshBuilder::GenerateOBJ("table", "OBJ/table.obj");
    meshList[GEO_TABLE]->textureID = LoadTGA("Image/table.tga");
    meshList[GEO_TABLE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_TABLE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_TABLE]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
    meshList[GEO_TABLE]->material.kShininess = 1.f;

    meshList[GEO_HAMMER] = MeshBuilder::GenerateOBJ("hammer", "OBJ/hammer.obj");
    meshList[GEO_HAMMER]->textureID = LoadTGA("Image/hammer.tga");
    meshList[GEO_HAMMER]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_HAMMER]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_HAMMER]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_HAMMER]->material.kShininess = 1.f;

    //New Objects
    meshList[GEO_BOXCRATE] = MeshBuilder::GenerateOBJ("boxcrate", "OBJ/boxcrate.obj");
    meshList[GEO_BOXCRATE]->textureID = LoadTGA("Image/boxcrate.tga");
    meshList[GEO_BOXCRATE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_BOXCRATE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_BOXCRATE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_BOXCRATE]->material.kShininess = 1.f;

    meshList[GEO_CHAIR] = MeshBuilder::GenerateOBJ("chair", "OBJ/chair.obj");
    meshList[GEO_CHAIR]->textureID = LoadTGA("Image/chair.tga");
    meshList[GEO_CHAIR]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_CHAIR]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_CHAIR]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
    meshList[GEO_CHAIR]->material.kShininess = 1.f;

    meshList[GEO_CONTROLROOM] = MeshBuilder::GenerateOBJ("controlroom", "OBJ/controlroom.obj");
    meshList[GEO_CONTROLROOM]->textureID = LoadTGA("Image/controlroom.tga");
    meshList[GEO_CONTROLROOM]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_CONTROLROOM]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
    meshList[GEO_CONTROLROOM]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
    meshList[GEO_CONTROLROOM]->material.kShininess = 1.f;

    meshList[GEO_FIREEXTINGUISHER] = MeshBuilder::GenerateOBJ("fireextinguisher", "OBJ/fireextinguisher.obj");
    meshList[GEO_FIREEXTINGUISHER]->textureID = LoadTGA("Image/fireextinguisher.tga");
    meshList[GEO_FIREEXTINGUISHER]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_FIREEXTINGUISHER]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_FIREEXTINGUISHER]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_FIREEXTINGUISHER]->material.kShininess = 1.f;

    meshList[GEO_FRIDGE] = MeshBuilder::GenerateOBJ("fridge", "OBJ/fridgedefault.obj");
    meshList[GEO_FRIDGE]->textureID = LoadTGA("Image/fridgedefault.tga");
    meshList[GEO_FRIDGE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_FRIDGE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_FRIDGE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
    meshList[GEO_FRIDGE]->material.kShininess = 1.f;
    
    meshList[GEO_PLANT] = MeshBuilder::GenerateOBJ("plant", "OBJ/plant.obj");
    meshList[GEO_PLANT]->textureID = LoadTGA("Image/plant.tga");
    meshList[GEO_PLANT]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_PLANT]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_PLANT]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_PLANT]->material.kShininess = 1.f;

    meshList[GEO_SHELF] = MeshBuilder::GenerateOBJ("shelf", "OBJ/shelf.obj");
    meshList[GEO_SHELF]->textureID = LoadTGA("Image/shelf.tga");
    meshList[GEO_SHELF]->material.kAmbient.Set(0.9f, 0.9f, 0.9f);
    meshList[GEO_SHELF]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SHELF]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SHELF]->material.kShininess = 1.f;

    meshList[GEO_SHOPSHELF] = MeshBuilder::GenerateOBJ("shelf", "OBJ/shelfshop.obj");
    meshList[GEO_SHOPSHELF]->textureID = LoadTGA("Image/bench.tga");
    meshList[GEO_SHOPSHELF]->material.kAmbient.Set(0.9f, 0.9f, 0.9f);
    meshList[GEO_SHOPSHELF]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SHOPSHELF]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SHOPSHELF]->material.kShininess = 1.f;

    meshList[GEO_TEMSHOP] = MeshBuilder::GenerateOBJ("temshop", "OBJ/temshop.obj");
    meshList[GEO_TEMSHOP]->textureID = LoadTGA("Image/temshop.tga");
    meshList[GEO_TEMSHOP]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_TEMSHOP]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_TEMSHOP]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
    meshList[GEO_TEMSHOP]->material.kShininess = 1.f;

    meshList[GEO_UMBRELLASTAND] = MeshBuilder::GenerateOBJ("umbrellastand", "OBJ/umbrellastand.obj");
    meshList[GEO_UMBRELLASTAND]->textureID = LoadTGA("Image/umbrellastand.tga");
    meshList[GEO_UMBRELLASTAND]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_UMBRELLASTAND]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_UMBRELLASTAND]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_UMBRELLASTAND]->material.kShininess = 1.f;

    meshList[GEO_TOILBOWL] = MeshBuilder::GenerateOBJ("toiletbowl", "OBJ/toilbowl.obj");
    meshList[GEO_TOILBOWL]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_TOILBOWL]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_TOILBOWL]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_TOILBOWL]->material.kShininess = 1.f;

    meshList[GEO_LMAO] = MeshBuilder::GenerateQuad("beautifulstuff", Color(0, 0, 0), 5, 5);
    meshList[GEO_LMAO]->textureID = LoadTGA("Image/LMAO.tga");
    meshList[GEO_LMAO]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_LMAO]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_LMAO]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_LMAO]->material.kShininess = 1.f;

    meshList[GEO_SHOPDISPLAY] = MeshBuilder::GenerateCube("display", Color(0, 0, 1), 5, 5, 5);
    meshList[GEO_SHOPDISPLAY]->textureID = LoadTGA("Image/layout/canteen_walls.tga");
    meshList[GEO_SHOPDISPLAY]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
    meshList[GEO_SHOPDISPLAY]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SHOPDISPLAY]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SHOPDISPLAY]->material.kShininess = 1.f;

    meshList[GEO_SHOP_ARROW] = MeshBuilder::GenerateQuad("shop UI arrow", Color(0, 0, 0), 8, 8);
    meshList[GEO_SHOP_ARROW]->textureID = LoadTGA("Image/shop arrow.tga");

    meshList[GEO_COUNTER] = MeshBuilder::GenerateCube("counter", Color(0.f, 0.7f, 1.f), 5, 5, 5);

    //Shop Display Objects
    meshList[GEO_ANIPOSTER] = MeshBuilder::GenerateOBJ("shop_poster", "OBJ/poster.obj");
    meshList[GEO_ANIPOSTER]->textureID = LoadTGA("Image/shop/aniPoster.tga");
    meshList[GEO_COOKBOOK] = MeshBuilder::GenerateOBJ("cookbook", "OBJ/shop/genBook.obj");
    meshList[GEO_COOKBOOK]->textureID = LoadTGA("Image/shop/cookbook.tga");
    meshList[GEO_CHONKEY] = MeshBuilder::GenerateOBJ("chon", "OBJ/shop/smallChon.obj");
    meshList[GEO_CHONKEY]->textureID = LoadTGA("Image/chonUV.tga");
    meshList[GEO_SHOVEL] = MeshBuilder::GenerateOBJ("butter pie", "OBJ/shop/shovel.obj");
    meshList[GEO_SHOVEL]->textureID = LoadTGA("Image/shop/shovel.tga");
    meshList[GEO_BOARDGAME] = MeshBuilder::GenerateOBJ("butter pie", "OBJ/shop/boardgame.obj");
    meshList[GEO_BOARDGAME]->textureID = LoadTGA("Image/shop/boardGame.tga");
    meshList[GEO_BUTTERPIE] = MeshBuilder::GenerateOBJ("butter pie", "OBJ/shop/pi.obj");
    meshList[GEO_BUTTERPIE]->textureID = LoadTGA("Image/shop/butterpie.tga");
    meshList[GEO_NOVEL] = MeshBuilder::GenerateOBJ("novel", "OBJ/shop/genBook.obj");
    meshList[GEO_NOVEL]->textureID = LoadTGA("Image/shop/novel.tga");
    meshList[GEO_JOKEBOOK] = MeshBuilder::GenerateOBJ("jokebook", "OBJ/shop/genBook.obj");
    meshList[GEO_JOKEBOOK]->textureID = LoadTGA("Image/shop/jokebook.tga");


    meshList[GEO_LABCOUNTER1] = MeshBuilder::GenerateOBJ("toiletbowl", "OBJ/linerLabtable.obj");
    meshList[GEO_LABCOUNTER1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_LABCOUNTER1]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_LABCOUNTER1]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_LABCOUNTER1]->material.kShininess = 1.f;

    meshList[GEO_LABCOUNTER_CORNER] = MeshBuilder::GenerateOBJ("toiletbowl", "OBJ/cornerLabtable.obj");
    meshList[GEO_LABCOUNTER_CORNER]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_LABCOUNTER_CORNER]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_LABCOUNTER_CORNER]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_LABCOUNTER_CORNER]->material.kShininess = 1.f;

    meshList[GEO_SPHEREYELLOW] = MeshBuilder::GenerateSphere("sphereyellow", Color(1, 1, 0), 5, 36, 18);
    meshList[GEO_SPHEREYELLOW]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHEREYELLOW]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHEREYELLOW]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHEREYELLOW]->material.kShininess = 1.f;

    meshList[GEO_SPHEREBLACK] = MeshBuilder::GenerateSphere("sphereblack", Color(0, 0, 0), 5, 36, 18);
    meshList[GEO_SPHEREBLACK]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHEREBLACK]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHEREBLACK]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHEREBLACK]->material.kShininess = 1.f;

    meshList[GEO_SPHEREWHITE] = MeshBuilder::GenerateSphere("spherewhite", Color(1, 1, 1), 5, 36, 18);
    meshList[GEO_SPHEREWHITE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHEREWHITE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHEREWHITE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHEREWHITE]->material.kShininess = 1.f;

    meshList[GEO_SPHEREBLUE] = MeshBuilder::GenerateSphere("sphereblue", Color(0.f, 0.3f, 1.f), 5, 36, 18);
    meshList[GEO_SPHEREBLUE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHEREBLUE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHEREBLUE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHEREBLUE]->material.kShininess = 1.f;

    meshList[GEO_SPHERERED] = MeshBuilder::GenerateSphere("spherered", Color(1.f, 0.3f, 0.f), 5, 36, 18);
    meshList[GEO_SPHERERED]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHERERED]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHERERED]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHERERED]->material.kShininess = 1.f;

    //Lab secrets
    meshList[GEO_SECRETGUIDE] = MeshBuilder::GenerateOBJ("shop_poster", "OBJ/poster.obj");
    meshList[GEO_SECRETGUIDE]->textureID = LoadTGA("Image/labPosters/secretGuide.tga");
    meshList[GEO_POSTSAO] = MeshBuilder::GenerateOBJ("shop_poster", "OBJ/poster.obj");
    meshList[GEO_POSTSAO]->textureID = LoadTGA("Image/labPosters/saoPoster.tga");
    meshList[GEO_POSTPUNCH] = MeshBuilder::GenerateOBJ("shop_poster", "OBJ/poster.obj");
    meshList[GEO_POSTPUNCH]->textureID = LoadTGA("Image/labPosters/opPoster.tga");
    meshList[GEO_POSTKYOJ] = MeshBuilder::GenerateOBJ("shop_poster", "OBJ/poster.obj");
    meshList[GEO_POSTKYOJ]->textureID = LoadTGA("Image/labPosters/kyojPoster.tga");
    meshList[GEO_POSTFATE] = MeshBuilder::GenerateOBJ("shop_poster", "OBJ/poster.obj");
    meshList[GEO_POSTFATE]->textureID = LoadTGA("Image/labPosters/fatePoster.tga");
    meshList[GEO_POSTDGRAY] = MeshBuilder::GenerateOBJ("shop_poster", "OBJ/poster.obj");
    meshList[GEO_POSTDGRAY]->textureID = LoadTGA("Image/labPosters/dgryPoster.tga");
    meshList[GEO_POSTRETT] = MeshBuilder::GenerateOBJ("shop_poster", "OBJ/poster.obj");
    meshList[GEO_POSTRETT]->textureID = LoadTGA("Image/labPosters/rettPoster.tga");

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
    meshList[GEO_CROSSHAIRS] = MeshBuilder::GenerateQuad("crosshairs", Color(1, 1, 1), 3, 3);
    meshList[GEO_CROSSHAIRS]->textureID = LoadTGA("Image/crosshairs.tga");

	meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("invbar", Color(1, 1, 1), 40, 5);
	meshList[GEO_INVENTORY]->textureID = LoadTGA("Image/inventorybar.tga");

	meshList[GEO_ITEMSELECT] = MeshBuilder::GenerateQuad("itemselect", Color(1, 1, 1), 4, 4);
	meshList[GEO_ITEMSELECT]->textureID = LoadTGA("Image/itemselect.tga");

	meshList[GEO_DAYNIGHTICON] = MeshBuilder::GenerateQuad("daynight", Color(1, 1, 1), 12, 12);
	meshList[GEO_DAYNIGHTICON]->textureID = LoadTGA("Image/daynighticon.tga");

	meshList[GEO_TOILETFLOOD] = MeshBuilder::GenerateQuad("toiletflood", Color(1, 1, 1), 18, 18);
	meshList[GEO_TOILETFLOOD]->textureID = LoadTGA("Image/shltwater.tga");

	meshList[GEO_SPAGHETTO] = MeshBuilder::GenerateOBJ("spaghetto", "OBJ/spaghetto.obj");
	meshList[GEO_SPAGHETTO]->textureID = LoadTGA("Image/spaghetto.tga");
	meshList[GEO_SPAGHETTO]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SPAGHETTO]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPAGHETTO]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_SPAGHETTO]->material.kShininess = 1.f;

	meshList[GEO_SPAGHETTOROTTEN] = MeshBuilder::GenerateOBJ("spaghettorotten", "OBJ/spaghetto.obj");
	meshList[GEO_SPAGHETTOROTTEN]->textureID = LoadTGA("Image/spaghettorotten.tga");
	meshList[GEO_SPAGHETTOROTTEN]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SPAGHETTOROTTEN]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPAGHETTOROTTEN]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_SPAGHETTOROTTEN]->material.kShininess = 1.f;

	meshList[GEO_BED] = MeshBuilder::GenerateOBJ("bed", "OBJ/bed.obj");
	meshList[GEO_BED]->textureID = LoadTGA("Image/bed.tga");
	meshList[GEO_BED]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BED]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BED]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_BED]->material.kShininess = 1.f;

    meshList[GEO_JAGCUPBOARDS] = MeshBuilder::GenerateOBJ("stairshelf", "OBJ//stairshelf.obj");
    meshList[GEO_JAGCUPBOARDS]->textureID = LoadTGA("Image//480wood.tga");
    meshList[GEO_TECHCHAIR] = MeshBuilder::GenerateOBJ("techChair", "OBJ/techChair.obj");
    meshList[GEO_TECHCHAIR]->textureID = LoadTGA("Image/layout/control_walls.tga");

	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJ("fence", "OBJ/fence.obj");
	meshList[GEO_FENCE]->textureID = LoadTGA("Image/fence.tga");
	meshList[GEO_FENCE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_FENCE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_FENCE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FENCE]->material.kShininess = 1.f;

	meshList[GEO_SHED] = MeshBuilder::GenerateOBJ("shed", "OBJ/shed.obj");
	meshList[GEO_SHED]->textureID = LoadTGA("Image/shed.tga");
	meshList[GEO_SHED]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SHED]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SHED]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_SHED]->material.kShininess = 1.f;

	meshList[GEO_DETERMINATION] = MeshBuilder::GenerateOBJ("determination", "OBJ/determination.obj");
	meshList[GEO_DETERMINATION]->textureID = LoadTGA("Image/determination.tga");
	meshList[GEO_DETERMINATION]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_DETERMINATION]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_DETERMINATION]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_DETERMINATION]->material.kShininess = 1.f;

	meshList[GEO_MEWMEWKISSYCUTIE] = MeshBuilder::GenerateOBJ("mewmewkissycutie", "OBJ/mewmewkissycutie.obj");
	meshList[GEO_MEWMEWKISSYCUTIE]->textureID = LoadTGA("Image/mewmewkissycutie.tga");
	meshList[GEO_MEWMEWKISSYCUTIE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_MEWMEWKISSYCUTIE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_MEWMEWKISSYCUTIE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_MEWMEWKISSYCUTIE]->material.kShininess = 1.f;

	meshList[GEO_DOGFOOD] = MeshBuilder::GenerateOBJ("dogfood", "OBJ/dogfood.obj");
	meshList[GEO_DOGFOOD]->textureID = LoadTGA("Image/dogfood.tga");
	meshList[GEO_DOGFOOD]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_DOGFOOD]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_DOGFOOD]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_DOGFOOD]->material.kShininess = 1.f;

	meshList[GEO_TRASH] = MeshBuilder::GenerateOBJ("trash", "OBJ/trash.obj");
	meshList[GEO_TRASH]->textureID = LoadTGA("Image/trash.tga");
	meshList[GEO_TRASH]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_TRASH]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_TRASH]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TRASH]->material.kShininess = 1.f;

	meshList[GEO_BOOK] = MeshBuilder::GenerateOBJ("book", "OBJ/book.obj");
	meshList[GEO_BOOK]->textureID = LoadTGA("Image/book.tga");
	meshList[GEO_BOOK]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BOOK]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BOOK]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_BOOK]->material.kShininess = 1.f;

    meshList[GEO_POSTER] = MeshBuilder::GenerateOBJ("trash", "OBJ/poster.obj");
    meshList[GEO_POSTER]->textureID = LoadTGA("Image/testposter.tga");
    meshList[GEO_POSTER]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_POSTER]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_POSTER]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_POSTER]->material.kShininess = 1.f;

    meshList[GEO_KEYBOARD] = MeshBuilder::GenerateOBJ("keyboard", "OBJ/poster.obj");
    meshList[GEO_KEYBOARD]->textureID = LoadTGA("Image/keyboard.tga");


    //SIGNBOARDS
    meshList[GEO_SIGNLAB] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNLAB]->textureID = LoadTGA("Image/signboards/labSign.tga");
    meshList[GEO_SIGNCANTEEN] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNCANTEEN]->textureID = LoadTGA("Image/signboards/canteenSign.tga");
    meshList[GEO_SIGNLEFT] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNLEFT]->textureID = LoadTGA("Image/signboards/leftSign.tga");
    meshList[GEO_SIGNRIGHT] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNRIGHT]->textureID = LoadTGA("Image/signboards/rightSign.tga");
    meshList[GEO_SIGNFARM] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNFARM]->textureID = LoadTGA("Image/signboards/farmSign.tga");
    meshList[GEO_SIGNCTRL] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNCTRL]->textureID = LoadTGA("Image/signboards/controlSign.tga");
    meshList[GEO_SIGNROOM1] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNROOM1]->textureID = LoadTGA("Image/testposter.tga");
    meshList[GEO_SIGNROOM2] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNROOM2]->textureID = LoadTGA("Image/testposter.tga");
    meshList[GEO_SIGNROOM3] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNROOM3]->textureID = LoadTGA("Image/signboards/playerSign.tga");
    meshList[GEO_SIGNROOM4] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNROOM4]->textureID = LoadTGA("Image/testposter.tga");
    meshList[GEO_SIGNTOILET] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNTOILET]->textureID = LoadTGA("Image/signboards/toiletSign.tga");
    meshList[GEO_SIGNSHOP] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNSHOP]->textureID = LoadTGA("Image/signboards/shopSign.tga");
    meshList[GEO_SIGNCOMP1] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNCOMP1]->textureID = LoadTGA("Image/signboards/comp1.tga");
    meshList[GEO_SIGNCOMP2] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNCOMP2]->textureID = LoadTGA("Image/signboards/comp2.tga");
    meshList[GEO_SIGNCOMP3] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNCOMP3]->textureID = LoadTGA("Image/signboards/comp3.tga");
    meshList[GEO_SIGNCOVER] = MeshBuilder::GenerateOBJ("trash", "OBJ/screen1.obj");
    meshList[GEO_SIGNCOVER]->textureID = LoadTGA("Image/signboards/coverSign.tga");

	    //GEO_SWITCH
    meshList[GEO_SWITCH_1] = MeshBuilder::GenerateCube("switch1", Color(1, 0, 0), 1, 15, 4);
    meshList[GEO_SWITCH_2] = MeshBuilder::GenerateCube("switch2", Color(1, 0, 0), 1, 15, 4);
    meshList[GEO_SWITCH_3] = MeshBuilder::GenerateCube("switch3", Color(1, 0, 0), 1, 15, 4);
    meshList[GEO_SWITCH_4] = MeshBuilder::GenerateCube("switch4", Color(1, 0, 0), 1, 15, 4);

	meshList[GEO_FARM] = MeshBuilder::GenerateQuad("farm", Color(0, 0, 0), 20, 20);
	meshList[GEO_FARM]->textureID = LoadTGA("Image/farm.tga");

	meshList[GEO_WEED] = MeshBuilder::GenerateQuad("weed", Color(1, 1, 1), 10, 10);
	meshList[GEO_WEED]->textureID = LoadTGA("Image/weed.tga");

	meshList[GEO_WEEDICON] = MeshBuilder::GenerateQuad("weed", Color(1, 1, 1), 3, 3);
	meshList[GEO_WEEDICON]->textureID = LoadTGA("Image/weed.tga");

	meshList[GEO_FARMPLANT] = MeshBuilder::GenerateQuad("farmplant", Color(1, 1, 1), 20, 20);
	meshList[GEO_FARMPLANT]->textureID = LoadTGA("Image/farmplant.tga");

    meshList[GEO_CHECK_1] = MeshBuilder::GenerateHemisphere("check1", Color(0, 0, 0), 2);
    meshList[GEO_CHECK_2] = MeshBuilder::GenerateHemisphere("check2", Color(0, 0, 0), 2);
    meshList[GEO_CHECK_3] = MeshBuilder::GenerateHemisphere("check3", Color(0, 0, 0), 2);
    meshList[GEO_CHECK_4] = MeshBuilder::GenerateHemisphere("check4", Color(0, 0, 0), 2);

    meshList[GEO_CHECKER_BUTTON] = MeshBuilder::GenerateCube("checker switch", Color(1, 1, 1), 10, 10, 10);

    meshList[GEO_CURSOR] = MeshBuilder::GenerateQuad("mouse_custom", Color(0, 0, 0), 3, 4);
    meshList[GEO_CURSOR]->textureID = LoadTGA("Image/mouse.tga");

    meshList[GEO_DIALOGUEOPTION] = MeshBuilder::GenerateQuad("selection button", Color(0, 0, 0), 30, 5);

    viewOptions = true;

    loadDown = -115;
    loadUp = 175;

    std::string temp[LOCATION_TOTAL] = {
        "OUTSIDE",
        "HANGAR",
        "CORRIDOR",
        "SHOP",
        "LABORATORY",
        "CONTROLROOM",
        "CANTEEN",
        "TOILET",
        "ROOM1",
        "ROOM2",
        "ROOM3",
        "ROOM4",
        "PLANTATION"
    };
    
    for (LOCATION i = OUTSIDE; i < LOCATION_TOTAL; i = static_cast<LOCATION>(i+1)) {
        locations[i] = temp[i];
    }

    meshList[GEO_RABBIT_SKYBOX] = MeshBuilder::GenerateQuad("rabbitskybox", Color(1, 1, 1), 1500, 1500);
    meshList[GEO_RABBIT_SKYBOX]->textureID = LoadTGA("Image/Skybox/rabbitsky.tga");

    meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("bullet", Color(0, 0, 1), 20);
    meshList[GEO_E_BULLET] = MeshBuilder::GenerateSphere("e_bullet", Color(1, 0, 0), 20);
}

/******************************************************************************/
/*!
\brief	SP2 default destructor
*/
/******************************************************************************/
SP2::~SP2()
{
}
/******************************************************************************/
/*!
\brief	SP2 Loading Inventory

\return None
*/
/******************************************************************************/
void SP2::loadInv()
{
	invmap.insert(std::pair<int, Gift>(1, Gift("Hammer", 100)));
	modelmap.insert(std::pair<int, GEOMETRY_TYPE>(1, GEO_HAMMER));
	invmap.insert(std::pair<int, Gift>(2, Gift("tEmmEh", 999999999)));
	modelmap.insert(std::pair<int, GEOMETRY_TYPE>(2, GEO_STEMMIE));
	invmap.insert(std::pair<int, Gift>(3, Gift("Weed", 4)));
	modelmap.insert(std::pair<int, GEOMETRY_TYPE>(3, GEO_WEEDICON));
	invmap.insert(std::pair<int, Gift>(4, Gift("Spaghetti", 10)));
	modelmap.insert(std::pair<int, GEOMETRY_TYPE>(4, GEO_SPAGHETTO));
	invmap.insert(std::pair<int, Gift>(5, Gift("\"spaghetti\"", 2)));
	modelmap.insert(std::pair<int, GEOMETRY_TYPE>(5, GEO_SPAGHETTOROTTEN));

    invmap.insert(std::pair<int, Gift>(6, Gift("Cookbook", 350)));
    modelmap.insert(std::pair<int, GEOMETRY_TYPE>(6, GEO_COOKBOOK));
    invmap.insert(std::pair<int, Gift>(7, Gift("Anime Poster", 300)));
    modelmap.insert(std::pair<int, GEOMETRY_TYPE>(7, GEO_ANIPOSTER));
    invmap.insert(std::pair<int, Gift>(8, Gift("Anime Keychain", 500)));
    modelmap.insert(std::pair<int, GEOMETRY_TYPE>(8, GEO_CHONKEY));
    invmap.insert(std::pair<int, Gift>(9, Gift("Shovel", 200)));
    modelmap.insert(std::pair<int, GEOMETRY_TYPE>(9, GEO_SHOVEL));
    invmap.insert(std::pair<int, Gift>(10, Gift("Butterscotch Pie", 650)));
    modelmap.insert(std::pair<int, GEOMETRY_TYPE>(10, GEO_BUTTERPIE));
    invmap.insert(std::pair<int, Gift>(11, Gift("Board Game", 550)));
    modelmap.insert(std::pair<int, GEOMETRY_TYPE>(11, GEO_BOARDGAME));
    invmap.insert(std::pair<int, Gift>(12, Gift("Novel", 700)));
    modelmap.insert(std::pair<int, GEOMETRY_TYPE>(12, GEO_NOVEL));
    invmap.insert(std::pair<int, Gift>(13, Gift("Jokebook", 700)));
    modelmap.insert(std::pair<int, GEOMETRY_TYPE>(13, GEO_JOKEBOOK));
}
/******************************************************************************/
/*!
\brief	SP2 loading Collisions
    Load Collision of Player, Objects, Enemies
\return None
*/
/******************************************************************************/
void SP2::loadCollisions()
{
    vector<ItemCollision> outsideVec, ICvec;
    ItemCollision IC;
    int loc;
    //IC.minX = 0;    IC.maxX = 0;    IC.minZ = 0;    IC.maxZ = 0;    ICvec.push_back(IC);

    for (LOCATION loc = OUTSIDE; loc < LOCATION_TOTAL; loc = static_cast<LOCATION>(loc+1)) {
        SharedData::GetInstance()->collisionMap.insert(std::pair<LOCATION, vector<ItemCollision>>(loc, ICvec));
    }

    std::ifstream inData;
    std::string data;
    inData.open("collisions.txt");

    if (inData.is_open()) {
        while (!inData.eof()) {
            std::getline(inData, data);

            std::stringstream dataStream(data);
            std::string line;

            //1st data
            std::getline(dataStream, line, ',');
            if (line == "#") {  //a comment
                continue;
            }
            loc = std::stoi(line);

            //2nd data
            std::getline(dataStream, line, ',');
            IC.minX = std::stof(line);

            //3rd data
            std::getline(dataStream, line, ',');
            IC.maxX = std::stof(line);

            //4th data
            std::getline(dataStream, line, ',');
            IC.minZ = std::stof(line);

            //5th data
            std::getline(dataStream, line);
            IC.maxZ = std::stof(line);

            map<LOCATION, vector<ItemCollision>>::iterator it = SharedData::GetInstance()->collisionMap.find(static_cast<LOCATION>(loc));
            it->second.push_back(IC);
        }
    }
    inData.close();
}

vector<Interaction*>spaghettilocation;

void SP2::Init()
{
    //light 0
    light[0].type = Light::LIGHT_DIRECTIONAL;
    light[0].position.Set(0, 1000, lightpos);
    light[0].color.Set(1, 1, 1);
    light[0].power = 0.5f;
    light[0].kC = 1.f;
    light[0].kL = 0.01f;
    light[0].kQ = 0.001f;

    //light[1].type = Light::LIGHT_SPOT;
    //light[1].position.Set(0, 5, 1);
    //light[1].color.Set(1, 1, 1);
    //light[1].power = 1;
    //light[1].kC = 1.f;
    //light[1].kL = 0.01f;
    //light[1].kQ = 0.001f;
    //
    //light[1].cosCutoff = cos(Math::DegreeToRadian(45));
    //light[1].cosInner = cos(Math::DegreeToRadian(30));
    //light[1].exponent = 3.f;
    //light[1].spotDirection.Set(0.f, 1.f, 0.f);
    //
    //light[2].type = Light::LIGHT_SPOT;
    //light[2].position.Set(0, 6, 1);
    //light[2].color.Set(1, 1, 1);
    //light[2].power = 1;
    //light[2].kC = 1.f;
    //light[2].kL = 0.01f;
    //light[2].kQ = 0.001f;
    //
    //light[2].cosCutoff = cos(Math::DegreeToRadian(45));
    //light[2].cosInner = cos(Math::DegreeToRadian(30));
    //light[2].exponent = 3.f;
    //light[2].spotDirection.Set(0.f, 1.f, 0.f);
    //
    //light[3].type = Light::LIGHT_POINT;
    //light[3].position.Set(0, 7, 1);
    //light[3].color.Set(1, 1, 1);
    //light[3].power = 1;
    //light[3].kC = 1.f;
    //light[3].kL = 0.01f;
    //light[3].kQ = 0.001f;
    //
    //light[4].type = Light::LIGHT_SPOT;
    //light[4].position.Set(0, 8, 1);
    //light[4].color.Set(1, 1, 1);
    //light[4].power = 1;
    //light[4].kC = 1.f;
    //light[4].kL = 0.01f;
    //light[4].kQ = 0.001f;
    //
    //light[4].cosCutoff = cos(Math::DegreeToRadian(45));
    //light[4].cosInner = cos(Math::DegreeToRadian(30));
    //light[4].exponent = 3.f;
    //light[4].spotDirection.Set(0.f, 1.f, 0.f);
    //
    //light[5].type = Light::LIGHT_SPOT;
    //light[5].position.Set(0, 9, 1);
    //light[5].color.Set(1, 1, 1);
    //light[5].power = 1;
    //light[5].kC = 1.f;
    //light[5].kL = 0.01f;
    //light[5].kQ = 0.001f;
    //
    //light[5].cosCutoff = cos(Math::DegreeToRadian(45));
    //light[5].cosInner = cos(Math::DegreeToRadian(30));
    //light[5].exponent = 3.f;
    //light[5].spotDirection.Set(0.f, 1.f, 0.f);
    //
    //light[6].type = Light::LIGHT_SPOT;
    //light[6].position.Set(0, 10, 1);
    //light[6].color.Set(1, 1, 1);
    //light[6].power = 1;
    //light[6].kC = 1.f;
    //light[6].kL = 0.01f;
    //light[6].kQ = 0.001f;
    //
    //light[6].cosCutoff = cos(Math::DegreeToRadian(45));
    //light[6].cosInner = cos(Math::DegreeToRadian(30));
    //light[6].exponent = 3.f;
    //light[6].spotDirection.Set(0.f, 1.f, 0.f);
    //
    //light[7].type = Light::LIGHT_POINT;
    //light[7].position.Set(0, 2, 1);
    //light[7].color.Set(1, 1, 1);
    //light[7].power = 1;
    //light[7].kC = 1.f;
    //light[7].kL = 0.01f;
    //light[7].kQ = 0.001f;
    
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
    ////light 1
    //glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
    //glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
    //glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
    //glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
    //glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
    //glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
    //glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
    //glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
    //glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
    ////light 2
    //glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
    //glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
    //glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
    //glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
    //glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
    //glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
    //glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
    //glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
    //glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);
    ////light 3
    //glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
    //glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
    //glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
    //glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
    //glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
    //glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
    ////light 4
    //glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
    //glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
    //glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
    //glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
    //glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
    //glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
    //glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], light[4].cosCutoff);
    //glUniform1f(m_parameters[U_LIGHT4_COSINNER], light[4].cosInner);
    //glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);
    ////light 5
    //glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
    //glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
    //glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
    //glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
    //glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
    //glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
    //glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], light[5].cosCutoff);
    //glUniform1f(m_parameters[U_LIGHT5_COSINNER], light[5].cosInner);
    //glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);
    ////light 6
    //glUniform1i(m_parameters[U_LIGHT6_TYPE], light[6].type);
    //glUniform3fv(m_parameters[U_LIGHT6_COLOR], 1, &light[6].color.r);
    //glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
    //glUniform1f(m_parameters[U_LIGHT6_KC], light[6].kC);
    //glUniform1f(m_parameters[U_LIGHT6_KL], light[6].kL);
    //glUniform1f(m_parameters[U_LIGHT6_KQ], light[6].kQ);
    //glUniform1f(m_parameters[U_LIGHT6_COSCUTOFF], light[6].cosCutoff);
    //glUniform1f(m_parameters[U_LIGHT6_COSINNER], light[6].cosInner);
    //glUniform1f(m_parameters[U_LIGHT6_EXPONENT], light[6].exponent);
    ////light 7
    //glUniform1i(m_parameters[U_LIGHT7_TYPE], light[7].type);
    //glUniform3fv(m_parameters[U_LIGHT7_COLOR], 1, &light[7].color.r);
    //glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
    //glUniform1f(m_parameters[U_LIGHT7_KC], light[7].kC);
    //glUniform1f(m_parameters[U_LIGHT7_KL], light[7].kL);
    //glUniform1f(m_parameters[U_LIGHT7_KQ], light[7].kQ);
   



    glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	
	srand((unsigned int)time(0));

    Interaction* interactions;

	//spaghetti random spawns
	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976f, 12.f, -378.31f + 2.f); interactions->bound2.Set(763.976f, 15.f, -368.31f - 2.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976f, 12.f, -391.7f + 2.f); interactions->bound2.Set(763.976f, 15.f, -381.7f - 2.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976f, 12.f, -437.6f + 2.f); interactions->bound2.Set(763.976f, 15.f, -427.6f - 2.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976f, 12.f, -452.9f + 2.f); interactions->bound2.Set(763.976f, 15.f, -442.9f - 2.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976f, 12.f, -498.8f + 2.f); interactions->bound2.Set(763.976f, 15.f, -488.8f - 2.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976f, 12.f, -510.7f + 2.f); interactions->bound2.Set(763.976f, 15.f, -500.7f - 2.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872f - 3.f, 12.f, -373.31f - 3.f); interactions->bound2.Set(721.872f + 3.f, 15.f, -373.31f + 3.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872f - 3.f, 12.f, -386.7f - 3.f); interactions->bound2.Set(721.872f + 3.f, 15.f, -386.7f + 3.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872f - 3.f, 12.f, -432.6f - 3.f); interactions->bound2.Set(721.872f + 3.f, 15.f, -432.6f + 3.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

    
	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872f - 3.f, 12.f, -447.9f - 3.f); interactions->bound2.Set(721.872f + 3.f, 15.f, -447.9f + 3.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872f - 3.f, 12.f, -493.8f - 3.f); interactions->bound2.Set(721.872f + 3.f, 15.f, -493.8f + 3.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872f - 3.f, 12.f, -505.7f - 3.f); interactions->bound2.Set(721.872f + 3.f, 15.f, -505.7f + 3.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);
	//spaghetto random spawns

    //Gate interaction bounds Inits
    gateOpenInteractions(); //interaction number = 12 ~ 19 (16~19 = rooms)

    //Other Interaction Bounds
    interactions = new ShopInteraction();
    interactions->bound1.Set(890.f, 20.f, -35.f);     interactions->bound2.Set(900.f, 38.f, -25.f);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

	interactions = new ManureInteraction();
	interactions->bound1.Set(780.f, 5.f, -616.f);     interactions->bound2.Set(800.f, 15.f, -612.f);
	SharedData::GetInstance()->interactionItems.push_back(interactions);

    ballboundfunct();

    //Vee Puzzle Interaction
    interactions = new VeePuzzleSwitchOneInteraction();
    interactions->bound1.Set(597.f, 10.f, 490.f);    interactions->bound2.Set(606.f, 25.f, 498.f);
    SharedData::GetInstance()->interactionItems.push_back(interactions);
    interactions = new VeePuzzleSwitchTwoInteraction();
    interactions->bound1.Set(580.f, 10.f, 490.f);    interactions->bound2.Set(587.f, 25.f, 498.f);
    SharedData::GetInstance()->interactionItems.push_back(interactions);
    interactions = new VeePuzzleSwitchThreeInteraction();
    interactions->bound1.Set(564.f, 10.f, 490.f);    interactions->bound2.Set(575.f, 25.f, 498.f);
    SharedData::GetInstance()->interactionItems.push_back(interactions);
    interactions = new VeePuzzleSwitchFourInteraction();
    interactions->bound1.Set(549.f, 10.f, 490.f);    interactions->bound2.Set(558.f, 25.f, 498.f);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

    
    interactions = new VeePuzzleCheckerSwitchInteraction();
    interactions->bound1.Set(608, 15, 490);    interactions->bound2.Set(617, 22, 500);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

	interactions = new BedTime();
	interactions->bound1.Set(518, 5, 176); interactions->bound2.Set(548, 20, 185);
	SharedData::GetInstance()->interactionItems.push_back(interactions);

    //Interacting with Jasim
    interactions = new Dialogue();
    interactions->bound1.Set(685 - 12, 5, -430 - 12); interactions->bound2.Set(685 + 12, 35, -430 + 12);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

    //Interacting with Chon
    interactions = new Dialogue();
    interactions->bound1.Set(400, 10, -479); interactions->bound2.Set(425, 35, -439);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

    //Interacting with Vee
    interactions = new Dialogue();
    interactions->bound1.Set(600 - 10, 8, 440 - 10); interactions->bound2.Set(600 + 10, 35, 440 + 10);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

    //Interacting with Wengstang
    interactions = new Dialogue();
    interactions->bound1.Set(950 -8, 5, -460 - 8); interactions->bound2.Set(950 + 8, 25, -460 + 8);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

	interactions = new SpaghettoInteraction();
	for (int i = 0; i < 12; i++)
	{
		if ((rand() % 12) > 9)
		{
		}
		else
		{
			interactions->bound1.Set(9999, 99, 9999); interactions->bound2.Set(9999, 99, 9999);
			SharedData::GetInstance()->interactionItems[i] = interactions;
		}
	}

    loadWeedGame();

	Interaction* interactions2;
    for (int i = 0; i < 10; i++)
    {
        interactions2 = new WeedInteraction();
        interactions2->bound1.Set(weedgame[i].x - 3, 0, weedgame[i].z - 3); interactions2->bound2.Set(weedgame[i].x + 3, 6, weedgame[i].z + 3);
        SharedData::GetInstance()->interactionItems.push_back(interactions2);
    }

    

	for (unsigned int i = 10; i < weedgame.size(); i++)
	{
		interactions = new FarmPlantInteraction();
		interactions->bound1.Set(weedgame[i].x - 3, 0, weedgame[i].z - 3); interactions->bound2.Set(weedgame[i].x + 3, 7, weedgame[i].z + 3);
		SharedData::GetInstance()->interactionItems.push_back(interactions);
	}
	
    rotating = 0;
    rotator = 0;
	SharedData::GetInstance()->daynighttime = 0000;

    //Init chon game variables
    for (int i = 0; i < 5; i++)
    {
        ball[i] = false;
    }
    ballyellX = 483; ballyellZ = -469;
    ballbluX = 339; ballbluZ = -465; 
    ballredX = 310; ballredZ = -364;
    ballwhitey = 15;
    ballblacky = 15;
    pickupcheck = false;
    pickupCounter = 0;

	soundtimer = 1;
    sleepTime = 0;

    postercounter = 0;

	loadInv();
    loadCollisions();

    lightpuzz.generatePuzzle();

    SharedData::GetInstance()->cursor_xpos = SharedData::GetInstance()->cursor_newxpos;
    SharedData::GetInstance()->cursor_ypos = SharedData::GetInstance()->cursor_newypos;
}

void SP2::loadWeedGame()
{
	if (weedgame.size() != 0)
	{
		Interaction* interactions4;
		for (int i = 0; i < 10; i++)
		{
			weedgame[i] = (Vector3(rand() % 140 + 865, 1 + (0.1f * i), rand() % 100 - 400));
			interactions4 = new WeedInteraction();
			interactions4->bound1.Set(weedgame[i].x - 3, 0, weedgame[i].z - 3); interactions4->bound2.Set(weedgame[i].x + 3, 6, weedgame[i].z + 3);
			SharedData::GetInstance()->interactionItems[i + 37] = interactions4;
		}
		for (unsigned int i = 10; i < weedgame.size(); i++)
		{
            weedgame[i] = (Vector3(rand() % 140 + 865, 10 + (0.1f * i), rand() % 100 - 400));
			interactions4 = new FarmPlantInteraction();
			interactions4->bound1.Set(weedgame[i].x - 3, 0, weedgame[i].z - 3); interactions4->bound2.Set(weedgame[i].x + 3, 7, weedgame[i].z + 3);
			SharedData::GetInstance()->interactionItems[i + 37] = interactions4;
		}

	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
            weedgame.push_back(Vector3(rand() % 140 + 865, 1 + (0.1f * i), rand() % 100 - 400));
		}
		for (int i = 10; i < 30; i++)
		{
            weedgame.push_back(Vector3(rand() % 140 + 865, 10 + (0.1 * i), rand() % 100 - 400));
		}
	}
	
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;
static float LSPEED = 10.f;
static double FramePerSecond;

void SP2::loadSpaghetti()
{
	Interaction* remover;
	remover = new SpaghettoInteraction();
	for (int i = 0; i < 12; i++)
	{
		if ((rand() % 12) > 3)
		{
			remover->bound1.Set(9999, 99, 9999); remover->bound2.Set(9999, 99, 9999);
			SharedData::GetInstance()->interactionItems[i] = remover;
		}
		else
		{
			SharedData::GetInstance()->interactionItems[i] = spaghettilocation[i];
		}
	}
}

void SP2::Update(double dt)
{
    FramePerSecond = 1 / dt;

    loadDown += (float)(30 * dt);
    loadUp -= (float)(30 * dt);

    if (loadDown >= 160 || loadUp <= -100)
    {
        loadDown = 160;
        loadUp = -100;
    }

    //temporary check - change to switch
    if (SharedData::GetInstance()->paused) {
        if (pause.animation) {
            pause.PauseAnimation(dt);
        }
        else {
            pause.CheckCursor(dt);
        }
    }
    else if (SharedData::GetInstance()->sleep) {
        Sleep(dt);    //clears off key press bools and cursor movement
    }
    else if (SharedData::GetInstance()->gamestate == GAME_STATE_DIALOGUE) {
        if (SharedData::GetInstance()->dialogueProcessor.convostate == CONVO_GIFT) {
            SharedData::GetInstance()->dialogueProcessor.GiveGift(dt);
        }
        SharedData::GetInstance()->dialogueProcessor.CheckCursor(dt);
    }
    else if (SharedData::GetInstance()->gamestate == GAME_STATE_SHOP) {
        shop.CheckCursor(dt, (int)(invmap.find(*shop.shopIterator)->second.getValue()));
        UpdateInventory(dt);
    }
    else {
        SharedData::GetInstance()->player->Walk(dt);
        SharedData::GetInstance()->camera->Update(dt);
        UpdateInventory(dt);
    }

    CheckCharacterLocation();
    
    SharedData::GetInstance()->player->CheckInteraction();

    if (SharedData::GetInstance()->canInteract && SharedData::GetInstance()->interactnumber >= 12 && SharedData::GetInstance()->interactnumber <= 15)
    {
        SharedData::GetInstance()->interactptr->DoInteraction();
    }

	if (Application::IsKeyPressed(SharedData::GetInstance()->interactbutton) && SharedData::GetInstance()->canInteract && delayBuffer >= 2) 
    {
		if (SharedData::GetInstance()->interactnumber != 32)
		{
			delayBuffer = 0;
		}
		else
		{
			loadSpaghetti();
			loadWeedGame();
			SharedData::GetInstance()->weedcounter = 10;
		}
        SharedData::GetInstance()->interactptr->DoInteraction();
    }
    else
    {
        delayBuffer += 0.1f;
        if (delayBuffer >= 2)
        {
            delayBuffer = 2;
        }
    }

	rotator += (float)(10 * dt);

    //day-night time increasing
    if (!SharedData::GetInstance()->paused && SharedData::GetInstance()->gamestate != GAME_STATE_SHOP) {
        SharedData::GetInstance()->daynighttime += (float)(dt * 10);
        if (((int)SharedData::GetInstance()->daynighttime % 100) > 60)
        {
            SharedData::GetInstance()->daynighttime += 40;
            if (SharedData::GetInstance()->interactnumber != 32)
            {
                SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() + 5);
            }
            SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() + 1);
        }
        if (SharedData::GetInstance()->daynighttime > 2400)
        {
            SharedData::GetInstance()->daynighttime = 0;
            SharedData::GetInstance()->daynumber++;
            for (int i = 0; i < SharedData::GetInstance()->player->itemHave(4); i++)
            {
                if (rand() % 2)
                {
                    SharedData::GetInstance()->player->convertItem(4, 5);
                }
            }
        }

    }

    //timer
    if (SharedData::GetInstance()->gamestate == GAME_STATE_JASIMGAME || SharedData::GetInstance()->gamestate == GAME_STATE_CHONGAME) {
        SharedData::GetInstance()->timeElapsed += dt;
    }

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
    /*if (Application::IsKeyPressed('R')) {       //reset
        Reset();
    }*/
    if (Application::IsKeyPressed(VK_ESCAPE)) {
        SharedData::GetInstance()->paused = true;
    }

    if (vibrateX < 0.6 && vibrateY < 0.6)
    {
        vibrateX += 0.25f;
        vibrateY += 0.17f;
    }
    else
    {
        vibrateX = 0;
        vibrateY = 0;
    }

    if (chonFloat == false)
    {
        chonFloaty += 0.1f;
        if (chonFloaty >= 10.0f)
        {
            chonFloat = true;
        }
    }
    if (chonFloat == true)
    {
        chonFloaty -= 0.1f;
        if (chonFloaty <= 0)
        {
            chonFloat = false;
        }
    }

	if (SharedData::GetInstance()->player->getHunger() <= 0)
		SharedData::GetInstance()->player->setHunger(0);
	if (SharedData::GetInstance()->player->getHunger() >= 100)
		SharedData::GetInstance()->player->setHunger(100);

    rotating += (float)(30 * dt);

    gateUpdate(dt);

    if (Application::IsKeyPressed('P'))
    {
        SharedData::GetInstance()->gamestate = GAME_STATE_RABBIT;

        SharedData::GetInstance()->player->position_= Vector3(5200, 25, 5000);
        SharedData::GetInstance()->camera->position = Vector3(5200, 25, 5000);
        SharedData::GetInstance()->camera->target = Vector3(5200, 25, 5001);
        //set hunger to 0
        //stop timer
        //camera i think
    }
	if (Application::IsKeyPressed('0'))
	{
		loadWeedGame();
	}
	SharedData::GetInstance()->interactnumber = 99;

    puzzleLogic();
    
 //   if ((SharedData::GetInstance()->player->footstepsound > true)&&(soundtimer < 0))
	//{
	//	PlaySound(TEXT("Sound/footsteps.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//	soundtimer = SharedData::GetInstance()->player->footstepsound;
	//}
	//soundtimer--;
    if (enemy.isDead() == false && player.isDead() == false)
    {
        bulletUpadtes(dt);
        if (enemy.getHealth() <= 400)
        {
            powerspike = 2;
        }
        if (enemy.getHealth() <= 300)
        {
            powerspike = 5;
        }
        if (enemy.getHealth() <= 150)
        {
            powerspike = 8;
        }
        if (enemy.getHealth() <= 100)
        {
            powerspike = 9;
        }
        if (enemy.getHealth() <= 50)
        {
            powerspike = 10;
        }
        if (enemy.getHealth() <= 15)
        {
            powerspike = 15;
        }
        if ((rand() % (50 / powerspike)+ 1) == 1)
        {
            rapidfireon = true;
        }
        if ((rand() % (500 / powerspike) + 1) == 42)
        {
            spreadfire = true;
        }
        if ((rand() % (5000 / powerspike)+ 1) == 19 && enemy.getHealth() <= 150)
        {
            unlimitedbulletworks = true;
        }
    }
    else
    {
        playerbullet.clear();
        enemybullet.clear();
    }

    //if (SharedData::GetInstance()->player->position_)
}

void SP2::bulletUpadtes(double dt)
{
    if (SharedData::GetInstance()->gamestate == GAME_STATE_RABBIT && enemy.isDead() == false && player.isDead() == false && (SharedData::GetInstance()->paused == false))
    {
        playerShoot(dt);
        enemyShoot(dt);
        bulletMove(dt);
    }

    //delete collided player bullets
    for (unsigned i = 0; i < playerbullet.size(); ++i)
    {
        if (playerbullet[i].p_ifCollide) 
        {    //there is collision
            playerbullet.erase(playerbullet.begin() + i);
            --i;
        }
    }
    //delete collided enemy bullets
    for (unsigned i = 0; i < enemybullet.size(); ++i)
    {
        if (enemybullet[i].e_ifCollide) 
        {    //there is collision
            enemybullet.erase(enemybullet.begin() + i);
            --i;
        }
    }
    //take damage
    if (enemy.iftakeDamage) 
    {
        enemy.takeDamage(2);
        enemy.iftakeDamage = false;
        if (enemy.isDead() == true)
        {
            enemybullet.clear();
            playerbullet.clear();
        }
    }
    if (player.iftakeDamage)
    {
        player.takeDamage();
        player.iftakeDamage = false;
        if (player.isDead() == true)
        {
            playerbullet.clear();
            enemybullet.clear();
        }
    }
    //check player bullets
    for (unsigned i = 0; i < playerbullet.size(); ++i)
    {
        checkP_BulletCollide(playerbullet[i]);
    }
    //check enemy bullets
    for (unsigned i = 0; i < enemybullet.size(); ++i)
    {
        checkE_BulletCollide(enemybullet[i]);
    }
}

void SP2::checkP_BulletCollide(PlayerBullet& bullet)
{
    if ((bullet.p_bulletPos.x > 6000 || bullet.p_bulletPos.x < 4000) || (bullet.p_bulletPos.y > 1000 || bullet.p_bulletPos.y < -1000) || (bullet.p_bulletPos.z > 6000 || bullet.p_bulletPos.z < 4000))
    {
        bullet.p_ifCollide = true;
        return;
    }
    //Body
    if (bullet.p_bulletPos.x > SharedData::GetInstance()->enemy->position_.x - 10 && bullet.p_bulletPos.x < SharedData::GetInstance()->enemy->position_.x + 10 &&
        bullet.p_bulletPos.y > 0 && bullet.p_bulletPos.y < SharedData::GetInstance()->enemy->position_.y + 31 &&
        bullet.p_bulletPos.z > SharedData::GetInstance()->enemy->position_.z - 10 && bullet.p_bulletPos.z < SharedData::GetInstance()->enemy->position_.z + 10)
    {
        enemy.takeDamage(2);
        bullet.p_ifCollide = true;
        return;
    }
    //shoulders
    if (bullet.p_bulletPos.x > SharedData::GetInstance()->enemy->position_.x - 20 && bullet.p_bulletPos.x < SharedData::GetInstance()->enemy->position_.x + 20 &&
        bullet.p_bulletPos.y > 25 && bullet.p_bulletPos.y < SharedData::GetInstance()->enemy->position_.y + 31 &&
        bullet.p_bulletPos.z > SharedData::GetInstance()->enemy->position_.z - 20 && bullet.p_bulletPos.z < SharedData::GetInstance()->enemy->position_.z + 20)
    {
        enemy.takeDamage(2);
        bullet.p_ifCollide = true;
        return;
    }
    //head
    if (bullet.p_bulletPos.x > SharedData::GetInstance()->enemy->position_.x - 6 && bullet.p_bulletPos.x < SharedData::GetInstance()->enemy->position_.x + 6 &&
        bullet.p_bulletPos.y > 31 && bullet.p_bulletPos.y < SharedData::GetInstance()->enemy->position_.y + 50 &&
        bullet.p_bulletPos.z > SharedData::GetInstance()->enemy->position_.z - 6 && bullet.p_bulletPos.z < SharedData::GetInstance()->enemy->position_.z + 6)
    {
        enemy.takeDamage(4);
        bullet.p_ifCollide = true;
        return;
    }
}

void SP2::checkE_BulletCollide(EnemyBullet& bullet)
{
    if ((bullet.e_bulletPos.x > 6000 || bullet.e_bulletPos.x < 4000) || (bullet.e_bulletPos.y > 1000 || bullet.e_bulletPos.y < -1000) || (bullet.e_bulletPos.z > 6000 || bullet.e_bulletPos.z < 4000))
    {
        bullet.e_ifCollide = true;
        return;
    }

    if (bullet.e_bulletPos.x > SharedData::GetInstance()->player->position_.x - 6 && bullet.e_bulletPos.x < SharedData::GetInstance()->player->position_.x + 6 &&
        bullet.e_bulletPos.y > 0 && bullet.e_bulletPos.y < 40 &&
        bullet.e_bulletPos.z > SharedData::GetInstance()->player->position_.z - 6 && bullet.e_bulletPos.z < SharedData::GetInstance()->player->position_.z + 6 && invulnerable >= 5)
    {
        player.takeDamage();
        bullet.e_ifCollide = true;
        invulnerable = 0;
    }
    else
    {
        if (invulnerable < 5)
            invulnerable += (float)(0.005);
    }
}
void SP2::Render()
{
    // Render VBO here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mtx44 MVP;

    Vector3 playerPos = (SharedData::GetInstance()->player->position_);

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

    //pass the position of light to shader based on light type
    //spotlights
    if (light[0].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
    }
    //if (light[1].type == Light::LIGHT_SPOT)
    //{
    //    Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
    //    glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
    //    Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
    //    glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    //}
    //if (light[2].type == Light::LIGHT_SPOT)
    //{
    //    Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
    //    glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
    //    Vector3 spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
    //    glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    //}
    //if (light[3].type == Light::LIGHT_POINT)
    //{
    //    Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
    //    glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
    //}
    //if (light[4].type == Light::LIGHT_SPOT)
    //{
    //    Position lightPosition_cameraspace = viewStack.Top() * light[4].position;
    //    glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
    //    Vector3 spotDirection_cameraspace = viewStack.Top() * light[4].spotDirection;
    //    glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    //}
    //if (light[5].type == Light::LIGHT_SPOT)
    //{
    //    Position lightPosition_cameraspace = viewStack.Top() * light[5].position;
    //    glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
    //    Vector3 spotDirection_cameraspace = viewStack.Top() * light[5].spotDirection;
    //    glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    //}
    //if (light[6].type == Light::LIGHT_SPOT)
    //{
    //    Position lightPosition_cameraspace = viewStack.Top() * light[6].position;
    //    glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightPosition_cameraspace.x);
    //    Vector3 spotDirection_cameraspace = viewStack.Top() * light[6].spotDirection;
    //    glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    //}
    //if (light[7].type == Light::LIGHT_POINT)
    //{
    //    Position lightPosition_cameraspace = viewStack.Top() * light[7].position;
    //    glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightPosition_cameraspace.x);
    //}

    std::stringstream s;
    if (SharedData::GetInstance()->gamestate != GAME_STATE_RABBIT) {
        RenderSkybox();
        RenderGround();
        RenderLayout();
        RenderSignboards();
    }

    switch (SharedData::GetInstance()->location)
    {
    case OUTSIDE: RenderHangar();
        break;
    case HANGAR: RenderHangar();
        break;
    case CORRIDOR: RenderRoom1();
        RenderRoom2();
        RenderRoom3();
        RenderRoom4();
        RenderShop();
        RenderLab();
        RenderCanteen();
        RenderControlRoom();
        break;
    case SHOP:
        RenderShop();
        break;
    case LABORATORY:
        RenderLab();
        break;
    case CONTROLROOM:
        RenderControlRoom();
        break;
    case CANTEEN:
        RenderShop();
        RenderCanteen();
        RenderPlantation();
        RenderToilet();
        break;
    case TOILET:
        RenderCanteen();
        RenderToilet();
        break;
    case ROOM1:
        RenderRoom1();
        break;
    case ROOM2:
        RenderRoom2();
        break;
    case ROOM3:
        RenderRoom3();
        break;
    case ROOM4:
        RenderRoom4();
        break;
    case PLANTATION:
        RenderCanteen();
        RenderPlantation();
        break;
    }

    //interaction
    if (SharedData::GetInstance()->canInteract && SharedData::GetInstance()->interactptr->pressButton) {
        std::stringstream ss;
        ss << "Press " << SharedData::GetInstance()->interactbutton;
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 2, 5.5);
    }
    if (SharedData::GetInstance()->sleep) {
        std::stringstream ss;
        ss << "Sleeping...";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 1), 3, 5, 7.5f);
    }

    //player crosshairs
    RenderObjectOnScreen(meshList[GEO_CROSSHAIRS], 40, 30);

    switch (SharedData::GetInstance()->gamestate)
    {
    case GAME_STATE_FREE:
        break;
    case GAME_STATE_SHOP:
        RenderInShop();
        break;
    case GAME_STATE_WSGAME:
        break;
    case GAME_STATE_CHONGAME: RenderChonGame();
        break;
    case GAME_STATE_VEEGAME:
        break;
    case GAME_STATE_JASIMGAME:
        break;
    case GAME_STATE_RABBIT: 
        loadRabbitGame();
        RenderBullets();
        RenderPlayerHealth();
        RenderBossHealth();
		//player crosshairs
		RenderObjectOnScreen(meshList[GEO_CROSSHAIRS], 40, 30);
        break;
    case GAME_STATE_DIALOGUE:
        RenderDialogueOnScreen(SharedData::GetInstance()->dialogueProcessor.npc->Speech(), Color(1, 1, 1), 3);

        //options
        switch (SharedData::GetInstance()->dialogueProcessor.convostate)
        {
        case CONVO_INTRO:
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 22, 1, 1);
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 29, 1, 1);
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 36, 1, 1);
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 43, 1, 1);

            RenderTextOnScreen(meshList[GEO_TEXT], "[Give a gift]", Color(1, 1, 1), 2, 26, 21);
            RenderTextOnScreen(meshList[GEO_TEXT], "[Compliment]", Color(1, 1, 1), 2, 26, 17.5f);
            RenderTextOnScreen(meshList[GEO_TEXT], "[Mini-game]", Color(1, 1, 1), 2, 26, 14);
            RenderTextOnScreen(meshList[GEO_TEXT], "\"Bye\"", Color(1, 1, 1), 2, 26, 10.5f);
            break;
        case CONVO_NEUTRAL:
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 22, 1, 1);
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 29, 1, 1);
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 36, 1, 1);
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 43, 1, 1);

            RenderTextOnScreen(meshList[GEO_TEXT], "[Give a gift]", Color(1, 1, 1), 2, 26, 21);
            RenderTextOnScreen(meshList[GEO_TEXT], "[Compliment]", Color(1, 1, 1), 2, 26, 17.5f);
            RenderTextOnScreen(meshList[GEO_TEXT], "[Mini-game]", Color(1, 1, 1), 2, 26, 14);
            RenderTextOnScreen(meshList[GEO_TEXT], "\"Bye\"", Color(1, 1, 1), 2, 26, 10.5f);
            break;
        case CONVO_GIFT:
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 22, 1, 1);
            RenderTextOnScreen(meshList[GEO_TEXT], "Back", Color(1, 1, 1), 2, 26, 10.5f);
            break;
        case CONVO_GIFTHAPPY:
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 22, 1, 1);
            RenderTextOnScreen(meshList[GEO_TEXT], "\"Welcome!\"", Color(1, 1, 1), 2, 26, 10.5f);
            break;
        case CONVO_GIFTUNHAPPY:
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 22, 1, 1);
            RenderTextOnScreen(meshList[GEO_TEXT], "\"Sorry...\"", Color(1, 1, 1), 2, 26, 10.5f);
            break;
        case CONVO_COMPLIMENT:
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 22, 1, 1);
            RenderTextOnScreen(meshList[GEO_TEXT], ":D", Color(1, 1, 1), 2, 26, 10.5f);
            break;
        case CONVO_STARTMINIGAME:
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 22, 1, 1);
            RenderTextOnScreen(meshList[GEO_TEXT], "\"Let's go\"", Color(1, 1, 1), 2, 26, 10.5f);
            break;
        case CONVO_FINISHMINIGAME:
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 22, 1, 1);
            RenderTextOnScreen(meshList[GEO_TEXT], "\"No problem!\"", Color(1, 1, 1), 2, 26, 10.5f);
            break;
        case CONVO_BOSSPROMPT:
            RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 65, 22, 1, 1);
            RenderTextOnScreen(meshList[GEO_TEXT], "\"I will miss you\"", Color(1, 1, 1), 2, 26, 10.5f);
            break;
        }
        RenderCursor();
        RenderLoveMeter();
        break;
    }

    RenderObjectOnScreen(meshList[GEO_LOADTOP], 40, loadDown, 1, 1, 0);
    RenderObjectOnScreen(meshList[GEO_LOADBTM], 40, loadUp, 1, 1, 0);

    RenderGates(); //gates b4 ui, aft others to hide contents of the room

    //hunger bar
    RenderObjectOnScreen(meshList[GEO_HUNGER_BAR], 23, 7, 1 + (float)(SharedData::GetInstance()->player->getHunger() / 3), 1);
    RenderTextOnScreen(meshList[GEO_TEXT], "Hunger", Color(0.5f, 1, 0.5f), 2, 11.5f, 3.7f);

    if (viewOptions) {
        RenderUI();
    }
    RenderInventory();

    //render prompt for end-game
    if (SharedData::GetInstance()->daynumber > 7) {   //a week has passed
        RenderTextOnScreen(meshList[GEO_TEXT], "The spacecraft is fixed", Color(0, 0.6f, 1), 3, 2, 11.5f);
        RenderTextOnScreen(meshList[GEO_TEXT], "Go to the hangar", Color(0, 0.6f, 1), 3, 2, 10.5f);
    }

    //render mini-game information
    if (SharedData::GetInstance()->gamestate == GAME_STATE_WSGAME)
    {
        std::stringstream s;
        s << "Points: " << SharedData::GetInstance()->pointscounter;
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 0.6f, 1), 3, 0, 13.f);
        s.str("");
        s << "Weeds left: " << SharedData::GetInstance()->weedcounter;
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0.1f), 3, 0, 12);
    }
    else if (SharedData::GetInstance()->gamestate == GAME_STATE_CHONGAME)
    {
        std::stringstream s;
        s << "Timer: " << (int)(SharedData::GetInstance()->timeElapsed);
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 0.6f, 1), 3, 0, 13);

        s.str("");
        s << "Counter: " << pickupCounter;
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 0.6f, 1), 3, 0, 12);
    }
    else if (SharedData::GetInstance()->gamestate == GAME_STATE_VEEGAME)
    {
        std::stringstream s;
        s << "No. of tries: " << SharedData::GetInstance()->switchCount;
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 0.6f, 1), 3, 0, 13);
    }
    else if (SharedData::GetInstance()->gamestate == GAME_STATE_JASIMGAME)
    {
        std::stringstream s;
        s << "Timer: " << (int)(SharedData::GetInstance()->timeElapsed);
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 0.6f, 1), 3, 0, 13);
    }

    //game is paused
    if (SharedData::GetInstance()->paused) {
        pauseGame();
        RenderCursor();
    }
}

void SP2::RenderLayout()
{
    //The Space Station
    modelStack.PushMatrix();
    modelStack.Translate(420, -1, -23);
    modelStack.Scale(50, 50, 50);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_LAYOUT], true);
    modelStack.PopMatrix();
}

void SP2::gateOpenInteractions()
{
    gateobjs[0] = 49;
    gateobjs[1] = 143;

    gateobjs[10] = -119;
    gateobjs[11] = -119;

    gateobjs[12] = 67;
    gateobjs[13] = 67;

    //Check out sp2.h for array list reference
    for (int i = 0; i <= 9; i++)
    {
        if (i >= 2 && i <= 9)
        {
            if (i % 2 == 0)
            {
                gateobjs[i] = 25;
            }
            else
            {
                gateobjs[i] = 70;
            }
        }
    }

    Interaction* gateInterInit;
    //hangar
    gateInterInit = new GateInteraction();
    gateInterInit->bound1.Set(220, -15, -45); gateInterInit->bound2.Set(360, 45, 50);
    SharedData::GetInstance()->interactionItems.push_back(gateInterInit);
    gatebounds[0].bound1 = gateInterInit->bound1; gatebounds[0].bound2 = gateInterInit->bound2;
    //lab
    gateInterInit = new GateInteraction();
    gateInterInit->bound1.Set(440, -15, -340); gateInterInit->bound2.Set(480, 45, -230);
    SharedData::GetInstance()->interactionItems.push_back(gateInterInit);
    gatebounds[1].bound1 = gateInterInit->bound1; gatebounds[1].bound2 = gateInterInit->bound2;
    //canteen
    gateInterInit = new GateInteraction();
    gateInterInit->bound1.Set(770, -15, -350); gateInterInit->bound2.Set(820, 45, -230);
    SharedData::GetInstance()->interactionItems.push_back(gateInterInit);
    gatebounds[2].bound1 = gateInterInit->bound1; gatebounds[2].bound2 = gateInterInit->bound2;
    //control 
    gateInterInit = new GateInteraction();
    gateInterInit->bound1.Set(570, -15, 270); gateInterInit->bound2.Set(617, 45, 370);
    SharedData::GetInstance()->interactionItems.push_back(gateInterInit);
    gatebounds[3].bound1 = gateInterInit->bound1; gatebounds[3].bound2 = gateInterInit->bound2;
    //rm1
    gateInterInit = new GateInteraction();
    gateInterInit->bound1.Set(545, -15, -135); gateInterInit->bound2.Set(555, 30, -100);
    SharedData::GetInstance()->interactionItems.push_back(gateInterInit);
    gatebounds[4].bound1 = gateInterInit->bound1; gatebounds[4].bound2 = gateInterInit->bound2;
    //rm3
    gateInterInit = new GateInteraction();
    gateInterInit->bound1.Set(619, -15, -135); gateInterInit->bound2.Set(630, 30, -100);
    SharedData::GetInstance()->interactionItems.push_back(gateInterInit);
    gatebounds[5].bound1 = gateInterInit->bound1; gatebounds[5].bound2 = gateInterInit->bound2;
    //rm2
    gateInterInit = new GateInteraction();
    gateInterInit->bound1.Set(545, -15, 50); gateInterInit->bound2.Set(555, 30, 85);
    SharedData::GetInstance()->interactionItems.push_back(gateInterInit);
    gatebounds[6].bound1 = gateInterInit->bound1; gatebounds[6].bound2 = gateInterInit->bound2;
    //rm4
    gateInterInit = new GateInteraction();
    gateInterInit->bound1.Set(619, -15, 50); gateInterInit->bound2.Set(630, 30, 85);
    SharedData::GetInstance()->interactionItems.push_back(gateInterInit);
    gatebounds[7].bound1 = gateInterInit->bound1; gatebounds[7].bound2 = gateInterInit->bound2;
}

void SP2::gateUpdate(double dt)
{
    if (SharedData::GetInstance()->gateopen)
    {
        for (int i = 0; i < 8; i++)
        {
            gateopenBool[i] = false;
            if (SharedData::GetInstance()->interactptr->bound1 == gatebounds[i].bound1 && SharedData::GetInstance()->interactptr->bound2 == gatebounds[i].bound2)
            {
                gateopenBool[i] = true;

                if (gateopenBool[0] == true)
                {//Hangar
                    gateobjs[0] += (float)(100 * dt);
                    gateobjs[1] += (float)(100 * dt);

                    if (gateobjs[0] >= 143)
                    {//btm, top
                        gateobjs[0] = 143;
                        gateobjs[1] = 237;
                        SharedData::GetInstance()->gateopen = false;
                    }
                    return;
                }
                if (gateopenBool[1] == true)
                {//lab
                    gateobjs[2] += (float)(100 * dt);
                    gateobjs[3] += (float)(100 * dt);

                    if (gateobjs[2] >= 70)
                    {//btm, top
                        gateobjs[2] = 70;
                        gateobjs[3] = 115;
                        SharedData::GetInstance()->gateopen = false;
                    }
                    return;
                }
                if (gateopenBool[2] == true)
                {//canteen 
                    gateobjs[4] += (float)(100 * dt);
                    gateobjs[5] += (float)(100 * dt);

                    if (gateobjs[4] >= 70)
                    {//btm, top
                        gateobjs[4] = 70;
                        gateobjs[5] = 115;
                        SharedData::GetInstance()->gateopen = false;
                    }
                    return;
                }
                if (gateopenBool[3] == true)
                {//control
                    gateobjs[6] += (float)(100 * dt);
                    gateobjs[7] += (float)(100 * dt);
                    gateobjs[8] += (float)(100 * dt);
                    gateobjs[9] += (float)(100 * dt);

                    if (gateobjs[6] >= 70)
                    {
                        gateobjs[6] = 70;
                        gateobjs[7] = 115;
                        gateobjs[8] = 70;
                        gateobjs[9] = 115;
                        SharedData::GetInstance()->gateopen = false;
                    }
                    return;
                }
                //if (gateopenBool[4] == true)
                //{//room 1
                //    gateobjs[10] += (float)(100 * dt);
                //
                //    if (gateobjs[10] >= 70)
                //        gateobjs[10] = 70;
                //    return;
                //}
                //if (gateopenBool[5] == true)
                //{//r3
                //    gateobjs[11] += (float)(100 * dt);
                //
                //    if (gateobjs[11] >= 143)
                //        gateobjs[11] = 143;
                //    return;
                //}
                //if (gateopenBool[6] == true)
                //{//r2
                //    gateobjs[12] += (float)(100 * dt);
                //
                //    if (gateobjs[12] >= 143)
                //        gateobjs[12] = 143;
                //    return;
                //}
                //if (gateopenBool[7] == true)
                //{//r4
                //    gateobjs[13] += (float)(100 * dt);
                //
                //    if (gateobjs[13] >= 143)
                //        gateobjs[13] = 143;
                //    return;
                //}
            }
            else
            {
                gateopenBool[i] = false;
                //SharedData::GetInstance()->gateopen = false;
            }
        }
        
    }
    if (SharedData::GetInstance()->gateopen == false)
    {
        /*for (int j = 0; j < 8; j++)
        {
            gateopenBool[j] = false;
        }*/

        for (int i = 0; i < 10; i++)
        {
            gateobjs[i] -= 0.5;

        }

        for (int i = 2; i <= 9; i++)
        {
            if (gateobjs[i] <= -20)     //other gates animation loop
                gateobjs[i] = 70;
        }

        //hangar gate loop
        if (gateobjs[0] <= -45)
            gateobjs[0] = 143;
        if (gateobjs[1] <= -45)
            gateobjs[1] = 143;
    }
}


void SP2::puzzleLogic()
{
    if (SharedData::GetInstance()->one == 1)
        meshList[GEO_SWITCH_1] = MeshBuilder::GenerateCube("switch1", Color(1, 0, 0), 1, 15, 4);
    if (SharedData::GetInstance()->one == 2)
        meshList[GEO_SWITCH_1] = MeshBuilder::GenerateCube("switch1", Color(0, 0, 1), 1, 15, 4);
    if (SharedData::GetInstance()->one == 3)
        meshList[GEO_SWITCH_1] = MeshBuilder::GenerateCube("switch1", Color(0, 1, 0), 1, 15, 4);
    if (SharedData::GetInstance()->one == 4)
        meshList[GEO_SWITCH_1] = MeshBuilder::GenerateCube("switch1", Color(1, 1, 0), 1, 15, 4);
    if (SharedData::GetInstance()->one == 5)
        meshList[GEO_SWITCH_1] = MeshBuilder::GenerateCube("switch1", Color(0, 1, 1), 1, 15, 4);
    if (SharedData::GetInstance()->one == 6)
        meshList[GEO_SWITCH_1] = MeshBuilder::GenerateCube("switch1", Color(1, 0, 1), 1, 15, 4);

    if (SharedData::GetInstance()->two == 1)
        meshList[GEO_SWITCH_2] = MeshBuilder::GenerateCube("switch2", Color(1, 0, 0), 1, 15, 4);
    if (SharedData::GetInstance()->two == 2)
        meshList[GEO_SWITCH_2] = MeshBuilder::GenerateCube("switch2", Color(0, 0, 1), 1, 15, 4);
    if (SharedData::GetInstance()->two == 3)
        meshList[GEO_SWITCH_2] = MeshBuilder::GenerateCube("switch2", Color(0, 1, 0), 1, 15, 4);
    if (SharedData::GetInstance()->two == 4)
        meshList[GEO_SWITCH_2] = MeshBuilder::GenerateCube("switch2", Color(1, 1, 0), 1, 15, 4);
    if (SharedData::GetInstance()->two == 5)
        meshList[GEO_SWITCH_2] = MeshBuilder::GenerateCube("switch2", Color(0, 1, 1), 1, 15, 4);
    if (SharedData::GetInstance()->two == 6)
        meshList[GEO_SWITCH_2] = MeshBuilder::GenerateCube("switch2", Color(1, 0, 1), 1, 15, 4);

    if (SharedData::GetInstance()->three == 1)
        meshList[GEO_SWITCH_3] = MeshBuilder::GenerateCube("switch3", Color(1, 0, 0), 1, 15, 4);
    if (SharedData::GetInstance()->three == 2)
        meshList[GEO_SWITCH_3] = MeshBuilder::GenerateCube("switch3", Color(0, 0, 1), 1, 15, 4);
    if (SharedData::GetInstance()->three == 3)
        meshList[GEO_SWITCH_3] = MeshBuilder::GenerateCube("switch3", Color(0, 1, 0), 1, 15, 4);
    if (SharedData::GetInstance()->three == 4)
        meshList[GEO_SWITCH_3] = MeshBuilder::GenerateCube("switch3", Color(1, 1, 0), 1, 15, 4);
    if (SharedData::GetInstance()->three == 5)
        meshList[GEO_SWITCH_3] = MeshBuilder::GenerateCube("switch3", Color(0, 1, 1), 1, 15, 4);
    if (SharedData::GetInstance()->three == 6)
        meshList[GEO_SWITCH_3] = MeshBuilder::GenerateCube("switch3", Color(1, 0, 1), 1, 15, 4);

    if (SharedData::GetInstance()->four == 1)
        meshList[GEO_SWITCH_4] = MeshBuilder::GenerateCube("switch4", Color(1, 0, 0), 1, 15, 4);
    if (SharedData::GetInstance()->four == 2)
        meshList[GEO_SWITCH_4] = MeshBuilder::GenerateCube("switch4", Color(0, 0, 1), 1, 15, 4);
    if (SharedData::GetInstance()->four == 3)
        meshList[GEO_SWITCH_4] = MeshBuilder::GenerateCube("switch4", Color(0, 1, 0), 1, 15, 4);
    if (SharedData::GetInstance()->four == 4)
        meshList[GEO_SWITCH_4] = MeshBuilder::GenerateCube("switch4", Color(1, 1, 0), 1, 15, 4);
    if (SharedData::GetInstance()->four == 5)
        meshList[GEO_SWITCH_4] = MeshBuilder::GenerateCube("switch4", Color(0, 1, 1), 1, 15, 4);
    if (SharedData::GetInstance()->four == 6)
        meshList[GEO_SWITCH_4] = MeshBuilder::GenerateCube("switch4", Color(1, 0, 1), 1, 15, 4);

    //Lights
    if (SharedData::GetInstance()->switch1 == true)
        meshList[GEO_CHECK_1] = MeshBuilder::GenerateHemisphere("check1", Color(1, 1, 1), 2);
    if (SharedData::GetInstance()->switch2 == true)
        meshList[GEO_CHECK_2] = MeshBuilder::GenerateHemisphere("check2", Color(1, 1, 1), 2);
    if (SharedData::GetInstance()->switch3 == true)
        meshList[GEO_CHECK_3] = MeshBuilder::GenerateHemisphere("check3", Color(1, 1, 1), 2);
    if (SharedData::GetInstance()->switch4 == true)
        meshList[GEO_CHECK_4] = MeshBuilder::GenerateHemisphere("check4", Color(1, 1, 1), 2);

    //win the mini-game
    if (lightpuzz.checkPuzzleAns((int)(SharedData::GetInstance()->one), (int)(SharedData::GetInstance()->two), (int)(SharedData::GetInstance()->three), (int)(SharedData::GetInstance()->four)) == true)
    {
        SharedData::GetInstance()->one = SharedData::GetInstance()->two = SharedData::GetInstance()->three = SharedData::GetInstance()->four = 1;
        lightpuzz.generatePuzzle();
        SharedData::GetInstance()->gamestate = GAME_STATE_DIALOGUE;
        SharedData::GetInstance()->dialogueProcessor.convostate = CONVO_FINISHMINIGAME;
        //increase love meter and gain gold based on number of tries
        int gained = 10 - SharedData::GetInstance()->switchCount;
        if (gained < 0) {   //cannot gain negative
            gained = 0;
        }
        SharedData::GetInstance()->player->changeGold(10 * gained);
        SharedData::GetInstance()->dialogueProcessor.npc->setLoveMeter(SharedData::GetInstance()->dialogueProcessor.npc->getLoveMeter() + gained);
    }
}

void SP2::compactBallrender()
{
    modelStack.PushMatrix();
    modelStack.Translate(ballyellX, 15, ballyellZ);
    modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_SPHEREYELLOW], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(410, ballblacky, -396);    //425
    modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_SPHEREBLACK], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(425, ballwhitey, -380);    //
    modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_SPHEREWHITE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(ballbluX, 15, ballbluZ);
    modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_SPHEREBLUE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(ballredX, 15, ballredZ);
    modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_SPHERERED], true);
    modelStack.PopMatrix();
}

void SP2::compactMovement(bool first, bool second, bool third, int i)
{
    if (first)
    {
        //yellow move to blue
        if (ballyellX >= 339)
        {
            ballyellX -= 1;
            if (ballyellZ <= -465)
            {
                ballyellZ += 1;
            }
        }


        //blue move to red
        if (ballbluX >= 310)
        {
            ballbluX -= 0.3f;
            if (ballbluZ <= -364)
            {
                ballbluZ += 1;
            }
        }

        //red moves to yellow
        if (ballredX <= 483)
        {
            ballredX += 1;
            if (ballredZ >= -469)
            {
                ballredZ -= 1;
            }
        }

        if (ballyellZ == -465 && ballbluZ == -364 && ballredZ == -469) {
            pickupCounter += 1;
        }
    }

    else if (second)
    {
        //yellow2 move to blue2(red pos)
        if (ballyellX >= 310)
        {
            ballyellX -= 0.3f;
            if (ballyellZ <= -364)
            {
                ballyellZ += 1;
            }
        }


        //blue2 move to red2(yellow pos)
        if (ballbluX <= 483)
        {
            ballbluX += 1;
            if (ballbluZ >= -469)
            {
                ballbluZ -= 0.7f;
            }
        }

        //red moves to yellow(blue pos)
        if (ballredX >= 339)
        {
            ballredX -= 1;
            if (ballredZ <= -465)
            {
                ballredZ += 1;
            }
        }
    }

    if (third)
    {
        //yellow2 move to oriYellow(yell pos)
        if (ballyellX <= 483)
        {
            ballyellX += 1;
            if (ballyellZ >= -469)
            {
                ballyellZ -= 0.7f;
            }
        }
        else 
            SharedData::GetInstance()->ballpickup = false;

        //blue2 move to oriBlue(blue pos)
         if (ballbluX >= 339)
        {
            ballbluX -= 1;
            if (ballbluZ <= -465)
            {
                ballbluZ += 1;
            }
        }

        //red moves to oriRed(red pos)
        if (ballredX >= 310)
        {
            ballredX -= 0.3f;
            if (ballredZ <= -364)
            {
                ballredZ += 1;
            }
        }
        

    }

    else
        return;

}

void SP2::ballmoveCheck()
{
    if (ball[0] == true)
    {
        //yellow first shift
        if (SharedData::GetInstance()->firstball == 1 || SharedData::GetInstance()->firstball == 2)
        {
            compactMovement(true, false, false, 0);
        }
        //blue third shift
        if (SharedData::GetInstance()->firstball == 3 || SharedData::GetInstance()->firstball == 4)
        {
            compactMovement(false, false, true, 0);
        }
        //red second shift
        if (SharedData::GetInstance()->firstball == 5 || SharedData::GetInstance()->firstball == 6)
        {
            compactMovement(false, true, false, 0);
        }
    }
    else if (ball[1] == true)
    {
        ballblacky += 1;
        if (ballblacky >= 20)
        {
            ballblacky = 15;
            ball[1] = false;
            SharedData::GetInstance()->ballpickup = false;


            if (postercounter == 0)
                postercounter = 1;
            else if (postercounter == 2)
                postercounter = 3;
            else if (postercounter == 4)
                postercounter = 5;

            return;
        }
    }
    else if (ball[2] == true)
    {
        ballwhitey += 1;
        if (ballwhitey >= 20)
        {
            ballwhitey = 15;
            ball[2] = false;
            SharedData::GetInstance()->ballpickup = false;


            if (postercounter == 1)
                postercounter = 2;
            else if (postercounter == 3)
                postercounter = 4;
            else if (postercounter == 5)
                postercounter = 6;

            return;
        }
    }
    else if (ball[3] == true)
    {
        //yellow second shift
        if (SharedData::GetInstance()->firstball == 1 || SharedData::GetInstance()->firstball == 2)
        {
            compactMovement(false, true, false, 3);
        }
        //blue first shift
        if (SharedData::GetInstance()->firstball == 3 || SharedData::GetInstance()->firstball == 4)
        {
            compactMovement(true, false, false, 3);
        }
        //red last shift
        if (SharedData::GetInstance()->firstball == 5 || SharedData::GetInstance()->firstball == 6)
        {
            compactMovement(false, false, true, 3);
        }
    }
    else if (ball[4] == true)
    {
        //yellow third shift
        if (SharedData::GetInstance()->firstball == 1 || SharedData::GetInstance()->firstball == 2)
        {
            compactMovement(false, false, true, 4);
        }
        //blue second shift
        if (SharedData::GetInstance()->firstball == 3 || SharedData::GetInstance()->firstball == 4)
        {
            compactMovement(false, true, false, 4);
        }
        //red first shift
        if (SharedData::GetInstance()->firstball == 5 || SharedData::GetInstance()->firstball == 6)
        {
            compactMovement(true, false, false, 4);
        }
    }


    
}

void SP2::ballboundfunct()
{
    Interaction* ballinteraction;
    //check sp2.h for arraylist of colors
    ballinteraction = new chonBallInteraction();   //ball 0 (1)
    ballinteraction->bound1.Set(480, 15, -469); ballinteraction->bound2.Set(485, 20, -460);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[0].bound1 = ballinteraction->bound1;     ballbounds[0].bound2 = ballinteraction->bound2;

    ballinteraction = new chonBallInteraction();   //ball 1 (2)
    ballinteraction->bound1.Set(405, 15, -400); ballinteraction->bound2.Set(410, 20, -390);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[1].bound1 = ballinteraction->bound1;    ballbounds[1].bound2 = ballinteraction->bound2;

    ballinteraction = new chonBallInteraction();   //ball 2 (3)
    ballinteraction->bound1.Set(420, 15, -385); ballinteraction->bound2.Set(427, 20, -375);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[2].bound1 = ballinteraction->bound1;    ballbounds[2].bound2 = ballinteraction->bound2;

    ballinteraction = new chonBallInteraction();   //ball 3 (4)
    ballinteraction->bound1.Set(334, 15, -469); ballinteraction->bound2.Set(343, 20, -460);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[3].bound1 = ballinteraction->bound1;    ballbounds[3].bound2 = ballinteraction->bound2;

    ballinteraction = new chonBallInteraction();   //ball 4 (5)
    ballinteraction->bound1.Set(300, 15, -367); ballinteraction->bound2.Set(309, 20, -360);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[4].bound1 = ballinteraction->bound1;    ballbounds[4].bound2 = ballinteraction->bound2;

}

void SP2::pauseGame()
{
    RenderObjectOnScreen(meshList[GEO_SHOPUI], 40, 30, 50, 50);
    RenderTextOnScreen(meshList[GEO_TEXT], "PAUSED", Color(0, 0.6f, 1), 5, 5.5f, 9);
    RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 40, pause.verticalDisp1);
    RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 40, pause.verticalDisp2);
    RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 40, pause.verticalDisp3);

    RenderTextOnScreen(meshList[GEO_TEXT], "RESUME GAME", Color(0, 0.6f, 1), 2, 15.5f, 18.5f);
    RenderTextOnScreen(meshList[GEO_TEXT], "EXIT TO MAIN MENU", Color(0, 0.6f, 1), 2, 13.5f, 13.5f);
    RenderTextOnScreen(meshList[GEO_TEXT], "EXIT GAME", Color(0, 0.6f, 1), 2, 16.5f, 8.5f);
}

void SP2::loadRabbitGame()
{
    SharedData::GetInstance()->player->setHunger(0);
    SharedData::GetInstance()->timeElapsed = 0;

    if (light[0].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
    }

    RenderFightSkybox();

    if (SharedData::GetInstance()->gamestate == GAME_STATE_RABBIT && enemy.isDead() == false)
    { 
        modelStack.PushMatrix();
        modelStack.Translate(SharedData::GetInstance()->enemy->position_.x, 0, SharedData::GetInstance()->enemy->position_.z);
        modelStack.Rotate(enemy.yaw, 0, 1, 0);
        modelStack.Scale(10, 10, 10);
        RenderMesh(meshList[GEO_ADOLPH], true);
        modelStack.PopMatrix();
    }
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

void SP2::RenderObjectOnScreen(Mesh* mesh, float x, float y, float scalex, float scaley, float rotate)
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
    modelStack.Rotate(rotate, 0, 0, 1);
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

    float xTranslation = 0.5f, yTranslation = 0.5f;
    for (unsigned i = 0; i < text.length(); ++i) {
        Mtx44 characterSpacing;
        if (i > 0) {
            xTranslation += 0.8f;
            if (xTranslation >= 22.9f) {
                yTranslation -= 1.f;
                xTranslation -= 22.9f;
            }
        }
        characterSpacing.SetToTranslation(xTranslation, yTranslation, 0);  //0.8f is the spacing of each character (can be changed)
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

    //name
    if (SharedData::GetInstance()->gamestate == GAME_STATE_SHOP) {
        RenderTextOnScreen(meshList[GEO_TEXT], "StEMMIE", color, size, 1.5f, 5.3f);
    }
    else {
        RenderTextOnScreen(meshList[GEO_TEXT], SharedData::GetInstance()->dialogueProcessor.npc->getName(), color, size, 1.5f, 5.3f);
    }
    //message
    RenderTextOnScreen(meshList[GEO_TEXT], text, color, size, 1.5f, 3.7f);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderSkybox()
{
    modelStack.PushMatrix();

    //follow player

    modelStack.Translate(SharedData::GetInstance()->player->position_.x, -600, SharedData::GetInstance()->player->position_.z);

    //left
    modelStack.PushMatrix();
    modelStack.Translate(-745, 745, 0);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_SKYBOX_LEFT], false);
    modelStack.PopMatrix();

    //right
    modelStack.PushMatrix();
    modelStack.Translate(745, 745, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_SKYBOX_RIGHT], false);
    modelStack.PopMatrix();

    //back
    modelStack.PushMatrix();
    modelStack.Translate(0, 745, 745);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_SKYBOX_BACK], false);
    modelStack.PopMatrix();

    //front
    modelStack.PushMatrix();
    modelStack.Translate(0, 745, -745);
    RenderMesh(meshList[GEO_SKYBOX_FRONT], false);
    modelStack.PopMatrix();

    //top
    modelStack.PushMatrix();
    modelStack.Translate(0, 1490, 0);
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

void SP2::RenderSignboards()
{
    modelStack.PushMatrix();
    modelStack.Translate(460, 45, -243);
    modelStack.Scale(30, 30, 30);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_SIGNLAB], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(797, 45, -243);
    modelStack.Scale(30, 30, 30);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_SIGNCANTEEN], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(582, 45, 278);
    modelStack.Scale(40, 30, 30);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_SIGNCTRL], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(686, 40, -525);
    modelStack.Scale(20, 30, 30);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_SIGNTOILET], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(825, 40, -441);
    modelStack.Scale(30, 30, 25);
    RenderMesh(meshList[GEO_SIGNFARM], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(835, 55, 5);
    modelStack.Scale(30, 30, 40);
    RenderMesh(meshList[GEO_SIGNSHOP], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(420, 25, -30);
    modelStack.Scale(30, 30, 30);
    RenderMesh(meshList[GEO_SIGNLEFT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(420, 25, 30);
    modelStack.Scale(30, 30, 30);
    RenderMesh(meshList[GEO_SIGNRIGHT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(833, 58, -323);
    modelStack.Scale(100, 100, 100);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_SIGNCOVER], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(552, 37, 67);
    modelStack.Scale(30, 25, 20);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_SIGNROOM3], false);
    modelStack.PopMatrix();
}



void SP2::chonSecret()
{
    modelStack.PushMatrix();
    modelStack.Translate(490, 20, -475);
    modelStack.Scale(15, 15, 5);
    RenderMesh(meshList[GEO_POSTSAO], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(470, 20, -475);
    modelStack.Scale(15, 15, 5);
    RenderMesh(meshList[GEO_POSTPUNCH], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(440, 20, -475);
    modelStack.Scale(15, 15, 5);
    RenderMesh(meshList[GEO_POSTKYOJ], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(360, 20, -475);
    modelStack.Scale(15, 15, 5);
    RenderMesh(meshList[GEO_POSTFATE], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(330, 20, -475);
    modelStack.Scale(15, 15, 5);
    RenderMesh(meshList[GEO_POSTDGRAY], false);
    modelStack.PopMatrix();
    

    //back
    modelStack.PushMatrix();
    modelStack.Translate(410, 20, -333);
    modelStack.Scale(15, 15, 5);
    RenderMesh(meshList[GEO_POSTRETT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(380, 20, -333);
    modelStack.Scale(15, 15, 5);
    RenderMesh(meshList[GEO_POSTSAO], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(350, 20, -333);
    modelStack.Scale(15, 15, 5);
    RenderMesh(meshList[GEO_POSTPUNCH], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(320, 20, -333);
    modelStack.Scale(15, 15, 5);
    RenderMesh(meshList[GEO_ANIPOSTER], false);
    modelStack.PopMatrix();



    //left
    modelStack.PushMatrix();
    modelStack.Translate(304, 20, -365);
    modelStack.Scale(15, 15, 15);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_POSTKYOJ], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(304, 20, -395);
    modelStack.Scale(15, 15, 15);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_POSTFATE], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(304, 20, -425);
    modelStack.Scale(15, 15, 15);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_POSTDGRAY], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(304, 20, -455);
    modelStack.Scale(15, 15, 15);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_POSTRETT], false);
    modelStack.PopMatrix();


    //right
    modelStack.PushMatrix();
    modelStack.Translate(511, 20, -365);
    modelStack.Scale(15, 15, 15);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_POSTSAO], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(511, 20, -395);
    modelStack.Scale(15, 15, 15);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_POSTPUNCH], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(511, 20, -425);
    modelStack.Scale(15, 15, 15);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_POSTKYOJ], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(511, 20, -455);
    modelStack.Scale(15, 15, 15);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_POSTFATE], false);
    modelStack.PopMatrix();

}


void SP2::RenderInventoryOnScreenStatic(Mesh* mesh, float x, float y)
{
	if (!mesh)  //error check
		return;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
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
	modelStack.Rotate(-75, 1, 0, 1);
	modelStack.Scale(1.2f, 1.2f, 1.2f);
	RenderMesh(mesh, false);
	modelStack.PopMatrix();

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void SP2::RenderInventoryOnScreenRotating(Mesh* mesh, float x, float y)
{
	if (!mesh)  //error check
		return;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
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
	modelStack.Scale(1.2f, 1.2f, 1.2f);
	RenderMesh(mesh, false);
	modelStack.PopMatrix();

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void SP2::RenderTimeOnScreen(Mesh* mesh, float x, float y)
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

	float timerotator = 0;
	timerotator += ((int)SharedData::GetInstance()->daynighttime / 100) * 60;
	timerotator += ((int)SharedData::GetInstance()->daynighttime % 100);
	timerotator /= 1440;
	timerotator *= 360;

	modelStack.PushMatrix();
	modelStack.Translate(x, y, 0);
	modelStack.Rotate(-timerotator, 0, 0, 1);
	modelStack.Rotate(-75, 0, 0, 1);
	//modelStack.Scale(1.3, 1.3, 1.3);
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
		//if (SharedData::GetInstance()->player->inventory[i] == 1)
		//{
			//gift1
			if (SharedData::GetInstance()->player->invselect != i)
			{
				RenderInventoryOnScreenStatic(meshList[modelmap.find(SharedData::GetInstance()->player->inventory[i])->second], 22.5f + (i * 5), 2.5f);
			}
			else
			{
				//RenderInventoryOnScreenRotating(meshList[GEO_HAMMER], 22.5 + (i * 5), 2.5);
				RenderInventoryOnScreenRotating(meshList[modelmap.find(SharedData::GetInstance()->player->inventory[i])->second], 22.5f + (i * 5), 2.5f);
			}
			//RenderTextOnScreen(meshList[GEO_TEXT], (invmap.find(1)->second).getName(), Color(1, 1, 0), 3, 10, 2);
			continue;
		//}
		//if (SharedData::GetInstance()->player->inventory[i] == 2)
		//{
		//	//gift2
		//	if (SharedData::GetInstance()->player->invselect != i)
		//	{
		//		RenderInventoryOnScreenStatic(meshList[GEO_STEMMIE], 22.5 + (i * 5), 2.5);
		//	}
		//	else
		//	{
		//		RenderInventoryOnScreenRotating(meshList[GEO_STEMMIE], 22.5 + (i * 5), 2.5);
		//	}
		//	continue;
		//}
	}

	RenderObjectOnScreen(meshList[GEO_ITEMSELECT], 22.5f + (SharedData::GetInstance()->player->invselect * 5), 2.5f);
    if (SharedData::GetInstance()->player->inventory[SharedData::GetInstance()->player->invselect] != 0) 
	{
        float namelength = (float)(invmap.find(SharedData::GetInstance()->player->inventory[SharedData::GetInstance()->player->invselect])->second).getName().size();
		RenderTextOnScreen(meshList[GEO_TEXT], (invmap.find(SharedData::GetInstance()->player->inventory[SharedData::GetInstance()->player->invselect])->second).getName(), Color(1, 1, 0), 3, 14.5f - namelength / 2.f, 3);
    }
	if (SharedData::GetInstance()->player->invfulldisplay == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Inventory Full", Color(1, 0, 0), 2, 29, 23);
	}
    RenderObjectOnScreen(meshList[GEO_INVENTORY], 40, 2.5);
}

void SP2::RenderTime()
{
	std::stringstream timey;
	timey << "TIME: " << ((int)SharedData::GetInstance()->daynighttime / 1000) << (((int)SharedData::GetInstance()->daynighttime / 100) % 10) << (((int)SharedData::GetInstance()->daynighttime / 10) % 10) << ((int)SharedData::GetInstance()->daynighttime % 10);
	timey.str();
	RenderTextOnScreen(meshList[GEO_TEXT], timey.str(), Color(1, 0, 0), 3, 0, 16);

	timey.str("");
    timey << "DAY " << SharedData::GetInstance()->daynumber;
	RenderTextOnScreen(meshList[GEO_TEXT], timey.str(), Color(0, 1, 0), 3, 0, 16);

	RenderTimeOnScreen(meshList[GEO_DAYNIGHTICON], 80, 60);
}

void SP2::RenderUI()
{
    std::stringstream s;

    //FPS
    s << "FPS:" << FramePerSecond;
    RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 3, 0, 19);

    //player position coordinates
    s.str("");
    s << "COORD:(" << (int)(SharedData::GetInstance()->player->position_.x) << "," << (int)(SharedData::GetInstance()->player->position_.y) << "," << (int)(SharedData::GetInstance()->player->position_.z) << ")";
    RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 3, 0, 18);

    //player's location
    s.str("");
    s << "LOCATION:" << locations[SharedData::GetInstance()->location];
    RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 3, 0, 17);

    //time
    std::stringstream timey;
    timey << "TIME: " << ((int)SharedData::GetInstance()->daynighttime / 1000) << (((int)SharedData::GetInstance()->daynighttime / 100) % 10) << (((int)SharedData::GetInstance()->daynighttime / 10) % 10) << ((int)SharedData::GetInstance()->daynighttime % 10);
    timey.str();
    RenderTextOnScreen(meshList[GEO_TEXT], timey.str(), Color(0, 0, 1), 3, 0, 16);
    RenderTimeOnScreen(meshList[GEO_DAYNIGHTICON], 80, 60);

    //moneh
    s.str("");
    s << "Gold: " << SharedData::GetInstance()->player->getGold();
    RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0.7f, 0.7f, 0), 3, 0, 15);
    
    //state
    s.str("");
    s << "State: " << SharedData::GetInstance()->gamestate;
    RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0.9f, 0.9f, 0), 3, 0, 14);

	//time
	s.str("");
    s << "DAY " << SharedData::GetInstance()->daynumber;
	s.str();
	RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 0, 1), 3, 20, 19);
}

void SP2::RenderGates()
{
    //hangar
    modelStack.PushMatrix();
    modelStack.Translate(290, gateobjs[0], 3);
    modelStack.Scale(3, 95, 95);
    RenderMesh(meshList[GEO_GATE1], false);
    modelStack.PopMatrix();
        modelStack.PushMatrix();
        modelStack.Translate(290, gateobjs[1], 3);
        modelStack.Scale(3, 95, 95);
        RenderMesh(meshList[GEO_GATE2], false);
        modelStack.PopMatrix();

    //lab
    modelStack.PushMatrix();
    modelStack.Translate(462, gateobjs[2], -287);
    modelStack.Scale(45, 45, 3);
    RenderMesh(meshList[GEO_GATE1], false);
    modelStack.PopMatrix();
        modelStack.PushMatrix();
        modelStack.Translate(462, gateobjs[3], -287);
        modelStack.Scale(45, 45, 3);  
        RenderMesh(meshList[GEO_GATE2], false);
        modelStack.PopMatrix();

        //canteen
    modelStack.PushMatrix();
    modelStack.Translate(797, gateobjs[4], -291);
    modelStack.Scale(53, 45, 3);
    RenderMesh(meshList[GEO_GATE1], false);
    modelStack.PopMatrix();
        modelStack.PushMatrix();
        modelStack.Translate(797, gateobjs[5], -291);
        modelStack.Scale(53, 45, 3);  
        RenderMesh(meshList[GEO_GATE2], false);
        modelStack.PopMatrix();

        //control room
    modelStack.PushMatrix();
    modelStack.Translate(601, gateobjs[6], 319);
    modelStack.Scale(45, 45, 3);
    RenderMesh(meshList[GEO_GATE1], false);
    modelStack.PopMatrix();
        modelStack.PushMatrix();
        modelStack.Translate(601, gateobjs[7], 319);
        modelStack.Scale(45, 45, 3);  
        RenderMesh(meshList[GEO_GATE2], false);
        modelStack.PopMatrix();
    modelStack.PushMatrix();
    modelStack.Translate(555, gateobjs[8], 319);
    modelStack.Scale(45, 45, 3);
    RenderMesh(meshList[GEO_GATE1], false);
    modelStack.PopMatrix();
        modelStack.PushMatrix();
        modelStack.Translate(555, gateobjs[9], 319);
        modelStack.Scale(45, 45, 3);  
        RenderMesh(meshList[GEO_GATE2], false);
        modelStack.PopMatrix();

        //Rooms
    modelStack.PushMatrix();
    modelStack.Translate(550, 18, -119);
    modelStack.Scale(1, 34, 34);
    RenderMesh(meshList[GEO_GATE1], false);        //room1  (away from ctrl, closest to lab)
    modelStack.PopMatrix();
        modelStack.PushMatrix();
        modelStack.Translate(626, 18, -119);
        modelStack.Scale(1, 34, 34);
        RenderMesh(meshList[GEO_GATE2], false);     //room3 (across rm1)
        modelStack.PopMatrix();                     //       4 - 2
    //modelStack.PushMatrix();                        //         /
    //modelStack.Translate(550, 18, 67);    //       3 - 1
    //modelStack.Scale(1, 34, 34);
    //RenderMesh(meshList[GEO_GATE1], false);         //room2 (same lane as rm1, closer to ctrl)
    //modelStack.PopMatrix();
        modelStack.PushMatrix();
        modelStack.Translate(626, 18, 67);
        modelStack.Scale(1, 34, 34);
        RenderMesh(meshList[GEO_GATE2], false);     //room4 (same lane as rm3, across rm2)
        modelStack.PopMatrix();
}


void SP2::RotateDisplay()
{
    modelStack.PushMatrix();
        modelStack.Rotate(20,1,0,0);
        glDisable(GL_DEPTH_TEST);
        Mtx44 ortho;
        ortho.SetToOrtho(0, 80, 0, 60, -100, 100);    //size of screen UI
        projectionStack.PushMatrix();
        projectionStack.LoadMatrix(ortho);
        viewStack.PushMatrix();
        viewStack.LoadIdentity();   //no need camera for ortho mode
	    modelStack.PushMatrix();
            modelStack.LoadIdentity();  //reset modelStack
            modelStack.PushMatrix();
                modelStack.Translate(40, 30, 20);
                modelStack.Scale(7, 7, 7);
                modelStack.Rotate(20, 1, 0, 0);
                modelStack.Rotate(rotating, 0, 1, 0);
                RenderMesh(meshList[modelmap.find(*shop.shopIterator)->second], false);
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

void SP2::CheckCharacterLocation()
{
    Vector3 playerPos = SharedData::GetInstance()->player->position_;

    if (playerPos.x >= 25 && playerPos.x <= 252 && playerPos.z >= -200 && playerPos.z <= 191) {
        SharedData::GetInstance()->location = HANGAR;
    }
    else if (playerPos.x >= 425 && playerPos.x <= 552 && playerPos.z >= -158 && playerPos.z <= 22) {
        SharedData::GetInstance()->location = ROOM1;
    }
    else if (playerPos.x >= 425 && playerPos.x <= 552 && playerPos.z >= 25 && playerPos.z <= 202) {
        SharedData::GetInstance()->location = ROOM2;
    }
    else if (playerPos.x >= 625 && playerPos.x <= 750 && playerPos.z >= -158 && playerPos.z <= 22) {
        SharedData::GetInstance()->location = ROOM3;
    }
    else if (playerPos.x >= 625 && playerPos.x <= 750 && playerPos.z >= 25 && playerPos.z <= 202) {
        SharedData::GetInstance()->location = ROOM4;
    }
    else if (playerPos.x >= 835 && playerPos.x <= 940 && playerPos.z >= -80 && playerPos.z <= 85) {
        SharedData::GetInstance()->location = SHOP;
    }
    else if (playerPos.x >= 306 && playerPos.x <= 510 && playerPos.z >= -470 && playerPos.z <= -334) {
        SharedData::GetInstance()->location = LABORATORY;
    }
    else if (playerPos.x >= 473 && playerPos.x <= 682 && playerPos.z >= 384 && playerPos.z <= 498) {
        SharedData::GetInstance()->location = CONTROLROOM;
    }
    else if ((playerPos.x >= 670 && playerPos.x <= 822 && playerPos.z >= -525 && playerPos.z <= -345) ||
        (playerPos.x >= 675 && playerPos.x <= 695 && playerPos.z >= -555 && playerPos.z <= -525)) {
        SharedData::GetInstance()->location = CANTEEN;
    }
    else if (playerPos.x >= 820 && playerPos.x <= 1045 && playerPos.z >= -530 && playerPos.z <= -270) {
        SharedData::GetInstance()->location = PLANTATION;
    }
    else if (playerPos.x >= 672 && playerPos.x <= 790 && playerPos.z >= -628 && playerPos.z <= -555) {
        SharedData::GetInstance()->location = TOILET;
    }

    else if (playerPos.x >= 252 && playerPos.x <= 352 && playerPos.z >= -40 && playerPos.z <= 45) {
        SharedData::GetInstance()->location = CORRIDOR;     //corridor to hangar
    }
    else if (playerPos.x >= 442 && playerPos.x <= 478 && playerPos.z >= -335 && playerPos.z <= -245) {
        SharedData::GetInstance()->location = CORRIDOR;     //corridor to laboratory
    }
    else if (playerPos.x >= 538 && playerPos.x <= 618 && playerPos.z >= 275 && playerPos.z <= 382) {
        SharedData::GetInstance()->location = CORRIDOR;     //corridor to control room
    }
    else if (playerPos.x >= 770 && playerPos.x <= 820 && playerPos.z >= -345 && playerPos.z <= -245) {
        SharedData::GetInstance()->location = CORRIDOR;     //corridor to canteen
    }
    else if (playerPos.x >= 352 && playerPos.x <= 835 && playerPos.z >= -245 && playerPos.z <= 275) {
        SharedData::GetInstance()->location = CORRIDOR;     //square corridor
    }
    else {
        SharedData::GetInstance()->location = OUTSIDE;
    }
}

void SP2::RenderCursor()
{
    RenderObjectOnScreen(meshList[GEO_CURSOR], (float)(SharedData::GetInstance()->cursor_newxpos) / (float)(SharedData::GetInstance()->width / 80), (float)(60 - SharedData::GetInstance()->cursor_newypos / (SharedData::GetInstance()->height / 60)), 1, 1);
}
void SP2::RenderFightSkybox()
{
    modelStack.PushMatrix();

    //follow player

    modelStack.Translate(SharedData::GetInstance()->player->position_.x, -600, SharedData::GetInstance()->player->position_.z);

    //left
    modelStack.PushMatrix();
    modelStack.Translate(-745, 745, 0);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_RABBIT_SKYBOX], false);
    modelStack.PopMatrix();

    //right
    modelStack.PushMatrix();
    modelStack.Translate(745, 745, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_RABBIT_SKYBOX], false);
    modelStack.PopMatrix();

    //back
    modelStack.PushMatrix();
    modelStack.Translate(0, 745, 745);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_RABBIT_SKYBOX], false);
    modelStack.PopMatrix();

    //front
    modelStack.PushMatrix();
    modelStack.Translate(0, 745, -745);
    RenderMesh(meshList[GEO_RABBIT_SKYBOX], false);
    modelStack.PopMatrix();

    //top
    modelStack.PushMatrix();
    modelStack.Translate(0, 1490, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Rotate(90, 1, 0, 0);
    RenderMesh(meshList[GEO_RABBIT_SKYBOX], false);
    modelStack.PopMatrix();

    //bottom
    modelStack.PushMatrix();
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_RABBIT_SKYBOX], false);
    modelStack.PopMatrix();
    
    modelStack.PopMatrix();
}

void SP2::playerShoot(double dt)
{
    Vector3 view = SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position;
    float scalar = view.Dot(Vector3(0, 1, 0));
    pitch = 110.f - Math::RadianToDegree(acos(scalar));
    pitch *= -1;

    float scalar2 = view.Dot(Vector3(0, 0, 1));
    yaw = Math::RadianToDegree(acos(scalar2));

    if (view.x <= 0) 
    {
        yaw = 360.f - yaw;
    }

    elapsedTime += dt;
    if (Application::IsKeyPressed(VK_LBUTTON) && elapsedTime >= 0.5) 
    {
        PlayerBullet bullet;
        bullet.p_bulletPos = SharedData::GetInstance()->player->position_;
        bullet.p_bulletPos.y = SharedData::GetInstance()->player->position_.y - 5;
        bullet.p_bulletDir = (BULLETSPEED + 150) * (float)(dt)* (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        bullet.p_ifCollide = false;

        bullet.p_pitch = pitch;
        bullet.p_yaw = yaw;

        playerbullet.push_back(bullet);
        elapsedTime = 0.f;  //reset
    }
}

void SP2::enemyShoot(double dt)
{
    enemy.position_ = Vector3(5000, 25, 5000);
    Vector3 enemyView = SharedData::GetInstance()->camera->target - enemy.position_;
    enemyView.y = 25;
    enemyView = enemyView.Normalized();
    float scalar3 = enemyView.Dot(Vector3(0, 0, 1));
    enemy.yaw = Math::RadianToDegree(acos(scalar3));

    if (enemyView.x <= 0) 
    {
        enemy.yaw = 360.f - enemy.yaw;
    }

    e_elapsedTime += dt;
    if (e_elapsedTime >= 0.5 && rapidfireon == false && spreadfire == false && unlimitedbulletworks == false)
    {
        EnemyBullet e_bullet;
        e_bullet.e_bulletPos = SharedData::GetInstance()->enemy->position_;
        e_bullet.e_bulletDir = (BULLETSPEED + 2 * powerspike)* (float)(dt)* (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->enemy->position_).Normalized();
        e_bullet.e_ifCollide = false;

        e_bullet.e_pitch = 0.f;
        e_bullet.e_yaw = yaw;
        enemybullet.push_back(e_bullet);
        e_elapsedTime = 0.f;  //reset
    }
    if (e_elapsedTime >= 0.2 && rapidfireon == true && spreadfire == false && unlimitedbulletworks == false)
    {
        EnemyBullet e_bullet;
        e_bullet.e_bulletPos = SharedData::GetInstance()->enemy->position_;
        e_bullet.e_bulletDir = (BULLETSPEED + 150)* (float)(dt)* (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->enemy->position_).Normalized();
        e_bullet.e_ifCollide = false;

        e_bullet.e_pitch = 0.f;
        e_bullet.e_yaw = enemy.yaw;

        enemybullet.push_back(e_bullet);
        e_elapsedTime = 0.f;  //reset
        rapidfirecount++;
        if (rapidfirecount > 2)
        {
            rapidfirecount = 0;
            rapidfireon = false;
        }
    }
    if (e_elapsedTime >= 0.5 && spreadfire == true)
    {
        for (int i = -300; i <= 300; i+=(rand() % 20 + 35))
        {
            EnemyBullet e_bullet;
            e_bullet.e_bulletPos = SharedData::GetInstance()->enemy->position_;
            e_bullet.e_bulletDir = (BULLETSPEED + 2 * powerspike) * (float)(dt)* (Vector3(SharedData::GetInstance()->camera->target.x + i, SharedData::GetInstance()->camera->target.y, SharedData::GetInstance()->camera->target.z) - SharedData::GetInstance()->enemy->position_).Normalized();
            e_bullet.e_ifCollide = false;
            e_bullet.e_pitch = 0.f;
            e_bullet.e_yaw = yaw;
            enemybullet.push_back(e_bullet);
        }
        circlecount++;
        if (circlecount > 2)
        {
            circlecount = 0;
            spreadfire = false;
        }
        e_elapsedTime = 0.f;  //reset
    }

    if (e_elapsedTime >= 0.8 && unlimitedbulletworks == true)
    {
        for (int i = -200; i < 200; i += (rand() % 20 + 40))
        {
            for (int p = -200; p < 200; p += (rand() % 20 + 40))
            {
                EnemyBullet e_bullet;
                e_bullet.e_bulletPos = SharedData::GetInstance()->enemy->position_;
                e_bullet.e_bulletPos.x = SharedData::GetInstance()->enemy->position_.x + i;
                e_bullet.e_bulletPos.z = SharedData::GetInstance()->enemy->position_.z + p;

                e_bullet.e_bulletDir = ((BULLETSPEED - 150) + 20 * powerspike) * (float)(dt)* (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->enemy->position_).Normalized();
                e_bullet.e_ifCollide = false;
                e_bullet.e_pitch = 0.f;
                e_bullet.e_yaw = enemy.yaw;
                enemybullet.push_back(e_bullet);
            }
        }
        ubwcount++;
        if (ubwcount > 2)
        {
            ubwcount = 0;
            unlimitedbulletworks = false;
        }
        e_elapsedTime = 0.f;  //reset
    }
}

void SP2::bulletMove(double dt)
{
    if (SharedData::GetInstance()->paused != 1) 
    {
        for (unsigned int i = 0; i < playerbullet.size(); ++i)
        {
            playerbullet[i].p_bulletPos += playerbullet[i].p_bulletDir;
        }
        for (unsigned int i = 0; i < enemybullet.size(); ++i)
        {
            enemybullet[i].e_bulletPos += enemybullet[i].e_bulletDir;
        }
    }
}

void SP2::RenderBullets()
{
    for (unsigned i = 0; i < playerbullet.size(); ++i) 
    {
        modelStack.PushMatrix();
        modelStack.Translate(playerbullet[i].p_bulletPos.x, playerbullet[i].p_bulletPos.y, playerbullet[i].p_bulletPos.z);
        modelStack.Rotate(playerbullet[i].p_pitch, 1, 0, 0);
        modelStack.Rotate(playerbullet[i].p_yaw, 0, 1, 0);
        modelStack.Scale(0.1f, 0.1f, 0.1f);
        RenderMesh(meshList[GEO_BULLET], false);
        modelStack.PopMatrix();
    }
    //enemy
    for (unsigned i = 0; i < enemybullet.size(); ++i) 
    {
        modelStack.PushMatrix();
        modelStack.Translate(enemybullet[i].e_bulletPos.x, enemybullet[i].e_bulletPos.y, enemybullet[i].e_bulletPos.z);
        modelStack.Rotate(enemybullet[i].e_yaw, 0, 1, 0);
        modelStack.Scale(0.1f, 0.1f, 0.1f);
        RenderMesh(meshList[GEO_E_BULLET], false);
        modelStack.PopMatrix();
    }
}

void SP2::UpdateInventory(double dt)
{
    delayer += (float)(dt);
    if (delayer >= 0.125f)
    {
        if (Application::IsKeyPressed('V')) {
            SharedData::GetInstance()->player->addItem(1);
            delayer = 0;
        }
        if (Application::IsKeyPressed('B')) {
            SharedData::GetInstance()->player->addItem(2);
            delayer = 0;
        }
        if ((Application::IsKeyPressed('Z')) && (SharedData::GetInstance()->player->invselect > 0))
        {
            SharedData::GetInstance()->player->invselect -= 1;
            rotator = 0;
            delayer = 0;
        }
        if (Application::IsKeyPressed('X'))
        {
			if (SharedData::GetInstance()->player->inventory[SharedData::GetInstance()->player->invselect] != 0)
			{
				if (SharedData::GetInstance()->gamestate == GAME_STATE_SHOP)
				{
                    SharedData::GetInstance()->player->changeGold(invmap.find((int)SharedData::GetInstance()->player->inventory[(int)(SharedData::GetInstance()->player->invselect)])->second.getValue() / 2);
				}
				SharedData::GetInstance()->player->removeItem(SharedData::GetInstance()->player->invselect);
				rotator = 0;
				delayer = 0;
			}
			SharedData::GetInstance()->player->invfulldisplay = false;
			
        }
        if ((Application::IsKeyPressed('C')) && (SharedData::GetInstance()->player->invselect < 7))
        {
            SharedData::GetInstance()->player->invselect += 1;
            rotator = 0;
            delayer = 0;
        }
    }
}

void SP2::RenderLoveMeter()
{
    RenderObjectOnScreen(meshList[GEO_HEART], 1.5f, 40, 1, 1);
    int loveMeter = SharedData::GetInstance()->dialogueProcessor.npc->getLoveMeter();
    RenderObjectOnScreen(meshList[GEO_LOVEMETER_BAR], 1.5f, 18, (0.5f + loveMeter / 5), 1, 90);
    
    std::stringstream ss;
    ss << loveMeter << "%";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 2.f, 18);
}
void SP2::RenderPlayerHealth(){

    RenderObjectOnScreen(meshList[GEO_PLAYERHEALTH_BAR], 23, 10, ((float)(player.getHealth()) / 3), 1, 0);

    std::stringstream ss;
    ss << player.getHealth() << "/100 HP";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 14, 6);
}
void SP2::RenderBossHealth()
{
    RenderObjectOnScreen(meshList[GEO_BOSSHEALTH_BAR], 23, 40, ((float)(enemy.getHealth()) / 10), 1, 0);

    std::stringstream ss;
    ss << enemy.getHealth() << "/450 HP";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 14, 22);
    std::stringstream sd;
    sd << "WEDDING CRASHER";
    RenderTextOnScreen(meshList[GEO_TEXT], sd.str(), Color(1, 0, 0), 2, 14, 21);
}

void SP2::Sleep(double dt)
{
    SharedData::GetInstance()->cursor_xpos = SharedData::GetInstance()->cursor_newxpos;
    SharedData::GetInstance()->cursor_ypos = SharedData::GetInstance()->cursor_newypos;

    Application::IsKeyPressed('W');
    Application::IsKeyPressed('S');
    Application::IsKeyPressed('A');
    Application::IsKeyPressed('D');

    if (SharedData::GetInstance()->sleep) {
        SharedData::GetInstance()->daynighttime += 200 * dt;
        sleepTime += 100 * dt;
        if (sleepTime > 400) {      //sleep for 4 hours
            SharedData::GetInstance()->sleep = false;
            sleepTime = 0;
            //reset the mini-games' bool
            for (vector<NPC*>::iterator it = SharedData::GetInstance()->NPCs.begin(); it != SharedData::GetInstance()->NPCs.end(); ++it) {
                (*it)->minigameComplete_ = false;
            }
        }
    }
}

void SP2::RenderHangar()
{
    for (int i = -175; i < 176; i += 350)
    {
        modelStack.PushMatrix();
        modelStack.Translate(75, 0, (float)(i));
        modelStack.Scale(5, 8, 5);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_TABLE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(115, 0, (float)(i));
        modelStack.Scale(5, 8, 5);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_TABLE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(155, 0, (float)(i));
        modelStack.Scale(5, 8, 5);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_TABLE], true);
        modelStack.PopMatrix();
    }
    modelStack.PushMatrix();
    modelStack.Translate(75, 15, -175);
    modelStack.Scale(5, 8, 5);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_TOOLBOX], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(75, 15, 175);
    modelStack.Scale(5, 8, 5);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_TOOLBOX], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(50, 7, 193);
    modelStack.Scale(3, 3, 3);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_FIREEXTINGUISHER], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(50, 7, -193);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_FIREEXTINGUISHER], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(100, 17, -175);
    modelStack.Scale(5, 8, 5);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_HAMMER], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(140, 17, -175);
    modelStack.Scale(5, 8, 5);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_HAMMER], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(100, 17, 175);
    modelStack.Scale(5, 8, 5);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_HAMMER], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(140, 17, 175);
    modelStack.Scale(5, 8, 5);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_HAMMER], true);
    modelStack.PopMatrix();
}

void SP2::RenderRoom1()
{
    modelStack.PushMatrix();
    modelStack.Translate(429, 2, -110);
    modelStack.Scale(5, 6, 5);
    RenderMesh(meshList[GEO_MEWMEWKISSYCUTIE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(430, 2, -75);
    modelStack.Scale(3, 6, 3);
    RenderMesh(meshList[GEO_TRASH], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(465, 2, -151);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(4, 6, 4);
    RenderMesh(meshList[GEO_DOGFOOD], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(500, 2, -151);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(4, 6, 4);
    RenderMesh(meshList[GEO_TABLE], true);
    modelStack.PopMatrix();
}

void SP2::RenderRoom2()
{
    //player room
    modelStack.PushMatrix();
    modelStack.Translate(533, 4, 188);
    modelStack.Scale(3, 5, 4);
    RenderMesh(meshList[GEO_BED], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(433, 2, 172);
    modelStack.Scale(12, 12, 12);
    modelStack.Rotate(270, 0, 1, 0);
    RenderMesh(meshList[GEO_LABCOUNTER_CORNER], true);  //btm right corner
    modelStack.PushMatrix();
    modelStack.Translate(-2, 0.9, 0);
    modelStack.Scale(0.3, 0.3, 0.3);
    //modelStack.Rotate(270, 0, 1, 0);
    RenderMesh(meshList[GEO_BOXCRATE], true);
    modelStack.PopMatrix();
    modelStack.PushMatrix();
    modelStack.Translate(0.744, 1.2, 0.2);
    modelStack.Scale(0.6, 1, 1);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_SIGNCOMP3], false);
    modelStack.PushMatrix();
    modelStack.Translate(-0.5, -0.2, 0.5);
    modelStack.Scale(1, 1, 1);
    modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_KEYBOARD], false);
    modelStack.PopMatrix();
    modelStack.PopMatrix();
    modelStack.PushMatrix();
    modelStack.Translate(1.4, 1.2, -0.15);
    modelStack.Scale(0.6, 1, 0.6);
    modelStack.Rotate(-40, 0, 1, 0);
    RenderMesh(meshList[GEO_SIGNCOMP2], false);
    modelStack.PopMatrix();
    modelStack.PushMatrix();
    modelStack.Translate(0.076, 1.2, -0.13);
    modelStack.Scale(0.6, 1, 0.6);
    modelStack.Rotate(-130, 0, 1, 0);
    RenderMesh(meshList[GEO_SIGNCOMP1], false);
    modelStack.PopMatrix();
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(433, 1.9, 58);
    modelStack.Scale(40, 40, 40);
    //modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_JAGCUPBOARDS], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(490, 2, 110);
    modelStack.Scale(7, 5.5, 5);
    RenderMesh(meshList[GEO_TABLE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(445, 2, 180);
    modelStack.Scale(4, 4, 4);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_TECHCHAIR], true);
    modelStack.PopMatrix();

    //end of player room

    // other room
    modelStack.PushMatrix();
    modelStack.Translate(740, 2, -140);
    modelStack.Scale(4, 6, 4);
    RenderMesh(meshList[GEO_TABLE], true);
    modelStack.PopMatrix();
}

void SP2::RenderRoom3()
{

}
void SP2::RenderRoom4()
{

}

void SP2::RenderShop()
{
    //stemmie herself
    modelStack.PushMatrix();
    modelStack.Translate(890, 15, -40);
    modelStack.Scale(4, 4, 4);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_STEMMIE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(895, 30 + vibrateY, -36.7f + vibrateX);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_STEMMIE_FACE], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(895, 15, -45);
    modelStack.Scale(4, 4, 4);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_TEMSHOP], true);
    modelStack.PopMatrix();

    //Shop objs
    modelStack.PushMatrix();
    modelStack.Translate(935, 2, 60);
    modelStack.Scale(10, 10, 10);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_VENDINGMACHINE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(900, 1, 30);
    modelStack.Scale(8, 8, 8);
    RenderMesh(meshList[GEO_SHOPSHELF], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(860, 1, 30);
    modelStack.Scale(8, 8, 8);
    RenderMesh(meshList[GEO_SHOPSHELF], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(845, 4, -70);
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(830, 10, 100);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_FIREEXTINGUISHER], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(900, 0, -45);
    modelStack.Scale(18, 6, 4);
    RenderMesh(meshList[GEO_COUNTER], true);
    modelStack.PopMatrix();
}

void SP2::RenderCanteen()
{
    for (int i = 0; i < 12; i++)
    {
        modelStack.PushMatrix();
        modelStack.Translate(SharedData::GetInstance()->interactionItems[i]->bound1.x + 3, 14, SharedData::GetInstance()->interactionItems[i]->bound1.z + 3);
        modelStack.Scale(2, 2, 2);
        RenderMesh(meshList[GEO_SPAGHETTO], true);
        modelStack.PopMatrix();
    }

    //Jasim is Hawt
    modelStack.PushMatrix();
    modelStack.Translate(935, 5, -10);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_FRIDGE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(935, 5, 20);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_FRIDGE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    //modelStack.Translate(935, 5, 50);
    //modelStack.Rotate(180, 0, 1, 0);
    modelStack.Translate(685, 0, -430);
    modelStack.Scale(10, 10, 10);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_JASIM], true);
    modelStack.PopMatrix();

    //OBJ stuff
    //Canteen
    for (int i = 0; i < 130; i += 60)
    {
        //-520 //-505
        modelStack.PushMatrix();
        modelStack.Translate(740, 0, (float)(-515 + i));
        modelStack.Scale(7, 7, 7);
        modelStack.Rotate(-90, 0, 1, 0);
        RenderMesh(meshList[GEO_BENCH], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(740, 0, (float)(-500 + i));
        modelStack.Scale(7, 7, 7);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_TABLE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(740, 0, (float)(-485 + i));
        modelStack.Scale(7, 7, 7);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_BENCH], true);
        modelStack.PopMatrix();
    }

    modelStack.PushMatrix();
    modelStack.Translate(685, 2, -375);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_VENDINGMACHINE], true);
    modelStack.PopMatrix();


    modelStack.PushMatrix();
    modelStack.Translate(819, 4, -355);
    modelStack.Scale(6, 6, 6);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(819, 4, -515);
    modelStack.Scale(6, 6, 6);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(685, 0, -475);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_FRIDGE], true);
    modelStack.PopMatrix();

    //pepper shaker
    modelStack.PushMatrix();
    modelStack.Translate(735, 14.f, -380);
    RenderMesh(meshList[GEO_JASIM], true);
    modelStack.PopMatrix();
}

void SP2::RenderPlantation()
{
    modelStack.PushMatrix();
    modelStack.Translate(950, 0, -460);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_WENGSTANG], true);
    modelStack.PopMatrix();

    for (int i = 0; i < 11; i++)
    {
        modelStack.PushMatrix();
        modelStack.Translate((float)(835.5 + (i * 19)), 0, -530);
        modelStack.Rotate(-90, 0, 1, 0);
        modelStack.Scale(5, 5, 5);
        RenderMesh(meshList[GEO_FENCE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate((float)(845 + (i * 19)), 0, -530);
        modelStack.Rotate(90, 0, 1, 0);
        modelStack.Scale(5, 5, 5);
        RenderMesh(meshList[GEO_FENCE], true);
        modelStack.PopMatrix();
    }

    for (int i = 0; i < 13; i++)
    {
        modelStack.PushMatrix();
        modelStack.Translate(1039.75f, 0, (float)(-525.25f + (i * 19)));
        modelStack.Scale(5, 5, 5);
        RenderMesh(meshList[GEO_FENCE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(1039.75f, 0, (float)(-515.75 + (i * 19)));
        modelStack.Rotate(180, 0, 1, 0);
        modelStack.Scale(5, 5, 5);
        RenderMesh(meshList[GEO_FENCE], true);
        modelStack.PopMatrix();
    }

    modelStack.PushMatrix();
    modelStack.Translate(1039.75, 0, -525.25 + (13 * 19));
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_FENCE], true);
    modelStack.PopMatrix();


    for (int i = 0; i < 11; i++)
    {
        modelStack.PushMatrix();
        modelStack.Translate((float)(835.5 + (i * 19)), 0, -273.5f);
        modelStack.Rotate(90, 0, 1, 0);
        modelStack.Scale(5, 5, 5);
        RenderMesh(meshList[GEO_FENCE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate((float)(845 + (i * 19)), 0, -273.5f);
        modelStack.Rotate(-90, 0, 1, 0);
        modelStack.Scale(5, 5, 5);
        RenderMesh(meshList[GEO_FENCE], true);
        modelStack.PopMatrix();
    }

    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 7; i++)
        {
            modelStack.PushMatrix();
            modelStack.Translate((float)(875 + (i * 20)), 1, (float)(-310 - (j * 20)));
            modelStack.Rotate(90, 0, 1, 0);
            modelStack.Rotate(-90, 1, 0, 0);
            RenderMesh(meshList[GEO_FARM], true);
            modelStack.PopMatrix();
        }
    }

    modelStack.PushMatrix();
    modelStack.Translate(1000, 1, -480);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(8, 8, 8);
    RenderMesh(meshList[GEO_SHED], true);
    modelStack.PopMatrix();

    for (int i = 0; i < 10; i++)
    {
        modelStack.PushMatrix();
        modelStack.Translate(SharedData::GetInstance()->interactionItems[i + 37]->bound1.x + 3, 1.1f + (i * 0.1f), SharedData::GetInstance()->interactionItems[i + 37]->bound1.z + 3);
        modelStack.Rotate(90, 0, 1, 0);
        modelStack.Rotate(-90, 1, 0, 0);
        RenderMesh(meshList[GEO_WEED], false);
        modelStack.PopMatrix();
    }
    for (unsigned int i = 10; i < weedgame.size(); i++)
    {
        modelStack.PushMatrix();
        modelStack.Translate(SharedData::GetInstance()->interactionItems[i + 37]->bound1.x + 3, 2.3f + (i * 0.02f), SharedData::GetInstance()->interactionItems[i + 37]->bound1.z + 3);
        modelStack.Rotate(90, 0, 1, 0);
        modelStack.Rotate(-90, 1, 0, 0);
        RenderMesh(meshList[GEO_FARMPLANT], false);
        modelStack.PopMatrix(); // teleporter
    }
}

void SP2::RenderControlRoom()
{
    //Switches and Lightings
    modelStack.PushMatrix();
    modelStack.Translate(600, 0, 500);
    modelStack.Rotate(-90, 0, 1, 0);

    modelStack.PushMatrix();
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_SWITCH_1], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 0, 15);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_SWITCH_2], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 0, 30);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_SWITCH_3], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 0, 45);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_SWITCH_4], true);
    modelStack.PopMatrix();
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(600, 0, 503);
    modelStack.Rotate(-90, 0, 1, 0);

    modelStack.PushMatrix();
    modelStack.Translate(0, 30, 0);
    modelStack.Rotate(90, 0, 0, 1);
    RenderMesh(meshList[GEO_CHECK_1], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 30, 15);
    modelStack.Rotate(90, 0, 0, 1);
    RenderMesh(meshList[GEO_CHECK_2], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 30, 30);
    modelStack.Rotate(90, 0, 0, 1);
    RenderMesh(meshList[GEO_CHECK_3], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 30, 45);
    modelStack.Rotate(90, 0, 0, 1);
    RenderMesh(meshList[GEO_CHECK_4], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 15, -15);
    RenderMesh(meshList[GEO_CHECKER_BUTTON], true);
    modelStack.PopMatrix();

    modelStack.PopMatrix();

    //Vee Model
    modelStack.PushMatrix();
    modelStack.Translate(600, 11, 440);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(6, 6, 6);
    RenderMesh(meshList[GEO_VEE], true);
    modelStack.PopMatrix();

    //Control Room
    modelStack.PushMatrix();
    modelStack.Translate(650, 2, 490);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(7, 7, 7);
    RenderMesh(meshList[GEO_CONTROLROOM], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(640, 4, 460);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(6, 6, 6);
    RenderMesh(meshList[GEO_CHAIR], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(510, 2, 490);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(7, 7, 7);
    RenderMesh(meshList[GEO_CONTROLROOM], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(510, 4, 460);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(6, 6, 6);
    RenderMesh(meshList[GEO_CHAIR], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(675, 2, 435);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_SHELF], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(480, 2, 435);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_SHELF], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(480, 4, 390);
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(670, 4, 390);
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(550, 5, 370);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_UMBRELLASTAND], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(620, 10, 360);
    modelStack.Scale(2, 2, 2);
    RenderMesh(meshList[GEO_FIREEXTINGUISHER], true);
    modelStack.PopMatrix();
}

void SP2::RenderLab()
{
    //NPC
    modelStack.PushMatrix();
    modelStack.Translate(412, 5 + chonFloaty, -459);
    modelStack.Scale(10, 10, 10);
    modelStack.Rotate(0, 0, 1, 0);
    RenderMesh(meshList[GEO_CHON], true);
    modelStack.PopMatrix();

    //OBJ
    modelStack.PushMatrix();
    modelStack.Translate(506, 2, -446);
    modelStack.Scale(12, 12, 12);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_LABCOUNTER_CORNER], true);      //top left corner
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(310, 2, -363);
    modelStack.Scale(12, 12, 12);
    modelStack.Rotate(270, 0, 1, 0);
    RenderMesh(meshList[GEO_LABCOUNTER_CORNER], true);  //btm right corner
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(334, 2, -468);
    modelStack.Scale(12, 12, 12);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_LABCOUNTER_CORNER], true);  //btm left corner
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(460, 2, -453);
    modelStack.Scale(12, 12, 8);
    modelStack.Rotate(270, 0, 1, 0);
    RenderMesh(meshList[GEO_LABCOUNTER1], true);    //facenorth
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(373, 2, -440);
    modelStack.Scale(12, 12, 8);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_LABCOUNTER1], true);    //facesouth
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(413, 2, -395);
    modelStack.Scale(8, 12, 12);
    modelStack.Rotate(0, 0, 1, 0);
    RenderMesh(meshList[GEO_LABCOUNTER1], true);    //faceleft
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(424, 2, -382);
    modelStack.Scale(8, 12, 12);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_LABCOUNTER1], true);    //faceright
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(459.2f, 13, -466);
    modelStack.Scale(9, 8, 6);
    RenderMesh(meshList[GEO_TOOLBOX], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(410, 15, -475);
    modelStack.Scale(10, 10, 5);
    RenderMesh(meshList[GEO_SECRETGUIDE], false);
    modelStack.PopMatrix();

    if (postercounter == 6)
    {
        chonSecret();
    }
}

void SP2::RenderToilet()
{
    modelStack.PushMatrix();
    modelStack.Translate(780, 0, -615);
    modelStack.Scale(30, 30, 30);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_TOILBOWL], true);

    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(732, SharedData::GetInstance()->floodlevel, -594);
    modelStack.Scale(7, 5, 4.3f);
    modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_TOILETFLOOD], true);
    modelStack.PopMatrix();

    if (SharedData::GetInstance()->shitintoilet) {     //in mini-game
        modelStack.PushMatrix();
        modelStack.Translate(779.6f, 7.39f, -614.9f);
        modelStack.Scale(1.11f, 1, 1.11f);
        modelStack.Rotate(-90, 1, 0, 0);
        RenderMesh(meshList[GEO_LMAO], true);
        modelStack.PopMatrix();
    }
}

void SP2::RenderChonGame()
{
    //Chon's Lab Balls
    if (SharedData::GetInstance()->ballpickup)
    {
        for (int i = 0; i < 5; i++)
        {
            if (SharedData::GetInstance()->interactptr->bound1 == ballbounds[i].bound1 && SharedData::GetInstance()->interactptr->bound2 == ballbounds[i].bound2)
            {
                ball[i] = true;
            }
            else
                ball[i] = false;
        }
    }

    //game complete
    if (pickupCounter == 3)
    {
        lightpuzz.generatePuzzle();     //to re-generate Chon's mini-game
        //pickupCounter = 0;
    }

    ballmoveCheck();

    compactBallrender();    //yellow, black, white, blue, red
}

void SP2::RenderInShop()
{
    //background
    RenderObjectOnScreen(meshList[GEO_SHOPUI], 40, 30, 50, 50);

    //right arrow
    RenderObjectOnScreen(meshList[GEO_SHOP_ARROW], 55, 30, 1, 1);
    //left arrow
    RenderObjectOnScreen(meshList[GEO_SHOP_ARROW], 25, 30, 1, 1, 180);

    //name of item
    float namelength = (float)((invmap.find(*shop.shopIterator)->second).getName().size());
    RenderTextOnScreen(meshList[GEO_TEXT], (invmap.find(*shop.shopIterator)->second).getName(), Color(0, 1, 1), 3, 14.5f - namelength / 2.f, 14);

    //Temmie text
    RenderDialogueOnScreen("Click Buy to buy. Press X to sell", Color(1, 1, 1), 3);

    RotateDisplay();

    RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 75, 22, 1, 1);
    RenderObjectOnScreen(meshList[GEO_DIALOGUEOPTION], 75, 43, 1, 1);

    std::stringstream s;
    s.str("");
    s << invmap.find(*shop.shopIterator)->second.getValue();
    RenderTextOnScreen(meshList[GEO_TEXT], "Buy(   )", Color(1, 1, 1), 2, 32, 21);
    if (SharedData::GetInstance()->player->getGold() < invmap.find(*shop.shopIterator)->second.getValue()) {    //player does not have enough gold
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(1, 0, 0), 2, 35, 21);
    }
    else {
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(1, 1, 0), 2, 35, 21);
    }
    RenderTextOnScreen(meshList[GEO_TEXT], "Bye", Color(1, 1, 1), 2, 32, 10.5f);
    RenderCursor();
}