/******************************************************************************/
/*!
\file	MainMenu.cpp
\author 3000 Lines Studio
\brief
Main Menu scene
*/
/******************************************************************************/

#include "MainMenu.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"

#include "Application.h"
#include "SharedData.h"

/******************************************************************************/
/*!
\brief	MainMenu default constructor
*/
/******************************************************************************/
MainMenu::MainMenu()
{
    glClearColor(0.f, 0.f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    //Get a handle for our "colorTexture" uniform
    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
    //Get a handle for our "textColor" uniform
    m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
    m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
    // Use our shader
    glUseProgram(m_programID);

    meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image/Text/small fonts.tga");

    meshList[GEO_LOADTOP] = MeshBuilder::GenerateQuad("load_top", Color(1,1,1), 80, 70);
    meshList[GEO_LOADTOP]->textureID = LoadTGA("Image/Text/loadTop.tga");
    meshList[GEO_LOADBTM] = MeshBuilder::GenerateQuad("load_btm", Color(1, 1, 1), 80, 70);
    meshList[GEO_LOADBTM]->textureID = LoadTGA("Image/Text/loadBtm.tga");

    meshList[GEO_BUTTON] = MeshBuilder::GenerateQuad("menu_btn", Color(0, 0.7, 1), 14, 5);
    meshList[GEO_BUTTON]->textureID = LoadTGA("Image/Text/dialogue box.tga");
    meshList[GEO_BUTTONHOVER] = MeshBuilder::GenerateQuad("menu_btnhover", Color(0, 0.2, 1), 18, 5);
    meshList[GEO_BUTTONHOVER]->textureID = LoadTGA("Image/Text/dialogue box.tga");
    meshList[GEO_BUTTONSELECTED] = MeshBuilder::GenerateQuad("menu_btnselect", Color(1, 1, 1), 16, 4);
    meshList[GEO_BUTTONSELECTED]->textureID = LoadTGA("Image/Text/dialogue box.tga");

    meshList[GEO_BUTTONRED] = MeshBuilder::GenerateQuad("menu_exit", Color(1, 0.2, 0), 14, 5);
    meshList[GEO_BUTTONREDHOVER] = MeshBuilder::GenerateQuad("menu_exithover", Color(1, 0.2, 0), 18, 5);

    meshList[GEO_PLAYBUTTON] = MeshBuilder::GenerateQuad("play_btn", Color(0, 1, 0.5), 14, 14);
    meshList[GEO_PLAYBUTTONHOVER] = MeshBuilder::GenerateQuad("play_btnhover", Color(0, 0.8, 0), 14, 14);
    meshList[GEO_PLAYBUTTONSELECTED] = MeshBuilder::GenerateQuad("play_btnselect", Color(1, 1, 1), 14, 14);
    //meshList[GEO_BUTTON]->textureID = LoadTGA("Image/Text/dialogue box.tga");

    meshList[GEO_BIGDIALOGUE] = MeshBuilder::GenerateQuad("big_diag", Color(0, 0, 0), 55, 55);
    meshList[GEO_BIGDIALOGUE]->textureID = LoadTGA("Image/layout/Inner_walls.tga");

    meshList[GEO_BG] = MeshBuilder::GenerateQuad("menu_bg", Color(0, 0, 0), 80, 60);
    meshList[GEO_BG]->textureID = LoadTGA("Image/layout/spGateTGA.tga");

    meshList[GEO_MOUSECUSTOM] = MeshBuilder::GenerateQuad("mouse_custom", Color(0, 0, 0), 3, 4);
    meshList[GEO_MOUSECUSTOM]->textureID = LoadTGA("Image/mouse.tga");

    btncheck = 0;
    helpBtnspd = optBtnspd = credBtnspd = 0;
    delaytime = 0;
    state = MENU_MAIN;

    loadDown = 60;
    loadUp = 0;

    elapsedTime = 0;
    bufferTime = 0.125;

    clicked = isClicked = selected = false;
}

/******************************************************************************/
/*!
\brief	MainMenu default destructor
*/
/******************************************************************************/
MainMenu::~MainMenu()
{

}

void MainMenu::Init()
{
    b_Play.Set(0.397f, 0.571f, 0.435f, 0.667f);
    b_Help.Set(0.f, 0.152f, 0.351f, 0.444f);
    b_Options.Set(0.f, 0.152f, 0.476f, 0.556f);
    b_Credits.Set(0.f, 0.152f, 0.588f, 0.68f);
    b_Exit.Set(0.f, 0.152f, 0.752f, 0.847f);
    b_MusicOn.Set(0.458f, 0.503f, 0.292f, 0.329f);
    b_MusicOff.Set(0.534f, 0.591f, 0.292f, 0.329f);
    b_SoundOn.Set(0.458f, 0.503f, 0.366f, 0.403f);
    b_SoundOff.Set(0.534f, 0.591f, 0.366f, 0.403f);
}

void MainMenu::Update(double dt)
{
    if (Application::IsKeyPressed(VK_LBUTTON)) {

        switch (state)
        {
        case MENU_MAIN:
            elapsedTime += dt;

            //PLAY BUTTON
            if (SharedData::GetInstance()->cursor_newxpos >= b_Play.minX && SharedData::GetInstance()->cursor_newxpos <= b_Play.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Play.minY && SharedData::GetInstance()->cursor_newypos <= b_Play.maxY) {
                if (!selected) {
                    isClicked = true;
                    btncheck = 1;
                }
                else if (isClicked && btncheck == 1) {
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }

            //HELP BUTTON
            else if (SharedData::GetInstance()->cursor_newxpos >= b_Help.minX && SharedData::GetInstance()->cursor_newxpos <= b_Help.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Help.minY && SharedData::GetInstance()->cursor_newypos <= b_Help.maxY) {
                if (!selected)
                {
                    isClicked = true;
                    btncheck = 3;
                }
                else if (isClicked && btncheck == 3) {
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }

            //OPTION BUTTON
            else if (SharedData::GetInstance()->cursor_newxpos >= b_Options.minX && SharedData::GetInstance()->cursor_newxpos <= b_Options.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Options.minY && SharedData::GetInstance()->cursor_newypos <= b_Options.maxY) {
                if (!selected && Application::IsKeyPressed(VK_LBUTTON)) {
                    isClicked = true;
                    btncheck = 4;
                }
                else if (isClicked && btncheck == 4) {
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }

            //CREDITS BUTTON
            else if (SharedData::GetInstance()->cursor_newxpos >= b_Credits.minX && SharedData::GetInstance()->cursor_newxpos <= b_Credits.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Credits.minY && SharedData::GetInstance()->cursor_newypos <= b_Credits.maxY) {
                if (!selected && Application::IsKeyPressed(VK_LBUTTON))
                {
                    isClicked = true;
                    btncheck = 5;
                }
                else if (isClicked && btncheck == 5) {
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }

            //EXIT BUTTON
            else if (SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY) {
                if (!selected && Application::IsKeyPressed(VK_LBUTTON) && elapsedTime >= bufferTime)
                {
                    isClicked = true;
                    btncheck = 6;
                }
                else if (isClicked && btncheck == 6) {
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }

            break;

        case MENU_INSTRUCTIONS:
            //back button
            if (SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY) {
                if (SharedData::GetInstance()->playSound) {
                    SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                }
                state = MENU_MAIN;
                elapsedTime = 0;
            }

            break;

        case MENU_OPTIONS:
            //music on button
            if (SharedData::GetInstance()->cursor_newxpos >= b_MusicOn.minX && SharedData::GetInstance()->cursor_newxpos <= b_MusicOn.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_MusicOn.minY && SharedData::GetInstance()->cursor_newypos <= b_MusicOn.maxY) {
                if (!SharedData::GetInstance()->playMusic) {    //music was switched off
                    if (SharedData::GetInstance()->playSound) {     //clicking sound effect
                        SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                    }
                    SharedData::GetInstance()->engine->play2D("Sound/main menu bgm.XM", true);
                    SharedData::GetInstance()->playMusic = true;
                    elapsedTime = 0;
                }
            }

            //music off button
            if (SharedData::GetInstance()->cursor_newxpos >= b_MusicOff.minX && SharedData::GetInstance()->cursor_newxpos <= b_MusicOff.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_MusicOff.minY && SharedData::GetInstance()->cursor_newypos <= b_MusicOff.maxY) {
                if (SharedData::GetInstance()->playMusic) {     //music was switched on
                    SharedData::GetInstance()->engine->stopAllSounds();
                    if (SharedData::GetInstance()->playSound) {     //clicking sound effect
                        SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                    }
                    SharedData::GetInstance()->playMusic = false;
                    elapsedTime = 0;
                }
            }

            //sound on button
            if (SharedData::GetInstance()->cursor_newxpos >= b_SoundOn.minX && SharedData::GetInstance()->cursor_newxpos <= b_SoundOn.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_SoundOn.minY && SharedData::GetInstance()->cursor_newypos <= b_SoundOn.maxY) {
                if (!SharedData::GetInstance()->playSound) {    //sound was switched off at first
                    SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                    SharedData::GetInstance()->playSound = true;
                    elapsedTime = 0;
                }
            }

            //sound off button
            if (SharedData::GetInstance()->cursor_newxpos >= b_SoundOff.minX && SharedData::GetInstance()->cursor_newxpos <= b_SoundOff.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_SoundOff.minY && SharedData::GetInstance()->cursor_newypos <= b_SoundOff.maxY) {
                if (SharedData::GetInstance()->playSound) {     //sound was switched on at first
                    SharedData::GetInstance()->playSound = false;
                    elapsedTime = 0;
                }
            }

            //back button
            if (SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY) {
                if (SharedData::GetInstance()->playSound) {
                    SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                }
                state = MENU_MAIN;
                elapsedTime = 0;
            }
            break;

        case MENU_CREDITS:
            //back button
            if (SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY) {
                if (SharedData::GetInstance()->playSound) {
                    SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                }
                state = MENU_MAIN;
                elapsedTime = 0;
            }
            break;

        }

    }

    else {
        if (state == MENU_MAIN) {
            elapsedTime += dt;

            //PLAY BUTTON
            if (SharedData::GetInstance()->cursor_newxpos >= b_Play.minX && SharedData::GetInstance()->cursor_newxpos <= b_Play.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Play.minY && SharedData::GetInstance()->cursor_newypos <= b_Play.maxY) {
                if (isClicked && btncheck == 1) {
                    if (SharedData::GetInstance()->playSound) {
                        SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                    }
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }

            //HELP BUTTON
            else if (SharedData::GetInstance()->cursor_newxpos >= b_Help.minX && SharedData::GetInstance()->cursor_newxpos <= b_Help.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Help.minY && SharedData::GetInstance()->cursor_newypos <= b_Help.maxY) {
                if (isClicked && btncheck == 3) {
                    if (SharedData::GetInstance()->playSound) {
                        SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                    }
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }

            //OPTION BUTTON
            else if (SharedData::GetInstance()->cursor_newxpos >= b_Options.minX && SharedData::GetInstance()->cursor_newxpos <= b_Options.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Options.minY && SharedData::GetInstance()->cursor_newypos <= b_Options.maxY) {
                if (isClicked && btncheck == 4) {
                    if (SharedData::GetInstance()->playSound) {
                        SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                    }
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }

            //CREDITS BUTTON
            else if (SharedData::GetInstance()->cursor_newxpos >= 0 && SharedData::GetInstance()->cursor_newxpos <= (SharedData::GetInstance()->width / 6.6)
                && SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 1.7) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 1.47)) {
                if (isClicked && btncheck == 5) {
                    if (SharedData::GetInstance()->playSound) {
                        SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                    }
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }

            //EXIT BUTTON
            else if (SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX
                && SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY) {
                if (isClicked && btncheck == 6) {
                    if (SharedData::GetInstance()->playSound) {
                        SharedData::GetInstance()->engine->play2D("Sound/button press 1.mp3");
                    }
                    clicked = true;
                    isClicked = false;
                    selected = true;
                }
            }
            else {
                if (isClicked && !selected) {
                    isClicked = false;
                    btncheck = 0;
                }
            }

        }

    }
    

    ButtonUpdater(dt);

}

void MainMenu::ButtonUpdater(double dt)
{
    //check for which button has been click'ed'
    //list for button reference on header
    if (clicked)
    {
        if (btncheck == 1)
        {
            loadDown -= 30 * dt;
            loadUp += 30 * dt;

            if (loadDown <= 30 || loadUp >= 30)
            {
                SharedData::GetInstance()->programstate_change = true;
                SharedData::GetInstance()->program_state = PROGRAM_GAME;
                selected = false;
            }
        }

        if (btncheck == 3)
        {
            helpBtnspd += (80 * dt);
            optBtnspd = 0;
            credBtnspd = 0;
        }
        else if (btncheck == 4)
        {
            optBtnspd += (80 * dt);
            helpBtnspd = 0;
            credBtnspd = 0;
        }
        else if (btncheck == 5)
        {
            credBtnspd += (80 * dt);
            optBtnspd = 0;
            helpBtnspd = 0;
        }
        else if (btncheck == 6)
        {
            SharedData::GetInstance()->program_state = PROGRAM_EXIT;
        }

    }

    /*check for button position, when button has moved a
    fixed distance,	change state*/
    if (state == MENU_MAIN)
    {
        if (helpBtnspd >= 100)
        {
            state = MENU_INSTRUCTIONS;
        }
        if (optBtnspd >= 100)
        {
            state = MENU_OPTIONS;
        }
        if (credBtnspd >= 100)
        {
            state = MENU_CREDITS;
        }
    }
}

void MainMenu::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mtx44 MVP;

    modelStack.LoadIdentity();

    RenderButtonsOnScreen(meshList[GEO_BG], " ", Color(0, 0, 0), 1, 40, 30, 1, 1);

    //check current states and render/update based on state
    switch (state)
    {
    case MENU_MAIN:MainMenuPage();
        break;
    case MENU_INSTRUCTIONS:HelpPage();
        break;
    case MENU_OPTIONS:OptionsPage();
        break;
    case MENU_CREDITS:CreditsPage();
        break;
    }

    //for testing without states, enable at will
    //MainMenuPage();
    //HelpPage();
    //OptionsPage();
    //CreditsPage();


    //custom mouse here! (o w o)b
    RenderButtonsOnScreen(meshList[GEO_MOUSECUSTOM], "", Color(0, 0, 0), 1, SharedData::GetInstance()->cursor_newxpos / (SharedData::GetInstance()->width / 80), 60 - SharedData::GetInstance()->cursor_newypos / (SharedData::GetInstance()->height / 60), 1, 1);
}

void MainMenu::RenderMesh(Mesh* mesh, bool enableLight)
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

void MainMenu::RenderButtonsOnScreen(Mesh* mesh, std::string text, Color color, float size, float xbtn, float ybtn, float xtxt, float ytxt)
{
    //if (!meshList[GEO_BUTTON] || !meshList[GEO_PLAYBUTTON]  || !meshList[GEO_BUTTONRED] )//|| meshList[GEO_BUTTON]->textureID <= 0)  //error check
    //return;

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
    modelStack.Translate(xbtn, ybtn, 0);
    RenderMesh(mesh, false);
    modelStack.PopMatrix();

    RenderTextOnScreen(meshList[GEO_TEXT], text, color, size, xtxt, ytxt);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void MainMenu::RenderText(Mesh* mesh, std::string text, Color color)
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

void MainMenu::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
        characterSpacing.SetToTranslation(i * 0.6f + 0.5f, 0.5f, 0);  //0.6f is the spacing of each character (can be changed)
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

void MainMenu::MainMenuPage()
{
    /*Each individual buttons for menu here. Seperated by comment line*/
    //PLAY BUTTON
    if (SharedData::GetInstance()->cursor_newxpos >= b_Play.minX && SharedData::GetInstance()->cursor_newxpos <= b_Play.maxX
        && SharedData::GetInstance()->cursor_newypos >= b_Play.minY && SharedData::GetInstance()->cursor_newypos <= b_Play.maxY) {
        if (isClicked)
        {
            RenderButtonsOnScreen(meshList[GEO_PLAYBUTTONSELECTED], "Play", Color(0, 0, 0), 3, 38, 27, 11.5, 8.5);
        }
        else
        {
            RenderButtonsOnScreen(meshList[GEO_PLAYBUTTONHOVER], "Play!", Color(0, 0, 0), 3, 38, 27, 11.5, 8.5);
        }
    }
    else
        RenderButtonsOnScreen(meshList[GEO_PLAYBUTTON], "Play", Color(0, 0, 0), 3, 38, 27, 11.5, 8.5);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //HELP BUTTON
    if (!clicked && SharedData::GetInstance()->cursor_newxpos >= b_Help.minX && SharedData::GetInstance()->cursor_newxpos <= b_Help.maxX
        && SharedData::GetInstance()->cursor_newypos >= b_Help.minY && SharedData::GetInstance()->cursor_newypos <= b_Help.maxY) {
        if (isClicked)
        {
            RenderButtonsOnScreen(meshList[GEO_BUTTONSELECTED], "Help", Color(1, 1, 1), 2, 5 + helpBtnspd, 36, 1 + helpBtnspd / 2, 17.4);
        }
        else
        {
            RenderButtonsOnScreen(meshList[GEO_BUTTONHOVER], "Help!", Color(1, 1, 1), 2, 5 + helpBtnspd, 36, 1 + helpBtnspd / 2, 17.4);
        }
    }
    else
        RenderButtonsOnScreen(meshList[GEO_BUTTON], "Help", Color(1, 1, 1), 2, 5 + helpBtnspd, 36, 1 + helpBtnspd / 2, 17.4);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //OPTION BUTTON
    if (!clicked && SharedData::GetInstance()->cursor_newxpos >= b_Options.minX && SharedData::GetInstance()->cursor_newxpos <= b_Options.maxX
        && SharedData::GetInstance()->cursor_newypos >= b_Options.minY && SharedData::GetInstance()->cursor_newypos <= b_Options.maxY) {
        if (isClicked)
        {
            RenderButtonsOnScreen(meshList[GEO_BUTTONSELECTED], "Options", Color(1, 1, 1), 2, 5 + optBtnspd, 29, 0.2 + optBtnspd / 2, 14);
        }
        else
        {
            RenderButtonsOnScreen(meshList[GEO_BUTTONHOVER], "Options!", Color(1, 1, 1), 2, 5 + optBtnspd, 29, 0.2 + optBtnspd / 2, 14);
        }
    }
    else
        RenderButtonsOnScreen(meshList[GEO_BUTTON], "Options", Color(1, 1, 1), 2, 5 + optBtnspd, 29, 0.2 + optBtnspd / 2, 14);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //CREDITS BUTTON
    if (!clicked && SharedData::GetInstance()->cursor_newxpos >= b_Credits.minX && SharedData::GetInstance()->cursor_newxpos <= b_Credits.maxX
        && SharedData::GetInstance()->cursor_newypos >= b_Credits.minY && SharedData::GetInstance()->cursor_newypos <= b_Credits.maxY) {
        if (isClicked)
        {
            RenderButtonsOnScreen(meshList[GEO_BUTTONSELECTED], "Credits", Color(1, 1, 1), 2, 5 + credBtnspd, 22, 0.4 + credBtnspd / 2, 10.5);
        }
        else
        {
            RenderButtonsOnScreen(meshList[GEO_BUTTONHOVER], "Credits!", Color(1, 1, 1), 2, 5 + credBtnspd, 22, 0.4 + credBtnspd / 2, 10.5);
        }
    }
    else
        RenderButtonsOnScreen(meshList[GEO_BUTTON], "Credits", Color(1, 1, 1), 2, 5 + credBtnspd, 22, 0.4 + credBtnspd / 2, 10.5);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //EXIT BUTTON
    if (!clicked && SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX
        && SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY) {
        if (isClicked)
        {
            RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Exit", Color(1, 1, 1), 2, 5, 12, 2.2, 5.4);
        }
        else
        {
            RenderButtonsOnScreen(meshList[GEO_BUTTONREDHOVER], "Exit", Color(1, 1, 1), 2, 5, 12, 2.2, 5.4);
        }
    }
    else
        RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Exit", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    RenderTextOnScreen(meshList[GEO_TEXT], " ROMANTICA", Color(1, 0.3, 1), 6, 3, 8);
    RenderTextOnScreen(meshList[GEO_TEXT], " ROOM", Color(1, 0.3, 1), 4, 8, 11);

    RenderButtonsOnScreen(meshList[GEO_LOADTOP], " ", Color(1, 1, 1), 3, 40, loadDown, 2, 2);
    RenderButtonsOnScreen(meshList[GEO_LOADBTM], " ", Color(1, 1, 1), 3, 40, loadUp, 2, 2);
}

void MainMenu::HelpPage()
{
    clicked = false;
    isClicked = false;
    selected = false;
    delaytime = 0;
    helpBtnspd = 0;
    optBtnspd = 0;
    credBtnspd = 0;
    btncheck = 0;

    RenderButtonsOnScreen(meshList[GEO_BIGDIALOGUE], "INSTRUCTIONS", Color(0, 0, 1), 3, 40, 30, 9, 17);
    RenderTextOnScreen(meshList[GEO_TEXT], "You have crashed landed ", Color(0, 1, 0), 2, 8, 23);
    RenderTextOnScreen(meshList[GEO_TEXT], "on a Space Station!", Color(0, 1, 0), 2, 8, 21.5);
    RenderTextOnScreen(meshList[GEO_TEXT], "Make friends with the alien  ", Color(0, 0, 1), 2, 8, 20);
    RenderTextOnScreen(meshList[GEO_TEXT], "inhabitants and earn their ", Color(0, 0, 1), 2, 8, 18.5);
    RenderTextOnScreen(meshList[GEO_TEXT], "trust to repair your ship.", Color(0, 0, 1), 2, 8, 17);
    RenderTextOnScreen(meshList[GEO_TEXT], "Help the aliens with tasks", Color(0, 0, 1), 2, 8, 14);
    RenderTextOnScreen(meshList[GEO_TEXT], "by playing their games.", Color(0, 0, 1), 2, 8, 12.5);
    RenderTextOnScreen(meshList[GEO_TEXT], "You can also buy them gifts!", Color(0, 0, 1), 2, 8, 11);
    RenderTextOnScreen(meshList[GEO_TEXT], "Games earns you money to buy gifts.", Color(0, 0, 1), 2, 8, 9.5);

    RenderTextOnScreen(meshList[GEO_TEXT], "WASD:Move    E:Interact    Mouse:Camera", Color(1, 1, 0), 1.5, 14, 10);
    RenderTextOnScreen(meshList[GEO_TEXT], "SHIFT:Dash    Esc:Pause    Esc:Quit", Color(1, 1, 0), 1.5, 15, 8);

    RenderTextOnScreen(meshList[GEO_TEXT], "Beware of the rabbit at the end...!", Color(1, 0.3, 0.3), 2, 10, 3);

    if (SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX
        && SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY)
    {
        RenderButtonsOnScreen(meshList[GEO_BUTTONREDHOVER], "Back", Color(0, 0, 0), 2, 5, 12, 0.5, 5.4);
    }
    else
        RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Back", Color(0, 0, 0), 2, 5, 12, 0.5, 5.4);

}

void MainMenu::OptionsPage()
{
    clicked = false;
    isClicked = false;
    selected = false;
    delaytime = 0;
    helpBtnspd = 0;
    optBtnspd = 0;
    credBtnspd = 0;
    btncheck = 0;

    RenderButtonsOnScreen(meshList[GEO_BIGDIALOGUE], "OPTIONS", Color(0, 0, 1), 3, 40, 30, 5, 17);
    RenderTextOnScreen(meshList[GEO_TEXT], "Music", Color(0, 0, 1), 3, 6, 13.5f);
    if (SharedData::GetInstance()->playMusic) {
        RenderTextOnScreen(meshList[GEO_TEXT], "On", Color(1, 1, 1), 3, 12, 13.5f);
        RenderTextOnScreen(meshList[GEO_TEXT], "Off", Color(0, 0, 1), 3, 14, 13.5f);
    }
    else {
        RenderTextOnScreen(meshList[GEO_TEXT], "On", Color(0, 0, 1), 3, 12, 13.5f);
        RenderTextOnScreen(meshList[GEO_TEXT], "Off", Color(1, 1, 1), 3, 14, 13.5f);
    }
    
    RenderTextOnScreen(meshList[GEO_TEXT], "Sound", Color(0, 0, 1), 3, 6, 12);
    if (SharedData::GetInstance()->playSound) {
        RenderTextOnScreen(meshList[GEO_TEXT], "On", Color(1, 1, 1), 3, 12, 12);
        RenderTextOnScreen(meshList[GEO_TEXT], "Off", Color(0, 0, 1), 3, 14, 12);
    }
    else {
        RenderTextOnScreen(meshList[GEO_TEXT], "On", Color(0, 0, 1), 3, 12, 12);
        RenderTextOnScreen(meshList[GEO_TEXT], "Off", Color(1, 1, 1), 3, 14, 12);
    }

    if (SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX
        && SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY)
    {
        RenderButtonsOnScreen(meshList[GEO_BUTTONREDHOVER], "Back", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
    }
    else
        RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Back", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
}

void MainMenu::CreditsPage()
{
    clicked = false;
    isClicked = false;
    selected = false;
    delaytime = 0;
    helpBtnspd = 0;
    optBtnspd = 0;
    credBtnspd = 0;
    btncheck = 0;

    RenderButtonsOnScreen(meshList[GEO_BIGDIALOGUE], "CREDITS", Color(1, 1, 0), 3, 40, 30, 11, 17);
    RenderTextOnScreen(meshList[GEO_TEXT], "Captain : Jing Ting", Color(1, 1, 0), 2, 8, 19);
    RenderTextOnScreen(meshList[GEO_TEXT], "Gunner : Reuben", Color(1, 1, 0), 2, 8, 16);
    RenderTextOnScreen(meshList[GEO_TEXT], "Droid : Noel", Color(1, 1, 0), 2, 8, 13);
    RenderTextOnScreen(meshList[GEO_TEXT], "1stMate : Ray", Color(1, 1, 0), 2, 8, 10);


    if (SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX
        && SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY)
    {
        RenderButtonsOnScreen(meshList[GEO_BUTTONREDHOVER], "Back", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
    }
    else
        RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Back", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);

}

void MainMenu::Exit()
{
    glDeleteVertexArrays(1, &m_vertexArrayID);
    glDeleteProgram(m_programID);
}

