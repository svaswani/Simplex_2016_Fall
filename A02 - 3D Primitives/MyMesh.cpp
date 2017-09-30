#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh point(other);
		Swap(point);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array obkect
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Obkect

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

																									   // Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProkection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader);

	//Bind the VAO of this obkect
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Prokection of the Camera
	matrix4 m4MVP = a_mProkection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));

	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other obkects
}
void MyMesh::AddTri(vector3 a_vbottomSideleftSide, vector3 a_vbottomSiderightSide, vector3 a_vtopSideleftSide)
{
	//C
	//| \
		//A--B
//This will make the triangle A->B->C 
	AddVertexPosition(a_vbottomSideleftSide);
	AddVertexPosition(a_vbottomSiderightSide);
	AddVertexPosition(a_vtopSideleftSide);
}
void MyMesh::AddQuad(vector3 a_vbottomSideleftSide, vector3 a_vbottomSiderightSide, vector3 a_vtopSideleftSide, vector3 a_vtopSiderightSide)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vbottomSideleftSide);
	AddVertexPosition(a_vbottomSiderightSide);
	AddVertexPosition(a_vtopSideleftSide);

	AddVertexPosition(a_vtopSideleftSide);
	AddVertexPosition(a_vbottomSiderightSide);
	AddVertexPosition(a_vtopSiderightSide);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float val = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-val, -val, val); //0
	vector3 point1(val, -val, val); //1
	vector3 point2(val, val, val); //2
	vector3 point3(-val, val, val); //3

	vector3 point4(-val, -val, -val); //4
	vector3 point5(val, -val, -val); //5
	vector3 point6(val, val, -val); //6
	vector3 point7(-val, val, -val); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

													   //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_radius, float a_height, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_radius < 0.01f)
		a_radius = 0.01f;

	if (a_height < 0.01f)
		a_height = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_radius * 2.0f, a_v3Color);
	// -------------------------------
	std::vector<vector3 > vert;
	vert.push_back(vector3(0.0f, -a_height / 2.0f, 0.0f));
	GLfloat angle = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		angle += (2 * PI / a_nSubdivisions); // radians
		vector3 point = vector3(cos(angle) * a_radius, -a_height / 2.0f, -sin(angle) * a_radius); // trig
		vert.push_back(point);
	}

	vert.push_back(vector3(0.0f, a_height / 2.0f, 0.0f)); // topSide point of cone

	// this creates the bottomSide parts of the cone
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddTri(vert[0], vert[i + 1], vert[i]); // adds a triangle 
	}

	AddTri(vert[0], vert[1], vert[a_nSubdivisions]);

	// this is for the side parts
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddTri(vert[i], vert[i + 1], vert[a_nSubdivisions + 1]);

	}

	AddTri(vert[a_nSubdivisions], vert[1], vert[a_nSubdivisions + 1]);

	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_radius, float a_height, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_radius < 0.01f)
		a_radius = 0.01f;

	if (a_height < 0.01f)
		a_height = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_radius * 2.0f, a_v3Color);
	// -------------------------------
	// bottomSide vertices
	std::vector<vector3 > vertexbottomSide;
	vertexbottomSide.push_back(vector3(0.0f, -a_height / 2.0f, 0.0f)); // bottomSide point
	GLfloat angle = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		angle += (2 * PI / a_nSubdivisions);
		vector3 point = vector3(cos(angle) * a_radius, -a_height / 2.0f, -sin(angle) * a_radius); // bottomSide - height
		vertexbottomSide.push_back(point);
	}

	// topSide vertices
	std::vector<vector3 > vertextopSide;
	vertextopSide.push_back(vector3(0.0f, a_height / 2.0f, 0.0f)); // topSide
	angle = 0;
	vector3 vHeight = vector3(0.0f, a_height, 0.0f);

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vertextopSide.push_back(vertexbottomSide[i + 1] + vHeight);
	}

	// bottomSide faces
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddTri(vertexbottomSide[0], vertexbottomSide[i + 1], vertexbottomSide[i]);

	}

	AddTri(vertexbottomSide[0], vertexbottomSide[1], vertexbottomSide[a_nSubdivisions]);

	// topSide faces
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddTri(vertextopSide[0], vertextopSide[i], vertextopSide[i + 1]);
	}

	AddTri(vertextopSide[0], vertextopSide[a_nSubdivisions], vertextopSide[1]);

	// sides
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddQuad(vertexbottomSide[i], vertexbottomSide[i + 1], vertextopSide[i], vertextopSide[i + 1]); // draw quad bottomSide leftSide rightSide topSide leftSide rightSide 4 params
	}

	AddQuad(vertexbottomSide[1], vertexbottomSide[a_nSubdivisions], vertextopSide[1], vertextopSide[a_nSubdivisions]); // draw quad

	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_height, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_height < 0.01f)
		a_height = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------
	std::vector<vector3 > vertTopSideOutsides;
	GLfloat angle = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		angle += (2 * PI / a_nSubdivisions);
		vector3 point = vector3(cos(angle) * a_fOuterRadius, a_height / 2.0f, -sin(angle) * a_fOuterRadius); // outside
		vertTopSideOutsides.push_back(point);
	}

	std::vector<vector3 > vertTopSideInsides;
	angle = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		angle += (2 * PI / a_nSubdivisions);
		vector3 point = vector3(cos(angle) * a_fInnerRadius, a_height / 2.0f, -sin(angle) * a_fInnerRadius); // isides
		vertTopSideInsides.push_back(point);
	}

	// bottomSide outside
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddTri(vertTopSideInsides[i], vertTopSideOutsides[i], vertTopSideOutsides[i + 1]);

	}

	AddTri(vertTopSideInsides[a_nSubdivisions - 1], vertTopSideOutsides[a_nSubdivisions - 1], vertTopSideOutsides[0]);

	// bottomSide isides
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddTri(vertTopSideOutsides[i + 1], vertTopSideInsides[i + 1], vertTopSideInsides[i]);

	}

	AddTri(vertTopSideOutsides[0], vertTopSideInsides[0], vertTopSideInsides[a_nSubdivisions - 1]);

	// bottomSide verticies 
	std::vector<vector3 > vertBottomSideOutside;
	angle = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// similar math for points here
		angle += (2 * PI / a_nSubdivisions);
		vector3 point = vector3(cos(angle) * a_fOuterRadius, -a_height / 2.0f, -sin(angle) * a_fOuterRadius);
		vertBottomSideOutside.push_back(point);
	}

	std::vector<vector3 > vertBottomSideInsides;
	angle = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		angle += static_cast<GLfloat>(2 * PI / a_nSubdivisions);
		vector3 point = vector3(cos(angle) * a_fInnerRadius, -a_height / 2.0f, -sin(angle) * a_fInnerRadius);
		vertBottomSideInsides.push_back(point);
	}

	// bottomSide outsides 
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddTri(vertBottomSideOutside[i + 1], vertBottomSideOutside[i], vertBottomSideInsides[i]);

	}

	AddTri(vertBottomSideOutside[0], vertBottomSideOutside[a_nSubdivisions - 1], vertBottomSideInsides[a_nSubdivisions - 1]);

	// bottomSide isides
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddTri(vertBottomSideInsides[i], vertBottomSideInsides[i + 1], vertBottomSideOutside[i + 1]);
	}

	AddTri(vertBottomSideInsides[a_nSubdivisions - 1], vertBottomSideInsides[0], vertBottomSideOutside[0]);

	// vertical insides parts
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddTri(vertTopSideInsides[i], vertBottomSideInsides[i + 1], vertBottomSideInsides[i]);

	}

	AddTri(vertTopSideInsides[a_nSubdivisions - 1], vertTopSideInsides[a_nSubdivisions - 1], vertBottomSideInsides[a_nSubdivisions - 1]);

	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddTri(vertTopSideInsides[i + 1], vertBottomSideInsides[i + 1], vertTopSideInsides[i]);
	}

	AddTri(vertTopSideInsides[0], vertBottomSideInsides[0], vertTopSideInsides[a_nSubdivisions - 1]);

	// vertical outside parts
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddTri(vertBottomSideOutside[i], vertBottomSideOutside[i + 1], vertTopSideOutsides[i]);
	}

	AddTri(vertBottomSideOutside[a_nSubdivisions - 1], vertBottomSideOutside[0], vertTopSideOutsides[a_nSubdivisions - 1]);

	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddTri(vertTopSideOutsides[i + 1], vertTopSideOutsides[i], vertBottomSideOutside[i + 1]);

	}

	AddTri(vertTopSideOutsides[0], vertTopSideOutsides[a_nSubdivisions - 1], vertBottomSideOutside[0]);

	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	float radius = (glm::distance(vector3(a_fOuterRadius, 0.0f, 0.0f), vector3(a_fInnerRadius, 0.0f, 0.0f))) / 2.0f;

	vector3 middle = (vector3(a_fOuterRadius, 0.0f, 0.0f) + vector3(a_fInnerRadius, 0.0f, 0.0f)) / 2.0f;

	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();
	std::vector<std::vector<vector3>> circle;
	GLfloat angle = 0;

	// Replace this with your code
	//GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------
	// top verts

	std::vector<vector3> torusPart;
	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		vector3 point = vector3(cos(angle) * radius, sin(angle) * radius, 0.0f);
		point += middle;
		torusPart.push_back(point);
		angle += (2 * PI / a_nSubdivisionsA);
	}
	circle.push_back(torusPart);
	GLfloat angle2 = 0;
	for (int numParts = 1; numParts < a_nSubdivisionsB; numParts++)
	{
		torusPart.clear();
		angle2 += (2 * PI / a_nSubdivisionsB);
		matrix4 m4rot = glm::rotate(matrix4(1.0f), glm::degrees(angle2), vector3(0.0f, 1.0f, 0.0f));
		for (int i = 0; i < a_nSubdivisionsA; i++)
		{
			torusPart.push_back(static_cast<vector3>((m4rot * vector4(circle[0][i], 1.0f))));
		}
		circle.push_back(torusPart);
	}

	for (int sides = 0; sides < a_nSubdivisionsA - 1; sides++)
	{
		for (int numParts = 0; numParts < a_nSubdivisionsB - 1; numParts++)
		{
			AddQuad(circle[numParts][sides], circle[numParts + 1][sides], circle[numParts][sides + 1], circle[numParts + 1][sides + 1]);
		}
		AddQuad(circle[a_nSubdivisionsB - 1][sides], circle[0][sides], circle[a_nSubdivisionsB - 1][sides + 1], circle[0][sides + 1]);
	}

	for (int numParts = 0; numParts < a_nSubdivisionsB - 1; numParts++)
	{
		AddQuad(circle[numParts][a_nSubdivisionsA - 1], circle[numParts + 1][a_nSubdivisionsA - 1], circle[numParts][0], circle[numParts + 1][0]);
	}
	AddQuad(circle[a_nSubdivisionsB - 1][a_nSubdivisionsA - 1], circle[0][a_nSubdivisionsA - 1], circle[a_nSubdivisionsB - 1][0], circle[0][0]);

	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_radius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_radius < 0.01f)
		a_radius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_radius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_radius * 2.0f, a_v3Color);
	// -------------------------------
	float val = 1.0f;
	vector3 point1(-val, -val, val);
	vector3 point2(val, -val, val);
	vector3 point3(-val, val, val);

	std::vector<vector3> vectorCombined;
	vectorCombined.push_back(point1);
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 point(point2 - point1);
		point /= a_nSubdivisions + 1;
		point *= (i + 1);
		vectorCombined.push_back(point + point1);
	}
	vectorCombined.push_back(point2);

	// height changes
	float height = point3.y - point1.y;
	// a list inside another list
	height /= a_nSubdivisions + 1;
	std::vector<std::vector<vector3>> list; // list of vec3
	list.push_back(vectorCombined);
	for (int k = 0; k < a_nSubdivisions + 1; k++)
	{
		std::vector<vector3> point = list[0];
		float increment = height * (k + 1);
		for (int i = 0; i < a_nSubdivisions + 2; i++)
		{
			point[i].y += increment;
		}
		list.push_back(point);
	}

	// draw all the quads and loop through each sub division
	for (int k = 0; k < a_nSubdivisions + 1; k++)
	{
		for (int i = 0; i < a_nSubdivisions + 1; i++)
		{
			AddQuad(list[k][i], list[k][i + 1], list[k + 1][i], list[k + 1][i + 1]);
		}
	}

	// normalize
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		m_lVertexPos[i] = glm::normalize(m_lVertexPos[i]);
		m_lVertexPos[i] *= a_radius;
	}

	// draw the rightSide part
	int verts = m_uVertexCount;
	std::vector<vector3> rightSide;
	for (int i = 0; i < verts; i++)
	{
		matrix4 rot;
		rot = glm::rotate(matrix4(1.0f), 90.0f, vector3(0.0f, 1.0f, 0.0f));
		rightSide.push_back(static_cast <vector3>(rot * glm::vec4(m_lVertexPos[i], 1.0f)));
	}


	for (int i = 0; i < verts; i++)
	{
		AddVertexPosition(rightSide[i]);
	}

	// draw the leftSide part
	std::vector<vector3> leftSide;
	for (int i = 0; i < verts; i++)
	{
		matrix4 rot;
		rot = glm::rotate(matrix4(1.0f), -90.0f, vector3(0.0f, 1.0f, 0.0f));
		leftSide.push_back(static_cast <vector3>(rot * glm::vec4(m_lVertexPos[i], 1.0f)));
	}

	for (int i = 0; i < verts; i++)
	{
		AddVertexPosition(leftSide[i]);
	}

	// back part
	std::vector<vector3> back;
	for (int i = 0; i < verts; i++)
	{
		matrix4 rot;
		rot = glm::rotate(matrix4(1.0f), 180.0f, vector3(0.0f, 1.0f, 0.0f));
		back.push_back(static_cast <vector3>(rot * glm::vec4(m_lVertexPos[i], 1.0f)));
	}

	for (int i = 0; i < verts; i++)
	{
		AddVertexPosition(back[i]);
	}

	// topSide part
	std::vector<vector3> topSide;
	for (int i = 0; i < verts; i++)
	{
		matrix4 rot;
		rot = glm::rotate(matrix4(1.0f), -90.0f, vector3(1.0f, 0.0f, 0.0f));
		topSide.push_back(static_cast <vector3>(rot * glm::vec4(m_lVertexPos[i], 1.0f)));
	}

	for (int i = 0; i < verts; i++)
	{
		AddVertexPosition(topSide[i]);
	}

	// bottomSide part
	std::vector<vector3> bottomSide;
	for (int i = 0; i < verts; i++)
	{
		matrix4 rot;
		rot = glm::rotate(matrix4(1.0f), 90.0f, vector3(1.0f, 0.0f, 0.0f)); // x, y, z
		bottomSide.push_back(static_cast <vector3>(rot * glm::vec4(m_lVertexPos[i], 1.0f)));
	}

	for (int i = 0; i < verts; i++)
	{
		AddVertexPosition(bottomSide[i]);
	}

	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}