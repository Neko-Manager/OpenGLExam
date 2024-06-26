#include "Scene.h"
#include <random>


Scene::Scene(std::string name)
{
	mName = name;
}

///Loaders & Unloaders
void Scene::LoadActors()
{
	//Load Actors into U-Map
	//uActorMap["pyramid"] = Actor::CreatePyramid();
	//uActorMap["curve"] = Actor::CreateInterpolationCurve3Points(0, 13, 0.2f); 
	//uActorMap["planeXY"] = Actor::CreatePlaneXY(-5, 0, 5, 5, 0.05f); 
	/*uActorMap["planeXZ"] = Actor::CreatePlaneXZ(-5, 0, 5, 5, 0.05f); */

	uActorMap["player"] = Actor::CreateCube();
	uActorMap["player"]->SetLocalPosition(glm::vec3(-2.f, 0.0f, -8.f)); 
	uActorMap["player"]->mAttachCamera = true;


	//uActorMap["cube2"] = Actor::CreateCube();


	//Create camera object
	
     mSceneCamera = new Camera("SceneCamera"); 
	 //mSceneCamera->mUseCamerMovement = true; 
	 
}

void Scene::Spawner()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> radiusXZ(-10.f, 10.f);
	std::uniform_real_distribution<float> radiusY(-5.f, 5.f);


	for (int amount = 0; amount < mAmount; amount++)
	{
		glm::vec3 spawnPos{ radiusXZ(gen),radiusY(gen),radiusXZ(gen) }; 
		Actor* spawnedActor{ nullptr }; 
		spawnedActor = Actor::CreatePyramid();
		spawnedActor->SetLocalPosition(spawnPos);
		spawnVector.emplace_back(spawnedActor);
	}
}

void Scene::LoadContent()
{
	Spawner(); 
	LoadActors();

	mShader = new Shader("Shaders/Triangle.vs", "Shaders/Triangle.fs");
	mTexture = new Texture("Shaders/wall.jpg",mShader);   
	
	for (auto actor = uActorMap.begin(); actor != uActorMap.end(); actor++)
	{
		actor->second->SetShader(mShader);  
	}
	for (auto object : spawnVector)  
	{
		object->SetShader(mShader);   
	}

}

void Scene::UnloadContent()
{
	for (auto actor = uActorMap.begin(); actor!= uActorMap.end(); actor++)
	{
		actor->second->~Actor(); 
	}

	for (auto it : spawnVector) 
	{
		it->~Actor();
	}
	delete uActorMap["pyramid"];
	uActorMap["pyramid"] = nullptr;

	delete uActorMap["planeXZ"];
	uActorMap["planeXZ"] = nullptr;

	delete uActorMap["planeXY"];
	uActorMap["planeXY"] = nullptr;

	delete uActorMap["curve"];
	uActorMap["curve"] = nullptr;

	delete uActorMap["player"];
	uActorMap["player"] = nullptr;

	delete uActorMap["cube2"];
	uActorMap["cube2"] = nullptr;

	delete uActorMap["curve"];
	uActorMap["curve"] = nullptr;

	delete mSceneCamera;
	mSceneCamera = nullptr;

	delete mShader;
	mShader = nullptr;

	mTexture->~Texture(); 
	delete mTexture; 
	mTexture = nullptr; 
}

///Updater
void Scene::UpdateScene(float dt)
{
	mSceneCamera->UpdateCamera(dt); 
}

///Rednerer
void Scene::RenderScene(float dt, Transform globaltransform)
{ 
	SpaceManipulation(); 
	BindCamera();
	UpdateScene(dt);  


	for (auto actor = uActorMap.begin(); actor!= uActorMap.end(); actor++) 
	{
		globaltransform.SetTransformMatrix(actor->second->GetLocalTransformMatrix());
		mShader->setMat4("model", globaltransform.GetTransformMatrix());   
		actor->second->UseTexture(actor->second->GetTexBool());    
		actor->second->drawActor(mShader); 
	}
	for (auto object : spawnVector) 
	{
		mShader->setMat4("model", object->GetLocalTransformMatrix());  
		object->UseTexture(object->GetTexBool()); 
		object->drawActor(mShader);
	}
	
}

///Tranformations
void Scene::SpaceManipulation() //Only rotation can be manipulated before call in Render. Offset needs to be set in LoacActors.
{
	/////Pyramid
	//uActorMap["pyramid"]->SetLocalPosition(glm::vec3(0.0f, 0.5f, -4.f));
	//uActorMap["pyramid"]->SetLocalRotation(glm::vec3(0.f, float(glfwGetTime()), 0.f)); 

	///PlaneXZ
	//uActorMap["planeXZ"]->SetLocalPosition(glm::vec3(-2.f, -1.f,-10.f));
	//uActorMap["planeXZ"]->SetLocalRotation(glm::vec3(0.f, 0.f, 0.f));

	/////PlaneXY
	//uActorMap["planeXY"]->SetLocalPosition(uActorMap["planeXZ"]->GetLocalPosition() + glm::vec3(-10.f,0.f,0.f));
	//uActorMap["planeXY"]->SetLocalRotation(glm::vec3(0.f, 0.f,0.f));


	///Cube
	//uActorMap["cube"]->SetLocalPosition(glm::vec3(-2.f, 0.0f, -8.f)); 
	//uActorMap["cube"]->SetLocalRotation(glm::vec3((float)glfwGetTime(), (float)glfwGetTime(), (float)glfwGetTime()));

	//uActorMap["cube2"]->SetLocalPosition(glm::vec3(2.f, 0.0f, -8.f));
	//uActorMap["cube2"]->SetLocalRotation(glm::vec3((float)glfwGetTime(), (float)glfwGetTime(), (float)glfwGetTime()));

	
	/////Curve
	//uActorMap["curve"]->SetLocalPosition(glm::vec3(-2.f, -1.0f, -8.f));

	/*for (auto object : spawnVector)
	{
		object->SetLocalRotation(glm::vec3((float)glfwGetTime(), (float)glfwGetTime(), (float)glfwGetTime())); 
	}*/

	for (auto object : spawnVector)
	{
		mCollision->AABB(uActorMap["player"], object);
	}
} 

///Shader Binder
void Scene::BindCamera()
{
	mShader->setMat4("view", mSceneCamera->GetViewMatrix()); 
	mShader->setMat4("projection", mSceneCamera->GetProjectionMatrix());
	mShader->setVec3("viewPos", mSceneCamera->GetLocalPosition());   
}


