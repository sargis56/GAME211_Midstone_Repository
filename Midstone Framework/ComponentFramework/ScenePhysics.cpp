#include <glew.h>
#include <iostream>
//#include <tgmath.h>
#include "Debug.h"
#include "ScenePhysics.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"
using namespace std;

ScenePhysics::ScenePhysics() : demoObject(nullptr), meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr) {
	Debug::Info("Created ScenePhysics: ", __FILE__, __LINE__);
}

ScenePhysics::~ScenePhysics() {}

bool ScenePhysics::OnCreate() {
	//Setting up the scene lights
	numLight = 2;
	lightArray[0] = Vec3(0.0f, 40.0f, 0.0f);
	lightArray[1] = Vec3(0.0f, -20.0f, 0.0f);
	Light1 = new Light(Vec3(0.0f, 40.0f, 0.0f), Vec3(0.6f, 0.6f, 0.6f), Vec3(0.1f, 0.6f, 0.6f));
	
	projectionMatrix = MMath::perspective(60.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	if (ObjLoader::loadOBJ("meshes/XWing.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/multiPhongVert.glsl", "shaders/multiPhongFrag.glsl");
	texturePtr = new Texture();
	if (meshPtr == nullptr || shaderPtr == nullptr || texturePtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}


	if (texturePtr->LoadImage("textures/skull_texture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}

	demoObject = new DemoObject(meshPtr, shaderPtr, texturePtr);
	if (demoObject == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	//Setting up the spaceship    -4000
	demoObject->ObjectSetUp(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1000000.0f, 26700000000);
	//Setting up the force before the engines go out
	force = Vec3(50000000.0f, 0.0f, 0.0f);
	//Creating text file
	//ofstream file;
	//file.open("Starship.csv");
	//file << "time, demoObject->getAngleAccel(),demoObject->getAngleVel(), demoObject->getAngleDisplacement() demoObject->getPos().x, demoObject->getPos().y, demoObject->getAccel().x, demoObject->getAccel().y, force.x, force.y, demoObject->getVel().x,demoObject->getVel().y " << endl;
	return true;
}

void ScenePhysics::HandleEvents(const SDL_Event& sdlEvent) {

}
void ScenePhysics::Update(const float deltaTime) {
	//Updating object every frame
	demoObject->Update(deltaTime);
	//After one engine fails: applying torque and updating the thrust vector using a 2D transformation matrix
	if (time >= 10) {
		//Appling Torque
		demoObject->ApplyTorque(629100000);
		//Updating angle and angleVel
		Physics::RigidBodyRotation(*demoObject, deltaTime);
		//Converting to radians from the displacement
		rads = (demoObject->getAngleDisplacement() * M_PI) / 180;
		//2D transformation matrix
		rotateMatrix.m[0] = cos(rads);
		rotateMatrix.m[1] = sin(rads);
		rotateMatrix.m[2] = sin(rads);
		rotateMatrix.m[3] = cos(rads);
		double forcex = rotateMatrix.DotX(force.x, force.y);
		double forcey = rotateMatrix.DotY(force.x, force.y);
		force.x = forcex;
		force.y = forcey;
		//Both engines are off
		if (time >= 30) {
			force.x = 0.0f;
			force.y = 0.0f;
			demoObject->ApplyTorque(0);
		}
	}
	//Set half the force when the one engine fails
	if (time >= 10 && engineFail == false) {
		force.x = 25000000.0f;
		engineFail = true;
		printf("ENGINE FAILURE\n");
	}
	//Update object Pos and Vel with the equations of motion
	Physics::SimpleNewtonMotion(*demoObject, deltaTime);
	//Applying the force to the spaceship
	demoObject->applyForce(force);
	//Updating the model matrix
	//With real rotation
	demoObject->setModelMatrix(MMath::translate(demoObject->getPos() * 0.002 /*0.0f,0.0f,0.0f */) * MMath::rotate(demoObject->getAngleDisplacement(), Vec3(0.0f, 0.0f, 1.0f)) * MMath::rotate(90, Vec3(0.0f, 1.0f, 0.0f)));
	//Printing out all important values
	count++;
	if (count == 33) {
		time++;
		printf("Time   Pos.x   Pos.y    Accel.x    Accel.y    Force.x      Force.y     Force Check \n");
		printf("%i %f %f %f %f %f %f %f\n", time, demoObject->getPos().x, demoObject->getPos().y, demoObject->getAccel().x, demoObject->getAccel().y, force.x, force.y, (sqrt(pow(force.x,2) + pow(force.y,2))));
		printf("Time   AngAccel   AngleVel    Angle    Force.x   Force.y    Accel.x     Accel.y \n");
		printf("%i    %f  %f  %f  %f  %f  %f  %f\n", time, demoObject->getAngleAccel(), demoObject->getAngleVel(), demoObject->getAngleDisplacement(), force.x, force.y, demoObject->getAccel().x, demoObject->getAccel().y);
		count = 0;
	}
}

void ScenePhysics::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	GLuint program = demoObject->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);

	glUniform3fv(demoObject->getShader()->getUniformID("lightPos[0]"), 2, *lightArray);
	glUniform1f(demoObject->getShader()->getUniformID("numLight"), numLight);

	demoObject->Render();

	glUseProgram(0);
}


void ScenePhysics::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (demoObject) delete demoObject, demoObject = nullptr;
}