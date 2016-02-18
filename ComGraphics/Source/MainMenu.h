#include "Scene.h"
#include "Mesh.h"
#include "Mtx44.h"
#include "MatrixStack.h"

#include "Light.h"
#include "Material.h"

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

class MainMenu : public Scene
{
    enum MENU_STATE
    {
        MENU_MAIN = 0,
        MENU_PLAYNEW,
        MENU_LOAD,
        MENU_INSTRUCTIONS,
        MENU_OPTIONS,
        MENU_CREDITS,
        MENU_EXIT,

        MENU_TOTAL
    };

    enum GEO_MENU
    {
        GEO_BG,

        GEO_BUTTON, GEO_BUTTONHOVER, GEO_BUTTONSELECTED,
        GEO_PLAYBUTTON, GEO_PLAYBUTTONHOVER, GEO_PLAYBUTTONSELECTED,
        GEO_BUTTONRED, GEO_BUTTONREDHOVER,

        GEO_MOUSECUSTOM,
        //GEO_SELECTED,
        GEO_ICON,
        GEO_TEXT,

        GEO_TOTAL
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

        U_COLOR_TEXTURE_ENABLED,
        U_COLOR_TEXTURE,
        U_TEXT_ENABLED,
        U_TEXT_COLOR,

        U_TOTAL,
    };


public:
    MainMenu();
    ~MainMenu();

    void Init();
    void Update(double dt);
    void Render();
    void Exit();

    unsigned m_vertexArrayID;
    Mesh *meshList[GEO_TOTAL];
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    MS modelStack, viewStack, projectionStack;
    void ButtonUpdater(double dt);
    void RenderMesh(Mesh* mesh, bool enableLight);
    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderCursor(Mesh* mesh, std::string text, Color color, float size, float xbtn, float ybtn, float xtxt, float ytxt);
    void RenderButtonsOnScreen(Mesh* mesh, std::string text, Color color, float size, float xbtn, float ybtn, float xtxt, float ytxt);
    void RenderUI();

    void MainMenuPage();
    void HelpPage();
    void OptionsPage();
    void CreditsPage();

    bool clicked;
    bool isClicked;

    int btncheck;
    /*main	= 0
    play	= 1
    load	= 2
    help	= 3
    options = 4
    credits = 5
    exit	= 6*/

    MENU_STATE state;

    float delaytime;

    float helpBtnspd;
    float optBtnspd;
    float credBtnspd;

    float objx, objy;
};

#endif
