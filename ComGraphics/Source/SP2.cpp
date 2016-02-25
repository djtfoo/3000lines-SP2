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

#include <iostream>

SP2::SP2()
{
    delayBuffer = 0;
    lightpower = 0.3;
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

	meshList[GEO_DEVTEXTURE] = MeshBuilder::GenerateQuad("devtexture", Color(1, 1, 1), 1, 1);
	meshList[GEO_DEVTEXTURE]->textureID = LoadTGA("Image/devtexture.tga");

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

    //GEO_MAP
    meshList[GEO_MAP] = MeshBuilder::GenerateQuad("minimap", Color(1, 1, 1), 10, 10);
    
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

    meshList[GEO_COUNTER] = MeshBuilder::GenerateCube("counter", Color(0, 0.7, 1), 5, 5, 5);


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

    meshList[GEO_SPHEREYELLOW] = MeshBuilder::GenerateSphere("spheretemp", Color(1, 1, 0), 5, 36, 18);
    //meshList[GEO_SPHERETEMP]->textureID = LoadTGA("Image/translucenttest.tga");
    meshList[GEO_SPHEREYELLOW]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHEREYELLOW]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHEREYELLOW]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHEREYELLOW]->material.kShininess = 1.f;

    meshList[GEO_SPHEREBLACK] = MeshBuilder::GenerateSphere("spheretemp", Color(0, 0, 0), 5, 36, 18);
    //meshList[GEO_SPHERETEMP]->textureID = LoadTGA("Image/translucenttest.tga");
    meshList[GEO_SPHEREBLACK]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHEREBLACK]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHEREBLACK]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHEREBLACK]->material.kShininess = 1.f;

    meshList[GEO_SPHEREWHITE] = MeshBuilder::GenerateSphere("spheretemp", Color(1, 1, 1), 5, 36, 18);
    //meshList[GEO_SPHERETEMP]->textureID = LoadTGA("Image/translucenttest.tga");
    meshList[GEO_SPHEREWHITE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHEREWHITE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHEREWHITE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHEREWHITE]->material.kShininess = 1.f;

    meshList[GEO_SPHEREBLUE] = MeshBuilder::GenerateSphere("spheretemp", Color(0, 0.3, 1), 5, 36, 18);
    //meshList[GEO_SPHERETEMP]->textureID = LoadTGA("Image/translucenttest.tga");
    meshList[GEO_SPHEREBLUE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHEREBLUE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHEREBLUE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHEREBLUE]->material.kShininess = 1.f;

    meshList[GEO_SPHERERED] = MeshBuilder::GenerateSphere("spheretemp", Color(1, 0.3, 0), 5, 36, 18);
    //meshList[GEO_SPHERETEMP]->textureID = LoadTGA("Image/translucenttest.tga");
    meshList[GEO_SPHERERED]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
    meshList[GEO_SPHERERED]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
    meshList[GEO_SPHERERED]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
    meshList[GEO_SPHERERED]->material.kShininess = 1.f;

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

    meshList[GEO_CROSSHAIRS] = MeshBuilder::GenerateQuad("crosshairs", Color(1, 1, 1), 3, 3);
    meshList[GEO_CROSSHAIRS]->textureID = LoadTGA("Image/crosshairs.tga");

	meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("invbar", Color(1, 1, 1), 40, 5);
	meshList[GEO_INVENTORY]->textureID = LoadTGA("Image/inventorybar.tga");

	meshList[GEO_ITEMSELECT] = MeshBuilder::GenerateQuad("itemselect", Color(1, 1, 1), 4, 4);
	meshList[GEO_ITEMSELECT]->textureID = LoadTGA("Image/itemselect.tga");

    meshList[GEO_MAP] = MeshBuilder::GenerateMinimap("map", 10, 10);
    meshList[GEO_MAP]->textureID = LoadTGA("Image/Donna.tga");

	meshList[GEO_DAYNIGHTICON] = MeshBuilder::GenerateQuad("daynight", Color(1, 1, 1), 18, 18);
	meshList[GEO_DAYNIGHTICON]->textureID = LoadTGA("Image/daynighticon.tga");

	meshList[GEO_TOILETFLOOD] = MeshBuilder::GenerateQuad("toiletflood", Color(1, 1, 1), 18, 18);
	meshList[GEO_TOILETFLOOD]->textureID = LoadTGA("Image/shltwater.tga");

	meshList[GEO_SPAGHETTO] = MeshBuilder::GenerateOBJ("spaghetto", "OBJ/spaghetto.obj");
	meshList[GEO_SPAGHETTO]->textureID = LoadTGA("Image/spaghetto.tga");
	meshList[GEO_SPAGHETTO]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SPAGHETTO]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPAGHETTO]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_SPAGHETTO]->material.kShininess = 1.f;

	meshList[GEO_BED] = MeshBuilder::GenerateOBJ("bed", "OBJ/bed.obj");
	meshList[GEO_BED]->textureID = LoadTGA("Image/bed.tga");
	meshList[GEO_BED]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BED]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BED]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_BED]->material.kShininess = 1.f;

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

    meshList[GEO_CHECK_1] = MeshBuilder::GenerateHemisphere("switch1", Color(0, 0, 0), 1, 15, 4);
    meshList[GEO_CHECK_2] = MeshBuilder::GenerateHemisphere("switch2", Color(0, 0, 0), 1, 15, 4);
    meshList[GEO_CHECK_3] = MeshBuilder::GenerateHemisphere("switch3", Color(0, 0, 0), 1, 15, 4);
    meshList[GEO_CHECK_4] = MeshBuilder::GenerateHemisphere("switch4", Color(0, 0, 0), 1, 15, 4);

    viewOptions = true;

    objx = objy = 1;

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
        "ROOM4"
    };
    
    for (LOCATION i = OUTSIDE; i < LOCATION_TOTAL; i = static_cast<LOCATION>(i+1)) {
        locations[i] = temp[i];
    }
}

SP2::~SP2()
{
}

void SP2::loadInv()
{
	invmap.insert(std::pair<int, Gift>(1, Gift("hammer")));
	modelmap.insert(std::pair<int, GEOMETRY_TYPE>(1, GEO_HAMMER));
	invmap.insert(std::pair<int, Gift>(2, Gift("tEmmEh")));
	modelmap.insert(std::pair<int, GEOMETRY_TYPE>(2, GEO_STEMMIE));
	invmap.insert(std::pair<int, Gift>(3, Gift(" weed")));
	modelmap.insert(std::pair<int, GEOMETRY_TYPE>(3, GEO_WEEDICON));
	invmap.insert(std::pair<int, Gift>(4, Gift("spaghetti")));
	modelmap.insert(std::pair<int, GEOMETRY_TYPE>(4, GEO_SPAGHETTO));
}

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
    light[0].power = lightpower;
    light[0].kC = 1.f;
    light[0].kL = 0.01f;
    light[0].kQ = 0.001f;

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

    Interaction* interactions;

	//spaghetti random spawns
	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976, -12, -378.31 + 2); interactions->bound2.Set(763.976, -10, -368.31 - 2);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976, -12, -391.7 + 2); interactions->bound2.Set(763.976, -10, -381.7 - 2);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976, -12, -437.6 + 2); interactions->bound2.Set(763.976, -10, -427.6 - 2);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976, -12, -452.9 + 2); interactions->bound2.Set(763.976, -10, -442.9 - 2);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976, -12, -498.8 + 2); interactions->bound2.Set(763.976, -10, -488.8 - 2);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(757.976, -12, -510.7 + 2); interactions->bound2.Set(763.976, -10, -500.7 - 2);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872 - 3, -12, -373.31 - 3); interactions->bound2.Set(721.872 + 3, -10, -373.31 + 3);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872 - 3, -12, -386.7 - 3); interactions->bound2.Set(721.872 + 3, -10, -386.7 + 3);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872 - 3, -12, -432.6 - 3); interactions->bound2.Set(721.872 + 3, -10, -432.6 + 3);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872 - 3, -12, -447.9 - 3); interactions->bound2.Set(721.872 + 3, -10, -447.9 + 3);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872 - 3, -12, -493.8 - 3); interactions->bound2.Set(721.872 + 3, -10, -493.8 + 3);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);

	interactions = new SpaghettoInteraction();
	interactions->bound1.Set(721.872 - 3, -12, -505.7 - 3); interactions->bound2.Set(721.872 + 3, -10, -505.7 + 3);
	SharedData::GetInstance()->interactionItems.push_back(interactions);
	spaghettilocation.push_back(interactions);
	//spaghetto random spawns

    //Gate interaction bounds Inits
    gateOpenInteractions();

    //Other Interaction Bounds
    interactions = new ShopInteraction();
    interactions->bound1.Set(890, -15, -35);     interactions->bound2.Set(900, 20, -25);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

	interactions = new ManureInteraction();
	interactions->bound1.Set(780, -5, -616);     interactions->bound2.Set(800, 5, -612);
	SharedData::GetInstance()->interactionItems.push_back(interactions);

    //interactions = new ChonGame();
    //interactions->bound1.Set(400,-15,-430); interactions->bound2.Set(425,-5,-440);
    //SharedData::GetInstance()->interactionItems.push_back(interactions);

    ballboundfunct();


    //Vee Puzzle Interaction
    interactions = new VeePuzzleSwitchOneInteraction();
    interactions->bound1.Set(597, -10, 490);    interactions->bound2.Set(607, 5, 500);
    SharedData::GetInstance()->interactionItems.push_back(interactions);
    interactions = new VeePuzzleSwitchTwoInteraction();
    interactions->bound1.Set(580, -10, 490);    interactions->bound2.Set(587, 5, 500);
    SharedData::GetInstance()->interactionItems.push_back(interactions);
    interactions = new VeePuzzleSwitchThreeInteraction();
    interactions->bound1.Set(564, -10, 490);    interactions->bound2.Set(575, 5, 500);
    SharedData::GetInstance()->interactionItems.push_back(interactions);
    interactions = new VeePuzzleSwitchFourInteraction();
    interactions->bound1.Set(549, -10, 490);    interactions->bound2.Set(558, 5, 500);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

    interactions = new VeePuzzleCheckerSwitchInteraction();
    interactions->bound1.Set(608, -10, 490);    interactions->bound2.Set(617, 5, 500);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

	interactions = new BedTime();
	interactions->bound1.Set(518, -5, 176); interactions->bound2.Set(548, -15, 185);
	SharedData::GetInstance()->interactionItems.push_back(interactions);

	loadWeedGame();

	for (int i = 0; i < 10; i++)
	{

		interactions = new WeedInteraction();
		interactions->bound1.Set(weedgame[i].x - 3, -25, weedgame[i].z - 3); interactions->bound2.Set(weedgame[i].x + 3, -19, weedgame[i].z + 3);
		SharedData::GetInstance()->interactionItems.push_back(interactions);
	}

    //TESTING DIALOGUES - Jasim
    interactions = new Dialogue();
    interactions->bound1.Set(685 - 50, 0 - 5, -430 - 50); interactions->bound2.Set(685 + 50, 0 + 5, -430 + 50);
    SharedData::GetInstance()->interactionItems.push_back(interactions);

    gate1pos = 49;
    gate2pos = 143;
    gatemove = 0;


    rotating = 0;
    ptxt1 = 70;     //pause textbox
    ptxt2 = 78;
    ptxt3 = 86;
    ptxtexit = 94;
    rotator = 0;
	SharedData::GetInstance()->daynighttime = 0000;
	floodlevel = -1;

    //Init chon game variables
    for (int i = 0; i < 5; i++)
    {
        ball[i] = false;
    }
    ballfloat= ballfloat1= ballfloat2= ballfloat3= ballfloat4 = 0;
    pickupcheck = false;


	loadInv();
    loadCollisions();

    lightpuzz.generatePuzzle();

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
}

void SP2::loadWeedGame()
{
	if (weedgame.size() != 0)
	{
		for (int i = 0; i < 10; i++)
		{
			weedgame[i] = (Vector3(rand() % 140 + 865, 1, rand() % 100 - 400));
		}

	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			weedgame.push_back(Vector3(rand() % 140 + 865, 1, rand() % 100 - 400));
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
    
    SharedData::GetInstance()->player->Walk(dt);

    CheckCharacterLocation();

    SharedData::GetInstance()->camera->Update(dt);
    
    SharedData::GetInstance()->player->CheckInteraction();

	if (Application::IsKeyPressed('U') && SharedData::GetInstance()->canInteract && delayBuffer >= 2) {
		if (SharedData::GetInstance()->interactnumber != 25)
		{
			delayBuffer = 0;
		}
		else
		{
			loadSpaghetti();
		}
        

        SharedData::GetInstance()->interactptr->DoInteraction();
    }
    else
    {
        delayBuffer += 0.1;
        if (delayBuffer >= 2)
        {
            delayBuffer = 2;
        }
    }

	if (delayer > 0)
		delayer -= 1;
	rotator++;
	SharedData::GetInstance()->daynighttime+= (dt * 10);
	if (((int)SharedData::GetInstance()->daynighttime % 100) > 60)
	{
		SharedData::GetInstance()->daynighttime += 40;
		if (SharedData::GetInstance()->interactnumber != 25)
		{
			SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() + 5);
		}
		SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() + 1);
	}
	if (SharedData::GetInstance()->daynighttime > 2400)
	{
		SharedData::GetInstance()->daynighttime = 0;
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
    if (Application::IsKeyPressed('R')) {       //reset
        Reset();
    }

    if (vibrateX < 0.6 && vibrateY < 0.6)
    {
        vibrateX += 0.25;
        vibrateY += 0.17;
    }
    else
    {
        vibrateX = 0;
        vibrateY = 0;
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
    

    //debugging code
    if (Application::IsKeyPressed('O'))
    {
		SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() - 1);
    }
    if (Application::IsKeyPressed('P'))
    {
		SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() + 1);
    }
	if (SharedData::GetInstance()->player->getHunger() <= 0)
		SharedData::GetInstance()->player->setHunger(0);
	if (SharedData::GetInstance()->player->getHunger() >= 100)
		SharedData::GetInstance()->player->setHunger(100);

    rotating += 30 * dt;

    if (Application::IsKeyPressed('I'))
        objy += 40 * dt;
    if (Application::IsKeyPressed('K'))
        objy -= 40 * dt;
    if (Application::IsKeyPressed('J'))
        objx -= 40 * dt;
    if (Application::IsKeyPressed('L'))
        objx += 40 * dt;

    //pause game
    if (Application::IsKeyPressed('N'))
    {
        SharedData::GetInstance()->gamestate = GAME_STATE_PAUSED;
        pauseAnimation(dt);
    }
    if (SharedData::GetInstance()->gamestate == GAME_STATE_PAUSED)
    {
        pauseAnimation(dt);
    }

        //Lighting
    if (SharedData::GetInstance()->daynighttime >= 0700 && SharedData::GetInstance()->daynighttime <= 1850 && lightpower <= 1.3)
    {
        lightpower += 0.001;
        light[0].power = lightpower;
        glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
    }
    if (SharedData::GetInstance()->daynighttime >= 1900 && lightpower >= 0.3 && lightpos <= 1000)
    {
        lightpower -= 0.001;
        lightpos += 1;
        light[0].power = lightpower;
        glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
    }

    //Interactions?
    if ((SharedData::GetInstance()->toiletflood == true) && (floodlevel < 3))
    {
        floodlevel += dt;
    }
    if (SharedData::GetInstance()->chonGamebool == true)
    {
        SharedData::GetInstance()->gamestate = GAME_STATE_CHONGAME;
    }
    if (SharedData::GetInstance()->ballpickup == true)
    {
        if (ball[0])
            ballfloat += 40 * dt;
        if (ball[1])
            ballfloat1 += 40 * dt;
        if (ball[2])
            ballfloat2 += 40 * dt;
        if (ball[3])
            ballfloat3 += 40 * dt;
        if (ball[4])
            ballfloat4 += 40 * dt;
    }

    if (SharedData::GetInstance()->gateopen)
    {
        //gatemove -= 5 * dt;
        gate1pos -= 0.5;
        gate2pos -= 0.5;

       
    }


    //Position of Light
    if (SharedData::GetInstance()->daynighttime >= 0700 && SharedData::GetInstance()->daynighttime <= 1850 && lightpos >= -1000)
    {
        lightpos -= 0.4;
        light[0].position.Set(0, 1000, lightpos);
    }
    if (SharedData::GetInstance()->daynighttime >= 1900 || SharedData::GetInstance()->daynighttime <= 0650 && lightpos <= 1000)
    {
        lightpos += 0.85;
        light[0].position.Set(0, 1000, lightpos);
    }

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

    //Lights
    if (SharedData::GetInstance()->switch1 == false)
        meshList[GEO_CHECK_1] = MeshBuilder::GenerateHemisphere("check1", Color(0, 0, 0), 2);
    if (SharedData::GetInstance()->switch1 == true)
        meshList[GEO_CHECK_1] = MeshBuilder::GenerateHemisphere("check1", Color(1, 1, 1), 2);
    if (SharedData::GetInstance()->switch2 == false)
        meshList[GEO_CHECK_2] = MeshBuilder::GenerateHemisphere("check2", Color(0, 0, 0), 2);
    if (SharedData::GetInstance()->switch2 == true)
        meshList[GEO_CHECK_2] = MeshBuilder::GenerateHemisphere("check2", Color(1, 1, 1), 2);
    if (SharedData::GetInstance()->switch3 == false)
        meshList[GEO_CHECK_3] = MeshBuilder::GenerateHemisphere("check3", Color(0, 0, 0), 2);
    if (SharedData::GetInstance()->switch3 == true)
        meshList[GEO_CHECK_3] = MeshBuilder::GenerateHemisphere("check3", Color(1, 1, 1), 2);
    if (SharedData::GetInstance()->switch4 == false)
        meshList[GEO_CHECK_4] = MeshBuilder::GenerateHemisphere("check4", Color(0, 0, 0), 2);
    if (SharedData::GetInstance()->switch4 == true)
        meshList[GEO_CHECK_4] = MeshBuilder::GenerateHemisphere("check4", Color(1, 1, 1), 2);

	if (lightpuzz.checkPuzzleAns(SharedData::GetInstance()->one, SharedData::GetInstance()->two, SharedData::GetInstance()->three, SharedData::GetInstance()->four) == true)
    {
        //if (SharedData::GetInstance()->switch1 == true)
        //{
        //    meshList[GEO_CHECK_1] = MeshBuilder::GenerateHemisphere("check1", Color(1, 1, 1), 2);
        //}
        //if (SharedData::GetInstance()->switch2 == true)
        //{
        //    meshList[GEO_CHECK_2] = MeshBuilder::GenerateHemisphere("check2", Color(1, 1, 1), 2);
        //}
        //if (SharedData::GetInstance()->switch3 == true)
        //{
        //    meshList[GEO_CHECK_3] = MeshBuilder::GenerateHemisphere("check3", Color(1, 1, 1), 2);
        //}
        //if (SharedData::GetInstance()->switch4 == true)
        //{
        //    meshList[GEO_CHECK_4] = MeshBuilder::GenerateHemisphere("check4", Color(1, 1, 1), 2);
        //}
        SharedData::GetInstance()->one = SharedData::GetInstance()->two = SharedData::GetInstance()->three = SharedData::GetInstance()->four = 1;
        std::cout << "You win!" << std::endl;
        lightpuzz.generatePuzzle();
    }

    if (Application::IsKeyPressed('P'))
    {
        SharedData::GetInstance()->gamestate = GAME_STATE_RABBIT;
    }
	SharedData::GetInstance()->interactnumber = 99;
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

    //pass the position of light to shader based on light type
    //spotlights
    if (light[0].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
    }

    modelStack.PushMatrix();
    modelStack.Translate(895, 30 + vibrateY, -36.7 + vibrateX);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_STEMMIE_FACE], false);
    modelStack.PopMatrix();

    switch (SharedData::GetInstance()->gamestate)
    {
    case GAME_STATE_FREE: loadFree();
        break;
    case GAME_STATE_SHOP: loadShop();
        break;
    case GAME_STATE_WSGAME: loadWSGame();
        break;
    case GAME_STATE_CHONGAME: loadChonGame();
        break;
    case GAME_STATE_VEEGAME: loadVeeGame();
        break;
    case GAME_STATE_PAUSED: pauseGame();
        break;
    case GAME_STATE_RABBIT: loadRabbitGame();
        break;
    case GAME_STATE_DIALOGUE: loadFree();
        RenderDialogueOnScreen(SharedData::GetInstance()->dialogueProcessor.npc->Speech(), Color(1, 1, 1), 3);
        break;
    }
    //RenderMinimap();
}

void SP2::loadFree()
{  
    RenderSkybox();
    RenderGround();
	RenderRoom();

    //The House
    modelStack.PushMatrix();
    modelStack.Translate(420, -1, -23);
    modelStack.Scale(50, 50, 50);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_LAYOUT], true);
    modelStack.PopMatrix();
    
    modelStack.PushMatrix();
    modelStack.Translate(0, 12, 0);
    modelStack.Translate(SharedData::GetInstance()->player->position_.x, SharedData::GetInstance()->player->position_.y, SharedData::GetInstance()->player->position_.z);
    modelStack.Rotate(SharedData::GetInstance()->player->direction_, 0, 1, 0);
    RenderPlayer();
    modelStack.PopMatrix();

	for (int i = 0; i < 12; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(SharedData::GetInstance()->interactionItems[i]->bound1.x + 3, 14, SharedData::GetInstance()->interactionItems[i]->bound1.z + 3);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_SPAGHETTO], true);
		modelStack.PopMatrix();
	}


    //RenderPuzzle();

    RenderNPC();
    stemmieShop();
    chonLab();
    veeControlroom();
    jasimCanteen();
    loadHangar();
	renderFarm();
    renderPuzzle();

    RenderGates(); //gates b4 ui, aft others to hide contents of the room

    RenderUI();

	RenderInventory();
	RenderTime();

    RenderObjectOnScreen(meshList[GEO_CROSSHAIRS], 40, 30);
    RenderObjectOnScreen(meshList[GEO_INVENTORY], 40, 2.5);
    RenderObjectOnScreen(meshList[GEO_CROSSHAIRS], 40, 30,1,1);

    //RenderMinimap();
    
    //interaction
    if (SharedData::GetInstance()->canInteract) {
        RenderTextOnScreen(meshList[GEO_TEXT], "Press U", Color(1, 0, 0), 3, 2, 2);
    }
}  

void SP2::gateOpenInteractions()
{
    Interaction* interactions;

    interactions = new GateInteraction();
    interactions->bound1.Set(220, -15, -45); interactions->bound2.Set(360, -5, 50);
    SharedData::GetInstance()->interactionItems.push_back(interactions);
}

void SP2::loadShop()
{   
    loadFree();
    shoptemp();

    stemmieShop();

    if (Application::IsKeyPressed('L'))
    {
        SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
    }
}  

void SP2::loadWSGame()
{  
   
}  

void SP2::loadChonGame()
{    
    loadFree();

    //Chon's Lab Balls
    if (SharedData::GetInstance()->ballpickup) 
    {
        for (int i = 0; i < 5; i++)
        {
            if (SharedData::GetInstance()->interactptr->bound1 == ballbounds[i].bound1 && SharedData::GetInstance()->interactptr->bound2 == ballbounds[i].bound2)
                ball[i] = true;
        }

       // SharedData::GetInstance()->ballpickup = false;
    }
    else
    {
        ballfloat = ballfloat1 = ballfloat2 = ballfloat3 = ballfloat4 = 0;
    }

    if (ball[0] == true)
    {
        modelStack.PushMatrix();
        modelStack.Translate(483, 15 + ballfloat, -469);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREYELLOW], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(410, 15, -396);    //425
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLACK], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(425, 15, -380);    //
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREWHITE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(339, 15, -465);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLUE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(310, 15, -364);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHERERED], true);
        modelStack.PopMatrix();

        if (ballfloat >= 15)
        {
            ball[0] = false;
            SharedData::GetInstance()->ballpickup = false;
        }
        
    }
    else if (ball[1] == true)
    {
        modelStack.PushMatrix();
        modelStack.Translate(410, 15 + ballfloat1, -396);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLACK], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(483, 15, -469);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREYELLOW], true);
        modelStack.PopMatrix();


        modelStack.PushMatrix();
        modelStack.Translate(425, 15, -380);    //
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREWHITE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(339, 15, -465);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLUE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(310, 15, -364);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHERERED], true);
        modelStack.PopMatrix();

        if (ballfloat1 >= 15)
        {
            ball[1] = false;
            SharedData::GetInstance()->ballpickup = false;
        }
    }
    else if (ball[2] == true)
    {
        modelStack.PushMatrix();
        modelStack.Translate(425, 15 + ballfloat2, -380);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREWHITE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(483, 15, -469);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREYELLOW], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(410, 15, -396);    //425
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLACK], true);
        modelStack.PopMatrix();


        modelStack.PushMatrix();
        modelStack.Translate(339, 15, -465);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLUE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(310, 15, -364);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHERERED], true);
        modelStack.PopMatrix();

        if (ballfloat2 >= 15)
        {
            ball[2] = false;
            SharedData::GetInstance()->ballpickup = false;
        }
    }
    else if (ball[3] == true)
    {
        modelStack.PushMatrix();
        modelStack.Translate(339, 15 + ballfloat3, -465);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLUE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(483, 15, -469);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREYELLOW], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(410, 15, -396);    //425
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLACK], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(425, 15, -380);    //
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREWHITE], true);
        modelStack.PopMatrix();


        modelStack.PushMatrix();
        modelStack.Translate(310, 15, -364);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHERERED], true);
        modelStack.PopMatrix();

        if (ballfloat3 >= 15)
        {
            ball[3] = false;
            SharedData::GetInstance()->ballpickup = false;
        }
    }
    else if (ball[4] == true)
    {
        modelStack.PushMatrix();
        modelStack.Translate(310, 15 + ballfloat4, -364);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHERERED], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(483, 15, -469);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREYELLOW], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(410, 15, -396);    //425
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLACK], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(425, 15, -380);    //
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREWHITE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(339, 15, -465);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLUE], true);
        modelStack.PopMatrix();

     

        if (ballfloat4 >= 15)
        {
            ball[4] = false;
            SharedData::GetInstance()->ballpickup = false;
        }
    }
    else
    {
        modelStack.PushMatrix();
        modelStack.Translate(483, 15, -469);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREYELLOW], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(410, 15, -396);    //425
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLACK], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(425, 15, -380);    //
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREWHITE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(339, 15, -465);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHEREBLUE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(310, 15, -364);
        modelStack.Scale(0.5, 0.5, 0.5);
        RenderMesh(meshList[GEO_SPHERERED], true);
        modelStack.PopMatrix();
    }
}    
void SP2::ballboundfunct()
{
    Interaction* ballinteraction;
    //check sp2.h for arraylist of colors
    ballinteraction = new chonBallInteraction();   //ball 0 (1)
    ballinteraction->bound1.Set(480, -15, -469); ballinteraction->bound2.Set(485, -5, -460);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[0].bound1 = ballinteraction->bound1;     ballbounds[0].bound2 = ballinteraction->bound2;

    ballinteraction = new chonBallInteraction();   //ball 1 (2)
    ballinteraction->bound1.Set(405, -15, -400); ballinteraction->bound2.Set(410, -5, -390);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[1].bound1 = ballinteraction->bound1;    ballbounds[1].bound2 = ballinteraction->bound2;

    ballinteraction = new chonBallInteraction();   //ball 2 (3)
    ballinteraction->bound1.Set(420, -15, -385); ballinteraction->bound2.Set(427, -5, -375);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[2].bound1 = ballinteraction->bound1;    ballbounds[2].bound2 = ballinteraction->bound2;

    ballinteraction = new chonBallInteraction();   //ball 3 (4)
    ballinteraction->bound1.Set(334, -15, -469); ballinteraction->bound2.Set(343, -5, -460);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[3].bound1 = ballinteraction->bound1;    ballbounds[3].bound2 = ballinteraction->bound2;

    ballinteraction = new chonBallInteraction();   //ball 4 (5)
    ballinteraction->bound1.Set(300, -15, -367); ballinteraction->bound2.Set(309, -5, -360);
    SharedData::GetInstance()->interactionItems.push_back(ballinteraction);
    ballbounds[4].bound1 = ballinteraction->bound1;    ballbounds[4].bound2 = ballinteraction->bound2;

}

void SP2::loadVeeGame()
{    
     
}    

void SP2::pauseGame()
{

    loadFree();

    RenderObjectOnScreen(meshList[GEO_SHOPUI], 40, 30, 50, 50);
    RenderTextOnScreen(meshList[GEO_TEXT], "PAUSED", Color(0, 0.6, 1), 5, 5, 9);
    RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 40, ptxt1, 0.3, 0.3);
    RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 40, ptxt2, 0.3, 0.3);
    RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 40, ptxt3, 0.3, 0.3);
    RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 40, ptxtexit, 0.3, 0.3);



    //RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 40, 25, 0.2, 0.2);

    if (Application::IsKeyPressed('L'))
    {
        SharedData::GetInstance()->gamestate = GAME_STATE_FREE;

        ptxt1 = 70;
        ptxt2 = 78;
        ptxt3 = 86;
        ptxtexit = 94;
    }
    
}
void SP2::pauseAnimation(double dt)
{
    

    ptxt1 -= 80 * dt;
    ptxt2 -= 80 * dt;
    ptxt3 -= 80 * dt;
    ptxtexit -= 80 * dt;

    if (ptxt1 <= 40)
        ptxt1 = 40;
    if (ptxt2 <= 32)
        ptxt2 = 32;
    if (ptxt3 <= 24)
        ptxt3 = 24;
    if (ptxtexit <= 12)
        ptxtexit = 12;
   
}

void SP2::loadRabbitGame()
{
    if (light[0].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
    }

    //RenderSkybox();
    modelStack.PushMatrix();
    modelStack.Translate(0, 12, 0);
    modelStack.Translate(SharedData::GetInstance()->player->position_.x, SharedData::GetInstance()->player->position_.y, SharedData::GetInstance()->player->position_.z);
    modelStack.Rotate(SharedData::GetInstance()->player->direction_, 0, 1, 0);
    RenderPlayer();
    modelStack.PopMatrix();

    RenderUI();

    RenderInventory();
    RenderTime();

    RenderObjectOnScreen(meshList[GEO_CROSSHAIRS], 40, 30);
    RenderObjectOnScreen(meshList[GEO_INVENTORY], 40, 2.5);
    RenderObjectOnScreen(meshList[GEO_CROSSHAIRS], 40, 30, 1, 1);
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

    //name
    RenderTextOnScreen(meshList[GEO_TEXT], SharedData::GetInstance()->dialogueProcessor.npc->getName(), color, size, 1.5f, 5.3f);
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

void SP2::RenderPlayer()
{
    modelStack.PushMatrix();
    modelStack.Scale(2, 5, 2);

    //body
    modelStack.PushMatrix();
    modelStack.Scale(1.5f, 2.f, 0.7f);
    RenderMesh(meshList[GEO_PLAYER_BODY], true);
    modelStack.PopMatrix();

    //head
    modelStack.PushMatrix();
    modelStack.Translate(0.f, 1.4f, 0.f);
    modelStack.Scale(1.f, 0.8f, 0.8f);
    RenderMesh(meshList[GEO_PLAYER_HEAD], true);
    modelStack.PopMatrix();

    //right leg
    modelStack.PushMatrix();
    modelStack.Translate(0.4f, -1.5f, 0.f);
	modelStack.Scale(0.6f, 1.5f, 0.5f);
	RenderMesh(meshList[GEO_PLAYER_LEG], true);
	modelStack.PopMatrix();

    //left leg
    modelStack.PushMatrix();
    modelStack.Translate(-0.4f, -1.5f, 0.f);
    modelStack.Scale(0.6f, 1.5f, 0.5f);
    RenderMesh(meshList[GEO_PLAYER_LEG], true);
    modelStack.PopMatrix();

    //right arm
    modelStack.PushMatrix();
    modelStack.Translate(1.f, 0.25f, 0.f);

    modelStack.PushMatrix();
    modelStack.Scale(0.5f, 1.5f, 0.5f);
    RenderMesh(meshList[GEO_PLAYER_UPPERARM], true);
    modelStack.PopMatrix();

    //lower arm
    modelStack.PushMatrix();
    modelStack.Translate(0.f, -1.f, 0.f);
    RenderMesh(meshList[GEO_PLAYER_LOWERARM], true);
    modelStack.PopMatrix();

    modelStack.PopMatrix();

    //left arm
    modelStack.PushMatrix();
    modelStack.Translate(-1.f, 0.25f, 0.f);

    modelStack.PushMatrix();
    modelStack.Scale(0.5f, 1.5f, 0.5f);
    RenderMesh(meshList[GEO_PLAYER_UPPERARM], true);
    modelStack.PopMatrix();

    //lower arm
    modelStack.PushMatrix();
    modelStack.Translate(0.f, -1.f, 0.f);
    RenderMesh(meshList[GEO_PLAYER_LOWERARM], true);
    modelStack.PopMatrix();
    
    modelStack.PopMatrix();

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
    modelStack.Translate(950, 0, -460);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_WENGSTANG], true);
    modelStack.PopMatrix();
}

void SP2::stemmieShop()
{
    //stemmie herself
    modelStack.PushMatrix();
    modelStack.Translate(890, 15, -40);
    modelStack.Scale(4, 4, 4);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_STEMMIE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(895, 15, -45);
    modelStack.Scale(4, 4, 4);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_TEMSHOP], true);
    modelStack.PopMatrix();



    //Shop objs
    modelStack.PushMatrix();
    modelStack.Translate(935, 0, 60);
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
    modelStack.Translate(845, 0, -70);
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(830, 6, 100);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_FIREEXTINGUISHER], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(900, 0, -45);
    modelStack.Scale(18, 6, 4);
    RenderMesh(meshList[GEO_COUNTER], true);
    modelStack.PopMatrix();
}

void SP2::RenderRoom()
{
	modelStack.PushMatrix();
	modelStack.Translate(533, 4, 188);
	modelStack.Scale(3, 5, 4);
	RenderMesh(meshList[GEO_BED], true);
	modelStack.PopMatrix();
}

void SP2::chonLab()
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
    modelStack.Translate(459.2, 12, -466);
    modelStack.Scale(9, 8, 6);
    RenderMesh(meshList[GEO_TOOLBOX], true);
    modelStack.PopMatrix();
   
}

void SP2::veeControlroom()
{
    //Vee Model
    modelStack.PushMatrix();
    modelStack.Translate(600, 0, 440);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(7, 7, 7);
    RenderMesh(meshList[GEO_VEE], true);
    modelStack.PopMatrix();

    //Control Room
    modelStack.PushMatrix();
    modelStack.Translate(650, 0, 490);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(7, 7, 7);
    RenderMesh(meshList[GEO_CONTROLROOM], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(640, 0, 460);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(6, 6, 6);
    RenderMesh(meshList[GEO_CHAIR], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(510, 0, 490);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(7, 7, 7);
    RenderMesh(meshList[GEO_CONTROLROOM], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(510, 0, 460);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(6, 6, 6);
    RenderMesh(meshList[GEO_CHAIR], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(675, 0, 435);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_SHELF], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(480, 0, 435);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_SHELF], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(480, 0, 390);
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(670, 0, 390);
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(550, 3, 370);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_UMBRELLASTAND], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(620, 3, 360);
    modelStack.Scale(3, 3, 3);
    RenderMesh(meshList[GEO_FIREEXTINGUISHER], true);
    modelStack.PopMatrix();
}

void SP2::jasimCanteen()
{
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
        modelStack.Translate(740, 0, -515 + i);
        modelStack.Scale(7, 7, 7);
        modelStack.Rotate(-90, 0, 1, 0);
        RenderMesh(meshList[GEO_BENCH], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(740, 0, -500 + i);
        modelStack.Scale(7, 7, 7);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_TABLE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(740, 0, -485 + i);
        modelStack.Scale(7, 7, 7);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_BENCH], true);
        modelStack.PopMatrix();
    }

    modelStack.PushMatrix();
    modelStack.Translate(685, 0, -375);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_VENDINGMACHINE], true);
    modelStack.PopMatrix();


    modelStack.PushMatrix();
    modelStack.Translate(819, 0, -350);
    modelStack.Scale(6, 6, 6);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(819, 0, -515);
    modelStack.Scale(6, 6, 6);
    RenderMesh(meshList[GEO_PLANT], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(685, 0, -475);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_FRIDGE], true);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(780, 0, -615);
    modelStack.Scale(30, 30, 30);
    modelStack.Rotate(180, 0, 1, 0);
    RenderMesh(meshList[GEO_TOILBOWL], true);

    modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(732, floodlevel, -594);
	modelStack.Scale(7, 5, 4.3);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_TOILETFLOOD], true);
	modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(779.6, 7.39, -614.9);
    modelStack.Scale(1.11, 1, 1.11);
    modelStack.Rotate(-90, 1, 0, 0);
    RenderMesh(meshList[GEO_LMAO], true);
    modelStack.PopMatrix();
}

void SP2::loadHangar()
{
    for (int i = -175; i < 176; i += 350)
    {
        modelStack.PushMatrix();
        modelStack.Translate(75, 0, i);
        modelStack.Scale(5, 8, 5);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_TABLE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(115, 0, i);
        modelStack.Scale(5, 8, 5);
        modelStack.Rotate(90, 0, 1, 0);
        RenderMesh(meshList[GEO_TABLE], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(155, 0, i);
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

void SP2::renderFarm()
{
	for (int i = 0; i < 11; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(835.5 + (i * 19), 0, -530);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(845 + (i * 19), 0, -530);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 13; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(1039.75, 0, -525.25 + (i * 19));
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(1039.75, 0, -515.75 + (i * 19));
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
		modelStack.Translate(835.5 + (i * 19), 0, -273.5);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(845 + (i * 19), 0, -273.5);
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
			modelStack.Translate(875 + (i * 20), 1, -310 - (j * 20));
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

	for (int i = 0; i < weedgame.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(SharedData::GetInstance()->interactionItems[i + 26]->bound1.x + 3, 2, SharedData::GetInstance()->interactionItems[i + 26]->bound1.z + 3);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		RenderMesh(meshList[GEO_WEED], false);
		modelStack.PopMatrix(); // teleporter
	}
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
	//modelStack.Rotate(rotator, 0, 1, 0);
	modelStack.Rotate(-75, 1, 0, 1);
	modelStack.Scale(1.2, 1.2, 1.2);
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
	modelStack.Scale(1.2, 1.2, 1.2);
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
	modelStack.Scale(1.3, 1.3, 1.3);
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
				RenderInventoryOnScreenStatic(meshList[modelmap.find(SharedData::GetInstance()->player->inventory[i])->second], 22.5 + (i * 5), 2.5);
			}
			else
			{
				//RenderInventoryOnScreenRotating(meshList[GEO_HAMMER], 22.5 + (i * 5), 2.5);
				RenderInventoryOnScreenRotating(meshList[modelmap.find(SharedData::GetInstance()->player->inventory[i])->second], 22.5 + (i * 5), 2.5);
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
	if (delayer < 1)
	{
		if (Application::IsKeyPressed('V'))
			SharedData::GetInstance()->player->addItem(1);
		if (Application::IsKeyPressed('B'))
			SharedData::GetInstance()->player->addItem(2);
		if ((Application::IsKeyPressed('Z')) && (SharedData::GetInstance()->player->invselect > 0))
		{
			SharedData::GetInstance()->player->invselect -= 1;
			rotator = 0;
		}
		if (Application::IsKeyPressed('X'))
		{
			SharedData::GetInstance()->player->removeItem(SharedData::GetInstance()->player->invselect);
			rotator = 0;
		}
		if ((Application::IsKeyPressed('C')) && (SharedData::GetInstance()->player->invselect < 7))
		{
			SharedData::GetInstance()->player->invselect += 1;
			rotator = 0;
		}
		delayer = 3;
	}

	RenderObjectOnScreen(meshList[GEO_ITEMSELECT], 22.5 + (SharedData::GetInstance()->player->invselect * 5), 2.5);
	if (SharedData::GetInstance()->player->inventory[SharedData::GetInstance()->player->invselect] != 0)
	RenderTextOnScreen(meshList[GEO_TEXT], (invmap.find(SharedData::GetInstance()->player->inventory[SharedData::GetInstance()->player->invselect])->second).getName(), Color(1, 1, 0), 3, 10, 2);
}

void SP2::RenderTime()
{
	std::stringstream timey;
	timey << "TIME: " << ((int)SharedData::GetInstance()->daynighttime / 1000) << (((int)SharedData::GetInstance()->daynighttime / 100) % 10) << (((int)SharedData::GetInstance()->daynighttime / 10) % 10) << ((int)SharedData::GetInstance()->daynighttime % 10);
	timey.str();
	RenderTextOnScreen(meshList[GEO_TEXT], timey.str(), Color(1, 0, 0), 3, 0, 15);
	RenderTimeOnScreen(meshList[GEO_DAYNIGHTICON], 80, 60);
}

void SP2::RenderUI()
{
    if (viewOptions) {
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

		RenderObjectOnScreen(meshList[GEO_HUNGER_BAR], 23, 7, 1 + (SharedData::GetInstance()->player->getHunger() / 3), 1);

        //practical purposes: check position of object
        s.str("");
        s << "objpos:(" << gate1pos << " , " << gate2pos << ")";
        RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 3, 0, 16);
    }
}

void SP2::RenderMinimap()
{
    minimappp.LoadIdentity();
    minimappp.LookAt(minimappy.pos.x, minimappy.pos.y, minimappy.pos.z, minimappy.targ.x, minimappy.targ.y, minimappy.targ.z, minimappy.UP.x, minimappy.UP.y, minimappy.UP.z);

    if (!meshList[GEO_MAP]->textureID <= 0)  //error check
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
    
    modelStack.Translate(15, 15, 0);
    RenderMesh(meshList[GEO_MAP], false);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SP2::RenderGates()
{

    if (gate1pos <= -45)
    {
        gate1pos = 143;
    }
    if (gate2pos <= -45)
    {
        gate2pos = 143;
    }


    modelStack.PushMatrix();
    modelStack.Translate(290, gate1pos, 3);
    modelStack.Scale(3, 95, 95);
    RenderMesh(meshList[GEO_GATE1], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(290, gate2pos, 3);
    modelStack.Scale(3, 95, 95);
    RenderMesh(meshList[GEO_GATE2], false);
    modelStack.PopMatrix();

}

void SP2::shoptemp()
{
  //  if (Application::IsKeyPressed('H'))
    //{
        RenderObjectOnScreen(meshList[GEO_SHOPUI], 40, 30, 50, 50);

        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 45, 0.2, 0.2);
        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 40, 0.2, 0.2);
        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 35, 0.2, 0.2);
        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 30, 0.2, 0.2);
        RenderObjectOnScreen(meshList[GEO_DIALOGUEBOX], 25, 25, 0.2, 0.2);
        
        RenderDialogueOnScreen("Buy this item?", Color(1, 1, 1), 3);


        

        RotateDisplay();

    //}
       
        //RenderObjectOnScreen(meshList[GEO_STEMMIE], objx,objy,2,2);
  
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
            modelStack.PushMatrix();
                modelStack.Translate(5, 18, 0);
                modelStack.Rotate(-90, 0, 1, 0);
                modelStack.Rotate(0, 0, 0, 1);
                modelStack.Scale(2, 2, 2);
                RenderMesh(meshList[GEO_STEMMIE], true);
            modelStack.PopMatrix();
            projectionStack.PopMatrix();
            viewStack.PopMatrix();
        modelStack.PopMatrix();
        glEnable(GL_DEPTH_TEST);
    modelStack.PopMatrix();
}

void SP2::renderPuzzle()
{
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