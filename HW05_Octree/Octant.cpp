#include "Octant.h"
using namespace BTX;
//  Octant
uint Octant::m_uOctantCount = 0;
uint Octant::m_uMaxLevel = 3;
uint Octant::m_uIdealEntityCount = 5;
Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	/*
	* This constructor is meant to be used ONLY on the root node, there is already a working constructor
	* that will take a size and a center to create a new space
	*/
	Init();//Init the default values
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	//create a rigid body that encloses all the objects in this octant, it necessary you will need
	//to subdivide the octant based on how many objects are in it already an how many you IDEALLY
	//want in it, remember each subdivision will create 8 children for this octant but not all children
	//of those children will have children of their own
	
	std::vector<vector3> lMinMax;
	vector3 min, max, current;

	//get min and max from entity manager
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		current = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal();

		//get min
		if (min.x > current.x) min.x = current.x;
		if (min.y > current.y) min.y = current.y;
		if (min.z > current.z) min.z = current.z;

		current = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal();

		//get max
		if (max.x < current.x) max.x = current.x;
		if (max.y < current.y) max.y = current.y;
		if (max.z < current.z) max.z = current.z;

	}
	
	//convert them all to be the same length 
	float length = max.x - min.x;
	if (max.y - min.y > length) length = max.y - min.y;
	if (max.z - min.z > length) length = max.z - min.z;

	length /= 2.0f;

	//adjust min and max to be cube shaped 
	min = vector3(m_v3Center.x - length, m_v3Center.y - length, m_v3Center.z - length);
	max = vector3(m_v3Center.x + length, m_v3Center.y + length, m_v3Center.z + length);

	//push back min and max values 
	lMinMax.push_back(min);
	lMinMax.push_back(max);

	RigidBody pRigidBody = RigidBody(lMinMax);

	//The following will set up the values of the octant, make sure the are right, the rigid body at start
	//is NOT fine, it has made-up values
	m_fSize = pRigidBody.GetHalfWidth().x * 2.0f;
	m_v3Center = pRigidBody.GetCenterLocal();
	m_v3Min = m_v3Center - pRigidBody.GetHalfWidth();
	m_v3Max = m_v3Center + pRigidBody.GetHalfWidth();

	m_uOctantCount++; //When we add an octant we increment the count
	ConstructTree(m_uMaxLevel); //Construct the children
}

bool Octant::IsColliding(uint a_uRBIndex)
{
	//Get how many objects there are in the world
	uint count = m_pEntityMngr->GetEntityCount();

	//If the index given is larger than the number of elements in the bounding object there is no collision
	if (a_uRBIndex > count) return false;

	//else get the entity
	Entity* thisEntity = m_pEntityMngr->GetEntity(a_uRBIndex);

	//As the Octree will never rotate or scale this collision is as easy as an Axis Alligned Bounding Box
	//Get all vectors in global space (the octant ones are already in Global)
	vector3 current;
	
	for (int i = 0; i < this->m_EntityList.size(); i++)
	{
		current = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal();

		if (thisEntity->GetRigidBody()->GetMinGlobal().x < current.x) continue;
		if (thisEntity->GetRigidBody()->GetMinGlobal().y < current.y) continue;
		if (thisEntity->GetRigidBody()->GetMinGlobal().z < current.z) continue;

		current = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal();

		if (thisEntity->GetRigidBody()->GetMaxGlobal().x > current.x) continue;
		if (thisEntity->GetRigidBody()->GetMaxGlobal().y > current.y) continue;
		if (thisEntity->GetRigidBody()->GetMaxGlobal().z > current.z) continue;

		return true;
	}

	
}
void Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// Display the specified octant
	Octant* current = (a_nIndex < m_lChild.size()) ? this->m_lChild.at(a_nIndex) : this;
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, current->m_v3Center) * glm::scale(vector3(current->m_fSize)), a_v3Color);

}
void Octant::Display(vector3 a_v3Color)
{
	//this is meant to be a recursive method, in starter code will only display the root
	//even if other objects are created
	//m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		//glm::scale(vector3(m_fSize)), a_v3Color);

	//base case - display this
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center)* glm::scale(vector3(m_fSize)), a_v3Color);
	
	//recursive case - if there are children, loop through until you find leaf nodes 

	for (int i = 0; i < this->m_uChildren; i++)
	{
		this->m_lChild.at(i)->Display(a_v3Color);
	}

	
}
void Octant::Subdivide(void)
{
	//If this node has reach the maximum depth return without changes
	if (m_uLevel >= m_uMaxLevel)
		return;

	//If this node has been already subdivided return without changes
	if (m_uChildren != 0)
		return;

	//Subdivide the space and allocate 8 children
	//loop through +/- 
	
	for (int x = -1; x <= 1; x+=2)
	{
		for (int y = -1; y <= 1; y+=2) 
		{
			for (int z = -1; z <= 1; z+=2)
			{
				//c = pc.x +/- pc.h/2
				float hw = (this->GetSize()/4);
				vector3 center = vector3(	this->m_v3Center.x + (x * hw), 
											this->m_v3Center.y + (y * hw), 
											this->m_v3Center.z + (z * hw));
				Octant* newOct = new Octant(center, hw*2.0f);
				this->m_lChild.push_back(newOct);
				this->m_pChild[this->m_uChildren] = newOct;
				this->m_uChildren++;
				newOct->m_uLevel = this->m_uLevel+1;
				if(newOct->ContainsAtLeast(m_uIdealEntityCount)) newOct->Subdivide();
			}
		}
		
	}
}
bool Octant::ContainsAtLeast(uint a_nEntities)
{
	//You need to check how many entity objects live within this octant
	uint total = 0;
	vector3 current;

	//get how many entities are inside this
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		//get position of current entity
		current = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal();

		if (this->GetMinGlobal().x > current.x) continue;
		if (this->GetMinGlobal().y > current.y) continue;
		if (this->GetMinGlobal().z > current.z) continue;

		current = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal();

		if (this->GetMaxGlobal().x < current.x) continue;
		if (this->GetMaxGlobal().y < current.y) continue;
		if (this->GetMaxGlobal().z < current.z) continue;

		total++;
	}

	return total > a_nEntities; //return something for the sake of start up code
}
void Octant::AssignIDtoEntity(void)
{
	//base case
	if (this->IsLeaf())
	{
		for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			if (this->IsColliding(i)) m_pEntityMngr->AddDimension(i, m_uID);
		}
	}

	//recursive case
	else
	{
		for (int i = 0; i < this->m_uChildren; i++)
		{
			m_pChild[i]->AssignIDtoEntity();
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
// You can assume the following is fine and does not need changes, you may add onto it but the code is fine as is
// in the proposed solution.
void Octant::Init(void)
{
	m_uChildren = 0;

	m_fSize = 0.0f;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint n = 0; n < 8; n++)
	{
		m_pChild[n] = nullptr;
	}
}
void Octant::Swap(Octant& other)
{
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}
void Octant::Release(void)
{
	//this is a special kind of release, it will only happen for the root
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}
void Octant::ConstructTree(uint a_nMaxLevel)
{
	//If this method is tried to be applied to something else
	//other than the root, don't.
	if (m_uLevel != 0)
		return;

	m_uMaxLevel = a_nMaxLevel; //Make sure you mark which is the maximum level you are willing to reach
	m_uOctantCount = 1;// We will always start with one octant

	//If this was initialized before make sure its empty
	m_EntityList.clear();//Make sure the list of entities inside this octant is empty
	KillBranches();
	m_lChild.clear();

	//If we have more entities than those that we ideally want in this octant we subdivide it
	if (ContainsAtLeast(m_uIdealEntityCount))
	{
		Subdivide();
	}
	AssignIDtoEntity();//Add octant ID to Entities
	ConstructList();//construct the list of objects
}
//The big 3
Octant::Octant(vector3 a_v3Center, float a_fSize)
{
	//Will create the octant object based on the center and size but will not construct children
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	m_uOctantCount++;
}
Octant::Octant(Octant const& other)
{
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;

	m_pRoot, other.m_pRoot;
	m_lChild, other.m_lChild;

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}
Octant& Octant::operator=(Octant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		Octant temp(other);
		Swap(temp);
	}
	return *this;
}
Octant::~Octant() { Release(); };
//Accessors
float Octant::GetSize(void) { return m_fSize; }
vector3 Octant::GetCenterGlobal(void) { return m_v3Center; }
vector3 Octant::GetMinGlobal(void) { return m_v3Min; }
vector3 Octant::GetMaxGlobal(void) { return m_v3Max; }
uint Octant::GetOctantCount(void) { return m_uOctantCount; }
bool Octant::IsLeaf(void) { return m_uChildren == 0; }
Octant* Octant::GetParent(void) { return m_pParent; }
//--- other methods
Octant * Octant::GetChild(uint a_nChild)
{
	//if its asking for more than the 8th children return nullptr, as we don't have one
	if (a_nChild > 7) return nullptr;
	return m_pChild[a_nChild];
}
void Octant::KillBranches(void)
{
	/*This method has recursive behavior that is somewhat hard to explain line by line but
	it will traverse the whole tree until it reaches a node with no children and
	once it returns from it to its parent it will kill all of its children, recursively until
	it reaches back the node that called it.*/
	for (uint nIndex = 0; nIndex < m_uChildren; nIndex++)
	{
		m_pChild[nIndex]->KillBranches();
		delete m_pChild[nIndex];
		m_pChild[nIndex] = nullptr;
	}
	m_uChildren = 0;
}
void Octant::DisplayLeaves(vector3 a_v3Color)
{
	/*
	* Recursive method
	* it will traverse the tree until it find leaves and once found will draw them
	*/
	uint nLeafs = m_lChild.size(); //get how many children this will have (either 0 or 8)
	for (uint nChild = 0; nChild < nLeafs; nChild++)
	{
		m_lChild[nChild]->DisplayLeaves(a_v3Color);
	}
	//Draw the cube
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::ClearEntityList(void)
{
	/*
	* Recursive method
	* It will traverse the tree until it find leaves and once found will clear its content
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ClearEntityList();
	}
	m_EntityList.clear();
}
void Octant::ConstructList(void)
{
	/*
	* Recursive method
	* It will traverse the tree adding children
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ConstructList();
	}
	//if we find a non-empty child add it to the tree
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}
