#ifndef FINDNODEVISTOR_H
#define FINDNODEVISTOR_H
#include <osg/NodeVisitor>
#include <osg/Node>
#include <string>
#include <vector>
class FindNodeVistor: public osg::NodeVisitor
{
public:
    FindNodeVistor(const std::string &name,const std::string &parent_name);

    virtual void apply(osg::Node &node);

    typedef std::vector<osg::ref_ptr<osg::Node>> NodeList;

    std::string _name;
    std::string _parentname;

    NodeList _foundNodes;
};

#endif // FINDNODEVISTOR_H
