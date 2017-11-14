#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	m_entityList.clear();

	m_uEntityCount = 0;
}

void MyEntityManager::Release(void)
{
	for (uint entity = 0; entity < m_uEntityCount; entity++) {

		MyEntity* deleteEntity = m_entityList[entity];

		SafeDelete(deleteEntity);
	}
	
	m_uEntityCount = 0;

	m_entityList.clear();
}

MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}

void MyEntityManager::ReleaseInstance()
{
	if (m_pInstance == nullptr) {
		return;
	}
	else {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (uint entity = 0; entity < m_uEntityCount; entity++) {

		if (m_entityList[entity]->GetUniqueID() == a_sUniqueID) {
			return entity;
		}
	}

	// otherwise.. 
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	if (m_entityList.size() > 0) {
		if (a_uIndex >= m_uEntityCount) {
			a_uIndex = m_uEntityCount - 1;
		}
		return m_entityList[a_uIndex]->GetModel();
	}

	if (m_entityList.size() == 0) {
		return nullptr;
	}
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	MyEntity* entity1 = MyEntity::GetEntity(a_sUniqueID);
	if (entity1) {
		return entity1->GetModel();
	}
	if (!entity1) {
		return nullptr;
	}
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	if (m_entityList.size() > 0) {
		if (a_uIndex >= m_uEntityCount) {
			a_uIndex = m_uEntityCount - 1;
		}
		return m_entityList[a_uIndex]->GetRigidBody();
	}
	if (m_entityList.size() == 0) {
		return nullptr;
	}
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	MyEntity* entity2 = MyEntity::GetEntity(a_sUniqueID);
	if (entity2) {
		return entity2->GetRigidBody();

	}
	if (!entity2) {
		return nullptr;
	}
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	if (m_entityList.size() == 0) {
		return IDENTITY_M4;
	}
	if (m_entityList.size() > 0) {
		if (a_uIndex >= m_uEntityCount) {
			a_uIndex = m_uEntityCount - 1;
		}
		return m_entityList[a_uIndex]->GetModelMatrix();
	}

}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{

	MyEntity* entity3 = MyEntity::GetEntity(a_sUniqueID);
	if (!entity3) {
		return IDENTITY_M4;
	}
	if (entity3) {
		return entity3->GetModelMatrix();
	}


	//return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	MyEntity* entity4 = MyEntity::GetEntity(a_sUniqueID);
	if (entity4) {
		entity4->SetModelMatrix(a_m4ToWorld);
	}
	if (!entity4) {
		return;
	}

}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	if (m_entityList.size() > 0) {
		if (a_uIndex >= m_uEntityCount) {
			a_uIndex = m_uEntityCount - 1;
		}
		m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
	}
	if (m_entityList.size() == 0) {
		return;
	}
	
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	for (uint i = 0; i < m_uEntityCount - 1; i++) {
		for (uint a = i + 1; a < m_uEntityCount; a++) {
			m_entityList[i]->IsColliding(m_entityList[a]);
		}
	}

}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	MyEntity* myNewEntity = new MyEntity(a_sFileName, a_sUniqueID);
	if (myNewEntity->IsInitialized()) {
		m_entityList.push_back(myNewEntity);
		// update size 
		m_uEntityCount = m_entityList.size();
	}

}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	if (m_entityList.size() > 0) {
		return;
	}
	if (m_entityList.size() > 0) {

		if (a_uIndex >= m_uEntityCount) {
			a_uIndex = m_uEntityCount - 1;
		}
		
		if (a_uIndex != m_uEntityCount - 1) {
			std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);
		}

		MyEntity* myNewEntity = m_entityList[m_uEntityCount - 1];
		m_uEntityCount--;
		SafeDelete(myNewEntity);
		m_entityList.pop_back();
		return;
	}
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int id = GetEntityIndex(a_sUniqueID);
	RemoveEntity((uint)id);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	if (m_entityList.size() == 0) {
		return "";
	}
	if (m_entityList.size() > 0) {
		if (a_uIndex >= m_entityList.size()) {
			a_uIndex = m_entityList.size() - 1;
		}
		return m_entityList[a_uIndex]->GetUniqueID();
	}
	else {
		return "";
	}
	//return "";
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	if (m_entityList.size() > 0) {
		if (a_uIndex >= m_entityList.size()) {
			a_uIndex = m_entityList.size() - 1;
		}
		return m_entityList[a_uIndex];
	}
	if (m_entityList.size() == 0) {
		return nullptr;
	}
	//return nullptr;
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	if (a_uIndex < m_uEntityCount) {
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
	if (a_uIndex >= m_uEntityCount) {
		// loop through all and add to the render list 
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; a_uIndex++) {
			m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	MyEntity* myEntity = MyEntity::GetEntity(a_sUniqueID);
	if (myEntity) {
		myEntity->AddToRenderList(a_bRigidBody);
	}
}