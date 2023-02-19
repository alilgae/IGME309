#include "MyMesh.h"
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

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
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//const variables 
	const vector3 CENTER(0, 0, 0), TOP(0, a_fHeight, 0);
	const float THETA = (360.0f / a_nSubdivisions) * (PI / 180.0f);

	//changing vertices 
	vector3 newVert((a_fRadius * cosf(THETA)), 0.0f, (a_fRadius * -sinf(THETA))), prevVert = newVert;

	//draw base, draw sides 
	for (int i = 2; i <= a_nSubdivisions+1; i++)
	{
		newVert = vector3((a_fRadius * cosf(THETA * i)), 0.0f, (a_fRadius * -sinf(THETA * i)));
		
		AddTri(TOP, prevVert, newVert);
		AddTri(CENTER, vector3(prevVert.x, prevVert.y, -prevVert.z), vector3(newVert.x, newVert.y, -newVert.z));

		prevVert = newVert;
	}
	
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//const variables
	const vector3 CENTER_BOTTOM(0, 0, 0), CENTER_TOP(0, a_fHeight, 0);
	const float THETA = (360.0f / a_nSubdivisions) * (PI / 180.0f);

	//changing vertices variables 
	vector3 newVertBtm((a_fRadius * cosf(THETA)), 0.0f,		 (a_fRadius * -sinf(THETA))), prevVertBtm = newVertBtm;
	vector3 newVertTop((a_fRadius * cosf(THETA)), a_fHeight, (a_fRadius * -sinf(THETA))), prevVertTop = newVertTop;
	
	//draw btm, draw top, draw quad sides 
	for (int i = 2; i <= a_nSubdivisions + 1; i++)
	{
		newVertBtm = vector3((a_fRadius * cosf(THETA * i)), 0.0f, (a_fRadius * -sinf(THETA * i)));
		newVertTop = vector3(newVertBtm.x, a_fHeight, newVertBtm.z);

		AddTri(CENTER_TOP, prevVertTop, newVertTop);
		AddTri(CENTER_BOTTOM, newVertBtm, prevVertBtm);
		AddQuad(prevVertBtm, newVertBtm, prevVertTop, newVertTop);

		prevVertBtm = newVertBtm;
		prevVertTop = newVertTop;
	}
	
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//const variables
	const float THETA = (360.0f / a_nSubdivisions) * (PI / 180.0f);
	const vector3 CENTER_TOP(0.0f, a_fHeight, 0.0f);

	//changing vertices variables 
	vector3 newInnerTopVert((a_fInnerRadius * cosf(THETA)), a_fHeight, (a_fInnerRadius * -sinf(THETA))), prevInnerTopVert = newInnerTopVert;
	vector3 newOuterTopVert((a_fOuterRadius * cosf(THETA)), a_fHeight, (a_fOuterRadius * -sinf(THETA))), prevOuterTopVert = newOuterTopVert;
	
	vector3 newInnerBottomVert((a_fInnerRadius * cosf(THETA)), 0.0f, (a_fInnerRadius * -sinf(THETA))), prevInnerBottomVert = newInnerBottomVert;
	vector3 newOuterBottomVert((a_fOuterRadius * cosf(THETA)), 0.0f, (a_fOuterRadius * -sinf(THETA))), prevOuterBottomVert = newOuterBottomVert;

	//draw btm, draw top, draw sides 
	for (int i = 2; i <= a_nSubdivisions + 1; i++)
	{
		newInnerTopVert = vector3((a_fInnerRadius * cosf(THETA * i)), a_fHeight, (a_fInnerRadius * -sinf(THETA * i)));
		newOuterTopVert = vector3((a_fOuterRadius * cosf(THETA * i)), a_fHeight, (a_fOuterRadius * -sinf(THETA * i)));
		newInnerBottomVert = vector3((a_fInnerRadius * cosf(THETA * i)), 0.0f, (a_fInnerRadius * -sinf(THETA * i)));
		newOuterBottomVert = vector3((a_fOuterRadius * cosf(THETA * i)), 0.0f, (a_fOuterRadius * -sinf(THETA * i)));

		//top
		AddQuad(prevOuterTopVert, newOuterTopVert, prevInnerTopVert, newInnerTopVert);

		//bottom
		AddQuad(newOuterBottomVert, prevOuterBottomVert, newInnerBottomVert, prevInnerBottomVert);

		//sides
		AddQuad(prevOuterBottomVert, newOuterBottomVert, prevOuterTopVert, newOuterTopVert);
		AddQuad(newInnerBottomVert, prevInnerBottomVert, newInnerTopVert, prevInnerTopVert);

		prevInnerTopVert = newInnerTopVert;
		prevOuterTopVert = newOuterTopVert;
		prevInnerBottomVert = newInnerBottomVert;
		prevOuterBottomVert = newOuterBottomVert;
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
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

	//const var
	const float THETA_TORUS = (360.0f / a_nSubdivisionsA) * (PI / 180.0f), THETA_CIRCLE = (360.0f / a_nSubdivisionsB) * (PI / 180.0f),
		RADIUS_CIRCLE = (a_fOuterRadius-a_fInnerRadius)/2, RADIUS_TORUS = a_fInnerRadius + RADIUS_CIRCLE;

	//list of verts for each circle
	std::vector<vector3> prevCircle, newCircle;
	
	//initialize lists 
	for (int j = 1; j <= a_nSubdivisionsB + 1; j++)
	{
		//generate circle of vertices 
		for (int i = 1; i <= a_nSubdivisionsA + 1; i++)
		{
			newCircle.push_back(
				vector3(
					(RADIUS_TORUS + RADIUS_CIRCLE * cosf(THETA_CIRCLE * i)) * cosf(THETA_TORUS * j),
					(RADIUS_CIRCLE * sinf(THETA_CIRCLE * i)),
					(RADIUS_TORUS + RADIUS_CIRCLE * cosf(THETA_CIRCLE * i)) * -sinf(THETA_TORUS * j)
				));
		}

		//draw quads connecting points in the two circles 
		if (prevCircle.size() > 0)
		{
			for (int i = 1; i < newCircle.size(); i++)
			{
				AddQuad(newCircle[i - 1], newCircle[i], prevCircle[i - 1], prevCircle[i]);
			}
		}

		prevCircle = newCircle;
		newCircle.clear();

	}

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	//consts
	const float RADIUS = a_fRadius, THETA = PI / a_nSubdivisions;

	//list of all the verts
	std::vector<vector3> verts;
	
	//whole sphere
	for (int j = 0; j < a_nSubdivisions; j++)
	{
		//each level 
		for (int i = 0; i < a_nSubdivisions; i++)
		{
			verts.push_back(vector3(
				RADIUS * sinf(THETA * i) * cosf(THETA * 2 * j),
				RADIUS * sinf(THETA * i) * sinf(THETA * 2 * j),
				RADIUS * cosf(THETA * i)
			));

			verts.push_back(vector3(
				RADIUS * sinf(THETA * (i + 1)) * cosf(THETA * 2 * j),
				RADIUS * sinf(THETA * (i + 1)) * sinf(THETA * 2 * j),
				RADIUS * cosf(THETA * (i + 1))
			));

			verts.push_back(vector3(
				RADIUS * sinf(THETA * i) * cosf(THETA * 2 * (j + 1)),
				RADIUS * sinf(THETA * i) * sinf(THETA * 2 * (j + 1)),
				RADIUS * cosf(THETA * i)
			));

			verts.push_back(vector3(
				RADIUS * sinf(THETA * (i + 1)) * cosf(THETA * 2 * (j + 1)),
				RADIUS * sinf(THETA * (i + 1)) * sinf(THETA * 2 * (j + 1)),
				RADIUS * cosf(THETA * (i + 1))
			));
		}
	}

	//drawing the whole sphere 
	for (int i = 3; i < verts.size(); i += 4)
	{
		AddQuad(verts[i - 3], verts[i - 2], verts[i - 1], verts[i]);
	}

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
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
		MyMesh temp(other);
		Swap(temp);
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
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

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
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
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

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
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