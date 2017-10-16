#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Sneha Vaswani - sxv6374@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);



	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		// start a loop for the sides the sphere will hit 
		// starting at point 2 
		// starting at i or 1 does a weird glitch ??
		sides.push_back(2.0);

		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager

		// push back a vector3
		points.push_back(std::vector<vector3>());

		// loop through
		// push a vector of the point
		for (uint k = 0; k < i; k++) {
			// angle for the rotation of the sphere depending on how many sides there are
			float angle = 3.1415f * 2 / i;

			float xPoint = cosf(angle * k);
			float yPoint = sinf(angle * k);
			points[i - uSides].push_back(vector3(xPoint, yPoint, 0) * fSize);
		}

		// made fSize a little bigger when incremented to make it easier to see
		fSize += 0.7f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/



	// we need a timer to be able to lerp between sides and verticies 
	// make a brand new timer like from the lerp ex. Then generate a clock for it and get its delta time 
	static float timer = 0;
	static uint clock = m_pSystem->GenClock();
	float deltaTime = m_pSystem->GetDeltaTime(clock);
	timer = timer + deltaTime;
	// time from one sphere at a point to the next 
	float time = .8f;
	// if this timer has gone past the amount of time allocated between stops then reset it to itself
	if (timer > time) {
		timer = 0.0f;
		// loop through each point
		for (uint i = 0; i < sides.size(); i++) {
			sides[i] = sides[i] + 1;
			// go back if needed if all the points are hit
			if (sides[i] + 1 > points[i].size()) {
				sides[i] = sides[i] - sides[i]; // go to the start again
			}
		}
	}

	m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);

	// draw a shapes
	// i will keep everything in the range of the orbits
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));

		//calculate the current position
		vector3 currentPos = ZERO_V3;
		float timeChange = timer / time;
		// stops for lerping if the sides are  > 0
		vector3 stopA = points[i][sides[i] - 1];
		vector3 stopB = points[i][sides[i]];
		// stops for lerping if sides = 0
		vector3 specialStopA = points[i][points[i].size() - 1];
		vector3 specialStopB = points[i][sides[i]];
		// lerping here
		// if not 0 go to the sides
		if (sides[i] > 0) {
			// lerping here
			// from the first point to the next point based off of the side between it
			currentPos = glm::lerp(stopA, stopB, timeChange);
		}
		// if 0 go to to the point before it 
		else {
			// lerping here
			currentPos = glm::lerp(specialStopA, specialStopB, timeChange);
		}

		//matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);
		matrix4 m4Model = glm::translate(m4Offset, currentPos);

		//draw spheres
		// made sphere a little bigger and magenta colored
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.4)), C_MAGENTA);
	}

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
	//release GUI
	ShutdownGUI();
}