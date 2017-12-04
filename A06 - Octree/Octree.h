#ifndef __OCTREE_
#define __OCTREE_

#include "Simplex\Mesh\Model.h"
#include "Definitions.h"
#include "AppClass.h"
#include "MyRigidBody.h"
#include "Main.h"
#include "MyEntityManager.h"
#include "Simplex\Simplex.h"

#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\OpenGL.hpp"
#include "SFML\Audio.hpp"


namespace Simplex {

	struct Node {
		vector3 center;
		vector3 halfWidth;

		Node* nodes[8];
		int maxObjects = 10;
	};

	class Octree
	{
		Octree* rootNode; 
		vector3 maxV3 = vector3(0.0f);
		vector3 minV3 = vector3(0.0f);
		vector3 centerV3 = vector3(0.0f);
		vector3 size = vector3(0.0f);
		Octree* object[8];
		Octree* parent;
		int entities = 0;
		std::vector<int> entityList;
		bool active;
		MyEntityManager* entityManager = nullptr;
		static Octree* instance;
	public:
		Octree(vector3 center, vector3 size);

		bool IsColliding(uint a_uRBIndex);

		static Octree* getInstance();



		// build octree
		void BuildOctree();

		// instert object into octree
		void InsertObject(Node* tree, MyRigidBody* body);

		// display
		void DisplayOctree();

		// destructor
		~Octree();
	};
}
#endif //__OCTREE_