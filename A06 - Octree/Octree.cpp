#include "Octree.h"


Simplex::Octree::Octree(vector3 center, vector3 size)
{

	// set Root node of tree
	rootNode = this;

	// get global center and entity count
	maxV3 = minV3 = entityManager->GetRigidBody()->GetCenterGlobal();
	entities = entityManager->GetEntityCount();

	// loop through each entity
	for (int i = 0; i < entities; i++) {
		// add to entity list
		entityList.push_back(i);

		// get min and max of entity
		vector3 min = entityManager->GetRigidBody(i)->GetMinGlobal;
		vector3 max = entityManager->GetRigidBody(i)->GetMaxGlobal;

		// set min and max of octree
		if (min.x < minV3.x) minV3.x = min.x;
		if (min.y < minV3.y) minV3.y = min.y;
		if (min.z < minV3.z) minV3.z = min.z;
		if (max.x > maxV3.x) maxV3.x = max.x;
		if (max.y > maxV3.y) maxV3.y = max.y;
		if (max.z > maxV3.z) maxV3.z = max.z;
	}

	// set center of octree
	centerV3 = (minV3 + maxV3) / 2.0f;

	// size of octree
	size = maxV3 - minV3;

	
}

// collisions 
bool Simplex::Octree::IsColliding(uint a_uRBIndex) {
	MyRigidBody* rb = entityManager->GetRigidBody(a_uRBIndex);
	vector3 rbMAX = rb->GetMaxGlobal();
	vector3 rbMIN = rb->GetMinGlobal();
	if (rbMAX.z > minV3.z && rbMIN.x < maxV3.x && rbMIN.y < maxV3.y &&rbMIN.z < maxV3.z && rbMAX.x > minV3.x && rbMAX.y > minV3.y) {
		return true;
	}
	else {
		return false;
	}
}

void Simplex::Octree::BuildOctree() {

	// for each object create new octree element that is 1/4 the size of the original
	// 8 objectren per tree node
	// for each corner of the node
	object[0] = new Octree(centerV3 + vector3(-size.x / 4, size.y / 4, -size.z / 4), size / 2.f);
	object[1] = new Octree(centerV3 + vector3(-size.x / 4, size.y / 4, size.z / 4), size / 2.f);
	object[2] = new Octree(centerV3 + vector3(-size.x / 4, -size.y / 4, -size.z / 4), size / 2.f);
	object[3] = new Octree(centerV3 + vector3(-size.x / 4, -size.y / 4, size.z / 4), size / 2.f);
	object[4] = new Octree(centerV3 + vector3(size.x / 4, -size.y / 4, -size.z / 4), size / 2.f);
	object[5] = new Octree(centerV3 + vector3(size.x / 4, -size.y / 4, size.z / 4), size / 2.f);
	object[6] = new Octree(centerV3 + vector3(size.x / 4, size.y / 4, -size.z / 4), size / 2.f);
	object[7] = new Octree(centerV3 + vector3(size.x / 4, size.y / 4, size.z / 4), size / 2.f);

	for (uint i = 0; i < 8; ++i) {
		// set parent of each object
		object[i]->parent = this;
		// set root node of object
		object[i]->rootNode = rootNode;
		for (uint j = 0; j < entities; ++j) {
			if (object[i]->IsColliding(entityList[j])) {
				// if entities are colliding add then to entity list
				object[i]->entityList.push_back(entityList[j]);
			}
		}
		// recursively subdivide again
		object[i]->entities = object[i]->entityList.size();
		object[i]->BuildOctree();
	}
}

Simplex::Octree::~Octree()
{
}
