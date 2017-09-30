#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//init the mesh
	//meshes = new MyMesh();
	//m_pMesh->GenerateCube(0.25f, C_BLACK);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);


	String r8[11] = { "n", "n", "y", "n", "n", "n", "n", "n", "y", "n", "n" };
	String r7[11] = { "n", "n", "n", "y", "n", "n", "n", "y", "n", "n", "n" };
	String r6[11] = { "n", "n", "y", "y", "y", "y", "y", "y", "y", "n", "n" };
	String r5[11] = { "n", "y", "y", "n", "y", "y", "y", "n", "y", "y", "n" };
	String r4[11] = { "y", "y", "y", "y", "y", "y", "y", "y", "y", "y", "y" };
	String r3[11] = { "y", "n", "y", "y", "y", "y", "y", "y", "y", "n", "y" };
	String r2[11] = { "y", "n", "y", "y", "y", "y", "y", "y", "y", "n", "y" };
	String r1[11] = { "n", "n", "n", "y", "y", "n", "y", "y", "n", "n", "n" };

	for (int i = 0; i < 11; i++) {
		if (r1[i] == "y") {
			MyMesh* mesh = new MyMesh();
			mesh->GenerateCube(1.0f, C_BLACK);
			meshes.push_back(mesh);
			translation.push_back(vector3(i, 1, 0));
		}
		if (r2[i] == "y") {
			MyMesh* mesh = new MyMesh();
			mesh->GenerateCube(1.0f, C_BLACK);
			meshes.push_back(mesh);
			translation.push_back(vector3(i, 2, 0));
		}
		if (r3[i] == "y") {
			MyMesh* mesh = new MyMesh();
			mesh->GenerateCube(1.0f, C_BLACK);
			meshes.push_back(mesh);
			translation.push_back(vector3(i, 3, 0));
		}
		if (r4[i] == "y") {
			MyMesh* mesh = new MyMesh();
			mesh->GenerateCube(1.0f, C_BLACK);
			meshes.push_back(mesh);
			translation.push_back(vector3(i, 4, 0));
		}
		if (r5[i] == "y") {
			MyMesh* mesh = new MyMesh();
			mesh->GenerateCube(1.0f, C_BLACK);
			meshes.push_back(mesh);
			translation.push_back(vector3(i, 5, 0));
		}
		if (r6[i] == "y") {
			MyMesh* mesh = new MyMesh();
			mesh->GenerateCube(1.0f, C_BLACK);
			meshes.push_back(mesh);
			translation.push_back(vector3(i, 6, 0));
		}
		if (r7[i] == "y") {
			MyMesh* mesh = new MyMesh();
			mesh->GenerateCube(1.0f, C_BLACK);
			meshes.push_back(mesh);
			translation.push_back(vector3(i, 7, 0));
		}
		if (r8[i] == "y") {
			MyMesh* mesh = new MyMesh();
			mesh->GenerateCube(1.0f, C_BLACK);
			meshes.push_back(mesh);
			translation.push_back(vector3(i, 8, 0));
		}

	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	//matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	//static float value = 0.0f;
	//matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	//value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	//matrix4 m4Model = m4Scale * m4Translate;
	static float value = 0.0f;
	value += 0.01f;


	for (int j = 0; j < 51; j++) {
		matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f, 2.0f, 2.0f));
		matrix4 m4Translate = glm::translate(IDENTITY_M4, translation[j] + vector3(value, 0, 0));
		matrix4 m4Model = m4Scale * m4Translate;
		meshes[j]->Render(m4Projection, m4View, m4Model);
	}
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//SafeDelete(mesh);

	//release GUI
	ShutdownGUI();
}