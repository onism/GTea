#ifndef TREEVIEWITEM_H
#define TREEVIEWITEM_H

#include <QTreeWidgetItem>
// OSG
#include <osg/ref_ptr>
#include <osg/Node>




class TreeViewItem : public QTreeWidgetItem
{
public:

                        TreeViewItem();
                        TreeViewItem(TreeViewItem *parent, int type = Type);
    virtual				~TreeViewItem() {}

    void				setOsgNode(osg::Node* node);
    osg::Node*			getOsgNode();
    const osg::Node*	getOsgNode() const;

    void				setOsgObject(osg::Object *obj);
    osg::Object*		getOsgObject();
    const osg::Object*	getOsgObject() const;


private:

    osg::ref_ptr<osg::Node> _node;
    osg::ref_ptr<osg::Object> _obj;
};

#endif // TREEVIEWITEM_H
