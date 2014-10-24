#include "findnodevistor.h"
#include <QDebug>

FindNodeVistor::FindNodeVistor(const std::string & name, const std::string &parent_name) :
    osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
    _name(name),
    _parentname(parent_name)
{
}

void FindNodeVistor::apply(osg::Node &node)
{
    if(node.getName() == _name){
    //      _foundNodes.push_back(&node);
      osg::NodePathList parentNodePaths =  node.getParentalNodePaths();
      qDebug()<<parentNodePaths.size();
      for(unsigned int i=0;i<parentNodePaths.size();++i)
         {
            osg::NodePath& nodePath = parentNodePaths[i];
           // qDebug()<<nodePath.size();
            for(int j=0;j<nodePath.size();j++)
            {
               osg::Node* temp = nodePath[j];
               if(!temp->getName().compare(_parentname))
               {
                     _foundNodes.push_back(&node);
                     break;
               }
            }
         }

    }
     traverse (node);
}
