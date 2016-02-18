#include "MainMenu.h"
//#include "SP2.h"





MainMenu::MainMenu()
{
	glClearColor(1.0f, 0.86f, 0.79f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(0, -140, 100), Vector3(0, -140, 110), Vector3(0, 1, 0));

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

	meshList[GEO_BUTTON] = MeshBuilder::GenerateQuad("menu_btn", Color(0, 0.7, 1), 14, 5);
	meshList[GEO_BUTTONHOVER] = MeshBuilder::GenerateQuad("menu_btnhover", Color(0, 0.2, 1), 14, 5);
	meshList[GEO_BUTTONSELECTED] = MeshBuilder::GenerateQuad("menu_btnselect", Color(1, 1, 1), 14, 5);

	meshList[GEO_BUTTONRED] = MeshBuilder::GenerateQuad("menu_exit", Color(1, 0.2, 0), 14, 5);
	meshList[GEO_BUTTONREDHOVER] = MeshBuilder::GenerateQuad("menu_exithover", Color(1, 0.2, 0), 18, 5);
	
	meshList[GEO_PLAYBUTTON] = MeshBuilder::GenerateQuad("play_btn", Color(0, 1, 0.5), 14, 14);
	meshList[GEO_PLAYBUTTONHOVER] = MeshBuilder::GenerateQuad("play_btnhover", Color(0, 0.8, 0), 14, 14);
	meshList[GEO_PLAYBUTTONSELECTED] = MeshBuilder::GenerateQuad("play_btnselect", Color(1, 1, 1), 14, 14);
	//meshList[GEO_BUTTON]->textureID = LoadTGA("Image/Text/dialogue box.tga");

	meshList[GEO_MOUSECUSTOM] = MeshBuilder::GenerateQuad("mouse_custom", Color(0, 0, 0), 3, 4);
	meshList[GEO_MOUSECUSTOM]->textureID = LoadTGA("Image/mouse.tga");

	objx = objy = 0;

	btncheck = 0;
	helpBtnspd = optBtnspd = credBtnspd = 0;
	delaytime = 0;
	state = MENU_MAIN;

	clicked = isClicked = false;

}
MainMenu::~MainMenu()
{

}

void MainMenu::Init()
{
	//MENU_STATE state = MENU_MAIN;
}

void MainMenu::Update(double dt)
{
	//objx,y is for testing purpose use to determine location of anything
	//can be applied on anything at will
	if (Application::IsKeyPressed('A'))
		objx -= (3*dt);
	if (Application::IsKeyPressed('D'))
		objx += (3 * dt);
	if (Application::IsKeyPressed('W'))
		objy += (3 * dt);
	if (Application::IsKeyPressed('S'))
		objy -= (3 * dt);

	//limit cursor to the resolution of the window
	if (SharedData::GetInstance()->cursor_newxpos <= 0)
		SharedData::GetInstance()->cursor_newxpos = 0;

	if (SharedData::GetInstance()->cursor_newxpos >= 1920)
		SharedData::GetInstance()->cursor_newxpos = 1920;

	if (SharedData::GetInstance()->cursor_newypos <= 0)
		SharedData::GetInstance()->cursor_newypos = 0;

	if (SharedData::GetInstance()->cursor_newypos >= 1080)
		SharedData::GetInstance()->cursor_newypos = 1080;

	
	ButtonUpdater(dt);

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

	//Play
	//if (Application::IsKeyPressed(VK_LBUTTON))
}

void MainMenu::ButtonUpdater(double dt)
{
	//check for which button has been click'ed'
	//list for button reference on header
	if (clicked)
	{
		if (btncheck == 3)
		{
			helpBtnspd += (80 * dt);
			optBtnspd = 0;
			credBtnspd = 0;
			
			/* for 3 btn sliding
			delaytime += dt;
			if (delaytime >= 0.1)
			{
				optBtnspd += (80 * dt);
			}
			if (delaytime >= 0.2)
			{
				credBtnspd += (80 * dt);
			}*/
		}
		else if (btncheck == 4)
		{
			optBtnspd += (80 * dt);
			helpBtnspd = 0;
			credBtnspd = 0;
			/*for 3 btn sliding
			delaytime += dt;
			if (delaytime >= 0.1)
			{
				helpBtnspd += (80 * dt);
			}
			if (delaytime >= 0.2)
			{
				credBtnspd += (80 * dt);
			}*/

		}
		else if (btncheck == 5)
		{
			credBtnspd += (80 * dt);
			optBtnspd = 0;
			helpBtnspd = 0;
			/*for 3 btn sliding
			delaytime += dt;
			if (delaytime >= 0.1)
			{
				optBtnspd += (80 * dt);
			}
			if (delaytime >= 0.2)
			{
				helpBtnspd += (80 * dt);
			}*/
		}


	}

	/*check for button position, when button has moved a 
		fixed distance,	change state*/
	if (state == MENU_MAIN)
	{
		if (helpBtnspd >= 150)
		{
			state = MENU_INSTRUCTIONS;
		}
		if (optBtnspd >= 150)
		{
			state = MENU_OPTIONS;
		}
		if (credBtnspd >= 150)
		{
			state = MENU_CREDITS;
		}
	}
}

void MainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 MVP;

	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

	modelStack.LoadIdentity();

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
	RenderButtonsOnScreen(meshList[GEO_MOUSECUSTOM], "", Color(0, 0, 0), 1, SharedData::GetInstance()->cursor_newxpos / 24, 60 - SharedData::GetInstance()->cursor_newypos / 18, 1, 1);
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

void MainMenu::RenderButtonsOnScreen(Mesh* mesh,std::string text, Color color, float size, float xbtn, float ybtn, float xtxt, float ytxt)
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

void MainMenu::MainMenuPage()
{
	//RenderButtonsOnScreen("Play", Color(0, 0, 0), 2,38,43);


	/*Each individual buttons for menu here. Seperated by comment line*/
		//PLAY BUTTON
		if (SharedData::GetInstance()->cursor_newxpos >= 746 && SharedData::GetInstance()->cursor_newxpos <= 1076
			&& SharedData::GetInstance()->cursor_newypos >= 470 && SharedData::GetInstance()->cursor_newypos <= 715)
		{
			if (Application::IsKeyPressed(VK_LBUTTON))
			{
				RenderButtonsOnScreen(meshList[GEO_PLAYBUTTONSELECTED], "Play", Color(0, 0, 0), 3, 38, 27, 10.9, 8.5);
				//INSERT SCENE TRANSITION TO GAMEPLAY
			}
			else
			{
				RenderButtonsOnScreen(meshList[GEO_PLAYBUTTONHOVER], "Play", Color(0, 0, 0), 3, 38, 27, 10.9, 8.5);
			}
		}
		else
			RenderButtonsOnScreen(meshList[GEO_PLAYBUTTON], "Play", Color(0, 0, 0), 3, 38, 27, 10.9, 8.5);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//HELP BUTTON
		if (SharedData::GetInstance()->cursor_newxpos >= 0 && SharedData::GetInstance()->cursor_newxpos <= 290
			&& SharedData::GetInstance()->cursor_newypos >= 380 && SharedData::GetInstance()->cursor_newypos <= 475)
		{
			if (Application::IsKeyPressed(VK_LBUTTON))
			{
				RenderButtonsOnScreen(meshList[GEO_BUTTONSELECTED], "Help!", Color(0, 0, 0), 2, 5 + helpBtnspd, 36, 1 + helpBtnspd / 2, 17.4);
				isClicked = true;
			}
			else 
			{
				RenderButtonsOnScreen(meshList[GEO_BUTTONHOVER], "Help!", Color(0, 0, 0), 2, 5 + helpBtnspd, 36, 1 + helpBtnspd / 2, 17.4);
				if (isClicked && btncheck == 0)
				{
					clicked = true;
					btncheck = 3;
					//isClicked = false;
				}
			}
		}
		else
			RenderButtonsOnScreen(meshList[GEO_BUTTON], "Help!", Color(0, 0, 0), 2, 5 + helpBtnspd, 36, 1 + helpBtnspd / 2, 17.4);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//OPTION BUTTON
		if (SharedData::GetInstance()->cursor_newxpos >= 0 && SharedData::GetInstance()->cursor_newxpos <= 290
			&& SharedData::GetInstance()->cursor_newypos >= 510 && SharedData::GetInstance()->cursor_newypos <= 600)
		{	
			if (Application::IsKeyPressed(VK_LBUTTON))
			{
				RenderButtonsOnScreen(meshList[GEO_BUTTONSELECTED], "Option", Color(0, 0, 0), 2, 5 + optBtnspd, 29, 0.2 + optBtnspd / 2, 14);
				isClicked = true;
			}
			else 
			{
				RenderButtonsOnScreen(meshList[GEO_BUTTONHOVER], "Option", Color(0, 0, 0), 2, 5 + optBtnspd, 29, 0.2 + optBtnspd / 2, 14);
				if (isClicked && btncheck == 0)
				{
					clicked = true;
					btncheck = 4;
					//isClicked = false;
				}
			}
		}
		else
			RenderButtonsOnScreen(meshList[GEO_BUTTON], "Option", Color(0, 0, 0), 2, 5 + optBtnspd, 29, 0.2 + optBtnspd / 2, 14);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//CREDITS BUTTON
		if (SharedData::GetInstance()->cursor_newxpos >= 0 && SharedData::GetInstance()->cursor_newxpos <= 290
			&& SharedData::GetInstance()->cursor_newypos >= 635 && SharedData::GetInstance()->cursor_newypos <= 730)
		{
			if (Application::IsKeyPressed(VK_LBUTTON))
			{
				RenderButtonsOnScreen(meshList[GEO_BUTTONSELECTED], "Credit", Color(0, 0, 0), 2, 5 + credBtnspd, 22, 0.4 + credBtnspd / 2, 10.5);
				isClicked = true;
			}
			else 
			{
				RenderButtonsOnScreen(meshList[GEO_BUTTONHOVER], "Credit", Color(0, 0, 0), 2, 5 + credBtnspd, 22, 0.4 + credBtnspd / 2, 10.5);
				if (isClicked && btncheck == 0)
				{
					clicked = true;
					btncheck = 5;
					//isClicked = false;
				}
			}
		}
		else
			RenderButtonsOnScreen(meshList[GEO_BUTTON], "Credit", Color(0, 0, 0), 2, 5 + credBtnspd, 22, 0.4 + credBtnspd / 2, 10.5);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//EXIT BUTTON
		if (SharedData::GetInstance()->cursor_newxpos >= 0 && SharedData::GetInstance()->cursor_newxpos <= 290
			&& SharedData::GetInstance()->cursor_newypos >= 815 && SharedData::GetInstance()->cursor_newypos <= 910)
		{
			if (Application::IsKeyPressed(VK_LBUTTON))
			{
				RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Exit", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
				//EXIT PROGRAM HERE
			}
			else
			{
				RenderButtonsOnScreen(meshList[GEO_BUTTONREDHOVER], "Exit", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
			}
		}
		else
			RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Exit", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		RenderTextOnScreen(meshList[GEO_TEXT], " Title", Color(0, 0, 0), 6, 3, 8);
		RenderTextOnScreen(meshList[GEO_TEXT], "objx : " + std::to_string(SharedData::GetInstance()->cursor_newxpos), Color(0, 0, 0), 2, 1, 2);
		RenderTextOnScreen(meshList[GEO_TEXT], "objy : " + std::to_string(SharedData::GetInstance()->cursor_newypos), Color(0, 0, 0), 2, 1, 1);
	
}

void MainMenu::HelpPage()
{
	clicked = false;
	isClicked = false;
	delaytime = 0;
	helpBtnspd = 0;
	optBtnspd = 0;
	credBtnspd = 0;
	btncheck = 0;

	RenderButtonsOnScreen(meshList[GEO_PLAYBUTTONSELECTED], "INSTRUCTIONS", Color(0, 0, 0), 3, 38, 27, 10.9, 8.5);

	if (SharedData::GetInstance()->cursor_newxpos >= 0 && SharedData::GetInstance()->cursor_newxpos <= 290
		&& SharedData::GetInstance()->cursor_newypos >= 815 && SharedData::GetInstance()->cursor_newypos <= 910)
	{
		if (Application::IsKeyPressed(VK_LBUTTON))
		{
			RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Return", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
			state = MENU_MAIN;
		}
		else
			RenderButtonsOnScreen(meshList[GEO_BUTTONREDHOVER], "Return", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
	}
	else
		RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Return", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);

}

void MainMenu::OptionsPage()
{
	clicked = false;
	isClicked = false;
	delaytime = 0;
	helpBtnspd = 0;
	optBtnspd = 0;
	credBtnspd = 0;
	btncheck = 0;

	RenderButtonsOnScreen(meshList[GEO_PLAYBUTTONSELECTED], "OPTIONS", Color(0, 0, 0), 3, 38, 27, 10.9, 8.5);
	if (SharedData::GetInstance()->cursor_newxpos >= 0 && SharedData::GetInstance()->cursor_newxpos <= 290
		&& SharedData::GetInstance()->cursor_newypos >= 815 && SharedData::GetInstance()->cursor_newypos <= 910)
	{
		if (Application::IsKeyPressed(VK_LBUTTON))
		{
			RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Return", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
			state = MENU_MAIN;
			isClicked = true;
		}
		else
			RenderButtonsOnScreen(meshList[GEO_BUTTONREDHOVER], "Return", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
	}
	else
		RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Return", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);

}

void MainMenu::CreditsPage()
{
	clicked = false;
	isClicked = false;
	delaytime = 0;
	helpBtnspd = 0;
	optBtnspd = 0;
	credBtnspd = 0;
	btncheck = 0;

	RenderButtonsOnScreen(meshList[GEO_PLAYBUTTONSELECTED], "CREDITS", Color(0, 0, 0), 3, 38, 27, 10.9, 8.5);
	if (SharedData::GetInstance()->cursor_newxpos >= 0 && SharedData::GetInstance()->cursor_newxpos <= 290
		&& SharedData::GetInstance()->cursor_newypos >= 815 && SharedData::GetInstance()->cursor_newypos <= 910)
	{
		if (Application::IsKeyPressed(VK_LBUTTON))
		{
			RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Return", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
			state = MENU_MAIN;
		}
		else
			RenderButtonsOnScreen(meshList[GEO_BUTTONREDHOVER], "Return", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);
	}
	else
		RenderButtonsOnScreen(meshList[GEO_BUTTONRED], "Return", Color(0, 0, 0), 2, 5, 12, 2.2, 5.4);

}

void MainMenu::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

