#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//Allocate the memory for the Meshes
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BLACK);
		
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Calculate the model, view and projection matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	static float displacement = 0;
	displacement += 0.01;

	//top row
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-3 + displacement, 4, 0)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(3 + displacement, 4, 0)));

	//second row
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-2 + displacement, 3, 0)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(2 + displacement, 3, 0)));

	//third
	for (int i = -3; i <= 3; i++)
	{
		m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(i + displacement, 2, 0)));
	}

	//fourth
	for (int i = -4; i <= 4; i++)
	{
		if (i == 2 || i == -2) continue;
		m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(i + displacement, 1, 0)));
	}

	//fifth
	for (int i = -5; i <= 5; i++)
	{
		m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(i + displacement, 0, 0)));
	}

	//sixth
	for (int i = -5; i <= 5; i++)
	{
		if (i == 4 || i == -4) continue;
		m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(i + displacement, -1, 0)));
	}

	//seventh
	for (int i = -5; i <= 5; i++)
	{
		if(i == -5 || i == 5 || i == -3 || i == 3)
			m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(i + displacement, -2, 0)));
	}

	//eighth 
	for (int i = -2; i <= 2; i++)
	{
		if (i == 0) continue;
		m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(i + displacement, -3, 0)));
	}
	




	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release meshes
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}