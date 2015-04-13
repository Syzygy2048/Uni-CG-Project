#include "SceneNode.h"

#include <rpc.h>

SceneNode::SceneNode(UUID uuid, NodeType type) : objectId(uuid), nodeType(type)
{

}


SceneNode::~SceneNode()
{
//	for (SceneNode* child : childList){
//		delete child;
//	}
}

/*void SceneNode::update(double timeStep){
	for (SceneNode* child : childList){
		child->update(timeStep);
	}
}*/

/*void SceneNode::draw(){
	for (SceneNode* child : childList){
		child->draw();
	}
}*/


/*void SceneNode::propagateMatrix(){

}*/
/*void SceneNode::attachChild(SceneNode* child){
	childList.push_back(child);
} 

bool SceneNode::detachChild(SceneNode* deleteChild){
	for (std::vector<SceneNode*>::iterator iter = childList.begin(); iter != childList.end(); iter++){
		if (*iter == deleteChild){
			childList.erase(iter);
			delete *iter;
			return true;
		}
		return (*iter)->detachChild(deleteChild);
	}
	return false;
}
*/

void SceneNode::setParent(SceneNode* parent){
	this->parent = parent;
}