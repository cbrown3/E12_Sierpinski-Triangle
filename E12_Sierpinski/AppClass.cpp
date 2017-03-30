#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("E12 - Instance Rendering");
	m_v4ClearColor = vector4(REBLACK, 1.0f);
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pMesh = new MyMesh();
	
	m_pTriangleMesh = new MyMesh();

	//Creating the Mesh points
	m_pMesh->AddVertexPosition(vector3(-1.0f, -1.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexPosition(vector3( 1.0f, -1.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexPosition(vector3(0.0f,  1.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);

	//Creating the Mesh points
	m_pTriangleMesh->AddVertexPosition(vector3(-1.0f, -1.0f, 0.0f));
	m_pTriangleMesh->AddVertexColor(REWHITE);
	m_pTriangleMesh->AddVertexPosition(vector3(1.0f, -1.0f, 0.0f));
	m_pTriangleMesh->AddVertexColor(REWHITE);
	m_pTriangleMesh->AddVertexPosition(vector3(0.0f, 1.0f, 0.0f));
	m_pTriangleMesh->AddVertexColor(REWHITE);
	//Compiling the mesh
	m_pMesh->CompileOpenGL3X();
	m_pTriangleMesh->CompileOpenGL3X();
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	ClearScreen();
	
	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	m_pMesh->Render(m4Projection, m4View, IDENTITY_M4);//Rendering nObject(s)

	//matricies for triangle
	matrix4 rotateMatrix = glm::rotate(180.0f, vector3(0.0f, 0.0f, 1.0f));
	matrix4 scaleMatrix = glm::scale(vector3(0.5f, 0.5f, 1.0f));
	matrix4 translateMatrix = glm::translate(vector3(0.0f, 1.0f, 0.0f));

	matrix4 triangleMatrix = rotateMatrix * scaleMatrix * translateMatrix;

	m_pTriangleMesh->Render(m4Projection, m4View, triangleMatrix);

	for (int i = 0; i < 3; i++)
	{
		matrix4 scale1Matrix = scaleMatrix * glm::scale(vector3(0.5f, 0.5f, 0.0f));
		matrix4 translate1Matrix = translateMatrix * glm::translate(vector3(0.0f, -0.75f, 0.0f));
		matrix4 scale2Matrix = scaleMatrix * glm::scale(vector3(0.5f, 0.5f, 0.0f));
		matrix4 translate2Matrix = translateMatrix * glm::translate(vector3(0.5f, -1.75f, 0.0f));
		matrix4 scale3Matrix = scaleMatrix * glm::scale(vector3(0.5f, 0.5f, 0.0f));
		matrix4 translate3Matrix = translateMatrix * glm::translate(vector3(-0.5f, -1.75f, 0.0f));
		
		switch (i)
		{
		case 0:
			triangleMatrix = translate1Matrix * rotateMatrix * scale1Matrix;
			break;
		case 1:
			triangleMatrix = translate2Matrix * rotateMatrix * scale2Matrix;
			break;
		case 2:
			triangleMatrix = translate3Matrix * rotateMatrix * scale3Matrix;
			break;
		}

		m_pTriangleMesh->Render(m4Projection, m4View, triangleMatrix);
	}

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}